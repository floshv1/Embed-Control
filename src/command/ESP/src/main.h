// This file is for indicates the different GPIOS used for the rover
#ifndef main.h
#define main.h

// I2C address of the esp 
#define I2C_DEV_ADDR 0x52

// Pins for the relay
#define Rightrelay1 5
#define Leftrelay1 18

// Pins for the driver 
const int  Rightdriver = 4;
const int  Leftdriver  = 15;


// ESP settings : 
#define Baud_rate 9600

#endif 