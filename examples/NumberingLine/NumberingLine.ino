/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Numbering line feature example.
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

  // You can get number of current line by call @getNumbeOfLine
  // function.
  //
  // If file contains lines marked as delete then these lines will
  // be ignored.
  // For enabled delete function go to CSVFile.h and set:
  // #define CSV_FILE_ENABLE_DELETING_LINE 1
  // If this define is zero (default) then this example display
  // 10 numbers else 8 numbers.

  // Numbering is start from zero.

  // Warning! For correct work this function you should be careful
  // when you use standard SdFile function (as e.g: seekCur, seekEnd,
  // read, print), because can corrupt line numbering (any standard
  // function are not overloading for correct work with numbering).
  
  initSdFile();

  // First we fill the file with content.
  // We use here standard SdFat function for write.
  // See "WriteCSV" example for write with CSVFile.

  csv.write("One\n"); 
  csv.write("Two\n"); 
  csv.write("Three\n");
  csv.write("@DELFour\n");  // Mark as delete
  csv.write("Five\n");
  csv.write("Six\n");
  csv.write("@DELSeven\n"); // Mark as delete
  csv.write("Eight\n");
  csv.write("Nine\n");
  csv.write("Ten");

  // This function set line counter to zero.
  csv.gotoBeginOfFile();

  do
  {
    Serial.println(csv.getNumberOfLine());
  }
  while (csv.nextLine());

  // If CSV_FILE_ENABLE_DELETING_LINE define is zero (default)
  // then this example display 10 numbers else 8 numbers.

  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
