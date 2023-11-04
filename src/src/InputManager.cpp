#include <Arduino.h>

#include "Pinout.h"
namespace InputManager
{
    void Init()
    {
        pinMode(PIN_ButtonUP, INPUT_PULLUP);
        pinMode(PIN_ButtonDOWN, INPUT_PULLUP);
        pinMode(PIN_ButtonLEFT, INPUT_PULLUP);
        pinMode(PIN_ButtonRIGHT, INPUT_PULLUP);
    }

    bool GetButtonState(uint8_t pin)
    {
        return digitalRead(pin) == LOW;
    }

    

    bool lastState_U = false;
    bool lastState_D = false;
    bool lastState_L = false;
    bool lastState_R = false;

    char GetPressedDirection()
    {
        bool UpState = GetButtonState(PIN_ButtonUP);
        if (UpState)
        {
            if (lastState_U == UpState) return 'N';
            lastState_U = UpState;
            return 'u';
        }
        lastState_U = UpState;


        
        bool DownState = GetButtonState(PIN_ButtonDOWN);
        if (DownState)
        {
            if (lastState_D == DownState) return 'N';
            lastState_D = DownState;
            return 'd';
        }
        lastState_D = DownState;


        
        bool LeftState = GetButtonState(PIN_ButtonLEFT);
        if (LeftState)
        {
            if (lastState_L == LeftState) return 'N';
            lastState_L = LeftState;
            return 'l';
        }
        lastState_L = LeftState;


        
        bool RightState = GetButtonState(PIN_ButtonRIGHT);
        if (RightState)
        {
            if (lastState_R == RightState) return 'N';
            lastState_R = RightState;
            return 'r';
        }
        lastState_R = RightState;
        
        return 'N';
    }
}