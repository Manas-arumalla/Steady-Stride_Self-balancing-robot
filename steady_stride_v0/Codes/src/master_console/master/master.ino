/*
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(0, 1);
*/

#define pan_CLK 5
#define pan_DT 4
#define tilt_CLK 9
#define tilt_DT 8

int motor_state,stand_state,gripper_state,pan_state,tilt_state;

int joy1, joy2;
char joy1c,joy2c;
const int stand_button = 10;
const int cw_gripper = 11;
const int ccw_gripper = 12;
int prev_pan_counter;
int pan_counter = 90;
int pan_current_CLK;
int pan_last_CLK;
int prev_tilt_counter = 0;
int tilt_counter = 90;
int tilt_current_CLK;
int tilt_last_CLK;
int stand_button_state = 0;
int cw_gripper_state = 0;
int ccw_gripper_state = 0;

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
  pinMode(stand_button,INPUT);
  pinMode(cw_gripper,INPUT);
  pinMode(ccw_gripper,INPUT);
  pinMode(pan_CLK,INPUT);
  pinMode(pan_DT,INPUT);
  pan_last_CLK = digitalRead(pan_CLK);
}

void loop() {
  
  joy1 = analogRead(A2); // Read Joysticks X-axis
  if(joy1<=505)
  { 
    motor_state=1;
    Serial.print(motor_state);
    bluetooth.print(motor_state);
  }
  if (joy1>=525)
  {
    motor_state=2;
    Serial.print(motor_state);
    bluetooth.print(motor_state);
  }
  joy2 = analogRead(A6);
  if(joy2<=485)
  {
    motor_state=3;
    Serial.print(motor_state);   
    bluetooth.print(motor_state);   
  }
  if (joy2>=505)
  {
    motor_state=4; 
    Serial.print(motor_state);       
    bluetooth.print(motor_state);
  }
  /*
  stand_button_state = digitalRead(stand_button);
  if (stand_button_state == HIGH){
    Serial.write(5);
  }
  else{
    //Serial.println('n');
  }
  cw_gripper_state = digitalRead(cw_gripper);
  if (cw_gripper_state == HIGH){
    Serial.write(6);
  }
  ccw_gripper_state = digitalRead(ccw_gripper);
  if (ccw_gripper_state == HIGH){
    Serial.write(7);
  } 
  pan_current_CLK = digitalRead(pan_CLK);
  if(pan_current_CLK!=pan_last_CLK&&pan_current_CLK==1){
    if (digitalRead(pan_DT)!=pan_current_CLK){
      pan_counter --;
      if (pan_counter<0)
      pan_counter = 0;
    }
    else{
      pan_counter++;
      if(pan_counter>179)
      pan_counter=179;
    }
    if (pan_counter>prev_pan_counter)
    {
      Serial.write(8);
    }
    else if(pan_counter<prev_pan_counter)
    {
      Serial.write(9);
    }
    prev_pan_counter=pan_counter;
  }
  pan_last_CLK = pan_current_CLK;
  tilt_current_CLK = digitalRead(tilt_CLK);
  if(tilt_current_CLK!=tilt_last_CLK&&tilt_current_CLK==1){
    if (digitalRead(tilt_DT)!=tilt_current_CLK){
      tilt_counter--;
      if(tilt_counter<0)
      tilt_counter = 0;
    }
    else{
      tilt_counter++;
      if(tilt_counter>179)
      tilt_counter=179;
    }
    if (tilt_counter>prev_tilt_counter)
    {
      Serial.write(0);
    } 
    else if(tilt_counter<prev_tilt_counter)
    {
      Serial.write();
    }  
    prev_tilt_counter=tilt_counter;      
  }
  tilt_last_CLK = tilt_current_CLK;
  */
}