#include "main.h"


// Servo object to control the ESC
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
  for (int j = 0; j < len; j++) {
    temp[j] = temp[j + 1];
  }
  Serial.println("End message");
  memmove(temp, temp + 1, strlen(temp));
  char* separator = strchr(temp,',');
  if(separator!=0){
      *separator = '\0'; // Replace the comma with a null terminator
      x = atof(temp);
      y = atof(separator+1);
  }
  else{
    Serial.println("Error listening");
  }
// TODO : Make different case for different input values 
  receive_Flag = true;
}

void setup(){
  Wire.onReceive(onReceive);
  Wire.begin((uint8_t)I2C_DEV_ADDR);
  Serial.begin(BAUD_RATE);
  
  // Initialize SErvo object 
  Serial.println("Initializing Motors...");

  // Check if Servo objects are initialized
  if (esc_right == nullptr || esc_left == nullptr) {
    Serial.println("Error: Failed to initialize Servo objects");
    while (true); // Halt execution
}

  Serial.println("Creating Motor objects...");
  
  pinMode(RIGHT_RELAY, OUTPUT);
  pinMode(LEFT_RELAY, OUTPUT);

  // Initialize relays
  digitalWrite(RIGHT_RELAY, LOW);
  digitalWrite(LEFT_RELAY, LOW);

  // Check if Motor objects are initialized
  if (Right_Motor == nullptr || Left_Motor == nullptr) {
    Serial.println("Error: Failed to initialize Motor objects");
    while (true); // Halt execution
  }
  delay(1000);
  digitalWrite(RIGHT_RELAY, HIGH);
  digitalWrite(LEFT_RELAY, HIGH);
  delay(1000);
  digitalWrite(RIGHT_RELAY, LOW);
  digitalWrite(LEFT_RELAY, LOW);  

}

void loop(){
  /*
  timer_no_info += millis() - delta;
  delta = millis();
  if(timer_no_info > Timer_stop){
    Serial.println("ETEINS TOUT ");
    Serial.println(timer_no_info);
    timer_no_info=0;
  }
  */
  if(receive_Flag){
    Serial.println("Received data");
    //TODO : write the correct pin for the ESC + setup I2C connection 
    Serial.println(temp);
    Navi.SetJoystickCommand(x,y);
    receive_Flag = false;
    delay(300);
  }
  else{
    Serial.println("No data received");
    Navi.SetJoystickCommand(0,0);
    delay(300);
  }
}