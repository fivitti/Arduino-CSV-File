/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Example of copying fields feature.
 */

#include <SdFat.h>
#include <CSVFile.h>

// =*= CONFIG =*=
// SPI pinout
#define PIN_SPI_CLK 13
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 12
#define PIN_SD_CS 10
// If you have connected other SPI device then
// put here number of pin for disable its.
// Provide -1 if you don't have other devices.
#define PIN_OTHER_DEVICE_CS -1
// Change this value if you have problems with SD card
// Available values: SPI_QUARTER_SPEED //SPI_HALF_SPEED
//It is enum from SdFat
#define SD_CARD_SPEED SPI_FULL_SPEED 

#define FILENAME "CSV.csv"
#define FILENAME_TARGET "CSV_2.csv"

// =*= END CONFIG =*=

SdFat sd;
CSVFile csv;
CSVFile csvTarget;

void setup() {
  // Setup pinout
  pinMode(PIN_SPI_MOSI, OUTPUT);
  pinMode(PIN_SPI_MISO, INPUT);
  pinMode(PIN_SPI_CLK, OUTPUT);
  //Disable SPI devices
  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, HIGH);
  
  #if PIN_OTHER_DEVICE_CS > 0
  pinMode(PIN_OTHER_DEVICE_CS, OUTPUT);
  digitalWrite(PIN_OTHER_DEVICE_CS, HIGH);
  #endif //PIN_OTHER_DEVICE_CS > 0
    
  // Setup serial
  Serial.begin(9600);
  while (!Serial) { /* wait for Leonardo */ } 
  // Setup SD card
  if (!sd.begin(PIN_SD_CS, SD_CARD_SPEED))
  {
    Serial.println("SD card begin error");
    return;
  }
}

void waitForKey()
{
  while (Serial.read() >= 0) { }
  Serial.println(F("Type any character to repeat.\n"));
  while (Serial.read() <= 0) { }
}

void initSdFile()
{
  if (sd.exists(FILENAME) && !sd.remove(FILENAME))
  {
    Serial.println("Failed init remove file");
    return;
  }
  if (!csv.open(FILENAME, O_RDWR | O_CREAT)) {
    Serial.println("Failed open file");
  }

  if (!csvTarget.open(FILENAME_TARGET, O_RDWR | O_CREAT)) {
    Serial.println("Failed open target file");
  }
}

void loop() {
  initSdFile();

  // First we fill the file with content.
  // We use here standard SdFat function for write.
  // See "WriteCSV" example for write with CSVFile.

  csv.write("One;Two;Three\n"); 
  csv.write("Four;Five;Six"); 

  csv.gotoBeginOfFile();
  csvTarget.gotoBeginOfFile();
  
  //Copying file 1:1 from csv to csvTarget
  do
  {
    do
    {
      csv.copyField(&csvTarget);
    }
    while (csv.nextField() && csvTarget.addField());
  }
  while (csv.nextLine() && csvTarget.addLine());

  // Don't forget close the file.
  csv.close();
  csvTarget.close();
   
  waitForKey();
}
