/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef Config_h
#define Config_h

#include <SdFat.h>

// SPI pinout
#define PIN_SPI_CLK 13
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12

#define PIN_SD_CS 10
#define PIN_OTHER_SPI_DEVICE_CS 4

// SD config
#define SPI_SD_MAX_SPEED SPI_FULL_SPEED //SPI_QUARTER_SPEED //SPI_HALF_SPEED

// SPI config
#define SPI_USE_TRANSACTION 1
//Remember set SD config in SD FAT Config

void setupPinout() {
  pinMode(PIN_SD_CS, OUTPUT);
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_CLK, OUTPUT);
  //Disable SPI devices
  digitalWrite(PIN_OTHER_SPI_DEVICE_CS, HIGH);
  digitalWrite(PIN_SD_CS, HIGH);
}

#endif //Config.h





