// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_DEFINES_H_
#define _MM_DEFINES_H_
#include "Arduino.h"

enum MM_ERRORS{MM_SUCCESS, MM_UNDEFINED_ERROR, MM_TFT_TEXT_TO_LONG, MM_TFT_NO_DISPLAY, MM_SD_NO_CARDREADER, MM_WRONG_SD_FORMAT, MM_NO_FILE_POINTER, MM_BMP085_INIT_ERROR};



void SerialPrintln(String , boolean);






#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"
#endif






//Do not add code below this line
#endif /* _MM_DEFINES_H_ */
