//Library für den Micro-SD Cardreader breakoutboard+ von Adafruit
#include "MM_CardReader.h"


// set up variables using the SD utility library functions:
static Sd2Card card;
static SdVolume volume;
static SdFile root;
const int SD_chipSelect = 4;
File myFile;

CardReaderReturn SDInit(const char* FileName)
{
	CardReaderReturn retVal;
	retVal.cardStatus = MM_UNDEFINED_ERROR;
	Serial.println("\nInitializing SD card...");

	pinMode(SD_chipSelect, OUTPUT);     //Arduino Due SD_chipSelect ist Pin 4 (Siehe PinOut)

	//Initialisiere den SD Card Reader
	if (!card.init(SPI_HALF_SPEED, SD_chipSelect))
	{
		Serial.println("initialization failed. Things to check:");
		Serial.println("* is a card is inserted?");
		Serial.println("* Is your wiring correct?");
		Serial.println("* did you change the chipSelect pin to match your shield or module?");
		retVal.cardStatus = MM_SD_NO_CARDREADER;
	}
	else
	{
		Serial.println("Wiring is correct and a card is present.");
		retVal.cardStatus = MM_SUCCESS;


		//Gebe den SDKarten Typ aus
		Serial.println("\nCard type: ");
		switch(card.type())
		{
			case SD_CARD_TYPE_SD1:
				Serial.println("SD1");
				break;

			case SD_CARD_TYPE_SD2:
				Serial.println("SD2");
				break;

			case SD_CARD_TYPE_SDHC:
				Serial.println("SDHC");
				break;

			default:
				Serial.println("Unknown");
		}

		// Öffnen der Partition
		if (!volume.init(card))
		{
			Serial.println("Could not find FAT16/FAT32 partition. Make sure you've formatted the card");

			retVal.cardStatus = MM_SD_WRONG_FORMAT;
		}
		else
		{
			//Gebe den Formattyp aus (Fat16 / Fat32)
			uint32_t volumesize;
			Serial.println("\nVolume type is FAT");
			Serial.println((String)volume.fatType());
			//Serial.println(volume.fatType(), DEC);
			Serial.println("\n");

			volumesize = volume.blocksPerCluster();
			volumesize *= volume.clusterCount();
			volumesize *= 512;
			Serial.println("Volume size (bytes): ");
			Serial.println((String)volumesize);
			Serial.println("Volume size (Kbytes): ");
			volumesize /= 1024;
			Serial.println((String)volumesize);
			Serial.println("Volume size (Mbytes): ");
			volumesize /= 1024;
			Serial.println((String)volumesize);




			//Zeige alle Files | Datum | Größe an

			Serial.println("\nFiles found on the card (name, date and size in bytes): ");
			root.openRoot(volume);
			root.ls(LS_R | LS_DATE | LS_SIZE);
			Serial.print("creating file: "); Serial.println(FileName);
			if (SD.begin(SD_chipSelect))
			{
				Serial.println("SD.begin - Success");
				//File öffnen und FilePointer füllen
				myFile = SD.open(FileName, WRITEAPPENDCREATE);
				if (myFile)
				{
					Serial.println("Valid pointer");
					if (SD.exists(FileName))
					{
						Serial.println("File Exists");
						retVal.cardStatus = MM_SUCCESS;
						retVal.myFilePointer = myFile;
					}
				}
				else
				{
					retVal.cardStatus = MM_SD_NO_FILE_POINTER;
				}

			}

		}

	}

	return retVal;
}
