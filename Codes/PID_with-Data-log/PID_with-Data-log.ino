#include <Wire.h>
// #include <Adafruit_MPU6050.h>
#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>

MPU6050 mpu(Wire);
Servo myservo1;
Servo myservo2;
File myFile;

int motor1Pin1 = 6;
int motor1Pin2 = 7;
int motor2Pin1 = 5;
int motor2Pin2 = 4;
int motorDirection = 1;
double setpoint = 0;
double Kp = 35;//10.532;
double Ki = 0.1;//18.338;
double Kd = -0.4;//1.00.577;

double error, lastError = 0;
double P, I, D;
double output;

// MPU6050 offsets
int accelXOffset = 0;
int accelYOffset = 0;
int accelZOffset = 0;
int gyroXOffset = 0;
int gyroYOffset = 0;
int gyroZOffset = 0;

// Tolerance range for the error
double errorTolerance = 0.2;

//bluetooth module code

// SoftwareSerial bluetooth (10,9);

void setup() {
  myservo1.attach(11);
  myservo1.write(102);
  delay(100);
  myservo1.detach();
  //myservo2.attach(8);

  //myservo2.write(0);
  Wire.begin();
  Serial.begin(115200);
  // bluetooth.begin(9600);
  while (!Serial){}
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    Serial.println("Hi");
    byte status = mpu.begin();
    Serial.print("MPU6050 status: ");
    Serial.println(status);
    while(status!=0){ } // stop everything if could not connect to MPU6050
    
    Serial.println(F("Calculating offsets, do not move MPU6050"));
    // mpu.calcOffsets();
    // Serial.print("Gx:"); Serial.print(mpu.getGyroXoffset());
    // Serial.print("\tGy:"); Serial.print(mpu.getGyroYoffset());
    // Serial.print("\tGz:"); Serial.print(mpu.getGyroZoffset());
    // Serial.print("\tAx:"); Serial.print(mpu.getAccXoffset());
    // Serial.print("\tAy:"); Serial.print(mpu.getAccYoffset());
    // Serial.print("\tAz:"); Serial.print(mpu.getAccZoffset());
    // delay(5000);

    // Gx:-2.83	Gy:3.19	Gz:1.33	Ax:0.02	Ay:-0.04	Az:0.04
    // Gx:-2.97	Gy:3.02	Gz:0.22	Ax:0.02	Ay:-0.02	Az:0.04
// Gx:-2.79	Gy:3.10	Gz:1.33	Ax:0.06	Ay:-0.03	Az:0.04
    mpu.setGyroOffsets(-2.79, 3.10, 1.33);
    mpu.setAccOffsets(0.06, -0.03, 0.04);
    
    // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is omounted upside-down
    Serial.println("Done!\n");


  Serial.println("");
  delay(100);

  // Calibrate MPU6050 offsets

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  Serial.print("Initializing SD card...");

  if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  SD.remove("test.txt");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print("P:");
    myFile.print(Kp);
    myFile.print(",I:");
    myFile.print(Ki);
    myFile.print(",D:");
    myFile.print(Kd);
    myFile.println("TIME,gyroAngle");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  

  mpu.update();  
  double gyroAngle = mpu.getAngleX();

  error = setpoint - gyroAngle;
  P = Kp * error;
  I += Ki * error;
  D = mpu.getGyroX();//Kd * (error - lastError);
  output = constrain(P + I + D, -255, 255);
  lastError = error;
  
  //Check if the error is within the tolerance range
  // if (fabs(error) < errorTolerance) {
  //   output = 0.0;  // Set the output to zero
  //   //I = 0.0;      // Reset the integral term
  // }

  // if (output > 255.00) {
  //   output = 254.00;
  // } else if (output < -255.00) {
  //   output = -254.00;
  // }
  int motorSpeed = abs(output);  // Map the absolute output to the motor speed range
  // Serial.println(motorSpeed);
  if (output < 0.00) {
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, motorSpeed);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, motorSpeed);
  }
  else if(output == 0){
      analogWrite(motor1Pin1, 0);
      analogWrite(motor1Pin2, 0);
      analogWrite(motor2Pin1, 0);
      analogWrite(motor2Pin2, 0);
  } 
  else {
    analogWrite(motor1Pin1, motorSpeed);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, motorSpeed);
    analogWrite(motor2Pin2, 0);
  }

  // Read gyro data

  //  Get current time
    unsigned long currentTime = millis();

    // Write data to SD card
    myFile = SD.open("test.txt", FILE_WRITE);
    if (myFile) {
      myFile.print(currentTime);
      myFile.print(",");
      myFile.println(gyroAngle);
      myFile.close();
    } else {
      Serial.println("Error");
    }

}