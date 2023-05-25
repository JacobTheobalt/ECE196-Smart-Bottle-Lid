//Libraries
#include "LSM6DS3.h"
#include "Wire.h"
#include <Arduino.h>
#include <vl53l4cd_class.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define DEV_I2C Wire

// Components.
VL53L4CD sensor_vl53l4cd_sat(&DEV_I2C, A1);
//Create a instance of class LSM6DS3
LSM6DS3 myIMU(I2C_MODE, 0x6A);    //I2C device address 0x6A

//Address the positional accelerations
struct Positions
{
  float x;
  float y;
  float z;
};
//initialize a Position variable called acceleration
Positions acceleration = {0, 0, 0};
//Non-blocking timing purposes
unsigned long current_time = 0;
unsigned long start_time = millis();
//create arrays to store data over 10 seconds
float az[100]; 
float ay[100];
float ax[100];
//average accelerations in each direction
float avg_z = 0;
float avg_y = 0;
float avg_x = 0;
//accounts for noise not caused by movement
float margin = 0.1;
//array for storing water levels then getting the average
float water_levels[100];
//variable for keeping track of water level
float water_level = 0;

void setup() {
    // put your setup code here, to run once:
  setup_IMU();
  setup_ToF();
}

void loop() 
{
  //fills the arrays with measurement data
  get_accelerations();
  get_averages();

  if(acceptable_position())
  {
    Serial.println("Upright position!");
    //define the initial positions only if the bottle is in an acceptable position at the time of measurement
    acceleration.z = avg_z;
    acceleration.y = avg_y;
    acceleration.x = avg_x;
    delay(100);
    //get the new average values
    get_accelerations();
    get_averages();
    if(check_stability())
    {
      Serial.println("Stable!");
      water_level = get_water_level();
      Serial.println(water_level);
    }
    else
    {
      Serial.println("Unstable!");
      //wait a minute and then try again.
      delay(1000);
    }
  }
  else
  {
    Serial.println("Not Upright!");
    acceleration.z = 0;
    acceleration.y = 0;
    acceleration.x = 0;
  }
}
