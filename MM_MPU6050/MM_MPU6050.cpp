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

//#############################
//GLOBALEVARIABLEN            #
//#############################


/*
 * Initialisiert die MPU6050 und prüft anschließend die Verbindung, ob die Initialisierung geklappt hat.
 * Übergabeparameter: boolean DebugPC
 * Rückgabewert: MM_ERROR
 */
int MPUInit(boolean DebugPC)
{
	int retVal = MM_UNDEFINED_ERROR;

    SerialPrintln("Initializing I2C devices...", DebugPC);
    accelgyro.initialize();

    if (accelgyro.testConnection())
    {
    	retVal = MM_SUCCESS;
	}
    else
    {
    	SerialPrintln("MPU nicht initialisiert!", DebugPC);
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

		}
	}


}
void loop()
{
	MPUData(&ax, &ay, &az, &gx, &gy, &gz);
	SerialPrint("Ax: ",Debug);
	SerialPrintln((String)ax, Debug);
	SerialPrint("Ay: ",Debug);
	SerialPrintln((String)ay, Debug);
	SerialPrint("Az: ",Debug);
	SerialPrintln((String)az, Debug);
	SerialPrint("Gx: ",Debug);
	SerialPrintln((String)gx, Debug);
	SerialPrint("Gy: ",Debug);
	SerialPrintln((String)gy, Debug);
	SerialPrint("Gz: ",Debug);
	SerialPrintln((String)gz, Debug);

	Sleep(1000);

}

//int16_t ax, ay, az;
//int16_t gx, gy, gz;
//
//#define LED_PIN 13
//bool blinkState = false;
//
//void setup() {
//    // join I2C bus (I2Cdev library doesn't do this automatically)
//    Wire.begin();
//
//    // initialize serial communication
//    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
//    // it's really up to you depending on your project)
//    Serial.begin(9600);
//
//    // initialize device
//    Serial.println("Initializing I2C devices...");
//    accelgyro.initialize();
//
//    // verify connection
//    Serial.println("Testing device connections...");
//    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
//
//    // configure Arduino LED for
//    pinMode(LED_PIN, OUTPUT);
//}
//
//void loop() {
//    // read raw accel/gyro measurements from device
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//
//    // these methods (and a few others) are also available
//    //accelgyro.getAcceleration(&ax, &ay, &az);
//    //accelgyro.getRotation(&gx, &gy, &gz);
//
//    // display tab-separated accel/gyro x/y/z values
//    Serial.print("a/g:\t");
//    Serial.print(ax); Serial.print("\t");
//    Serial.print(ay); Serial.print("\t");
//    Serial.print(az); Serial.print("\t");
//    Serial.print(gx); Serial.print("\t");
//    Serial.print(gy); Serial.print("\t");
//    Serial.println(gz);
//
//    // blink LED to indicate activity
//    blinkState = !blinkState;
//    digitalWrite(LED_PIN, blinkState);
//}
