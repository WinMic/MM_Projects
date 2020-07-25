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
boolean DebugPC = false; //Soll Debug Info auf dem Display dargestellt werden
//TODO: DebugPC Variable muss durch Pinabfrage auf Arduino gesetzt werden -Hotswap Display-

short displayStatus = MM_UNDEFINED_ERROR;
CardReaderReturn CardReaderValue;
short kompassStatus = MM_UNDEFINED_ERROR;

GPSValues myGPSData;
GPSValues *p_myGPSData;

void setup()
{
	CardReaderValue.cardStatus = MM_UNDEFINED_ERROR;

	Serial.begin(115200);
	Serial.println("Init");

	//init Display
	displayStatus = TFTInit();
	switch (displayStatus)
	{
		case MM_SUCCESS:
			DisplayMaster("Display Initialisiert\n", DebugPC);
			Serial.println("Display Initialisiert");
			break;

		default:
			DisplayError("Display Init-ERROR!", DebugPC);
			Serial.println("Display Init-ERROR!");
			DebugPC = false;
			break;
	}


	//init GPS
	p_myGPSData = &myGPSData;
	initGPS();

	//Versuche ein GPS Signal zu bekommen, damit die Logdatei das Datum als Namen bekommt
	while((p_myGPSData->status != MM_SUCCESS) || (p_myGPSData->day == 0))	//When day != 0 we have at least one satalite with a timestamp
	{
		DisplayMaster("GPS nicht Initialisiert\n", DebugPC);
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
			DisplayMaster("CardReader Initialisiert\n", DebugPC);
			Serial.println("CardReader Initialisiert");
			break;

		default:
			DisplayError("CardReader Init-ERROR!", DebugPC);
			Serial.println("CardReader Init-ERROR!");
			break;
	}



}



void loop()
{
	static uint32_t timer = millis();
	readGPS(p_myGPSData);

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


		CardReaderValue.myFilePointer.print("latitude: ");  CardReaderValue.myFilePointer.print(myGPSData.latitude);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("longitude: "); CardReaderValue.myFilePointer.print(myGPSData.longitude);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("speed: "); CardReaderValue.myFilePointer.print(myGPSData.speed);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("angle: "); CardReaderValue.myFilePointer.print(myGPSData.angle);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("altitude: "); CardReaderValue.myFilePointer.print(myGPSData.altitude);
		CardReaderValue.myFilePointer.print("\n");

		CardReaderValue.myFilePointer.print("satellites: "); CardReaderValue.myFilePointer.print(myGPSData.satellites);
		CardReaderValue.myFilePointer.print("\n");CardReaderValue.myFilePointer.print("\n");
		CardReaderValue.myFilePointer.flush();

		clearGPSData(p_myGPSData);
	}

}
