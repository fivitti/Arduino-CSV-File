/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Example navigation in CSV file.
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

  // Warning! For correct work functions gotoLine and gotoField
  // you should be careful when you use standard SdFile function
  // (as e.g: seekCur, seekEnd, read, print), because can corrupt
  // line numbering (any standard function are not overloading
  // for correct work with numbering).
  
  initSdFile();

  // First we fill the file with content.
  // We use here standard SdFat function for write.
  // See "WriteCSV" example for write with CSVFile.

  csv.write("One;Two;Three\n");
  csv.write("Four;Five;Six\n"); 
  csv.write("Seven;Eight;Nine\n");  
  csv.write("Ten;Eleven;Twelve");

  // === Navigation in file ===
  
  // This function set position in file at the begin
  csv.gotoBeginOfFile();
  // Now we are in first line. For go to next line:
  csv.nextLine();
  // Now we are in second line.
  csv.nextLine();
  // Now we are in thrird line
  csv.nextLine();
  // Now we are in fourth line. It is last line. Check it.
  if (csv.nextLine())
    Serial.println(F("Oh no! Something is wrong!"));
  else
    Serial.println(F("It is last line."));

  // When function nextLine return false pointer is at the end of file
  // We can check it:
  if (csv.isEndOfFile())
    Serial.println(F("End of file."));

  // We can go to specific line. Line are numbered from zero.
  csv.gotoLine(0); //First line
  csv.gotoLine(2); //Third line
  csv.gotoLine(1); //Second line

  // If CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH define is enabled
  // then we can go to line which begin with specific string
  // It search from current position then we must call gotoBeginOfFile
  #if CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
  csv.gotoBeginOfFile();
  csv.gotoLine("One"); // First line
  csv.gotoBeginOfFile();
  csv.gotoLine("Fo"); // Second line
  #endif //CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH

  // === Navigation in line ===
  // We can easy navigate to begin of line by call
  csv.gotoBeginOfLine();
  // We can check if we are at begin of line:
  if (csv.isBeginOfLine())
    Serial.println(F("Begin of line."));
  // For go to end of line we use trick.
  // It is UNSAFE and corrupt numbering lines and fields
  csv.nextLine();
  csv.seekCur(-1);  // <-- Here we can invalid numbering   
  // We are at end of line we can check it:
  if (csv.isEndOfLine())
    Serial.println(F("End of line."));
  // Ok, go back to begin of line.
  csv.gotoBeginOfLine(); // <-- This function repair field numbering, but not line!
  // We can move in fields. We are now in first field secon line ("Four")
  csv.nextField();
  // We are now in "Five"
  csv.nextField();
  // We are now in "Six". It is last field.
  // If we call now nextField then function return false and position
  // in file will be set at the end of line
  if (!csv.nextField() && csv.isEndOfLine())
    Serial.println(F("End field, end of line"));
  else
    Serial.println(F("Oh, no! Something is wrong!"));

  // We can navigate in line by number of field.
  // Fields are numbering from zero.
  csv.gotoField(0);   //First field ("Four");
  csv.gotoField(2);   //Third field ("Six");
  csv.gotoField(1);   //Second fild ("Five");
  
  // === Navigation in field ===
  // We can go to begin of field:
  csv.gotoBeginOfField();
  // Function for check if is begin of field is missing in this version
  // but we can check if it it end of field
  // First we should go to end of field. We use trick. This
  // trick corupted field numbering
  csv.seekCur(-1);
  // We are now at end of first field. Check it:
  if (csv.isEndOfField())
    Serial.println(F("End field"));
  else
    Serial.println(F("Oh, no! Something is wrong!"));
  
  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
