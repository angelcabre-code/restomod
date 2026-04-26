#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>
#include <Preferences.h>
#include "SensorManager.h"

class DisplayManager {
public:
    void begin();
    void update();
    
    // Dynamic controls for BLE and Button
    void setClockFont(int fontId);
    void setAccentColorRGB(uint8_t r, uint8_t g, uint8_t b);
    void setPrimaryColorRGB(uint8_t r, uint8_t g, uint8_t b);
    void setBackgroundColorRGB(uint8_t r, uint8_t g, uint8_t b);
    void setBackgroundMode(int mode);
    void toggleFont();
    void cycleColorProfile();
    void setColorProfile(int index);
    void loadPreferences();
    void savePreferences();

private:
    TFT_eSPI tft = TFT_eSPI();
    Preferences prefs;
    
    // Config State
    volatile int clockFont = 7;
    volatile int colorProfileIndex = -1; // -1 significa personalizado por App
    volatile int backgroundMode = 0; // 0=Solid, 1=GradDark, 2=GradLight, 3=Grid, 4=Dots
    volatile bool forceFullRedraw = false;
    
    // Theme Colors
    volatile uint16_t primaryColor = TFT_WHITE;
    volatile uint16_t accentColor = 0xF800; // RED
    uint16_t secondaryColor = TFT_DARKGREY;
    volatile uint16_t bgColor = TFT_BLACK;
    
    // Drawing State variables
    int lastMin = -1;
    float lastGx = 999, lastGy = 999;
    int lastPx = -1, lastPy = -1;
    float lastAlt = -9999;
    
    // Helpers
    void applyColorProfile();
    void drawVerticalGradient(uint16_t color1, uint16_t color2);
};

extern DisplayManager display;

#endif
