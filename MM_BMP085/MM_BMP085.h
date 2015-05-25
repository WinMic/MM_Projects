// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_BMP085_H_
#define _MM_BMP085_H_
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include "MM_DEFINES.h"



#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"
#endif

int BMPInit(boolean);
void BMPData(float*, int32_t*, float*);


#endif /* _MM_BMP085_H_ */
