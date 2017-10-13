/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart_1_h
#define CSVFileTestPart_1_h

#include "CSVFileTestInit.h"

test(gotoBeginOfFile_ok) {
  beginTest();
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  file.seekSet(10);
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  file.seekSet(8);
  assertTrue(file.gotoBeginOfFile());
  assertEqual(file.curPosition(), 0);
  file.gotoField(0);
  assertEqual(file.curPosition(), 0);
  endTest();
}

//test(substring_ok) {
//  beginTest();
//  bool status_ = file.isCurrentSubstring("Ala");
//  assertTrue(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("A");
//  assertTrue(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("Al");
//  assertTrue(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("");
//  assertTrue(status_);
//  endTest();
//}
//
//test(substring_failed) {
//  beginTest();
//  file.gotoBeginOfFile();
//  bool status_ = file.isCurrentSubstring("ala");
//  assertFalse(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("ba");
//  assertFalse(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("123");
//  assertFalse(status_);
//  file.gotoBeginOfFile();
//  status_ = file.isCurrentSubstring("!@");
//  assertFalse(status_);
//  endTest();
//}

test(isEndOfFile_ok) {
  beginTest();
  file.seekEnd();
  assertTrue(file.isEndOfFile());
  endTest();
}

test(isEndOfFile_failed) {
  beginTest();
  assertFalse(file.isEndOfFile());
  endTest();
}

test(isBeginOfLine_ok) {
  beginTest();
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(isBeginOfLine_ok_2) {
  beginTest();
  file.nextLine();
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(isBeginOfLine_failed) {
  beginTest();
  file.seekSet(1);
  assertFalse(file.isBeginOfLine());
  endTest();
}

test(isEndOfLine_ok) {
  beginTest();
  file.nextLine();
  file.seekCur(-1);
  assertTrue(file.isEndOfLine());
  endTest();
}

test(isEndOfLine_ok_2) {
  beginTest();
  file.seekEnd();
  assertTrue(file.isEndOfLine());
  endTest();
}

test(isEndOfLine_failed) {
  beginTest();
  file.seekCur(1);
  assertFalse(file.isEndOfLine());
  endTest();
}

test(isLineMarkedAsDelete_ok) {
  beginTest();
  assertFalse(file.isLineMarkedAsDelete());
  endTest();
}

test(isLineMarkedAsDelete_ok2) {
  beginTest();
  file.nextLine();
  file.nextLine();
  file.seekCur(length3Line);
  assertTrue(file.isLineMarkedAsDelete());
  endTest();
}

test(isLineMarkedAsDelete_failed) {
  beginTest();
  file.nextLine();
  file.nextLine();
  file.seekCur(length3Line);
  assertTrue(file.isLineMarkedAsDelete());
  // When we repeat check at the same line we get false,
  // because this function don't back pointer in file
  assertFalse(file.isLineMarkedAsDelete());
  endTest();
}

test(gotoBeginOfLine_ok) {
  beginTest();
  assertTrue(file.gotoBeginOfLine());
  assertTrue(file.isBeginOfLine());
  file.gotoField(0);
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(gotoBeginOfLine_ok2) {
  beginTest();
  file.nextLine();
  file.seekCur(2);
  assertTrue(file.gotoBeginOfLine());
  assertTrue(file.isBeginOfLine());
  file.gotoField(0);
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(nextLine_ok) {
  beginTest();
  assertTrue(file.nextLine());  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.nextLine());  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  file.gotoField(0);
  assertTrue(file.isBeginOfLine());
  //Delete line
  assertTrue(file.nextLine());  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  file.gotoField(0);
  assertTrue(file.isBeginOfLine());
  endTest();
}

test(nextLine_failed) {
  beginTest();
  file.seekEnd();
  uint32_t endFilePosition = file.curPosition();
  file.seekEnd();
  assertFalse(file.nextLine());
  assertEqual(file.curPosition(), endFilePosition);
  file.seekEnd(-3);
  assertFalse(file.nextLine());
  assertEqual(file.curPosition(), endFilePosition);
}

#endif //CSVFileTestPart_1_h





