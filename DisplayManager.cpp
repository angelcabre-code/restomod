#include "DisplayManager.h"

DisplayManager display;

void DisplayManager::begin() {
    tft.init();
    tft.setRotation(1); 
    tft.invertDisplay(false); 
    tft.setViewport(18, 82, 284, 76);
    
    loadPreferences(); 
    forceFullRedraw = true;
    tft.fillScreen(bgColor);
}

void DisplayManager::setClockFont(int fontId) {
    clockFont = fontId;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::setAccentColorRGB(uint8_t r, uint8_t g, uint8_t b) {
    accentColor = tft.color565(r, g, b);
    colorProfileIndex = -1;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::setPrimaryColorRGB(uint8_t r, uint8_t g, uint8_t b) {
    primaryColor = tft.color565(r, g, b);
    colorProfileIndex = -1;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::setBackgroundColorRGB(uint8_t r, uint8_t g, uint8_t b) {
    bgColor = tft.color565(r, g, b);
    backgroundMode = 0; // Cambiar a sólido al elegir color
    colorProfileIndex = -1;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::setBackgroundMode(int mode) {
    backgroundMode = mode;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::toggleFont() {
    clockFont = (clockFont == 7) ? 1 : 7;
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::cycleColorProfile() {
    colorProfileIndex = (colorProfileIndex + 1) % 10; 
    applyColorProfile();
    savePreferences();
    forceFullRedraw = true;
}

void DisplayManager::setColorProfile(int index) {
    if (index >= 0 && index < 10) { // Ahora 10 perfiles
        colorProfileIndex = index;
        applyColorProfile();
        savePreferences();
        forceFullRedraw = true;
    }
}

void DisplayManager::applyColorProfile() {
    switch(colorProfileIndex) {
        case 0: // Cyberpunk
            backgroundMode = 0;
            bgColor = TFT_BLACK;
            primaryColor = TFT_WHITE;
            accentColor = tft.color565(255, 0, 0);
            break;
        case 1: // Matrix
            backgroundMode = 0;
            bgColor = TFT_BLACK;
            primaryColor = tft.color565(0, 255, 0);
            accentColor = tft.color565(0, 255, 0);
            break;
        case 2: // Synthwave
            backgroundMode = 1; // Degradado Oscuro
            primaryColor = tft.color565(0, 255, 255);
            accentColor = tft.color565(255, 0, 255);
            break;
        case 3: // Día (Blanco Sólido)
            backgroundMode = 0;
            bgColor = TFT_WHITE;
            primaryColor = TFT_BLACK;
            accentColor = tft.color565(255, 0, 0);
            break;
        case 4: // Grad. Oscuro / Texto Blanco
            backgroundMode = 1;
            primaryColor = TFT_WHITE;
            accentColor = tft.color565(255, 100, 0);
            break;
        case 5: // Grad. Claro / Texto Azul
            backgroundMode = 2;
            primaryColor = tft.color565(0, 50, 150);
            accentColor = tft.color565(200, 0, 0);
            break;
        case 6: // Rejilla / Matrix Green
            backgroundMode = 3;
            bgColor = tft.color565(10, 20, 10);
            primaryColor = tft.color565(0, 255, 0);
            accentColor = tft.color565(0, 255, 0);
            break;
        case 7: // Puntos / Minimal Blue
            backgroundMode = 4;
            bgColor = tft.color565(240, 240, 255);
            primaryColor = tft.color565(0, 100, 200);
            accentColor = tft.color565(255, 0, 50);
            break;
        case 8: // Deep Purple Grad
            backgroundMode = 1;
            primaryColor = tft.color565(255, 200, 0);
            accentColor = tft.color565(0, 255, 255);
            break;
        case 9: // Retro Grid (Amber)
            backgroundMode = 3;
            bgColor = tft.color565(20, 15, 0);
            primaryColor = tft.color565(255, 180, 0);
            accentColor = tft.color565(255, 100, 0);
            break;
    }
}

void DisplayManager::loadPreferences() {
    prefs.begin("restomod", false);
    clockFont = prefs.getUInt("font", 7);
    colorProfileIndex = prefs.getInt("profile", -1);
    backgroundMode = prefs.getInt("bgmode", 0);
    
    if (colorProfileIndex != -1) {
        applyColorProfile();
    } else {
        accentColor = prefs.getUInt("accent", 0xF800);
        primaryColor = prefs.getUInt("primary", TFT_WHITE);
        bgColor = prefs.getUInt("bg", TFT_BLACK);
    }
    prefs.end();
}

void DisplayManager::savePreferences() {
    prefs.begin("restomod", false);
    prefs.putUInt("font", clockFont);
    prefs.putInt("profile", colorProfileIndex);
    prefs.putInt("bgmode", backgroundMode);
    prefs.putUInt("accent", accentColor);
    prefs.putUInt("primary", primaryColor);
    prefs.putUInt("bg", bgColor);
    prefs.end();
}

void DisplayManager::drawVerticalGradient(uint16_t color1, uint16_t color2) {
    for (int y = 0; y < 76; y++) {
        uint8_t r1 = (color1 >> 11) & 0x1F, r2 = (color2 >> 11) & 0x1F;
        uint8_t g1 = (color1 >> 5) & 0x3F, g2 = (color2 >> 5) & 0x3F;
        uint8_t b1 = color1 & 0x1F, b2 = color2 & 0x1F;
        
        uint8_t r = r1 + (r2 - r1) * y / 75;
        uint8_t g = g1 + (g2 - g1) * y / 75;
        uint8_t b = b1 + (b2 - b1) * y / 75;
        
        tft.drawFastHLine(0, y, 284, (r << 11) | (g << 5) | b);
    }
}

void DisplayManager::update() {
    if (forceFullRedraw) {
        if (backgroundMode == 1) { // Grad. Oscuro
            drawVerticalGradient(tft.color565(60, 60, 120), tft.color565(0, 0, 0));
        } else if (backgroundMode == 2) { // Grad. Claro
            drawVerticalGradient(tft.color565(255, 255, 255), tft.color565(180, 200, 255));
        } else if (backgroundMode == 3) { // Rejilla (Sutil)
            tft.fillScreen(bgColor);
            uint16_t gridColor = tft.color565(35, 35, 55); 
            if (bgColor == TFT_WHITE || bgColor > 0xEEEE) gridColor = tft.color565(220, 220, 240);
            for (int x = 0; x < 284; x += 20) tft.drawFastVLine(x, 0, 76, gridColor);
            for (int y = 0; y < 76; y += 20) tft.drawFastHLine(0, y, 284, gridColor);
        } else if (backgroundMode == 4) { // Puntos (Sutil)
            tft.fillScreen(bgColor);
            uint16_t dotColor = tft.color565(40, 40, 60);
            if (bgColor == TFT_WHITE || bgColor > 0xEEEE) dotColor = tft.color565(210, 210, 230);
            for (int x = 5; x < 284; x += 12) {
                for (int y = 5; y < 76; y += 12) {
                    tft.drawPixel(x, y, dotColor);
                }
            }
        } else {
            tft.fillScreen(bgColor);
        }
        
        lastMin = -1; lastGx = 999; lastGy = 999; lastPx = -1; lastPy = -1; lastAlt = -9999;
        forceFullRedraw = false;
        
        tft.setTextColor(secondaryColor); // Sin fondo para que funcione sobre degradados
        tft.setTextDatum(MC_DATUM);
        tft.drawString("ALT", 185, 20, 1); 
        tft.drawString("m", 185, 62, 1);   
    }

    DateTime now = sensors.getLocalTime();
    if (now.minute() != lastMin) {
        // Limpiar área reloj/fecha
        if (backgroundMode == 0) {
            tft.fillRect(0, 0, 150, 76, bgColor);
        } else {
            if (backgroundMode == 1) drawVerticalGradient(tft.color565(60, 60, 120), tft.color565(0, 0, 0));
            else if (backgroundMode == 2) drawVerticalGradient(tft.color565(255, 255, 255), tft.color565(180, 200, 255));
            else if (backgroundMode == 3) { // Rejilla Sutil
                tft.fillRect(0, 0, 150, 76, bgColor);
                uint16_t gridColor = tft.color565(35, 35, 55); 
                if (bgColor == TFT_WHITE || bgColor > 0xEEEE) gridColor = tft.color565(220, 220, 240);
                for (int x = 0; x < 150; x += 20) tft.drawFastVLine(x, 0, 76, gridColor);
                for (int y = 0; y < 76; y += 20) tft.drawFastHLine(0, y, 150, gridColor);
            } else if (backgroundMode == 4) { // Puntos Sutil
                tft.fillRect(0, 0, 150, 76, bgColor);
                uint16_t dotColor = tft.color565(40, 40, 60);
                if (bgColor == TFT_WHITE || bgColor > 0xEEEE) dotColor = tft.color565(210, 210, 230);
                for (int x = 5; x < 150; x += 12) {
                    for (int y = 5; y < 76; y += 12) tft.drawPixel(x, y, dotColor);
                }
            }
        }
        
        tft.setTextColor(primaryColor);
        tft.setTextDatum(MC_DATUM);
        
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", now.hour(), now.minute());
        
        if (clockFont == 7) {
            tft.drawString(timeStr, 75, 28, 7); 
        } else {
            // Moderno Grueso por defecto para el resto (1 o 6)
            tft.drawString(timeStr, 75, 30, 6); 
            tft.drawString(timeStr, 76, 30, 6); 
        }
        
        tft.setTextColor(secondaryColor);
        const char* meses[] = {"enero", "febrero", "marzo", "abril", "mayo", "junio", 
                              "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"};
        char dateStr[32];
        sprintf(dateStr, "%d %s %d", now.day(), meses[now.month()-1], now.year());
        tft.drawString(dateStr, 75, 65, 2);
        
        lastMin = now.minute();
    }
    
    static unsigned long lastAltUpdate = 0;
    if (millis() - lastAltUpdate > 1000 || lastAlt == -9999) {
        lastAltUpdate = millis();
        float alt = sensors.getAltitude();
        
        // Limpiar área altitud
        if (backgroundMode == 0) {
            tft.fillRect(165, 28, 45, 26, bgColor);
        } else {
            // En modos especiales, redibujamos el parche del fondo
            if (backgroundMode == 1) { // Gradiente Oscuro
                for (int y = 28; y < 54; y++) {
                    uint8_t r = (60 * (75-y) + 0 * y) / 75;
                    uint8_t g = (60 * (75-y) + 0 * y) / 75;
                    uint8_t b = (120 * (75-y) + 0 * y) / 75;
                    tft.drawFastHLine(165, y, 45, tft.color565(r, g, b));
                }
            } else if (backgroundMode == 2) { // Gradiente Claro
                for (int y = 28; y < 54; y++) {
                    uint8_t r = (255 * (75-y) + 180 * y) / 75;
                    uint8_t g = (255 * (75-y) + 200 * y) / 75;
                    uint8_t b = (255 * (75-y) + 255 * y) / 75;
                    tft.drawFastHLine(165, y, 45, tft.color565(r, g, b));
                }
            } else if (backgroundMode == 3) { // Rejilla
                tft.fillRect(165, 28, 45, 26, bgColor);
                uint16_t gridColor = tft.color565(35, 35, 55); 
                if (bgColor == TFT_WHITE || bgColor > 0xEEEE) gridColor = tft.color565(220, 220, 240);
                for (int x = 165; x < 210; x++) if (x % 20 == 0) tft.drawFastVLine(x, 28, 26, gridColor);
                for (int y = 28; y < 54; y++) if (y % 20 == 0) tft.drawFastHLine(165, y, 45, gridColor);
            } else if (backgroundMode == 4) { // Puntos
                tft.fillRect(165, 28, 45, 26, bgColor);
                uint16_t dotColor = tft.color565(40, 40, 60);
                if (bgColor == TFT_WHITE || bgColor > 0xEEEE) dotColor = tft.color565(210, 210, 230);
                for (int x = 165; x < 210; x++) {
                    for (int y = 28; y < 54; y++) {
                        if (x % 12 == 5 && y % 12 == 5) tft.drawPixel(x, y, dotColor);
                    }
                }
            }
        }
        
        tft.setTextColor(primaryColor);
        tft.setTextDatum(MC_DATUM);
        char altStr[10];
        sprintf(altStr, "%.0f", alt);
        tft.drawString(altStr, 185, 42, 4);
        lastAlt = alt;
    }

    float gx = sensors.getGForceX();
    float gy = sensors.getGForceY();
    if (fabs(gx - lastGx) > 0.05 || fabs(gy - lastGy) > 0.05) {
        int centerX = 245; int r = 30; int cy = 38;
        float maxG = 1.5;
        int px = centerX + (gx / maxG * r);
        int py = cy + (gy / maxG * r);
        
        if (lastPx != -1) {
            if (backgroundMode == 0) {
                tft.fillCircle(lastPx, lastPy, 6, bgColor);
            } else {
                tft.fillRect(lastPx-6, lastPy-6, 12, 12, bgColor);
                if (backgroundMode == 3) {
                     uint16_t gridColor = tft.color565(35, 35, 55); 
                     if (bgColor == TFT_WHITE || bgColor > 0xEEEE) gridColor = tft.color565(220, 220, 240);
                     for (int x = lastPx-6; x <= lastPx+6; x++) if (x % 20 == 0) tft.drawFastVLine(x, lastPy-6, 12, gridColor);
                     for (int y = lastPy-6; y <= lastPy+6; y++) if (y % 20 == 0) tft.drawFastHLine(lastPx-6, y, 12, gridColor);
                } else if (backgroundMode == 4) {
                     uint16_t dotColor = tft.color565(40, 40, 60);
                     if (bgColor == TFT_WHITE || bgColor > 0xEEEE) dotColor = tft.color565(210, 210, 230);
                     for (int x = lastPx-6; x <= lastPx+6; x++) {
                         for (int y = lastPy-6; y <= lastPy+6; y++) {
                             if (x % 12 == 5 && y % 12 == 5) tft.drawPixel(x, y, dotColor);
                         }
                     }
                }
            }
        }
        
        tft.drawCircle(centerX, cy, r, secondaryColor);
        tft.drawCircle(centerX, cy, r/2, secondaryColor);
        tft.drawLine(centerX, cy - r, centerX, cy + r, secondaryColor);
        tft.drawLine(centerX - r, cy, centerX + r, cy, secondaryColor);
        tft.fillCircle(px, py, 6, accentColor);
        
        lastPx = px; lastPy = py; lastGx = gx; lastGy = gy;
    }
}
