/*
 * Benötigte Librarys: 	I2Cdev
 * 						MM_DEFINES
 * 						Sparkfun_MPU60X0
 * 						SPI
 * 						Wire
 */

//Includes
#include "MM_MPU6050.h"


//#############################
//Objects                     #
//#############################
MPU60X0 accelgyro;

/*
 * Initialisiert die MPU6050 und prüft anschließend die Verbindung, ob die Initialisierung geklappt hat.
 * Übergabeparameter: boolean DebugPC
 * Rückgabewert: MM_ERROR
 */
int MPUInit(boolean DebugPC)
{
	int retVal = MM_UNDEFINED_ERROR;

    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    if (accelgyro.testConnection())
    {
    	retVal = MM_SUCCESS;
	}
    else
    {
    	Serial.println("MPU nicht initialisiert!");
    	retVal = MM_MPU_INIT_ERROR;
    }

	return retVal;
}

/*
 * Fragt die MPU6050 ab.
 * Übergabeparameter: 6 Pointer auf int16_t Integer.
 * Rückgabewert: KEINEN
 */
void MPUData(int16_t* p_ax, int16_t* p_ay, int16_t* p_az, int16_t* p_gx, int16_t* p_gy, int16_t* p_gz)
{
	accelgyro.getMotion6(p_ax, p_ay, p_az, p_gx, p_gy, p_gz);
}

/*
 * Fragt die MPU6050 nach der temperatur ab.
 * Übergabeparameter: KEINEN
 * Rückgabewert: Integer (°C)
 */
int MPUTemp()
{	//holt sich 16 bit im 2er Kompliment format
	int16_t smallInt = accelgyro.getTemperature();


	const int negative = (smallInt & (1 << 17)) != 0;
	int nativeInt;

	if (negative)
	  nativeInt = smallInt | ~((1 << 18) - 1);
	else
	  nativeInt = smallInt;

	return nativeInt;
}



#ifdef DEBUGLIB
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    boolean Debug = true;

    void setup()
    {
        Serial.begin(115200);
        Wire.begin();

        int Status = -1;
        Status = MPUInit(Debug);

        if (Status != MM_SUCCESS)
        {
            while(1)
            {
                Serial.print("Initfehler");
            }
        }

        }
    void loop()
    {
        MPUData(&ax, &ay, &az, &gx, &gy, &gz);

        float beschX = ax/16384.0;
        float beschY = ay/16384.0;
        float beschZ = az/16384.0;

        Serial.print("Gx: "); Serial.print(beschX);
        Serial.print("Gy: "); Serial.print(beschY);
        Serial.print("Gz: "); Serial.print(beschZ);

        short temp = MPUTemp();
        Serial.print("temp: "); Serial.print(temp);

    Sleep(1000);
}
#endif
