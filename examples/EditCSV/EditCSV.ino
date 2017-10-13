/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Edit numberic field with fixed size feature example.
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

// =*= END CONFIG =*=

SdFat sd;
CSVFile csv;

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
}

void loop() {
  // Data in CSV file is stored in lines.
  // Each line have some (or zero) fields.

  // Each line is ended by end line character '\n',
  // (UNIX style - without '\r').

  // We cannot add new character in the middle of file.
  // We can only replace characters or add new at end 
  // of file.
  // We assume that:
  //  1. Field for edit have constant size and it size is
  //     known for programmer
  //  2. Field for edit is numberic field.
  
  initSdFile();

  // First we fill the file with content.
  // We add one field with size 3.
  // CSVFile support (in this version) only
  // edit byte value. We can set new value
  // from 0 to 255.
  //
  // For size 2 we can set new value 0-99.
  // If we set greater then 99 value then
  // we override delimiter of file when edit.
  csv.addField(1, 3);
  csv.addField("not override");
  csv.gotoBeginOfLine();

  // Edit field
  csv.editField(255);

  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
