/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Write CSV fields feature example.
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
  // Important note!
  // You should use flag O_RDWR even if you use CSV File
  // only for writting.
  if (!csv.open(FILENAME, O_RDWR | O_CREAT)) {
    Serial.println("Failed open file");
  }
}

void loop() {
  // Data in CSV file is stored in lines.
  // Each line have some (or zero) fields.
  // First you should add line and next
  // add fields. After you can add next line.

  // Each line is ended by end line character '\n',
  // (UNIX style - without '\r').
  // You shouldn't use "println" method (and similars).
  // The fields are separated by delimiter ';'.
  // You can change this character in source file.
  // Your CSV file shouldn't contain this characters.

  // Important note!
  // You should use flag O_RDWR for initialize CSV File even if you use CSV File
  // only for writting.
  
  initSdFile();

  // At the begin of file we don't need
  // add new line.

  // We can add four types of field
  // 1. Text field with string (const char array)
  //    content.
  //    Function don't have size limit of content.
  //    Current version library don't support string
  //    stored in flash memory.
  csv.addField("One");
  csv.addField("Two");
  csv.addField("Three");
  //    This project support UTF-8 strings, but process
  //    single bytes (one UTF-8 character is represented
  //    by two bytes.
  csv.addField("zażółć");
  csv.addField("gęślą");
  csv.addField("jaźń");  

  //Next line
  csv.addLine();
  
  // 2. Number field with non-fixed size.
  //    Use this field if you don't need
  //    edit field's value later.
  //    Support only positive integers.
  //    It is function designed for write
  //    line numbers.
  csv.addField((unsigned int)0);
  csv.addField(65535);
  csv.addField(3444);

  csv.addLine();

  // 3. Number field with fixed size.
  //    Use this field if you need edit
  //    field's value later.
  //    At the moment support only byte
  //    values (from 0 to 255).
  //    You should be sure, that size of field
  //    is equals or greater then right number size.
  csv.addField(4, 1);
  csv.addField(4, 2);
  csv.addField(4, 3);
  csv.addField(55, 2);
  csv.addField(55, 3);
  csv.addField(255, 3);

  csv.addLine();
  
  // 4. Empty field.
  //    Use this field if you want add special content
  //    using standard function from SdFat or if you
  //    copy field from other file
  csv.addField();
  csv.write("Hello"); //Standard function for SdFile, but you should prefer csv.addField("Hello")
  csv.addField();
  csv.write("world");
  //Coping field - commented, because we don't have other CSV file
  //csv.addField();
  //otherCsv.copyField(csv);

  // We don't add empty line at the end of file.
  // CSV file shouldn't end by '\n' char.

  // Don't forget close the file.
  csv.close();

  // After this operations your CSV file should look like this
  // ('\0' is null character):
  /*  One;Two:Three;zazółć;gęślą;jaźń\n
   *  0;65535;3444\n
   *  4;4\0;4\0\0;55;55\0;777\n
   *  Hello;world
   */
   
  waitForKey();
}
