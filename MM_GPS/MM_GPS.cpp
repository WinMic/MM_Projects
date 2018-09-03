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
//Thx to http://bradsduino.blogspot.com/2013/06/adafruit-ultimate-gps-breakout-arduino.html
void readGPS(GPSValues* p_myGpsData)
{
    static String sentence = ""; //static to keep an unfinished sentence

    char data[85];  //maximum length of NMEA-0183 sentence is 82 byte
    char *dataPtr = data;
    char *value;
    //how many chars available?
    short readable = Serial1.available();

    if (0 < readable)
    {
        for (int i = 0; i <= readable; ++i)
        {
            char c = Serial1.read();
            // Check if end of NMEA sentence. If yes start validation of sentence
            if(c == '\n')
            {
                // Check if $GPRMC NMEA sentence
                if(sentence.startsWith("$GPRMC"))
                {
                    sentence.toCharArray(data, sentence.length());
                    int j = 0;
                    // Tokensize each line into values using comma delimiter.
                    // Returns NULL when no more tokens.
                    // strtok_r is part of C standard library.
                    while ((value = strtok_r(dataPtr, ",", &dataPtr)) != NULL)
                    {
                        switch (j)
                        {
                            case 0:
                                //NOOP first value is $GPRMC
                                break;

                            case 1:
                                //extract time (Note UTC-timeformat)
                                myGpsData.hour = atoi(value)/10000;
                                myGpsData.minute = (atoi(value) - 10000*myGpsData.hour) / 100;
                                myGpsData.seconds = (atoi(value) - 10000*myGpsData.hour - 100*myGpsData.minute);
#if GPSECHO
                                Serial.println("Stunde: "); Serial.println(myGpsData.hour);
                                Serial.println("Minute: "); Serial.println(myGpsData.minute);
                                Serial.println("Sekunde: "); Serial.println(myGpsData.seconds);
#endif
                                break;

                            case 2:
                                if ('A' == *value)
                                {
                                    myGpsData.status = true;
                                } else if ('V' == *value)
                                {
                                    myGpsData.status = false;
                                }
                                else
                                    myGpsData.status = false;
                                //TODO: set failure returnvalue

                                break;
                            case 3:
                            case 5:
                                int degMin = atoi(value);
                                int degree = (int) degMin / 100;
                                int minutes = degMin - (degree * 100);

                                float seconds = (float) (atof(value) - (float) degMin) * 60.0;
                                String label;
                                if(j == 3)
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
                                break;

                            case 7:
                                myGpsData.speed = atoi(value)*1,852;
                                break;

                            case 8:
                                myGpsData.angle = atoi(value);
                                break;

                            case 9:
                                myGpsData.day = atoi(value)/10000;
                                myGpsData.month =(atoi(value) - 10000*myGpsData.day) / 100;
                                myGpsData.year =(atoi(value) - 10000*myGpsData.month - 100*myGpsData.day);
                                break;

                            default:
                                break;
                        }
                        j++;
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
}

// Arduino IDE libraries do not include dtostrf()
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}
