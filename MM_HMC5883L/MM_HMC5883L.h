#ifndef _MM_HMC5883L_H_
#define _MM_HMC5883L_H_
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <MM_DEFINES.h>


struct magnetometerValues
{
	/*
	 * Die Richtung kann auf dem Arduino berechnet werden, jedoch nur für einen declinationAngle.
	 * Dieser Fehler des magnetfeldes ist Ortsabhängig und kann unter http://www.magnetic-declination.com/ ermitelt werden.
	 * (Magnetic declination: +2° 53' ~ umrechnen in radiant!
	 * Für eine spätere auswertung/fehlerbegrenzung werden deshalb die Rohwerte der einzelnen Magnetometer gespeichert.
	 */
	float magneticX = 0.0;
	float magneticY = 0.0;
	float magneticZ = 0.0;
	float heading = 0.0;
	float declinationAngle = 0.0;
};

int HMC5883lInit();
magnetometerValues HMC5883lHeading();

#endif /* _MM_HMC5883L_H_ */
