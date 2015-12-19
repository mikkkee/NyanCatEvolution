#ifndef EVOART_TOOLS_H_
#define EVOART_TOOLS_H_

#include <string>
#include <opencv/cv.hpp>
#include <opencv2/highgui/highgui.hpp>

class DnaPoint;
class DnaPolygon;
class DnaCanvas;

namespace tools {

// Tool to save canvas into image.
class SaveTool
{
public:
	SaveTool();
	void SaveCanvasAsImage(const std::string& filename, const DnaCanvas& canvas,
		const double scale_x, const double scale_y, bool opaque);
private:
	cv::Mat plot;     // A plot that holds blended canvas.
	cv::Mat subplot;  // A plot that holds each polygon before blended to canvas plot.
};

// Starts a generic evolution towards target_name, an image file.
void StartEvolution(const std::string& target_name);

// Sets random seed.
void RandInit();

// Returns a random number between (min, max),
// inclusive at min, not inclusive at max.
// However, if max == min, it always returns min.
int GetRandomNumber(const int min, const int max);

// Prints a DnaPoint object as "(x, y)".
// Do NOT call this function directly. Instead, one should call PrintCanvas()
// to invoke this function.
void PrintPoint(const DnaPoint& point);

// Prints a DnaPolygon object as:
//     Polygon(3):
//         point1, point2, point3
// Leading spaces are used for formating when called in PrintCanvas.
// Do NOT call this function directly. Instead, one should call PrintCanvas()
// to invoke this function.
void PrintPolygon(const DnaPolygon& polygon);

// Prints a canvas to console in human readable format:
// Canvas(# of polygons):
//     polygon1
//     polygon2...
void PrintCanvas(const DnaCanvas& canvas);

// Prints evolution details into console in human readable format:
// P: # of mutations | S: # of "good" mutations | Nplgns: # of polygons used
// | Npts: # of points on canvas | Score: current fittness score
void PrintEvolution(const int generation, const int selected,
	const double score, const DnaCanvas& canvas);

// Used to log number of mutations / good mucations / fitness score vs time.
void WriteLog(const int population, const int selected, const double score);
}
#endif