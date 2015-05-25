// Do not remove the include below
#include "MM_DEFINES.h"


void SerialPrintln(String sText, boolean DebugPC)
{
	if (DebugPC == true)
	{
		Serial.println(sText);
	}
}

void SerialPrint(String sText, boolean DebugPC)
{
	if (DebugPC == true)
	{
		Serial.print(sText);
	}
}
