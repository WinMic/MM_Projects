#include "MM_HMC5883L.h"

Adafruit_HMC5883_Unified magnetometer = Adafruit_HMC5883_Unified(1);

int HMC5883lInit()
{
	short retVal = MM_UNDEFINED_ERROR;

	//Prüfe ob der Kompass ausgelesen werden kann.
	if(!magnetometer.begin())
	{
		Serial.println("Kompass konnte nicht gefunden werden.");
		retVal = MM_HMC_INIT_ERROR;
	}
	else
	{
		retVal = MM_SUCCESS;
	}

	return retVal;
}

magnetometerValues HMC5883lHeading()
{
/* Dieser Fehler des magnetfeldes ist Ortsabhängig und kann unter http://www.magnetic-declination.com/ ermitelt werden.
 * (Magnetic declination: +2° 53' ~ umrechnen in radiant!
 * declinationAngle = 0.05235987755983 ~ Ortsfehler für München*/
	float declinationAngle = 0.05235987755983;

	//Rückgabewert, der dann auf die SD geschrieben wird.
	struct magnetometerValues retVal;

	//Lese den sensor aus
	sensors_event_t event;
	magnetometer.getEvent(&event);

	//Die raw-Werte speichern
	retVal.magneticX = event.magnetic.x;
	retVal.magneticY = event.magnetic.y;
	retVal.magneticZ = event.magnetic.z;

	// Das Modul liegt in der X&Y Ebene, wobei Z nach oben zeigt
	float heading = atan2(event.magnetic.y, event.magnetic.x);

	//Den declinationAngle/Fehler mit einberechnen
	heading += declinationAngle;

	// Vorzeichenkontrolle Winkel[0, 360]°
	if(heading < 0)
		heading += 2*PI;

	// Alle 2Pi Sprünge kompensieren
	if(heading > 2*PI)
		heading -= 2*PI;

	// umwandeln in Grad
	float headingDegrees = heading * 180/M_PI;

	retVal.heading = headingDegrees;

	//Den declinationAngle ebenfalls zurückgeben, um ggf. nachträglich den Ortsfehler zu korrigieren
	retVal.declinationAngle = declinationAngle;

	return retVal;
}

//Test Main
//void setup()
//{
//	Serial.begin(115200);
//	int initstatus = HMC5883lInit();
//	if (initstatus == MM_SUCCESS)
//	{
//		Serial.println("Kompass Initialisiert");
//	} else
//	{
//		Serial.println("Kompass Fehlerhaft");
//	}
//}
//
//void loop()
//{
//	struct magnetometerValues result;
//	result = HMC5883lHeading();
//	Serial.print("Kompass headding:");
//	Serial.println(result.heading);
//	Sleep(1000);
//}
