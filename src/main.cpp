#include <Arduino.h>
#include "TFT_ILI9341.h"
#include "TFT_Touch.h"
#include "AntController.h"
#include "AntAutoma.h"
//#define DOUT 3  /* Data out pin (T_DO) of touch screen */
//#define DIN  4  /* Data in pin (T_DIN) of touch screen */
//#define DCS  5  /* Chip select pin (T_CS) of touch screen */
//#define DCLK 6  /* Clock pin (T_CLK) of touch screen */
/*
This is intended as an ant simulator to run on an Arduino Uno connected to a ILI9341 TFT
set number of ants in AntController.h

*/
//set up screen
const uint16_t SCREENWIDTH = 320;
const uint16_t SCREENHEIGHT = 240;
//TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
byte waitDelay = 50;
AntController antsCtl(SCREENWIDTH, SCREENHEIGHT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  antsCtl.init(4);
}
void loop() {
  antsCtl.checkTouchScreen();
  antsCtl.moveAnts();
  delay(waitDelay);
  antsCtl.checkFoodRemoveTimer();
}