/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestInit_h
#define CSVFileTestInit_h

#include <CSVFile.h> 
#include <SdFat.h>
#include <ArduinoUnit.h>
#include "Config.h"

#define UNIT_TEST_FILE "unit.txt"
#define SECOND_UNIT_TEST_FILE "unit2.txt"

SdFat sd;
CSVFile file;

const int length1Line1Field = 3;
const int length1Line2Field = 2;
const int length1Line3Field = 4;
const int length2Line1Field = 1;
const int length2Line2Field = 3;
const int length2Line3Field = 2;
const int length3Line1Field = 3;
const int length3Line2Field = 3;
const int length3Line3Field = 4 + 1; //Polish diacritic
const int length5Line1Field = 8;
const int length5Line2Field = 0;
const int length5Line3Field = 0;

const int lengthDelimiter = 1;
const int lengthEndOfLine = 1;

const int length1Line = length1Line1Field + lengthDelimiter + length1Line2Field + lengthDelimiter + length1Line3Field + lengthEndOfLine;
const int length2Line = length2Line1Field + lengthDelimiter + length2Line2Field + lengthDelimiter + length2Line3Field + lengthEndOfLine;
const int length3Line = length3Line1Field + lengthDelimiter + length3Line2Field + lengthDelimiter + length3Line3Field + lengthEndOfLine;
const int length4Line = 27;
const int length5Line = length5Line1Field + lengthDelimiter + length5Line2Field + lengthDelimiter + length5Line3Field;

const int start1Line = 0;
const int start2Line = start1Line + length1Line;
const int start3Line = start2Line + length2Line;
const int start4Line = start3Line + length3Line;
const int start5Line = start4Line + length4Line;

void setupTest() {
  if (!sd.begin(PIN_SD_CS, SPI_SD_MAX_SPEED)) {
    Serial.println("SD card init error");
    return;
  }
  if (sd.exists(UNIT_TEST_FILE) && !sd.remove(UNIT_TEST_FILE))
  {
    Serial.println("Failed init remove file");
    return;
  }
}

void endTest() {
  file.remove();
  file.close();
}

void beginTest() {
  if (file.isOpen() || sd.exists(UNIT_TEST_FILE))
    endTest();
  
  if (!file.open(UNIT_TEST_FILE, O_RDWR | O_CREAT)) {
    Serial.println("Failed open file");
  }
  file.write("Ala;ma;kota\n");  
  file.write("2;kot;33\n");
  file.write("Ale;Oto;cała\n");
  file.write("@DELcostam;skasowana;linia\n");
  file.write("historia;;");
  file.gotoBeginOfFile();
}

#endif //CSVFileTestInit_h




