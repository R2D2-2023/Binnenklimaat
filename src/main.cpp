#include <Arduino.h>
#include <sensors.hpp>

Sensors climate_sensors = Sensors();

void setup() {
  Serial.begin(115200);
  Serial2.begin(57600);
  Serial3.begin(9600);
}

void loop() {
  climate_sensors.sendLiveLocationValues();
  climate_sensors.setUpSensors();
  climate_sensors.doMeasurements();
  delay(3000);
}
