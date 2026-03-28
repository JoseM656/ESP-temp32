#pragma once

// Backlight
#define PIN_TFT_BL      13  // ! Compartido con Flash, puede causar problemas en boot

// TFT SPI
#define PIN_TFT_MOSI    23
#define PIN_TFT_MISO    19
#define PIN_TFT_SCLK    18
#define PIN_TFT_CS      5   // SDIO, úsalo con cuidado
#define PIN_TFT_DC      2   // ! BOOT pin, debe estar LOW al arrancar
#define PIN_TFT_RST     4
// BMP280 I2C
#define PIN_I2C_SDA     21  // WIRE_SDA, ideal para I2C
#define PIN_I2C_SCL     22  // WIRE_SCL, ideal para I2C

// SD SPI (comparte bus con TFT)
#define PIN_SD_CS       15  // ! Compartido con Flash

// Pines a NO usar
// GPIO 6, 7, 8, 9, 10, 11 — HSPI/Flash interno, inutilizables
// GPIO 0  — BOOT, debe estar HIGH al arrancar
// GPIO 2  — afecta boot (debe estar LOW), úsalo solo si sabés lo que hacés
// GPIO 12 — MTDI, afecta voltaje de Flash en boot, evitar
// GPIO 13, 15 — compartidos con Flash, pueden dar problemas
// GPIO 34, 35, 36, 39 — input only, no sirven para CS, DC, RST