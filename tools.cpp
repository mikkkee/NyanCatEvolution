#include "tools.h"
#include <ctime>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <sstream>
#include <utility>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "DnaCanvas.h"
#include "DnaPoint.h"
#include "DnaPolygon.h"
#include "fitness.h"
#include "renderer.h"
#include "settings.h"

namespace tools {

namespace {
// Mersenne twister engine shared by all random draws.
// rand() % n is both low quality and biased (on MSVC RAND_MAX is only 32767,
// making the bias severe for the mutation-rate bases used here).
std::mt19937 rng;
}

SaveTool::SaveTool() :
plot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
subplot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType))
{}

// Resize the size of plot and subplot; Render canvas using them; save plot to image.
void SaveTool::SaveCanvasAsImage(const std::string& filename, const DnaCanvas& canvas,
	const double scale_x, const double scale_y, bool opaque)
{
	cv::Size size(settings::MaxWidth * scale_x, settings::MaxHeight * scale_y);
	cv::resize(plot, plot, size);
	cv::resize(subplot, subplot, size);
	// Saved images are for viewing, so render them anti-aliased; fitness
	// rendering keeps cv::LINE_8 for speed and a noise-free selection signal.
	if (opaque) {
		renderer::Render(canvas, plot, scale_x, scale_y, cv::LINE_AA);
	}
	else {
		renderer::Render(canvas, plot, subplot, scale_x, scale_y, cv::LINE_AA);
	};
	if (!cv::imwrite(filename, plot)) {
		std::cerr << "Failed to save snapshot to " << filename << std::endl;
	};
}

// Starts an evolution run.
// 1. Sets up fitness object and saving size.
// 2. Initializes evolution statistic parameters.
// 3. Starts genetic evolution and log / dump.
void StartEvolution(const std::string& target_name)
{
	using namespace std;
	using namespace cv;

	RandInit();  // Sets random seed.
	// Snapshots are dumped into DumpPrefix; make sure it exists, since
	// cv::imwrite does not create parent directories.
	if (!settings::DumpPrefix.empty()) {
		std::filesystem::create_directories(settings::DumpPrefix);
	};
	Fitness fitness(target_name);
	const double scale_x = static_cast<double>(fitness.original_width) / settings::MaxWidth;
	const double scale_y = static_cast<double>(fitness.original_height) / settings::MaxHeight;
	// Use opaque polygons if and only if alpha is fixed to 255 in settings.h.
	bool opaque = (settings::BrushAlphaMutationHigh == settings::BrushAlphaMutationLow) &&
		(settings::BrushAlphaMutationHigh == 255);
	tools::SaveTool save_tool;

	DnaCanvas parent;
	long long population = 0;    // Count of mutated DNAs.
	long long selected = 0;      // Count of "good" mutated DNAs.
	double current_score = fitness.GetFitness(parent, opaque);
	double next_score = 0.0;

	// Keeps evoluting until reaching the convergence condition or the
	// generation cap, whichever comes first.
	while (current_score > settings::FitConvergence &&
		population < settings::MaxGenerations) {
		DnaCanvas offspring(parent);
		offspring.ClearDirty();
		offspring.Mutate();
		if (offspring.is_dirty) {
			population++;
			next_score = fitness.GetFitness(offspring, opaque);
			if (next_score <= current_score) {
				selected++;
				parent = std::move(offspring);
				current_score = next_score;
				// Dumps "good" mutated DNAs.
				if (selected % settings::DumpFrequency == 0) {
					ostringstream name;
					name << settings::DumpPrefix << selected << settings::DumpExtension;
					save_tool.SaveCanvasAsImage(name.str(), parent, scale_x, scale_y, opaque);
				};
			};
			if (population % settings::ConsoleLogFrequency == 0) {
				tools::PrintEvolution(population, selected, current_score, parent);
			};
			if (settings::FileLog && population % settings::FileLogFrequency == 0) {
				tools::WriteLog(population, selected, current_score);
			};
		};
	};

	// Saves the final state and reports why the run stopped.
	tools::PrintEvolution(population, selected, current_score, parent);
	std::cout << std::endl;
	if (current_score <= settings::FitConvergence) {
		std::cout << "Converged: score " << current_score
			<< " <= " << settings::FitConvergence << std::endl;
	} else {
		std::cout << "Stopped: reached MaxGenerations ("
			<< settings::MaxGenerations << ")" << std::endl;
	};
	std::string final_name = settings::DumpPrefix + "final" + settings::DumpExtension;
	save_tool.SaveCanvasAsImage(final_name, parent, scale_x, scale_y, opaque);
}

void RandInit() { rng.seed(std::random_device{}()); };

int GetRandomNumber(const int min, const int max)
{
	assert(max >= min);
	if (max == min) return min;
	return std::uniform_int_distribution<int>(min, max - 1)(rng);
}

void PrintPoint(const DnaPoint& point)
{
	std::cout << "(" << point.x << ", " << point.y << ") ";
}

void PrintPolygon(const DnaPolygon& polygon)
{
	std::cout << "    Polygon("
		<< polygon.points.size()
		<< "):\n        ";
	for (auto& point : polygon.points) {
		PrintPoint(point);
	};
	std::cout << std::endl;
}

void PrintCanvas(const DnaCanvas& canvas)
{
	std::cout << "Canvas:("
		<< canvas.polygons.size() << "):\n";
	for (auto& polygon : canvas.polygons) {
		PrintPolygon(polygon);
	};
}

void PrintEvolution(const long long generation, const long long selected,
	const double score,
	const DnaCanvas& canvas)
{
	std::cout << std::fixed;
	std::cout << "\rP: " << generation << " | "
		<< "S: " << selected << " | "
		<< "Nplgns: " << std::setw(3) << canvas.polygons.size() << " | "
		<< "Npts: " << std::setw(1) << canvas.PointCount() << " | "
		<< "Score: " << std::setw(8) << score << std::flush;
}

// Write log to a file named evolution.log.
void WriteLog(const long long population, const long long selected, const double score)
{
	std::ofstream log_file;
	log_file.open("evolution.log", std::ios::app);
	std::ostringstream log_line;
	time_t now = time(0);
	log_line << now << " " << population << " " << selected << " " << score << "\n";
	log_file << log_line.str();
	log_file.close();
}
}
