#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

enum ELetter_Type
{
    ELT_None,
    ELT_O,
};

enum EBrick_Type
{
    EBT_None,

    EBT_Red,
    EBT_Blue
};

enum EKey_Type
{
    EKT_Left,
    EKT_Right,
    EKT_Space
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const int Timer_ID = WM_USER + 1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine;
class ALevel;
class AsPlatform;
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ABall
{
public:
    ABall(void);
    void Init(void);


    void Draw(HDC hdc, RECT& paint_area, AsEngine *engine);
    void Move(AsEngine *engine, ALevel *level, AsPlatform *platform);

    double Ball_Direction;       

    static const int Ball_Size = 4;


private:
    double Ball_Speed;
    int Ball_X_Pos, Ball_Y_Pos;

    RECT Ball_Rect, Prev_Ball_Rect;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class ALevel
{
public:
    void Init(void);

    void Check_Level_Brick_Hit(int& next_y_pos, double &ball_Direction);
    void Draw(HDC hdc, RECT &paint_area, AsEngine *engine);


    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Level_Height_01 = 14;    // высота уровн€ в €чейках
    static const int Level_Width_01 = 12;     // ширина уровн€ в €чейках
    static const int Level_X_Offset = 8;
    static const int Level_Y_Offset = 6;


private:
    void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type, AsEngine *engine);
    void Set_Brick_Letter(HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush, bool is_switch_color, AsEngine *engine);
    void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step, AsEngine *engine);

    HPEN pen_Letter;
    HBRUSH brush_Letter;
    RECT Level_Rect;

    static const int Brick_Width = 15;
    static const int Brick_Height = 7;


};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
    AsPlatform(void);
    void Init();

    void Redraw_Platform(AsEngine *engine);
    void Draw(HDC hdc, AsEngine *engine, RECT &paint_area);


    int Width;
    int X_Pos;
    int X_Step;


    static const int Height = 7;
    static const int Y_Pos = 185;
    static const int Circle_size = 7;

private:
    RECT Platform_Rect, Prev_Platform_Rect;

    int Inner_width;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
    AsBorder(void);
    void Init(void);

    void Draw(HDC hdc, AsEngine *engine);

    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;
    
private:
    void Draw_Element(HDC hdc, int x, int y, bool is_top_border, AsEngine *engine);
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
    AsEngine(void);


    void Init_Engine(HWND);
    void Draw_Frame(HDC hdc, RECT& paint_area);
    int  On_Key_Down(EKey_Type);
    int  On_Timer(void);

    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush, int width);

    HWND hwnd;
    HPEN pen_blue, pen_bg, pen_red, pen_white;
    HBRUSH brush_red, brush_bg, brush_blue, brush_white, brush_Letter;


    static const int Global_Scale = 3;
    static const int Max_Y_Pos = 195 - ABall::Ball_Size;
    static const int Max_X_Pos = ALevel::Level_X_Offset + ALevel::Cell_Width * ALevel::Level_Width_01;


private:

    ABall Ball;
    ALevel Level;
    AsPlatform Platform;
    AsBorder Border;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------