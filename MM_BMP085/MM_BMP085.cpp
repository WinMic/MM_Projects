//includes
#include "MM_BMP085.h"

//Objects
static Adafruit_BMP085 bmp;

/*
 * Initialisiert den BMP085 und �berpr�ft, ob die Initialisierung geklappt hat.
 * �bergabeparameter: boolean DebugPC
 * R�ckgabewert: MM_ERROR
 */
int BMPInit(boolean DebugPC)
{
	int retVal = MM_BMP085_INIT_ERROR;

	if (bmp.begin())
	{
		SerialPrintln("BMP085 wurde initialisiert.", DebugPC);
		retVal = MM_SUCCESS;
	}

	return retVal;
}


/*
 * Liest die Daten vom BMP085 aus.
 * �bergabeparameter: float* Temperatur, int32_t* Druck, float* H�he
 * R�ckgabewert: keiner
 */
void BMPData(float* temp, int32_t* pres, float* alt)
{
    *temp = bmp.readTemperature();

    *pres = bmp.readPressure();

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    *alt = bmp.readAltitude(101500);
}
