#ifndef EVOART_DNACANVAS_H_
#define EVOART_DNACANVAS_H_

#include <vector>

class DnaPolygon;

class DnaCanvas
{
public:
	DnaCanvas();
	DnaCanvas(DnaCanvas& canvas);
	~DnaCanvas();
	DnaCanvas& operator=(const DnaCanvas& canvas);
	void SetDirty();
	int PointCount() const;
	void Mutate();

	bool is_dirty;                      // A polygon gets dirty after a successful mutation.
	std::vector<DnaPolygon> *polygons;  // List of polygons.

private:
	void AddPolygon();
	void RemovePolygon();
	void MovePolygon();
	void MutatePolygon();
};
#endif