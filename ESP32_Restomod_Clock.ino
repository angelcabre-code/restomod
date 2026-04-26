#include <Arduino.h>
#include "SensorManager.h"
#include "DisplayManager.h"
#include "BLESync.h"

// Pin para el botón externo
#define BUTTON_PIN 33 

bool buttonState = HIGH;
bool lastReading = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200; // 200ms anti-ruido

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    sensors.begin();
    display.begin();
    bleSync.begin();
    
    Serial.println("Restomod OS Inicializado");
}

void loop() {
    sensors.update();
    bleSync.process();

    bool reading = digitalRead(BUTTON_PIN);
    
    // Si la lectura cambia (por ruido o pulsación real)
    if (reading != lastReading) {
        lastDebounceTime = millis();
    }

    // Si ha pasado suficiente tiempo de estabilidad
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
            buttonState = reading;
            
            if (digitalRead(33) == LOW) {
                unsigned long pressStart = millis();
                while(digitalRead(33) == LOW) {
                    if (millis() - pressStart > 1000) break; // Límite para evitar esperas infinitas
                    delay(10);
                }
                unsigned long duration = millis() - pressStart;
                
                if (duration > 50) { // Antirrebote
                    if (duration < 600) {
                        display.cycleColorProfile();
                    } else {
                        display.toggleFont();
                    }
                }
            }
        }
    }
    
    lastReading = reading;

    // Actualizar motor de widgets
    display.update();
}
