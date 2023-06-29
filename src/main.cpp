#include <Arduino.h>
#include <sensors.hpp>

Sensors climate_sensors = Sensors(20, 50, 1000, 700, 12, 3, 10, 17);


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
  // climate_sensors.setUpSensors();
  climate_sensors.doMeasurements();
  delay(3000);
}