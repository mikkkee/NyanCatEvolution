#include "DnaPoint.h"
#include <algorithm>
#include "DnaCanvas.h"
#include "settings.h"
#include "tools.h"

DnaPoint::DnaPoint(int x, int y) : x(x), y(y) {}

DnaPoint* DnaPoint::Clone() const 
{
	return new DnaPoint(x, y);
}

void DnaPoint::Random()
{
	x = tools::GetRandomNumber(0, settings::MaxWidth);
	y = tools::GetRandomNumber(0, settings::MaxHeight);
}

// Moves point to a mutated position.
// There are three possible mutation ways:
// 1. MaxMutation, moves all around the canvas. This event should be very rare.
// 2. MidMutation, moves within a medium range around current position
// 3. MinMutation, moves slightly (~3px) around current position.
void DnaPoint::Mutate(DnaCanvas& canvas)
{
	int prob = tools::GetRandomNumber(0, settings::PointMutationRateBase);
	int cut1 = settings::PointMaxMutationRate;
	int cut2 = cut1 + settings::PointMidMutationRate;
	int cut3 = cut2 + settings::PointMinMutationRate;
	if (prob < cut1) {
		x = tools::GetRandomNumber(0, settings::MaxWidth);
		y = tools::GetRandomNumber(0, settings::MaxHeight);
		canvas.SetDirty();
	} else if (prob < cut2) {
		int mutationRange = settings::PointMidMutationRange;
		x += tools::GetRandomNumber(-mutationRange, mutationRange + 1);
		y += tools::GetRandomNumber(-mutationRange, mutationRange + 1);
		// Keep x and y in canvas range.
		x = std::min(std::max(0, x), settings::MaxWidth - 1);
		y = std::min(std::max(0, y), settings::MaxHeight - 1);
		canvas.SetDirty();
	} else if (prob < cut3) {
		int mutationRange = settings::PointMinMutationRange;
		x += tools::GetRandomNumber(-mutationRange, mutationRange + 1);
		y += tools::GetRandomNumber(-mutationRange, mutationRange + 1);
		// Keep x and y in canvas range.
		x = std::min(std::max(0, x), settings::MaxWidth - 1);
		y = std::min(std::max(0, y), settings::MaxHeight - 1);
		canvas.SetDirty();
	};
}