///*Benoetigte Librarys:
// *MM_DEFINES
// *
// *MM_Display
// *Adafruit_GFX
// *Adafruit_TFTLCD
// *
// *Adafruit_GPS
// *
// *SD (CardReader)
// *SPI
// *
// *Sparfun HMC6352
// *Wire
// *
// */
#include "GPS_Tracker.h"

//init Daten
CardReaderReturn CardReaderValue;
short compassStatus = MM_UNDEFINED_ERROR;

GPSValues myGPSData;
GPSValues *p_myGPSData;

bool errorflag = false;

magnetometerValues myCompassData;

void setup()
{
	pinMode(ERROR_LED, OUTPUT);
	CardReaderValue.cardStatus = MM_UNDEFINED_ERROR;

	Serial.begin(115200);
	Serial.println("Init");

	//init GPS
	p_myGPSData = &myGPSData;
	initGPS();

	//Versuche ein GPS Signal zu bekommen, damit die Logdatei das Datum als Namen bekommt
	while((p_myGPSData->status != MM_SUCCESS) || (p_myGPSData->day == 0))	//When day != 0 we have at least one satalite with a timestamp
	{
		Serial.println("GPS nicht Initialisiert");
		readGPS(p_myGPSData);
		Serial.print("p_myGPSData->day: "); Serial.println(p_myGPSData->day);
	}


	//init CardReader
	char FileName[13];	//FAT maximum filename length is 8.3!
	String sFileName = String(myGPSData.year, DEC) + "_" + String(myGPSData.month, DEC)+ "_" + String(myGPSData.day, DEC) + ".txt";
	sFileName.toCharArray(FileName, sFileName.length() + 1);
	Serial.print("Filename: "); Serial.println(FileName);

	CardReaderValue = SDInit(FileName);

	switch (CardReaderValue.cardStatus)
	{
		case MM_SUCCESS:
			Serial.println("CardReader Initialisiert");
			CardReaderValue.myFilePointer.print("The following GPS coordinates are printed as followed:\n" \
					"Lat: 4042.6142 --> 40 degrees, 42.6142 decimal minutes North:\n" \
					"Lon: 07400.4168 --> 74 degrees, 00.4168 decimal minutes West\n" \
					"Unfortunately gmaps requires you to use +/- instead of NSWE notation. N and E are positive, S and W are negative.\n\n");
			break;

		default:
			Serial.println("CardReader Init-ERROR!");
			errorflag = true;
			while(1)
			{
				Serial.println("CardReader Init-ERROR!");
				//TODO: Set Error LED and so not continue to loop()
			}
			break;
	}

	compassStatus = HMC5883lInit();
	switch (compassStatus)
	{
		case MM_SUCCESS:
			Serial.println("Compass initialisiert");
			CardReaderValue.myFilePointer.print("Compass-heading is calculated with the magnetic declination of Munic\n" \
					"for individual calculation please use RAW values and use individual magnetic declination --> http://www.magnetic-declination.com/ \n\n");
			break;

		default:
			Serial.println("Compass Init-ERROR!");
			CardReaderValue.myFilePointer.print("Kompass Init-ERROR!\n");
			errorflag = true;
			break;
	}

	digitalWrite(ERROR_LED, HIGH);
}



void loop()
{
	static uint32_t timer = millis();
	readGPS(p_myGPSData);
	myCompassData = HMC5883lHeading();

	if (timer > millis()) { timer = millis(); }

	if ((millis() - timer > WRITE_EVERY_MILLIS_TO_FILE) && (p_myGPSData->status == MM_SUCCESS))
	{
		printGPSData(&myGPSData);

		CardReaderValue.myFilePointer.print(myGPSData.day); CardReaderValue.myFilePointer.print(".");
		CardReaderValue.myFilePointer.print(myGPSData.month); CardReaderValue.myFilePointer.print(".");
		CardReaderValue.myFilePointer.print(myGPSData.year); CardReaderValue.myFilePointer.print(" - ");

		CardReaderValue.myFilePointer.print(myGPSData.hour); CardReaderValue.myFilePointer.print("h ");
		CardReaderValue.myFilePointer.print(myGPSData.minute); CardReaderValue.myFilePointer.print("m ");
		CardReaderValue.myFilePointer.print(myGPSData.seconds); CardReaderValue.myFilePointer.print("s ");
		CardReaderValue.myFilePointer.print(myGPSData.milliseconds); CardReaderValue.myFilePointer.print("ms\n");

		CardReaderValue.myFilePointer.print("fix: "); CardReaderValue.myFilePointer.print(myGPSData.fix);
		CardReaderValue.myFilePointer.print(" quality: "); CardReaderValue.myFilePointer.print(myGPSData.fixquality);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("Compass-RAW-angle X: "); CardReaderValue.myFilePointer.print(myCompassData.magneticX); CardReaderValue.myFilePointer.print("\n");
		CardReaderValue.myFilePointer.print("Compass-RAW-angle Y: "); CardReaderValue.myFilePointer.print(myCompassData.magneticY); CardReaderValue.myFilePointer.print("\n");
		CardReaderValue.myFilePointer.print("Compass-RAW-angle Z: "); CardReaderValue.myFilePointer.print(myCompassData.magneticZ); CardReaderValue.myFilePointer.print("\n");
		CardReaderValue.myFilePointer.print("Compass-heading: "); CardReaderValue.myFilePointer.print(myCompassData.heading); CardReaderValue.myFilePointer.print("\n");

		if (myGPSData.fix) {
			CardReaderValue.myFilePointer.print("GPS-angle: "); CardReaderValue.myFilePointer.print(myGPSData.angle); CardReaderValue.myFilePointer.print("\n");
			CardReaderValue.myFilePointer.print("\n");

			CardReaderValue.myFilePointer.print("latitude: ");  CardReaderValue.myFilePointer.print((double)myGPSData.latitude, 8);
			CardReaderValue.myFilePointer.print(" "); CardReaderValue.myFilePointer.print(myGPSData.lat);
			CardReaderValue.myFilePointer.print("\n");

			CardReaderValue.myFilePointer.print("longitude: "); CardReaderValue.myFilePointer.print((double)myGPSData.longitude, 8);
			CardReaderValue.myFilePointer.print(" "); CardReaderValue.myFilePointer.print(myGPSData.lon);
			CardReaderValue.myFilePointer.print("\n");

			CardReaderValue.myFilePointer.print("speed: "); CardReaderValue.myFilePointer.print(myGPSData.speed);
			CardReaderValue.myFilePointer.print("\n");

			CardReaderValue.myFilePointer.print("altitude: "); CardReaderValue.myFilePointer.print(myGPSData.altitude);
			CardReaderValue.myFilePointer.print("\n");

			CardReaderValue.myFilePointer.print("satellites: "); CardReaderValue.myFilePointer.print(myGPSData.satellites);
			CardReaderValue.myFilePointer.print("\n");CardReaderValue.myFilePointer.print("\n");
		}
		CardReaderValue.myFilePointer.flush();

		clearGPSData(p_myGPSData);
	}

}
