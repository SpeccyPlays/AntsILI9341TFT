#include <Arduino.h>
#include "AntAutoma.h"
void Ant::resetAnt(uint16_t screenWidth, uint16_t screenHeight, byte velocity, uint8_t boundary){
    this->currentPos.x = random(boundary, (screenWidth - boundary)); //random(10, screenWidth - 10);
    this->currentPos.y = random(boundary, (screenHeight - boundary));//random(10, screenHeight - 10);
    this->antState = WANDER;
    this->velocity.x = 4;
    this->velocity.y = 4;
    setDesired(screenWidth / 2, screenHeight / 2);
};
void Ant::setCurrentPosToOldPos(){
    oldPos.x = currentPos.x;
    oldPos.y = currentPos.y;
};
void Ant::setDesired(int16_t x, int16_t y){
    desired.x = x;
    desired.y = y;
};
void Ant::setAvoidPos(int16_t x, int16_t y){
    avoidPos.x = x;
    avoidPos.y = y;
};
CoOrds Ant::setMagnitude(CoOrds temp, int8_t newMag){
    float length = sqrt((temp.x * temp.x + temp.y * temp.y));
    if (length != 0){
        temp.x =  (temp.x / length) * newMag;
        temp.y = (temp.y / length) * newMag;
    }
    return temp;
};
BigCoOrds Ant::setMagnitude(BigCoOrds temp, int8_t newMag){
    float length = sqrt((temp.x * temp.x + temp.y * temp.y));
    if (length != 0){
        temp.x =  (temp.x / length) * newMag;
        temp.y = (temp.y / length) * newMag;
    }
    return temp;
};
void Ant::addToVelocityX(int32_t x){
    velocity.x += x;
};
void Ant::addToVelocityY(int32_t y){
    velocity.y += y;
};
void Ant::checkBoundary(int16_t &width, int16_t &height, const uint8_t &boundary, const uint8_t &hudBoundary){
    //check if we're going to go off screen
    if (currentPos.x < boundary){
        velocity.x += 2;
    }
    else if (currentPos.x > (width - boundary)){
        velocity.x -= 2;
    }
    if (currentPos.y < hudBoundary){
        velocity.y += 2;
    }
    else if (currentPos.y > (height - boundary)){
        velocity.y -= 2;
    }
};
uint8_t Ant::detectCollision(int16_t &x, int16_t &y, int16_t r){
  /*
  Check if we're in the circle of x and y
  Including the radius as there's a different range for if we hit another ant or desired location
  */
  int32_t dx = x - currentPos.x;
  int32_t dy = y - currentPos.y;
  uint16_t rr = r * r; //(r + r) * (r + r);
  uint32_t distance = (dx * dx) + (dy * dy);
  if ( distance <= rr ){
    return 1;
  }
  else {
    return 0;
  }
};
void Ant::slowDown(uint8_t &collisionDetectRadius){
    /*
    Slow down if we're nearing the desired target
    */
    if(detectCollision(desired.x, desired.y, collisionDetectRadius)){
        if (velocity.x != 0){
            velocity.x /= 2;
        }
        if (velocity.y != 0){
            velocity.y /= 2;
        }
    }
};
void Ant::slowDown(){
    if (velocity.x != 0){
        velocity.x /= 2;
    }
    if (velocity.y != 0){
        velocity.y /= 2;
    }
};
void Ant::seeking(int16_t x, int16_t y){
    setDesired(x, y);
};
void Ant::wandering(const int8_t &wanderingDistance){
    /*
    Go to a point in front of the ant
    Pick a random point on a circle from there
    Set that to where the ant wants to head towards
    Makes the movement look more natural

    */
    int16_t tempX = currentPos.x + (velocity.x * wanderingDistance);
    int16_t tempY = currentPos.y + (velocity.y * wanderingDistance);
    float randAngle = (random(360) * PI) / 180.0;
    uint8_t randomDistance = random(10, 30);
    tempX += randomDistance * cos(randAngle);
    tempY += randomDistance * sin(randAngle);
    setDesired(tempX, tempY);
};
void Ant::calculateVelocties(){
    /*
    Set the desired velocity to the amount we need to move to desired
    Reduce the magnitude or it'll go straight there
    Limit the velocity for the same reason 
    (I think for the last two)
    */
    desiredVelocity.x = (desired.x - currentPos.x);
    desiredVelocity.y = (desired.y - currentPos.y);
    desiredVelocity = setMagnitude(desiredVelocity, maxSpeed);
    desiredVelocity.x *= maxSpeed;
    desiredVelocity.y *= maxSpeed;
    if (desiredVelocity.x > maxSpeed){
        desiredVelocity.x = maxSpeed;
    }
    else if (desiredVelocity.x < -maxSpeed){
        desiredVelocity.x = -maxSpeed;
    }
    if (desiredVelocity.y > maxSpeed){
        desiredVelocity.y = maxSpeed;
    }
    else if(desiredVelocity.y < -maxSpeed){
        desiredVelocity.y = -maxSpeed;
    }
};
void Ant::steering(const int8_t &maxForce){
    /*
    How much the ant will turn towards it's desired destination
    Use the class maxforce value to limit or increase the turning force
    */
    calculateVelocties();
    steeringForce.x = desiredVelocity.x - velocity.x;
    steeringForce.y = desiredVelocity.y - velocity.y;
    if (steeringForce.x > maxForce){
        steeringForce.x = maxForce;
    }
    else if (steeringForce.x < -maxForce){
        steeringForce.x = -maxForce;
    }
    if (steeringForce.y > maxForce){
        steeringForce.y = maxForce;
    }
    else if (steeringForce.y < -maxForce){
        steeringForce.y = -maxForce;
    }
    velocity.x += steeringForce.x;
    velocity.y += steeringForce.y;
};
void Ant::locomotion(){
    currentPos.x += velocity.x;
    currentPos.y += velocity.y;
};
int16_t Ant::getDesiredX(){
    return desired.x;
};
int16_t Ant::getDesiredY(){
    return desired.y;
};