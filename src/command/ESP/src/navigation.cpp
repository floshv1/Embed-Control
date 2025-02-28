#include "navigation.h"

// constructor 
Navigation::Navigation (Motor* Left_Motor, Motor* Right_Motor){
    this -> Left_Motor = Left_Motor;
    this -> Right_Motor = Right_Motor;
    this -> x_coordinate = 0; 
    this -> y_coordinate = 0;
    this -> x_joystick = 0; 
    this -> y_joystick = 0;
    this -> message  = 0;
    this -> right_speed = 0;
    this -> left_speed = 0;
}

void Navigation::setLeftMotor(Motor* motor) {
    this->Left_Motor = motor;
}

void Navigation::setRightMotor(Motor* motor) {
    this->Right_Motor = motor;
}

void Navigation::JoystickSign (float x, float y){
    y_joystick = (float)y*127/128;
    x_joystick = (float)x*127/128;
}

// transform the values in a unsign byte
void Navigation::JoystickUnsign(float x, float y){
    y_joystick = (float)sqrt(y*y)*255;// absolute value to have 0 255 positive
    x_joystick = sqrt(x*x)*255; // absolute value to have 0 255 positive
}

void Navigation::ComputeSpeed(){
    // TODO : make math to have the rigth speed for each motor
    // Calculate the speed for each motor based on x and y coordinates
    float leftSpeed = y_joystick - x_joystick;
    float rightSpeed = y_joystick + x_joystick;

    // Normalize the speeds to be within the range of -255 to 255
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Set the speeds to the motors
    Left_Motor->setSpeed(leftSpeed);
    Right_Motor->setSpeed(rightSpeed);
}

// update the direction of the 2 motors
void Navigation::JoystickCommandDirection(){
    Left_Motor->updateDirection(y_joystick);
    Right_Motor->updateDirection(y_joystick); 
}

// Make the math based on the received value 
void Navigation::SetJoystickCommand(float x, float y){
    JoystickSign(x,y);
    JoystickCommandDirection();
    JoystickUnsign(x,y);
    ComputeSpeed();
}




// TODO : Make the convert coordinates to indicates the direction of the robot

// TODO : Make the convert coordinates to indicates the direction of the robot