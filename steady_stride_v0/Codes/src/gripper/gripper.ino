// Include the library
#include <Servo.h>

// Create the servo object
Servo myservo;

// Setup section to run once
void setup() {
  myservo.attach(10); // attach the servo to our servo object
}

// Loop to keep the motor turning!
void loop() {

  myservo.write(135); // rotate the motor clockwise
  delay(5000);
  myservo.write(45);
  delay(5000); // keep rotating 
}