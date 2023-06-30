#include <Arduino.h>
#include <sensors.hpp>

Sensors climate_sensors = Sensors();

void setup() {
  Serial.begin(115200);
}

void loop() {
  climate_sensors.setUpSensors();
  climate_sensors.doMeasurements();
  delay(3000);
}