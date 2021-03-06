#ifndef EVOART_FITNESS_H_
#define EVOART_FITNESS_H_

#include <string>
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "DnaCanvas.h"

class Fitness
{
public:
	Fitness(const std::string& target_name);
	double GetFitness(const DnaCanvas& canvas, bool opaque);
	int original_height;
	int original_width;
private:
	void ResizeTarget();
	void ResetTargetDataPtr();
	void ResetPlotDataPtr();
	cv::Mat plot;
	cv::Mat subplot;
	cv::Mat target;
	const unsigned char* target_data;
	const unsigned char* plot_data;
};
#endif