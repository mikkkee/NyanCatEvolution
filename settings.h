#ifndef EVOART_DNA_SETTINGS_H_
#define EVOART_DNA_SETTINGS_H_

#include <string>
#include <opencv/cv.hpp>

namespace settings
{
// Evolution settings.
const int DumpFrequency = 1000;  // Sets frequency of snapshot saving.
const int LogFrequency = 50;     // Sets frequency of console output.
const std::string DumpPrefix = "selected/";
const std::string DumpExtension = ".png";
const int FitConvergence = 1000;

// Rendering settings.
const auto CanvasMatDataType = CV_8UC4;
const int MaxWidth = 314;   // Sets the size used for fitness calculation.
const int MaxHeight = 314;  // Adjust MaxHeight and MaxWidth to comprimise between speed and image quality.

// Mutation rates, a muration rate N means this mutation
// happens with a probability of 1 / N.
const int CanvasAddPolygonRate = 20;
const int CanvasRemovePolygonRate = 200;
const int CanvasMovePolygonRate = 200;
const int PointMaxMutationRate = 2000;  // Move anywhere in canvas.
const int PointMidMutationRate = 600;   // Move around original position.
const int PointMinMutationRate = 600;   // Move around original position slightly.
const int BrushBlueMutationRate = 300;
const int BrushGreenMutationRate = 300;
const int BrushRedMutationRate = 300;
const int BrushAlphaMutationRate = 300;
const int PolygonAddPointRate = 300;
const int PolygonRemovePointRate = 300;

// Structure settings.
const int PolygonInitRange = 0;      // Limits a polygon to 1px to raise its survival probability.
const int PointsPerPolygonMax = 10;
const int PointsPerPolygonMin = 3;
const int PointsPerCanvasMax = 1500;
const int PointsPerCanvasMin = 0;
const int PolygonsPerCanvasMax = 250;
const int PolygonsPerCanvasMin = 1;

// Mutation range.
const int ColorMutationRange = 15;
const int PointMidMutationRange = 2;
const int PointMinMutationRange = 2;
const int BrushBlueMutationLow = 0;
const int BrushBlueMutationHigh = 255;
const int BrushGreenMutationLow = 0;
const int BrushGreenMutationHigh = 255;
const int BrushRedMutationLow = 0;
const int BrushRedMutationHigh = 255;
const int BrushAlphaMutationLow = 10;
const int BrushAlphaMutationHigh = 60;
};
#endif
