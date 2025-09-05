#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <PWMServo.h>

PWMServo myservo; 

//=====================================
// 1) HARDWARE PINS & CONSTANTS
//=====================================
const int encoderA1   = 2;    // Left wheel encoder A
const int encoderB1   = 3;    // Left wheel encoder B
const int encoderA2   = 4;    // Right wheel encoder A
const int encoderB2   = 5;    // Right wheel encoder B

const int motor1Dir   = 7;    // Left motor direction pin
const int motor1Pwm   = 6;    // Left motor PWM pin
const int motor2Dir   = 9;    // Right motor direction pin
const int motor2Pwm   = 8;    // Right motor PWM pin
const int servopin = 12;

volatile long count1 = 0, count2 = 0;      // encoder counts
const float PPR      = 1259.0;             // pulses per revolution

//=====================================
// 2) LQR GAINS (from MATLAB offline)
//    u = –[k1 k2 k3 k4] · [θ θ̇ φ φ̇]^T
//=====================================
const float k1 = 9844.0;   // gain on body angle θ
const float k2 =  363.6;   // gain on body rate θ̇
const float k3 =  1.0;   // gain on wheel angle φ
const float k4 =  44.0;   // gain on wheel rate φ̇

//=====================================
// 3) IMU SETUP
//=====================================
Adafruit_BNO055 bno = Adafruit_BNO055(55);

// For finite difference φ̇
float lastPhi1  = 0, lastPhi2  = 0;
float lastTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);

  myservo.attach(servopin);
  myservo.write(101);
  // myservo.detach();

  // Initialize BNO055
  if (!bno.begin()) {
    Serial.println("BNO055 not detected. Check wiring!");
    while (1) delay(10);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  // Encoder pins & attach ISRs
  pinMode(encoderA1, INPUT_PULLUP);
  pinMode(encoderB1, INPUT_PULLUP);
  pinMode(encoderA2, INPUT_PULLUP);
  pinMode(encoderB2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderA1), isr1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderA2), isr2, CHANGE);

  // Motor pins
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor1Pwm, OUTPUT);
  pinMode(motor2Dir, OUTPUT);
  pinMode(motor2Pwm, OUTPUT);

  lastTime = millis() * 0.001;  // seconds
}

void loop() {
  //---- 1) Read body pitch θ and rate θ̇ from BNO055 ----
  sensors_event_t evt;
  bno.getEvent(&evt);
  float theta    = evt.orientation.y * DEG_TO_RAD;  // radians
  float thetaDot = evt.gyro.y;                      // rad/s

  //---- 2) Read wheel angles φ and rates φ̇ ----
  float now   = millis() * 0.001;
  float dt    = now - lastTime;
  if (dt <= 0) dt = 0.001;

  float phi1    = (count1 / PPR) * 2.0 * PI;       // left wheel [rad]
  float phi2    = (count2 / PPR) * 2.0 * PI;       // right wheel [rad]
  float phiDot1 = (phi1 - lastPhi1) / dt;          // left wheel [rad/s]
  float phiDot2 = (phi2 - lastPhi2) / dt;          // right wheel [rad/s]

  lastPhi1 = phi1;
  lastPhi2 = phi2;
  lastTime = now;

  // average the two wheels for φ and φ̇
  float phiAvg    = 0.5 * (phi1    + phi2);
  float phiDotAvg = 0.5 * (phiDot1 + phiDot2);

  //---- 3) Compute LQR control: u = –K·x ----
  float u = -(k1*theta + k2*thetaDot + k3*phiAvg + k4*phiDotAvg);
  u = constrain(u, -255, 255);  // saturate to PWM limits
  Serial.print("u:");
  Serial.print(u);
  Serial.print("\t");

  //---- 4) Drive both motors ----
  bool dir = (u >= 0);
  int pwm  = abs((int)u);

  // Left motor
  digitalWrite(motor1Dir, !dir);
  analogWrite(motor1Pwm, pwm);
  // Right motor
  digitalWrite(motor2Dir, dir);
  analogWrite(motor2Pwm, pwm);

  // Debug output
  Serial.print("θ=");     Serial.println(theta,   3);

  delay(5);  // ~200 Hz loop
}

//=====================================
// Encoder ISRs (quadrature count)
//=====================================
void isr1() {
  bool A = digitalRead(encoderA1);
  bool B = digitalRead(encoderB1);
  
      if (A == B) {
    count1++;
    } 
    else {
    count1--;
  }
}

void isr2() {
  bool A = digitalRead(encoderA2);
  bool B = digitalRead(encoderB2);

    if (A == B) {
    count2++;
    } 
    else {
    count2--;
  }
}
