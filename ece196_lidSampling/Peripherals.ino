  #define MIN_LIGHT 20
  int light_level = 0;
  
  float getWaterLevel()
  {
    uint8_t NewDataReady = 0;
    VL53L4CD_Result_t results;
    uint8_t status;
    char report[64];
    
    do {
      status = sensor_vl53l4cd_sat.VL53L4CD_CheckForDataReady(&NewDataReady);
    } while (!NewDataReady);

    if ((!status) && (NewDataReady != 0)) {
      // (Mandatory) Clear HW interrupt to restart measurements
      sensor_vl53l4cd_sat.VL53L4CD_ClearInterrupt();

      // Read measured distance. RangeStatus = 0 means valid data
      sensor_vl53l4cd_sat.VL53L4CD_GetResult(&results);
      snprintf(report, sizeof(report), "Status = %3u, Distance = %5u mm, Signal = %6u kcps/spad\r\n",
              results.range_status,
              results.distance_mm,
              results.signal_per_spad_kcps);
    }
      return results.distance_mm; //return the range read in mm
  }

  bool checkLid()
  {
    light_level = analogRead(pdiodePin)
    if(light_level > MIN_LIGHT)
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool checkStability()
  {
    //insert code here for stability checking using the IMU
  }