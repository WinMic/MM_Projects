// Do not remove the include below
#include "MM_GPS.h"


Adafruit_GPS GPS(&mySerial);

//GPS wird ohne Interrupts verwendet
boolean usingInterrupt = false;


//Sp�ter Init
void initGPS()
{
	  //Die TX und RX verbindung zwischen Arduino und GPS mit BAUD 9600
	  GPS.begin(9600);
	  mySerial.begin(9600);

	  //Wir wollen RMC (recommended minimum) und GGA (fix data) inklusive der H�he wissen
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


//Sp�ter hole Daten
//uint32_t timer = millis(); TODO: Timer l�schen

GPSValues readGPS()
{
	GPSValues myGPSData;
	GPS.read();

	// if a sentence is received, we can check the checksum, parse it...
	  if (GPS.newNMEAreceived())
	  {
	    // a tricky thing here is if we print the NMEA sentence, or data
	    // we end up not listening and catching other sentences!
	    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
	    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

	    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
	    	myGPSData.status = MM_GPS_ERROS;  // we can fail to parse a sentence in which case we should just wait for another
	  }

	  // if millis() or timer wraps around, we'll just reset it
//	  if (timer > millis())  timer = millis(); TODO: Timer l�schen

	  // approximately every 2 seconds or so, print out the current stats
//	  if (millis() - timer > 2000) TODO: Timer l�schen
	  {
//	    timer = millis(); // reset the timer TODO: Timer l�schen


	    myGPSData.hour = (int)GPS.hour;
	    myGPSData.minute = (int)GPS.minute;
	    myGPSData.seconds = (int)GPS.seconds;
	    myGPSData.milliseconds = (int)GPS.milliseconds;
	    myGPSData.day = (int)GPS.day;
	    myGPSData.month = (int)GPS.month;
	    myGPSData.year = (int)GPS.year;
	    myGPSData.fix = GPS.fix;
	    myGPSData.fixquality = (int)GPS.fixquality;
		//
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

//		  Serial.print(myGPSData.hour); Serial.print("h ");
//		  Serial.print(myGPSData.minute); Serial.print("m ");
//		  Serial.print(myGPSData.seconds); Serial.print("s ");
//		  Serial.print(myGPSData.milliseconds); Serial.println("ms ");
//		  Serial.print(myGPSData.day); Serial.print(".");
//		  Serial.print(myGPSData.month); Serial.print(".");
//		  Serial.println(myGPSData.year);
//
//		  Serial.print("fix: "); Serial.print(myGPSData.fix);
//		  Serial.print(" qualety: "); Serial.println(myGPSData.fixquality);
//
//		  Serial.print("lat: ");  Serial.println(myGPSData.latitude);
//		  Serial.print("lon: "); Serial.println(myGPSData.longitude);
//		  Serial.print("Speed: "); Serial.println(myGPSData.speed);
//		  Serial.print("angle: "); Serial.println(myGPSData.angle);
//		  Serial.print("h�he: "); Serial.println(myGPSData.altitude);
//		  Serial.print("satellites: "); Serial.println(myGPSData.satellites);
//		  Serial.println("");
	  }

	  return myGPSData;
}
