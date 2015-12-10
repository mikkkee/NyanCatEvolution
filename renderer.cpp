#include "renderer.h"
#include "DnaPoint.h"
#include "settings.h"

namespace renderer{
// Renders image by using opaque polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot)
{
	plot.setTo(cv::Scalar(255, 255, 255, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, 1.0, 1.0);
	};
}
// Renders image by using opaque polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(255, 255, 255, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, scale_x, scale_y);
	};
}
// Renders image by using semi-transparent polygons.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot)
{
	plot.setTo(cv::Scalar(255, 255, 255, 255));  // Reset the image color.
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, plot, subplot, 1.0, 1.0);
	};
}
// Renders image by using semi-transparent polygons with scale factors.
void Render(const DnaCanvas& canvas, cv::Mat& plot, cv::Mat& subplot,
	const double scale_x, const double scale_y)
{
	plot.setTo(cv::Scalar(255, 255, 255, 255));  // Reset the image color.
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
	alpha_composite(subplot, plot, plot);
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

// Blends image top over image bot and save result into image out.
// The method used for blending is alpha compositing.
// To save time, the dimensions of input images are not checked,
// make sure the three input images have exactly the same size while
// using this function!
void alpha_composite(cv::Mat& top, cv::Mat& bot, cv::Mat& out)
{
	uchar* bot_data = bot.data;
	uchar* top_data = top.data;
	uchar* out_data = out.data;
	int rows = top.rows;
	int cols = top.cols;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			int blue_a = static_cast<int>(*top_data++);
			int green_a = static_cast<int>(*top_data++);
			int red_a = static_cast<int>(*top_data++);
			int alpha_a = static_cast<int>(*top_data++);

			int blue_b = static_cast<int>(*bot_data++);
			int green_b = static_cast<int>(*bot_data++);
			int red_b = static_cast<int>(*bot_data++);
			int alpha_b = static_cast<int>(*bot_data++);

			double aa = alpha_a / 255.0;
			double ab = alpha_b / 255.0;
			double ca = aa / (aa + ab - aa * ab);
			double cb = (ab - aa * ab) / (aa + ab - aa * ab);

			*out_data++ = blue_a * ca + blue_b * cb;
			*out_data++ = green_a * ca + green_b * cb;
			*out_data++ = red_a * ca + red_b * cb;
			*out_data++ = alpha_a * ca + alpha_b * cb;
		};
	};
}
}
