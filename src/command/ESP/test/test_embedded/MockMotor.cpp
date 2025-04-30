#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../lib/Motor/include/motor.h"

using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

class MockMotor : public Motor {
public:
    MockMotor() : Motor(0, nullptr, 0) {}

    MOCK_METHOD(void, setSpeed, (byte speed), (override));
    MOCK_METHOD(void, stop, (), ());
    MOCK_METHOD(int, getRelay, (), ());
    MOCK_METHOD(int, getSpeed, (), ());
};

namespace {
    class MotorTestEnvironment : public ::testing::Environment {
    public:
        void SetUp() override {
            Serial.println("Setting up Motor test environment");
        }
    };
}

// Register the test environment
testing::Environment* const motor_env = testing::AddGlobalTestEnvironment(new MotorTestEnvironment);

TEST(MotorBasicTest, BasicFunctionality) {
    Serial.println("Running basic motor test");
    MockMotor motor;
    EXPECT_CALL(motor, setSpeed(_)).Times(1);
    motor.setSpeed(50);
}

// Debug test to verify test framework
TEST(test_mocker, ketestedestrics) {
    Serial.println("Running basic test");
    EXPECT_EQ(true, true);
}

// Motor tests with debug prints
TEST(MotorTest, TestSetSpeed) {
    Serial.println("Running TestSetSpeed");
    MockMotor motor;
    ON_CALL(motor, setSpeed(_))
        .WillByDefault(testing::Return());
    
    EXPECT_CALL(motor, setSpeed(50))
        .Times(AtLeast(1));
    
    motor.setSpeed(50);
}

TEST(MotorTest, TestStop) {
    Serial.println("Running TestStop");
    MockMotor motor;
    ON_CALL(motor, stop())
        .WillByDefault(testing::Return());
    ON_CALL(motor, setSpeed(_))
        .WillByDefault(testing::Return());
    
    EXPECT_CALL(motor, stop())
        .Times(AtLeast(1));
    
    motor.stop();
}

TEST(MotorTest, TestGetSpeed) {
    Serial.println("Running TestGetSpeed");
    MockMotor motor;
    ON_CALL(motor, getSpeed())
        .WillByDefault(Return(50));
    
    EXPECT_CALL(motor, getSpeed())
        .Times(AtLeast(1));
    
    int speed = motor.getSpeed();
    EXPECT_EQ(speed, 50);
}