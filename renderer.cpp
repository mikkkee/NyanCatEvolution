#include "renderer.h"
#include "DnaPoint.h"
#include "settings.h"

namespace renderer{

void Render(const DnaCanvas& canvas, System::Drawing::Graphics^ graph)
{
	graph->Clear(System::Drawing::Color::Black);
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, graph, 1.0, 1.0);
	};
}

void Render(const DnaCanvas& canvas, System::Drawing::Graphics^ graph,
	double scale_x, double scale_y)
{
	graph->Clear(System::Drawing::Color::Black);
	for (auto & polygon : *canvas.polygons) {
		RenderPolygon(polygon, graph, scale_x, scale_y);
	};
}

void RenderPolygon(const DnaPolygon& polygon, System::Drawing::Graphics^ graph,
	double scale_x, double scale_y)
{
	System::Drawing::SolidBrush^ brush = GetPolygonBrush(polygon);
	array<System::Drawing::PointF>^ points = 
		GetPolygonPoints(polygon, scale_x, scale_y);
	graph->FillPolygon(brush, points);
}

System::Drawing::SolidBrush^ GetPolygonBrush(
	const DnaPolygon& polygon)
{
	int alpha = polygon.brush->alpha;
	int red = polygon.brush->red;
	int green = polygon.brush->green;
	int blue = polygon.brush->blue;
	return gcnew System::Drawing::SolidBrush(
		System::Drawing::Color::FromArgb(alpha, red, green, blue));
}

array<System::Drawing::PointF>^ GetPolygonPoints(
	const DnaPolygon& polygon,
	const double scaleX, const double scaleY)
{
	array<System::Drawing::PointF>^ pointList = gcnew 
		array<System::Drawing::PointF>(polygon.points->size());
	int i = 0;
	for (auto& point : *polygon.points) {
		int x = static_cast<int>(point.x * scaleX);
		int y = static_cast<int>(point.y * scaleY);
		pointList[i++] = System::Drawing::PointF(x, y);
	};
	return pointList;
}

void SaveCanvasToImageAs(
	const DnaCanvas& canvas, const std::string& filename,
	System::Drawing::Bitmap^ bm, System::Drawing::Graphics^ graph,
	const double scale_x, double scale_y)
{
	// Convert std::string to System::String.
	System::String^ img_name = gcnew System::String(filename.c_str());
	Render(canvas, graph, scale_x, scale_y);
	bm->Save(img_name);
}

}