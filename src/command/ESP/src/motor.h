#ifndef motor
#define motor
#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
class Motor {
    private: 
    // the ESC objects
    Servo esc;
    // the relay & driver pins 
    int relay1;
    int relay2;
    int driver;


    public : 
    Motor();
    // constructor 
    Motor(int relay1, int relay2, int driver);

    // Update the direction of the servo motors : 
    void updateDirection(float y);

    void setSpeed(float speed);
    void setEsc(Servo esc);

    //TODO  : make the feedback about the speed of the motor 
    

    // TODO : make the feedback about the speed of the motor 
};


#endif