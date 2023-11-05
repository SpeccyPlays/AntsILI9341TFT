#ifndef ANTCTL_H   /* Include guard */
#define ANTCTL_H
#include <Arduino.h>
#include "CoOrds.h"
#include "TFT_ILI9341.h"
#include "AntAutoma.h"

class AntController {
private:
    TFT_ILI9341 tft;
    int16_t screenWidth;
    int16_t screenHeight;
    byte numOfAnts;
    Ant *ants;
    int8_t maxSpeed = 4;//Top speed of the ant
    int8_t maxForce = 1;//how much steering force is applied - greater number means more sharp turns (I think)
    int8_t wanderingDistance = 4;//how far in front of the ant when setting up wandering
    uint8_t boundary = 5;//screenboundary
    uint8_t collisionDetectRadius = 10;//the size of the circle used to determine if an ant is gonna collide
    uint8_t antDetectRadius = 5;
    float avoidanceFactor = 0.005;
public:
    AntController(int16_t screenWidth, int16_t screenHeight, byte numOfAnts);
    void init();
    void moveAnts();
    void setToWander();
    void setToSeek(int16_t x, int16_t y);
    void showFood(int16_t x, int16_t y);
    void removeFood(int16_t x, int16_t y);

}; //end class



#endif