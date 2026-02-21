#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
float q0 = 1.0, q1 = 0.0, q2 = 0.0, q3 = 0.0;

float twoKp = 2.0 * 0.5;   
float twoKi = 2.0 * 0.0;   
float integralFBx = 0, integralFBy = 0, integralFBz = 0;      

unsigned long lastTime;
float dt;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
  lastTime = millis();
}

void loop() {

  unsigned long now = millis();
  dt = (now - lastTime) / 1000.0;
  lastTime = now;

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accX = ax / 16384.0;
  float accY = ay / 16384.0;
  float accZ = az / 16384.0;

  float gyroX = gx * PI / (180.0 * 131.0);
  float gyroY = gy * PI / (180.0 * 131.0);
  float gyroZ = gz * PI / (180.0 * 131.0);

  float norm = sqrt(accX*accX + accY*accY + accZ*accZ);
  if(norm == 0) return;
  accX /= norm;
  accY /= norm;
  accZ /= norm;

  float vx = 2*(q1*q3 - q0*q2);
  float vy = 2*(q0*q1 + q2*q3);
  float vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

  float ex = (accY * vz - accZ * vy);
  float ey = (accZ * vx - accX * vz);
  float ez = (accX * vy - accY * vx);

  integralFBx += twoKi * ex * dt;
  integralFBy += twoKi * ey * dt;
  integralFBz += twoKi * ez * dt;

  gyroX += twoKp * ex + integralFBx;
  gyroY += twoKp * ey + integralFBy;
  gyroZ += twoKp * ez + integralFBz;

  float qDot0 = 0.5 * (-q1*gyroX - q2*gyroY - q3*gyroZ);
  float qDot1 = 0.5 * ( q0*gyroX + q2*gyroZ - q3*gyroY);
  float qDot2 = 0.5 * ( q0*gyroY - q1*gyroZ + q3*gyroX);
  float qDot3 = 0.5 * ( q0*gyroZ + q1*gyroY - q2*gyroX);

  q0 += qDot0 * dt;
  q1 += qDot1 * dt;
  q2 += qDot2 * dt;
  q3 += qDot3 * dt;

  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 /= norm;
  q1 /= norm;
  q2 /= norm;
  q3 /= norm;

  Serial.print(q0); Serial.print(",");
  Serial.print(q1); Serial.print(",");
  Serial.print(q2); Serial.print(",");
  Serial.println(q3);

  delay(5);
}


