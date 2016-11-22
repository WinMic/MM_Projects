// Do not remove the include below
//#include "MM_Display.h"
#include "MM_Display.h"

//#############################
//GLOBALEVARIABLEN            #
//#############################
static char Displaychars[TFT_ZEICHEN];
static int DisplayPosition = 0;

//#############################
//Objects                     #
//#############################
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


void DisplayError(String sErrorText, boolean DebugDisplay)
{
	if (DebugDisplay == true)
	{
	    tft.fillScreen(WHITE);
	    tft.setCursor(0, 0);
	    tft.setTextColor(RED);
	    tft.print(sErrorText);
	    tft.setTextColor(BLACK);
	}
}


int DisplayMaster(String sText, boolean DebugDisplay)
/*
 * Stellt einen Beliebigen Text auf dem Display dar.
 *
 * Übergabewerte:
 * -------------
 *
 * sText: ein String, der mit "\n" terminiert werden sollte
 *
 * DebugDisplay: Typ boolean, der das darstellen auf dem Display ein-/ausschaltet
 */
{
	short retVal = MM_TFT_NO_DISPLAY;
	if (DebugDisplay == true)
	{

		tft.setCursor(0, 0);
		short textLaenge = sText.length();
		retVal = MM_UNDEFINED_ERROR;


		/*
		 * Kann der Text dargestellt werden? Oder ist er zulang für das Display?
		 */
		if(textLaenge <= TFT_ZEICHEN)
		{

			//ist das letzte Element des Strings ein Newline?
			if (sText[textLaenge-1] == '\n')
			{
				//anzahl der vollen Zeilen
				byte zeilen = textLaenge / TFT_ZEICHEN_IN_ZEILE;

				//Diese anzahl von zeichen wird benötigt um alle Zeichen der Zeilen voll zu schreiben
				byte sollZeichen = (zeilen+1)*TFT_ZEICHEN_IN_ZEILE;

				byte benoetigteZeichen = sollZeichen - textLaenge - DisplayPosition%TFT_ZEICHEN_IN_ZEILE;

				//ersetze das \n durch ein Leerzeichen
				sText[textLaenge-1] = ' ';

				//ersetze alle benötigten zeichen (außer das \n das ist ja shcon ersetzt) durch leerzeichen
				for (short i = 0; i < benoetigteZeichen; i++)
				{
					sText = sText + ' ';
					textLaenge = textLaenge + 1;
				}

			}

			/*
			 * Gibt es noch genug freien Platz für den Text?
			 * Wenn Ja füge den neuen Text hinten an das Displaychars-Array an.
			 */
			if(textLaenge <= (TFT_ZEICHEN-DisplayPosition))
			{
				for (short i = 0; i <= textLaenge; i++)
				{
					Displaychars[DisplayPosition+i] = sText[i];	//Der angepasste Text wird in den "Virtuellen Display" geschrieben.
				}

				//Den "Offset" des gesamten Textes speichern
				DisplayPosition = DisplayPosition + textLaenge;

				tft.fillScreen(WHITE); //Lösche das Gesammte Display

				//Schreibe den alten Text in Schwarz auf das Display
				tft.setTextColor(BLACK);
				for (short i = 0; i < DisplayPosition-textLaenge; i++)
				{
					tft.print(Displaychars[i]);
				}

				//Schreibe den neuen Text in Blau auf das Display
				tft.setTextColor(BLUE);
				for (short i = DisplayPosition-textLaenge; i < DisplayPosition; i++)
				{
					tft.print(Displaychars[i]);
				}

				//Setze wieder die Standartfarbe
				tft.setTextColor(BLACK);

			}
			/*
			 * Nein es gibt nicht mehr genug freien Platz für den Text.
			 * Somit muss im Array etwas verschoben werden!
			 */
			else
			{
				/*
				 * Shifte den gesamten Text um eine Zeile, damit die Leerzeichen, die newlines ersetzen noch passend stehen.
				 * Wenn das Shiften um eine Zeile nicht ausreicht, wird beim nächsten durchlauf noch einmal geshiftet, bis es klappt.
				 */
				for (short i = 0; i <= TFT_ZEICHEN-TFT_ZEICHEN_IN_ZEILE; i++)
				{
				  Displaychars[i] = Displaychars[i+TFT_ZEICHEN_IN_ZEILE];
				}

				DisplayPosition = DisplayPosition - TFT_ZEICHEN_IN_ZEILE;


				DisplayMaster(sText, DebugDisplay); //Rufe dich selbst nocheinmal auf, den nun ist genug Platz im vorhanden.
			}

			retVal = MM_SUCCESS;
		}

		//Der Text ist zu lang und kann nicht dargestellt werden.
		else
		{
			retVal = MM_TFT_TEXT_TO_LONG;
		}

	}
	return(retVal);
}


int TFTInit()
{
	short retVal = MM_UNDEFINED_ERROR;

	Serial.println("TFT LCD test");
	Serial.println("Using Adafruit 2.8\" TFT Breakout Board Pinout");
	Serial.println("TFT size is ");
	Serial.println((String)tft.width());
	Serial.println("x");
	Serial.println((String)tft.height());

	tft.reset();

	uint16_t identifier = tft.readID();

	if(identifier == 0x9325)
	{
	Serial.println("Found ILI9325 LCD driver");
	retVal = MM_SUCCESS;
	}
	else if(identifier == 0x9328)
	{
	Serial.println("Found ILI9328 LCD driver");
	retVal = MM_SUCCESS;
	}
	else if(identifier == 0x7575)
	{
	Serial.println("Found HX8347G LCD driver");
	retVal = MM_SUCCESS;
	}
	else if(identifier == 0x9341)
	{
	Serial.println("Found ILI9341 LCD driver");
	retVal = MM_SUCCESS;
	}
	else if(identifier == 0x8357)
	{
	Serial.println("Found HX8357D LCD driver");
	retVal = MM_SUCCESS;
	}
	else
	{
	Serial.println("Unknown LCD driver chip: ");

	//Serial.println(identifier, HEX);
	Serial.println((String)identifier);
	Serial.println("If using the Adafruit 2.8\" TFT Arduino shield, the line:");
	Serial.println("  #define USE_ADAFRUIT_SHIELD_PINOUT");
	Serial.println("should appear in the library header (Adafruit_TFT.h).");
	Serial.println("If using the breakout board, it should NOT be #defined!");
	Serial.println("Also if using the breakout, double-check that all wiring");
	Serial.println("matches the tutorial.");
	retVal = MM_TFT_NO_DISPLAY;

	}

	tft.setRotation(TFT_ROTATION);
	tft.fillScreen(WHITE);
	tft.setTextColor(BLACK);
	tft.setTextSize(TFT_TEXT_SIZE);
	tft.begin(identifier);

	return retVal;
  }
