#ifndef EVOART_FITNESS_H_
#define EVOART_FITNESS_H_

#include <string>
#include <opencv\cv.hpp>
#include "DnaCanvas.h"
#using <system.drawing.dll>

class Pixel
{
public:
	Pixel(int red, int green, int blue);
	int R;
	int G;
	int B;
};

class Fitness
{
public:
	Fitness();
	static double GetFitness(
		const DnaCanvas& canvas, const unsigned char* target,
		System::Drawing::Bitmap^ bmp, System::Drawing::Graphics^ graph);
	static Pixel GetPixel(System::Drawing::Imaging::BitmapData^ bmd,
		int x, int y);
private:
	cv::Mat plot;
	cv::Mat polygonPlot;
};

#endif