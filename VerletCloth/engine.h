#pragma once

#include "olcPixelGameEngine.h"
#include "point.h"
#include "stick.h"


class Engine : public olc::PixelGameEngine
{
private:
    float bounce = 1.0f;
    float gravity = 95.0f;
    float friction = 0.8f;
    float pointDistance = 15.0f;
    float mouseDetectionThreshold = 5.0f;
    int numPointsX = 80;
    int numPointsY = 35;
    int pinnedPointStep = 1;    // Steps to pin points after (1 = every 1 point; 2 = every 2 points etc)
    Stick* selectedStick;
    std::vector<Point> points;
    std::vector<Stick> sticks;

private:
    void ConstructPoints();
    void ConstructSticks();
    void UpdatePoints(float deltaT);
    void ConstrainPoints(float deltaT);
    void UpdateSticks();
    void RenderPoints();
    void RenderSticks();
    bool IsMouseIntersectingStick(Stick* s);
    void Reload();
    void HandleStatChanges();
    void HandleStickInteractions();

public:
    Engine()
    {
        sAppName = "Cloth Simulator";
    }

public:
    bool OnUserCreate() override
    {
        ConstructPoints();
        ConstructSticks();

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::R).bPressed)
        {
            Reload();
        }

        HandleStatChanges();
        HandleStickInteractions();

        UpdatePoints(fElapsedTime);
        UpdateSticks();
        for (int i = 0; i < 3; i++)
        {
            ConstrainPoints(fElapsedTime);
        }

        Clear(olc::BLACK);
        RenderSticks();
        DrawString(10, 0, std::to_string(GetFPS()));
        DrawString(10, ScreenHeight() - 50, "Pin Every [C+/Z-]: " + std::to_string(pinnedPointStep), olc::YELLOW);
        DrawString(10, ScreenHeight() - 40, "NumPointsX [D+/A-]: " + std::to_string(numPointsX), olc::YELLOW);
        DrawString(10, ScreenHeight() - 30, "NumPointsY [S+/W-]: " + std::to_string(numPointsY), olc::YELLOW);
        DrawString(10, ScreenHeight() - 20, "PointDistance [E+/Q-]: " + std::to_string(pointDistance), olc::YELLOW);
        DrawString(10, ScreenHeight() - 10, "R: Reload", olc::YELLOW);
        DrawString(ScreenWidth() - 250, ScreenHeight() - 40, "Gravity [I+/K-]: " + std::to_string(gravity), olc::YELLOW);
        DrawString(ScreenWidth() - 250, ScreenHeight() - 30, "Friction [L+/J-]: " + std::to_string(friction), olc::YELLOW);
        DrawString(ScreenWidth() - 250, ScreenHeight() - 20, "Bounce [O+/U-]: " + std::to_string(bounce), olc::YELLOW);

        return true;
    }
};
