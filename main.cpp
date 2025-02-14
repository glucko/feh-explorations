#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <string>
#include "FEHFile.h"
#include "FEHSD.h"

#define LEFT_TURN 1
#define RIGHT_TURN -1

#define FORWARD 25
#define BACKWARD 25

using namespace std;

AnalogInputPin leftSensor(FEHIO::P0_0);
AnalogInputPin middleSensor(FEHIO::P0_0);
AnalogInputPin rightSensor(FEHIO::P0_0);

DigitalInputPin frontRight(FEHIO::P1_0);
DigitalInputPin backRight(FEHIO::P1_1);
DigitalInputPin frontLeft(FEHIO::P1_2);
DigitalInputPin backLeft(FEHIO::P1_3);

FEHMotor leftMotor(FEHMotor::Motor0, 9);
FEHMotor rightMotor(FEHMotor::Motor2, 9);

enum LineStates { 
    MIDDLE, 
    RIGHT, 
    LEFT
   }; 

void lineFollowing()
{
    int state = MIDDLE;
    while (true) {
        switch(state) { 

        case MIDDLE:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD);

            if (rightSensor.Value() < 1.5) {
                state = RIGHT;
            } 

            if (leftSensor.Value() < 1.5) {
                state = LEFT;
            }
            break; 

        case RIGHT:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD + 5);
            
            // when right is no longer triggered
            if(rightSensor.Value() > 1.5){ 
                state = MIDDLE;
            }
            break; 

        case LEFT:
            leftMotor.SetPercent(FORWARD + 5);
            rightMotor.SetPercent(FORWARD);
            
            // when left is no longer triggered
            if(leftSensor.Value() > 1.5){ 
                state = MIDDLE;
            }
            break; 

        default:
            break; 
        }

        Sleep(.1);
    } 
}

void correction()
{
    if(!backRight.Value() && backLeft.Value()){ //Right one has hit but left hasn't
        LCD.Write("Right one hit, left not");
        leftMotor.SetPercent(BACKWARD);
        rightMotor.SetPercent(0);
    }
    if(backRight.Value() && !backLeft.Value()){ //Right one hasn't hit but left one has
        LCD.Write("Left one hit, right not");
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(BACKWARD);
    }
    while(backRight.Value() || backLeft.Value())
    {
        //Looping while at least one switch isn't clicked
    }
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void turn(int dir)
{
    if (dir == LEFT_TURN)
    {
        rightMotor.SetPercent(FORWARD);
        LCD.Write("Turning left");
    } else{
        leftMotor.SetPercent(FORWARD);
        LCD.Write("Turning right");
    }

    while(!backRight.Value() || !backLeft.Value()) {
        //Loops until a bump switch is hit
    }

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);

    correction();
}

void driveUntilHit()
{
    leftMotor.SetPercent(FORWARD);
    rightMotor.SetPercent(FORWARD);

    while(frontLeft.Value() || frontRight.Value()) {}

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void waitUntilTouch()
{
    float left;
    float right;

    while(!LCD.Touch(&left, &right)) {}

    while(LCD.Touch(&left, &right)) {}
}

void log(string message)
{

    FEHFile *ofptr = SD.FOpen("Output.txt", "w");

    SD.FPrintf(ofptr, message.c_str());

    SD.FClose(ofptr);
}

int main()
{
    waitUntilTouch();
    
    while(true)
    {
        lineFollowing();
    }
}