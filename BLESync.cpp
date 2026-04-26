#include "BLESync.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "SensorManager.h"
#include "DisplayManager.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLESync bleSync;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      bleSync.deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      bleSync.deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        if (rxValue[0] == 'T' && rxValue[1] == ':') {
            int y, mo, d, h, mi, s;
            if (sscanf(rxValue.c_str() + 2, "%d,%d,%d,%d,%d,%d", &y, &mo, &d, &h, &mi, &s) == 6) {
                sensors.setTime(y, mo, d, h, mi, s);
            }
        }
        else if (rxValue[0] == 'C' && rxValue[1] == ':') {
            int r, g, b;
            if (sscanf(rxValue.c_str() + 2, "%d,%d,%d", &r, &g, &b) == 3) {
                display.setAccentColorRGB(r, g, b);
            }
        }
        else if (rxValue[0] == 'P' && rxValue[1] == ':') {
            int r, g, b;
            if (sscanf(rxValue.c_str() + 2, "%d,%d,%d", &r, &g, &b) == 3) {
                display.setPrimaryColorRGB(r, g, b);
            }
        }
        else if (rxValue[0] == 'B' && rxValue[1] == ':') {
            int r, g, b;
            if (sscanf(rxValue.c_str() + 2, "%d,%d,%d", &r, &g, &b) == 3) {
                display.setBackgroundColorRGB(r, g, b);
            }
        }
        else if (rxValue[0] == 'F' && rxValue[1] == ':') {
            int f;
            if (sscanf(rxValue.c_str() + 2, "%d", &f) == 1) {
                display.setClockFont(f);
            }
        }
        else if (rxValue[0] == 'M' && rxValue[1] == ':') {
            int m;
            if (sscanf(rxValue.c_str() + 2, "%d", &m) == 1) {
                display.setColorProfile(m);
            }
        }
        else if (rxValue[0] == 'G' && rxValue[1] == ':') {
            int g;
            if (sscanf(rxValue.c_str() + 2, "%d", &g) == 1) {
                display.setBackgroundMode(g);
            }
        }
      }
    }
};

void BLESync::begin() {
    BLEDevice::init("Restomod_Clock");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pNotifyCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                      );
    pNotifyCharacteristic->addDescriptor(new BLE2902());
    pNotifyCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}

void BLESync::process() {
    static unsigned long lastTelemetry = 0;
    if (deviceConnected && pNotifyCharacteristic != nullptr) {
        if (millis() - lastTelemetry > 500) {
            lastTelemetry = millis();
            float alt = sensors.getAltitude();
            float temp = sensors.getTemperature();
            float gx = sensors.getGForceX();
            float gy = sensors.getGForceY();
            char telemetry[64];
            sprintf(telemetry, "D:Alt:%.0fm,Temp:%.1fC,X:%.2f,Y:%.2f", alt, temp, gx, gy);
            pNotifyCharacteristic->setValue(telemetry);
            pNotifyCharacteristic->notify();
        }
    }
}
