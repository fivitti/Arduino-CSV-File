/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart_4_h
#define CSVFileTestPart_4_h

#include "CSVFileTestInit.h"
#include "UnitTestUtils.h"

test(isEndOfField_ok) {
  beginTest();
  // End of first field second line
  file.gotoLine(1);
  file.seekCur(length2Line1Field);
  assertTrue(file.isEndOfField());
  // End of last field second line
  file.gotoLine(2);
  file.seekCur(-1);
  assertTrue(file.isEndOfField());
  // End of last field last line
  file.seekEnd();
  assertTrue(file.isEndOfField());
  // Second (empty field) last line
  file.seekEnd(-1);
  assertTrue(file.isEndOfField());
  endTest();
}

test(isEndOfField_failed) {
  beginTest();
  // Begin of first field second line
  file.gotoLine(1);
  assertFalse(file.isEndOfField());
  // Begin of second field third line
  file.gotoLine(2);
  file.seekCur(length3Line1Field);
  file.seekCur(1);
  assertFalse(file.isEndOfField());
  // End of line - 1
  file.gotoLine(2);
  file.seekCur(-2);
  assertFalse(file.isEndOfField());
  endTest();
}

test(gotoBeginOfField_ok) {
  beginTest();
  
  file.gotoBeginOfFile();
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), 0);
  
  file.seekSet(start1Line + 2);
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), 0);
  
  file.seekSet(start1Line + length1Line1Field + lengthDelimiter + 1);
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter);
  
  file.seekSet(start1Line + length1Line1Field + lengthDelimiter);
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter);
  
  file.seekSet(start2Line + length2Line1Field + lengthDelimiter + length2Line2Field + lengthDelimiter + 1);
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), start2Line + length2Line1Field + lengthDelimiter + length2Line2Field + lengthDelimiter);

  // 3 field - empty
  file.seekSet(start5Line + length5Line1Field + lengthDelimiter + length5Line2Field + lengthDelimiter); // 5 line 3 field.
  assertTrue(file.gotoBeginOfField());
  assertEqual(file.curPosition(), start5Line + length5Line1Field + lengthDelimiter + length5Line2Field + lengthDelimiter);

  endTest();
}

test(readField_ok_whole_words) {
  beginTest();
  char buffer_[4];
  byte bufferSize = 4;
  
  file.gotoBeginOfFile();

  assertEqual(file.readField(buffer_, bufferSize), length1Line1Field);
  assertTrue(equalCharArrays(buffer_, "Ala"));
  assertEqual(file.readField(buffer_, bufferSize), 0);

  file.nextField();
  
  assertEqual(file.readField(buffer_, bufferSize), length1Line2Field);
  assertTrue(equalCharArrays(buffer_, "ma"));
  assertEqual(file.readField(buffer_, bufferSize), 0);

  file.nextField();

  assertEqual(file.readField(buffer_, bufferSize), length1Line3Field);
  assertTrue(equalCharArrays(buffer_, "kota"));
  assertEqual(file.readField(buffer_, bufferSize), 0);
  
  endTest();
}

test(readField_ok_part_words) {
  beginTest();
  char buffer_[2];
  byte bufferSize = 2;
  
  file.gotoBeginOfFile();

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "Al"));
  assertEqual(file.readField(buffer_, bufferSize), 1);
  assertTrue(equalCharArrays(buffer_, "a"));

  file.nextField();
  
  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "ma"));

  file.nextField();

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "ko"));
  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "ta"));
  
  endTest();
}

test(readField_ok_first_part_words) {
  beginTest();
  char buffer_[2];
  byte bufferSize = 2;
  
  file.gotoBeginOfFile();

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "Al"));

  file.nextField();

  file.nextField();

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "ko"));
  
  endTest();
}

test(read_ok_end_of_file) {
  beginTest();
  char buffer_[2];
  byte bufferSize = 2;
  
  file.seekEnd();
  assertEqual(file.readField(buffer_, bufferSize), 0);
  
  endTest();  
}

test(read_num_ok) {
  beginTest();
  
  int result = 0;
  char buffer_[2];
  byte bufferSize = 2;

  file.gotoLine(1);
  assertTrue(file.readField(result, buffer_, bufferSize));
  assertEqual(result, 2);

  file.nextField();
  assertTrue(file.readField(result, buffer_, bufferSize));
  assertEqual(result, 0);
  
  file.nextField();
  assertTrue(file.readField(result, buffer_, bufferSize));
  assertEqual(result, 33);

  endTest();
}

test(read_num_failed) {
  beginTest();
  
  int result = 0;
  char buffer_[2];
  byte bufferSize = 2;

  file.gotoLine(4);
  //Empty field
  file.nextField();
  assertFalse(file.readField(result, buffer_, bufferSize));
  //End of file
  file.seekEnd();
  assertFalse(file.readField(result, buffer_, bufferSize));

  endTest();
}

#endif //CSVFileTestPart_4_h

