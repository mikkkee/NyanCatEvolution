#ifndef EVOART_DNACANVAS_H_
#define EVOART_DNACANVAS_H_

#include <vector>
#include "DnaPolygon.h"

class DnaCanvas
{
public:
	DnaCanvas();
	void SetDirty();
	void ClearDirty();
	int PointCount() const;
	void Mutate();

	bool is_dirty;                     // A canvas gets dirty after a mutation changed it.
	std::vector<DnaPolygon> polygons;  // List of polygons.

private:
	void AddPolygon();
	void RemovePolygon();
	void MovePolygon();
	void MutatePolygon();
};
#endif
