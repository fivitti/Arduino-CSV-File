/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Mark line as delete feature example.
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

  // For enabled delete function go to CSVFile.h and set:
  // #define CSV_FILE_ENABLE_DELETING_LINE 1

  // We don't really remove characters from file.
  // We should overwrite deleted fragment by all tail file content,
  // but for large file it can during a long time.
  // For fast processing CSVFile has function which mark line as delelte,
  // but nor really remove it.
  // We overwrite beign of line with delete marker. When line is start
  // with delete marker we known, that this line should be ignored.

  // Warning! This function has line length limitation. For avoid conflicts
  // with correct content file the delete marker has size 4.
  // Line shorter then 4 characters cannot be deleted! Be careful, because
  // delete shorter line can overwrite data.
  // For debug you can enable function that deny overwrite data. You can
  // enabled it by define CSV_CHECK_OVERWRITE_ERROR.

  // Warning! For correct work this function position in file should be set
  // at begin of deleting line.
  
  initSdFile();

  // First we fill the file with content.
  // We use here standard SdFat function for write.
  // See "WriteCSV" example for write with CSVFile.

  csv.write("OneLong\n"); 
  csv.write("TwoLong\n"); 
  csv.write("Three\n");
  csv.write("Four\n");  // Mark as delete
  csv.write("Five\n");
  csv.write("Six\n");
  csv.write("Seven\n"); // Mark as delete
  csv.write("Eight\n");
  csv.write("Nine\n");
  csv.write("Ten");

  csv.gotoBeginOfFile();
  while (csv.nextLine()) ; //Skip
  Serial.print(F("Begin number of lines: "));
  Serial.println(csv.getNumberOfLine() + 1);
  
  csv.gotoBeginOfFile();
  csv.markLineAsDelete();
  csv.nextLine();
  csv.nextLine();
  csv.markLineAsDelete();
  csv.nextLine();
  csv.markLineAsDelete();
  
  csv.gotoBeginOfFile();
  while (csv.nextLine()) ; //Skip
  Serial.print(F("After deleting number of lines: "));
  Serial.println(csv.getNumberOfLine() + 1);

  // In your custom logic you can use function isLineMarkedAsDelete.
  // In standard this function isn't need, because pointer never set
  // at begin deleted line.
  // Warning! This function not set position in file at the begin of line.
  // After call you can do it manually.
  csv.seekSet(0);
  Serial.print(F("First line is deleted? "));
  Serial.println(csv.isLineMarkedAsDelete() ? F("True") : F("False"));
  
  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
