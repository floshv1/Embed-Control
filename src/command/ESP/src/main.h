// This file is for indicates the different GPIOS used for the rover
#ifndef MAIN_H
#define MAIN_H
#include "navigation.h"


// I2C address of the esp 
#define I2C_DEV_ADDR 0x52

// I2C pin assignments for ESP32
#define I2C_SDA_PIN 21  // Default SDA pin for ESP32
#define I2C_SCL_PIN 22  // Default SCL pin for ESP32

// Pins for the relay
#define RIGHT_RELAY 19
#define LEFT_RELAY 33

// Pins for the driver 

const int Rightdriver = 18;
const int Leftdriver  = 32;

// ESP settings : 
#define BAUD_RATE 115200
#define BUFFER_SIZE 32  // Adjust if needed
// I2C received flag 
volatile boolean receive_Flag = false; 

// I2C received data
char temp[BUFFER_SIZE];

// Intersept position : 
float y_left;
float y_right;

// Timer intteruption
int timer_no_info = 0;
int delta; 
const int Timer_stop = 20000;
// attach the esc 

#endif