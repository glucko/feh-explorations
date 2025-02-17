#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <string>
#define FORWARD 10
#define BACKWARD 10

#define ON_LINE_THRESHOLD 2
#define OFF_LINE_THRESHOLD 1.2
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
        LCD.WriteLine(state);

        switch(state) {

        case MIDDLE:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD);

            // if middle is off line, switch based on what sensor is on line
            if (middleOpt.Value() < OFF_LINE_THRESHOLD)
            {
                if (rightOpt.Value() > ON_LINE_THRESHOLD) {
                    state = RIGHT;
                } 
    
                if (leftOpt.Value() > ON_LINE_THRESHOLD) {
                    state = LEFT;
                }
            }
            // do code when middle and left/right are triggered
            // else 
            // {

            // }

            break; 

        case RIGHT:
            leftMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            rightMotor.SetPercent(FORWARD);
            
            // stop when middle is on line
            if(middleOpt.Value() > ON_LINE_THRESHOLD){ 
                state = MIDDLE;
            }
            break; 

        case LEFT:
            leftMotor.SetPercent(FORWARD);
            rightMotor.SetPercent(FORWARD + TURN_ADJUSTMENT);
            
            // stop when middle is on line
            if(middleOpt.Value() > ON_LINE_THRESHOLD){ 
                state = MIDDLE;
            }
            break; 

        default:
            break; 
        
        }
        Sleep(.1);
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
    
    lineFollowing();
    // while(true)
    // {
    //     string leftVal = "Left value: " + to_string(leftOpt.Value());
    //     string middleVal =  "Middle Value: " + to_string(middleOpt.Value()); 
    //     string rightVal =  "Right Value: " + to_string(rightOpt.Value());

    //     LCD.WriteLine(leftVal.c_str());
    //     LCD.WriteLine(middleVal.c_str());
    //     LCD.WriteLine(rightVal.c_str());

    //     lineFollowing();

    //     Sleep(.5);

    //     LCD.Clear();
    // }
}