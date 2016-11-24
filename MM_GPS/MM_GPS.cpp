// Do not remove the include below
#include "MM_GPS.h"


Adafruit_GPS GPS(&mySerial);


//GPS wird ohne Interrupts verwendet
boolean usingInterrupt = false;

#ifdef DEBUGLIB
	void setup()
#else
   void initGPS()
#endif
{

#ifdef DEBUGLIB
		Serial.begin(115200);
		Serial.println("Setup");
#endif

	  //Die TX und RX verbindung zwischen Arduino und GPS mit BAUD 9600
	  GPS.begin(9600);
	  mySerial.begin(9600);

	  //Wir wollen RMC (recommended minimum) und GGA (fix data) inklusive der Höhe wissen
	  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

	  /*
	   * UpdateRaten:
	   * PMTK_SET_NMEA_UPDATE_1HZ	~1Hz
	   * PMTK_SET_NMEA_UPDATE_5HZ	~5Hz
	   * PMTK_SET_NMEA_UPDATE_10HZ	~10Hz
	   */
	  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);

	  //Antennenstatus bei abfrage anfordern
	  GPS.sendCommand(PGCMD_ANTENNA);

	  //kurz warten bis das GPS-Modul alles verdaut hat
	  delay(1000);

	  //FirmwareVersion abfragen
	  mySerial.println(PMTK_Q_RELEASE);
}


uint32_t timer = millis();

#ifdef DEBUGLIB
	void loop()
#else
	GPSValues readGPS()
#endif
{
#ifdef DEBUGLIB
	Sleep(2000);
	Serial.println("loop");
#endif


	GPSValues myGPSData;
	char c = GPS.read();

	// if a sentence is received, we can check the checksum, parse it...
	  if (GPS.newNMEAreceived())
	  {
	    // a tricky thing here is if we print the NMEA sentence, or data
	    // we end up not listening and catching other sentences!
	    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
	    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

	    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
	    {
	    	myGPSData.status = MM_GPS_ERROS;  // we can fail to parse a sentence in which case we should just wait for another
	    }

	  }

	  if (myGPSData.status != MM_GPS_ERROS)
	  {
		  	  if (timer > millis())  timer = millis(); //TODO: Timer löschen

//		  	   approximately every 2 seconds or so, print out the current stats
		  	  if (millis() - timer > 2000) //TODO: Timer löschen
		  	  {
			  	    timer = millis(); // reset the timer TODO: Timer löschen


			  	    myGPSData.hour = (int)GPS.hour;
			  	    myGPSData.minute = (int)GPS.minute;
			  	    myGPSData.seconds = (int)GPS.seconds;
			  	    myGPSData.milliseconds = (int)GPS.milliseconds;
			  	    myGPSData.day = (int)GPS.day;
			  	    myGPSData.month = (int)GPS.month;
			  	    myGPSData.year = (int)GPS.year;
			  	    myGPSData.fix = GPS.fix;
			  	    myGPSData.fixquality = (int)GPS.fixquality;
			  	    if (GPS.fix)
			  	    {
			  	    	myGPSData.status = MM_SUCCESS;
			  	    	myGPSData.latitude = GPS.latitude;
			  	    	myGPSData.longitude = GPS.longitude;

			  	    	//Von Knoten auf km/h
			  	    	myGPSData.speed = GPS.speed*1.852;
			  	    	myGPSData.angle = GPS.angle;
			  	    	myGPSData.altitude = GPS.altitude;
			  	    	myGPSData.satellites = (int)GPS.satellites;
			  	    }

			  	    //Ausgabe aus dem BSP. TODO: Löschen
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
			  	    if (GPS.fix)
			  	    {
			  	      Serial.print("Location: ");
			  	      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
			  	      Serial.print(", ");
			  	      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

			  	      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
			  	      Serial.print("Angle: "); Serial.println(GPS.angle);
			  	      Serial.print("Altitude: "); Serial.println(GPS.altitude);
			  	      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
			  	    } //TODO: bis hierher die ausgabe löschen, wenn alles läuft!



	//		  		Serial.print(myGPSData.hour); Serial.print("h ");
	//		  		Serial.print(myGPSData.minute); Serial.print("m ");
	//		  		Serial.print(myGPSData.seconds); Serial.print("s ");
	//		  		Serial.print(myGPSData.milliseconds); Serial.println("ms ");
	//		  		Serial.print(myGPSData.day); Serial.print(".");
	//		  		Serial.print(myGPSData.month); Serial.print(".");
	//		  		Serial.print(myGPSData.year);
	//
	//		  		Serial.print("fix: "); Serial.print(myGPSData.fix);
	//		  		Serial.print(" qualety: "); Serial.println(myGPSData.fixquality);
	//
	//		  		Serial.print("lat: ");  Serial.println(myGPSData.latitude);
	//		  		Serial.print("lon: "); Serial.println(myGPSData.longitude);
	//		  		Serial.print("Speed: "); Serial.println(myGPSData.speed);
	//		  		Serial.print("angle: "); Serial.println(myGPSData.angle);
	//		  		Serial.print("höhe: "); Serial.println(myGPSData.altitude);
	//		  		Serial.print("satellites: "); Serial.println(myGPSData.satellites);
	//		  		Serial.println("");
		  	  }

	  }

#ifdef DEBUGLIB

#else
	  return myGPSData;
#endif
}
