#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>

#define LEFT 1
#define RIGHT -1

AnalogInputPin lightSensor(FEHIO::P0_0);

DigitalInputPin frontRight(FEHIO::P1_0);
DigitalInputPin backRight(FEHIO::P1_1);
DigitalInputPin frontLeft(FEHIO::P1_2);
DigitalInputPin backLeft(FEHIO::P1_3);

FEHMotor leftMotor(FEHMotor::Motor0, 9);
FEHMotor rightMotor(FEHMotor::Motor2, 9);

#define forward 25
#define backward -25

// FEHServo servo(FEHServo::Servo0);

void correction()
{
    if(!backRight.Value() && backLeft.Value()){ //Right one has hit but left hasn't
        LCD.Write("Right one hit, left not");
        leftMotor.SetPercent(backward);
        rightMotor.SetPercent(0);
    }
    if(backRight.Value() && !backLeft.Value()){ //Right one hasn't hit but left one has
        LCD.Write("Left one hit, right not");
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(backward);
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
    if (dir == LEFT)
    {
        rightMotor.SetPercent(forward);
        LCD.Write("Turning left");
    } else{
        leftMotor.SetPercent(forward);
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
    leftMotor.SetPercent(forward);
    rightMotor.SetPercent(forward);

    while(frontLeft.Value() || frontRight.Value()) {}

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

// void controlServo()
// {
//     double servoCoefficient = 180.0/3.3;
//     servo.SetDegree(lightSensor.Value() * servoCoefficient);
// }


int main()
{
    // servo.SetMax(2500);
    // servo.SetMin(500);

    // while(true)
    // {
    //     controlServo();
    // }

    float left;
    float right;

    while(!LCD.Touch(&left, &right)) {}

    while(LCD.Touch(&left, &right)) {}
    
    driveUntilHit();
    turn(RIGHT);

    driveUntilHit();
    turn(LEFT);

    driveUntilHit();
}