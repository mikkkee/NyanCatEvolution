#ifndef EVOART_DNAPOINT_H_
#define EVOART_DNAPOINT_H_

class DnaCanvas;

// Point class, the range of (x, y) is actually limited
// to be inside the canvas whenever it mutates.
// Example:
//     int x =0, y = 0;
//     DnaPoint point(x, y);
class DnaPoint
{
public:
	DnaPoint(int x = 0, int y = 0);
	void Random();
	void Mutate(DnaCanvas& canvas);

	int x;
	int y;
};
#endif
