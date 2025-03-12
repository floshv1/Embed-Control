#include <unity.h>
#include "motor.h"


// Test for the speed 

void test_setSpeed_half(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    byte speed = 128;
    motor->setSpeed(speed);
    int expected = map(speed, 0, 255, 1000, 2000);
    TEST_ASSERT_EQUAL_INT(expected, motor->getSpeed());
}
void test_setSpeed_full(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    byte speed = 255;
    motor->setSpeed(speed);
    int expected = map(speed, 0, 255, 1000, 2000);
    TEST_ASSERT_EQUAL_INT(expected, motor->getSpeed());
}
void test_setSpeed_zero(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    byte speed = 0;
    motor->setSpeed(speed);
    int expected = map(speed, 0, 255, 1000, 2000);
    TEST_ASSERT_EQUAL_INT(expected, motor->getSpeed());
}
void test_setSpeed_negative(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    byte speed = 120;
    motor->setSpeed(speed);
    int expected = map(speed, 0, 255, 1000, 2000);
    TEST_ASSERT_EQUAL_INT(expected, motor->getSpeed());
}

void test_setSpeed(void){
    RUN_TEST(test_setSpeed_half);
    RUN_TEST(test_setSpeed_full);
    RUN_TEST(test_setSpeed_zero);
    RUN_TEST(test_setSpeed_negative);
}

void test_updateDirection_front(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    float direction = 1.0;
    motor->updateDirection(direction);
    int relay = motor->getRelay();
    TEST_ASSERT_EQUAL(HIGH,digitalRead(relay));
}

void test_updateDirection_back(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    float direction = 0;
    motor -> updateDirection(direction);
    int relay = motor->getRelay();
    TEST_ASSERT_EQUAL(LOW,digitalRead(relay));
}

void test_updateDirection_error(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    float direction;
    motor -> updateDirection(direction);
    int relay = motor->getRelay();
    TEST_ASSERT_EQUAL(LOW,digitalRead(relay));
}


void test_stop(){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    motor->setSpeed(0);
    TEST_ASSERT_EQUAL_FLOAT(0, motor->getSpeed());
}
