#pragma once

#include "Config.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
    AsBorder(void);
    void Init(void);

    void Draw(HDC hdc);

private:
    void Draw_Element(HDC hdc, int x, int y, bool is_top_border);
    HPEN Border_pen_blue, Border_pen_white, Border_pen_bg;
    HBRUSH Border_brush_blue, Border_brush_white, Border_brush_bg;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------