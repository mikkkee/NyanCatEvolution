#include "fitness.h"
#include <iostream>
#include "renderer.h"
#include "settings.h"

Pixel::Pixel(int red, int green, int blue): R(red), G(green), B(blue) {}

// Initializes Fitness object with its plot & polygonPlot data member 
// to avoid creating/destroying Mat repeatly.
// plot and polygonPlot act as buffer holder when rendering canvas and 
// polygons in canvas.
Fitness::Fitness():
plot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType)),
polygonPlot(cv::Mat(settings::MaxHeight, settings::MaxWidth, settings::CanvasMatDataType))
{
}

// Calculates fitness score between canvas and target.
// Uses a pointer to Mat.data to speedup scann in target Mat.
// Uses a pointer to BitmapData to speedup scan in rendered Bitmap image.
// bm is used to plot canvas on it.
// graph manages operations on bm.
double Fitness::GetFitness(
	const DnaCanvas& canvas, const unsigned char* target,
	System::Drawing::Bitmap^ bm, System::Drawing::Graphics^ graph)
{
	renderer::Render(canvas, graph);

	// Locks bm in memory to access its data.
	System::Drawing::Imaging::BitmapData^ bmd = bm->LockBits(
		System::Drawing::Rectangle(0, 0, settings::MaxWidth, settings::MaxHeight),
		System::Drawing::Imaging::ImageLockMode::ReadOnly,
		System::Drawing::Imaging::PixelFormat::Format32bppArgb
		);

	double score = 0;
	// Applies penalty for too much polygons.
	// Less polygons means less calculation.
	score += canvas.polygons->size();
	for (int y = 0; y < settings::MaxHeight; ++y) {
		for (int x = 0; x < settings::MaxWidth; ++x) {
			// Target is a unsigned char * pointed to the array
			// of three channel pixel values.
			int targetBlue = static_cast<int>(*target++);
			int targetGreen = static_cast<int>(*target++);
			int targetRed = static_cast<int>(*target++);

			Pixel p = GetPixel(bmd, x, y);
			int r = p.R - targetRed;
			int g = p.G - targetGreen;
			int b = p.B - targetBlue;

			score += abs(r) + abs(b) + abs(g);
		};
	};
	bm->UnlockBits(bmd);
	return score;
}

// Returns pixel value of a BitmapData at position (x, y) by using pointer
// to the beginning of the data:
// p = ptr + row_length * y + pixel_bit_size * x;
Pixel Fitness::GetPixel(System::Drawing::Imaging::BitmapData^ bmd,
	int x, int y)
{
	System::Byte* p = (System::Byte *)bmd->Scan0.ToPointer() + y * bmd->Stride + 4 * x;
	Pixel px(p[2], p[1], p[0]);
	return px;
}