#ifndef EVOART_DNAPOLYGON_H_
#define EVOART_DNAPOLYGON_H_

#include <vector>
#include "DnaBrush.h"
#include "DnaPoint.h"

class DnaCanvas;

// DnaPolygon consists of DnaPoints.
class DnaPolygon
{
public:
	DnaPolygon();

	int Count() const;  // Number of points.
	void Mutate(DnaCanvas& canvas);

	std::vector<DnaPoint> points;  // List of points.
	DnaBrush brush;                // Fill color - (b, g, r, a).

private:
	void AddPoint(DnaCanvas& canvas);
	void RemovePoint(DnaCanvas& canvas);
	void MutatePoint(DnaCanvas& canvas);
	void MutateBrush(DnaCanvas& canvas);
};
#endif
