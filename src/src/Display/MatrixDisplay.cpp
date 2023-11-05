#include <Arduino.h>

#include "AsyncDelay.h"

#include "Pinout.h"
#include "Display\MatrixDisplay.h"

namespace MatrixDisplay
{

  int pic[8];

  void Clear(bool state = false)
  {
    for (int i = 0; i < 8; i++)
    {
      pic[i] = state ? 255 : 0;
    }
  }

  AsyncDelay del;

  void Init()
  {
    pinMode(PIN_MatrixDATA, OUTPUT);
    pinMode(PIN_MatrixCLOCK, OUTPUT);
    pinMode(PIN_MatrixLATCH, OUTPUT);

    Clear();
    del.start(2000, AsyncDelay::MILLIS);
  }

  void TriggerLatch()
  {
    digitalWrite(PIN_MatrixLATCH, HIGH);
    //delayMicroseconds(10);
    digitalWrite(PIN_MatrixLATCH, LOW);
    //delayMicroseconds(10);
  }




  void Scan()
  {

    for (int i = 0; i < 8; i++)
    {
      shiftOut(PIN_MatrixDATA, PIN_MatrixCLOCK, LSBFIRST, ~pic[i]);
      shiftOut(PIN_MatrixDATA, PIN_MatrixCLOCK, LSBFIRST, 128 >> i);
      TriggerLatch();
    }

  }

  void SetPixel(int X, int Y, bool state)
  {
    if (X < 0 || X > 7 || Y < 0 || Y > 7)
      return;

    if (state)
    {
      pic[Y] |= 1 << X;
    }
    else
    {
      pic[Y] &= ~(1 << X);
    }
  }

  void SetLine(int Y, int value)
  {
    if (Y < 0 || Y > 7)
      return;
    if (value < 0 || value > 255)
      return;


    pic[Y] = value;
  }

  inline void PlayDropdownFade()
  {
    const int speed = 50;

    AsyncDelay del;

    for(int Y = 0; Y < 8; Y++)
    {
      SetLine(Y, 255);
      del.start(speed, AsyncDelay::MILLIS);
      while(!del.isExpired()) Scan();
    }
  }

  AsyncDelay extraDelay;

  void PlayAnimation(Animations anim, int extraDelayMS = 0)
  {
    extraDelay.start(extraDelayMS, AsyncDelay::MILLIS);

    switch (anim)
    {
      
    case DropdownFade:
    PlayDropdownFade();
      break;
    
    default:
      break;
    }

    while (!extraDelay.isExpired()) Scan();
    
  }
}