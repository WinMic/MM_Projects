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
boolean DebugPC = true; //Soll Debug Info auf dem Display dargestellt werden
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
	while(p_myGPSData->status != MM_SUCCESS)
	{
		DisplayMaster("GPS noch nicht Initialisiert\n", DebugPC);
		Serial.println("GPS noch nicht Initialisiert");
		delay(2500);
		readGPS(p_myGPSData);

	}


	//init CardReader
	char * FileName = "";
	String sFileName = "GPS_Log" + String(myGPSData.year, DEC) + "_" + String(myGPSData.month, DEC)+ "_" + String(myGPSData.day, DEC) + ".txt";
	sFileName.toCharArray(FileName, sFileName.length() + 1);

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
	p_myGPSData = &myGPSData;

	readGPS(p_myGPSData);

	CardReaderValue.myFilePointer.print(myGPSData.hour); CardReaderValue.myFilePointer.print("h ");
	CardReaderValue.myFilePointer.print(myGPSData.minute); CardReaderValue.myFilePointer.print("m ");
	CardReaderValue.myFilePointer.print(myGPSData.seconds); CardReaderValue.myFilePointer.print("s ");
	CardReaderValue.myFilePointer.print(myGPSData.milliseconds); CardReaderValue.myFilePointer.print("ms\n");
	CardReaderValue.myFilePointer.print(myGPSData.day); CardReaderValue.myFilePointer.print(".");
	CardReaderValue.myFilePointer.print(myGPSData.month); CardReaderValue.myFilePointer.print(".");
	CardReaderValue.myFilePointer.print(myGPSData.year);

	CardReaderValue.myFilePointer.print("fix: "); CardReaderValue.myFilePointer.print(myGPSData.fix);
	CardReaderValue.myFilePointer.print(" qualety: "); CardReaderValue.myFilePointer.print(myGPSData.fixquality);
	CardReaderValue.myFilePointer.print("\n");


	CardReaderValue.myFilePointer.print("lat: ");  CardReaderValue.myFilePointer.print(myGPSData.latitude);
	CardReaderValue.myFilePointer.print("\n");

	CardReaderValue.myFilePointer.print("lon: "); CardReaderValue.myFilePointer.print(myGPSData.longitude);
	CardReaderValue.myFilePointer.print("\n");

	CardReaderValue.myFilePointer.print("Speed: "); CardReaderValue.myFilePointer.print(myGPSData.speed);
	CardReaderValue.myFilePointer.print("\n");

	CardReaderValue.myFilePointer.print("angle: "); CardReaderValue.myFilePointer.print(myGPSData.angle);
	CardReaderValue.myFilePointer.print("\n");

	CardReaderValue.myFilePointer.print("höhe: "); CardReaderValue.myFilePointer.print(myGPSData.altitude);
	CardReaderValue.myFilePointer.print("\n");

	CardReaderValue.myFilePointer.print("satellites: "); CardReaderValue.myFilePointer.print(myGPSData.satellites);
	CardReaderValue.myFilePointer.print("\n");



	Serial.print(myGPSData.hour); Serial.print("h ");
	Serial.print(myGPSData.minute); Serial.print("m ");
	Serial.print(myGPSData.seconds); Serial.print("s ");
	Serial.print(myGPSData.milliseconds); Serial.println("ms ");
	Serial.print(myGPSData.day); Serial.print(".");
	Serial.print(myGPSData.month); Serial.print(".");
	Serial.print(myGPSData.year);

	Serial.print("fix: "); Serial.print(myGPSData.fix);
	Serial.print(" qualety: "); Serial.println(myGPSData.fixquality);

	Serial.print("lat: ");  Serial.println(myGPSData.latitude);
	Serial.print("lon: "); Serial.println(myGPSData.longitude);
	Serial.print("Speed: "); Serial.println(myGPSData.speed);
	Serial.print("angle: "); Serial.println(myGPSData.angle);
	Serial.print("höhe: "); Serial.println(myGPSData.altitude);
	Serial.print("satellites: "); Serial.println(myGPSData.satellites);
	Serial.println("");

	Sleep(2000);
}
