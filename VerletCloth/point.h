#pragma once

class Point
{
public:
    Point(float x, float y, float oldX, float oldY, bool isPinned = false);

public:
    float x, y;
    float oldX, oldY;
    bool m_IsPinned;
};
