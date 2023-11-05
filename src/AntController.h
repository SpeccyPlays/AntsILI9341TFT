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
    byte numOfAnts;
    Ant *ants;
    uint8_t boundary = 5;//screenboundary
    uint8_t collisionDetectRadius = 10;//the size of the circle used to determine if an ant is gonna collide
    uint8_t antDetectRadius = 5;//size of circle to detect another ant
    float avoidanceFactor = 0.005;
    TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
    int16_t foodPosX = 0;
    int16_t foodPosY = 0;
    CoOrds avoidPos ={.x = 0, .y = 0};
    CoOrds basePos = {.x = 0, .y = 0};
    uint8_t showingFood = 0;
    uint8_t showingAvoid = 0;
    //used for timing how long to show the food for
    unsigned long startTime = 0;
    uint16_t foodDisplayTime = 5000;
public:
    AntController(int16_t screenWidth, int16_t screenHeight, byte numOfAnts);
    void init(int8_t startSpeed);
    void showCoords(int16_t &x, int16_t &y, int16_t size, uint16_t color);
    void removeCoords(int16_t &x, int16_t &y, int16_t size);
    void moveAnts();
    void setToAvoid(int16_t x, int16_t y);
    void setToWander();
    void setToSeek(int16_t x, int16_t y);
    void checkTouchScreen();
    void checkFoodRemoveTimer();

}; //end class



#endif