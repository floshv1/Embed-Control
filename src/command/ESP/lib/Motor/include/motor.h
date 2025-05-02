#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <Wire.h>

#include <ESP32Servo.h>
class Motor {
    private: 
    // the ESC objects
    Servo* esc;
    Servo* reverse;
    // the relay & driver pins 
    int relay;
    int driver;
    int actual_speed;
    byte actual_direction;
    
    public : 
    // constructor 
    Motor(int relay, Servo* esc, int driver);

    // Update the direction of the servo motors : 
    void updateDirection(float y);

    // getters
    int getSpeed();
    int getRelay();
    Servo* getEsc();
    int getActualSpeed();

    // setters
    void setSpeed(byte speed);
    void setRelay(int relay);
    void setDriver(int driver);


    void MotorOperation(byte targetSpeed, float newDirection);
    void MotorOperationESC(byte targetSpeed);
    //TODO  : make the feedback about the speed of the motor 

};
#endif