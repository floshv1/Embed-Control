#include <unity.h>
#include "motor.h"

Servo* esc = new Servo();
Motor* motor = new Motor(0, esc, 0);

void test_setSpeed(){
    float speed = 0.5;
    motor->setSpeed(speed);
    TEST_ASSERT_EQUAL_FLOAT(speed, motor->getSpeed());
}

void test_updateDirection(){
    float direction = 1.0;
    motor->updateDirection(direction);
    int relay = motor->getRelay();
    TEST_ASSERT_EQUAL(HIGH,digitalRead(relay));
}

void test_stop(){
    motor->setSpeed(0);
    TEST_ASSERT_EQUAL_FLOAT(0, motor->getSpeed());
}

void setup(){
    Serial.begin(115200);
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_setSpeed);
    RUN_TEST(test_updateDirection);
    RUN_TEST(test_stop);
    UNITY_END();
}

void loop(){}