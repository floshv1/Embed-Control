#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>

class Motor {
    private: 
    // the ESC and reverse objects
    Servo* esc;
    Servo* reverse;
    // the relay & driver pins 
    int relay;
    int driver;
    int actual_speed;
    int actual_direction;
    
    public : 
    // constructor 
    Motor(int relay, Servo* esc, int driver, Servo* reverse);

    // Update the direction of the servo motors : 
    void updateDirection(int y);

    // getters
    int getSpeed();
    int getRelay();
    Servo* getEsc();
    Servo* getReverse();
    int getActualSpeed();

    // setters
    void setSpeed(byte speed);
    void setRelay(int relay);
    void setDriver(int driver);
    void setReverse(Servo* reverse);

    void MotorOperation(byte targetSpeed, float newDirection);
    void MotorOperationESC(byte targetSpeed);

};
#endif