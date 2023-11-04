#include <Arduino.h>
#include "AntController.h"

AntController::AntController(int16_t screenWidth, int16_t screenHeight, byte numOfAnts){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->numOfAnts = numOfAnts;
    Ant ants[numOfAnts];
    this->ants = ants;
    TFT_ILI9341 temp(screenWidth, screenHeight);
    this->tft = temp;
};
void AntController::init(){
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    randomSeed(analogRead(0));
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].resetAnt(screenWidth, screenHeight, maxSpeed);
    }
};
void AntController::moveAnts(){
  for (byte i = 0; i < numOfAnts; i++){
    for (byte j = 0; j < numOfAnts; j++){
      if(j != i){
        ants[i].detectCollision(ants[j].getCurrentX(), ants[j].getCurrentY(), antDetectRadius);
      }
    }
    ants[i].moveAnt();
    tft.drawCircle(ants[i].getOldX(), ants[i].getOldY(), 2, TFT_BLACK);
    tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, TFT_WHITE);
  }
};