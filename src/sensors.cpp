#include "sensors.hpp"

Sensors::Sensors()
{}

int Sensors::setUpSensors(){
    if (!bme.begin(0X76)) {
        return 1;
    }

    int16_t ret;
    uint8_t auto_clean_days = 4;
    sensirion_i2c_init();

    while (sps30_probe() != 0) {
        return 2;
    }

    ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
    if (ret) {
        return 3;
    }

    ret = sps30_start_measurement();
    if (ret < 0) {
        return 3;
    }
    
    // Try to initialize!
    if (!scd30.begin()) {
        return 4;
    }

    // Wire.begin();
    // if (!INA.begin() )
    // {
    //     return 5;
    // }

    // INA.setMaxCurrentShunt(1, 0.002);

    return 0;
}

void Sensors::doMeasurements() {
    for (int i = 0; i < 5; i++) {
        while(!scd30.dataReady()) {
            delay(10);
        }
        scd30.read();
        struct sps30_measurement pm;
        float temperature = bme.readTemperature();
        float humidity = bme.readHumidity();
        float co2 = scd30.CO2;
        float pressure = bme.readPressure() / 100;
        float voltage = INA.getBusVoltage();
        while (sps30_read_measurement(&pm) < 0)
        {}

        temp_filter.addDatapoint(temperature);
        hum_filter.addDatapoint(humidity);
        co2_filter.addDatapoint(co2);
        pres_filter.addDatapoint(pressure);
        volt_filter.addDatapoint(voltage);
        pm1_filter.addDatapoint(pm.mc_1p0);
        pm25_filter.addDatapoint(pm.mc_2p5);
        pm10_filter.addDatapoint(pm.mc_10p0);
        delay(3000);
    }

    sendValues();
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
    unsigned int temperature = temp_filter.getValue();
    unsigned int humidity = hum_filter.getValue();
    unsigned int co2 = co2_filter.getValue();
    unsigned int pressure = pres_filter.getValue();
    unsigned int voltage = volt_filter.getValue();
    unsigned int pm1 = pm1_filter.getValue();
    unsigned int pm25 = pm25_filter.getValue();
    unsigned int pm10 = pm10_filter.getValue();
    
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.print(co2);
    Serial.print(",");
    Serial.print(pressure);
    Serial.print(",");
    Serial.print(voltage);
    Serial.print(",");
    Serial.print(pm1);
    Serial.print(",");
    Serial.print(pm25);
    Serial.print(",");
    Serial.println(pm10);
}