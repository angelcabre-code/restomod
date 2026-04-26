# ESP32 Restomod Dashboard & Atomic Clock 🚗💨

Este proyecto es un cuadro de instrumentos digital y reloj de alta precisión diseñado para vehículos clásicos y restomods. Combina estética retro-digital con sensores modernos y control total desde una Web App vía Bluetooth (BLE).

## ✨ Características Principales

- **Doble Estilo de Reloj**: Alterna entre un estilo **Digital Retro** (segmentos clásicos) y un **Digital Moderno** (grueso y minimalista).
- **Auto-Ajuste Horario (DST)**: Sincronización UTC pura con el RTC DS3231. El sistema calcula automáticamente el horario de verano/invierno europeo según la fecha.
- **Sensores en Tiempo Real**:
  - **Altímetro**: Lectura de altitud barométrica mediante BMP280.
  - **G-Force Meter**: Visualización gráfica de fuerzas G (X/Y) mediante MPU6500.
  - **Termómetro**: Temperatura ambiente del habitáculo.
- **Sistema de Temas Avanzado**:
  - 10 perfiles de color preconfigurados (Cyberpunk, Matrix, Synthwave, etc.).
  - Fondos dinámicos: **Degradados** suaves y **tramas técnicas** (rejilla y puntos) ultra-sutiles.
  - Totalmente personalizable (colores de texto, acento y fondo).
- **Web App Companion**: Interfaz móvil optimizada para controlar todo el sistema, ver telemetría y sincronizar la hora atómica.
- **Control Físico**: Soporte para un botón físico (clic corto para temas, clic largo para cambiar tipografía).
- **Persistencia Total**: Todos los ajustes se guardan en la memoria interna (NVS) y sobreviven a cortes de energía.

## 🛠️ Hardware Necesario

1. **Microcontrolador**: ESP32 (WROOM-32 / DevKit v1).
2. **Pantalla**: TFT ST7789 1.14" (240x135 píxeles) - Variante ES32Lab.
3. **Sensores (GY-91)**:
   - MPU6500 (Acelerómetro/Giroscopio).
   - BMP280 (Barómetro/Temperatura).
4. **Reloj**: RTC DS3231 (I2C) para máxima estabilidad.
5. **Botón**: Pulsador momentáneo (conectado al Pin 33).

## 🚀 Instalación

1. Clona este repositorio.
2. Abre el proyecto en el Arduino IDE.
3. Instala las librerías necesarias:
   - `TFT_eSPI` (configurada para ST7789 240x135).
   - `RTClib` (Adafruit).
   - `Adafruit_MPU6050` o similar para el MPU6500.
   - `Adafruit_BMP280`.
4. Sube el código a tu ESP32.
5. Abre `app/index.html` en tu móvil (vía HTTPS) para controlar el reloj.

---
Proyecto desarrollado para la comunidad Restomod. ¡Disfrútalo!