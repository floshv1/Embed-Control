#include "motor.h"
#ifndef navigation
#define navigation

class Navigation{

private:
    // We use pointer to Motor class to access its methods
    Motor* Left_Motor;
    Motor* Right_Motor;
    
    float xCoordinate;
    float yCoordinate;

    float xJoystick;
    float yJoystick;   

    int Rightspeed; 
    int Leftspeed;
    
    char* message;

    bool is_arrived();
        // TODO : check if the rover is arrived to is destination with coordinates 
        // -> update the coordinatesCommand flag
        // TODO : check if the rover is arrived to is destination with coordinates 

public:
    // constructor
    Navigation(Motor *Left_Motor, Motor *Right_Motor);
    // convert the joystick values to sign float 
    void Joystick_sign(float x, float y);
    // convert the joystick values to sign float   
    void Joystick_unsign(float x, float y);

    void Joystick_command_direction();

    void Compute_Speed();

    void Set_Joystick_Command(float  x, float y);

    // Setters for esc
    void setLeftEsc(Servo esc);
    void setRightEsc(Servo esc);
    

    // TODO : Make the convert GPS coordinates to indicates the direction of the rover

    // TODO : Make the convert GPS coordinates to indicates the direction of the rover
};



#endif

