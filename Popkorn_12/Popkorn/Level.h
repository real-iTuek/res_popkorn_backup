#pragma once

#include "Config.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum EBrick_Type
{
    EBT_None,

    EBT_Red,
    EBT_Blue
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum ELetter_Type
{
    ELT_None,
    ELT_O,
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
    ALevel(void);
    void Init(void);

    void Check_Level_Brick_Hit(int& next_y_pos, double &ball_Direction);
    void Draw(HDC hdc, RECT &paint_area);


private:
    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type);
    void Set_Brick_Letter(HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush, bool is_switch_color);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step);

    HPEN Level_pen_Letter, Level_pen_red, Level_pen_blue;
    HBRUSH Level_brush_Letter, Level_brush_red, Level_brush_blue;
    RECT Level_Rect;

    static const int Brick_Width = 15;
    static const int Brick_Height = 7;


};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

