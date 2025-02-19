/*
  TRAVELERS version DELTA 
  
  Basé sur le code de controle COSTE@2024 et réception I2C TOURON@2022

  Ce programme contrôle un microcontrôleur ESP32 qui interfère avec deux ESCs 
  (Electronic Speed Controllers) et des moteurs via la communication I2C.
*/
#include "navigation.h"
#include "main.h"


// I2C address of the esp


// Servo object to control the ESC
Servo esc_right;
Servo esc_left;

// I2C received flag 
volatile boolean receiveFlag = false; // Drapeau pour la réception de données I2C


// I2C received data
char temp[32]; 

// joystick position : 
float x;
float y;

int pwm;
// Timer intteruption
int timer_no_info =0;
int delta; 
const int Timer_stop = 20000;
// attach the esc 

// creation of the 2 Motor 
Motor* RightMotor = new Motor(Rightrelay1, 0, Rightdriver);
Motor* LeftMotor = new Motor(Leftrelay1, 0, Leftdriver);

// creation of the navigation object 
Navigation Navi = Navigation(LeftMotor, RightMotor);


//Function for reading the received data
void onReceive(int len){
  // TODO : Make different case for different input values 
  for (int j = 0; j < len; j++) {
    temp[j] = Wire.read();
    temp[j + 1] = '\0';
  }
  for (int j = 0; j < len; j++) {
    temp[j] = temp[j + 1];
  }
  Serial.println("End message");
  memmove(temp, temp + 1, strlen(temp));
  char* separator = strchr(temp,',');
  if(separator!=0){
      *separator = '\0'; // Replace the comma with a null terminator
      x = atof(temp);
      y = atof(separator+1);
      Serial.println(x);
      Serial.println(y);
  }
  else{
    Serial.println("PROBLEM DE MERDE ");
  }
// TODO : Make different case for different input values 
  receiveFlag = true;
}

void setup(){
  
    Wire.onReceive(onReceive);
    Wire.begin((uint8_t)I2C_DEV_ADDR);
    Serial.begin(Baud_rate);
  

  	pinMode(Rightrelay1,OUTPUT);
  	pinMode(Leftrelay1,OUTPUT);
  
  	// write to go in front 
  	digitalWrite(Rightrelay1,LOW);
  	digitalWrite(Leftrelay1,LOW);

    delay(1000);
    digitalWrite(Rightrelay1,HIGH);
  	digitalWrite(Leftrelay1,HIGH);
    delay(1000);
    digitalWrite(Rightrelay1,LOW);
  	digitalWrite(Leftrelay1,LOW);
    delay(2000);
    pinMode(Rightdriver,OUTPUT);
    digitalWrite(Rightdriver,HIGH);
    delay(2000);
    digitalWrite(Rightdriver,LOW);

    // Attach the ESCs
    esc_left.attach(Leftdriver);
    esc_right.attach(Rightdriver);
    
    // Set the ESCs in the navigation object
    if(!esc_left.attached())
      Navi.setLeftEsc(esc_left);
    if(!esc_right.attached())
      Navi.setRightEsc(esc_right);

    // Initialize the ESCs with minimum throttle
    esc_left.write(180);
    esc_right.write(0);
    delay(2000); // Wait for 2 seconds to allow the ESCs to initialize

    //TODO : write the correct pin for the ESC + setup I2C connection 

    //TODO : write the correct pin for the ESC + setup I2C connection 
}

void loop(){
  /*
  timer_no_info += millis() - delta;
  delta = millis();
  if(timer_no_info > Timer_stop){
    Serial.println("ETEINS TOUT ");
    Serial.println(timer_no_info);
    timer_no_info=0;
  }
  */
  if(receiveFlag){
    Serial.println("Received data");
    //TODO : write the correct pin for the ESC + setup I2C connection 
    Serial.println(temp);
    Navi.Set_Joystick_Command(x,y);
    receiveFlag = false;
  }
  else{
    Serial.println("No receive");
    delay(1000);
  }

    
}