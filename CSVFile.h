/**
 *  CSV File - extend class for SdFat library
 *
 *  @author: Slawomir "Fivitti" Figiel
 *  @date: 18.07.2016
 *  @version: 1.0
 *  @license: MIT
 *
**/

#ifndef CSVFile_h
#define CSVFile_h

#include <SdFat.h>

#define CSV_ENABLE_GOTO_BEGIN_STARTS_WITH 0
#define CSV_ENABLE_CHECK_EMPTY_LINE 0

class CSVFile : public SdFile {
	
	private:
	unsigned int numLine = 0;
	unsigned int numField = 0;
	
	// *** Utilities ***
	//UNSAFE FUNCTION!
	//Change pointer position
	//Don't stop when end of field/line.
	//May destroy correct line/field counting
	bool isCurrentSubstring(const char * substr);
	
	public:
	CSVFile() : SdFile() {}
	virtual ~CSVFile() {}
	
	// *** Utilities ***
	void clearToEnd();
	
	// *** Files ***
	bool isEndOfFile();
	bool gotoBeginOfFile();
	
	// *** Lines ***
	bool isBeginOfLine();
	bool isEndOfLine();
	
	//Return number current line. Lines are numbered from 0.
	unsigned int getNumberOfLine();
	
	#if CSV_ENABLE_CHECK_EMPTY_LINE
	//After set pointer at end of line or at first not empty character
	//Require gotoBeginOfLine()
	bool isEmptyLine();
	#endif
	//Require gotoBeginOfLine();
	bool isLineMarkedAsRemove();
	bool gotoBeginOfLine();
	bool nextLine();
	bool gotoLine(int number);
	#if CSV_ENABLE_GOTO_BEGIN_STARTS_WITH
	bool gotoLine(const char * startsWith_);
	#endif
	
	//Required call gotoBeginOfLine
	bool markLineAsRemove();
	
	//Use only on the end of file
	bool addLine();
	
	// *** Fields ***
	bool isEndOfField();
	
	// Always return TRUE
	bool gotoBeginOfField();
	bool gotoField(byte num);
	
	// Return number of reading bytes
	// Return 0 when end reading current field
	byte readField(char * buffer_, byte bufferSize);
	
	//Buffer size should be equals or greater then decimal place
	//reading value.
	//Return FALSE if not read field or field is empty.
	//Number can have a sign.
	//Number can be preceded by whitespaces.
	//Converting to number is end when is read first non-number char
	//Warning! If reading buffer don't contain any number char
	//then function return 0!
	//
	//If safeBuffer is true real buffer size is least at one greater then bufferSize value.
	//In array in position bufferSize will be set null char (\0)
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
	//Always return true.
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
	bool addField(byte content, byte fieldSize);
	
	// *** Interact with other file ***
	// Copy current field to other file.
	// Pointer in target file should be set at end of file
	byte copyField(SdFile * target);
};
	
#endif //CSVFile_h





