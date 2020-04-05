/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: LM75 temperature sensor
     File Name		: SENSOR_LM75.c
     Function		: SENSOR_LM75
     Create Date	: 2017/03/26
---------------------------------------------------------------------- */
#ifndef __LM75_FUNCTION__
#define __LM75_FUNCTION__

#include <stdio.h>
#include <math.h>
#include <delay.h>

#include "SENSOR_LM75.h"
#include "Porting_Layer.h"
#include "i2c.h"

//--------------------------------------------------------------------------------------------------
CHAR8S LM75_READ_TEMPERATURE(CHAR8U LM75_S_ADDRESS,FLOAT *LM75_Temperature)
{
	CHAR8U busy,ack;

    CHAR8U lm75_high_byte=0,lm75_low_byte=0;
    CHAR8U LM75_8U;

    CHAR8S temp;
    INT16S LM75_16S;
    FLOAT  LM75_Final_Value;

            i2c_stop();
            busy = i2c_start();
            if(busy)
            {
                ack = i2c_write(LM75_S_ADDRESS | WRITE_BIT);
                if(ack)
                  {
                    ack = i2c_write(0x00); //read temperature , continous 2 byte data
                    if(ack)
                      {
                        busy = i2c_start();
                         if(busy)
                           {
                            ack = i2c_write(LM75_S_ADDRESS | READ_BIT);
                             if(ack)
                             {
                                 //read 2byte data of temperature
                                 lm75_high_byte = i2c_read(MASTER_ACK);
                                 lm75_low_byte  = i2c_read(MASTER_NACK);
                             }
                             else
                             {
                                 goto exit;
                             }
                           }
                           else
                           {
                            goto exit;
                           }
                      }
                      else
                      {
                        goto exit;
                      }
                  }
                  else
                  {
                    goto exit;
                  }
            }
            else
            {
                goto exit;
            }


             //calculate LM75 temperature
            //---------------------------------------------------------
             LM75_8U = (lm75_high_byte << 1) + (lm75_low_byte >>7);

            if( (lm75_high_byte & 0x80) == 0x80)
            {
                //if Temperature is negative
                temp = (~LM75_8U)+1;
                LM75_16S = - (INT16S)(temp);
            }
            else
            {   //if Temperature is positive
                LM75_16S = LM75_8U;

            }
               //unit step:1 ( 0.5 oC )
               LM75_Final_Value = ( (FLOAT)LM75_16S ) * 0.5;
            //---------------------------------------------------------

            // LM75 read Temperature Success;
            *LM75_Temperature = LM75_Final_Value;
            return 0;



            //LM75 read Temperature Fail
      exit:

            i2c_stop();
            delay_us(30);
            *LM75_Temperature = 0;
            return -1;

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
CHAR8S LM75_CONFIG_CONTROL(CHAR8U LM75_S_ADDRESS,CHAR8U RW_BIT,CHAR8U *LM75_Configure)
{
    /*
     SWI2C_STATUS_OK      1
     SWI2C_STATUS_FAIL   -1

     RW_BIT = 1  -> read configure
     RW_BIT = 0  -> write configure
    */

     CHAR8S check_flag = 0;
     CHAR8U DATA=0;

    if(RW_BIT==1) //read configure
      {

        check_flag = i2c_read_1_byte_data(LM75_S_ADDRESS,0x01,&DATA);
         if(check_flag == SWI2C_STATUS_OK)
          {
            *LM75_Configure = DATA;
            return 0;

          }
          else
          {
            *LM75_Configure = 0;
            return -1;
          }
      }
     else if(RW_BIT==0)
      {
        DATA = *LM75_Configure;
        check_flag = i2c_write_1_byte_data(LM75_S_ADDRESS,0x01,DATA);
         if(check_flag == SWI2C_STATUS_OK)
          {
            return 0;

          }
          else
          {
            return -1;
          }

      }
     else
     {
      return -1;  // RW_BIT set error
     }


}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
CHAR8S LM75_THYST_TOS_CONRTOL(CHAR8U LM75_S_ADDRESS,CHAR8U RW_BIT,CHAR8U THYST_TOS_BIT,FLOAT *LM75_HYOS_Data)
{

    /*
    #define LM75_THYST  0
    #define LM75_TOS   1
    */

    CHAR8U register_address = 0x00;
    CHAR8U busy,ack;

    CHAR8U lm75_high_byte=0,lm75_low_byte=0;
    CHAR8U LM75_8U;

    CHAR8S temp;
    INT16S LM75_16S;
    FLOAT  LM75_Final_Value;


    //check THYST_TOS_BIT
    if(THYST_TOS_BIT == LM75_THYST )register_address = 0x02;          //SET register address ->THYST
    else if (THYST_TOS_BIT == LM75_TOS )register_address = 0x03;      //SET register address ->TOS
    else {
             return -1;
            }

    //********** read THTST or TOS
    if(RW_BIT==1)
    {
            i2c_stop();
            busy = i2c_start();
            if(busy)
            {
                ack = i2c_write(LM75_S_ADDRESS | WRITE_BIT);
                if(ack)
                  {
                    ack = i2c_write(register_address); // 0x02 ->THYST , 0x03 ->TOS
                    if(ack)
                      {
                        busy = i2c_start();
                         if(busy)
                           {
                            ack = i2c_write(LM75_S_ADDRESS | READ_BIT);
                             if(ack)
                             {
                                 //read 2byte data of temperature
                                 lm75_high_byte = i2c_read(MASTER_ACK);
                                 lm75_low_byte  = i2c_read(MASTER_NACK);
                             }
                             else
                             {
                                 goto exit;
                             }
                           }
                           else
                           {
                            goto exit;
                           }
                      }
                      else
                      {
                        goto exit;
                      }
                  }
                  else
                  {
                    goto exit;
                  }
            }
            else
            {
                goto exit;
            }



            //LM 75 read THYST or TOS Scucess
            // STEP 1 (0.5oC)
            //---------------------------------------------------------
            LM75_8U = (lm75_high_byte << 1) + (lm75_low_byte >>7);

            if( (lm75_high_byte & 0x80) == 0x80)
            {
                //if Temperature is negative
                temp = (~LM75_8U)+1;
                LM75_16S = - (INT16S)(temp);
            }
            else
            {   //if Temperature is positive
                LM75_16S = LM75_8U;

            }
               //unit step:1 ( 0.5 oC )
               LM75_Final_Value = ( (FLOAT)LM75_16S ) * 0.5;
            //---------------------------------------------------------

            // LM75 read Temperature Success;
            *LM75_HYOS_Data = LM75_Final_Value;
            return 0;



      //LM75 read  THTST or TOS fail
      exit:

            i2c_stop();
            delay_us(30);
            *LM75_HYOS_Data = 0;
            return -1;
    }

    else if(RW_BIT==0)      //********** read THTST or TOS **********
    {

            //LM 75 write THYST or TOS Scucess
            // STEP 1 (0.5oC)

            LM75_Final_Value  = *LM75_HYOS_Data;

            LM75_16S = ((INT16S)LM75_Final_Value) * 2;

      //      printf("high 0x%X , low %d\r\n",LM75_16S,LM75_16S ) ;

               if( ( LM75_16S & 0x8000) == 0x8000)
            {
               lm75_high_byte = 0X80;
               lm75_high_byte |= (CHAR8U) ((LM75_16S & 0x00FF)>>1);
               lm75_low_byte =(CHAR8U)((LM75_16S & 0x00FF)<<7);

        //       printf("high %d , low%d\r\n",lm75_high_byte,lm75_low_byte);
        //       printf("high 0x%X , low 0x%x\r\n",lm75_high_byte,lm75_low_byte);

            }
            else
            {
               lm75_high_byte = 0X00;
               lm75_high_byte |= (CHAR8U) ((LM75_16S & 0x00FF)>>1);
               lm75_low_byte =(CHAR8U)((LM75_16S & 0x00FF)<<7);

         //      printf("high %d , low%d\r\n",lm75_high_byte,lm75_low_byte);
          //     printf("high 0x%X , low 0x%x\r\n",lm75_high_byte,lm75_low_byte);
            }
       /*
        SWI2C_STATUS_OK      1
        SWI2C_STATUS_FAIL   -1                           LM75_S_ADDRESS | WRITE_BIT
       */

               i2c_stop();
                    busy = i2c_start();
                    if(busy)
                    {
                        ack = i2c_write(LM75_S_ADDRESS | WRITE_BIT);
                        if(ack)
                          {
                            ack = i2c_write(register_address); // 0x02 ->THYST , 0x03 ->TOS
                            if(ack)
                              {
                                ack = i2c_write(lm75_high_byte);
                                  if(ack)
                                    {
                                     ack = i2c_write(lm75_low_byte);
                                      if(ack)
                                           {
                                             i2c_stop();
                                             delay_us(10);
                                             return 0;         //wtite data(THYST or TOS) finish!!
                                           }
                                      else
                                           {
                                             goto exit_2;
                                           }

                                    }
                                  else
                                    {
                                     goto exit_2;
                                    }

                              }
                          }
                          else
                          {
                            goto exit_2;
                          }
                    }
                    else
                    {
                        goto exit_2;
                    }


      //LM75 write THTST or TOS fail
       exit_2:

            i2c_stop();
            delay_us(30);
            *LM75_HYOS_Data = 0;
            return -1;
    }
    else
		{
		 return -1;
		}
}
//--------------------------------------------------------------------------------------------------
#endif		//#ifndef __LM75_FUNCTION__