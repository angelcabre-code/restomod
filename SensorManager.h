#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <MPU9250.h> // MPU9250 by hideakitai

class SensorManager {
public:
    void begin();
    void update();
    
    // RTC
    DateTime getTime(); // Retorna hora UTC (RTC)
    DateTime getLocalTime(); // Retorna hora ajustada (CET/CEST)
    void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
    void setTime(uint32_t timestamp);

    // MPU9250
    float getGForceX();
    float getGForceY();

    // BMP280
    float getAltitude();
    float getTemperature();

private:
    RTC_DS3231 rtc;
    Adafruit_BMP280 bmp;
    MPU9250 mpu;
    float baselinePressure = 1013.25; 
    bool bmpActive = false;
    bool mpuActive = false;
    uint8_t rawMpuAddress = 0;
};

extern SensorManager sensors;

#endif
