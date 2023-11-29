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
    //for main loop
    const static unsigned long runDelay = 200;
    unsigned long runStart = 0;

    const static uint8_t boundary = 5;//screenboundary
    const static uint8_t hudBoundary = 30;//how much space the HUD takes up - from y = 0 up
    const static int8_t maxForce = 1;//how much steering force is applied - greater number means more sharp turns (I think)
    const static int8_t wanderingDistance = 4;//how far in front of the ant when setting up wandering
    //the size of the circle used to determine if an ant is gonna collide
    //also used for the size of the food
    uint8_t collisionDetectRadius = 10;

    const uint8_t antDetectRadius = antSize * 2;//size of circle to detect another ant
    const float avoidanceFactor = 0.1; //used for avoiding a predator
    const static int16_t minSeparationDistance = 3;//3 best value for both wandering and follow the leader 
    TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
    const static unsigned long touchDelay = 10000; //to stop double touches
    unsigned long touchedStart = 0;
    //Keep track of which ant is predator or leader if they're switched on
    uint8_t leaderNumber = 0;
    uint8_t predatorNumber = 0;
    uint8_t predatorLoose = 0; //this used for HUD and to determine if an ant should check for predators
    //used for the HUD
    uint8_t wanderingOn = 0;
    uint8_t followLeaderOn = 0;
    uint8_t autoFeedOn = 0;//this used for HUD and to run the autofeeder
    uint8_t hudCol1 = 0;
    uint8_t hudCol2 = 0;
    uint8_t hudCol3 = 0;
    //stuff for food
    CoOrds foodPos = {.x = 0, .y = 0};
    CoOrds basePos = {.x = 0, .y = 0};
    uint8_t showingFood = 0;
    //used for timing how long to show the food for
    unsigned long startTime = 0;
    const static unsigned long foodDisplayTime = 20000;
    unsigned long autoFeedStartTime = 0;
    const static unsigned long autoFeedDelay = 60000;
    void checkTouchScreen();
    void autoFeedStart();
    void checkFoodRemoveTimer();
    void checkTimeForAutoFeed();
    void setToWander();
    void setToSeek(int16_t x, int16_t y);
    void setRandomPredator();
    void setToFollowLeader();
    void drawHud();
    void showCoords(int16_t &x, int16_t &y, int16_t size, uint16_t color);
    void removeCoords(int16_t &x, int16_t &y, int16_t size);
public:
    AntController(uint16_t screenWidth, uint16_t screenHeight);
    void init(int8_t startSpeed);
    void run();
    void moveAnts();


    const uint16_t colors[5] = {TFT_WHITE, TFT_ORANGE, TFT_GREEN, TFT_YELLOW, TFT_MAGENTA};//colors for states in AntAutoma.h
}; //end class



#endif