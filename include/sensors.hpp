#ifndef SENSORS_HPP
#define SENSORS_HPP

#include <Arduino.h>
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <sps30.h>
#include "INA226.h"
#include <Filter.hpp>

class Sensors {
private:
	//Sensors
	
	Adafruit_BME280 bme; 
	INA226 INA = INA226(0x40);

	Adafruit_SCD30 scd30;
	//Values
	unsigned int temperature;
	unsigned int humidity;
	unsigned int pressure;
	unsigned int co2;
	unsigned int location_x;
	unsigned int location_y;

	Filter temp_filter = Filter();
	Filter hum_filter = Filter();
	Filter co2_filter = Filter();
	Filter pres_filter = Filter();
	Filter volt_filter = Filter();
	Filter pm1_filter = Filter();
	Filter pm25_filter = Filter();
	Filter pm10_filter = Filter();

	void sendValues();

public:
	//constructor
	Sensors();

	int setUpSensors();
	void doMeasurements();
	void sendLiveLocationValues();
};
#endif //SENSORS_HPP
