#pragma once

#define _USE_MATH_DEFINES

#include <Windows.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
    static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush, int width);

    static const int Global_Scale = 3;

    static const int Cell_Width = 16;
    static const int Cell_Height = 8;
    static const int Level_Height_01 = 14;    // высота уровня в ячейках
    static const int Level_Width_01 = 12;     // ширина уровня в ячейках
    static const int Level_X_Offset = 8;
    static const int Level_Y_Offset = 6;


    static const int Height = 7;
    static const int Y_Pos = 185;
    static const int Circle_size = 7;

    static const int Ball_Size = 4;

    static const int Border_X_Offset = 6;
    static const int Border_Y_Offset = 4;

    static const int Max_Y_Pos = 195 - Ball_Size;
    static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width_01;

};
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------