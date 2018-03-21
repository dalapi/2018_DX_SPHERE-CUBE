#include "Number.h"
#include "polygon.h"

void Number::DrawNumber(int n, float x, float y)
{
	if (n<0 || n>9) return;

	Set2DTexture(TEX_NUMBER);
	SetPolygonColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	DrawPolygon(x, y, NUMBER_WIDTH * n, 0, NUMBER_WIDTH, NUMBER_HEIGHT, false);
}
