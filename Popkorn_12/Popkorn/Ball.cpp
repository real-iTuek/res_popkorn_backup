#include "Ball.h"



//ABall
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall(void)
    :   Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4), Ball_pen_white(0), Ball_pen_bg(0), Ball_brush_white(0), Ball_brush_bg(0), Ball_Rect{}, Prev_Ball_Rect{}
{   }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Init(void)
{
    AsConfig::Create_Pen_Brush(15, 63, 31, Ball_pen_bg, Ball_brush_bg, 0);
    AsConfig::Create_Pen_Brush(255, 255, 255, Ball_pen_white, Ball_brush_white, 0);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area)
{
    RECT intersection_rect;

    if(!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect)) 
        return;

    //1. Очищаем фон
    SelectObject(hdc, Ball_pen_bg);
    SelectObject(hdc, Ball_brush_bg);
    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

    //2. Рисует шарик
    SelectObject(hdc, Ball_pen_white);
    SelectObject(hdc, Ball_brush_white);
    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(HWND Hwnd, ALevel *level, int platform_x_pos, int platform_width)
{
    int next_x_pos, next_y_pos;
    int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
    int platform_y_pos = AsConfig::Y_Pos - AsConfig::Ball_Size;


    Prev_Ball_Rect = Ball_Rect;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

    // корректировка направления шарика при достижении границ уровня или платформы
    //1. Левая граница
    if (next_x_pos < AsConfig::Border_X_Offset)
    {
        next_x_pos = AsConfig::Level_X_Offset - (next_x_pos - AsConfig::Level_X_Offset);
        Ball_Direction = M_PI - Ball_Direction;
    }

    //2. Верхняя граница
    if (next_y_pos < AsConfig::Border_Y_Offset)
    {
        next_y_pos = AsConfig::Border_Y_Offset - (next_y_pos - AsConfig::Border_Y_Offset);
        Ball_Direction =  - Ball_Direction;
    }

    //3. Правая граница
    if (next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }

    //4. Пол
    if (next_y_pos > AsConfig::Max_Y_Pos)
    {
        next_y_pos = AsConfig::Max_Y_Pos - (next_y_pos - AsConfig::Max_Y_Pos);
        Ball_Direction =  -Ball_Direction;
    }

    //5. От платформы
    if (next_y_pos > platform_y_pos)
    {
        if ((next_x_pos >= platform_x_pos) && (next_x_pos <= platform_x_pos + platform_width))
        {
            next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
            Ball_Direction =  -Ball_Direction;
        }
    }

    //6. От кирпичей
    level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

    // Смещаем шарик
    Ball_X_Pos = next_x_pos;
    Ball_Y_Pos = next_y_pos;   //y rises down

    Ball_Rect.left   = Ball_X_Pos * AsConfig::Global_Scale;
    Ball_Rect.top    = Ball_Y_Pos * AsConfig::Global_Scale;
    Ball_Rect.right  = Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Global_Scale;
    Ball_Rect.bottom = Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Global_Scale;

    InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(Hwnd, &Ball_Rect, FALSE);


    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
