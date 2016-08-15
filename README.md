# Arduino-CSV-File
Extend class for SdFat library for easy work with CSV files.

## Main features
* Write CSV file  
    CSV File provide functions for add CSV fields and lines without worrying
    about delimiters or end of line character.
* Read CSV file  
    CSV File designed for work with big files and memory limits. It encapsulate
    parsing process and return clear data.
    
* Edit CSV file  
    CSV File can modify data in field without problems with override value in next field.
    In this version support only numberic byte fields (range values 0-255).
    
* Deleting content  
    Lines in CSV file can be marked as deleted. In a further processing these lines are ignored.
    It isn't really deleting but it fast solution.
    
* Navigation  
    CSV File can easy move across the fields and the lines without worrying about position in file.
    
* Support UTF-8 content  
    CSV File correct work with non-ASCII characters (characters consist of two parts).

## How to use
First you need download [SdFat](https://github.com/greiman/SdFat). 
It is only plug-in class for this library.
SdFat is fine, light and fast library for support SD cards.
It is better than standard Arduino SD library.

After install SdFat you should put CSVFile repository in subfolder in your Arduino path.
(On Windows probably: ~/Documents/Arduino/libraries/CSVFile)

It is all. In Arduino IDE you can see examples how use CSVFile.
(Menu File->Examples->CSVFile) to learn how use CSVFile.

Example usage:

```cpp
// SdFat library should be included before CSV File
#include <SdFat.h>
#include <CSVFile.h>

// Standard SdFat object for support SD card
SdFat sd;
// Instead of SdFile use CSV File
CSVFile csv;

void setup() {
    sd.begin(); // Initialize SD card
    // Important note!
    // You should use flag O_RDWR even if you use CSV File
    // only for writting.
    csv.open("file.csv", O_RDWR | O_CREAT);
    
    // See "HelloWorld.ino" for example more operations.
    csv.addField("Hello CSV!");
    
    //Don't forget close file
    csv.close();
}

void loop() { }
```

## CSV structure
CSV File assume that file consist of lines. Each line consist of fields.  
Each line without last is ended by end of line character in UNIX-style ('\n').  
Fields are separate by single character delimiter (default it is semicolon: ';').  
Lines can have different length.

Example:

> field1;field2;field3\n  
  field4;field5\n  
  field6;field7;field8;field9

## Known issues
* CSV File support only UNIX-style end of line (\n).  
    Arduino and SdFat use Windows-style end of line (\r\n). You should not use standard SdFat functions
    for write end of line character (e.g. println).
* CSV File don't override any SdFat function.  
    You should be carreful when you use standard functions
    manipulate position in file (e.g. seekSet, seekCur seekEnd, read, write, print), because they
    can corrupt line numbering and some functions (e.g. gotoLine, gotoField, getNumberOfLine) will not work properly.
* Don't support standard Arduino SD library
* Delete line feature don't work corectly with short line (default length less then 4).
* CSV File has limitation for reading buffer to byte (255) size.
* CSV File cannot convert numberic field when reading buffer size is less then number of digits reading value
* CSV File can read only byte numberic field
* CSV File can edit only numberic field
* CSV File don't support add fields with content from flash memory (const character arrays created by using F() macro).
* Cannot define configuration of CSV File from outside library
* Missing method for check begin of field

## Author
Slawomir "Fivitti" Figiel

## License
CSVFile is under MIT license.