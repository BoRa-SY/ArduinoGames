#include <Arduino.h>

#include "InputManager.h"
#include "Display\MatrixDisplay.h"

namespace MainMenu
{
    const int thumbnailCount = 2;

    const unsigned int thumbnails[2][8] =
    {
        {
            0b11111111,
            0b11111111,
            0b11000000,
            0b11000000,
            0b11000000,
            0b00000000,
            0b11000000,
            0b11000000,
        },
        {
            0b00000000,
            0b00011000,
            0b00011000,
            0b00000000,
            0b00011000,
            0b00011000,
            0b01111110,
            0b01111110,
        }
    };



    void Draw(int thumbnailIndex)
    {
        MatrixDisplay::Clear();

        for(int i = 0; i<8; i++)
        {
            MatrixDisplay::SetLine(i, thumbnails[thumbnailIndex][i]);
        }

    }

    int currentSelected = 0;
    int GetGameIndex()
    {
        while(true)
        {
            char dir = InputManager::GetPressedDirection();
            
            if(dir == 'l')
            {
                if(currentSelected != 0)
                currentSelected--;
                else currentSelected = thumbnailCount - 1;
            }
            else if (dir == 'r')
            {
                if(currentSelected != thumbnailCount - 1)
                currentSelected++;
                else currentSelected = 0;
            } 
            else if (dir == 'u' || dir == 'd') return currentSelected;
            

            Draw(currentSelected);
            MatrixDisplay::Scan();
        }
    }
}