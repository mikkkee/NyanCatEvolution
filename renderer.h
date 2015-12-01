#ifndef EVOART_RENDERER_H_
#define EVOART_RENDERER_H_

#include <string>
#include <vector>
#include <opencv\cv.hpp>
#include "DnaBrush.h"
#include "DnaCanvas.h"
#include "DnaPolygon.h"

// Functions that render/save canvas to image.
namespace renderer{

// Plots canvas onto graph.
// If scale_x and scale_y are present, it scales canvas onto graph.
void Render(const DnaCanvas& canvas, cv::Mat& plot);
void Render(const DnaCanvas& canvas, cv::Mat& plot,
	double scale_x, double scale_y);

void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot,
	double scale_x, double scale_y);

// Returns a Scalar object which defines the same color of polygon's brush.
cv::Scalar GetPolygonBrush(const DnaPolygon& polygon);

std::vector<cv::Point>* GetPolygonPointsVector(const DnaPolygon& polygon,
	double scale_x, double scale_y);

// Saves a canvas to image by using bm and graph. scale_x and scale_y are 
// scale factors.;
// Example:
//     SaveCanvasToImageAs(canvas, "happy.png", bm, graph, 2.0, 2.0);
void SaveCanvasToImageAs(const DnaCanvas& canvas, const std::string& filename,
	cv::Mat& plot, double scale_x, double scale_y);
}

#endif