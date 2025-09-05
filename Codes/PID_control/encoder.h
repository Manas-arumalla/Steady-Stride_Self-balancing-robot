long encoderValueL = 0;
long encoderValueR = 0;
int c=0;

long lastEncoded= 0;
int angleR= 0;
int angleL= 0;

void encoderHandlerL() {
  int MSB = digitalRead(2); //MSB = most significant bit
  int LSB = digitalRead(3); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValueL++; //CW
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValueL--; //CCW
  }

  lastEncoded = encoded; //store this value for next time 
  angleL = encoderValueL*360/810; 
  // Serial.print("L:");Serial.println(angleL);


}

void encoderHandlerR() {
  int MSB = digitalRead(20); //MSB = most significant bit
  int LSB = digitalRead(21); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValueR++; //CW
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValueR--; //CCW
  }

  lastEncoded = encoded; //store this value for next time 
  angleR = encoderValueR*360/810; 
  // Serial.print("R:");Serial.println(angleR);



}

void driveMotor(){
  
}