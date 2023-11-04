#include <Arduino.h>

#include "Display\MatrixDisplay.h"

#include "InputManager.h"

#include "AsyncDelay.h"

#include "Pinout.h"

#include "CoordinateSystem.h"



namespace SnakeGame
{

    const int speed = 50;

    const Size GridSize = {8,8};

    int snakeCellCount = 4;
    Point Snake[64];



    char direction = 'r';

    AsyncDelay mainDelay;

    void Display()
    {
        MatrixDisplay::Clear();

        for(int i = 0; i < snakeCellCount; i++)
        {
            Point p = Snake[i];
            MatrixDisplay::SetPixel(p.X, p.Y, true);
        }
    }

    void Init()
    {
        Snake[0] = {3, 0};
        Snake[1] = {2, 0};
        Snake[2] = {1, 0};
        Snake[3] = {0, 0};

        Display();
        mainDelay.start(speed, AsyncDelay::MILLIS);
    }



    bool IsInBounds(Point p)
    {
        return p.X >= 0 && p.X < GridSize.Width && p.Y >= 0 && p.Y < GridSize.Height;
    }

    Point getOffsetByDirection(char drc)
    {
        switch (drc)
        {
        case 'u':
            return {0, -1};
        case 'd':
            return {0, 1};
        case 'l':
            return {-1, 0};
        case 'r':
            return {1, 0};
        default:
            return {0, 0};
        }
    }

    inline void MainLogic()
    {
        Point offset = getOffsetByDirection(direction);
        Point head = Snake[0];
        Point newHead = {head.X + offset.X, head.Y + offset.Y};

        if(!IsInBounds(newHead))
        {
            // Player Lost
            return;
        }


        for(int i = snakeCellCount - 1; i > 0; i--)
        {
            Snake[i] = Snake[i - 1];
        }

        Snake[0] = newHead;

        Display();
    }

    void Update()
    {
        char drc = InputManager::GetPressedDirection();
        if(drc != 'N') direction = drc;

        if (!mainDelay.isExpired()) return;

        MainLogic();

        mainDelay.start(speed, AsyncDelay::MILLIS);
    }

}
