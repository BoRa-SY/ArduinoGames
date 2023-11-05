#include <Arduino.h>

#include "Display\MatrixDisplay.h"

#include "InputManager.h"

#include "AsyncDelay.h"

#include "Pinout.h"

#include "CoordinateSystem.h"

namespace SnakeGame
{
    bool gameOngoing = true;

    const int speed = 250;

    const Size GridSize = MatrixDisplay::GridSize;

    int snakeCellCount;
    Point Snake[64];

    Point FoodPoint;

    char direction = 'r';

    AsyncDelay mainDelay;

    void UpdateDisplay(bool playerLost = false)
    {
        MatrixDisplay::Clear();

        if (playerLost)
        {
            MatrixDisplay::PlayAnimation(MatrixDisplay::DropdownFade, 2000);
            return;
        }

        for (int i = 0; i < snakeCellCount; i++)
        {
            Point p = Snake[i];
            MatrixDisplay::SetPixel(p.X, p.Y, true);
        }

        MatrixDisplay::SetPixel(FoodPoint.X, FoodPoint.Y, true);
    }

    void createFood()
    {
        Point food = {random(0, GridSize.Width), random(0, GridSize.Height)};

        for (int i = 0; i < snakeCellCount; i++)
        {
            Point snakeCell = Snake[i];

            if (snakeCell.X == food.X && snakeCell.Y == food.Y)
            {
                createFood();
                return;
            }
        }

        FoodPoint = food;
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

        if (!IsInBounds(newHead))
        {
            gameOngoing = false;
            UpdateDisplay(true);

            return;
        }

        Point lastCellPoint;

        for (int i = snakeCellCount - 1; i > 0; i--)
        {
            Snake[i] = Snake[i - 1];

            if (i == snakeCellCount - 1)
                lastCellPoint = Snake[i];
        }

        Snake[0] = newHead;

        if (newHead.X == FoodPoint.X && newHead.Y == FoodPoint.Y)
        {
            Snake[snakeCellCount] = lastCellPoint;
            snakeCellCount++;
            createFood();
        }

        UpdateDisplay();
    }

    void Update()
    {
        if (!gameOngoing)
            return;
        char drc = InputManager::GetPressedDirection();
        if (drc != 'N')
            direction = drc;

        if (!mainDelay.isExpired())
            return;

        MainLogic();

        mainDelay.start(speed, AsyncDelay::MILLIS);
    }

    void StartGame()
    {
        snakeCellCount = 2;
        Snake[0] = {1, 0};
        Snake[1] = {0, 0};
        direction = 'r';

        createFood();

        UpdateDisplay();

        gameOngoing = true;

        mainDelay.start(speed, AsyncDelay::MILLIS);

        while (gameOngoing)
        {
            Update();
            MatrixDisplay::Scan();
        }
    }
}
