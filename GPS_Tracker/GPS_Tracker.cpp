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
short kompassStatus = MM_UNDEFINED_ERROR;

void setup()
{
	Serial.begin(115200);
	Serial.println("Init");

	//init Display
	displayStatus = TFTInit(DebugPC);
	switch (displayStatus)
	{
		case MM_SUCCESS:
			DisplayMaster("Display Initialisiert\n", DebugPC);
			Serial.println("Display Initialisiert");
			break;

		default:
			DisplayError("Display Init-ERROR!", DebugPC);
			Serial.println("Display Init-ERROR!");
			break;
	}


}


void loop()
{

}
