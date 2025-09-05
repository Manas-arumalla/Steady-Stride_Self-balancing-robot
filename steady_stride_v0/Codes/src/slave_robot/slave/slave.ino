#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

Adafruit_MPU6050 mpu;
Servo standservo1;
Servo standservo2;
Servo gripper;
Servo pan;
Servo tilt;

#define D0 2
#define D1 3
#define D2 4
#define D3 5

int pan_pos = 90;

int tilt_pos = 90;

int self_count = 0;

int motorDirection = 0;
double setpoint = 0;
double Kp = 10.532;
double Ki = 18.338;
double Kd = 1.077;

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
double errorTolerance = 1.0;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
  Serial.println("");
  delay(100);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  standservo1.attach(11);
  standservo2.attach(12); 
  gripper.attach(10);
  pan.attach(8);
  tilt.attach(9); 
  pan.write(90);
  tilt.write(90);
  standservo1.write(0);
  standservo2.write(0);
  Serial.begin(9600);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply offsets to accelerometer and gyroscope readings
  int accelX = a.acceleration.x - accelXOffset;
  int accelY = a.acceleration.y - accelYOffset;
  int accelZ = a.acceleration.z - accelZOffset;
  int gyroX = g.gyro.x - gyroXOffset;
  int gyroY = g.gyro.y - gyroYOffset;
  int gyroZ = g.gyro.z - gyroZOffset;

  double angle = atan2(accelY, accelZ) * 180 / PI;
  double gyroAngle = angle + (gyroX / 131.0) * 0.98;

  error = setpoint - gyroAngle;
  P = Kp * error;
  I += Ki * error;
  D = Kd * (error - lastError);
  output = P + I + D;
  lastError = error;
  
  standservo1.write(0);
  standservo2.write(0);
  pan.write(80);
  
  // Check if the error is within the tolerance range
  if (fabs(error) < errorTolerance) {
    output = 0.0;  // Set the output to zero
    I = 0.0;      // Reset the integral term
  }

  if (output > 255.00) {
    output = 255.00;
  } else if (output < -255.00) {
    output = -255.00;
  }
  int motorSpeed = abs(output);  // Map the absolute output to the motor speed range
  if (output < 0.00) {
    motorDirection = -1;
  } else if(output==0){
    motorDirection = 0;
  }
  else{
    motorDirection = 1;
  }
  /*

  if (motorDirection == 1) {
    analogWrite(motor1Pin1, 60);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, 60);
    analogWrite(motor2Pin2, 0);
  } else if (motorDirection == -1) {
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, 60);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, 60);
  }
  else{
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, 0);
  }
  */
  if (Serial.available()) {
    char command = Serial.read();
    Serial.println(command);
    /*
    stand_state = Serial.read();
    gripper_state = Serial.read();
    pan_state = Serial.read();
    tilt_state = Serial.read();
    */
    if (command=='1')
    {
      Serial.print("motor_state:");
      Serial.println('w');
      analogWrite(D0, 50);
      digitalWrite(D1, LOW);
      analogWrite(D2, 50);
      digitalWrite(D3, LOW);   
    }
    if (command=='2')
    {
      Serial.print("motor_state:");
      Serial.println('s');
      digitalWrite(D0, LOW);
      analogWrite(D1,50);
      digitalWrite(D2, LOW);
      analogWrite(D3,50); 
    }
    if (command=='3')
    {
      Serial.print("motor_state:");
      Serial.println('a');
      analogWrite(D0,50);
      digitalWrite(D1,LOW);
      analogWrite(D2,0);
      digitalWrite(D3,LOW);
    }
    if (command=='4')
    {
      Serial.print("motor_state:");
      Serial.println('d');
      analogWrite(D0,0);
      digitalWrite(D1,LOW);
      analogWrite(D2,50);
      digitalWrite(D3,LOW);
    }
    if (command=='a')
    {
      Serial.print("stand_state:");
      Serial.println("on");
      gripper.write(90);
    }
    if (command=='b')
    {
      analogWrite(D0,0);
      digitalWrite(D1,LOW);
      analogWrite(D2,0);
      digitalWrite(D3,LOW);

    }
    if (command=='6')
    {
      Serial.print("gripper_state:");
      Serial.println("cw");    
      gripper.write(45);
    }
    else if(command=='7')
    {
      Serial.print("gripper_state:");
      Serial.println("ccw");
      gripper.write(135);
    }
    if (command=='8'){
      pan_pos=pan_pos+1;
      if(pan_pos>180){
        pan_pos=180;
      }
      Serial.print("pan_pos:");
      Serial.println(pan_pos);
      pan.write(pan_pos);
    }
    else if (command=='9')
    {
      pan_pos=pan_pos-1;
      if(pan_pos<0)
      {
        pan_pos=0;
      }
      Serial.print("pan_pos:");
      Serial.println(pan_pos);
      pan.write(pan_pos);
    }
    if (command=='0')
    {
      tilt_pos = tilt_pos+1;
      if(pan_pos>180){
        tilt_pos=180;
      }
      Serial.print("tilt_pos:");
      Serial.println(tilt_pos);
      tilt.write(tilt_pos);
    }
    if (command=='5')
    {
      tilt_pos = tilt_pos-1;
      if(tilt_pos<0)
      {
        tilt_pos=0;
      }
      Serial.print("tilt_pos:");
      Serial.println(tilt_pos);
      tilt.write(tilt_pos);
    }
    tilt.write(tilt_pos);
    if (command=='c')
    {
      self_count+=1; 
    }
    if (command=='d')
    {
      self_count=0;
    }

    if(self_count>0)
    {
      //Serial.println("Self balancing enabled");
      if (motorDirection == 1) {
        analogWrite(D0, 60);
        analogWrite(D1, 0);
        analogWrite(D2, 60);
        analogWrite(D3, 0);
      } else if (motorDirection == -1) {
        analogWrite(D0, 0);
        analogWrite(D1, 60);
        analogWrite(D2, 0);
        analogWrite(D3, 60);
      }
      else{
        analogWrite(D0, 0);
        analogWrite(D1, 0);
        analogWrite(D2, 0);
        analogWrite(D3, 0);
        }
    }
    if(self_count==0){
      //Serial.println("Self balancing disabled");
      analogWrite(D0, 0);
      analogWrite(D1, 0);
      analogWrite(D2, 0);
      analogWrite(D3, 0);
    }
  }
}
