#include "motor.h"


  // Constructor : 
Motor::Motor (int relay1,Servo* esc, int driver) {
    this->relay1 = relay1;
    this->esc = esc;
    this -> driver = driver;
    esc->attach(driver);
    esc->writeMicroseconds(1000);
    delay(1000);
    
  }
  // Update the direction of the servo motors 
  void Motor::updateDirection (float direction){
    if(direction<0){
      digitalWrite(relay1,LOW);
    }
    else{
      digitalWrite(relay1,HIGH);
    }
  }

    // Make the motor go to a certain speed
  void Motor::setSpeed(byte speed){
    int esc_speed = map(speed, 0, 255, 1000, 2000);
    esc->writeMicroseconds(esc_speed);
  }
  