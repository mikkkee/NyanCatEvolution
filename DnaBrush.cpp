#include "DnaBrush.h"
#include "DnaCanvas.h"
#include "settings.h"
#include "tools.h"

DnaBrush::DnaBrush():
blue(tools::GetRandomNumber(
settings::BrushBlueMutationLow, settings::BrushBlueMutationHigh + 1)),
green(tools::GetRandomNumber(
settings::BrushGreenMutationLow, settings::BrushGreenMutationHigh + 1)),
red(tools::GetRandomNumber(
settings::BrushRedMutationLow, settings::BrushRedMutationHigh + 1)),
alpha(tools::GetRandomNumber(
settings::BrushAlphaMutationLow, settings::BrushAlphaMutationHigh + 1))
{}

DnaBrush* DnaBrush::Clone(){
	DnaBrush * brush = new DnaBrush;
	brush->blue = blue;
	brush->green = green;
	brush->red = red;
	brush->alpha = alpha;
	return brush;
}

// Mutates color values.
// RGB values are mutated around current values.
// Alpha value is changed regardless of current value.
void DnaBrush::Mutate(DnaCanvas& canvas)
{
	blue += tools::GetRandomNumber(
		-settings::ColorMutationRange, settings::ColorMutationRange + 1);
	blue = std::min(255, std::max(0, blue));

	green += tools::GetRandomNumber(
		-settings::ColorMutationRange, settings::ColorMutationRange + 1);
	green = std::min(255, std::max(0, green));

    red += tools::GetRandomNumber(
		-settings::ColorMutationRange, settings::ColorMutationRange + 1);
	red = std::min(255, std::max(0, red));

	alpha = tools::GetRandomNumber(
		settings::BrushAlphaMutationLow, settings::BrushAlphaMutationHigh +1);

	canvas.SetDirty();
}