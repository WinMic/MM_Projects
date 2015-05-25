// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _MM_MPU6050_H_
#define _MM_MPU6050_H_
#include "Arduino.h"
#include "I2Cdev.h"
#include "MPU60X0.h"
#include "MM_DEFINES.h"


//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif

int MPUInit(boolean);
void MPUData(int16_t* , int16_t* , int16_t* , int16_t* , int16_t* , int16_t*);


#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project MM_MPU6050 here




//Do not add code below this line
#endif /* _MM_MPU6050_H_ */
