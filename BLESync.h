#ifndef BLE_SYNC_H
#define BLE_SYNC_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BLESync {
public:
    void begin();
    void process();
    bool deviceConnected = false;
    
private:
    BLECharacteristic* pNotifyCharacteristic = nullptr;
};

extern BLESync bleSync;

#endif
