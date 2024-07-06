#include <Arduino.h>

#include "M5StickCPlus2.h"

// Variables for storing sensor readings
float ax, ay, az;
float gx, gy, gz;

// Variables for orientation
float roll, pitch, yaw;
float rollGyro, pitchGyro, yawGyro;
float rollAccel, pitchAccel;

// Time tracking
unsigned long lastTime;
float dt;

void setup()
{
  M5.begin();
  M5.Imu.init();

  lastTime = millis();

  // Set up the display
  M5.Lcd.setRotation(3); // Set the display rotation
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
}

void loop()
{
  // Read accelerometer and gyroscope data
  M5.Imu.getAccelData(&ax, &ay, &az);
  M5.Imu.getGyroData(&gx, &gy, &gz);

  // Calculate elapsed time
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // Convert gyroscope data from degrees per second to radians per second
  gx = radians(gx);
  gy = radians(gy);
  gz = radians(gz);

  // Integrate gyroscope data to get angles
  rollGyro += gx * dt;
  pitchGyro += gy * dt;
  yawGyro += gz * dt;

  // Calculate accelerometer angles
  rollAccel = atan2(ay, az);
  pitchAccel = atan2(-ax, sqrt(ay * ay + az * az));

  // Complementary filter to combine accelerometer and gyroscope data
  float alpha = 0.98; // filter coefficient
  roll = alpha * rollGyro + (1.0 - alpha) * rollAccel;
  pitch = alpha * pitchGyro + (1.0 - alpha) * pitchAccel;
  yaw = yawGyro; // Gyroscope is generally used for yaw

  // Convert to degrees for better readability
  roll = degrees(roll);
  pitch = degrees(pitch);
  yaw = degrees(yaw);

  // Clear the display
  M5.Lcd.fillScreen(BLACK);

  // Print the orientation
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("Roll: %.2f", roll);
  M5.Lcd.setCursor(10, 40);
  M5.Lcd.printf("Pitch: %.2f", pitch);
  M5.Lcd.setCursor(10, 70);
  M5.Lcd.printf("Yaw: %.2f", yaw);

  delay(10); // Adjust the delay as needed
}
