#ifndef CSVFileConfig_h
#define CSVFileConfig_h

#define CSV_FILE_ENABLE_GOTO_BEGIN_STARTS_WITH 0
#define CSV_FILE_ENABLE_CHECK_EMPTY_LINE 	   0
#define CSV_FILE_ENABLE_DELETING_LINE 		   1
// If CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR is equals 1 adding function return false when
// current operation can overwrite other data
#define CSV_FILE_ENABLE_CHECK_OVERWRITE_ERROR  0

#define CSV_FILE_DELIMITER ';'

#define CSV_FILE_CSV_FILE_DELETE_MARKER_SIZE 4
#define CSV_FILE_DELETE_MARKER "@DEL"

#endif //CSVFileConfig_h