#include "DnaCanvas.h"
#include "DnaBrush.h"
#include "DnaPoint.h"
#include "DnaPolygon.h"
#include "tools.h"
#include "settings.h"

DnaCanvas::DnaCanvas()
{
	polygons = new std::vector < DnaPolygon >;
	for (int i = 0; i < settings::PolygonsPerCanvasMin; i++) {
		AddPolygon();
	};
	SetDirty();
}

DnaCanvas::DnaCanvas(DnaCanvas& canvas)
{
	// Copy polygons.
	polygons = new std::vector < DnaPolygon >;
	for (auto& polygon : *(canvas.polygons)) {
		polygons->push_back(polygon);
	};
	is_dirty = canvas.is_dirty;
}

DnaCanvas::~DnaCanvas() { delete polygons; }

DnaCanvas& DnaCanvas::operator=(const DnaCanvas& canvas)
{
	if (this != &canvas) {
		// Copy polygons.
		delete polygons;
		polygons = new std::vector < DnaPolygon >;
		for (auto& polygon : *(canvas.polygons)) {
			polygons->push_back(polygon);
		};
		is_dirty = canvas.is_dirty;
	};
	return *this;
}

void DnaCanvas::SetDirty() { is_dirty = true; }

int DnaCanvas::PointCount() const
{
	int count = 0;
	for (auto& polygon : *polygons) {
		count += polygon.points->size();
	};
	return count;
};

// Canvas can mutate in the following ways:
// 1. Add a polygon.
// 2. Remove a polygon.
// 3. Move the order of its polygons.
// 4. Mutate its polygons.
// The number of polygons in a single canvas is limited in settings.h.
void DnaCanvas::Mutate()
{
	int prob = tools::GetRandomNumber(0, settings::CanvasMutationRateBase);
	int cut1 = settings::CanvasAddPolygonRate;
	int cut2 = cut1 + settings::CanvasRemovePolygonRate;
	int cut3 = cut2 + settings::CanvasMovePolygonRate;
	int cut4 = cut3 + settings::CanvasMutatePolygonRate;
	if (prob < cut1) { AddPolygon(); }
	else if (prob < cut2) { RemovePolygon(); }
	else if (prob < cut3) { MovePolygon(); }
	else if (prob < cut4) { MutatePolygon(); };
}

void DnaCanvas::AddPolygon()
{
	if (polygons->size() < settings::PolygonsPerCanvasMax) {
		DnaPolygon polygon;
		polygons->push_back(polygon);
		SetDirty();
	};
}

void DnaCanvas::RemovePolygon()
{
	if (polygons->size() > settings::PolygonsPerCanvasMin) {
		int index = tools::GetRandomNumber(0, polygons->size());
		polygons->erase(polygons->begin() + index);
		SetDirty();
	};
}

void DnaCanvas::MovePolygon()
{
	if (polygons->size() <= 1) return;

	int index_rm = tools::GetRandomNumber(0, polygons->size());
	DnaPolygon poly = (*polygons)[index_rm];
	polygons->erase(polygons->begin() + index_rm);

	int index_insert = tools::GetRandomNumber(0, polygons->size());
	polygons->insert(polygons->begin() + index_insert, poly);

	SetDirty();
}

void DnaCanvas::MutatePolygon()
{
	int index_mutate = tools::GetRandomNumber(0, polygons->size());
	(*polygons)[index_mutate].Mutate(*this);
	SetDirty();
}
