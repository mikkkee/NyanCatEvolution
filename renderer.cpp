#include "renderer.h"
#include <stdexcept>
#include "DnaPoint.h"
#include "settings.h"

namespace renderer{
// Renders image by using opaque polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot)
{
	Render(canvas, plot, 1.0, 1.0);  // Calls opaque version with scale factor 1.0.
}

// Renders image by using opaque polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(0, 0, 0));  // Reset the image color.
	for (auto& polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, scale_x, scale_y);
	};
}

// Renders image by using semi-transparent polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot)
{
	Render(canvas, plot, subplot, 1.0, 1.0); // Calls non-opaque version with scale factor 1.0.
}

// Renders image by using semi-transparent polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(0, 0, 0));  // Reset the image color.
	for (auto& polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, subplot, scale_x, scale_y);
	};
}

// Renders a opaque polygon to plot.
void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot,
	const double scale_x, const double scale_y)
{
	cv::Scalar brush = GetPolygonBrush(polygon);
	std::vector<cv::Point>* points_vec = GetPolygonPointsVector(polygon, scale_x, scale_y);
	cv::Point* points = &((*points_vec)[0]);
	int n = static_cast<int>(polygon.points->size());
	cv::fillConvexPoly(plot, points, n, brush, 16);
	delete points_vec;  // Releases vector allocated by new.
}

// Renders a semi-transparent, i.e. non-opaque, polygon to plot.
// Firstly the polygon is rendered on subplot, then subplot is blended with plot by
// using alpha composition method: composite subplot over plot.
// Todo: implement high efficiency alpha composition strategy.
void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y)
{
	throw std::runtime_error("Non-opaque rendering method does not work currently!");
	subplot.setTo(cv::Scalar(0, 0, 0));
	cv::Scalar brush = GetPolygonBrush(polygon);
	std::vector<cv::Point>* points_vec = GetPolygonPointsVector(polygon, scale_x, scale_y);
	cv::Point* points = &((*points_vec)[0]);
	int n = static_cast<int>(polygon.points->size());
	cv::fillConvexPoly(subplot, points, n, brush, 16);  // Plot polygon.
	cv::add(plot, subplot, plot);                       // Blend polygon with canvas.
	delete points_vec;                                  // Releases vector allocated by new.
}

cv::Scalar GetPolygonBrush(
	const DnaPolygon& polygon)
{
	int alpha = polygon.brush->alpha;
	int red = polygon.brush->red;
	int green = polygon.brush->green;
	int blue = polygon.brush->blue;
	return cv::Scalar(blue, green, red, alpha);
}

std::vector<cv::Point>* GetPolygonPointsVector(
	const DnaPolygon& polygon,
	const double scaleX, const double scaleY)
{
	std::vector<cv::Point>* point_vec = new std::vector<cv::Point>;
	for (auto& point : *polygon.points) {
		int x = static_cast<int>(point.x * scaleX);
		int y = static_cast<int>(point.y * scaleY);
		point_vec->push_back(cv::Point(x, y));
	};
	return point_vec;
}

void SaveCanvasToImageAs(
	const DnaCanvas& canvas, const std::string& filename, cv::Mat& plot,
	const double scale_x, const double scale_y)
{
	Render(canvas, plot, scale_x, scale_y);
	cv::imwrite(filename, plot);
}
}
