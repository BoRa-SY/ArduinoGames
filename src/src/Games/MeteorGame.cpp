#include <Arduino.h>

#include "Display\MatrixDisplay.h"

#include "InputManager.h"

#include "AsyncDelay.h"

#include "Pinout.h"

#include "CoordinateSystem.h"

namespace MeteorGame
{

    const Size GridSize = MatrixDisplay::GridSize;

    const int bulletSpeed = 100;
    const int meteorSpeed = 500;
    const int meteorSpawnSpeed = 1000;

    Point meteors[3];
    bool meteorFired[3];

    Point bullets[5];
    AsyncDelay bulletDelays[5];
    bool bulletFired[5];

    Point gunPoint = {3, 6};

    bool gameOngoing = true;

    void UpdateDisplay()
    {
        MatrixDisplay::Clear();

        // Gun

        MatrixDisplay::SetPixel(gunPoint.X, gunPoint.Y, true);
        MatrixDisplay::SetPixel(gunPoint.X, gunPoint.Y + 1, true);
        MatrixDisplay::SetPixel(gunPoint.X - 1, gunPoint.Y + 1, true);
        MatrixDisplay::SetPixel(gunPoint.X + 1, gunPoint.Y + 1, true);

        // Bullets

        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                continue;
            Point bullet = bullets[i];
            MatrixDisplay::SetPixel(bullet.X, bullet.Y, true);
        }

        // Meteors

        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i]) continue;
            Point meteor = meteors[i];
            MatrixDisplay::SetPixel(meteor.X, meteor.Y, true);
        }

    }

    inline int GetEmptyBulletSlot()
    {
        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                return i;
        }

        return -1;
    }

    inline void CreateBullet()
    {
        int emptySlot = GetEmptyBulletSlot();
        if (emptySlot == -1) return;

        bulletFired[emptySlot] = true;
        bullets[emptySlot] = {gunPoint.X, gunPoint.Y - 1};
        bulletDelays[emptySlot].start(bulletSpeed, AsyncDelay::MILLIS);
    }

    inline void MoveBullets()
    {
        for (int i = 0; i < 5; i++)
        {
            if (!bulletFired[i])
                continue;
            if (!bulletDelays[i].isExpired())
                continue;
            bulletDelays[i].start(bulletSpeed, AsyncDelay::MILLIS);
            bullets[i].Y--;
            if (bullets[i].Y < 0)
            {
                bulletFired[i] = false;
            }
        }
    }

    inline int GetEmptyMeteorSlot()
    {
        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i])
                return i;
        }

        return -1;
    }

    AsyncDelay meteorSpawnDelay;
    inline void CreateMeteor()
    {
        if(!meteorSpawnDelay.isExpired()) return;
        meteorSpawnDelay.start(meteorSpawnSpeed, AsyncDelay::MILLIS);

        int emptySlot = GetEmptyMeteorSlot();
        if (emptySlot == -1) return;

        meteorFired[emptySlot] = true;
        meteors[emptySlot] = {(int)random(0, 8), 0};
    }

    AsyncDelay meteorMoveDelay;

    inline void MoveMeteors()
    {        
        if(!meteorMoveDelay.isExpired()) return;
        meteorMoveDelay.start(meteorSpeed, AsyncDelay::MILLIS);

        for (int i = 0; i < 3; i++)
        {
            if (!meteorFired[i]) continue;

            meteors[i].Y++;

            if (meteors[i].Y > GridSize.Height - 1)
            {
                meteorFired[i] = false;

                gameOngoing = false;
            }
        }
    }

    inline void CheckCollision()
    {
        for(int i = 0; i < 3; i++)
        {
            if(!meteorFired[i]) continue;

            for(int j = 0; j < 5; j++)
            {
                if(!bulletFired[j]) continue;

                if(meteors[i].X == bullets[j].X && meteors[i].Y == bullets[j].Y)
                {
                    meteorFired[i] = false;
                    bulletFired[j] = false;
                }
            }
        }
    }

    void Update()
    {

        char drc = InputManager::GetPressedDirection();
        switch (drc)
        {
        case 'l':
            if (gunPoint.X > 0)
            {
                gunPoint.X--;
            }
            break;

        case 'r':
            if (gunPoint.X < GridSize.Width - 1)
            {
                gunPoint.X++;
            }
            break;

        case 'u':
            CreateBullet();
            break;
        }
    }

    void StartGame()
    {
        gameOngoing = true;

        UpdateDisplay();

        for(int i = 0; i<3; i++)
        {
            meteorFired[i] = false;
        }

        for(int i = 0; i<5; i++)
        {
            bulletFired[i] = false;
        }



        while (gameOngoing)
        {
            MoveBullets();

            CheckCollision();

            MoveMeteors();

            CreateMeteor();

            Update();

            UpdateDisplay();

            MatrixDisplay::Scan();
        }

        MatrixDisplay::PlayAnimation(MatrixDisplay::Animations::DropdownFade, 2000);
    }
}