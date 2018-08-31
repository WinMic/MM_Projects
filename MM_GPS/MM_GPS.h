// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_GPS_H_
#define _MM_GPS_H_
#include "Arduino.h"
#include <Adafruit_GPS.h>
#include "MM_DEFINES.h"

//   GPS TX to Arduino Due Serial1 RX pin 19
//   GPS RX to Arduino Due Serial1 TX pin 18
#define mySerial Serial1

#define DEBUGLIB

//GPS ECHO (RAW-Daten auf die Console schreiben)
#define GPSECHO  false

typedef struct GPSValues
{
	boolean status = MM_UNDEFINED_ERROR;
	int hour, minute, seconds, year, month, day, milliseconds, fixquality, satellites;
	// Floating point latitude and longitude value in degrees.
	float latitude, longitude, altitude, speed, angle;
	boolean fix;
};


void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

#ifdef DEBUGLIB
	void setup();
	void loop();
#endif

void initGPS();
void readGPS(GPSValues*);


char *dtostrf (double, signed char, unsigned char, char*);



//Do not add code below this line
#endif /* _MM_GPS_H_ */
