#include <Adafruit_GPS.h>
#include "MM_GPS.h"

Adafruit_GPS GPS(&GPSSerial);


#ifdef DEBUGLIB
	GPSValues myGpsData_debug;
	GPSValues *p_myGpsData_debug;
#else
	GPSValues myGpsData;
	GPSValues *p_myGpsData;

	/*
	 * Wir können das GPS Modul nur etwar alle 2 Sekunden abfragen
	 * daher setzen wir ihn uns einmal und laufen in unserem Programm weiter.
	 * sobald wir das GPS Modul auslesen, bevor 2Sekunden vorbei sind, warten wir einfach die differenzZeit
	 */


#endif
#ifdef DEBUGLIB
	void setup()
	{
		p_myGpsData_debug = &myGpsData_debug;
		Serial.begin(115200);
		Serial.println("MM GPSLib test");
		initGPS();
	}
#endif

uint32_t timer = millis();

#ifdef DEBUGLIB
	void loop()
	{
		static int checkedGPS = 0;
		// if millis() or timer wraps around, we'll just reset it
		if (timer > millis())  timer = millis();

		readGPS(p_myGpsData_debug);

		if (millis() - timer > 250) {
			timer = millis();
			checkedGPS++;
		}

		//if(p_myGpsData_debug->status == MM_SUCCESS)
		if (checkedGPS >= 10)
		{
			checkedGPS = 0;
			printGPSData(p_myGpsData_debug);
		    clearGPSData(p_myGpsData_debug);
		}


	}
#endif


//void initGPS()
	void initGPS()
{
	Serial.println("GPS: initGPS()");

#ifdef DEBUGLIB
#else
	p_myGpsData = &myGpsData;
#endif




  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  GPSSerial.begin(9600);



  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
  timer = millis();
  Serial.println("GPS: initGPS() - End");
}


void readGPS(GPSValues* p_myGpsData)
{
	//Serial.println("GPS: readGPS()");
	(void)GPS.read();
	// if a sentence is received, we can check the checksum, parse it...
	if (GPS.newNMEAreceived())
	{
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

		if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
		{
			p_myGpsData->status = MM_GPS_ERROR; // we can fail to parse a sentence in which case we should just wait for another
		}
		else{
			p_myGpsData->status = MM_SUCCESS;
		}
	}
	else{
		p_myGpsData->status = MM_GPS_ERROR;
	}

	if(p_myGpsData->status == MM_SUCCESS)
	{
		if(p_myGpsData->status != MM_GPS_ERROR)
		{
			p_myGpsData->hour = GPS.hour;
			p_myGpsData->minute = GPS.minute;
			p_myGpsData->seconds = GPS.seconds;
			p_myGpsData->milliseconds = GPS.milliseconds;

			p_myGpsData->day = GPS.day;
			p_myGpsData->month = GPS.month;
			p_myGpsData->year = GPS.year;

			if (GPS.fix)
			{
				p_myGpsData->latitude = GPS.latitude;
				p_myGpsData->lat = GPS.lat;
				p_myGpsData->longitude = GPS.longitude;
				p_myGpsData->lon = GPS.lon;

				p_myGpsData->speed = GPS.speed;
				p_myGpsData->angle = GPS.angle;
				p_myGpsData->altitude = GPS.altitude;
				p_myGpsData->satellites = GPS.satellites;
			}
		}
	}
	//Serial.println("GPS: readGPS() - End");
}

void clearGPSData(GPSValues* p_myGpsData)
{
	p_myGpsData->hour = 0;
	p_myGpsData->minute = 0;
	p_myGpsData->seconds = 0;
	p_myGpsData->milliseconds = 0;

	p_myGpsData->day = 0;
	p_myGpsData->month = 0;
	p_myGpsData->year = 0;

	p_myGpsData->latitude = 0.0F;
	p_myGpsData->longitude = 0.0F;

	p_myGpsData->speed = 0.0F;
	p_myGpsData->angle = 0.0F;
	p_myGpsData->altitude = 0.0F;
	p_myGpsData->satellites = 0;

	p_myGpsData->status = MM_GPS_ERROR;
}

void printGPSData(GPSValues* p_myGpsData)
{
    Serial.print("\nTime: ");
    Serial.print(p_myGpsData->hour, DEC); Serial.print(':');
    Serial.print(p_myGpsData->minute, DEC); Serial.print(':');
    Serial.print(p_myGpsData->seconds, DEC); Serial.print('.');
    Serial.println(p_myGpsData->milliseconds);
    Serial.print("Date: ");
    Serial.print(p_myGpsData->day, DEC); Serial.print('/');
    Serial.print(p_myGpsData->month, DEC); Serial.print("/20");
    Serial.println(p_myGpsData->year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(p_myGpsData->latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(p_myGpsData->longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(p_myGpsData->speed);
      Serial.print("Angle: "); Serial.println(p_myGpsData->angle);
      Serial.print("Altitude: "); Serial.println(p_myGpsData->altitude);
      Serial.print("Satellites: "); Serial.println((int)p_myGpsData->satellites);


    }
}
