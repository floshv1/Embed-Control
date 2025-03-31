#include "navigation.h"

// constructor 
Navigation::Navigation (Motor* Left_Motor, Motor* Right_Motor){
    this -> Left_Motor = Left_Motor;
    this -> Right_Motor = Right_Motor;
    this -> x_coordinate = 0; 
    this -> y_coordinate = 0;
    this -> y_left_joystick = 0; 
    this -> y_right_joystick = 0;
    this -> message  = 0;
    this -> right_speed = 0;
    this -> left_speed = 0;
    this->leftCurrentSpeed = 0;
    this->rightCurrentSpeed = 0;
    this->lastUpdateTime = millis();
}

//getters 
float Navigation::getLeftJoystick(void) {
    return y_left_joystick;
}

float Navigation::getRightJoystick(void) {
    return y_right_joystick;
}

Motor* Navigation::getRightMotor(void){return Right_Motor;}

Motor* Navigation::getLeftMotor(void){return Left_Motor;}




// setters
void Navigation::setLeftMotor(Motor* motor) {
    this->Left_Motor = motor;
}

void Navigation::setRightMotor(Motor* motor) {
    this->Right_Motor = motor;
}


void Navigation::JoystickSign (float left, float right){
    y_left_joystick = (float)left*127/128;
    y_right_joystick = (float)right*127/128;
}

// transform the values in a unsign byte
void Navigation::JoystickUnsign(float left, float right){
    y_left_joystick = (float)sqrt(left*left)*255;// absolute value to have 0 255 positive
    y_right_joystick = (float)sqrt(right*right)*255; // absolute value to have 0 255 positive
}

void Navigation::ComputeSpeed() {
    // Set the smoothed speeds to the motors
    Left_Motor->controlMotor(left_speed,y_left_joystick);
    Right_Motor->controlMotor(right_speed,y_right_joystick);
}

// update the direction of the 2 motors
void Navigation::JoystickCommandDirection(){
    Left_Motor->updateDirection(y_left_joystick);
    Right_Motor->updateDirection(y_right_joystick); 
}

// Make the math based on the received value 
void Navigation::SetJoystickCommand(float left, float right){
    JoystickSign(left,right);
    left_speed = (byte)(fabs(left) * 255 );
    right_speed = (byte)(fabs(right) * 255 );
    //JoystickCommandDirection();
    //JoystickUnsign(left,right);
    ComputeSpeed();
}

float Navigation::smoothSpeed(float currentSpeed, float targetSpeed, float alpha, float dt) {
    // True exponential approach: v(t) = target - (target - initial) * e^(-alpha * t)
    float deltaSpeed = targetSpeed - currentSpeed;
    return targetSpeed - deltaSpeed * exp(-alpha * dt);
}

void Navigation::UpdateWithoutInput() {
    unsigned long currentTime = millis();

    // If we haven't received input for a while, gradually decrease speed
    if (currentTime - lastCommandTime > TIMEOUT_MS) {
        // Calculate time delta
        float dt = (currentTime - lastUpdateTime) / 1000.0f;
        lastUpdateTime = currentTime;

        // Gradually reduce current speeds
        leftCurrentSpeed *= DECAY_RATE;
        rightCurrentSpeed *= DECAY_RATE;

        // Apply the reduced speeds
        Left_Motor->setSpeed(leftCurrentSpeed);
        Right_Motor->setSpeed(rightCurrentSpeed);
    }
}
// TODO : Make the convert coordinates to indicates the direction of the robot

// TODO : Make the convert coordinates to indicates the direction of the robot