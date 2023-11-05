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
void AntController::init(int8_t startSpeed){
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    randomSeed(analogRead(0));
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].resetAnt(screenWidth, screenHeight, startSpeed, boundary);
        tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, TFT_WHITE);
    }
};
void AntController::moveAnts(){
  for (byte i = 0; i < numOfAnts; i++){
    ants[i].setCurrentPosToOldPos();
    ants[i].checkBoundary(screenWidth, screenHeight, boundary);
    int32_t dx = 0;
    int32_t dy = 0;
    uint16_t neighbourAnts = 0;
    for (byte j = 0; j < numOfAnts; j++){
      if(i != j){
        if(ants[i].detectCollision(ants[j].getCurrentX(), ants[j].getCurrentY(), antDetectRadius)){
            dx += (ants[j].getCurrentX() - ants[i].getCurrentX());
            dy += (ants[j].getCurrentY() - ants[i].getCurrentY());
            neighbourAnts ++;
        }
      }
    }
    if (neighbourAnts > 0){
        ants[i].addToVelocityX((dx / neighbourAnts) * avoidanceFactor);
        ants[i].addToVelocityY((dy / neighbourAnts) * avoidanceFactor);
    }
    state tempState = ants[i].getState();

    //I used a switch before but it caused a lot of unintended behaviour so changed to if statements
    if (tempState != WANDER){
        if (tempState == SEEK){
            ants[i].slowDown(collisionDetectRadius);
        }
    }
    else {
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
        ants[i].setDesired(x, y);
    }
};
void AntController::setToAvoid(int16_t x, int16_t y){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setAvoidPos(x, y);
    }
};
void AntController::showFood(int16_t x, int16_t y){
    tft.drawCircle(x, y, 4, TFT_GREEN);
};
void AntController::removeFood(int16_t x, int16_t y){
    tft.drawCircle(x, y, 4, TFT_BLACK);
};