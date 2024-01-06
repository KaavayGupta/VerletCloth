#include "engine.h"

void Engine::Engine::ConstructPoints()
{
    float length = (numPointsX - 1) * pointDistance;
    float percent = (ScreenWidth() - length) / (float)ScreenWidth();
    percent *= 0.5f;
    float offsetX = percent * ScreenWidth();

    for (int i = 0; i < numPointsY; i++)
    {
        for (int j = 0; j < numPointsX; j++)
        {
            float x = (j * pointDistance) + offsetX;
            float y = i * pointDistance;
            points.push_back(Point(x, y, x, y, i == 0 && j % pinnedPointStep == 0));
        }
    }
}

void Engine::ConstructSticks()
{
    for (int i = 0; i < numPointsY; i++)
    {
        for (int j = 0; j < numPointsX; j += 2)
        {
            int index = numPointsX * i + j;
            if (j + 1 < numPointsX)
            {
                sticks.push_back(Stick(&points[index], &points[index + 1]));
            }
            if (j - 1 > 0)
            {
                sticks.push_back(Stick(&points[index], &points[index - 1]));
            }
        }
    }

    for (int i = 0; i < numPointsY; i += 2)
    {
        for (int j = 0; j < numPointsX; j++)
        {
            int index = numPointsX * i + j;
            if (i + 1 < numPointsY)
            {
                sticks.push_back(Stick(&points[index], &points[index + numPointsX]));
            }
            if (i - 1 > 0)
            {
                sticks.push_back(Stick(&points[index], &points[index - numPointsX]));
            }
        }
    }
}

void Engine::UpdatePoints(float deltaT)
{
    float deltaTSquared = deltaT * deltaT;

    for (auto& p : points)
    {
        if (!p.m_IsPinned)
        {
            float vx = (p.x - p.oldX) * friction;
            float vy = (p.y - p.oldY) * friction + gravity * deltaT;

            p.oldX = p.x;
            p.oldY = p.y;

            p.x += vx + vx * deltaTSquared;
            p.y += vy + vy * deltaTSquared;
        }
    }
}

void Engine::ConstrainPoints(float deltaT)
{
    float deltaTSquared = deltaT * deltaT; // Square deltaT for verlet integration

    for (auto& p : points)
    {
        if (!p.m_IsPinned)
        {
            float vx = (p.x - p.oldX) * friction;
            float vy = (p.y - p.oldY) * friction;

            if (p.x > ScreenWidth())
            {
                p.x = ScreenWidth();
                p.oldX = p.x + (vx * bounce * deltaTSquared); // Adjust for deltaT
            }
            else if (p.x < 0)
            {
                p.x = 0;
                p.oldX = p.x + (vx * bounce * deltaTSquared); // Adjust for deltaT
            }
            if (p.y > ScreenHeight())
            {
                p.y = ScreenHeight();
                p.oldY = p.y + (vy * bounce * deltaTSquared); // Adjust for deltaT
            }
            else if (p.y < 0)
            {
                p.y = 0;
                p.oldY = p.y + (vy * bounce * deltaTSquared); // Adjust for deltaT
            }
        }
    }
}

void Engine::UpdateSticks()
{
    for (int i = 0; i < sticks.size(); i++)
    {
        Stick* s = &sticks[i];
        float dx = s->m_p1->x - s->m_p0->x;
        float dy = s->m_p1->y - s->m_p0->y;
        float distance = std::sqrt(dx * dx + dy * dy);
        float difference = s->m_Length - distance;
        float percent = difference / distance / 2.0f;
        float offsetX = dx * percent;
        float offsetY = dy * percent;

        if (!s->m_p0->m_IsPinned)
        {
            s->m_p0->x -= offsetX;
            s->m_p0->y -= offsetY;
        }
        if (!s->m_p1->m_IsPinned)
        {
            s->m_p1->x += offsetX;
            s->m_p1->y += offsetY;
        }
    }
}

void Engine::RenderPoints()
{
    for (int i = 0; i < points.size(); i++)
    {
        Point* p = &points[i];
        FillCircle(p->x, p->y, 2, olc::WHITE);
    }
}

