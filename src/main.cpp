#include <Arduino.h>
#include "TFT_ILI9341.h"
#include "AntAutoma.h"
/*
This is intended as an ant simulator to run on an Arduino Uno connected to a ST7735 TFT
add the screenwidth and screenheight values to the .h file also
*/
//set up screen
const uint16_t SCREENWIDTH = 240;
const uint16_t SCREENHEIGHT = 320;
TFT_ILI9341 tft(SCREENWIDTH, SCREENHEIGHT);
//set up ant
Ant ants[25];
byte amountOfAnts = 25;
byte waitDelay = 50;
byte counter = 0;
int16_t foodX = 0;
int16_t foodY = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  randomSeed(analogRead(0));
  for (byte i = 0; i < amountOfAnts; i++){
    ants[i].resetAnt(SCREENWIDTH, SCREENHEIGHT, 4);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  for (byte i = 0; i < amountOfAnts; i++){
    tft.drawCircle(ants[i].getCurrentX(), ants[i].getCurrentY(), 2, TFT_WHITE);
    ants[i].moveAnt();
    //tft.drawLine(ants[i].getCurrentX(), ants[i].getCurrentY(), ants[i].getDesiredX(), ants[i].getDesiredY(), TFT_WHITE);
  }
  delay(waitDelay);
  for (byte i = 0; i < amountOfAnts; i++){
    tft.drawCircle(ants[i].getOldX(), ants[i].getOldY(), 2, TFT_BLACK);
    //tft.drawLine(ants[i].getCurrentX(), ants[i].getCurrentY(), ants[i].getDesiredX(), ants[i].getDesiredY(), TFT_BLACK);
  }
  counter ++;
  if (counter % 150 == 0){
    for (byte i = 0; i < amountOfAnts; i++){
      ants[i].setState(WANDER);
    }
    counter = 0;
    tft.drawCircle(foodX, foodY, 4, TFT_BLACK);
  }
  else if (counter % 100 == 0){
    foodX = random(30, (SCREENWIDTH - 30));
    foodY = random(30, (SCREENHEIGHT - 30));
    for (byte i = 0; i < amountOfAnts; i++){
      //ants[i].setState(AVOID);
      //ants[i].setAvoidPos(foodX, foodY);
      ants[i].setState(SEEK);
      ants[i].setDesired(foodX, foodY);
    }
  tft.drawCircle(foodX, foodY, 4, TFT_GREEN);
  }
}