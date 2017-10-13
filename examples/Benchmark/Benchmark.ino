/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/**
 *	Benchmark reading and writing CSV file.
 *	It is program similar to code from SdFat examples ("bench").
 *  @cSVFile is of course slower then clear SdFat file.
 */
 
#include <SPI.h>
#include <SdFat.h>
#include <CSVFile.h>
#include <SdFatUtil.h>

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

// =*= BENCHMARK CONFIG =*=
//CSVFile not support larger buffer then 254.
const byte BUFFER_SIZE = 255-1; //Not contain end of line char
const byte WRITE_COUNT = 5;
const byte READ_COUNT = 5;
// File size in MB where MB = 1,000,000 bytes.
const byte FILE_SIZE_MB = 5;
// =*= END BENCHMARK CONFIG =*=

SdFat sd;
CSVFile csv;

// Serial output stream
ArduinoOutStream cout(Serial);

const uint32_t FILE_SIZE = 1000000UL * FILE_SIZE_MB;
char buffer[BUFFER_SIZE + 1];
const unsigned int BUF_SIZE = BUFFER_SIZE + 1;

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

  buffer[BUFFER_SIZE] = '\0';
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
  float s;
  uint32_t t;
  uint32_t maxLatency;
  uint32_t minLatency;
  uint32_t totalLatency;
  
  initSdFile();

  for(unsigned int i=0; i < BUFFER_SIZE; ++i)
    buffer[i] = 'A' + (i % 26);

  cout << F("File size ") << (int)FILE_SIZE_MB << F(" MB\n");
  cout << F("Buffer size ") << BUF_SIZE << F(" bytes\n");
  cout << F("Starting write test, please wait.") << endl << endl;

  // do write test
  uint32_t n = FILE_SIZE/sizeof(buffer);
  cout <<F("write speed and latency") << endl;
  cout << F("speed,max,min,avg") << endl;
  cout << F("KB/Sec,usec,usec,usec") << endl;
  for (uint8_t nTest = 0; nTest < WRITE_COUNT; nTest++) {
    csv.truncate(0);
    maxLatency = 0;
    minLatency = 9999999;
    totalLatency = 0;
    t = millis();
    for (uint32_t i = 0; i < n; i++) {
      uint32_t m = micros();
      if (!csv.addField(buffer)) {
        cout << F("write failed");
        return;
      }
      m = micros() - m;
      if (maxLatency < m) {
        maxLatency = m;
      }
      if (minLatency > m) {
        minLatency = m;
      }
      totalLatency += m;

      if (i < n - 1);
        csv.addLine();
    }
    csv.sync();
    t = millis() - t;
    s = csv.fileSize();
    cout << s/t <<',' << maxLatency << ',' << minLatency;
    cout << ',' << totalLatency/n << endl;
  }
  
  cout << endl << F("Starting read test, please wait.") << endl;
  cout << endl <<F("read speed and latency") << endl;
  cout << F("speed,max,min,avg") << endl;
  cout << F("KB/Sec,usec,usec,usec") << endl;
  // do read test
  for (uint8_t nTest = 0; nTest < READ_COUNT; nTest++) {
    csv.rewind();
    csv.gotoBeginOfFile();
    maxLatency = 0;
    minLatency = 9999999;
    totalLatency = 0;
    t = millis();
    for (uint32_t i = 0; i < n; i++) {
      uint32_t m = micros();
      if (csv.readField(buffer, BUFFER_SIZE) != BUFFER_SIZE)
      {
        cout << F("Error reading file");
        return;
      }
      m = micros() - m;
      if (maxLatency < m) {
        maxLatency = m;
      }
      if (minLatency > m) {
        minLatency = m;
      }
      totalLatency += m;
      csv.nextLine();
    }
    t = millis() - t;
    cout << s/t <<',' << maxLatency << ',' << minLatency;
    cout << ',' << totalLatency/n << endl;
  }
  cout << endl << F("Done") << endl;
  
  // Don't forget close the file.
  csv.close();
   
  waitForKey();
}
