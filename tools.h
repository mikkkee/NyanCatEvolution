#ifndef EVOART_TOOLS_H_
#define EVOART_TOOLS_H_

#include <string>
#include <opencv\cv.hpp>

class DnaPoint;
class DnaPolygon;
class DnaCanvas;

namespace tools {

// Starts a generic evolution towards target_name, a image file.
void StartEvolution(const std::string& target_name);

// Returns a pointer to target Mat's data array.
// Make sure the Mat's data is continous in memory, this function
// will fail otherwise.
const unsigned char* const PrepareTarget(cv::Mat& target);

// Dumps canvas to disk.
// bm's size should match scale_x * settings::MaxWidth, and
// scale_y * settings::MaxHeight. Otherwise, the saved image may be incomplete.
// graph should be created from bm.
void DumpCanvas(const DnaCanvas& canvas, const int selected,
	cv::Mat& plot, const double scale_x, const double scale_y);

// Sets random seed.
void RandInit();

// Returns a random number between (min, max),
// inclusive at min, not inclusive at max.
// However, if max == min, it always returns min.
int GetRandomNumber(const int min, const int max);

// Determins whether one should mutate with a given mutation_rate.
// Example:
//    WillMutate(500) returns true with probability of 0.002.
bool WillMutate(const int mutation_rate);

// Prints a point / polygon to console in human readable format.
// This two function Shall NOT be called directly. One should call
// PrintCanvas() to invoke these two functions.
void PrintPoint(const DnaPoint& point);
void PrintPolygon(const DnaPolygon& polygon);

// Prints a canvas to console in human readable format.
void PrintCanvas(const DnaCanvas& canvas);

// Prints evolution details into console in human readable format.
void PrintEvolution(const int generation, const int selected,
	const double score, const DnaCanvas& canvas);
}

#endif