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

AntController antsCtl(SCREENWIDTH, SCREENHEIGHT);

void setup() {
  antsCtl.init(4);
}
void loop() {
  antsCtl.run();
}