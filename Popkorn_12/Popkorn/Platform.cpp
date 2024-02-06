#include "Platform.h"



//APlatform
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform(void) 
    : Inner_width(21), X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Global_Scale * 2), Width(28), Platform_Rect{}, Prev_Platform_Rect{}, 
      platform_pen_white(0), platform_pen_red(0), platform_pen_blue(0), platform_pen_bg(0), platform_brush_white(0), platform_brush_red(0), platform_brush_blue(0), platform_brush_bg(0)
{   }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(void)
{
    AsConfig::Create_Pen_Brush(255, 85, 85, platform_pen_red, platform_brush_red, 0);
    AsConfig::Create_Pen_Brush(85, 225, 225, platform_pen_blue, platform_brush_blue, 0);
    AsConfig::Create_Pen_Brush(255, 255, 255, platform_pen_white, platform_brush_white, 0);
    AsConfig::Create_Pen_Brush(15, 63, 31, platform_pen_bg, platform_brush_bg, 0);
    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = X_Pos * AsConfig::Global_Scale;
    Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Width* AsConfig::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + AsConfig::Height* AsConfig::Global_Scale;

    InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(hwnd, &Platform_Rect, FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{//отрисовка платформы
    RECT intersection_rect;
    int x = X_Pos;
    int y = AsConfig::Y_Pos;

    if(! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect)) 
        return;

    SelectObject(hdc, platform_pen_bg);
    SelectObject(hdc, platform_brush_bg);
    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    //1. Рисуем боковые шарики
    SelectObject(hdc, platform_pen_red);
    SelectObject(hdc, platform_brush_red);
    Ellipse(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Circle_size) * AsConfig::Global_Scale, (y + AsConfig::Circle_size) * AsConfig::Global_Scale);
    Ellipse(hdc, (x + Inner_width) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + AsConfig::Circle_size + Inner_width) * AsConfig::Global_Scale, (y + AsConfig::Circle_size) * AsConfig::Global_Scale);

    //2. Рисуем блик
    SelectObject(hdc, platform_pen_white);
    Arc(hdc, (x + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + AsConfig::Circle_size - 1) * AsConfig::Global_Scale, (y + AsConfig::Circle_size - 1) * AsConfig::Global_Scale,
        (x + 1 + 1) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 1 + 2) * AsConfig::Global_Scale);

    //3. Рисуем среднюю часть
    SelectObject(hdc, platform_pen_blue);
    SelectObject(hdc, platform_brush_blue);
    RoundRect(hdc, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4 + Inner_width - 1) * AsConfig::Global_Scale, (y + 1 + 5) * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

    return;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------