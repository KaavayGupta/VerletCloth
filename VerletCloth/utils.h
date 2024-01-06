#pragma once

#include <cmath>
#include "point.h"

inline float GetDistance(float x1, float y1, float x2, float y2) { return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); }
inline float GetDistance(const Point& p1, const Point& p2) { return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)); }
