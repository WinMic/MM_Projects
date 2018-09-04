#include <Adafruit_GPS.h>
#include "MM_GPS.h"

Adafruit_GPS GPS(&mySerial);


GPSValues myGpsData;
GPSValues *p_myGpsData;


#ifdef DEBUGLIB
    void setup()
    {
        Serial.begin(115200);
        Serial.println("MM GPSLib test");
        initGPS();
    }
#endif

#ifdef DEBUGLIB
    void loop()
    {
        readGPS(p_myGpsData);
	}
#endif


//void initGPS()
void initGPS()
{
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS.begin(9600);
    mySerial.begin(9600);

    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
      GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
      // uncomment this line to turn on only the "minimum recommended" data
      //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
      // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
      // the parser doesn't care about other sentences at this time

      // Set the update rate
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
      // For the parsing code to work nicely and have time to sort thru the data, and
      // print it out we don't suggest using anything higher than 1 Hz

      // Request updates on antenna status, comment out to keep quiet
      GPS.sendCommand(PGCMD_ANTENNA);


      delay(1000);
      // Ask for firmware version
      mySerial.println(PMTK_Q_RELEASE);
}

void readGPS(GPSValues* p_myGpsData)
{
    static String sentence = ""; //static to keep an unfinished sentence

    char data[85];  //maximum length of NMEA-0183 sentence is 82 byte

    //how many chars available?
    short readable = Serial1.available();

    if (0 < readable)
    {
        for (int i = 0; i <= readable; ++i)
        {
            char c = Serial1.read();
            // Check if end of NMEA sentence. If yes start validation of sentence
            if(c == '\n')
            {   sentence.toCharArray(data, sentence.length());
                GPS.parse(data);
                sentence = "";
            }
            else
            {
                // Append character if not end of sentence
                sentence += c;
            }
        }
    }
}
