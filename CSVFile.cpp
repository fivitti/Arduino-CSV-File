/**
 *  Project: CSV File - extend class for SdFat library
 *  @author: Slawomir Figiel
 *  @contact: fivitti@gmail.com
 *  @date: 18.07.2016
 *  @version: 1.0.1
 *  @license: MIT
 */

#include "CSVFile.h"

// We use UNIX-style end of line
#define CSV_END_OF_LINE '\n'
#define NULL_CHAR '\0'

#if CSV_FILE_ENABLE_DELETING_LINE || CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
// *** Utilities ***
bool CSVFile::isCurrentSubstring(const char * substr) 
{
	while(*substr)
	{
		int chVal = read();
		if (*substr++ != chVal)
		{
			return false;
		}
	}
	return true;
}
#endif //CSV_FILE_ENABLE_DELETING_LINE || CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH

#define MAXIMAL_DIGITS_IN_UNSIGNED_INT 5 //Max:65 535
#define ANSII_ZERO 0x30
// Write number to file as text. Return false if failed.
int CSVFile::writeNumber(unsigned int number)
{
	unsigned int power10 = pow(10, MAXIMAL_DIGITS_IN_UNSIGNED_INT-1); //10000
	byte digits = 0;
	byte currentDigit = 0;
	// Without last from right digit
	while (power10 != 1)
	{
		currentDigit = number / power10;
		number = number % power10;
		power10 /= 10;
		
		if (digits == 0 && currentDigit == 0)
			continue;
		
		digits += 1;
		write(ANSII_ZERO + currentDigit);
	}
	
	// Last digit
	digits += 1;
	return write(ANSII_ZERO + number) == -1 ? -1 : digits;
}
#undef MAXIMAL_DIGITS_IN_UNSIGNED_INT
#undef ANSII_ZERO

// *** Files ***
bool CSVFile::isEndOfFile() 
{
	return available() == 0;
}

// Always return true.
// If first lines are delete then set
// pointer on the begin of first not deleted
// line or if all lines are deleted on the
// end of file.
bool CSVFile::gotoBeginOfFile() 
{
	rewind();
	numField = 0;
	
	#if CSV_FILE_ENABLE_DELETING_LINE
	//Search for first not deleted line
	if (isLineMarkedAsDelete())
		nextLine();
	else
		gotoBeginOfLine();
	#endif //CSV_FILE_ENABLE_DELETING_LINE
	
	numLine = 0;
	return true;
}

// *** Lines ***
bool CSVFile::isBeginOfLine()
{
	//Begin of file;
	if (!seekCur(-1))
	{
		return true;
	}
	int ch = read();
	return ch == CSV_END_OF_LINE;
}

bool CSVFile::isEndOfLine() 
{
	//End of file
	if (isEndOfFile())
	{
		return true;
	}
	bool status_ = read() == CSV_END_OF_LINE;
	seekCur(-1);
	return status_;
}

//Return number current line. Lines are numbered from 0.
unsigned int CSVFile::getNumberOfLine()
{
	return numLine;
}

#if CSV_FILE_ENABLE_CHECK_EMPTY_LINE
//After set pointer at end of line or at first not empty character
//Require gotoBeginOfLine()
bool CSVFile::isEmptyLine()
{
	int chVal = read();
	while (chVal >= 0)
	{
		if (((char) chVal) == CSV_END_OF_LINE)
		{
			seekCur(-1);
			return true;
		}
		else if (((char)chVal) != NULL_CHAR)
		{
			return false;
		}
		chVal = read();
	}
	return true; //End of line
}
#endif

#if CSV_FILE_ENABLE_DELETING_LINE
//Require gotoBeginOfLine();
//After process DON'T set pointer at begin of line
bool CSVFile::isLineMarkedAsDelete() 
{
	return isCurrentSubstring(CSV_FILE_DELETE_MARKER);
}
#endif //CSV_FILE_ENABLE_DELETING_LINE

bool CSVFile::gotoBeginOfLine() 
{
	numField = 0;
	//Begin of file;
	if (!seekCur(-1))
	{
		return true;
	}

	int chVal = read();

	while (chVal != CSV_END_OF_LINE)
	{
		if (!seekCur(-2))
		{
			seekCur(-1);
			return true;
		}
		chVal = read();
	}

	return true;
}

