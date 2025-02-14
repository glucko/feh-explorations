#include <FEHLCD.h>

#include <FEHIO.h>

#include <FEHUtility.h>

#include <FEHMotor.h>

#include <FEHServo.h>

#include <FEHAccel.h>

 

#define FRONT 0

#define RIGHT 1

#define BACK 2

#define LEFT 3

 

//AnalogInputPin lightSensor(FEHIO::P0:0);

 

DigitalInputPin frontRight(FEHIO::P1_1);

DigitalInputPin backRight(FEHIO::P1_0);

DigitalInputPin frontLeft(FEHIO::P1_2);

DigitalInputPin backLeft(FEHIO::P3_0);

 

FEHMotor leftMotor(FEHMotor::Motor0, 9);

FEHMotor rightMotor(FEHMotor::Motor2, 9);

 

#define forward 25

#define backward -25

 

// FEHServo servo(FEHServo::Servo0);

 

//Assumes that person is inputting either LEFT or RIGHT

void turn(int dir)

{

    if(dir == LEFT)

    {

        leftMotor.SetPercent(backward);

    }

    else

    {

        rightMotor.SetPercent(backward);

    }

    while(backRight.Value() && backLeft.Value()) //While neither has been hit

    {

        Sleep(0.1);

    }

    leftMotor.SetPercent(0);

    rightMotor.SetPercent(0);

}

 

bool hitSideWithOne(int dir)

{

    if(dir == FRONT)

    {

        return !(frontRight.Value() && frontLeft.Value()); //Returns 1 if either is hit

    }

    else

    {

        return !(backRight.Value() && backLeft.Value()); //Returns 1 if either is hit

    }

}

 

bool flushWithWall(int dir)

{

    if(dir == FRONT)

    {

        return !(frontRight.Value() || frontLeft.Value()); //Returns 1 if both are hit

    }

    else

    {

        return !(backRight.Value() || backLeft.Value()); //Returns 1 if both are hit

    }

}

 

void correction(int dir)

{

    if(dir == FRONT)

    {

        if(frontRight.Value()) //Front right is not touching wall

        {

            rightMotor.SetPercent(forward);

        }

        else //Front left is not touching wall

        {

            leftMotor.SetPercent(forward);

        }

    }

    else

    {

        if(backRight.Value()) //Back right is not touching wall

        {

            rightMotor.SetPercent(backward);

        }

        else //Back left is not touching wall

        {

            leftMotor.SetPercent(backward);

        }

    }

    while(!flushWithWall(dir))

    {

        Sleep(0.1);

    }

}

 

void driveForwardUntilHit()

{

    leftMotor.SetPercent(forward);

    rightMotor.SetPercent(forward);

    while(!hitSideWithOne(FRONT))

    {

        Sleep(0.1);

    }

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

   
    LCD.Write("Driving Forward");
    driveForwardUntilHit();
    LCD.Write("Correcting");
    correction(FRONT);
    LCD.Write("Turning");
    turn(RIGHT);

    correction(BACK);

 

    driveForwardUntilHit();

    correction(FRONT);

    turn(LEFT);

    correction(BACK);

 

    driveForwardUntilHit();

    correction(FRONT);

}