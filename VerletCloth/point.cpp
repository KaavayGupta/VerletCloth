#include "point.h"

Point::Point(float x, float y, float oldX, float oldY, bool isPinned)
{
    this->x = x;
    this->y = y;
    this->oldX = oldX;
    this->oldY = oldY;
    this->m_IsPinned = isPinned;
}
