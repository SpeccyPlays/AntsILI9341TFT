#ifndef ANTAUTOMA_H   /* Include guard */
#define ANTAUTOMA_H
#include <Arduino.h>
#include "CoOrds.h"
#include "TFT_ILI9341.h"
/*
This is the class for a single ant in the system
*/
enum state {WANDER = 0, SEEK = 1, HASFOOD = 2, FOLLOW = 3, NOMATES = 4};
class Ant {
    /*
    All the class instances
    These are used to determine the ants behavious
    */
    const static int8_t maxSpeed = 4;//Top speed of the ant
    const static int8_t maxForce = 1;//how much steering force is applied - greater number means more sharp turns (I think)
    const static int8_t wanderingDistance = 4;//how far in front of the ant when setting up wandering
    public :
    void resetAnt(uint16_t screenWidth, uint16_t screenHeight, byte velocity, uint8_t boundary);
    int16_t getCurrentX();
    int16_t getCurrentY();
    int16_t getOldX();
    int16_t getOldY();
    int16_t getDesiredX();
    int16_t getDesiredY();
    state getState();
    void setCurrentPosToOldPos();
    void setDesired(int16_t x, int16_t y);
    void setAvoidPos(int16_t x, int16_t y);
    void addToVelocityX(int32_t x);
    void addToVelocityY(int32_t y);
    void checkBoundary(int16_t &width, int16_t &height, uint8_t &boundary);
    void seeking(int16_t x, int16_t y);
    void wandering();
    void steering();
    void slowDown(uint8_t &collisionDetectRadius);
    void slowDown();
    void queue();
    void locomotion();
    uint8_t detectCollision(int16_t &x, int16_t &y, int16_t r);
    state antState; //if I use a get state then it gets optimised out by the compiler
    CoOrds currentPos;//where we're at now
    CoOrds oldPos;//used to remove ant from screen 
    private :
    CoOrds desired;//where we want to go
    CoOrds avoidPos;//where to avoid
    //float angle; left this here if I want to draw according to direction ant faces
    BigCoOrds velocity;
    BigCoOrds desiredVelocity;
    BigCoOrds steeringForce;
    CoOrds setMagnitude(CoOrds temp, int8_t newMag);
    BigCoOrds setMagnitude(BigCoOrds temp, int8_t newMag);
    void calculateVelocties();

};

#endif