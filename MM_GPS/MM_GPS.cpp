#include <Adafruit_GPS.h>
#include "MM_GPS.h"

Adafruit_GPS GPS(&mySerial);




GPSValues myGpsData;
GPSValues *p_myGpsData;
String sentence = "";

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
//from http://bradsduino.blogspot.com/2013/06/adafruit-ultimate-gps-breakout-arduino.html
void readGPS(GPSValues* p_myGpsData)
{
    if (Serial1.available() > 0)
    {
        char c = Serial1.read();
        // Check if end of NMEA sentence
        if(c == '\n')
        {
            // Check if $GPRMC NMEA sentence
            if(sentence.startsWith("$GPRMC"))
            {
                char data[150];
                char *dataPtr = data;
                char *value;
                sentence.toCharArray(data, sentence.length());
                int i = 0;
                // Tokensize each line into values using comma delimiter.
                // Returns NULL when no more tokens.
                // strtok_r is part of C standard library.
                while ((value = strtok_r(dataPtr, ",", &dataPtr)) != NULL)
                {
                    // 4th value is north latitude, 6th is west longitude
                    if (i == 1)
                    {
                        myGpsData.hour = atoi(value)/10000;
                        myGpsData.minute = (atoi(value) - 10000*myGpsData.hour) / 100;
                        myGpsData.seconds = (atoi(value) - 10000*myGpsData.hour - 100*myGpsData.minute);
                        Serial.println("Stunde: "); Serial.println(myGpsData.hour);
                        Serial.println("Minute: "); Serial.println(myGpsData.minute);
                        Serial.println("Sekunde: "); Serial.println(myGpsData.seconds);
                    }
                    if(i == 3 || i == 5)
                    {
                        int degMin = atoi(value);
                        int degree = (int) degMin / 100;
                        int minutes = degMin - (degree * 100);
                        float seconds = (float) (atof(value) - (float) degMin) * 60.0;
                        String label;
                        if(i == 3)
                        {
                            label = " N";
                        }
                        else
                        {
                            label = " E";
                        }
                        char secBuffer[6];
                        dtostrf(seconds, 6, 3, secBuffer);
                        char location[100];
                        sprintf(location, "%02d\xB0 %02d' %s\"", degree, minutes, secBuffer);
                        Serial.println(location + label);
                    }
                    i++;
                }
                Serial.println();
            }
            sentence = "";
        }
        else
        {
            // Append character if not end of sentence
            sentence += c;
        }
    }
}

// Arduino IDE libraries do not include dtostrf()
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
