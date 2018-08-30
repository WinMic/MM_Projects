#include <Adafruit_GPS.h>
#include "MM_GPS.h"

Adafruit_GPS GPS(&mySerial);


//Keine Interrupts
boolean usingInterrupt = false;


	GPSValues myGpsData;
	GPSValues *p_myGpsData;

#ifdef DEBUGLIB
	void setup()
	{
		Serial.begin(115200);
		Serial.println("MM GPSLib test");
		initGPS();
	}
#endif

#ifdef DEBUGLIB
	void loop()
	{


		readGPS(p_myGpsData);


		Serial.print(p_myGpsData->hour); Serial.print("h ");
		Serial.print(p_myGpsData->minute); Serial.print("m ");
		Serial.print(p_myGpsData->seconds); Serial.print("s ");
		Serial.print(p_myGpsData->milliseconds); Serial.println("ms ");
		Serial.print(p_myGpsData->day); Serial.print(".");
		Serial.print(p_myGpsData->month); Serial.print(".");
		Serial.print(p_myGpsData->year);

		Serial.print("fix: "); Serial.print(p_myGpsData->fix);
		Serial.print(" qualety: "); Serial.println(p_myGpsData->fixquality);

		Serial.print("lat: ");  Serial.println(p_myGpsData->latitude);
		Serial.print("lon: "); Serial.println(p_myGpsData->longitude);
		Serial.print("Speed: "); Serial.println(p_myGpsData->speed);
		Serial.print("angle: "); Serial.println(p_myGpsData->angle);
		Serial.print("höhe: "); Serial.println(p_myGpsData->altitude);
		Serial.print("satellites: "); Serial.println(p_myGpsData->satellites);
		Serial.println("");
		Sleep(2000);
	}
#endif


//void initGPS()
	void initGPS()
{
	Serial.begin(115200);
	Serial.println("setup");


  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  mySerial.begin(9600);



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
  mySerial.println(PMTK_Q_RELEASE);
}

/*
 * Wir können das GPS Modul nur etwar alle 2 Sekunden abfragen
 * daher setzen wir ihn uns einmal und laufen in unserem Programm weiter.
 * sobald wir das GPS Modul auslesen, bevor 2Sekunden vorbei sind, warten wir einfach die differenzZeit
 */
uint32_t timer = millis();

void readGPS(GPSValues* p_myGpsData)
{
	Serial.println("loop");
	GPS.read();

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
			GPS.read();
		}
	}

//  if millis() or timer wraps around, we'll just reset it
	if (timer > millis())  timer = millis();

	if(p_myGpsData->status != MM_GPS_ERROR)
	{
		Serial.print("dies sind wie werte direkt aus dem GPS Objekt ");
	    Serial.print("\nTime: ");
	    Serial.print(GPS.hour, DEC); Serial.print(':');
	    Serial.print(GPS.minute, DEC); Serial.print(':');
	    Serial.print(GPS.seconds, DEC); Serial.print('.');
	    Serial.println(GPS.milliseconds);
	    Serial.print("Date: ");
	    Serial.print(GPS.day, DEC); Serial.print('/');
	    Serial.print(GPS.month, DEC); Serial.print("/20");
	    Serial.println(GPS.year, DEC);
	    Serial.print("Fix: "); Serial.print((int)GPS.fix);
	    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
		Serial.print("Location: ");
		Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
		Serial.print(", ");
		Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

		Serial.print("Speed (knots): "); Serial.println(GPS.speed);
		Serial.print("Angle: "); Serial.println(GPS.angle);
		Serial.print("Altitude: "); Serial.println(GPS.altitude);
		Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
		Serial.println(" ");
		/*
		 * Wenn das Programm auf den Arduino an dieser Stelle wieder ankommt, und dafür weniger als 2Sec gebraucht hat
		 * Warten wir die Zeit, bis 2 Sec verstrichen sind.
		 * braucht das Programm länger, ist alles gut.
		 * Diese funktion muss in der Lib des GPS sein, da man sich um die Timings nicht jedes mal neu gedanken machen will.
		 */
		//TODO: Die sache mit dem Timer ist noch unsauber
//		uint32_t timeWaited = 0;
//		timeWaited = millis() - timer;
//		Serial.print("TimeWaited: "); Serial.println(timeWaited);
//		if (timeWaited < 2100)
//		{
//			Sleep(2100-timeWaited);
//		}
		if (millis() - timer > 2000)
		{
			Serial.println("sortiere ein");
			timer = millis(); // reset the timer

			p_myGpsData->hour = (int)GPS.hour;
			p_myGpsData->minute = (int)GPS.minute;
			p_myGpsData->seconds = (int)GPS.seconds;
			p_myGpsData->milliseconds = (int)GPS.milliseconds;
			p_myGpsData->day = (int)GPS.day;
			p_myGpsData->month = (int)GPS.month;
			p_myGpsData->year = (int)GPS.year;
			p_myGpsData->fix = GPS.fix;
			p_myGpsData->fixquality = (int)GPS.fixquality;

			if (GPS.fix)
			{
				Serial.println("Jau also status ist MM_Success...");
				p_myGpsData->status = MM_SUCCESS;
				p_myGpsData->latitude = GPS.latitude;
				p_myGpsData->longitude = GPS.longitude;

				//Von Knoten auf km/h
				p_myGpsData->speed = GPS.speed*1.852;
				p_myGpsData->angle = GPS.angle;
				p_myGpsData->altitude = GPS.altitude;
				p_myGpsData->satellites = (int)GPS.satellites;
			}
		}
	}
}
