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
	unsigned int voltage;
	unsigned int pm1;
	unsigned int pm25;
	unsigned int pm10;

    //Error bytes
    uint8_t check_values_byte_0;
    uint8_t check_values_byte_1;

	Filter temp_filter = Filter();
	Filter hum_filter = Filter();
	Filter co2_filter = Filter();
	Filter pres_filter = Filter();
	Filter volt_filter = Filter();
	Filter pm1_filter = Filter();
	Filter pm25_filter = Filter();
	Filter pm10_filter = Filter();

	void sendValues();
	void checkValues();
	void sendErrorByte();


public:
	//constructor
	Sensors();


	int setUpSensors();
	void doMeasurements();
};
#endif //SENSORS_HPP