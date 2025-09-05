
#include <SoftwareSerial.h>
SoftwareSerial hc05(1,2);
void setup() {
  // put your setup code here, to run once:
  
  hc05.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   //Send message to smartphone
  int pot = analogRead(A0);
  
  hc05.print(pot);
  delay(500);

}

// Basic demo for accelerometer readings from Adafruit MPU6050

// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>

// #include <SoftwareSerial.h>
// SoftwareSerial hc05(1,0);
// Adafruit_MPU6050 mpu;

// void setup(void) {
//   // Serial.begin(115200);
//   hc05.begin(9600);
//   while (!hc05) {
//     delay(10); // will pause Zero, Leonardo, etc until serial console opens
//   }

//   // Try to initialize!
//   if (!mpu.begin()) {
//     hc05.println("Failed to find MPU6050 chip");
//     while (1) {
//       delay(10);
//     }
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
//   mpu.setGyroRange(MPU6050_RANGE_250_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
//   // Serial.println("");
//   delay(100);
// }

// void loop() {

//   /* Get new sensor events with the readings */
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   /* Print out the values */

//   // Serial.print("GyroX:");
//   // Serial.print(g.gyro.x);
//   // Serial.print(",");
//   // Serial.print("GyroY:");
//   // Serial.print(g.gyro.y);
//   // Serial.print(",");
//   // Serial.print("GyroZ:");
//   // Serial.print(g.gyro.z);
//   // Serial.println("");

//   hc05.print("GyroX:");
//   hc05.print(g.gyro.x);
//   hc05.print(",");
//   hc05.print("GyroY:");
//   hc05.print(g.gyro.y);
//   hc05.print(",");
//   hc05.print("GyroZ:");
//   hc05.print(g.gyro.z);
//   hc05.println("");

//   delay(500);
// }
