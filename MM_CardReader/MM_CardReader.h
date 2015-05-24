// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_CardReader_H_
#define _MM_CardReader_H_



#include "Arduino.h"
#include "MM_DEFINES.h"
#include <SPI.h>
#include <SD.h>

//add your includes for the project MM_CardReader here


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif

int SDInit(const char*, boolean);

#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project MM_CardReader here




//Do not add code below this line
#endif /* _MM_CardReader_H_ */
