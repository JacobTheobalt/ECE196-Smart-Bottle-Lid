void setup_IMU() {
  myIMU.begin();
}

void get_accelerations() {
  int i = 0;
  while (i < 100) {
    if (current_time - start_time > 100) {
      //read Accelerometer values
      az[i] = myIMU.readFloatAccelZ();
      ay[i] = myIMU.readFloatAccelY();
      ax[i] = myIMU.readFloatAccelX();
      start_time = millis();
//      Serial.print(az[i]);
//      Serial.print(",");
//      Serial.print(ay[i]);
//      Serial.print(",");
//      Serial.println(ax[i]);
      i += 1;
    }
    current_time = millis();
  }
}

void get_averages() {
  //for loop counter
  int j;
  //data variables
  float sum_z = 0;
  float sum_y = 0;
  float sum_x = 0;
  //build the sum for all 3 positional accelerations
  for (j = 0; j < 100; j++) {
    sum_z = sum_z + az[j];
    sum_y = sum_y + ay[j];
    sum_x = sum_x + ax[j];
  }
  //define the average acceleration in each direction
  avg_z = (sum_z / 100);
  avg_y = (sum_y / 100);
  avg_x = (sum_x / 100);
}

bool acceptable_position()
{
  //determine if the average values are within the acceptable range for the bottle position. These values came from experimentation
  if(avg_z <= -0.65 && avg_z >= -1.1 && avg_y >= -0.55 && avg_y <= 0.55 && avg_x >= -0.55 && avg_x <= 0.55)
  {
    return true;
  }
  return false;
}

bool check_stability()
{
  //compare the new average acceleration values with the initial accelerations values. Return true as long as they are almost equal
  if(avg_z <= acceleration.z + margin && avg_z >= acceleration.z - margin && avg_y <= acceleration.y + margin && avg_y >= acceleration.y - margin && avg_x <= acceleration.x + margin && avg_x >= acceleration.x - margin)
  {
    return true;
  }
  return false;
}
