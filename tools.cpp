#include "tools.h"
#include <ctime>
#include <cassert>
#include <iostream>
#include <opencv\highgui.h>
#include "DnaCanvas.h"
#include "DnaPoint.h"
#include "DnaPolygon.h"
#include "fitness.h"
#include "renderer.h"
#include "settings.h"


namespace tools {

void StartEvolution(const std::string& targetName)
{
	using namespace std;
	using namespace cv;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;

	RandInit();  // Sets random seed.
	cv::Mat target = cv::imread(targetName);
	// Saves target size here to set up saving canvas.
	int height = target.rows, width = target.cols;
	double scale_x = static_cast<double>(width) / settings::MaxWidth;
	double scale_y = static_cast<double>(height) / settings::MaxHeight;
	// Get pointer to target data to scan target pixel by pixel.
	// 200*200 pixels are compared in each calculation of fitness,
	// performance really matters here.
	const unsigned char* const target_ptr = PrepareTarget(target);
	
	// Sets up drawing bitmap and graphics for fitness calculation.
	Bitmap^ mockup_bm = gcnew Bitmap(settings::MaxWidth, settings::MaxHeight,
		PixelFormat::Format32bppArgb);
	Graphics^ mockup_graph = Graphics::FromImage(mockup_bm);
	mockup_graph->SmoothingMode = Drawing2D::SmoothingMode::HighQuality;
	// Sets up drawing bitmap and graphics for saving.
	Bitmap^ save_bm = gcnew Bitmap(width, height, PixelFormat::Format32bppArgb);
	Graphics^ save_graph = Graphics::FromImage(save_bm);
	save_graph->SmoothingMode = Drawing2D::SmoothingMode::HighQuality;

	DnaCanvas parent;
	int population = 0;    // Count of mutated DNAs.
	int selected = 0;      // Count of "good" mutated DNAs.
	bool written = false;  // Whether parent has been written to disk.

	double current_score = Fitness::GetFitness(parent, target_ptr,
		mockup_bm, mockup_graph);
	double next_score = 0.0;

	unsigned char** spatial = new unsigned char*[settings::MaxWidth];
	for (int i = 0; i < settings::MaxWidth; ++i) {
		spatial[i] = new unsigned char[settings::MaxHeight]; 
	};

	// Keeps evoluting until reaches convergence condition.
	while (current_score > settings::FitConvergence) {
		DnaCanvas offspring(parent);
		offspring.Mutate();
		if (offspring.is_dirty) {
			population++;
			next_score = Fitness::GetFitness(offspring, target_ptr,
				mockup_bm, mockup_graph);

			if (next_score <= current_score) {
				selected++;
				written = false;
				parent = offspring;
				current_score = next_score;
			};
			if (population % settings::LogFrequency == 0) {
				tools::PrintEvolution(population, selected, current_score, parent);
			};
		};
		// Dumps "gooded" mutated images.
		if ((selected % settings::DumpFrequency == 0) && !written) {
			DumpCanvas(parent, selected, save_bm, save_graph, scale_x, scale_y);
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
	System::Drawing::Bitmap^ bm, System::Drawing::Graphics^ graph,
	const double scale_x, const double scale_y)
{
	std::ostringstream img_name;
	img_name << settings::DumpPrefix << selected << settings::DumpExtension;
	renderer::SaveCanvasToImageAs(canvas, img_name.str(),
		bm, graph, scale_x, scale_y);
}

void RandInit() { srand(time(NULL)); };

int GetRandomNumber(const int min, const int max)
{
	assert(max >= min);
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
	std::cout << "\rP: " << generation << " | "
		<< "S: " << selected << " | "
		<< "Nplgns: " << canvas.polygons->size() << " | "
		<< "Npts: " << canvas.PointCount() << " | "
		<< "Score: " << score;
}

int ToolsWin::GetRandomNumber(int min, int max)
{
	return random_->Next(min, max);
}

bool ToolsWin::WillMutate(int mutationRate)
{
	if (GetRandomNumber(0, mutationRate) == 1)
		return true;
	return false;
}
}