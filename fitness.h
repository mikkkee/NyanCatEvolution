#ifndef EVOART_FITNESS_H_
#define EVOART_FITNESS_H_

#include <string>
#include <opencv\cv.hpp>
#include "DnaCanvas.h"

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
	Fitness(const std::string& target_name);
	double GetFitness(const DnaCanvas& canvas);
	int original_height;
	int original_width;
private:
	void ResizeTarget();
	void ResetTargetDataPtr();
	void ResetPlotDataPtr();
	cv::Mat plot;
	cv::Mat target;
	const unsigned char* target_data;
	const unsigned char* plot_data;
};

#endif