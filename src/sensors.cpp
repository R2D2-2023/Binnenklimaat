#include "sensors.hpp"

Sensors::Sensors(Adafruit_SCD30  scd30,Adafruit_BME280 bme): scd30(scd30), bme(bme){}

void Sensors::setUpSensors(){
  unsigned status;

  // default settings
  status = bme.begin(0x76);  
  // You can also pass in a Wire library object like &Wire2
  // status = bme.begin(0x76, &Wire2)
  if (!status) {
      Serial.println("BME280 could not be initialized!");
      while (1) delay(10);
  }

  // Try to initialize!
    if (!scd30.begin()) {
        Serial.println("Failed to find SCD30 chip");
        while (1) { delay(10); 
        }
    }
}

// (T) temp = range: 0-100 - 1 byte (real temp = value / 2) (resolution = 0.5°C)
// (H) humidity = range: 0-100 - 1 byte (real humidity = value) (resolution = 3%)
// (C) co2 = range: 0-200 - 1 byte (real co2 = value * 10) (resolution = 30ppm + 3% measured value)
// (P) pressure = range: 0-250 - 1 byte (real pressure = value + 800) (resolution = 0.25%)
// (PM) particulate matter = range: 0-300 - 2 bytes (real PM = value)

// sent message would be:
// THCP (T H C P PM)
// i.e.
// 411506015030 (41 150 60 150 30) 

void Sensors::sendValues(){
	temperature = scd30.temperature*2;
    if (temperature < 0) {
      temperature = 0;
    } 
	  else if (temperature >65){
      temperature = 50 * 2;
    }

    unsigned int humidity = bme.readHumidity(); // done
    unsigned int co2 = scd30.CO2 / 10; // done
    unsigned int pressure = (bme.readPressure() / 100.0F) - 800;
  
    Serial.print(temperature);
    Serial.print(humidity);
    Serial.println(co2);
    Serial.print(pressure);
    
}
