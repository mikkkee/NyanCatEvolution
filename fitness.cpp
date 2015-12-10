#include "fitness.h"
#include <cassert>
#include <iostream>
#include "renderer.h"
#include "settings.h"

// Initializes Fitness object with its plot & polygonPlot data member 
// to avoid creating/destroying Mat repeatly.
// plot and polygonPlot act as buffer holder when rendering canvas and 
// polygons in canvas.
Fitness::Fitness(const std::string& target_name) :
plot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
subplot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
target(cv::imread(target_name))
{
	original_height = target.rows;
	original_width = target.cols;
	ResizeTarget();
}

// Calculates fitness score between canvas and target.
// Uses a pointer to Mat.data to speedup scann in target Mat.
// Uses a pointer to BitmapData to speedup scan in rendered Bitmap image.
// bm is used to plot canvas on it.
// graph manages operations on bm.
double Fitness::GetFitness(const DnaCanvas& canvas ,bool opaque)
{
	if (opaque) {
		renderer::Render(canvas, plot, subplot);
	} else {
		renderer::Render(canvas, plot);
	};
	ResetTargetDataPtr();
	ResetPlotDataPtr();
	double score = 0;
	// Applies penalty for too much polygons.
	// Less polygons means less calculation.
	score += canvas.polygons->size();
	for (int y = 0; y < settings::MaxHeight; ++y) {
		for (int x = 0; x < settings::MaxWidth; ++x) {
			// Target is a unsigned char * pointed to the array
			// of three channel pixel values.
			int target_blue = static_cast<int>(*target_data++);
			int target_green = static_cast<int>(*target_data++);
			int target_red = static_cast<int>(*target_data++);
			int plot_blue = static_cast<int>(*plot_data++);
			int plot_green = static_cast<int>(*plot_data++);
			int plot_red = static_cast<int>(*plot_data++);

			int r = plot_red - target_red;
			int g = plot_green - target_green;
			int b = plot_blue - target_blue;
			score += abs(r) + abs(b) + abs(g);

			target_data++;
			plot_data++;
		};
	};
	return score;
}

void Fitness::ResizeTarget()
{
	assert(target.data);
	target.convertTo(target, settings::CanvasMatDataType);
	// Resize target image to:
	// 1. Reduce calculations if image is too large.
	// 2. Fixed canvas range makes it easier when generating random points.
	cv::resize(target, target, cv::Size(settings::MaxHeight, settings::MaxWidth));
}

void Fitness::ResetTargetDataPtr()
{
	// Get a pointer to pixel arrays to target Mat's transposition.
	// The Mat should be continuous to allow one pointer to scan whole data.
	assert(target.isContinuous());
	target_data = target.data;
}

void Fitness::ResetPlotDataPtr()
{
	// Get a pointer to pixel arrays to target Mat's transposition.
	// The Mat should be continuous to allow one pointer to scan whole data.
	assert(plot.isContinuous());
	plot_data = plot.data;
}