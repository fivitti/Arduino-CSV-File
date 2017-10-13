/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#include <SPI.h>
#include <SdFat.h>
#include <ArduinoUnit.h>

/*
 * The test files are very big
 * and only in part it can be stored
 * in flash memory.
 */

#include "CSVFileTestPart1.h"
//#include "CSVFileTestPart2.h"
//#include "CSVFileTestPart3.h"
//#include "CSVFileTestPart4.h"
//#include "CSVFileTestPart5.h"
//#include "CSVFileTestPart6.h"

void setup() {
  setupPinout();
  SPI.begin();  
  
  #if ! SPI_USE_TRANSACTION  
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  #endif //SPI_USE_TRANSACTION

  Serial.begin(9600);

  setupTest();
}

void loop() {
  Test::run();
}
