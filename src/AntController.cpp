#include <Arduino.h>
#include "AntController.h"

AntController::AntController(uint16_t screenWidth, uint16_t screenHeight){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    TFT_ILI9341 temp(this->screenWidth, this->screenHeight);
    this->tft = temp;
};
void AntController::init(int8_t startSpeed){
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    randomSeed(analogRead(0));
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].resetAnt(screenWidth, screenHeight, startSpeed, boundary);
        tft.drawCircle(ants[i].currentPos.x, ants[i].currentPos.y, 2, colors[ants[i].antState]);
    }
    touch.setResolution(screenWidth, screenHeight);
    touch.setCal(3555, 680, 3313, 569, 320, 240, 1);
};
void AntController::setRandomLeader(){
    leaderNumber = random(0, numOfAnts);
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
  }
}
void AntController::moveAnts(){
    showCoords(basePos.x, basePos.y, collisionDetectRadius * 2, TFT_BLUE);
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setCurrentPosToOldPos();
        ants[i].checkBoundary(screenWidth, screenHeight, boundary);
        //collision detection works better if steering here
                //I used a switch before but it caused a lot of unintended behaviour so changed to if statements
        if (showingFood){
            if (ants[i].detectCollision(foodPos.x, foodPos.y, collisionDetectRadius)){
                ants[i].setDesired(basePos.x, basePos.y);
                ants[i].antState = HASFOOD;
            }
        }
        if (ants[i].antState == WANDER){
            ants[i].wandering(wanderingDistance);
        }
        else if (ants[i].antState == HASFOOD){
            ants[i].slowDown(collisionDetectRadius);
            if (ants[i].detectCollision(basePos.x, basePos.y, collisionDetectRadius)){
                ants[i].antState = WANDER;
            }
        }
        else if (ants[i].antState == FOLLOW){
            if (i != leaderNumber){
                uint8_t leaderRadius = collisionDetectRadius * 2;
                ants[i].slowDown(leaderRadius);
                ants[i].setDesired(ants[leaderNumber].currentPos.x, ants[leaderNumber].currentPos.y);
            }
        }
        ants[i].steering(maxForce);
        int32_t dx = 0;
        int32_t dy = 0;
        uint8_t neighbourAnts = 0;
        for (byte j = 0; j < numOfAnts; j++){
            if(i != j){
                if(ants[i].detectCollision(ants[j].currentPos.x, ants[j].currentPos.y, antDetectRadius)){
                    dx += (ants[j].currentPos.x - ants[i].currentPos.x);
                    dy += (ants[j].currentPos.y - ants[i].currentPos.y);
                    neighbourAnts ++;
                }
            }
        }
        /***** chatGPT wrote this bit for avoiding neighbours
         ***** the sqrt slows it down a bit but not much **/
        int16_t distance = sqrt(dx * dx + dy * dy);
        if (neighbourAnts > 0 && distance < minSeparationDistance) {
            // Calculate a separation force to move the ant away from its neighbors
            int16_t separationForceX = -(dx / distance) * minSeparationDistance;
            int16_t separationForceY = -(dy / distance) * minSeparationDistance;
            ants[i].addToVelocityX(separationForceX);
            ants[i].addToVelocityY(separationForceY);
        }
        /***** end of chatgpt code ***/
        ants[i].locomotion();
        removeCoords(ants[i].oldPos.x, ants[i].oldPos.y, 2);
        showCoords(ants[i].currentPos.x, ants[i].currentPos.y, 2, colors[ants[i].antState]);
    }
};
void AntController::setToWander(){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].antState = WANDER;
    }
};
void AntController::setToSeek(int16_t x, int16_t y){
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].antState = SEEK;
        ants[i].setDesired(x, y);
    }
};
void AntController::setToFollowLeader(){
    ants[leaderNumber].antState = WANDER;
    for (byte i = 0; i < numOfAnts; i++){
        if (i != leaderNumber){
            ants[i].antState = FOLLOW;
            ants[i].setDesired(ants[leaderNumber].currentPos.x, ants[leaderNumber].currentPos.y);
        }
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