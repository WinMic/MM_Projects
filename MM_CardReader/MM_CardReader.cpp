//Library für den Micro-SD Cardreader breakoutboard+ von Adafruit
#include "MM_CardReader.h"


// set up variables using the SD utility library functions:
static Sd2Card card;
static SdVolume volume;
static SdFile root;
const int SD_chipSelect = 4;
File myFile;

int SDInit(const char* FileName, boolean DebugPC)
{
	short retVal = MM_UNDEFINED_ERROR;
	SerialPrintln("\nInitializing SD card..." , DebugPC);

	pinMode(SD_chipSelect, OUTPUT);     //Arduino Due SD_chipSelect ist Pin 4 (Siehe PinOut)


	//Initialisiere den SD Card Reader
	if (!card.init(SPI_HALF_SPEED, SD_chipSelect))
	{
		SerialPrintln("initialization failed. Things to check:" , DebugPC);
		SerialPrintln("* is a card is inserted?" , DebugPC);
		SerialPrintln("* Is your wiring correct?" , DebugPC);
		SerialPrintln("* did you change the chipSelect pin to match your shield or module?" , DebugPC);
		retVal = MM_SD_NO_CARDREADER;
	}
	else
	{
		SerialPrintln("Wiring is correct and a card is present." , DebugPC);
		retVal = MM_SUCCESS;


		//Gebe den SDKarten Typ aus
		SerialPrintln("\nCard type: ", DebugPC);
		switch(card.type())
		{
			case SD_CARD_TYPE_SD1:
				SerialPrintln("SD1", DebugPC);
				break;

			case SD_CARD_TYPE_SD2:
				SerialPrintln("SD2", DebugPC);
				break;

			case SD_CARD_TYPE_SDHC:
				SerialPrintln("SDHC", DebugPC);
				break;

			default:
				SerialPrintln("Unknown", DebugPC);
		}

		// Öffnen der Partition
		if (!volume.init(card))
		{
			SerialPrintln("Could not find FAT16/FAT32 partition. Make sure you've formatted the card", DebugPC);

			retVal = MM_SD_WRONG_FORMAT;
		}
		else
		{
			//Gebe den Formattyp aus (Fat16 / Fat32)
			uint32_t volumesize;
			SerialPrintln("\nVolume type is FAT", DebugPC);
			SerialPrintln((String)volume.fatType(), DebugPC);
			//Serial.println(volume.fatType(), DEC);
			SerialPrintln("\n", DebugPC);

			volumesize = volume.blocksPerCluster();
			volumesize *= volume.clusterCount();
			volumesize *= 512;
			SerialPrintln("Volume size (bytes): ", DebugPC);
			SerialPrintln((String)volumesize, DebugPC);
			SerialPrintln("Volume size (Kbytes): ", DebugPC);
			volumesize /= 1024;
			SerialPrintln((String)volumesize, DebugPC);
			SerialPrintln("Volume size (Mbytes): ", DebugPC);
			volumesize /= 1024;
			SerialPrintln((String)volumesize, DebugPC);




			//Zeige alle Files | Datum | Größe an
			if (DebugPC == true)
			{
				SerialPrintln("\nFiles found on the card (name, date and size in bytes): ", DebugPC);
				root.openRoot(volume);
				root.ls(LS_R | LS_DATE | LS_SIZE);
			}

			if (SD.begin(SD_chipSelect))
			{

				//File öffnen und FilePointer füllen
				myFile = SD.open(FileName, FILE_WRITE);
				if (myFile)
				{
					retVal = MM_SUCCESS;
				}
				else
				{
					retVal = MM_SD_NO_FILE_POINTER;
				}

			}

		}

	}

	return retVal;
}

