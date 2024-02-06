#pragma once

#include "Config.h"

class AsPlatform
{
public:
    AsPlatform(void);
    void Init();

    void Redraw_Platform(HWND hwnd);
    void Draw(HDC hdc, RECT &paint_area);


    int Width;
    int X_Pos;
    int X_Step;


private:
    RECT Platform_Rect, Prev_Platform_Rect;
    HPEN platform_pen_white, platform_pen_red, platform_pen_blue, platform_pen_bg;
    HBRUSH platform_brush_white, platform_brush_red, platform_brush_blue, platform_brush_bg;

    int Inner_width;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
