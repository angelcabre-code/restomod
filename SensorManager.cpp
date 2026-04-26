#include "SensorManager.h"

SensorManager sensors;

void SensorManager::begin() {
    Wire.begin();
    Wire.setClock(100000);
    delay(250);

    if (!rtc.begin()) {
        Serial.println("Error: RTC DS3231 no encontrado");
    }

    if (bmp.begin(0x76) || bmp.begin(0x77)) { 
        bmpActive = true;
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                        Adafruit_BMP280::SAMPLING_X2,
                        Adafruit_BMP280::SAMPLING_X16,
                        Adafruit_BMP280::FILTER_X16,
                        Adafruit_BMP280::STANDBY_MS_500);
    } else {
        Serial.println("Error: BMP280 no encontrado");
    }

    if (mpu.setup(0x69) || mpu.setup(0x68)) {  
        mpuActive = true;
    } else {
        Serial.println("Librería MPU falló. Activando modo RAW I2C (Clon MPU6500 detectado)");
        Wire.beginTransmission(0x69);
        Wire.write(0x6B);
        Wire.write(0x00);
        if (Wire.endTransmission() == 0) {
            rawMpuAddress = 0x69;
        } else {
            Wire.beginTransmission(0x68);
            Wire.write(0x6B);
            Wire.write(0x00);
            if (Wire.endTransmission() == 0) {
                rawMpuAddress = 0x68;
            }
        }
    }
}

void SensorManager::update() {
    if (mpuActive) {
        mpu.update(); 
    }
}

DateTime SensorManager::getTime() {
    return rtc.now();
}

bool isDST_EU(DateTime now) {
    int year = now.year();
    int month = now.month();
    int day = now.day();
    int hour = now.hour();
    if (month < 3 || month > 10) return false;
    if (month > 3 && month < 10) return true;
    int lastSunMarch = 31 - (DateTime(year, 3, 31).dayOfTheWeek());
    if (month == 3) {
        if (day > lastSunMarch) return true;
        if (day == lastSunMarch && hour >= 1) return true;
        return false;
    }
    int lastSunOct = 31 - (DateTime(year, 10, 31).dayOfTheWeek());
    if (month == 10) {
        if (day < lastSunOct) return true;
        if (day == lastSunOct && hour < 1) return true;
        return false;
    }
    return false;
}

DateTime SensorManager::getLocalTime() {
    DateTime utc = rtc.now();
    int offset = isDST_EU(utc) ? 2 : 1;
    return utc + TimeSpan(0, offset, 0, 0);
}

void SensorManager::setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
    rtc.adjust(DateTime(year, month, day, hour, min, sec));
}

void SensorManager::setTime(uint32_t timestamp) {
    rtc.adjust(DateTime(timestamp));
}

float SensorManager::getGForceX() {
    if (mpuActive) return mpu.getAccX();
    if (rawMpuAddress != 0) {
        Wire.beginTransmission(rawMpuAddress);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(rawMpuAddress, (uint8_t)2);
        if (Wire.available() == 2) {
            int16_t ax = (Wire.read() << 8) | Wire.read();
            return (float)ax / 16384.0;
        }
    }
    return 0.0;
}

float SensorManager::getGForceY() {
    if (mpuActive) return mpu.getAccY();
    if (rawMpuAddress != 0) {
        Wire.beginTransmission(rawMpuAddress);
        Wire.write(0x3D);
        Wire.endTransmission(false);
        Wire.requestFrom(rawMpuAddress, (uint8_t)2);
        if (Wire.available() == 2) {
            int16_t ay = (Wire.read() << 8) | Wire.read();
            return (float)ay / 16384.0;
        }
    }
    return 0.0;
}

float SensorManager::getAltitude() {
    return bmpActive ? bmp.readAltitude(baselinePressure) : 0.0;
}

float SensorManager::getTemperature() {
    return bmpActive ? bmp.readTemperature() : 0.0;
}
