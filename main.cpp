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

// FEHServo servo(FEHServo::Servo0);

void correction()
{
    while(!backRight.Value() && backLeft.Value()){
        leftMotor.SetPercent(0);
        rightMotor.SetPercent(25);
    }
    while(backRight.Value() && !backLeft.Value()){
        leftMotor.SetPercent(25);
        rightMotor.SetPercent(0);
    }
    
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void turn(int dir)
{
    if (dir == LEFT)
    {
        rightMotor.SetPercent(25);
    } else{
        leftMotor.SetPercent(25);

    }

    while(!backRight.Value() || !backLeft.Value()) {}

    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);

    correction();
}

void driveUntilHit()
{
    leftMotor.SetPercent(-25);
    rightMotor.SetPercent(-25);

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