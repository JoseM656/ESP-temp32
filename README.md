# Welcome

Portable thermometer using an ESP32, a BMP280 sensor and a TFT display. Measures temperature, atmospheric pressure and estimated altitude, logging the data to a microSD card.

## Hardware

- **ESP32** — Dual-core 240MHz microcontroller with Wi-Fi and Bluetooth.
- **BMP280** — I2C sensor, measures temperature and atmospheric pressure.
- **TFT 240x140** — Small color display for real-time data visualization.
- **MicroSD module** — SPI storage for data logging with timestamp.

## Software

- **C** — Primary language.
- **FreeRTOS** — Task scheduling and resource management via ESP-IDF.