#include "Engine.h"




//AsEngine
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine(void) 
    :hwnd(0), pen_blue(0), pen_bg(0), pen_red(0), pen_white(0), brush_red(0), brush_bg(0), brush_blue(0), brush_white(0), brush_Letter(0)
{   }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hWnd)
{//задает значения цветов для разных констант
    hwnd = hWnd;

    AsConfig::Create_Pen_Brush(255, 85, 85, pen_red, brush_red, 0);
    AsConfig::Create_Pen_Brush(85, 225, 225, pen_blue, brush_blue, 0);
    AsConfig::Create_Pen_Brush(255, 255, 255, pen_white, brush_white, 0);
    AsConfig::Create_Pen_Brush(15, 63, 31, pen_bg, brush_bg, 0);

    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    Platform.Redraw_Platform(hwnd);

    SetTimer(hWnd, Timer_ID, 50, 0);

    // начальное положение мячика
    //Ball.Ball_Rect.left   = (Level_X_Offset + Ball.Ball_X_Pos) * Global_Scale;
    //Ball.Ball_Rect.top    = (Level_Y_Offset + Ball.Ball_Y_Pos) * Global_Scale;
    //Ball.Ball_Rect.right  = Ball.Ball_Rect.left + Ball.AsConfig::Ball_Size * Global_Scale;
    //Ball.Ball_Rect.bottom = Ball.Ball_Rect.top + Ball.AsConfig::Ball_Size * Global_Scale;

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры

    Level.Draw(hdc, paint_area);

    Platform.Draw(hdc, paint_area);

    /*for (int i = 0; i < 16; i++)
    {
    Draw_Brick_Letter(hdc, 20 + (i * Cell_Width * Global_Scale), 100, EBT_Blue, ELT_O, i);
    Draw_Brick_Letter(hdc, 20 + (i * Cell_Width * Global_Scale), 130, EBT_Red, ELT_O, i);
    }*/

    Ball.Draw(hdc, paint_area);

    Border.Draw(hdc);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;

        if (Platform.X_Pos <= AsConfig::Border_X_Offset)
            Platform.X_Pos = AsConfig::Border_X_Offset;

        Platform.Redraw_Platform(hwnd);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
            Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

        Platform.Redraw_Platform(hwnd);
        break;

    case EKT_Space:
        break;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
    Ball.Move(hwnd, &Level, Platform.X_Pos, Platform.Width);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

