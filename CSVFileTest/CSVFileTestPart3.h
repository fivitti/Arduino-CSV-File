/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart_3_h
#define CSVFileTestPart_3_h

#include "CSVFileTestInit.h"

test(gotoLine_start_ok) {
#if CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
  beginTest();
  // Enumeration from 0 to 3

  assertTrue(file.gotoLine("Ala"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine("2"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  assertTrue(file.gotoLine("Ale"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  //Delete line
  assertTrue(file.gotoLine("historia"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);

  // Enumeration from 3 to 0
  file.seekEnd();
  assertFalse(file.gotoLine("his"));  //Fourth line
  file.gotoBeginOfFile();
  assertTrue(file.gotoLine("his"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  assertFalse(file.gotoLine("Ale;"));  //Third line
  file.gotoBeginOfFile();
  assertTrue(file.gotoLine("Ale;"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  assertFalse(file.gotoLine("2;kot"));  //Second line
  file.gotoBeginOfFile();
  assertTrue(file.gotoLine("2;kot"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);

  // Random access
  assertTrue(file.gotoLine("Ale"));  //Third line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start3Line);
  file.gotoBeginOfFile();
  assertTrue(file.gotoLine("Ala;"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), 0);
  assertTrue(file.gotoLine("hi"));  //Fourth line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start5Line);
  file.gotoBeginOfFile();
  assertTrue(file.gotoLine("2;"));  //Second line
  assertTrue(file.isBeginOfLine());
  assertEqual(file.curPosition(), start2Line);
  
  endTest();
#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
}

test(gotoLine_start_failed) {
#if CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
  beginTest();
  assertFalse(file.gotoLine("ala"));
  assertTrue(file.isEndOfFile());
  assertFalse(file.gotoLine("xxx"));
  assertTrue(file.isEndOfFile());
  endTest();
#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
}

test(markLineAsDelete_begin_file_ok) {
  beginTest();
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsDelete());
  file.gotoBeginOfFile();
  assertFalse(file.isLineMarkedAsDelete());
  file.seekSet(0);
  assertTrue(file.isLineMarkedAsDelete());
  endTest();
}

test(markLineAsDelete_remark_ok) {
  beginTest();
  // Mark first
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsDelete());
  file.gotoBeginOfFile();
  assertFalse(file.isLineMarkedAsDelete());
  file.seekSet(0);
  assertTrue(file.isLineMarkedAsDelete());
  // Mark second
  file.gotoBeginOfFile();
  assertTrue(file.markLineAsDelete());
  file.gotoBeginOfFile();
  assertFalse(file.isLineMarkedAsDelete());
  file.seekSet(start2Line);
  assertTrue(file.isLineMarkedAsDelete());
  endTest();
}

test(markLineAsDelete_ok) {
  beginTest();
  file.nextLine();
  assertTrue(file.markLineAsDelete());
  file.seekSet(start2Line);
  assertTrue(file.isLineMarkedAsDelete());
  endTest();
}

test(addLine_ok) {
  beginTest();
  file.seekEnd();
  assertTrue(file.addLine());
  assertTrue(file.isEndOfFile());
  file.seekCur(-1);
  assertTrue(file.read() == '\n');
  endTest();
}

test(addLine_failed) {
  #if ! CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
    skip();
    return;
  #endif
  beginTest();
  file.gotoBeginOfFile();
  assertFalse(file.addLine());
  endTest();
}
#endif //CSVFileTestPart_3_h





