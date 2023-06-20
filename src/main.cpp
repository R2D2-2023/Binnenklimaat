/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor
  This example shows how to take Sensor Events instead of direct readings
  
  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2652

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BME280.h>

// Unified sensor demo for readings from Adafruit SCD30
#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>

#include <sps30.h>

Adafruit_SCD30  scd30;

Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup() {
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  Serial.begin(9600);
  Serial.println(F("BME280 Sensor event test"));

  if (!bme.begin(0X76)) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1) delay(10);
  }

    int16_t ret;
    uint8_t auto_clean_days = 4;
    uint32_t auto_clean;
    sensirion_i2c_init();

    while (sps30_probe() != 0) {
      Serial.print("SPS sensor probing failed\n");
      delay(500);
    }

  #ifndef PLOTTER_FORMAT
    Serial.print("SPS sensor probing successful\n");
  #endif /* PLOTTER_FORMAT */

    ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
    if (ret) {
      Serial.print("error setting the auto-clean interval: ");
      Serial.println(ret);
    }

    ret = sps30_start_measurement();
    if (ret < 0) {
      Serial.print("error starting measurement\n");
    }

  #ifndef PLOTTER_FORMAT
    Serial.print("measurements started\n");
  #endif /* PLOTTER_FORMAT */

  #ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
    Serial.print("Your Arduino hardware has a limitation that only\n");
    Serial.print("  allows reading the mass concentrations. For more\n");
    Serial.print("  information, please check\n");
    Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
    Serial.print("\n");
    delay(2000);
  #endif

  
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();


  Serial.println("Adafruit SCD30 Temperature and humidity unified sensor test!");


  // Try to initialize!
  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }

  Serial.println("SCD30 Found!");
}

void loop() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  Serial.println("BME280");
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(1000);
  Serial.println("SCD30");

  if(scd30.dataReady()){
    Serial.println("Data available!");
    scd30.read();

    // if (!scd30.getEvent(&humidity, &temp, &co2)){
    //   Serial.println("Error reading sensor data");
    //   return;
    // }

    Serial.print("Temperature: ");
    Serial.print(scd30.temperature);
    Serial.println(" degrees C");
    
    Serial.print("Relative Humidity: ");
    Serial.print(scd30.relative_humidity);
    Serial.println(" %");
    
    Serial.print("CO2: ");
    Serial.print(scd30.CO2, 3);
    Serial.println(" ppm");

    Serial.println("");
  }
  else{
    Serial.println("Data not ready");
  }

  struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("error reading data-ready flag: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("data not ready, no new measurement available\n");
    else
      break;
    delay(100); /* retry in 100ms */
  } while (1);

  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("error reading measurement\n");
  } else {

#ifndef PLOTTER_FORMAT
    Serial.print("PM  1.0: ");
    Serial.println(m.mc_1p0);
    Serial.print("PM  2.5: ");
    Serial.println(m.mc_2p5);
    Serial.print("PM  4.0: ");
    Serial.println(m.mc_4p0);
    Serial.print("PM 10.0: ");
    Serial.println(m.mc_10p0);

#ifndef SPS30_LIMITED_I2C_BUFFER_SIZE
    Serial.print("NC  0.5: ");
    Serial.println(m.nc_0p5);
    Serial.print("NC  1.0: ");
    Serial.println(m.nc_1p0);
    Serial.print("NC  2.5: ");
    Serial.println(m.nc_2p5);
    Serial.print("NC  4.0: ");
    Serial.println(m.nc_4p0);
    Serial.print("NC 10.0: ");
    Serial.println(m.nc_10p0);

    Serial.print("Typical partical size: ");
    Serial.println(m.typical_particle_size);
#endif

    Serial.println();

#else
    // since all values include particles smaller than X, if we want to create buckets we 
    // need to subtract the smaller particle count. 
    // This will create buckets (all values in micro meters):
    // - particles        <= 0,5
    // - particles > 0.5, <= 1
    // - particles > 1,   <= 2.5
    // - particles > 2.5, <= 4
    // - particles > 4,   <= 10

    Serial.print(m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_1p0  - m.nc_0p5);
    Serial.print(" ");
    Serial.print(m.nc_2p5  - m.nc_1p0);
    Serial.print(" ");
    Serial.print(m.nc_4p0  - m.nc_2p5);
    Serial.print(" ");
    Serial.print(m.nc_10p0 - m.nc_4p0);
    Serial.println();


#endif /* PLOTTER_FORMAT */

  }

  delay(1000);
}
