#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <string>
#include "FEHFile.h"
#include "FEHSD.h"

#define FORWARD 25
#define BACKWARD 25

using namespace std;

AnalogInputPin leftSensor(FEHIO::P0_0);
AnalogInputPin middleSensor(FEHIO::P0_1);
AnalogInputPin rightSensor(FEHIO::P0_2);

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
    } 
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
        Sleep(.1);
    }
}