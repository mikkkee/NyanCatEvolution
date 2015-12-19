#include "tools.h"
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "DnaCanvas.h"
#include "DnaPoint.h"
#include "DnaPolygon.h"
#include "fitness.h"
#include "renderer.h"
#include "settings.h"

namespace tools {

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
	if (opaque) {
		renderer::Render(canvas, plot, scale_x, scale_y);
	}
	else {
		renderer::Render(canvas, plot, subplot, scale_x, scale_y);
	};
	cv::imwrite(filename, plot);
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
	Fitness fitness(target_name);
	const double scale_x = static_cast<double>(fitness.original_width) / settings::MaxWidth;
	const double scale_y = static_cast<double>(fitness.original_height) / settings::MaxHeight;
	// Use opaque polygons if and only if alpha is fixed to 255 in settings.h.
	bool opaque = (settings::BrushAlphaMutationHigh == settings::BrushAlphaMutationLow) &&
		(settings::BrushAlphaMutationHigh == 255);
	tools::SaveTool save_tool;

	DnaCanvas parent;
	int population = 0;    // Count of mutated DNAs.
	int selected = 0;      // Count of "good" mutated DNAs.
	double current_score = fitness.GetFitness(parent, opaque);
	double next_score = 0.0;

	// Keeps evoluting until reaches convergence condition.
	while (current_score > settings::FitConvergence) {
		DnaCanvas offspring(parent);
		offspring.Mutate();
		if (offspring.is_dirty) {
			population++;
			next_score = fitness.GetFitness(offspring, opaque);
			if (next_score <= current_score) {
				selected++;
				parent = offspring;
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
}

void RandInit() { srand(time(NULL)); };

int GetRandomNumber(const int min, const int max)
{
	assert(max >= min);
	if (max == min) return min;
	return (rand() % (max - min)) + min;
}

void PrintPoint(const DnaPoint& point)
{
	std::cout << "(" << point.x << ", " << point.y << ") ";
}

void PrintPolygon(const DnaPolygon& polygon)
{
	std::cout << "    Polygon("
		<< polygon.points->size()
		<< "):\n        ";
	for (auto& point : *(polygon.points)) {
		PrintPoint(point);
	};
	std::cout << std::endl;
}

void PrintCanvas(const DnaCanvas& canvas)
{
	std::cout << "Canvas:("
		<< canvas.polygons->size() << "):\n";
	for (auto& polygon : *(canvas.polygons)) {
		PrintPolygon(polygon);
	};
}

void PrintEvolution(const int generation, const int selected,
	const double score,
	const DnaCanvas& canvas)
{
	std::cout << std::fixed;
	std::cout << "\rP: " << generation << " | "
		<< "S: " << selected << " | "
		<< "Nplgns: " << std::setw(3) << canvas.polygons->size() << " | "
		<< "Npts: " << std::setw(1) << canvas.PointCount() << " | "
		<< "Score: " << std::setw(8) << score << std::flush;
}

// Write log to a file named evolution.log. 
void WriteLog(const int population, const int selected, const double score)
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
