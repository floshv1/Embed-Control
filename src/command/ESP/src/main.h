// This file is for indicates the different GPIOS used for the rover
#ifndef MAIN_H
#define MAIN_H
#include "navigation.h"


// I2C address of the esp 
#define I2C_DEV_ADDR 0x52

// Pins for the relay
#define Rightrelay1 5
#define Leftrelay1 18

// Pins for the driver 

const int Rightdriver = 4;
const int Leftdriver  = 15;

// ESP settings : 
#define Baud_rate 9600

// I2C received flag 
volatile boolean receiveFlag = false; 

// I2C received data
char temp[32]; 

// Intersept position : 
float x;
float y;

// Timer intteruption
int timer_no_info =0;
int delta; 
const int Timer_stop = 20000;
// attach the esc 


#endif 