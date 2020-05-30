/*-----------------------------------------------------------------------
     Creator :Morris chiou
     Sensor      : LM75 temperature sensor
     File Name   : EXAMPLE_LM75.c
     Function    : EXAMPLE_LM75
     Create Date : 2017/04/29
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <alcd.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <i2c.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_LM75.h>


void EXAMPLE_LM75(void);


void EXAMPLE_LM75(void)
{

	FLOAT LM75_Temperature;
	CHAR8S check_flag_LM75 = 0;
	CHAR8U LM75_Configure = 0;
	FLOAT  LM75_HYOS_Data = 0;
	CHAR8U LM75_HYOS_BIT = 0;
	CHAR8U lcd_char_data[3][4]={0}; 
		
               /* LM75 Sensor ; */
		printf("-------------------- LM75 --------------------\r\n");

	  	i2c_stop_hang();
		check_flag_LM75 = LM75_READ_TEMPERATURE(LM75_SLAVE_ADDRESS,&LM75_Temperature);
		i2c_stop_hang();
			
		if(check_flag_LM75 == 0)
		{
			printf("Temperature : %f C\r\n",LM75_Temperature);

			/* Temperature Format : XXX.X C  */ 
			lcd_char_data[0][0]=(INT32U)(LM75_Temperature/100)%10;
			lcd_char_data[0][1]=(INT32U)(LM75_Temperature/10)%10;   
			lcd_char_data[0][2]=(INT32U)(LM75_Temperature)%10;  
			lcd_char_data[0][3]=(INT32U)(LM75_Temperature*10)%10;   
			                     
			/* SHOW Temperature DATA */                    
			lcd_gotoxy(0,0);
			lcd_putsf("T:");     
			/* show Temperature data on LCD */
			lcd_putchar(48+lcd_char_data[0][0]);
			lcd_putchar(48+lcd_char_data[0][1]);
			lcd_putchar(48+lcd_char_data[0][2]);
			lcd_putchar(46);	/* show "."*/
			lcd_putchar(48+lcd_char_data[0][3]);
			lcd_putchar(32);	/* show " "*/	  
			lcd_putchar(67);	/* show "C"*/			  
									
		}
		else 		
		{
			printf("LM75 Read Temperature error\r\n");
		}
					

		LM75_Configure = 0x18; /*Set LM75 config */
		check_flag_LM75 = LM75_CONFIG_CONTROL(LM75_SLAVE_ADDRESS,LM75_WRITE,&LM75_Configure);
		check_flag_LM75 = LM75_CONFIG_CONTROL(LM75_SLAVE_ADDRESS,LM75_READ,&LM75_Configure);
		i2c_stop_hang();
              if(check_flag_LM75 == 0 )
		{
			printf("Configure : 0x%X\r\n",LM75_Configure);	
		}
		else 
		{
			printf("LM75 Read Configure error\r\n");
		}
				
		/*SET THYST*/
		LM75_HYOS_Data =60;
		check_flag_LM75 = LM75_THYST_TOS_CONRTOL(LM75_SLAVE_ADDRESS,LM75_WRITE,LM75_THYST,&LM75_HYOS_Data);
		i2c_stop_hang();
		/*SET TOS*/
		LM75_HYOS_Data =80;
		check_flag_LM75 = LM75_THYST_TOS_CONRTOL(LM75_SLAVE_ADDRESS,LM75_WRITE,LM75_TOS,&LM75_HYOS_Data);
		i2c_stop_hang();

		check_flag_LM75 = LM75_THYST_TOS_CONRTOL(LM75_SLAVE_ADDRESS,LM75_READ,LM75_THYST,&LM75_HYOS_Data);
		i2c_stop_hang();
		if(check_flag_LM75 == 0 && LM75_HYOS_BIT == 0 ) 
		{
			printf("T HYST : %f C\r\n",LM75_HYOS_Data);

			//THYST Format : XXX.X C   
			lcd_char_data[1][0]=(INT32U)(LM75_HYOS_Data/100)%10;
			lcd_char_data[1][1]=(INT32U)(LM75_HYOS_Data/10)%10;   
			lcd_char_data[1][2]=(INT32U)(LM75_HYOS_Data)%10;  
			lcd_char_data[1][3]=(INT32U)(LM75_HYOS_Data*10)%10;   
									                     
			/* SHOW THYST DATA */                    
			lcd_gotoxy(0,1);
			lcd_putsf("THYST:");     
			/* show THYST data on LCD */
			lcd_putchar(48+lcd_char_data[1][0]);
			lcd_putchar(48+lcd_char_data[1][1]);
			lcd_putchar(48+lcd_char_data[1][2]);
			lcd_putchar(46);	/* show "."*/				  
			lcd_putchar(48+lcd_char_data[1][3]);
			lcd_putchar(32);	/* show " "*/	  
			lcd_putchar(67);	/* show "C"*/																  
		}					
		else
		{
			printf("LM75 Read Configure error\r\n");			
		}

				
		check_flag_LM75 = LM75_THYST_TOS_CONRTOL(LM75_SLAVE_ADDRESS,LM75_READ,LM75_TOS,&LM75_HYOS_Data);
		i2c_stop_hang();
		if(check_flag_LM75 == 0 && LM75_HYOS_BIT == 0 ) 
		{
			printf("T OS : %f C\r\n",LM75_HYOS_Data);

			//TOS Format : XXX.X C   
			lcd_char_data[2][0]=(INT32U)(LM75_HYOS_Data/100)%10;
			lcd_char_data[2][1]=(INT32U)(LM75_HYOS_Data/10)%10;   
			lcd_char_data[2][2]=(INT32U)(LM75_HYOS_Data)%10;  
			lcd_char_data[2][3]=(INT32U)(LM75_HYOS_Data*10)%10;   
									                     
			/* SHOW THYST DATA */                    
			lcd_gotoxy(0,2);
			lcd_putsf("TOS:");     
			/* show THYST data on LCD */
			lcd_putchar(48+lcd_char_data[2][0]);
			lcd_putchar(48+lcd_char_data[2][1]);
			lcd_putchar(48+lcd_char_data[2][2]);
			lcd_putchar(46);	/* show "."*/								 
			lcd_putchar(48+lcd_char_data[2][3]);
			lcd_putchar(32);	/* show " "*/	  
			lcd_putchar(67);	/* show "C"*/						
															
		}
		else 
		{
			printf("LM75 Read Configure error\r\n");
		}

		delay_ms(500);	/*read LM75 period 500mS*/
				

		printf("-------------------- LM75 --------------------\r\n");
}
