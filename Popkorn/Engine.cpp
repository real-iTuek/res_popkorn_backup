#include "Engine.h"

char Level_01[ALevel::Level_Height_01][ALevel::Level_Width_01] =
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




//ABall
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ABall::ABall(void)
    :   Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{   }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Init(void)
{
    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area, AsEngine *engine)
{
    RECT intersection_rect;

    if(!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect)) 
        return;

    //1. Очищаем фон
    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

    //2. Рисует шарик
    SelectObject(hdc, engine->pen_white);
    SelectObject(hdc, engine->brush_white);
    Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
    int next_x_pos, next_y_pos;
    int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
    int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;


    Prev_Ball_Rect = Ball_Rect;

    next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
    next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

    // корректировка направления шарика при достижении границ уровня или платформы
    //1. Левая граница
    if (next_x_pos < AsBorder::Border_X_Offset)
    {
        next_x_pos = ALevel::Level_X_Offset - (next_x_pos - ALevel::Level_X_Offset);
        Ball_Direction = M_PI - Ball_Direction;
    }

    //2. Верхняя граница
    if (next_y_pos < AsBorder::Border_Y_Offset)
    {
        next_y_pos = AsBorder::Border_Y_Offset - (next_y_pos - AsBorder::Border_Y_Offset);
        Ball_Direction =  - Ball_Direction;
    }

    //3. Правая граница
    if (next_x_pos > max_x_pos)
    {
        next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
        Ball_Direction = M_PI - Ball_Direction;
    }

    //4. Пол
    if (next_y_pos > AsEngine::Max_Y_Pos)
    {
        next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
        Ball_Direction =  -Ball_Direction;
    }

    //5. От платформы
    if (next_y_pos > platform_y_pos)
    {
        if ((next_x_pos >= platform->X_Pos) && (next_x_pos <= platform->X_Pos + platform->Width))
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

    Ball_Rect.left   = Ball_X_Pos * AsEngine::Global_Scale;
    Ball_Rect.top    = Ball_Y_Pos * AsEngine::Global_Scale;
    Ball_Rect.right  = Ball_Rect.left + Ball_Size * AsEngine::Global_Scale;
    Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsEngine::Global_Scale;

    InvalidateRect(engine->hwnd, &Prev_Ball_Rect, FALSE);
    InvalidateRect(engine->hwnd, &Ball_Rect, FALSE);


    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//ALevel
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Init(void)
{
    Level_Rect.left = ALevel::Level_X_Offset * AsEngine::Global_Scale;
    Level_Rect.top = ALevel::Level_Y_Offset * AsEngine::Global_Scale;
    Level_Rect.right = Level_Rect.left + ALevel::Cell_Width * ALevel::Level_Width_01 * AsEngine::Global_Scale;
    Level_Rect.bottom = Level_Rect.top + ALevel::Cell_Height * ALevel::Level_Height_01 * AsEngine::Global_Scale;
    AsEngine::Create_Pen_Brush(255, 255, 255, pen_Letter, brush_Letter, AsEngine::Global_Scale);

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int &next_y_pos, double &ball_Direction)
{// проверка на отражение от кирпичей
    int brick_y_pos = Level_Y_Offset + Level_Height_01 * Cell_Height;

    for (int i = Level_Height_01 - 1; i >= 0; i--)
    {
        for (int j = 0; j < Level_Width_01; j++)
        {
            if (Level_01[i][j] == 0)
                continue;

            if (next_y_pos < brick_y_pos)
            {
                next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
                ball_Direction =  -ball_Direction;
            }

        }

        brick_y_pos -= Cell_Height;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type, AsEngine *engine)
{//вывод кирпича
    HPEN pen;
    HBRUSH brush;

    switch (brick_type)
    {
    case EBT_None:
        return;

    case EBT_Red:
        pen = engine->pen_red;
        brush = engine->brush_red;
        break;

    case EBT_Blue:
        pen = engine->pen_blue;
        brush = engine->brush_blue;
        break;

    default:
        return;
    }

    SelectObject(hdc, pen);
    SelectObject(hdc, brush);
    RoundRect(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + Brick_Width) * AsEngine::Global_Scale, (y + Brick_Height) * AsEngine::Global_Scale, 2*AsEngine::Global_Scale, 2*AsEngine::Global_Scale);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Set_Brick_Letter(HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush, bool is_switch_color, AsEngine *engine)
{
    if (is_switch_color)
    {
        front_pen = engine->pen_red;
        front_brush = engine->brush_red;

        back_pen = engine->pen_blue;
        back_brush = engine->brush_blue;
    }
    else
    {
        front_pen = engine->pen_blue;
        front_brush = engine->brush_blue;

        back_pen = engine->pen_red;
        back_brush = engine->brush_red;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step, AsEngine *engine)
{
    bool switch_color;
    double offset;
    int brick_half_height = Brick_Height * AsEngine::Global_Scale / 2;
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
    Set_Brick_Letter(front_pen, front_brush, back_pen, back_brush, switch_color, engine);

    if (rotation_step == 4 || rotation_step == 12)
    {
        // выводим фон
        SelectObject(hdc, back_pen);                                                            
        SelectObject(hdc, back_brush);

        Rectangle(hdc, x, y +brick_half_height - AsEngine::Global_Scale, x + Brick_Width * AsEngine::Global_Scale, y + brick_half_height);

        // выводим перед
        SelectObject(hdc, front_pen);                                                            
        SelectObject(hdc, front_brush);

        Rectangle(hdc, x, y +brick_half_height, x + Brick_Width * AsEngine::Global_Scale, y +brick_half_height + AsEngine::Global_Scale - 1);
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

        offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsEngine::Global_Scale;
        back_part_offset = (int)round(offset);
        Rectangle(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * AsEngine::Global_Scale, brick_half_height - back_part_offset);

        // выводим перед
        SelectObject(hdc, front_pen);                                                            
        SelectObject(hdc, front_brush);
        Rectangle(hdc, 0, -brick_half_height, Brick_Width * AsEngine::Global_Scale, brick_half_height);

        if (rotation_step > 4 && rotation_step <= 12)
        {
            switch (letter_type)
            {
            case ELT_O:
                SelectObject(hdc, pen_Letter);
                Ellipse(hdc, 0 + 5 * AsEngine::Global_Scale, (-5 * AsEngine::Global_Scale) / 2, 0 + 10 * AsEngine::Global_Scale, 5 * AsEngine::Global_Scale / 2);
                break;

            default:
                break;
            }
        }

        SetWorldTransform(hdc, &old_xform);
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{//вывод уровня
    RECT intersection_rect;

    if(! IntersectRect(&intersection_rect, &paint_area, &Level_Rect)) 
        return;

    for (int i = 0; i < Level_Height_01; i++)
    {
        for (int j = 0; j < Level_Width_01; j++)
        {
            Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j], engine);
        }
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




//APlatform
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform(void) 
    : Inner_width(21), X_Pos(AsBorder::Border_X_Offset), X_Step(AsEngine::Global_Scale * 2), Width(28)
{   }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Init(void)
{
    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(AsEngine *engine)
{
    Prev_Platform_Rect = Platform_Rect;

    Platform_Rect.left = X_Pos * AsEngine::Global_Scale;
    Platform_Rect.top = Y_Pos * AsEngine::Global_Scale;
    Platform_Rect.right = Platform_Rect.left + Width* AsEngine::Global_Scale;
    Platform_Rect.bottom = Platform_Rect.top + Height* AsEngine::Global_Scale;

    InvalidateRect(engine->hwnd, &Prev_Platform_Rect, FALSE);
    InvalidateRect(engine->hwnd, &Platform_Rect, FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, AsEngine *engine, RECT &paint_area)
{//отрисовка платформы
    RECT intersection_rect;
    int x = X_Pos;
    int y = Y_Pos;

    if(! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect)) 
        return;

    SelectObject(hdc, engine->pen_bg);
    SelectObject(hdc, engine->brush_bg);
    Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

    //1. Рисуем боковые шарики
    SelectObject(hdc, engine->pen_red);
    SelectObject(hdc, engine->brush_red);
    Ellipse(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + Circle_size) * AsEngine::Global_Scale, (y + Circle_size) * AsEngine::Global_Scale);
    Ellipse(hdc, (x + Inner_width) * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + Circle_size + Inner_width) * AsEngine::Global_Scale, (y + Circle_size) * AsEngine::Global_Scale);

    //2. Рисуем блик
    SelectObject(hdc, engine->pen_white);
    Arc(hdc, (x + 1) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale, (x + Circle_size - 1) * AsEngine::Global_Scale, (y + Circle_size - 1) * AsEngine::Global_Scale,
        (x + 1 + 1) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale, (x + 1) * AsEngine::Global_Scale, (y + 1 + 2) * AsEngine::Global_Scale);

    //3. Рисуем среднюю часть
    SelectObject(hdc, engine->pen_blue);
    SelectObject(hdc, engine->brush_blue);
    RoundRect(hdc, (x + 4) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale, (x + 4 + Inner_width - 1) * AsEngine::Global_Scale, (y + 1 + 5) * AsEngine::Global_Scale, 3 * AsEngine::Global_Scale, 3 * AsEngine::Global_Scale);

    return;

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//AsBorder
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder(void)
{   }  
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Init(void)
{
    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border, AsEngine *engine)
{// отрисовка элемента границы уровня (tile)

    switch (is_top_border)
    {
    case false:
        // основная линия 
        SelectObject(hdc,   engine->pen_blue);
        SelectObject(hdc, engine->brush_blue);
        Rectangle(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + 4) * AsEngine::Global_Scale, (y + 4) * AsEngine::Global_Scale);

        // белая окантовка
        SelectObject(hdc,   engine->pen_white);
        SelectObject(hdc, engine->brush_white);
        Rectangle(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + 1) * AsEngine::Global_Scale, (y + 4) * AsEngine::Global_Scale);

        // черная точка, перфорация
        SelectObject(hdc,   engine->pen_bg);
        SelectObject(hdc, engine->brush_bg);
        Rectangle(hdc, (x + 2) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale, (x + 3) * AsEngine::Global_Scale, (y + 2) * AsEngine::Global_Scale);
        break;

    case true:
        // основная линия 
        SelectObject(hdc,   engine->pen_blue);
        SelectObject(hdc, engine->brush_blue);
        Rectangle(hdc, x * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale, (x + 4) * AsEngine::Global_Scale, (y + 4) * AsEngine::Global_Scale);

        // белая окантовка
        SelectObject(hdc,   engine->pen_white);
        SelectObject(hdc, engine->brush_white);
        Rectangle(hdc, x * AsEngine::Global_Scale, y * AsEngine::Global_Scale, (x + 4) * AsEngine::Global_Scale, (y + 1) * AsEngine::Global_Scale);

        // черная точка, перфорация
        SelectObject(hdc,   engine->pen_bg);
        SelectObject(hdc, engine->brush_bg);
        Rectangle(hdc, (x + 2) * AsEngine::Global_Scale, (y + 2) * AsEngine::Global_Scale, (x + 3) * AsEngine::Global_Scale, (y + 3) * AsEngine::Global_Scale);
        break;
    deafault:
        break;
    }

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsBorder::Draw(HDC hdc, AsEngine *engine)
{// отрисовка всех границ уровня

 //1. левая граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 2, i*4 + 1, false, engine);
    }

    //2. правая граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, 201, i*4 + 1, false, engine);
    }

    //3. верхняя граница
    for (int i = 0; i < 50; i++)
    {
        Draw_Element(hdc, i*4 + 3, 0, true, engine);
    }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




//AsEngine
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine(void) 
{   }
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Init_Engine(HWND hWnd)
{//задает значения цветов для разных констант
    hwnd = hWnd;

    Create_Pen_Brush(255, 85, 85, pen_red, brush_red, 0);
    Create_Pen_Brush(85, 225, 225, pen_blue, brush_blue, 0);
    Create_Pen_Brush(255, 255, 255, pen_white, brush_white, 0);
    Create_Pen_Brush(15, 63, 31, pen_bg, brush_bg, 0);

    Level.Init();
    Ball.Init();
    Platform.Init();
    Border.Init();

    Platform.Redraw_Platform(this);

    SetTimer(hWnd, Timer_ID, 50, 0);

    // начальное положение мячика
    //Ball.Ball_Rect.left   = (Level_X_Offset + Ball.Ball_X_Pos) * Global_Scale;
    //Ball.Ball_Rect.top    = (Level_Y_Offset + Ball.Ball_Y_Pos) * Global_Scale;
    //Ball.Ball_Rect.right  = Ball.Ball_Rect.left + Ball.Ball_Size * Global_Scale;
    //Ball.Ball_Rect.bottom = Ball.Ball_Rect.top + Ball.Ball_Size * Global_Scale;

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Отрисовка экрана игры

    Level.Draw(hdc, paint_area, this);

    Platform.Draw(hdc, this, paint_area);

    /*for (int i = 0; i < 16; i++)
    {
    Draw_Brick_Letter(hdc, 20 + (i * Cell_Width * Global_Scale), 100, EBT_Blue, ELT_O, i);
    Draw_Brick_Letter(hdc, 20 + (i * Cell_Width * Global_Scale), 130, EBT_Red, ELT_O, i);
    }*/

    Ball.Draw(hdc, paint_area, this);

    Border.Draw(hdc, this);

    return;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Key_Down(EKey_Type key_type)
{
    switch (key_type)
    {
    case EKT_Left:
        Platform.X_Pos -= Platform.X_Step;

        if (Platform.X_Pos <= AsBorder::Border_X_Offset)
            Platform.X_Pos = AsBorder::Border_X_Offset;

        Platform.Redraw_Platform(this);
        break;

    case EKT_Right:
        Platform.X_Pos += Platform.X_Step;

        if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
            Platform.X_Pos = Max_X_Pos - Platform.Width + 1;

        Platform.Redraw_Platform(this);
        break;

    case EKT_Space:
        break;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AsEngine::On_Timer()
{
    Ball.Move(this, &Level, &Platform);
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush, int width)
{
    pen = CreatePen(PS_SOLID, width, RGB(r, g, b));     //pen
    brush = CreateSolidBrush(RGB(r, g, b));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

