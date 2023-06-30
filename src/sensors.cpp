#include "sensors.hpp"

Sensors::Sensors(){}

void Sensors::setUpSensors(){
    if (!bme.begin(0X76)) {
        check_values_byte_0 |= (0x01); //raise bit 0 of byte 0
    }

    int16_t ret;
    uint8_t auto_clean_days = 4;
    sensirion_i2c_init();

    while (sps30_probe() != 0) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
        Serial.print(sps30_probe());
    }

    ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
    if (ret) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
    }

    ret = sps30_start_measurement();
    if (ret < 0) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
    }
    
    if (!scd30.begin()) {
        check_values_byte_0 |= (0x01 << 4); //raise bit 4 of byte 0
    }

    Wire.begin();
    if (!INA.begin() )
    {
        check_values_byte_1 |= (0x01 << 4); //raise bit 4 of byte 1
    }


}

void Sensors::doMeasurements() {
    INA.setMaxCurrentShunt(1, 0.002);
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
    getValues();
    checkValues();
    sendValues();
    if(check_values_byte_0 != 0 || check_values_byte_1 != 0){
		sendErrorByte();
	}
}

// (T) temp = range: 0-100 - 1 byte (real temp = value / 2) (resolution = 0.5°C)
// (H) humidity = range: 0-100 - 1 byte (real humidity = value) (resolution = 3%)
// (C) co2 = range: 0-200 - 1 byte (real co2 = value * 10) (resolution = 30ppm + 3% measured value)
// (P) pressure = range: 0-250 - 1 byte (real pressure = value + 800) (resolution = 0.25%)
// (V) voltage = range: 0-25V - 1 byte (real voltage = value / 10) (resolution = 0.1v)
// (PM) particulate matter = range: 0-250 - 1 byte (times 3) (real PM = value)

// sent message would be:
// THCP (T H C P V PM1 PM2.5 PM10)

void Sensors::getValues(){
    temperature = temp_filter.getValue() * 2;
    humidity = hum_filter.getValue();
    co2 = co2_filter.getValue() / 10;
    pressure = pres_filter.getValue() - 800;
    voltage = volt_filter.getValue() * 10;
    pm1 = pm1_filter.getValue();
    pm25 = pm25_filter.getValue();
    pm10 = pm10_filter.getValue();
}

void Sensors::sendValues(){
    Serial.print(1);
    Serial.print(",");
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

void Sensors::sendErrorByte(){
    Serial.print(3);
    Serial.print(",");
    Serial.print(check_values_byte_0);
    Serial.print(",");
    Serial.println(check_values_byte_1);
    check_values_byte_0 = 0x00;
    check_values_byte_1 = 0x00;
}

void Sensors::checkValues(){
    if(15 * 2 > temperature || temperature > 25 * 2){
        check_values_byte_0 |= (0x01 << 1); //raise bit 1 of byte 0
    }

    if(30 > humidity || humidity > 70){
        check_values_byte_0 |= (0x01 << 2); //raise bit 2 of byte 0
    }

    if(960 - 800 > pressure || pressure > 1050 - 800){
        check_values_byte_0 |= (0x01 << 3); //raise bit 3 of byte 0
    }

    if(200 / 10 > co2 || co2 > 1000 / 10){
        check_values_byte_0 |= (0x01 << 5); //raise bit 5 of byte 0

    }

    if(pm1 > 5){
        check_values_byte_0 |= (0x01 << 7); //raise bit 7 of byte 0
    }

    if(pm25 > 12){
        check_values_byte_1 |= (0x01); //raise bit 0 of byte 1
    }

    if(pm10 > 45){
        check_values_byte_1 |= (0x01 << 1); //raise bit 1 of byte 1
    }

    if(voltage > 14 * 10){
        check_values_byte_1 |= (0x01 << 2); //raise bit 2 of byte 1
    }

    if(voltage < 11.6 * 10){
        check_values_byte_1 |= (0x01 << 3); //raise bit 3 of byte 1
    }
    // if(){
    //  check_values_byte_1 |= (0x01 << 5); //raise bit 5 of byte 1
    // }

    // if(){
    //  check_values_byte_1 |= (0x01 << 6); //raise bit 6 of byte 1
    // }

    // if(){
    //  check_values_byte_1 |= (0x01 << 7); //raise bit 7 of byte 1
    // }
}