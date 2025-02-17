#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <string>
#include "FEHFile.h"
#include "FEHSD.h"
#include "math.h"

#define FORWARD 25
#define BACKWARD -25

#define SENSOR_THRESHOLD 1.5
#define TURN_ADJUSTMENT 5

#define CIRCUMFERENCE M_PI*2.5
#define TURN_DISTANCE 5.89048623

using namespace std;

FEHMotor leftMotor(FEHMotor::Motor1, 9);
FEHMotor rightMotor(FEHMotor::Motor0, 9);

DigitalEncoder leftEncoder(FEHIO::P0_0);
DigitalEncoder rightEncoder(FEHIO::P0_1);
; 

enum move {
    TURN_RIGHT,
    TURN_LEFT
};

void travel(int distance) {
    int counts = (leftEncoder.Counts()+rightEncoder.Counts())/2;
    leftMotor.SetPercent(FORWARD);
    rightMotor.SetPercent(FORWARD);

    while(CIRCUMFERENCE*counts/318 < distance) {
        counts = (leftEncoder.Counts()+rightEncoder.Counts())/2;
    }
    leftEncoder.ResetCounts();
}

void turn(int direction){
    int counts = (leftEncoder.Counts()+rightEncoder.Counts())/2;
    if(direction == TURN_RIGHT){
        leftMotor.SetPercent(BACKWARD);
        rightMotor.SetPercent(FORWARD);
    } else{
        leftMotor.SetPercent(BACKWARD);
        rightMotor.SetPercent(FORWARD);
    }

    while(CIRCUMFERENCE*counts/318 < TURN_DISTANCE) {
        counts = (leftEncoder.Counts()+rightEncoder.Counts())/2;
    }

    leftEncoder.ResetCounts();
}


void waitUntilTouch()
{
    float left;
    float right;

    while(!LCD.Touch(&left, &right)) {}

    while(LCD.Touch(&left, &right)) {}
}


int main()
{
    waitUntilTouch();
    
    while(true)
    {
        //lineFollowing();
        travel(6);
        Sleep(.1);
    }
}
