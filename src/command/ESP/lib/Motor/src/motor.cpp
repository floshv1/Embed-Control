#include "motor.h"

// Constructor : 
Motor::Motor(int relay1, Servo* esc, int driver, Servo* reverse) {
  this->relay = relay1;
  this->esc = esc;
  this->reverse = reverse;
  this->driver = driver;
  this->actual_speed = 1000;
  this->actual_direction = 0;
  this->reverse->attach(relay1);
  this->reverse->writeMicroseconds(1000);
  this->esc->attach(driver);
  this->esc->writeMicroseconds(1000);
  delay(1000);
}

// getters
int Motor::getSpeed() { return this->actual_speed; }
int Motor::getRelay() { return this->relay; }
Servo* Motor::getEsc() { return esc; }
Servo* Motor::getReverse() { return reverse; }
int Motor::getActualSpeed() { return this->actual_speed; }

// setters
void Motor::setSpeed(byte speed) {
  int esc_speed = map(speed, 0, 255, 1000, 2000);
// Only ramp if the difference is significant
  if (esc_speed > actual_speed) {
    for (int i = actual_speed; i <= esc_speed; i += 2) {
      esc->writeMicroseconds(i);
      delay(0.1); 
    }
  } else {
    for (int i = actual_speed; i >= esc_speed; i -= 2) {
      esc->writeMicroseconds(i);
      delay(0.1); 
    }
  }
  actual_speed = esc_speed;
}

void Motor::setRelay(int relay) { this->relay = relay; }
void Motor::setDriver(int driver) { this->driver = driver; }
void Motor::setReverse(Servo* reverse) { this->reverse = reverse; }

void Motor::updateDirection(int direction) {  
  if (direction > 0) {
    reverse->writeMicroseconds(1000);
    Serial.println("→ Reverse pin set to FORWARD");
  } else {
    reverse->writeMicroseconds(2000);
    Serial.println("← Reverse pin set to REVERSE");
  }
  delay(100);
  actual_direction = direction;
}

void Motor::MotorOperation(byte targetSpeed, float newDirection) {
  int desired = (newDirection >= 0) ? 1 : -1;
  int current = (actual_direction >= 0) ? 1 : -1;

  if (desired != current) {
    Serial.println("↔ Direction change needed");
    setSpeed(0);
    Serial.println("⚠ Throttle set to 0");
    updateDirection(desired);
  }
  setSpeed(targetSpeed);
}