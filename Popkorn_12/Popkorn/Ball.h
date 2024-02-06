#pragma once

#include "Config.h"
#include "Level.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
    ABall(void);
    void Init(void);


    void Draw(HDC hdc, RECT& paint_area);
    void Move(HWND Hwnd, ALevel *level, int platform_x_pos, int platform_width);

    double Ball_Direction;       


private:
    double Ball_Speed;
    int Ball_X_Pos, Ball_Y_Pos;

    HPEN Ball_pen_white, Ball_pen_bg;
    HBRUSH Ball_brush_white, Ball_brush_bg;
    RECT Ball_Rect, Prev_Ball_Rect;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
