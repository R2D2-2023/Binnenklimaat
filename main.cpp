#include <Arduino.h>
// Basic demo for readings from Adafruit SCD30
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_SCD30  scd30;
Adafruit_BME280 bme; // I2C

#define SEALEVELPRESSURE_HPA (1013.5f)

void setup(void) {
  Serial.begin(9600);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
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


  // if (!scd30.setMeasurementInterval(10)){
  //   Serial.println("Failed to set measurement interval");
  //   while(1){ delay(10);}
  // }
  Serial.print("Measurement Interval: "); 
  Serial.print(scd30.getMeasurementInterval()); 
  Serial.println(" seconds");
}

void sendValues() {
    int temp = scd30.temperature;
    unsigned int temperature;
    if (scd30.temperature > 0 && scd30.temperature < 65) {
        temperature = scd30.temperature;
    } else if (temp < 0) {
        temperature = 0;
    } else {
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

void loop() {
  if (scd30.dataReady()){
    scd30.read();
    sendValues();
  } else {
    //Serial.println("No data");
  }

  delay(30);
}