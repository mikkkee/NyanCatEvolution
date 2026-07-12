#ifndef EVOART_DNABRUSH_H_
#define EVOART_DNABRUSH_H_

class DnaCanvas;

// This class defines a RGBA color.
// Example:
//     DnaBrush brush;
class DnaBrush
{
public:
	DnaBrush();
	void Mutate(DnaCanvas& canvas);

	int blue;
	int green;
	int red;
	int alpha;
};
#endif