void Engine::RenderSticks()
{
    for (const auto& s : sticks)
    {
        if (!s.m_IsHidden)
            DrawLine(s.m_p0->x, s.m_p0->y, s.m_p1->x, s.m_p1->y, s.m_Color);
    }
}

bool Engine::IsMouseIntersectingStick(Stick* s)
{
    float left = s->m_p0->x > s->m_p1->x ? s->m_p1->x : s->m_p0->x;
    float right = s->m_p0->x > s->m_p1->x ? s->m_p0->x : s->m_p1->x;
    float up = s->m_p0->y > s->m_p1->y ? s->m_p0->y : s->m_p1->y;
    float down = s->m_p0->y > s->m_p1->y ? s->m_p1->y : s->m_p0->y;
    bool isHorizontal = up - down <= 10.0f;
    if (isHorizontal)
    {
        if (GetMouseX() >= left && GetMouseX() <= right
            && GetMouseY() >= up - mouseDetectionThreshold && GetMouseY() <= down + mouseDetectionThreshold)
        {
            return true;
        }
    }
    else
    {
        if (GetMouseY() >= down && GetMouseY() <= up
            && GetMouseX() >= left - mouseDetectionThreshold && GetMouseX() <= right + mouseDetectionThreshold)
        {
            return true;
        }
    }

    return false;
}

void Engine::Reload()
{
    sticks.clear();
    points.clear();
    ConstructPoints();
    ConstructSticks();
}

void Engine::HandleStatChanges()
{
    if (GetKey(olc::Key::D).bPressed)
    {
        numPointsX++;
        Reload();
    }
    if (GetKey(olc::Key::A).bPressed)
    {
        numPointsX--;
        Reload();
    }
    if (GetKey(olc::Key::W).bPressed)
    {
        numPointsY--;
        Reload();
    }
    if (GetKey(olc::Key::S).bPressed)
    {
        numPointsY++;
        Reload();
    }
    if (GetKey(olc::Key::Q).bPressed)
    {
        pointDistance--;
        Reload();
    }
    if (GetKey(olc::Key::E).bPressed)
    {
        pointDistance++;
        Reload();
    }

    if (GetKey(olc::Key::I).bPressed)
        gravity += 0.1f;
    if (GetKey(olc::Key::K).bPressed)
        gravity = std::max(0.0f, gravity - 0.1f);

    if (GetKey(olc::Key::L).bPressed)
        friction += 0.01f;
    if (GetKey(olc::Key::J).bPressed)
        friction = std::max(0.0f, friction - 0.01f);

    if (GetKey(olc::Key::O).bPressed)
        bounce += 0.01f;
    if (GetKey(olc::Key::U).bPressed)
        bounce = std::max(0.0f, bounce - 0.01f);

    if (GetKey(olc::Key::Z).bPressed)
    {
        pinnedPointStep = std::max(1, pinnedPointStep - 1);
        Reload();
    }
    if (GetKey(olc::Key::C).bPressed)
    {
        pinnedPointStep = std::min(pinnedPointStep + 1, numPointsX - 1);
        Reload();
    }
}

void Engine::HandleStickInteractions()
{
    if (GetMouse(0).bHeld)
    {
        auto it = sticks.begin();
        while (it != sticks.end())
        {
            if (IsMouseIntersectingStick(&*it))
            {
                it = sticks.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    if (GetMouse(1).bPressed)
    {
        for (auto& s : sticks)
        {
            if (IsMouseIntersectingStick(&s))
            {
                selectedStick = &s;
            }
        }
    }
    if (GetMouse(1).bHeld)
    {
        if (selectedStick != nullptr)
        {
            selectedStick->m_p0->x = selectedStick->m_p0->oldX = GetMouseX();
            selectedStick->m_p0->y = selectedStick->m_p0->oldY = GetMouseY();
        }
    }
    if (GetMouse(1).bReleased)
    {
        selectedStick = nullptr;
    }
}