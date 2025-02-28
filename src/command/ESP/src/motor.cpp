#include "motor.h"


  // Constructor : 
Motor::Motor (int relay1,Servo* esc, int driver) {
    this->relay1 = relay1;
    this->esc = esc;
    this -> driver = driver;
    this -> actual_speed = 1000;
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
    /*
    if(esc_speed*0.8 > actual_speed*0.8){
      for(int i=actual_speed; i<esc_speed;i++){
        esc->writeMicroseconds(i);
        delay(1);
      }
    }
    else{
      for(int i=actual_speed*0.8; i>esc_speed*0.8;i--){
        esc->writeMicroseconds(i);
        delay(1);
      }
    }*/    
  }
  