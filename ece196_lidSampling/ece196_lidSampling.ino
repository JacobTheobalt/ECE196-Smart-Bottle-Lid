#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cd_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define DEV_I2C Wire
#define SerialPort Serial

// Components.
VL53L4CD sensor_vl53l4cd_sat(&DEV_I2C, A1);

unsigned long current_time = 0;
unsigned long start_time = millis();
int samplePin = D1;
int pdiodePin = A0;
/* Setup ---------------------------------------------------------------------*/

void setup()
{
  // Initialize serial for output.
  SerialPort.begin(115200);
  // Initialize I2C bus.
  DEV_I2C.begin();
  // Configure VL53L4CD satellite component.
  sensor_vl53l4cd_sat.begin();
  // Switch off VL53L4CD satellite component.
  sensor_vl53l4cd_sat.VL53L4CD_Off();
  //Initialize VL53L4CD satellite component.
  sensor_vl53l4cd_sat.InitSensor();
  // Program the highest possible TimingBudget, without enabling the
  // low power mode. This should give the best accuracy
  sensor_vl53l4cd_sat.VL53L4CD_SetRangeTiming(200, 0);
  // Start Measurements
  sensor_vl53l4cd_sat.VL53L4CD_StartRanging();
  pinMode(samplePin, OUTPUT);
  digitalWrite(samplePin, HIGH);
  pinMode(pdiodePin, INPUT);
}

void loop()
{
  if(current_time - start_time > 2000)
  {
    Serial.println(getWaterLevel());
    start_time = millis();
  }
  current_time = millis();
}