#include <Arduino.h>
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "crc8.hpp"
#include "sensors.hpp"

Adafruit_SCD30  scd30;
Adafruit_BME280 bme; // I2C
Sensors climate_sensors(scd30,bme);
#define SEALEVELPRESSURE_HPA (1013.5f)

void setup(void) {
  Serial.begin(9600);
  while (!Serial){}      // will pause Zero, Leonardo, etc until serial console opens
  delay(500);
  climate_sensors.setUpSensors();
  // Serial.println("test!");
}

void loop() {
  // Serial.println("data is not ready");
  if (climate_sensors.scd30.dataReady()){
    Serial.println("data ready");
    scd30.read();
    climate_sensors.sendValues();
  } else {
    //Serial.println("No data");
  }

  delay(30);
}