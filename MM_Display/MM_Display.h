// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _Display_H_
#define _Display_H_

//#############################
//INCLUDES                    #
//#############################
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <MM_DEFINES.h>



//#############################
//DEFINES                     #
//#############################

//ALLGEMEIN
#define BAUD 115200

//DISPLAY
#define TFT_ROTATION 3
#define TFT_TEXT_SIZE 2
#define TFT_ZEICHEN 390
#define TFT_ZEICHEN_IN_ZEILE 26

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define LCD_RESET A4
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0


//#############################
//DEKLARATION                 #
//#############################

//Display
int DisplayMaster(String, boolean);
void DisplayError(String);
int TFTInit(boolean);



#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Display here




//Do not add code below this line
#endif /* _Display_H_ */
