// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _GPS_Tracker_H_
#define _GPS_Tracker_H_
#include "Arduino.h"
#include <Wire.h>

#include "MM_CardReader.h"
#include "MM_DEFINES.h"
#include "MM_GPS.h"
#include "MM_HMC5883L.h"





//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project GPS_Tracker here
#define WRITE_EVERY_MILLIS_TO_FILE 2500

#define ERROR_LED 7



//Do not add code below this line
#endif /* _GPS_Tracker_H_ */
