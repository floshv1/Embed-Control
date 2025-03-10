#include <unity.h>
#include "navigation.h"
Servo* esc1 = new Servo();
Servo* esc2 = new Servo();
Motor* motor1 = new Motor(0, esc1, 0);
Motor* motor2 = new Motor(0, esc2, 0);
Navigation nav = Navigation(motor1, motor2);

void test_JoystickSign(){
    float left_value = 0.4;
    float right_value = -0.3;
    nav.JoystickSign(left_value, right_value);
    float search_left_value = abs(left_value) * 127 / 128;
    float search_right_value = abs(right_value) * 127 / 128;
    TEST_ASSERT_EQUAL_FLOAT(search_left_value, nav.getLeftJoystick());
    TEST_ASSERT_EQUAL_FLOAT(search_right_value, nav.getRightJoystick());
}



void setup(){
    Serial.begin(115200);
    delay(1000);
    UNITY_BEGIN();
    RUN_TEST(test_JoystickSign);

    UNITY_END();
}

void loop(){}