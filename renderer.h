#ifndef EVOART_RENDERER_H_
#define EVOART_RENDERER_H_

#include <string>
#include <vector>
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "DnaBrush.h"
#include "DnaCanvas.h"
#include "DnaPolygon.h"

// Contains functions that render/save canvas to image.
namespace renderer{

// Plots canvas onto an image.
// Renders by using opaque polygons if only one Mat plot is provided.
// Renders by using semi-transparent polygon is two Mat plots are provided.
// If scale_x and scale_y are present, this function scales points coordinations
// by factor scale_x and scale_y before ploting canvas onto graph.
void Render(const DnaCanvas& canvas, cv::Mat& plot);
void Render(const DnaCanvas& canvas, cv::Mat& plot,
	const double scale_x, const double scale_y);
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot);
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y);
// Plots a DnaPolygon object on to an image.
// Renders using opaque or semi-transparent polygons specified by presence of subplot.
// scale_x and scale_y are used to scale the polygon before ploting.
void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot,
	const double scale_x, const double scale_y);
void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y);

// Returns a Scalar object which defines the same color of polygon's brush.
cv::Scalar GetPolygonBrush(const DnaPolygon& polygon);
// Returns a vector that contains all points of a DnaPolygon in cv::Point format.
std::vector<cv::Point>* GetPolygonPointsVector(const DnaPolygon& polygon,
	const double scale_x, const double scale_y);
// Saves a canvas to image of given name. scale_x and scale_y are scale factors.
// Example:
//     SaveCanvasToImageAs(canvas, "happy.png", bm, graph, 2.0, 2.0);
void SaveCanvasToImageAs(const DnaCanvas& canvas, const std::string& filename,
	cv::Mat& plot, const double scale_x, const double scale_y);
}

#endif
