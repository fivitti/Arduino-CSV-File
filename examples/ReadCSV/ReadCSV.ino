/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Read CSV fields feature examples.
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
  // You shouldn't use "readln" method (and similars).
  // The fields are separated by delimiter ';'.
  // You can change this character in source file.
  // Your CSV file shouldn't contain this characters.
  
  initSdFile();

  // First we fill the file with content.
  // We use here standard SdFat function for write.
  // See "WriteCSV" example for write with CSVFile.

  csv.write("One;Two;Three\n"); 
  csv.write("Very long long field.;Second long field.\n"); 
  csv.write("zażółć gęślą jaźń\n");
  csv.write("11;22\0;3df;null\n");
  csv.write(";");
  
  csv.gotoBeginOfFile();

  // We are now at the begin of file.
  // We need buffer for content fields.
  //
  // Buffer can have any size.
  //
  // Good practice is declare buffer size larger by one
  // then we need and put as last null character.
  // It is prevent from overbuffer.
  const byte BUFFER_SIZE = 5;
  char buffer[BUFFER_SIZE + 1];
  buffer[BUFFER_SIZE] = '\0';

  // We can read three types of field
  // 1. Text field with string content.
  //    This field don't have size limit of content.
  //    If buffer has size less then size of filed then
  //    we need read field until read function return false
  //
  //    First line - short fields
  csv.readField(buffer, BUFFER_SIZE);
  Serial.print(buffer); Serial.print(';');
  //    Function read set position in file at last char in field
  //    (before delimiter or end of line). We must call function
  //    for go to next field.
  csv.nextField();
  csv.readField(buffer, BUFFER_SIZE);
  Serial.print(buffer); Serial.print(';');
  csv.nextField();
  csv.readField(buffer, BUFFER_SIZE);
  Serial.println(buffer);

  //    Second line - long fields
  csv.nextLine();
  while (csv.readField(buffer, BUFFER_SIZE))
  {
    Serial.print(buffer);                                 
  }
  Serial.print(';');
  csv.nextField();
  while (csv.readField(buffer, BUFFER_SIZE))
  {
    Serial.print(buffer);
  }
  Serial.println();

  //    Third line - UTF-8 fields
  //    This project support UTF-8 strings. We should
  //    remeber that each non-ASCII character is stored
  //    as pair bytes. You need sufficiently large buffer
  //    (minimum size: 2).
  //    Note: Serial don't support UTF-8 and you can see
  //    random characters.
  csv.nextLine();
  while (csv.readField(buffer, BUFFER_SIZE))
  {
    Serial.print(buffer);
  }
  Serial.println();

  // 2. Numberic field. For read this fields
  //    us buffer should has size equal or greater then
  //    number of digits reading number.
  //    Minimal buffer for bytes: 3 (range from 0 to 255)
  //    Minimal buffer for int:   5 (range from 0 to 65535)
  //    This version of library don't support larger types.
  csv.nextLine();
  //    We need temporary variable for store result convertion
  int numBuffer = 0;

  //    Only number in field:
  csv.readField(numBuffer, buffer, BUFFER_SIZE);
  Serial.print(numBuffer); Serial.print(';');
  //    Number and null char:
  csv.nextField();
  csv.readField(numBuffer, buffer, BUFFER_SIZE);
  Serial.print(numBuffer); Serial.print(';');
  //    Number and literal stuff (only begin digits are important)
  csv.nextField();
  csv.readField(numBuffer, buffer, BUFFER_SIZE);
  Serial.print(numBuffer); Serial.print(';');
  //    Missing digits only string.
  //    Warning! Value of num buffer is set to zero,
  //    but readField function return false!
  csv.nextField();
  if (csv.readField(numBuffer, buffer, BUFFER_SIZE))
    Serial.println(numBuffer);
  else
    Serial.println(F("Missing digits!"));

  // 3. Empty lines, without content
  csv.nextField();
  csv.readField(buffer, BUFFER_SIZE);
  Serial.print(buffer); Serial.print(';');
  csv.nextField();
  csv.readField(buffer, BUFFER_SIZE);
  Serial.println(buffer);
   
  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
