#include <Arduino.h>
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "sensors.hpp"

Adafruit_SCD30  scd30;
Adafruit_BME280 bme; // I2C
Sensors climate_sensors(scd30,bme);
#define SEALEVELPRESSURE_HPA (1013.5f)

void setup(void) {
  Serial.begin(115200);
  while (!Serial){}      // will pause Zero, Leonardo, etc until serial console opens
  climate_sensors.setUpSensors();
}

void loop() {
  if (climate_sensors.scd30.dataReady()){
        climate_sensors.scd30.read();
        climate_sensors.sendValues(); 
        delay(30000);
    } 
    else {
        delay(1000);
    }
}
