#include <Arduino.h>
#include <sensors.hpp>

Sensors climate_sensors = Sensors();

void setup() {
  Serial.begin(115200);
  // int result = climate_sensors.setUpSensors();
  // if (result)
  // {
  //   Serial.print(result);
  //   Serial.print(" ");
  //   Serial.println("unlucky bro");
  //   while (1){};
  // }
}

void loop() {
  int result = climate_sensors.setUpSensors();
  climate_sensors.doMeasurements();
  delay(3000);
}