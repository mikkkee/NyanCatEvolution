#ifndef EVOART_DNA_SETTINGS_H_
#define EVOART_DNA_SETTINGS_H_

#include <string>
#include <opencv/cv.hpp>

namespace settings
{
// Evolution settings.
const int FitConvergence = 1000;
const int DumpFrequency = 1000;
const int LogFrequency = 50;
const std::string DumpPrefix = "selected\\";
const std::string DumpExtension = ".png";


// Rendering settings.
const auto CanvasMatDataType = CV_8UC3;
const int MaxWidth = 200;
const int MaxHeight = 200;

// Mutation rates, a muration rate N means this mutation
// happens with a probability of 1 / N.
const int PointMaxMutationRate = 2000;  // Random point in canvas.
const int PointMidMutationRate = 600;  // Move around original position.
const int PointMinMutationRate = 600;  // Move around original position slightly.
const int BrushBlueMutationRate = 300;
const int BrushGreenMutationRate = 300;
const int BrushRedMutationRate = 300;
const int BrushAlphaMutationRate = 300;
const int PolygonAddPointRate = 300;
const int PolygonRemovePointRate = 300;
const int CanvasAddPolygonRate = 20;
const int CanvasRemovePolygonRate = 100;
const int CanvasMovePolygonRate = 100;

// Mutation settings.
const int ColorMutationRange = 5;
const int PointMidMutationRange = 5;
const int PointMinMutationRange = 5;
const int BrushBlueMutationLow = 0;
const int BrushBlueMutationHigh = 255;
const int BrushGreenMutationLow = 0;
const int BrushGreenMutationHigh = 255;
const int BrushRedMutationLow = 0;
const int BrushRedMutationHigh = 255;
const int BrushAlphaMutationLow = 255;
const int BrushAlphaMutationHigh = 255;

// Init settings.
const int PolygonInitRange = 0; // (-5, 5) range of a point.

// Structure settings.
const int PointsPerPolygonMax = 10;
const int PointsPerPolygonMin = 3;
const int PointsPerCanvasMax = 1500;
const int PointsPerCanvasMin = 0;
const int PolygonsPerCanvasMax = 250;
const int PolygonsPerCanvasMin = 1;
};
#endif