bool CSVFile::nextLine() 
{
	int chVal = read();
	//End of file
	if (chVal < 0)
	{
		return false;
	}
	//Content of current line
	while (chVal != CSV_END_OF_LINE)
	{
		chVal = read();
		//End of file
		if (chVal < 0)
		{
			return false;
		}
	}
	//We are in next line
	#if CSV_FILE_ENABLE_DELETING_LINE
	//Ignored Deleted lines
	if (isLineMarkedAsDelete())
	{
		return nextLine();
	}
	else
	{
		gotoBeginOfLine();
	}
	#else
	numField = 0;
	#endif //CSV_FILE_ENABLE_DELETING_LINE

	numLine += 1;
	return true;
}

bool CSVFile::gotoLine(unsigned int number) 
{
	if (number == numLine)
	{
		return gotoBeginOfLine();
	}
	else if (number < numLine)
	{
		gotoBeginOfFile();
	}

	while (number != numLine)
	{
		if (!nextLine())
		{
			return false;
		}
	}
	return true;
}

#if CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH
// Find first line starts with argument string
// from CURRENT position.
// Probably you should call gotoBeginOfFile().
bool CSVFile::gotoLine(const char * startsWith_) 
{
	while (!isCurrentSubstring(startsWith_))
	{
		if (!nextLine())
		{
			return false;
		}
	}
	gotoBeginOfLine();
	return true;
}
#endif

#if CSV_FILE_ENABLE_DELETING_LINE
//Required call gotoBeginOfLine
//Don't set pointer at begin of line.
//If you call isLineMarkedAsDelete after
//this function you get false.
//Line should be long then CSV_FILE_DELETE_MARKER
bool CSVFile::markLineAsDelete() 
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	//Check line size
	int chVal;
	for (byte i = 0; i < CSV_FILE_CSV_FILE_DELETE_MARKER_SIZE; ++i)
	{
		chVal = read();
		if (chVal < 0 || ((char) chVal) == CSV_END_OF_LINE)
		{
			seekCur(-(i+1));
			return false;
		}
	}
	
	seekCur(-CSV_FILE_CSV_FILE_DELETE_MARKER_SIZE);
	#endif //CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	
	return write(CSV_FILE_DELETE_MARKER) != 0;
}
#endif //CSV_FILE_ENABLE_DELETING_LINE

//Use only on the end of file
bool CSVFile::addLine() 
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	numLine += 1;
	numField = 0;
	write(CSV_END_OF_LINE);  // Unix style end of line
	return true;
}

// *** Fields ***
bool CSVFile::isEndOfField() 
{
	//End of line
	if (isEndOfLine())
	{
		return true;
	}
	else
	{
		bool status_ = read() == CSV_FILE_DELIMITER;
		seekCur(-1);
		return status_;
	}
}

// Always return TRUE
bool CSVFile::gotoBeginOfField() 
{
	//Begin of file;
	if (! seekCur(-1))
	{
		return true;
	}

	int chVal = read();

	while (chVal != CSV_END_OF_LINE && chVal != CSV_FILE_DELIMITER)
	{
		//Begin of file
		if (!seekCur(-2))
		{
			seekCur(-1);
			return true;
		}
		chVal = read();
	}

	return true;
}

bool CSVFile::gotoField(byte num) {
	if (num == numField)
	{
		gotoBeginOfField();
		return true;
	}
	else if (num < numField)
	{
		gotoBeginOfLine();
	}
	
	while (num != numField)
	{
		if (!nextField())
		{
			return false;
		}
	}
	return true;
}

// Return number of reading bytes
// Return 0 when end reading current field
// Auto-safe. If reading bytes is less then @bufferSize
// then after last reading bytes is put null char.
byte CSVFile::readField(char * buffer_, byte bufferSize) 
{
	byte numReading = read(buffer_, bufferSize);
	char chVal = 0;
	byte correctBytes = 0;
	
	for (correctBytes=0; correctBytes < numReading; ++correctBytes)
	{
		chVal = buffer_[correctBytes];
		if (chVal == CSV_END_OF_LINE || chVal == CSV_FILE_DELIMITER)
		{
			// If read over end of field or end of line then we should
			// seek before first delimiter or first end of line
			// -numReading is state before reading
			//  correctBytes is number of bytes to first delimiter or first end of line
			//    (without this char)
			// Sum -numReading and correctBytes is position before delimiter
			seekCur(-numReading + correctBytes);
			break;
		}
	}
	
	// If read file to end or if not read full single field
	// then position in file is correctBytes.
	
	// Special case: last reading bytes is first half of non-ASCII character
	if (correctBytes == numReading && chVal < 0 && buffer_[correctBytes - 2] > 0) //correctBytes - 1 is index of chVal
	{
		correctBytes -= 1;
		seekCur(-1);
	}
	
	if (correctBytes < bufferSize)
		buffer_[correctBytes] = NULL_CHAR;
	
	return correctBytes;
}

