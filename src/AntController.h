#ifndef ANTCTL_H   /* Include guard */
#define ANTCTL_H
#include <Arduino.h>
#include "CoOrds.h"
#include "TFT_ILI9341.h"
#include "AntAutoma.h"
#include "TFT_Touch.h"
//tft touch pins
#define DOUT 3  /* Data out pin (T_DO) of touch screen */
#define DIN  4  /* Data in pin (T_DIN) of touch screen */
#define DCS  5  /* Chip select pin (T_CS) of touch screen */
#define DCLK 6  /* Clock pin (T_CLK) of touch screen */

class AntController {
private:
    TFT_ILI9341 tft;
    int16_t screenWidth;
    int16_t screenHeight;
    /****
     * Update both numOfAnts to the same value other behaviour may not be as expected
    */
    const byte numOfAnts = 25;
    byte antSize = 2;
    Ant ants[25];
    const static uint8_t boundary = 5;//screenboundary
    const static int8_t maxForce = 1;//how much steering force is applied - greater number means more sharp turns (I think)
    const static int8_t wanderingDistance = 4;//how far in front of the ant when setting up wandering
    uint8_t collisionDetectRadius = 10;//the size of the circle used to determine if an ant is gonna collide
    const uint8_t antDetectRadius = antSize * 2;//size of circle to detect another ant
    const float avoidanceFactor = 0.1; //used for avoiding a predator
    const static int16_t minSeparationDistance = 3;//3 best value for both wandering and follow the leader 
    TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
    byte leaderNumber = 0;
    byte predatorNumber = 0;
    byte predatorLose = 0;
    CoOrds foodPos = {.x = 0, .y = 0};
    CoOrds avoidPos ={.x = 0, .y = 0};
    CoOrds basePos = {.x = 0, .y = 0};
    uint8_t showingFood = 0;
    uint8_t showingAvoid = 0;
    //used for timing how long to show the food for
    unsigned long startTime = 0;
    const static unsigned long foodDisplayTime = 20000;
public:
    AntController(uint16_t screenWidth, uint16_t screenHeight);
    void init(int8_t startSpeed);
    void showCoords(int16_t &x, int16_t &y, int16_t size, uint16_t color);
    void removeCoords(int16_t &x, int16_t &y, int16_t size);
    void moveAnts();
    void setToAvoid(int16_t x, int16_t y);
    void setToWander();
    void setToSeek(int16_t x, int16_t y);
    void setRandomLeader();
    void setToFollowLeader();//run setRandomLeader first
    void setRandomPredator();
    void checkTouchScreen();
    void checkFoodRemoveTimer();
    const uint16_t colors[5] = {TFT_WHITE, TFT_ORANGE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA};//colors for states in AntAutoma.h
}; //end class



#endif