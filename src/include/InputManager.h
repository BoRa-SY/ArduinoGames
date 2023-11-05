#pragma once

#include <Arduino.h>

namespace InputManager
{
    void Init();

    bool GetButtonState(uint8_t pin);

    char GetPressedDirection();


    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    bool GetDirection(Direction dir);
}