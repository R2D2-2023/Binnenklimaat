#include "sensors.hpp"

Sensors::Sensors(
    unsigned int temp,
    unsigned int hum,
    unsigned int press,
    unsigned int kool,
    unsigned int volt,
    unsigned int partm1,
    unsigned int partm25,
    unsigned int partm10
    )
    { 
    temperature = temp * 2, humidity = hum,
    pressure = press - 800, co2 = kool / 10, 
    voltage  = volt * 10, pm1 = partm1, 
    pm25 = partm25, pm10 = partm10;
}

void Sensors::setUpSensors(){
    if (!bme.begin(0X76)) {
        check_values_byte_0 |= (0x01); //raise bit 0 of byte 0
        // return 1;
    }

    int16_t ret;
    uint8_t auto_clean_days = 4;
    sensirion_i2c_init();

    while (sps30_probe() != 0) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
        // return 2;
    }

    ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
    if (ret) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
        // return 3;
    }

    ret = sps30_start_measurement();
    if (ret < 0) {
        check_values_byte_0 |= (0x01 << 6); //raise bit 6 of byte 0
        // return 3;
    }
    
    // Try to initialize!
    if (!scd30.begin()) {
        check_values_byte_0 |= (0x01 << 4); //raise bit 4 of byte 0
        // return 4;
    }

    Wire.begin();
    if (!INA.begin() )
    {
        check_values_byte_1 |= (0x01 << 4); //raise bit 4 of byte 1
        // return 5;
    }


    // return 0; //returns 
}

void Sensors::doMeasurements() {
    // INA.setMaxCurrentShunt(1, 0.002);
    // for (int i = 0; i < 5; i++) {
    //     while(!scd30.dataReady()) {
    //         delay(10);
    //     }
    //     scd30.read();
    //     struct sps30_measurement pm;
    //     float temperature = bme.readTemperature();
    //     float humidity = bme.readHumidity();
    //     float co2 = scd30.CO2;
    //     float pressure = bme.readPressure() / 100;
    //     float voltage = INA.getBusVoltage();
    //     while (sps30_read_measurement(&pm) < 0)
    //     {}

    //     temp_filter.addDatapoint(temperature);
    //     hum_filter.addDatapoint(humidity);
    //     co2_filter.addDatapoint(co2);
    //     pres_filter.addDatapoint(pressure);
    //     volt_filter.addDatapoint(voltage);
    //     pm1_filter.addDatapoint(pm.mc_1p0);
    //     pm25_filter.addDatapoint(pm.mc_2p5);
    //     pm10_filter.addDatapoint(pm.mc_10p0);
    //     delay(3000);
    // }
    // getValues();
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
    unsigned int temperature = temp_filter.getValue() * 2;
    unsigned int humidity = hum_filter.getValue();
    unsigned int co2 = co2_filter.getValue() / 10;
    unsigned int pressure = pres_filter.getValue() - 800;
    unsigned int voltage = volt_filter.getValue() * 10;
    unsigned int pm1 = pm1_filter.getValue();
    unsigned int pm25 = pm25_filter.getValue();
    unsigned int pm10 = pm10_filter.getValue();
}

void Sensors::sendValues(){
    Serial.print(1);
    Serial.print(",");
    // Serial.print("temp:");
    Serial.print(temperature);
    Serial.print(",");
    // Serial.print("hum:");
    Serial.print(humidity);
    Serial.print(",");
    // Serial.print("co2:");
    Serial.print(co2);
    Serial.print(",");
    // Serial.print("press:");
    Serial.print(pressure);
    Serial.print(",");
    // Serial.print("volt:");
    Serial.print(voltage);
    Serial.print(",");
    // Serial.print("pm1:");
    Serial.print(pm1);
    Serial.print(",");
    // Serial.print("pm2.5:");
    Serial.print(pm25);
    Serial.print(",");
    // Serial.print("pm10:");
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
        Serial.println("1");
    }

    if(30 > humidity || humidity > 70){
        check_values_byte_0 |= (0x01 << 2); //raise bit 2 of byte 0
        Serial.println("2");
    }

    if(960 - 800 > pressure || pressure > 1050 - 800){
        check_values_byte_0 |= (0x01 << 3); //raise bit 3 of byte 0
        Serial.println("3");
    }

    if(200 / 10 > co2 || co2 > 800 / 10){
        check_values_byte_0 |= (0x01 << 5); //raise bit 5 of byte 0
        Serial.println("4");

    }

    if(pm1 > 5){
        check_values_byte_0 |= (0x01 << 7); //raise bit 7 of byte 0
        Serial.println("5");

    }

    if(pm25 > 12){
        check_values_byte_1 |= (0x01); //raise bit 0 of byte 1
        Serial.println("6");

    }

    if(pm10 > 45){
        check_values_byte_1 |= (0x01 << 1); //raise bit 1 of byte 1
        Serial.println("7");

    }

    if(voltage > 14 * 10){
        check_values_byte_1 |= (0x01 << 2); //raise bit 2 of byte 1
        Serial.println("8");

    }

    if(voltage < 11.6 * 10){
        check_values_byte_1 |= (0x01 << 3); //raise bit 3 of byte 1
        Serial.println("9");

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