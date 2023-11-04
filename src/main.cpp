#include <Arduino.h>
#include "TFT_ILI9341.h"
#include "TFT_Touch.h"
#include "AntController.h"
#include "AntAutoma.h"
#define DOUT 3  /* Data out pin (T_DO) of touch screen */
#define DIN  4  /* Data in pin (T_DIN) of touch screen */
#define DCS  5  /* Chip select pin (T_CS) of touch screen */
#define DCLK 6  /* Clock pin (T_CLK) of touch screen */
/*
This is intended as an ant simulator to run on an Arduino Uno connected to a ILI9341 TFT
add the screenwidth and screenheight values to the antautomata.h file also
*/
//set up screen
const uint16_t SCREENWIDTH = 320;
const uint16_t SCREENHEIGHT = 240;
TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
int X_RawData;
int Y_RawData;
int X_Coord;
int Y_Coord;
byte waitDelay = 250;
byte counter = 0;
int16_t foodX = 0;
int16_t foodY = 0;
byte showFood = 0;
AntController antsCtl(SCREENWIDTH, SCREENWIDTH, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  antsCtl.init();
  touch.setResolution(SCREENWIDTH, SCREENHEIGHT);
  touch.setCal(3555, 680, 3313, 569, 320, 240, 1);
}
void loop() {
  antsCtl.moveAnts();
  delay(waitDelay);
  // put your main code here, to run repeatedly:
  /*if (touch.Pressed()){
    //get the data if the touch screen has been pressed
    //set food to same values (not sure if can go direct)
    X_Coord = touch.X();
    Y_Coord = touch.Y();
    foodX = X_Coord;
    foodY = Y_Coord;
    tft.drawCircle(foodX, foodY, 4, TFT_GREEN);
    showFood = 1;
  }
  counter ++;
  if (counter % 199 == 0){
    for (byte i = 0; i < amountOfAnts; i++){
      ants[i].setState(WANDER);
    }
    counter = 0;
    showFood = 0;
    tft.drawCircle(foodX, foodY, 4, TFT_BLACK);
  }
  else if (showFood){
    for (byte i = 0; i < amountOfAnts; i++){
      //ants[i].setState(AVOID);
      //ants[i].setAvoidPos(foodX, foodY);
      ants[i].setState(SEEK);
      ants[i].setDesired(foodX, foodY);
    }
  }*/
}