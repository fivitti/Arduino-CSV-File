/**
 *  Project: CSV File - extend class for SdFat library
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 18.07.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

#ifndef CSVFile_h
#define CSVFile_h

#include <SdFat.h>
#include "CSVFileConfig.h"

class CSVFile : public FatFile {
	
	private:
	unsigned int numLine = 0;
	byte numField = 0;
	
	#if CSV_FILE_ENABLE_DELETING_LINE || CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
	// *** Utilities ***
	//UNSAFE FUNCTION!
	//Change pointer position
	//Don't stop when end of field/line.
	//May destroy correct line/field counting
	bool isCurrentSubstring(const char * substr);
	#endif //CSV_FILE_ENABLE_DELETING_LINE || CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
	
	// Write number to file as text. Return false if failed.
	int writeNumber(unsigned int number);
	
	public:
	CSVFile() : FatFile() {}
	virtual ~CSVFile() {}
	
	// *** Files ***
	bool isEndOfFile();
	// Always return true. If first lines are delete then set
	// pointer on the begin of first not deleted line or if all lines are deleted on the
	// end of file.
	bool gotoBeginOfFile();
	
	// *** Lines ***
	bool isBeginOfLine();
	bool isEndOfLine();
	
	//Return number current line. Lines are numbered from 0.
	unsigned int getNumberOfLine();
	
	#if CSV_FILE_ENABLE_CHECK_EMPTY_LINE
	//After set pointer at end of line or at first not empty character
	//Require gotoBeginOfLine()
	bool isEmptyLine();
	#endif	//CSV_FILE_ENABLE_CHECK_EMPTY_LINE
	#if CSV_FILE_ENABLE_DELETING_LINE
	//Require gotoBeginOfLine();1
	bool isLineMarkedAsDelete();
	#endif //CSV_FILE_ENABLE_DELETING_LINE
	bool gotoBeginOfLine();
	
	// Return false when next line not exist. When return false
	// then positon in file is set at end.
	bool nextLine();
	bool gotoLine(unsigned int number);
	
	#if CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
	// Find first line starts with argument string
	// from CURRENT position.
	// Probably you should call gotoBeginOfFile().
	bool gotoLine(const char * startsWith_);
	#endif
	
	#if CSV_FILE_ENABLE_DELETING_LINE
	//Required call gotoBeginOfLine
	bool markLineAsDelete();
	#endif //CSV_FILE_ENABLE_DELETING_LINE
	
	//Use only on the end of file
	bool addLine();
	
	// *** Fields ***
	bool isEndOfField();
	
	// Always return TRUE
	bool gotoBeginOfField();
	bool gotoField(byte num);
	
	// Return number of reading bytes
	// Return 0 when end reading current field
	// Auto-safe. If reading bytes is less then @bufferSize
	// then after last reading bytes is put null char.
	byte readField(char * buffer_, byte bufferSize);
	
	//Buffer size should be equals or greater then decimal place
	//reading value.
	//Return FALSE if not read field or field is empty.
	//Number can have a sign.
	//Number can be preceded by whitespaces.
	//Converting to number is end when is read first non-number char
	//Warning! If reading buffer don't contain any number char
	//then function return 0!
	bool readField(int& value, char * buffer_, byte bufferSize);
	
	// If this field is end field in line (file)
	// NOT go to next line and return FALSE
	// When return true set pointer to position
	// first char of file (after delimiter last field).
	bool nextField();
	
	//I cannot resize field.
	//Required gotoBeginOfField() (pointer at begin of field)
	//Should be sure, that number of decimal place of value
	//is equal or less then field size. 
	//Set pointer at the end of field. Before deilimiter or end of line
	bool editField(byte value);
	
	//Call this only on the end of file
	//Add only delimiter, without content.
	bool addField();
	
	//Call this only on the end of file.
	bool addField(const char * content);
	
	//Method to add variable size number field.
	bool addField(unsigned int content);
	
	//Method for add fixed size number field.
	//Therefore support only bytes @fieldSize should be
	//equals or less then 3.
	bool addField(byte content, byte fieldSize);
	
	// *** Interact with other file ***
	// Copy current field to other file.
	// Pointer in target file should be set at end of file
	byte copyField(FatFile * target);
};
	
#endif //CSVFile_h





