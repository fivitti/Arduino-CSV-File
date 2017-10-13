/**
*  Project: CSVFile - unit test
*  @author: Slawomir Figiel
*  @contact: fivitti@gmail.com
*  @date: 14.08.2016
*  @version: 1.0.1
*  @license: MIT
*/

#ifndef CSVFileTestPart6_h
#define CSVFileTestPart6_h

#include "CSVFileTestInit.h"
#include "UnitTestUtils.h"

test(getNumberOfLine_ok)
{
  beginTest();

  file.gotoBeginOfFile();
  file.nextField();
  assertEqual(file.getNumberOfLine(), 0);
  file.nextLine();
  assertEqual(file.getNumberOfLine(), 1);
  file.nextLine();
  file.nextField();
  assertEqual(file.getNumberOfLine(), 2);
  assertEqual(file.getNumberOfLine(), 2);
  assertEqual(file.getNumberOfLine(), 2);
  file.nextLine(); //Skipped remove line
  assertEqual(file.getNumberOfLine(), 3);
  endTest();
}

test(isEmptyLine_ok)
{
#if CSV_FILE_ENABLE_CHECK_EMPTY_LINE

  beginTest();
  file.seekEnd();
  file.addLine();
  file.gotoBeginOfLine();
  assertTrue(file.isEmptyLine());
  file.gotoBeginOfLine();
  file.write('\0');
  file.write('\0');
  file.gotoBeginOfLine();
  assertTrue(file.isEmptyLine());
  endTest();

#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_CHECK_EMPTY_LINE
}

test(isEmptyLine_failed)
{
#if CSV_FILE_ENABLE_CHECK_EMPTY_LINE
  beginTest();
  file.gotoBeginOfFile();
  assertFalse(file.isEmptyLine());
  endTest();
#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_CHECK_EMPTY_LINE
}

test(gotoField_ok)
{
  beginTest();
  file.gotoBeginOfFile();
  assertTrue(file.gotoField(1));
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter);
  assertTrue(file.gotoField(2));
  assertEqual(file.curPosition(), start1Line + length1Line1Field + lengthDelimiter + length1Line2Field + lengthDelimiter);

  file.nextLine();
  file.seekCur(1);
  assertTrue(file.gotoField(1));
  assertEqual(file.curPosition(), start2Line + length2Line1Field + lengthDelimiter);
  file.seekCur(2);
  assertTrue(file.gotoField(2));
  assertEqual(file.curPosition(), start2Line + length2Line1Field + lengthDelimiter + length2Line2Field + lengthDelimiter);
  endTest();
}

test(gotoField_failed)
{
  beginTest();
  file.gotoBeginOfFile();
  assertFalse(file.gotoField(100));
  assertTrue(file.isEndOfLine());
  endTest();
}

test(addField_empty_ok)
{
  beginTest();

  file.seekEnd();   //It function break numbering of line and fields.
  file.gotoBeginOfLine(); //It function fix fields numbering.
  file.nextField();
  file.nextField();
  file.nextField();
  assertTrue(file.addField());
  int beginOfCurrentField = file.curPosition();
  assertTrue(file.gotoField(3));
  assertEqual(file.curPosition(), beginOfCurrentField);

  file.seekCur(-1);
  assertEqual((char)file.read(), ';');
  assertTrue(file.isEndOfFile());

  assertTrue(file.addField());
  beginOfCurrentField = file.curPosition();
  assertTrue(file.gotoField(4));
  assertEqual(file.curPosition(), beginOfCurrentField);
  assertTrue(file.addField());
  beginOfCurrentField = file.curPosition();
  assertTrue(file.gotoField(5));
  assertEqual(file.curPosition(), beginOfCurrentField);
  assertTrue(file.addField());
  beginOfCurrentField = file.curPosition();
  assertTrue(file.gotoField(6));
  assertEqual(file.curPosition(), beginOfCurrentField);
  file.seekCur(-2);
  assertEqual((char)file.read(), ';');
  assertEqual((char)file.read(), ';');
  assertTrue(file.isEndOfFile());

  endTest();
}

test(addField_empty_failed)
{
#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
  beginTest();

  file.gotoBeginOfFile();
  assertFalse(file.addField());

  endTest();
#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
}

test(addField_num_non_fixed_size_failed)
{
#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
  beginTest();

  file.gotoBeginOfFile();
  assertFalse(file.addField(344));

  endTest();
#else
  skip();
  return;
#endif //CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
}

test(addField_num_non_fixed_size_ok)
{
  beginTest();
  file.seekEnd();
  file.gotoBeginOfLine(); //It function fix fields numbering.
  file.nextField();
  file.nextField();
  file.nextField();
  int beginOfCurrentField = file.curPosition() + 1;
  assertTrue(file.addField(123));
  assertTrue(file.gotoField(3));
  assertEqual(file.curPosition(), beginOfCurrentField);

  const int bufferSize = 3;
  char buffer_[bufferSize];
  int bufferNum = -1;

  assertTrue(file.readField(bufferNum, buffer_, bufferSize));
  assertEqual(bufferNum, 123);

  endTest();
}

test(copyField_ok)
{
  beginTest();

  SdBaseFile * outFile = new SdBaseFile();
  outFile->open(SECOND_UNIT_TEST_FILE, O_RDWR | O_CREAT);

  file.gotoBeginOfFile();
  file.copyField(outFile);

  outFile->seekSet(0);
  
  const int bufferSize = 4;
  char buffer_[bufferSize];

  outFile->read(buffer_, bufferSize);
  assertTrue(equalCharArrays(buffer_, "Ala"));

  outFile->remove();
  outFile->close();
  delete outFile;
  endTest();
}

test(copyField_ok2)
{
  beginTest();

  SdBaseFile * outFile = new SdBaseFile();
  outFile->open(SECOND_UNIT_TEST_FILE, O_RDWR | O_CREAT);

  file.gotoBeginOfFile();
  file.copyField(outFile);
  file.nextField();
  file.copyField(outFile);

  outFile->seekSet(0);

  const int bufferSize = 4;
  char buffer_[bufferSize];

  outFile->read(buffer_, bufferSize);
  assertTrue(equalCharArrays(buffer_, "Alam"));

  outFile->read(buffer_, bufferSize);
  assertTrue(equalCharArrays(buffer_, "a"));

  outFile->remove();
  outFile->close();
  delete outFile;
  endTest();
}

test(copyField_ok3)
{
  beginTest();

  SdBaseFile * outFile = new SdBaseFile();
  outFile->open(SECOND_UNIT_TEST_FILE, O_RDWR | O_CREAT);

  file.gotoBeginOfFile();
  file.copyField(outFile);
  file.nextField();
  file.nextField();
  file.copyField(outFile);

  outFile->seekSet(0);

  const int bufferSize = 4;
  char buffer_[bufferSize];

  outFile->read(buffer_, bufferSize);
  assertTrue(equalCharArrays(buffer_, "Alak"));
  outFile->read(buffer_, bufferSize);
  assertTrue(equalCharArrays(buffer_, "ota"));

  outFile->remove();
  outFile->close();
  delete outFile;
  endTest();
}

#endif //CSVFileTestPart6_h
