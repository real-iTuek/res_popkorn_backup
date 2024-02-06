#include "Level.h"

char Level_01[AsConfig::Level_Height_01][AsConfig::Level_Width_01] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,
    2,2,2,2,2,2,2,2,2,2,2,2,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//ALevel
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ALevel::ALevel(void)
    :Level_pen_Letter(0), Level_pen_red(0), Level_pen_blue(0), Level_brush_Letter(0), Level_brush_red(0), Level_brush_blue(0), Level_Rect{}
{   }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Init(void)
{
    Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
    Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
    Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width_01 * AsConfig::Global_Scale;
    Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height_01 * AsConfig::Global_Scale;
    AsConfig::Create_Pen_Brush(255, 255, 255, Level_pen_Letter, Level_brush_Letter, AsConfig::Global_Scale);
    AsConfig::Create_Pen_Brush(255, 85, 85, Level_pen_red, Level_brush_red, AsConfig::Global_Scale);
    AsConfig::Create_Pen_Brush(85, 225, 225, Level_pen_blue, Level_brush_blue, AsConfig::Global_Scale);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_Direction)
{// проверка на отражение от кирпичей
    int brick_y_pos = AsConfig::Level_Y_Offset + AsConfig::Level_Height_01 * AsConfig::Cell_Height;

    for (int i = AsConfig::Level_Height_01 - 1; i >= 0; i--)
    {
        for (int j = 0; j < AsConfig::Level_Width_01; j++)
        {
            if (Level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_Direction =  -ball_Direction;
            }

        }

        brick_y_pos -= AsConfig::Cell_Height;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{//вывод кирпича
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Red:
        pen = Level_pen_red;
        brush = Level_brush_red;
        break;

    case EBT_Blue:
        pen = Level_pen_blue;
        brush = Level_brush_blue;
        break;

    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + Brick_Width) * AsConfig::Global_Scale, (y + Brick_Height) * AsConfig::Global_Scale, 2*AsConfig::Global_Scale, 2*AsConfig::Global_Scale);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter(HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush, bool is_switch_color)
{
    if (is_switch_color)
    {
        front_pen = Level_pen_red;
        front_brush = Level_brush_red;

        back_pen = Level_pen_blue;
        back_brush = Level_brush_blue;
    }
    else
    {
        front_pen = Level_pen_blue;
        front_brush = Level_brush_blue;

        back_pen = Level_pen_red;
        back_brush = Level_brush_red;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
    bool switch_color;
    double offset;
    int brick_half_height = Brick_Height * AsConfig::Global_Scale / 2;
    double rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;      // Преобразование шага в угол
    int back_part_offset;
    HPEN front_pen, back_pen;
    HBRUSH front_brush, back_brush;
    XFORM xform, old_xform;

    if (!(brick_type == EBT_Blue || brick_type == EBT_Red)) return;        // падающие буквы только красные и синие

    // корректиируем угол поворота ии шаг вращения
    rotation_step = rotation_step % 16;
    if (rotation_step < 8)  rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
    else   rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - rotation_step);

    // меняем или не меняем цвет
    if (rotation_step > 4 && rotation_step <= 12)
    {
        if (brick_type == EBT_Blue)  switch_color = true;
        else  switch_color = false;
    }
    else
    {
        if (brick_type == EBT_Red)  switch_color = true;
        else  switch_color = false;
    }
    Set_Brick_Letter(front_pen, front_brush, back_pen, back_brush, switch_color);

    if (rotation_step == 4 || rotation_step == 12)
    {
        // выводим фон
        SelectObject(hdc, back_pen);                                                            
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y +brick_half_height - AsConfig::Global_Scale, x + Brick_Width * AsConfig::Global_Scale, y + brick_half_height);

        // выводим перед
        SelectObject(hdc, front_pen);                                                            
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y +brick_half_height, x + Brick_Width * AsConfig::Global_Scale, y +brick_half_height + AsConfig::Global_Scale - 1);
    }
    else
    {
        SetGraphicsMode(hdc, GM_ADVANCED);

        // Настраииваем матрицу "переворота" буквы
        xform.eM11 = 1.0f; 
        xform.eM12 = 0.0f; 
        xform.eM21 = 0.0f; 
        xform.eM22 = (float)cos(rotation_angle); 
        xform.eDx  = (float)x;
        xform.eDy  = (float)y + (float)brick_half_height;  
        GetWorldTransform(hdc, &old_xform);
        SetWorldTransform(hdc, &xform);

        // выводим фон
        SelectObject(hdc, back_pen);                                                           
        SelectObject(hdc, back_brush);

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * AsConfig::Global_Scale, brick_half_height - back_part_offset);

        // выводим перед
        SelectObject(hdc, front_pen);                                                            
        SelectObject(hdc, front_brush);
        Rectangle(hdc, 0, -brick_half_height, Brick_Width * AsConfig::Global_Scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            switch (letter_type)
            {
            case ELT_O:
                SelectObject(hdc, Level_pen_Letter);
                Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, (-5 * AsConfig::Global_Scale) / 2, 0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
                break;

            default:
                break;
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area)
{//вывод уровня
    RECT intersection_rect;

    if(! IntersectRect(&intersection_rect, &paint_area, &Level_Rect)) 
        return;

    for (int i = 0; i < AsConfig::Level_Height_01; i++)
    {
        for (int j = 0; j < AsConfig::Level_Width_01; j++)
        {
            Draw_Brick(hdc, AsConfig::Level_X_Offset + j * AsConfig::Cell_Width, AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Level_01[i][j]);
        }
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
