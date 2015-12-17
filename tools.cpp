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
	bool opaque = (settings::BrushAlphaMutationHigh == settings::BrushAlphaMutationLow) &&
		(settings::BrushAlphaMutationHigh == 255);  // Use opaque polygons if alpha is fixed to 255.
	tools::SaveTool save_tool;

	DnaCanvas parent;
	int population = 0;    // Count of mutated DNAs.
	int selected = 0;      // Count of "good" mutated DNAs.
	bool written = false;  // Whether parent has been written to disk.
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
				written = false;
				parent = offspring;
				current_score = next_score;
			};
			if (population % settings::ConsoleLogFrequency == 0) {
				tools::PrintEvolution(population, selected, current_score, parent);
			};
			if (population % settings::FileLogFrequency == 0) {
				tools::WriteLog(population, selected, current_score);
			};
		};
		// Dumps "gooded" mutated images.
		if ((selected % settings::DumpFrequency == 0) && !written) {
			ostringstream name;
			name << settings::DumpPrefix << selected << settings::DumpExtension;
			save_tool.SaveCanvasAsImage(name.str(), parent, scale_x, scale_y, opaque);
			written = true;
		};
	};
}

// Prepare the Mat object loaded from target image for fitness calculation:
// 1. Validates it data to ensure a successful load.
// 2. Converts its DataType to what Settings specifies.
// 3. Resizes it to speed up fitness calculation.
// 4. Returns a pointer to the beginning of its data for fast scan accross all pixels.
//
// The return pointer points to the beginning of target Mat's data array.
//
// This function can fail if !target.isContinuous().
const unsigned char* const PrepareTarget(cv::Mat& target)
{
	assert(target.data);
	target.convertTo(target, settings::CanvasMatDataType);
	// Resize target image to:
	// 1. Reduce calculations if image is too large.
	// 2. Fixed canvas range makes it easier when generating random points.
	cv::resize(target, target, cv::Size(settings::MaxHeight, settings::MaxWidth));

	// Get a pointer to pixel arrays to target Mat's transposition.
	// The Mat should be continuous to allow one pointer to scan whole data.
	assert(target.isContinuous());
	const unsigned char* const target_ptr = target.data;
	return target_ptr;
}

void DumpCanvas(const DnaCanvas& canvas, const int selected,
	cv::Mat& plot, const double scale_x, const double scale_y)
{
	std::ostringstream img_name;
	img_name << settings::DumpPrefix << selected << settings::DumpExtension;
	renderer::SaveCanvasToImageAs(canvas, img_name.str(),
		plot, scale_x, scale_y);
}

void RandInit() { srand(time(NULL)); };

int GetRandomNumber(const int min, const int max)
{
	assert(max >= min);
	if (max == min) return min;
	return (rand() % (max - min)) + min;
}

bool WillMutate(const int mutation_rate)
{
	// A mutation rate of 1 should always be true.
	if (mutation_rate == 1) return true;
	if (GetRandomNumber(0, mutation_rate) == 1) return true;
	return false;
}

void PrintPoint(const DnaPoint& point)
{
	std::cout << "(" << point.x << ", " << point.y << ") ";
}

void PrintPolygon(const DnaPolygon& polygon)
{
	std::cout << "    Polygons("
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
