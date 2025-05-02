#include "main.h"
#include <ESP32Servo.h>

const int escPin = 23; // Change this to the actual pin your ESC signal wire is connected to
Servo esc;

// Servo object to control the this ESC
Servo* esc_right = new Servo();
Servo* esc_left = new Servo();

// creation of the 2 Motor 
Motor* Right_Motor  = new Motor(RIGHT_RELAY, esc_right, Rightdriver);
Motor* Left_Motor = new Motor(LEFT_RELAY, esc_left, Leftdriver);

// creation of the navigation object 
Navigation Navi = Navigation(Left_Motor, Right_Motor);

//Function for reading the received data
void onReceive(int len){
  // TODO : Make different case for different input values 
  for (int j = 0; j < len; j++) {
    temp[j] = Wire.read();
    temp[j + 1] = '\0';
  }
  bool isnegative = (temp[0] == '-');
  for (int j = 0; j < len; j++) {
    temp[j] = temp[j + 1];
  }
  Serial.print("Raw received [");
  for(int i = 0; i < len; i++) {
      Serial.print((char)temp[i]);
      Serial.print(" (");
      Serial.print((int)temp[i]);
      Serial.print(") ");
  }
  Serial.println("]");

  char* start = strchr(temp, '(');
  if(!start){
    Serial.println("Error: No opening parenthesis found in string: ");
    return;
  }
  char* end = strchr(temp,')');
  *end = '\0';
  memmove(temp, temp + 1, strlen(temp));
  char* separator = strchr(temp,',');
  if(separator!=0){
      *separator = '\0'; // Replace the comma with a null terminator
      // TODO : put the minus bool in operation 
      y_left = atof(temp);
      y_right = atof(separator+1);
      Serial.print("right : ");
      Serial.println(y_right);
  }
  else{
    Serial.println("Error listening");
  }
// TODO : Make different case for different input values 
  receive_Flag = true;
}

void setup(){
  Wire.onReceive(onReceive);
  Wire.begin(); // SDA = GPIO21, SCL = GPIO22
  Serial.begin(BAUD_RATE);
  Serial.println("Initializing Motors...");

  if (esc_right == nullptr || esc_left == nullptr) {
    Serial.println("Error: Failed to initialize Servo objects");
    while (true); // Halt execution
  }
  Serial.println("Creating Motor objects...");

  // Check if Motor objects are initialized
  if (Right_Motor == nullptr || Left_Motor == nullptr) {
    Serial.println("Error: Failed to initialize Motor objects");
    while (true); // Halt execution
  }

  esc.attach(escPin);
  esc.writeMicroseconds(1000); // Min throttle
  Serial.println("Arming ESC...");
  delay(5000); // Wait for arming
}

void loop(){
  if(receive_Flag){
    Serial.println("Received data");
    Navi.ApplyJoystickCommand(y_left,y_right);
    receive_Flag = false;
    delay(200); 
  }
  else{
    Serial.println("No data received");
    Navi.UpdateWithoutInput();
    delay(300);
  }

  // Ramp up throttle
  for (int pw = 1000; pw <= 2000; pw += 50) {
    esc.writeMicroseconds(pw);
    Serial.println(pw);
    delay(500);
  }
  while (1); // Stop
}
