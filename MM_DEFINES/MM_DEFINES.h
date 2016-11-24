// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_DEFINES_H_
#define _MM_DEFINES_H_
#include "Arduino.h"

enum MM_ERRORS{MM_SUCCESS, MM_UNDEFINED_ERROR,
/*Display*/	   MM_TFT_TEXT_TO_LONG, MM_TFT_NO_DISPLAY,
/*CardReader*/ MM_SD_NO_CARDREADER, MM_SD_WRONG_FORMAT, MM_SD_NO_FILE_POINTER,
/*Barometer*/  MM_BMP085_INIT_ERROR,
/*MPU*/		   MM_MPU_INIT_ERROR,
/*Kompass*/	   MM_HMC_INIT_ERROR,
/*GPS*/		   MM_GPS_ERROR};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"
#endif






//Do not add code below this line
#endif /* _MM_DEFINES_H_ */
