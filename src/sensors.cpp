#include "sensors.hpp"

Sensors::Sensors(Adafruit_SCD30  scd30,Adafruit_BME280 bme): scd30(scd30), bme(bme){}

void Sensors::setUpSensors(){
  unsigned status;
  
  // default settings
  status = bme.begin(0x76);  
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
      Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
      Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
      Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
      Serial.print("        ID of 0x60 represents a BME 280.\n");
      Serial.print("        ID of 0x61 represents a BME 680.\n");
      while (1) delay(10);
  }

    Serial.println("Adafruit SCD30 test!");

  // Try to initialize!
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");
  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");

  
}

void Sensors::sendValues(){
	temperature = scd30.temperature;
    if (temperature < 0) {
        temperature = 0;
    } 
	else if (temperature >65){
        temperature = 65;
    }

    unsigned int humidity = bme.readHumidity();
    unsigned int pressure = bme.readPressure() / 100.0F;
    unsigned int co2 = scd30.CO2;
    
    Serial.print("t=");
    Serial.print(temperature);
    Serial.print(",h=");
    Serial.print(humidity);
    Serial.print(",p=");
    Serial.print(pressure);
    Serial.print(",c=");
    Serial.println(co2);
}