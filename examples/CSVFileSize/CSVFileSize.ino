/**
 *  Project: CSVFile - examples
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 14.08.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

/*
 * Program to compare size of CSVFile with SdFile from SdFat.
 * See SdFatSize.ino (in SdFat examples) for SdFat SD program.
 *
 */
#include <SdFat.h>
#include <CSVFile.h>

SdFat sd;
CSVFile csv;

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* wait for Leonardo */ } 
  // Setup SD card
  if (!sd.begin())
  {
    Serial.println("begin failed");
    return;
  }

  csv.open("CSV.csv", O_RDWR | O_CREAT);
  csv.addField("Hello");
  csv.close();
  Serial.println("Done");
}

void loop() {}
