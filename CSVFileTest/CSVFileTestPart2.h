/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart_2_h
#define CSVFileTestPart_2_h

#include "CSVFileTestInit.h"

test(gotoLine_num_ok) {
  beginTest();
  // Enumeration from 0 to 3
  assertTrue(file.gotoLine(0));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine(1));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.gotoLine(2));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  //Delete line
  assertTrue(file.gotoLine(3));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);

  // Enumeration from 3 to 0
  file.seekEnd();
  assertTrue(file.gotoLine(3));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  assertTrue(file.gotoLine(2));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  assertTrue(file.gotoLine(1));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.gotoLine(0));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);

  // Random access
  assertTrue(file.gotoLine(2));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  assertTrue(file.gotoLine(0));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine(3));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  assertTrue(file.gotoLine(1));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  
  endTest();
}

test(gotoLine_num_failed) {
  beginTest();
  assertFalse(file.gotoLine(18));
  file.seekEnd();
  int endOfFile = file.curPosition();
  file.gotoBeginOfLine();
  assertFalse(file.gotoLine(-17));
  assertEqual(file.curPosition(), endOfFile);
  endTest();
}

#endif //CSVFileTestPart_2_h




