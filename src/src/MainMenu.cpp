#include <Arduino.h>

#include "InputManager.h"
#include "Display\MatrixDisplay.h"

namespace MainMenu
{
    const unsigned int thumbnail_0[] =
    {
        0b01011110,
        0b00000000,
        0b00000000
    };

    const unsigned int thumbnail_0_selected[] =
    {
        0b01011110,
        0b00000000,
        0b00111100
    };


    const unsigned int thumbnail_1[] =
    {
        0b00000000,
        0b00000001,
        0b10101011,
        0b00000001
    };

    const unsigned int thumbnail_1_selected[] =
    {
        0b00111100,
        0b00000001,
        0b10101011,
        0b00000001
    };


    int currentSelected = 0;


    void DrawThumbnail(int thumbnailIndex, int startY, bool isSelected)
    {
        if(thumbnailIndex == 0) for(int i = 0; i < 3; i++)
        {
            int line =  isSelected ? thumbnail_0_selected[i] : thumbnail_0[i];

            
            MatrixDisplay::SetLine(startY + i, line);
        }
        else if(thumbnailIndex == 1) for(int i = 0; i < 4; i++)
        {
            int line =  isSelected ? thumbnail_1_selected[i] : thumbnail_1[i];

            MatrixDisplay::SetLine(startY + i, line);
        }
    }

    void DrawSnake(bool selected)
    {
    }

    void DrawMeteorGame(bool selected)
    {
    }

    void Draw()
    {
        MatrixDisplay::Clear();

        DrawThumbnail(0, 0, currentSelected == 0);
        DrawThumbnail(1, 4, currentSelected == 1);
        MatrixDisplay::SetLine(3, 255);
    }

    int GetGameIndex()
    {
        Draw();
        while(true)
        {
            char dir = InputManager::GetPressedDirection();
            
            if(dir == 'u')
            {
                currentSelected = 0;
                Draw();
            }
            else if (dir == 'd')
            {
                currentSelected = 1;
                Draw();
            }
            else if (dir == 'r')
            {
                return currentSelected;
            }
            

            MatrixDisplay::Scan();
        }
    }
}