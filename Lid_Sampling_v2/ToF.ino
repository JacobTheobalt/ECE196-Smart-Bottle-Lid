//another Non-blocking timing variable
unsigned long run_time = 0;
//ToF variables, setup purposes
uint8_t NewDataReady = 0;
VL53L4CD_Result_t results;
uint8_t status;

void setup_ToF()
{
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
}

//get the water level of the bottle by sampling over 10 seconds and taking the average
float get_water_level()
{
  //variable for keeping track of iterations
  int measurement = 0;
  //keep track of the water level sum
  float water_level_sum = 0;
  //take 100 samples of the water level
  while(measurement < 100)
  {
    do {
      status = sensor_vl53l4cd_sat.VL53L4CD_CheckForDataReady(&NewDataReady);
    } while (!NewDataReady);

    if ((!status) && (NewDataReady != 0)) {
      // (Mandatory) Clear HW interrupt to restart measurements
      sensor_vl53l4cd_sat.VL53L4CD_ClearInterrupt();

      // Read measured distance. RangeStatus = 0 means valid data
      sensor_vl53l4cd_sat.VL53L4CD_GetResult(&results);
      //store the measurement at index 'measurement' in water_levels array
      water_levels[measurement] = results.distance_mm;
      measurement += 1;
      water_level_sum = results.distance_mm + water_level_sum;
    }
  }
  return water_level_sum / 100; //return the average water level
}

void calculate_water()
{
  if(new_water_level <= water_level)
  {
    //if the water level increased, change the water level variable to the new measured level
    //we say the water level "increased" if the measured water level is closer to the lid, which would result in a smaller measurement
    water_level = new_water_level;
  }
  else if(new_water_level > water_level)
  {
    water_level = new_water_level;
    //add the amount of water consumed to the total amount. Formula is just the volume of a cylinder
    water_consumed = water_consumed + (new_water_level - water_level) * (3.141592) * (0.5*width*width);
  }
}

