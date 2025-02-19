#include "motor.h"


  // Constructor : 
Motor::Motor (int relay1,int relay2, int driver) {
    this->relay1 = relay1;
    this -> relay2 = relay2;
    this -> driver = driver;
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
    int escSpeed = map(sqrt(speed*speed), -255, 255, 1000, 2000);
    esc.writeMicroseconds(escSpeed);
    Serial.print("esc : ");
    Serial.println(esc.read());
  }
  