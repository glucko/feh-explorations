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

AnalogInputPin leftOpt(FEHIO::P1_2);
AnalogInputPin middleOpt(FEHIO::P1_1);
AnalogInputPin rightOpt(FEHIO::P1_0);

FEHMotor leftMotor(FEHMotor::Motor1, 9);
FEHMotor rightMotor(FEHMotor::Motor0, 9);

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

            // if middle is off line, switch based on what sensor is on line
            if (middleOpt.Value() > SENSOR_THRESHOLD)
            {
                if (rightOpt.Value() < SENSOR_THRESHOLD) {
                    state = RIGHT;
                } 
    
                if (leftOpt.Value() < SENSOR_THRESHOLD) {
                    state = LEFT;
                }
            }
            // else 
            // {

            // }

            // do code when middle and left/right are triggered
            break; 

        case RIGHT:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            
            // stop when middle is on line
            if(middleOpt.Value() < SENSOR_THRESHOLD){ 
                state = MIDDLE;
            }
            break; 

        case LEFT:
            leftMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            rightMotor.SetPercent(FORWARD);
            
            // stop when middle is on line
            if(leftOpt.Value() < SENSOR_THRESHOLD){ 
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
        string leftVal = "Left value: " + to_string(leftOpt.Value());
        string middleVal =  "Middle Value: " + to_string(middleOpt.Value()); 
        string rightVal =  "Right Value: " + to_string(rightOpt.Value());

        LCD.WriteLine(leftVal.c_str());
        LCD.WriteLine(middleVal.c_str());
        LCD.WriteLine(rightVal.c_str());
        // lineFollowing();
        Sleep(5.0);

        LCD.Clear();
    }
}