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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define DEV_I2C Wire

//Components.
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
//create an enum for the Functional State Machine
enum Bottle
{
  WAIT,
  MEASURE,
  LOG_DATA,
};
//initialize the Bottle to be in the WAIT state
enum Bottle state = WAIT;
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
//variables for keeping track of water level
float water_level = 0;
float new_water_level = 0;
//variable for tracking water consumption
float water_consumed = 0;
//variable for tracking lid on/off
float light_level;
//define the width of the user's water bottle. Default to standard width of 91mm
unsigned int width = 91;
//define volume of the user's water bottle. Default to 40oz.
unsigned int volume = 40;
//photodiode pin
int pdiode = A0;

void setup() {
  //do all the setup work in other functions for cleaner code
  setup_IMU();
  setup_ToF();
  setup_LCD();
  pinMode(pdiode, INPUT);
}

void loop() 
{
  //###############################################################################################
  if(state == WAIT)
  {
    //check the state of the lid
    //lid_on() returns true if lid is on, false otherwise.
    bool lid_state = lid_on();
    //if the lid is off, wait until it is put back on
    if(lid_state == false)
    {
      //while the lid is detected as off, wait 10 seconds and check again
      while(!lid_on())
      {
        delay(10000);
      }
      //once the lid is placed back on, measure the water level
      state = MEASURE;
    }
    //if the lid is on, wait 3 seconds and check again 
    if(lid_state == true)
    {
      delay(3000);
    }
  }
  //###############################################################################################
  if(state == MEASURE)
  {
    showInDisplay("time to measure");
    //fills the arrays with measurement data
    get_accelerations();
    get_averages();

    if(acceptable_position())
    {
      showInDisplay("Upright position!");
      //define the initial positions only if the bottle is in an acceptable position at the time of measurement
      acceleration.z = avg_z;
      acceleration.y = avg_y;
      acceleration.x = avg_x;
      delay(100);
      //get the new average values
      get_accelerations();
      get_averages();
      //if the bottle position is similar to the position measured earlier,
      //then check the water level
      if(check_stability())
      {
        showInDisplay("Stable!");
        //if there is no initial water level yet, define the initial water level
        if(water_level == 0)
        {
          water_level = get_water_level();
          new_water_level = water_level;
        }
        else
        {
          new_water_level = get_water_level();
        }
        calculate_water();
        //go to wait state since we took our measurement already
        state = LOG_DATA;
      }
      else
      {
        showInDisplay("Unstable!");
        //wait 30 seconds and then try again.
        delay(30000);
      }
    }
    else
    {
      showInDisplay("Not Upright!");
      acceleration.z = 0;
      acceleration.y = 0;
      acceleration.x = 0;
    }
  }
  //###############################################################################################
  if(state == LOG_DATA)
  {
    //send the data to the LCD
    String hydration = String(water_consumed, 2);
    showInDisplay(hydration);
    state = WAIT;
  }
}
