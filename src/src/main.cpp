#include <Arduino.h>

#include "AsyncDelay.h"

#include "Display\MatrixDisplay.h"

#include "InputManager.h"

#include "Pinout.h"

#include "SnakeGame.h"

void setup() {
  Serial.begin(9600);
  
  MatrixDisplay::Init();
  InputManager::Init();
  SnakeGame::Init();
}

void loop() {
  SnakeGame::Update();
  MatrixDisplay::Scan();
}