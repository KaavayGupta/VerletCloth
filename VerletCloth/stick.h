#pragma once

#include "olcPixelGameEngine.h"
#include "point.h"

class Stick
{
public:
    Stick(Point* p0, Point* p1, bool isHidden = false);

public:
    Point* m_p0;
    Point* m_p1;
    bool m_IsHidden;
    float m_Length;
    olc::Pixel m_Color;
};