#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "main.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Starting test setup...");
    
    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock();
    ::testing::InitGoogleTest();
    
    Serial.println("Running tests...");
    if (RUN_ALL_TESTS() == 0) {
        Serial.println("ALL TESTS PASSED");
    } else {
        Serial.println("SOME TESTS FAILED");
    }
    Serial.println("Tests completed");
}

void loop() {
    delay(1000);
}