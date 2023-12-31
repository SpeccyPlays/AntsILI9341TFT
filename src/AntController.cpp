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
        tft.drawCircle(ants[i].currentPos.x, ants[i].currentPos.y, antSize, colors[ants[i].antState]);
    }
    touch.setResolution(screenWidth, screenHeight);
    touch.setCal(3555, 680, 3313, 569, 320, 240, 1);
    wanderingOn = 1;
    basePos.x = screenWidth - boundary;
    basePos.y = screenHeight - boundary;
    hudCol1 = screenWidth / 4;
    hudCol2 = hudCol1 * 2;
    hudCol3 = hudCol1 * 3;
    drawHud();
};
void AntController::run(){
    checkTouchScreen();
    if (millis() - runStart > runDelay){
        moveAnts();
        checkFoodRemoveTimer();
        checkTimeForAutoFeed();
        runStart = 0;
  }
}
void AntController::drawHud(){
    tft.setCursor(boundary, boundary);
    tft.setTextFont(2);
    tft.setTextColor(colors[WANDER]);
    tft.print("  WANDER");
    
    tft.setCursor(hudCol1, boundary);
    tft.setTextColor(colors[HASFOOD]);
    tft.print(" AUTOFEED");
    
    tft.setCursor(hudCol2, boundary);
    tft.setTextColor(colors[FOLLOW]);
    tft.print("  FOLLOW");
    
    tft.setCursor(hudCol3, boundary);
    tft.setTextColor(colors[PREDATOR]);
    tft.print(" PREDATOR");
    //fast line uses width which will be the width of hudCol1
    if (wanderingOn){
       tft.drawFastHLine(0, 2, hudCol1, colors[WANDER]); 
    }
    if (autoFeedOn){
        tft.drawFastHLine(hudCol1, 2, hudCol1, colors[HASFOOD]);
    }
    if (followLeaderOn){
        tft.drawFastHLine(hudCol2, 2, hudCol1, colors[FOLLOW]);
    }
    if (predatorLoose){
        tft.drawFastHLine(hudCol3, 2, hudCol1, colors[PREDATOR]);
    }
};
void AntController::checkTouchScreen(){
    if (touch.Pressed() && millis() - touchedStart > touchDelay){
        uint16_t touchX = touch.X();
        uint16_t touchY = touch.Y();
        touchedStart = 0;
        if (touchY < hudBoundary){
            tft.drawFastHLine(0, 2, screenWidth, TFT_BLACK);
            if (touchX < hudCol1){
                setToWander();
            }
            else if (touchX > hudCol1 && touchX < hudCol2 && !showingFood){
                //if foods already showing, don't do anything as no capacity for multiple food
                autoFeedOn = !autoFeedOn;
                removeCoords(foodPos.x, foodPos.y, collisionDetectRadius);
                if (autoFeedOn){
                    autoFeedStart();
                }
            }
            else if (touchX > hudCol2 && touchX < hudCol3){
                showingFood = 0; //otherwise ants can get stuck with food and not following
                removeCoords(foodPos.x, foodPos.y, collisionDetectRadius);
                setToFollowLeader();
            }
            else if (touchX > hudCol3){
                setRandomPredator();
            }
        }
        else if (!showingFood){
            tft.drawFastHLine(0, 2, screenWidth, TFT_BLACK);
            foodPos.x = touch.X();
            foodPos.y = touch.Y();
            showingFood = 1;
            autoFeedOn = 0;
            showCoords(foodPos.x, foodPos.y, collisionDetectRadius, TFT_GREEN);
            startTime = millis();
        }
    }
};
void AntController::autoFeedStart(){
    autoFeedOn = 1;
    showingFood = 1;
    foodPos.x = random(boundary, screenWidth - boundary);
    foodPos.y = random(hudBoundary, screenHeight - boundary);
    showCoords(foodPos.x, foodPos.y, collisionDetectRadius, TFT_GREEN);
    autoFeedStartTime = millis();
    startTime = millis();
};
void AntController::checkFoodRemoveTimer(){
    if (showingFood){
        if (millis() - startTime > foodDisplayTime){
            startTime = 0;
            showingFood = 0;
            removeCoords(foodPos.x, foodPos.y, collisionDetectRadius);
        }
    }
};
void AntController::checkTimeForAutoFeed(){
    if(autoFeedOn){
        if (millis() - autoFeedStartTime > autoFeedDelay){
            autoFeedStart();
        }
    }
};
void AntController::moveAnts(){
    showCoords(basePos.x, basePos.y, collisionDetectRadius * 2, TFT_BLUE);
    for (byte i = 0; i < numOfAnts; i++){
        ants[i].setCurrentPosToOldPos();
        ants[i].checkBoundary(screenWidth, screenHeight, boundary, hudBoundary);
        //I used a switch before but it caused a lot of unintended behaviour so changed to if statements
        if ((showingFood) && (ants[i].antState != PREDATOR) && (ants[i].antState != FOLLOW)){
            if (ants[i].detectCollision(foodPos.x, foodPos.y, collisionDetectRadius)){
                ants[i].setDesired(basePos.x, basePos.y);
                ants[i].antState = HASFOOD;
            }
        }
        if (ants[i].antState == WANDER || ants[i].antState == PREDATOR){
            ants[i].wandering(wanderingDistance);
        }
        else if (ants[i].antState == HASFOOD){
            uint8_t baseRad = collisionDetectRadius * 2;
            ants[i].slowDown(baseRad);
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
        //collision detection works better if steering here
        ants[i].steering(maxForce);
        //required for collision detection calculations
        int32_t dx = 0;
        int32_t dy = 0;
        uint8_t neighbourAnts = 0;
        //loop through all ants to check for collisions
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
        if((predatorLoose) && (i != predatorNumber)){
        /*
        Check if a predator lose, we're not the predator, and avoid
        the larger collision radius gives much better looking results
        */
            if (ants[i].detectCollision(ants[predatorNumber].currentPos.x, ants[predatorNumber].currentPos.y, collisionDetectRadius * 3)){
                ants[i].addToVelocityX((ants[i].currentPos.x - ants[predatorNumber].currentPos.x) * avoidanceFactor);
                ants[i].addToVelocityY((ants[i].currentPos.y - ants[predatorNumber].currentPos.y) * avoidanceFactor);
            }
        }
        ants[i].locomotion();
        removeCoords(ants[i].oldPos.x, ants[i].oldPos.y, antSize);
        showCoords(ants[i].currentPos.x, ants[i].currentPos.y, antSize, colors[ants[i].antState]);
    }
    drawHud();//ants can sometimes move over the hud so needs to be redrawn
};
void AntController::setToWander(){
    predatorLoose = 0;
    wanderingOn = 1;
    followLeaderOn = 0;
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
    leaderNumber = random(0, numOfAnts);
    ants[leaderNumber].antState = WANDER;
    wanderingOn = 0;
    followLeaderOn = 1;
    predatorLoose = 0;
    for (byte i = 0; i < numOfAnts; i++){
        if (i != leaderNumber){
            ants[i].antState = FOLLOW;
            ants[i].setDesired(ants[leaderNumber].currentPos.x, ants[leaderNumber].currentPos.y);
        }
    }
};
void AntController::setRandomPredator(){
    predatorNumber = random(0, numOfAnts);
    ants[predatorNumber].antState = PREDATOR;
    predatorLoose = 1;
    wanderingOn = 0;
    followLeaderOn = 0;
    for (byte i = 0; i < numOfAnts; i++){
        if (i != predatorNumber){
            ants[i].antState = WANDER;
        }
    }
};
void AntController::showCoords(int16_t &x, int16_t &y, int16_t size, uint16_t color){
    tft.drawCircle(x, y, size, color);
};
void AntController::removeCoords(int16_t &x, int16_t &y, int16_t size){
    tft.drawCircle(x, y, size, TFT_BLACK);
};