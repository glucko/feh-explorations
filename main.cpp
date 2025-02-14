#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <string>
#define FORWARD 25
#define BACKWARD 25

#define SENSOR_THRESHOLD 1.5
#define TURN_ADJUSTMENT 5

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

            if (rightSensor.Value() < SENSOR_THRESHOLD) {
                state = RIGHT;
            } 

            if (leftSensor.Value() < SENSOR_THRESHOLD) {
                state = LEFT;
            }
            break; 

        case RIGHT:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            
            // when right is no longer triggered
            if(rightSensor.Value() > SENSOR_THRESHOLD){ 
                state = MIDDLE;
            }
            break; 

        case LEFT:
            leftMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            rightMotor.SetPercent(FORWARD);
            
            // when left is no longer triggered
            if(leftSensor.Value() > SENSOR_THRESHOLD){ 
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

// void log(string message)
// {

//     FEHFile *ofptr = SD.FOpen("Output.txt", "w");

//     SD.FPrintf(ofptr, message.c_str());

//     SD.FClose(ofptr);
// }

int main()
{
    waitUntilTouch();
    
    while(true)
    {
        lineFollowing();
        Sleep(.1);
    }
}