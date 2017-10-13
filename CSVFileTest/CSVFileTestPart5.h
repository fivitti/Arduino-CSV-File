/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart5_h
#define CSVFileTestPart5_h

#include "CSVFileTestInit.h"
#include "UnitTestUtils.h"

test(nextField_ok) {
  beginTest();
  file.gotoBeginOfFile();
  assertTrue(file.nextField());
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter);
  assertTrue(file.nextField());
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter + length1Line2Field + lengthDelimiter);

  file.nextLine();
  file.seekCur(1);
  assertTrue(file.nextField());
  assertEqual(file.curPosition(), start2Line + length2Line1Field + lengthDelimiter);
  file.seekCur(2);
  assertTrue(file.nextField());
  assertEqual(file.curPosition(), start2Line + length2Line1Field + lengthDelimiter + length2Line2Field + lengthDelimiter);
  endTest();
}

test(nextField_failed) {
  beginTest();

  file.gotoBeginOfFile();
  file.nextField();
  file.nextField();
  assertFalse(file.nextField());

  file.seekEnd();
  assertFalse(file.nextField());
  endTest();
}

test(editField_ok) {
  beginTest();

  char buffer_[3];
  byte bufferSize = 3;

  file.gotoBeginOfFile();
  file.editField(32);
  file.gotoBeginOfFile();;

  assertEqual(file.readField(buffer_, bufferSize), 3);
  assertTrue(equalCharArrays(buffer_, "32\0"));

  file.gotoBeginOfFile();
  assertTrue(file.editField(145));
  file.gotoBeginOfFile();;

  assertEqual(file.readField(buffer_, bufferSize), 3);
  assertTrue(equalCharArrays(buffer_, "145"));

  endTest();
}

test(addField_ok_existed_row) {
  beginTest();
  file.seekEnd();

  assertTrue(file.addField("new_field"));
  assertTrue(file.gotoBeginOfLine()); // 0 field
  assertTrue(file.nextField()); // 1 field
  assertTrue(file.nextField()); // 2 field
  assertTrue(file.nextField()); // 3 (new) field

  char buffer_[9];
  byte bufferSize = 9;
  
  assertEqual(file.readField(buffer_, bufferSize), 9);
  assertTrue(equalCharArrays(buffer_, "new_field"));

  assertTrue(file.gotoBeginOfLine()); // 0 field
  assertTrue(file.gotoField(3));

  assertEqual(file.readField(buffer_, bufferSize), 9);
  assertTrue(equalCharArrays(buffer_, "new_field"));

  // Is end of file (without end delimiter or end line char)?
  assertEqual(file.available(), 0);
  endTest();
}

test(addField_ok_new_line) {
  beginTest();
  
  file.seekEnd();
  file.addLine();
  
  assertTrue(file.addField("new_field"));
  assertTrue(file.gotoBeginOfLine()); // 1 (new) field

  char buffer_[9];
  byte bufferSize = 9;
  
  assertEqual(file.readField(buffer_, bufferSize), 9);
  assertTrue(equalCharArrays(buffer_, "new_field"));

  file.gotoField(0);
  assertEqual(file.readField(buffer_, bufferSize), 9);
  assertTrue(equalCharArrays(buffer_, "new_field"));
  // Is end of file (without end delimiter or end line char)?
  assertEqual(file.available(), 0);
  endTest();
}


test(addField_failed) {
  #if ! CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
    skip();
    return;
  #else
    beginTest();

    file.gotoBeginOfFile();
    assertFalse(file.addField("any"));
    endTest();
  #endif //CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
}

test(addField_num_ok) {
  beginTest();

  char buffer_[4];
  byte bufferSize = 4;

  file.seekEnd();
  assertTrue(file.addField(33, 2));
  assertTrue(file.addField(7, 4));
  assertTrue(file.gotoBeginOfLine()); // 0 field
  assertTrue(file.nextField()); // 1 field
  assertTrue(file.nextField()); // 2 field
  assertTrue(file.nextField()); // 3 (new) field

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "33"));

  assertTrue(file.nextField()); // 4 (new) field
  assertEqual(file.readField(buffer_, bufferSize), 4);

  Serial.print(F("Buffer: "));
  Serial.println(buffer_);
  assertTrue(equalCharArrays(buffer_, "7\0\0\0"));
  
  assertTrue(file.gotoBeginOfLine()); // 0 field
  assertTrue(file.gotoField(3)); // 3 (new) field

  assertEqual(file.readField(buffer_, bufferSize), 2);
  assertTrue(equalCharArrays(buffer_, "33"));

  assertTrue(file.gotoField(4)); // 4 (new) field
  assertEqual(file.readField(buffer_, bufferSize), 4);
  assertTrue(equalCharArrays(buffer_, "7\0\0\0"));

  // Is end of file (without end delimiter or end line char)?
  assertEqual(file.available(), 0);
  endTest();
}
#endif //CSVFileTestPart5_h

