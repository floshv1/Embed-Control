#include <gtest/gtest.h>
#include "navigation.h"

TEST(setSpeed, test_qui_verifie_la_valeur_de_la_vitesse){
    Servo* esc = new Servo();
    Motor* motor = new Motor(0, esc, 0);
    motor->setSpeed(128);
    int expected = map(128, 0, 255, 1000, 2000);
    ASSERT_EQ(expected,motor->getSpeed());
}