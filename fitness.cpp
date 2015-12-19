#include "fitness.h"
#include <cassert>
#include <iostream>
#include "renderer.h"
#include "settings.h"

// By using plot & subplot, we are able to avoid 
// creating/destroying Mat repeatly.
// plot and subplot act as buffer holders when rendering canvas.
Fitness::Fitness(const std::string& target_name) :
original_height(),
original_width(),
plot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
subplot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
target(cv::imread(target_name)),
target_data(nullptr),
plot_data(nullptr)
{
	original_height = target.rows;
	original_width = target.cols;
	ResizeTarget();
}

// Calculates fitness score between canvas and target.
// Uses pointers to Mat.data to speedup scann over all pixels.
// opaque determines whether the opaque rendering method should be used.
// non-opaque rendering method is not supported currently at 19 Dec 2015.
double Fitness::GetFitness(const DnaCanvas& canvas ,bool opaque)
{
	if (opaque) {
		renderer::Render(canvas, plot);
	} else {
		renderer::Render(canvas, plot, subplot);
	};
	ResetTargetDataPtr();
	ResetPlotDataPtr();
	double score = 0;
	// Applies penalty for too much polygons.
	// Less polygons means less time on rendering.
	score += canvas.polygons->size();
	for (int y = 0; y < settings::MaxHeight; ++y) {
		for (int x = 0; x < settings::MaxWidth; ++x) {
			// Target is a unsigned char * pointed to the array
			// of three channel pixel values.
			int target_blue = *target_data++;
			int target_green = *target_data++;
			int target_red = *target_data++;
			int plot_blue = *plot_data++;
			int plot_green = *plot_data++;
			int plot_red = *plot_data++;

			int r = plot_red - target_red;
			int g = plot_green - target_green;
			int b = plot_blue - target_blue;
			score += abs(r) + abs(b) + abs(g);
		};
	};
	return score;
}

// Converts target mat's datatype and resizes it to specified size.
void Fitness::ResizeTarget()
{
	assert(target.data);
	target.convertTo(target, settings::CanvasMatDataType);
	// Benefit of resizing:
	// 1. Reduce calculations if image is too large.
	// 2. Fixed canvas range makes it easier when generating random points.
	cv::resize(target, target, cv::Size(settings::MaxHeight, settings::MaxWidth));
}

void Fitness::ResetTargetDataPtr()
{
	// Get a pointer to pixel arrays of target mat.
	// The data should be continuous to allow pointer arithmethic when scanning.
	assert(target.isContinuous());
	target_data = target.data;
}

void Fitness::ResetPlotDataPtr()
{
	// Get a pointer to pixel arrays of plot mat.
	// The data should be continuous to allow pointer arithmethic when scanning.
	assert(plot.isContinuous());
	plot_data = plot.data;
}