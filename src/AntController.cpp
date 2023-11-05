#include <Arduino.h>
#include "AntController.h"

AntController::AntController(int16_t screenWidth, int16_t screenHeight){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
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
        tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, ants[i].color);
    }
    touch.setResolution(screenWidth, screenHeight);
    touch.setCal(3555, 680, 3313, 569, 320, 240, 1);
};
void AntController::checkTouchScreen(){
    if (!showingFood){
        if (touch.Pressed()){
            //get the data if the touch screen has been pressed
            //set food to same values (not sure if can go direct)
            foodPos.x = touch.X();
            foodPos.y = touch.Y();
            showingFood = 1;
            showCoords(foodPos.x, foodPos.y, collisionDetectRadius, TFT_GREEN);
            startTime = millis();
        }
    }
};
void AntController::checkFoodRemoveTimer(){
  if (millis() - startTime > foodDisplayTime){
    startTime = 0;
    showingFood = 0;
    removeCoords(foodPos.x, foodPos.y, collisionDetectRadius);
    //setToWander();
  }
}
void AntController::moveAnts(){
    showCoords(basePos.x, basePos.y, collisionDetectRadius * 2, TFT_BLUE);
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setCurrentPosToOldPos();
        ants[i].checkBoundary(screenWidth, screenHeight, boundary);
        int32_t dx = 0;
        int32_t dy = 0;
        uint16_t neighbourAnts = 0;
        for (byte j = 0; j < numOfAnts; j++){
            if(i != j){
                if(ants[i].detectCollision(ants[j].currentPos.x, ants[j].currentPos.y, antDetectRadius)){
                    dx += (ants[j].currentPos.x - ants[i].currentPos.x);
                    dy += (ants[j].currentPos.y - ants[i].currentPos.y);
                    neighbourAnts ++;
                }
            }
        }
        if (neighbourAnts > 0){
            ants[i].addToVelocityX((dx / neighbourAnts) * avoidanceFactor);
            ants[i].addToVelocityY((dy / neighbourAnts) * avoidanceFactor);
        }
        //I used a switch before but it caused a lot of unintended behaviour so changed to if statements
        if (showingFood){
            if (ants[i].detectCollision(foodPos.x, foodPos.y, collisionDetectRadius)){
                ants[i].setDesired(basePos.x, basePos.y);
                ants[i].antState = HASFOOD;
                ants[i].color = TFT_GREENYELLOW;
            }
        }
        if (ants[i].antState == WANDER){
            ants[i].wandering();
        }
        else if (ants[i].antState == HASFOOD){
            ants[i].slowDown(collisionDetectRadius);
            if (ants[i].detectCollision(basePos.x, basePos.y, collisionDetectRadius)){
                ants[i].antState = WANDER;
                ants[i].color = TFT_WHITE;
            }
        }
        ants[i].steering();
        ants[i].locomotion();
        tft.drawCircle(ants[i].getOldX(), ants[i].getOldY(), 2, TFT_BLACK);
        tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, ants[i].color);
    }
};
void AntController::setToWander(){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].antState = WANDER;
        ants[i].color = TFT_WHITE;
    }
};
void AntController::setToSeek(int16_t x, int16_t y){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].antState = SEEK;
        ants[i].setDesired(x, y);
    }
};
void AntController::setToAvoid(int16_t x, int16_t y){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setAvoidPos(x, y);
    }
};
void AntController::showCoords(int16_t &x, int16_t &y, int16_t size, uint16_t color){
    tft.drawCircle(x, y, size, color);
};
void AntController::removeCoords(int16_t &x, int16_t &y, int16_t size){
    tft.drawCircle(x, y, size, TFT_BLACK);
};