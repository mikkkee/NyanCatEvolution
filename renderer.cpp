#include "renderer.h"
#include "DnaPoint.h"
#include "settings.h"

namespace renderer{

void Render(const DnaCanvas& canvas, cv::Mat& plot)
{
	plot.setTo(cv::Scalar(0, 0, 0));
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, 1.0, 1.0);
	};
}

void Render(const DnaCanvas& canvas, cv::Mat& plot,
	double scale_x, double scale_y)
{
	plot.setTo(cv::Scalar(0, 0, 0));
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, scale_x, scale_y);
	};
}

void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot,
	double scale_x, double scale_y)
{
	cv::Scalar brush = GetPolygonBrush(polygon);
	std::vector<cv::Point>* points_vec = GetPolygonPointsVector(polygon, scale_x, scale_y);
	cv::Point* points = &((*points_vec)[0]);
	int n = static_cast<int>(polygon.points->size());
	cv::fillConvexPoly(plot, points, n, brush, cv::LINE_AA);
	delete points_vec;  // Release vector allocated by new.
}

cv::Scalar GetPolygonBrush(
	const DnaPolygon& polygon)
{
	double alpha = static_cast<double>(polygon.brush->alpha) / 255;
	int red = polygon.brush->red * alpha;
	int green = polygon.brush->green * alpha;
	int blue = polygon.brush->blue * alpha;
	return cv::Scalar(blue, green, red, 255);
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
	const double scale_x, double scale_y)
{
	// Convert std::string to System::String.
	Render(canvas, plot, scale_x, scale_y);
	cv::imwrite(filename, plot);
}

}