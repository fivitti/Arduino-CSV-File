/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Simply example of write, read and edit CSV file.
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
  
  initSdFile();

  // Create CSV
  csv.addField("Hello");
  csv.addField("world!");
  csv.addLine();
  csv.addField(7, 2);

  // Read CSV
  csv.gotoBeginOfFile();

  int bufferSize = 5;
  // Good practice is declare buffer larger by one then need
  // and put null char as end element. It is prevent by
  // overbuffer.
  char buffer[bufferSize + 1];
  buffer[bufferSize] = '\0';
  int numBuffer = 0;

  csv.readField(buffer, bufferSize);
  Serial.println(buffer);
  csv.nextField();
  csv.readField(buffer, bufferSize);
  Serial.println(buffer);
  csv.nextLine();
  csv.readField(numBuffer, buffer, bufferSize);
  Serial.println(numBuffer);

  // Edit CSV
  csv.gotoBeginOfField();
  csv.editField(11);
  csv.gotoBeginOfField();
  csv.readField(numBuffer, buffer, bufferSize);
  Serial.println(numBuffer);
  
  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
