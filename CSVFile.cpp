#include "CSVFile.h"

#define CSV_DELIMITER ';'
// We use UNIX-style end of line
#define CSV_END_OF_LINE '\n'
#define DELETE_MARKER "@DEL"
#define NULL_CHAR '\0'
// If CSV_CHECK_ADDING_ERROR is equals 1 adding function return false when
// current position isn't on the end of file
#define CSV_CHECK_ADDING_ERROR 0

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

void CSVFile::clearToEnd()
{
	while(available() != 0)
		print(NULL_CHAR);
}

// *** Files ***
bool CSVFile::isEndOfFile() 
{
	return available() == 0;
}

bool CSVFile::gotoBeginOfFile() 
{
	numLine = 0;
	numField = 0;
	return seekSet(0);
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

#if CSV_ENABLE_CHECK_EMPTY_LINE
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

//Require gotoBeginOfLine();
//After process DON'T set pointer at begin of line
bool CSVFile::isLineMarkedAsRemove() 
{
	return isCurrentSubstring(DELETE_MARKER);
}

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

// When false set pointer to begin of current line
bool CSVFile::nextLine() 
{
	int chVal = read();
	//End of file
	if (chVal < 0)
	{
		gotoBeginOfLine();
		return false;
	}
	//Content of current line
	while (chVal != CSV_END_OF_LINE)
	{
		chVal = read();
		//End of file
		if (chVal < 0)
		{
			gotoBeginOfLine();
			return false;
		}
	}
	//We are in next line
	//Ignored removed lines
	if (isLineMarkedAsRemove())
	{
		return nextLine();
	}
	else
	{
		gotoBeginOfLine();
	}
	//numField = 0;
	numLine += 1;
	return true;
}

bool CSVFile::gotoLine(int number) 
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

#if CSV_ENABLE_GOTO_BEGIN_STARTS_WITH
bool CSVFile::gotoLine(const char * startsWith_) 
{
	gotoBeginOfFile();

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

//Required call gotoBeginOfLine
//Don't set pointer at begin of line.
//If you call isLineMarkedAsRemove after
//this function you get false.
//Line should be long then DELETE_MARKER
bool CSVFile::markLineAsRemove() 
{
	return print(DELETE_MARKER) != 0;
}

//Use only on the end of file
bool CSVFile::addLine() 
{
	#if CSV_CHECK_ADDING_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	numLine += 1;
	numField = 0;
	print(CSV_END_OF_LINE);  // Unix style end of line
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
		bool status_ = read() == CSV_DELIMITER;
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

	while (chVal != CSV_END_OF_LINE && chVal != CSV_DELIMITER)
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
byte CSVFile::readField(char * buffer_, byte bufferSize) 
{
	byte numReading = 0;
	int chVal = 0;

	while (numReading < bufferSize) //if not buffer end
	{
		chVal = read();
		//End of file or end of field
		if (chVal < 0)
		{
			//numField += 1;
			return numReading;
		}
		//End of line
		else if (chVal == CSV_END_OF_LINE || chVal == CSV_DELIMITER)
		{
			seekCur(-1);
			return numReading;
		}
		//Reading first part char from extension ASCII
		else if (((char) chVal) < 0)
		{
			//and it is last position on buffer
			if (bufferSize - numReading == 1)
			{
				seekCur(-1);
				return numReading;
			}
			//and have two place
			else
			{
				buffer_[numReading] = (char) chVal;
				numReading += 1;
				chVal = read();
				//Second bit is save below
			}
		}

		buffer_[numReading] = (char) chVal;
		numReading += 1;
	}
	
	return numReading;
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
		if (reading < bufferSize)
			buffer_[reading] = '\0';
		
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
	while (chVal >= 0 && chVal != CSV_DELIMITER && chVal != CSV_END_OF_LINE)
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
//Always return true.
//Set pointer at the end of field. Before deilimiter or end of line
bool CSVFile::editField(byte value) 
{
	print(value);

	int ch = read();
	//End of file, end of line, end of field
	while(ch >= 0 && ch != CSV_END_OF_LINE && ch != CSV_DELIMITER)
	{
		seekCur(-1);
		print(NULL_CHAR);
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
	#if CSV_CHECK_ADDING_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		print(CSV_DELIMITER);
	}
	
	numField += 1;
	
	return true;
}

//Call this only on the end of file.
bool CSVFile::addField(const char * content) 
{
	#if CSV_CHECK_ADDING_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		print(CSV_DELIMITER);
	}

	print(content);

	numField += 1;
	
	return true;
}

//Method to add variable size number field.
bool CSVFile::addField(unsigned int content)
{
	#if CSV_CHECK_ADDING_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		print(CSV_DELIMITER);
	}

	print(content);

	numField += 1;
	
	return true;
}

//Method for add fixed size number field.
bool CSVFile::addField(byte content, byte fieldSize) 
{
	#if CSV_CHECK_ADDING_ERROR
	if (!isEndOfFile())
	{
		return false;
	}
	#endif
	if (!isBeginOfLine())
	{
		print(CSV_DELIMITER);
	}

	fieldSize -= print(content);

	while (fieldSize > 0)
	{
		fieldSize -= print(NULL_CHAR);
	}

	numField += 1;
	
	return true;
}

// *** Interact with file ***
// Copy current field to other file.
// Pointer in target file should be set at end of file
byte CSVFile::copyField(SdFile * target)
{
	byte copied = 0;
	int chVal = read();
	while (chVal >= 0 && ((char)chVal != CSV_DELIMITER) && ((char)chVal != CSV_END_OF_LINE))
	{
		target->print((char)chVal);
		copied += 1;
		chVal = read();
	}
	
	if (((char)chVal) == CSV_END_OF_LINE || ((char) chVal) == CSV_DELIMITER)
	{
		seekCur(-1);
	}
	
	return copied;
}

#undef CSV_DELIMITER
#undef CSV_END_OF_LINE
#undef DELETE_MARKER
#undef NULL_CHAR
#undef CSV_CHECK_ADDING_ERROR
