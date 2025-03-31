#include "motor.h"


  // Constructor : 
Motor::Motor (int relay1,Servo* esc, int driver) {
  this->relay = relay1;
  this->esc = esc;
  this -> driver = driver;
  this -> actual_speed = 1000;
  this -> actual_direction = 0;

  esc->attach(driver);
  esc->writeMicroseconds(1000);

  delay(1000);
  
}

// getters
int Motor::getSpeed(){ return this-> actual_speed;}
int Motor::getRelay(){ return this -> relay;}
Servo* Motor::getEsc(){return esc;}
int Motor::getActualSpeed(){return this->actual_speed;}

// setters
void Motor::setSpeed(byte speed){
  int esc_speed = map(speed, 0, 255, 1000, 2000);  
  //esc->writeMicroseconds(esc_speed);
  if(esc_speed > actual_speed){
    for(int i=actual_speed; i<esc_speed;i+=2){
      esc->writeMicroseconds(i);
      delay(0.1);
    }
  }
  else{
    for(int i=actual_speed; i>esc_speed;i-=2){
      esc->writeMicroseconds(i);
      delay(0.1);
    }
  }
  actual_speed=esc_speed;
}

void Motor::setRelay(int relay){ this->relay = relay;}
void Motor::setDriver(int driver){ this->driver = driver;}


// Update the direction of the servo motors 
void Motor::updateDirection (float direction){
  try{
    if(direction>=0){
      digitalWrite(relay,LOW);
    }
    else{
      digitalWrite(relay,HIGH);
    }
    actual_direction = direction;
  }
  catch(float my_direction){
    Serial.println("Direction is not good");
    throw(direction);
  }
}


void Motor::controlMotor(byte targetSpeed, float newDirection) {
  // We'll use 1 for positive and -1 for negative.
  int desiredDirection = (newDirection >= 0) ? 1 : -1;
  
  // Check if a direction change is required.
  if (desiredDirection != actual_direction ) {
      setSpeed(0);
      delay(0.1);
      updateDirection(newDirection);
  }
  // Finally, if the target speed is nonzero, accelerate to it.
  setSpeed(targetSpeed);
  
}
