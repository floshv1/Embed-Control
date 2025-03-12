#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
class Motor {
    private: 
    // the ESC objects
    Servo* esc;
    // the relay & driver pins 
    int relay1;
    int driver;

    int actual_speed;

    byte actual_direction;
    
    public : 
    // constructor 
    Motor(int relay1, Servo* esc, int driver);

    // Update the direction of the servo motors : 
    void updateDirection(float y);

    // Make the motor go to a certain speed
    void setSpeed(byte speed);

    int getSpeed();

    int getRelay();

    //TODO  : make the feedback about the speed of the motor 
    

    // TODO : make the feedback about the speed of the motor 
};
#endif