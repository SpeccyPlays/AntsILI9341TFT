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
        ants[i].resetAnt(screenWidth, screenHeight, maxSpeed, boundary);
    }
};
void AntController::moveAnts(){
  for (byte i = 0; i < numOfAnts; i++){
    ants[i].setCurrentPosToOldPos();
    ants[i].checkBoundary(screenWidth, screenHeight, boundary);
    for (byte j = 0; j < numOfAnts; j++){
      if(j != i){
        //ants[i].detectCollision(ants[j].getCurrentX(), ants[j].getCurrentY(), antDetectRadius);
        //ants[i].avoidAnts(ants[j].getCurrentX(), ants[j].getCurrentY(), antDetectRadius);
      }
    }
    if (ants[i].getState() == WANDER){
        ants[i].wandering();
    }
    ants[i].steering();
    ants[i].locomotion();
    tft.drawCircle(ants[i].getOldX(), ants[i].getOldY(), 2, TFT_BLACK);
    tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, TFT_WHITE);
  }
};
void AntController::setToWander(){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setState(WANDER);
    }
};
void AntController::setToSeek(int16_t x, int16_t y){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setState(SEEK);
    }
};