//Buffer size should be equals or greater then decimal place
//reading value.
//Return FALSE if not read field or field is empty.
//Number can have a sign.
//Number can be preceded by whitespaces.
//Converting to number is end when is read first non-number char
//Warning! If reading buffer don't contain any number char
//then function return 0!
bool CSVFile::readField(int& value, char * buffer_, byte bufferSize) 
{
	int reading = readField(buffer_, bufferSize);
	if (reading > 0)
	{
		value = atoi(buffer_);
		return true;
	}
	else
	{
		return false;
	}
}

// If this field is end field in line (file)
// NOT go to next line and return FALSE
// When return true set pointer to position
// first char of file (after delimiter last field).
bool CSVFile::nextField() 
{
	int chVal = read();
	//End of file
	if (chVal == -1)
	{
		return false;
	}
	//Any content of field
	//Not end of file, not end of field, not end of line
	while (chVal >= 0 && chVal != CSV_FILE_DELIMITER && chVal != CSV_END_OF_LINE)
	{
		chVal = read();
	}
	//End of line
	if (chVal == CSV_END_OF_LINE)
	{
		seekCur(-1);
		return false;
	}
	
	numField += 1;
	
	return true;
}

//I cannot resize field.
//Required gotoBeginOfField() (pointer at begin of field)
//Should be sure, that number of decimal place of value
//is equal or less then field size.
//Set pointer at the end of field. Before deilimiter or end of line
bool CSVFile::editField(byte value) 
{
	if (writeNumber(value) == -1)
		return false;

	int ch = read();
	//End of file, end of line, end of field
	while(ch >= 0 && ch != CSV_END_OF_LINE && ch != CSV_FILE_DELIMITER)
	{
		seekCur(-1);
		write((byte)NULL_CHAR);
		ch = read();
	}

	if (ch >= 0)
	{
		seekCur(-1);
	}

	return true;
}

//Add only delimiter, without content.
bool CSVFile::addField()
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	// On the begin of line this add empty field too
	write(CSV_FILE_DELIMITER);
	
	numField += 1;
	
	return true;
}

//Call this only on the end of file.
bool CSVFile::addField(const char * content) 
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		write(CSV_FILE_DELIMITER);
	}

	if (write(content) == -1)
		return false;

	numField += 1;
	
	return true;
}

//Method to add variable size number field.
bool CSVFile::addField(unsigned int content)
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		write(CSV_FILE_DELIMITER);
	}

	if (writeNumber(content) == -1)
		return false;

	numField += 1;
	
	return true;
}

//Method for add fixed size number field.
//Therefore support only bytes @fieldSize should be
//equals or less then 3.
bool CSVFile::addField(byte content, byte fieldSize) 
{
	#if CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		write(CSV_FILE_DELIMITER);
	}

	fieldSize -= writeNumber(content);
  
	while (fieldSize > 0)
	{
		fieldSize -= write((byte)NULL_CHAR);
	}

	numField += 1;
	
	return true;
}

// *** Interact with file ***
// Copy current field to other file.
// Pointer in target file should be set at end of file
byte CSVFile::copyField(FatFile * target)
{
	byte copied = 0;
	int chVal = read();
	while (chVal >= 0 && ((char)chVal != CSV_FILE_DELIMITER) && ((char)chVal != CSV_END_OF_LINE))
	{
		target->write((char)chVal);
		copied += 1;
		chVal = read();
	}
	
	if (chVal >= 0) // ((char)chVal) == CSV_END_OF_LINE || ((char) chVal) == CSV_FILE_DELIMITER)
	{
		seekCur(-1);
	}
	
	return copied;
}

#undef CSV_FILE_DELIMITER
#undef CSV_END_OF_LINE
#undef CSV_FILE_DELETE_MARKER
#undef CSV_FILE_CSV_FILE_DELETE_MARKER_SIZE
#undef NULL_CHAR
#undef CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR
