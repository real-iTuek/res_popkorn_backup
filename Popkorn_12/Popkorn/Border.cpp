#include "Border.h"



//AsBorder
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder(void)
    :Border_pen_blue(0), Border_pen_white(0), Border_pen_bg(0), Border_brush_blue(0), Border_brush_white(0), Border_brush_bg(0)
{   }  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Init(void)
{
    AsConfig::Create_Pen_Brush(85, 225, 225, Border_pen_blue, Border_brush_blue, 0);
    AsConfig::Create_Pen_Brush(255, 255, 255, Border_pen_white, Border_brush_white, 0);
    AsConfig::Create_Pen_Brush(15, 63, 31, Border_pen_bg, Border_brush_bg, 0);
    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border)
{// отрисовка элемента границы уровня (tile)

    switch (is_top_border)
    {
    case false:
        // основная линия 
        SelectObject(hdc,   Border_pen_blue);
        SelectObject(hdc, Border_brush_blue);
        Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

        // белая окантовка
        SelectObject(hdc,   Border_pen_white);
        SelectObject(hdc, Border_brush_white);
        Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

        // черная точка, перфорация
        SelectObject(hdc,   Border_pen_bg);
        SelectObject(hdc, Border_brush_bg);
        Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale);
        break;

    case true:
        // основная линия 
        SelectObject(hdc,   Border_pen_blue);
        SelectObject(hdc, Border_brush_blue);
        Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

        // белая окантовка
        SelectObject(hdc,   Border_pen_white);
        SelectObject(hdc, Border_brush_white);
        Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale);

        // черная точка, перфорация
        SelectObject(hdc,   Border_pen_bg);
        SelectObject(hdc, Border_brush_bg);
        Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
        break;
    deafault:
        break;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc)
{// отрисовка всех границ уровня

 //1. левая граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, i*4 + 1, false);
    }

    //2. правая граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 201, i*4 + 1, false);
    }

    //3. верхняя граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, i*4 + 3, 0, true);
    }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
