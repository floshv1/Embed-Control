#include <unity.h>
#include <Arduino.h>

#include "motor.h"
#include "navigation.h"

#include "Navigation_test.hpp"
#include "Motor_test.hpp"

void setup() {
    Serial.begin(115200);
    delay(1000);
    UNITY_BEGIN();
    // Run test for the motors 
    // test for setSpeed 
    test_setSpeed();
    UNITY_END();
}

void loop() {
    // Leave empty
}