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

DigitalInputPin distanceSensor(FEHIO::P1_1);

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

void driveUntilWall(int power)
{
    leftMotor.SetPercent(power);
    rightMotor.SetPercent(power * RIGHT_MULTIPLIER);

    while (!distanceSensor.Value())
    {
    }
    stopMotors();
}

void driveDistance(int distance)
{
    int counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;
    leftMotor.SetPercent(FORWARD);
    rightMotor.SetPercent(FORWARD * RIGHT_MULTIPLIER);

    while (CIRCUMFERENCE * counts / 318 < distance)
    {
        counts = (leftEncoder.Counts() + rightEncoder.Counts()) / 2;
    }

    stopMotors();

    // LCD.Clear();

    // string line1 = "Left Encoder count: " + to_string(leftEncoder.Counts());
    // string line2 = "Right Encoder count: " + to_string(rightEncoder.Counts());

    // LCD.WriteLine(line1.c_str());
    // LCD.WriteLine(line2.c_str());

    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
}

void turn(int direction)
{
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

    leftEncoder.ResetCounts();
    rightEncoder.ResetCounts();
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
    driveUntilWall(BACKWARD);
    turn(TURN_LEFT);

    driveUntilWall(BACKWARD);
    turn(TURN_LEFT);

    driveUntilWall(BACKWARD);
}

// void correctWall()
// {
//     // driveDistance(60 cm);

//     // if no wall, drive until wall
//     if (!distanceSensor.Value())
//     {
//         driveUntilWall(FORWARD);
//     }
//     else
//     {
//         // if wall, drive until no wall

//         while (distanceSensor.Value())
//         {
//             d
//         }
//     }
// }

int main()
{
    RCS.InitializeTouchMenu("1240E4ZQS");

    while (true)
    {
        LCD.WriteLine(distanceSensor.Value());

        Sleep(.5);
    }
}
