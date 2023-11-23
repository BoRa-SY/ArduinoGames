#include <Arduino.h>

#include "AsyncDelay.h"

#include "Display\MatrixDisplay.h"

#include "InputManager.h"

#include "Pinout.h"

#include "Games\SnakeGame.h"
#include "Games\MeteorGame.h"

#include "MainMenu.h"

void setup() {
  Serial.begin(9600);
  
  MatrixDisplay::Init();
  InputManager::Init();
}

void loop() {

  for(int i = 0; i<8; i++)
  {
    MatrixDisplay::SetLine(i, 255);
  }
  MatrixDisplay::SetPixel(7,7, false);
  MatrixDisplay::Scan();
  return;
  int game = MainMenu::GetGameIndex();
  
  switch (game)
  {
    case 0:
      SnakeGame::StartGame();
      break;
    case 1:
      MeteorGame::StartGame();
      break;
  }
}