#include "motor.h"


  // Constructor : 
Motor::Motor (int relay,int relay2, Servo esc, int driver) {
    this->relay1 = relay;
    this -> relay2 = relay2;
    this -> driver = driver;
    this-> esc = esc;
  }
  // Update the direction of the servo motors :
  void Motor::updateDirection (float y){
    // we only consider y for the  direction 
    if(y<0){
      digitalWrite(relay1,LOW);
    }
    else{
      digitalWrite(relay1,HIGH);
    }
  }
  void Motor::setEsc(Servo esc) {
    this->esc = esc;
  }

  void Motor::setSpeed(float speed){
    int escSpeed = map(speed, -255, 255, 1000, 2000);
    esc.writeMicroseconds(speed*0.8);
  }
  