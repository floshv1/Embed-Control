#include <gtest/gtest.h>
#include "navigation.h"

void setup(){
  Serial.begin(115200);
  delay(1000);
  testing::InitGoogleTest();
  if(!RUN_ALL_TESTS()){
    Serial.println("Test failures Ignored : PASS");
  }
  else{
    Serial.println("Test failure Ignored: Fail");
  }
}

void loop() {}