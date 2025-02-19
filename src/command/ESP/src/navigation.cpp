#include "navigation.h"

// constructor 
Navigation::Navigation (Motor* Left_Motor, Motor* Right_Motor){
    this -> Left_Motor = Left_Motor;
    this -> Right_Motor = Right_Motor;
    this -> xCoordinate = 0; 
    this -> yCoordinate = 0;
    this -> xJoystick = 0; 
    this -> yJoystick = 0;
    this -> message  = 0;
    this -> Rightspeed = 0;
    this -> Leftspeed = 0;
}
// Setters for esc
void Navigation::setLeftEsc(Servo esc) {
    Left_Motor->setEsc(esc);
}

void Navigation::setRightEsc(Servo esc) {
    Right_Motor->setEsc(esc);
}

void Navigation::Joystick_sign (float x, float y){
    yJoystick = (float)y*127/128;
    Serial.print("Sign y : ");
    Serial.println(yJoystick);
    
    xJoystick = (float)x*127/128;
    Serial.print("Sign x : ");
    Serial.println(xJoystick);
}

void Navigation::Joystick_unsign(float x, float y){
    yJoystick = (float)sqrt(y*y)*255;// absolute value to have 0 255 positive
    Serial.print("Unsign y : ");
    Serial.println(yJoystick);

    xJoystick = sqrt(x*x)*255; // absolute value to have 0 255 positive
    Serial.print("Unsign x : ");
    Serial.println(xJoystick);
}

void Navigation::Compute_Speed(){
    // TODO : make math to have the rigth speed for each motor
    // Calculate the speed for each motor based on x and y coordinates
    float leftSpeed = yJoystick - xJoystick;
    float rightSpeed = yJoystick + xJoystick;

    // Normalize the speeds to be within the range of -255 to 255
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Set the speeds to the motors
    Left_Motor->setSpeed(leftSpeed);
    Right_Motor->setSpeed(rightSpeed);

    Serial.print("Left Speed: ");
    Serial.println(leftSpeed);
    Serial.print("Right Speed: ");
    Serial.println(rightSpeed);
}


void Navigation::Joystick_command_direction(){
    // we ensure the rigth direction of the motors
    Left_Motor->updateDirection(yJoystick);
    Right_Motor->updateDirection(yJoystick); 

    //now we send the unsign value to control the speed;
}


void Navigation::Set_Joystick_Command(float x, float y){
    Serial.println("Joystick sign");
    Joystick_sign(x,y);
    Serial.println("update direction ");
    Joystick_command_direction();
    Joystick_unsign(x,y);
    Serial.println("Compute speed");
    Compute_Speed();
}




// TODO : Make the convert coordinates to indicates the direction of the robot

// TODO : Make the convert coordinates to indicates the direction of the robot