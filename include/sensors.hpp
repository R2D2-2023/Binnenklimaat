#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <Arduino.h>
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

class Sensors {
private:
	//Sensors
	
	Adafruit_BME280 bme; 
	//Values
	unsigned int temperature;
	unsigned int humidity;
	unsigned int pressure;
	unsigned int co2;

public:
	Adafruit_SCD30  scd30;
	//constructor
	Sensors(Adafruit_SCD30  scd30,Adafruit_BME280 bme);


	void setUpSensors();
	void sendValues();
};
#endif //SENSORS_HPP