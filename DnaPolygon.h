#ifndef EVOART_DNAPOLYGON_H_
#define EVOART_DNAPOLYGON_H_

#include <vector>
#include "DnaBrush.h"
#include "DnaCanvas.h"
#include "DnaPoint.h"

// DnaPolygon consists of DnaPoints.
class DnaPolygon
{
public:
	DnaPolygon();
	DnaPolygon(const DnaPolygon& polygon);
	~DnaPolygon();
	DnaPolygon& operator=(const DnaPolygon& polygon);

	int Count() const;  // Number of points.
	DnaPolygon * Clone();
	void Mutate(DnaCanvas& canvas);

	std::vector<DnaPoint> *points;  // List of points.
	DnaBrush *brush;                // Fill color - (b, g, r, a).

private:
	void AddPoint(DnaCanvas& canvas);
	void RemovePoint(DnaCanvas& canvas);
};
#endif