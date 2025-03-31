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

    float y_left_joystick;
    float y_right_joystick;   

    int right_speed; 
    int left_speed;
    
    char* message;

    float leftCurrentSpeed;
    float rightCurrentSpeed;
    unsigned long lastUpdateTime;

    const float DECAY_RATE = 0.95;  // Rate at which speed decays when no input (95% of current speed)
    unsigned long lastCommandTime;   // Track when we last received a command
    const unsigned long TIMEOUT_MS = 500;  // Time after which we consider connection lost

    
public:
    // constructor
    Navigation(Motor *Left_Motor, Motor *Right_Motor);
    // getters 
    float getLeftJoystick(void);
    float getRightJoystick(void);
    Motor* getRightMotor(void);
    Motor* getLeftMotor(void);

    // Setters 
    void setLeftMotor(Motor* motor);
    void setRightMotor(Motor* motor);


    // convert the joystick values to sign float 
    void JoystickSign(float left, float right);
    // convert the joystick values to sign float   
    void JoystickUnsign(float left, float right);

    void JoystickCommandDirection();

    void ComputeSpeed();

    void SetJoystickCommand(float  left, float right);
    
    float smoothSpeed(float currentSpeed, float targetSpeed,float alpha, float dt=0.1);

    void UpdateWithoutInput();
    // TODO : Make the convert GPS coordinates to indicates the direction of the rover

};

#endif

