#pragma once


#include "Config.h"
#include "Border.h"
#include "Ball.h"
#include "Platform.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

enum EKey_Type
{
    EKT_Left,
    EKT_Right,
    EKT_Space
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const int Timer_ID = WM_USER + 1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
    AsEngine(void);


    void Init_Engine(HWND);
    void Draw_Frame(HDC hdc, RECT& paint_area);
    int  On_Key_Down(EKey_Type);
    int  On_Timer(void);


    HWND hwnd;
    HPEN pen_blue, pen_bg, pen_red, pen_white;
    HBRUSH brush_red, brush_bg, brush_blue, brush_white, brush_Letter;


private:

    ABall Ball;
    ALevel Level;
    AsPlatform Platform;
    AsBorder Border;
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------