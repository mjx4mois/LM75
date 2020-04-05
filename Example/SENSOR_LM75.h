/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LM75 temperature sensor
     File Name		: SENSOR_LM75.h
     Function		: SENSOR_LM75
     Create Date	: 2017/03/26

     HAL Layer : SWI2C
---------------------------------------------------------------------- */
#ifndef __LM75_HEADER__ 
#define __LM75_HEADER__


//--------------------------------- Define SLAVE ADDRESS -------------------------------------
// LM75_A2 ~ A0 --> pin define Slave  Address

// Ground set "0" , Vcc set "1"
//-----------------
#define LM75_A2 0
#define LM75_A1 0
#define LM75_A0 0

#define LM75_SLAVE_ADDRESS  ( 0X90 | (LM75_A2<<3) | (LM75_A1<<2) | (LM75_A0<<1) )
//--------------------------------- Define SLAVE ADDRESS -------------------------------------


#define LM75_READ  1
#define LM75_WRITE 0

#define LM75_THYST 0
#define LM75_TOS   1


//----------------------------------- Function --------------------------------------


CHAR8S LM75_READ_TEMPERATURE(CHAR8U LM75_S_ADDRESS,FLOAT *LM75_Temperature);
CHAR8S LM75_THYST_TOS_CONRTOL(CHAR8U LM75_S_ADDRESS,CHAR8U RW_BIT,CHAR8U THYST_TOS_BIT,FLOAT *LM75_HYOS_Data);
CHAR8S LM75_CONFIG_CONTROL(CHAR8U LM75_S_ADDRESS,CHAR8U RW_BIT,CHAR8U *LM75_Configure);


//----------------------------------- Function --------------------------------------

#endif		//#ifndef __LM75_HEADER__ 