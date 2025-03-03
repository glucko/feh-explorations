#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <string>
#include "FEHSD.h"
#include "math.h"
#include "FEHRCS.h"

#define FORWARD 25
#define BACKWARD -25

#define RIGHT_MULTIPLIER -1

#define CIRCUMFERENCE M_PI * 2.5
#define TURN_DISTANCE 5.89048623

using namespace std;

FEHMotor leftMotor(FEHMotor::Motor0, 9);
FEHMotor rightMotor(FEHMotor::Motor1, 9);

DigitalEncoder leftEncoder(FEHIO::P0_0);
DigitalEncoder rightEncoder(FEHIO::P0_1);

DigitalInputPin distanceSensor(FEHIO::P1_0);

// rcs string: 1240E4ZQS
enum move
{
    TURN_RIGHT,
    TURN_LEFT
};

void stopMotors()
{
    leftMotor.SetPercent(0);
    rightMotor.SetPercent(0);
}

void driveUntilWall(int power, bool wallDetected)
{
    leftMotor.SetPercent(power);
    rightMotor.SetPercent(power * RIGHT_MULTIPLIER);

    // if waiting until wall detected, wait until distance sensor is false
    if (wallDetected)
    {
        while (distanceSensor.Value())
        {
            LCD.WriteLine(distanceSensor.Value());
        }
    }
    else
    {
        // else, wait until no wall is detected
        while (!distanceSensor.Value())
        {
            LCD.WriteLine(distanceSensor.Value());
        }
    }

    stopMotors();
}

void driveDistance(int distance)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    int counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;

    if (distance > 0)
    {
        leftMotor.SetPercent(FORWARD);
        rightMotor.SetPercent(FORWARD * RIGHT_MULTIPLIER);
    }
    else
    {
        leftMotor.SetPercent(BACKWARD);
        rightMotor.SetPercent(BACKWARD * RIGHT_MULTIPLIER);
        distance *= -1;
    }

    while (CIRCUMFERENCE * counts / 318 < distance)
    {
        counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;
    }

    stopMotors();
}

void turn(int direction)
{
    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();

    int counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;
    if (direction == TURN_RIGHT)
    {
        leftMotor.SetPercent(FORWARD);
        rightMotor.SetPercent(BACKWARD * RIGHT_MULTIPLIER);
    }
    else
    {
        leftMotor.SetPercent(BACKWARD);
        rightMotor.SetPercent(FORWARD * RIGHT_MULTIPLIER);
    }

    while (CIRCUMFERENCE * counts / 318 < TURN_DISTANCE)
    {
        counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;
    }

    stopMotors();
}

void waitUntilTouch()
{
    float left;
    float right;

    while (!LCD.Touch(&left, &right))
    {
    }

    while (LCD.Touch(&left, &right))
    {
    }
}

void maze()
{
    // part 1
    driveUntilWall(BACKWARD, true);
    driveDistance(1);
    turn(TURN_LEFT);

    LCD.WriteLine("Finished turn 1");
    Sleep(.5);

    // part 2
    driveUntilWall(BACKWARD, true);
    driveDistance(3);
    turn(TURN_RIGHT);

    LCD.WriteLine("Finished turn 2");
    Sleep(.5);

    // part 3
    driveUntilWall(BACKWARD, true);

    LCD.WriteLine("finished");
}

void correctWall()
{
    driveDistance(-(60 / 2.54));

    LCD.WriteLine("finished driving distance");

    driveUntilWall(25, distanceSensor.Value());
}

int main()
{
    waitUntilTouch();
    correctWall();
}
