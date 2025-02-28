#include "motor.h"
#ifndef NAVIGATION_H
#define NAVIGATION_H

class Navigation{

private:
    // We use pointer to Motor class to access its methods
    Motor* Left_Motor;
    Motor* Right_Motor;
    
    float x_coordinate;
    float y_coordinate;

    float x_joystick;
    float y_joystick;   

    int right_speed; 
    int left_speed;
    
    char* message;

public:
    // constructor
    Navigation(Motor *Left_Motor, Motor *Right_Motor);
    // convert the joystick values to sign float 
    void JoystickSign(float x, float y);
    // convert the joystick values to sign float   
    void JoystickUnsign(float x, float y);

    void JoystickCommandDirection();

    void ComputeSpeed();

    void SetJoystickCommand(float  x, float y);

    // Setters for motors
    void setLeftMotor(Motor* motor);
    void setRightMotor(Motor* motor);
    

    // TODO : Make the convert GPS coordinates to indicates the direction of the rover

    // TODO : Make the convert GPS coordinates to indicates the direction of the rover
};



#endif

