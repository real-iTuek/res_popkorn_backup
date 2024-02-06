#include "Config.h"

//AsConfig
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsConfig::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush, int width)
{
    pen = CreatePen(PS_SOLID, width, RGB(r, g, b));     //pen
    brush = CreateSolidBrush(RGB(r, g, b));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
