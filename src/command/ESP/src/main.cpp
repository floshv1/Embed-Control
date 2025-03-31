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
  digitalWrite(RIGHT_RELAY, HIGH);
  delay(1000);
  digitalWrite(RIGHT_RELAY, LOW);
  delay(1000);
  digitalWrite(LEFT_RELAY, HIGH);
  delay(1000);
  digitalWrite(LEFT_RELAY, LOW);

  delay(1000);
  digitalWrite(RIGHT_RELAY, HIGH);
  digitalWrite(LEFT_RELAY, HIGH);
  delay(1000);
  digitalWrite(RIGHT_RELAY, LOW);
  digitalWrite(LEFT_RELAY, LOW);  

}

void loop(){
  if(receive_Flag){
    Serial.println("Received data");
    //TODO : write the correct pin for the ESC + setup I2C connection 
    //Serial.println(temp);
    Navi.SetJoystickCommand(y_left,y_right);
    receive_Flag = false;
    delay(200);
  }
  else{
    Serial.println("No data received");
    Navi.UpdateWithoutInput();
    delay(300);
  }
}