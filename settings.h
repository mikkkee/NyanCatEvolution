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
// MaxWidth and MaxHeight sets the image size used for fitness calculation.
// Increasing of MaxWidth and MaxHeight increases the matching quality as well as
// computation loads.
// Carefully choose the values to compromise between image quality and speed.
const auto CanvasMatDataType = CV_8UC3;
const int MaxWidth = 200;
const int MaxHeight = 200;

// Mutation Rates.
// The sum of all mutations rates for a class shall not exceed its MutationRateBase.
// Mutation rates are calculated by dividing its value by MutationRateBase for that class.
// Mutation selection strategy:
// 1. A random integer p is generated between [0, MutationRateBase).
// 2. Test if p falls in one of the intevals: [0, r1), [r1, r1+r2),  [r1+r2, r1+r2+r3), ....
// 3. Apply mutation with rate r1, r2, or r3, ..., accordingly.
// Note: it's not necessary for mutation rates sum to be equal to MutationRateBase.
const int CanvasAddPolygonRate = 1000;
const int CanvasRemovePolygonRate = 1000;
const int CanvasMovePolygonRate = 1000;
const int CanvasMutatePolygonRate = 7000;
const int CanvasMutationRateBase = 10000;

const int PolygonAddPointRate = 2500;
const int PolygonRemovePointRate = 2500;
const int PolygonMutatePointRate = 2500;
const int PolygonMutateBrushRate = 2500;
const int PolygonMutationRateBase = 10000;

const int PointMaxMutationRate = 100;  // Move anywhere in canvas.
const int PointMidMutationRate = 1000;   // Move around original position.
const int PointMinMutationRate = 8900;   // Move around original position slightly.
const int PointMutationRateBase = 10000;

const int BrushBlueMutationRate = 1;
const int BrushGreenMutationRate = 1;
const int BrushRedMutationRate = 1;
const int BrushAlphaMutationRate = 1;


// Structure settings.
const int PolygonInitRange = 1;      // Limits a polygon to 1px to raise its survival probability.
const int PointsPerPolygonMax = 10;
const int PointsPerPolygonMin = 3;
const int PointsPerCanvasMax = 1500;
const int PointsPerCanvasMin = 0;
const int PolygonsPerCanvasMax = 250;
const int PolygonsPerCanvasMin = 1;

// Mutation range.
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
};
#endif
