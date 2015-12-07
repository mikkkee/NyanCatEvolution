#include "renderer.h"
#include "DnaPoint.h"
#include "settings.h"

namespace renderer{
// Renders image by using opaque polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot)
{
	plot.setTo(cv::Scalar(0, 0, 0, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, 1.0, 1.0);
	};
}
// Renders image by using opaque polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(0, 0, 0, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, scale_x, scale_y);
	};
}
// Renders image by using semi-transparent polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot)
{
	plot.setTo(cv::Scalar(0, 0, 0, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, subplot, 1.0, 1.0);
	};
}
// Renders image by using semi-transparent polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(0, 0, 0, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, subplot, scale_x, scale_y);
	};
}

// Renders opaque polygon.
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
// Renders semi-transparent polygon by alpha composition blending method.
void RenderPolygon(const DnaPolygon& polygon, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y)
{
	subplot.setTo(cv::Scalar(0, 0, 0, 0));
	cv::Scalar brush = GetPolygonBrush(polygon);
	std::vector<cv::Point>* points_vec = GetPolygonPointsVector(polygon, scale_x, scale_y);
	cv::Point* points = &((*points_vec)[0]);
	int n = static_cast<int>(polygon.points->size());
	cv::fillConvexPoly(subplot, points, n, brush, 16);  // Plot polygon.

	uchar* subplot_data = subplot.data;
	uchar* plot_data = plot.data;
	for (int y = 0; y < settings::MaxHeight; ++y) {
		for (int x = 0; x < settings::MaxWidth; ++x) {
			int subplot_blue = static_cast<int>(*subplot_data++);
			int subplot_green = static_cast<int>(*subplot_data++);
			int subplot_red = static_cast<int>(*subplot_data++);
			int subplot_alpha = static_cast<int>(*subplot_data++);

			int plot_blue = static_cast<int>(*plot_data++);
			int plot_green = static_cast<int>(*plot_data++);
			int plot_red = static_cast<int>(*plot_data++);
			int plot_alpha = static_cast<int>(*plot_data++);

			double alpha_fore = static_cast<double>(subplot_alpha) / 255.0;
			double alpha_back = static_cast<double>(plot_alpha) / 255.0;
			double alpha_multi = alpha_fore * alpha_back;
			double alpha_tot = alpha_fore + alpha_back - alpha_multi;
			double coeff_fore = alpha_fore / alpha_tot;
			double coeff_back = (alpha_back - alpha_multi) / alpha_tot;

			uchar r = static_cast<uchar>(coeff_fore * subplot_red + coeff_back * plot_red);
			uchar g = static_cast<uchar>(coeff_fore * subplot_green + coeff_back * plot_green);
			uchar b = static_cast<uchar>(coeff_fore * subplot_blue + coeff_back * plot_blue);
			uchar a = static_cast<uchar>(coeff_fore * subplot_alpha + coeff_back * plot_alpha);

			*(--plot_data) = a;
			*(--plot_data) = r;
			*(--plot_data) = g;
			*(--plot_data) = b;

			plot_data += 4;
		};
	};

	// cv::add(plot, subplot, plot);                       // Blend polygon with canvas.
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
	const const double scale_x, const double scale_y)
{
	// Convert std::string to System::String.
	Render(canvas, plot, scale_x, scale_y);
	cv::imwrite(filename, plot);
}
}
