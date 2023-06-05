//max value for the lid to be considered "on"
int light_threshold = 50;

//return true if the lid detects very little light
bool lid_on()
{
  light_level = analogRead(pdiode);
  if(light_level >= light_threshold)
  {
    return false;
  }
  return true;
}
