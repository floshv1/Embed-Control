#include "main.h"
#include <ESP32Servo.h>


// Servo object to control the this ESC
Servo* esc_right = new Servo();
Servo* esc_left = new Servo();

Servo* esc = new Servo();
Servo* reverse = new Servo();

Servo* reverse_right = new Servo();
Servo* reverse_left = new Servo();
// creation of the 2 Motor 
Motor* Right_Motor  = new Motor(15, esc_right, 23,reverse_right);
Motor* Left_Motor = new Motor(LEFT_RELAY, esc_left, Leftdriver,reverse_left);

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
  Wire.begin(21,22,I2C_DEV_ADDR); // Use default SDA=21, SCL=22
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

  //esc.attach(escPin);
  //esc.writeMicroseconds(1000); // Min throttle
  Serial.println("Arming ESC...");
  delay(5000); 
}

void loop(){
  static String inputString = "";
  static bool inputComplete = false;
  delay(20);  // cadence stable (~50Hz)
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      inputComplete = true;
      break;
    } else {
      inputString += inChar;
    }
  }

  if (inputComplete && inputString.length() > 0) {
    float value = inputString.toFloat(); // Accepts values like -1.0 to 1.0
    value = constrain(value, -1.0, 1.0);

    // Map value to speed and direction
    byte speed = (byte)(fabs(value) * 255);
    float direction = value;

    Serial.print("Received value: ");
    Serial.print(value);
    Serial.print(" | Speed: ");
    Serial.print(speed);
    Serial.print(" | Direction: ");
    Serial.println(direction);

    // Example: control right motor
    Navi.ApplyJoystickCommand(value, value);

    inputString = "";
    inputComplete = false;
  }
  // ...optional: add a small delay to avoid flooding
  delay(10);
  /*
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
  }*/
}
