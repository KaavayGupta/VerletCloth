#include "stick.h"

#include "utils.h"

Stick::Stick(Point* p0, Point* p1, bool isHidden)
{
    this->m_p0 = p0;
    this->m_p1 = p1;
    this->m_IsHidden = isHidden;
    m_Length = GetDistance(*p0, *p1);
    m_Color = olc::WHITE;
}