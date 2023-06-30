# Binnenklimaat
## Introduction
Binnenklimaat is a repository for sensors and for sending data useing LoRa to the back-end. We use an arduino and a Pycom Lopy4 in our project. The arduino fetches the data from the chips and sends it through a serial bus to the Pycom Lopy4. The Pycom Lopy4 then sends that sensor data through a Long-Range ( Lo-Ra ) protocol to the TheThingsNetwork server from the back-end. The arduino code is written in C++ and the Pycom Lopy4 code is written in micro-Python.
What our chips measure:
```* Temperature
* Humidity
* CO2 level
* Airpressure
* particulate matter
* Voltsensor 
```

These measurements, with the location of team "positie bepalen" are send to the back-end server.

## Hardware
```The chips we use in our project are and what they are used for:
* BME280 ( Temperature, Humidity, Airpressure )
* SCD30 ( CO2 Level ) 
* SPS30 ( Particulate Matter )
* INA266 (Volt Sensor)
```

## Installation
Libraries we use for C++ code:
```* Wire
* SPI
* Adafruit_Sensor
* Adafruit_BME280
* Adafruit_SCD30
* INA226 robtillaart
* Arduino
```

Libraries we use for Python code:
```* socket
* binascii
* struct
* time
* config
* network ( LoRa )
```

## Doxyen
If you want to see all private variables/functions. You need to change settings in Doxywizard. 
* EXTRACT_PRIVATE = YES
* HIDE_UNDOC_MEMBERS = YES
* INTERAL_DOCS = YES 