#include <Arduino.h>
#include "TFT_ILI9341.h"
#include "TFT_Touch.h"
#include "AntController.h"
#include "AntAutoma.h"
/*
This is intended as an ant simulator to run on an Arduino Uno connected to a ILI9341 TFT
set number of ants in AntController.h

*/
//set up screen
const uint16_t SCREENWIDTH = 320;
const uint16_t SCREENHEIGHT = 240;
//TFT_Touch touch = TFT_Touch(DCS, DCLK, DIN, DOUT);
byte waitDelay = 20;
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
  antsCtl.checkTimeForAutoFeed();
}