/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: 3 - Axis Accelerometer 
     File Name		: SENSOR_KXSD9.h
     Function		: SENSOR_KXSD9
     Create Date	: 2017/07/12
---------------------------------------------------------------------- */

#ifndef __KXSD9_EXAMPLE__
#define __KXSD9_EXAMPLE__

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <alcd.h>

#include "SENSOR_KXSD9.h"
#include "Porting_Layer.h"


//------- Define KXSD9 variable ------
//if = 1 , read KXSD9 inifite .
//if = 0 , read once . 
#define EXAMPLE_KXSD9_INIFITE_READ 	1

KXSD9_AXIS KXSD9_DATA;	// X,Y,Z,AUXOUT
//------- Define KXSD9 variable ------

void  EXAMPLE_KXSD9(void);

void  EXAMPLE_KXSD9(void)
{
		CHAR8S status = 0;
    		CHAR8U lcd_char_data[4][4]={0}; 

			
               printf("-------------------- 3 - Axis Accelerometer  KXSD9 --------------------\r\n");
              // 3 - Axis Accelerometer  KXSD9 ;
		status = KXSD9_INIT();
		switch(status)
			{
			case 0:
					printf("initial ok!!\r\n");
					break;
						
			case -1:
					printf("reset fail \r\n");
					break;
					
			case -2:
					printf("set low pass fail\r\n");
					break;
					
			case -3:
					printf("set FS , MOTlen , MOTlat  fail \r\n");
					break;
					
			case -4:
					printf("set mode & disable CLKhld  fail \r\n");
					break;
					
			case -5:
					printf("set MOTlen fail \r\n");
					break;								
			}


#if EXAMPLE_KXSD9_INIFITE_READ == 1
		while(1)
			{
#endif
			status = KXSD9_READ_AXIS(&KXSD9_DATA);
			if(status ==0 )
						{
						printf("read XYZ,AUXOUT success !!\r\n");	
						printf("X axis 	data = %d \r\n",KXSD9_DATA.X_AXIS);							
						printf("Y axis 	data = %d \r\n",KXSD9_DATA.Y_AXIS);		
						printf("Z axis 	data = %d \r\n",KXSD9_DATA.Z_AXIS);		
						printf("AUXOUT  data = %d \r\n",KXSD9_DATA.AUXOUT);			



			                     // X AXIS   
			                     lcd_char_data[0][0]=(INT32U)(KXSD9_DATA.X_AXIS/1000)%10;
			                     lcd_char_data[0][1]=(INT32U)(KXSD9_DATA.X_AXIS/100)%10;   
			                     lcd_char_data[0][2]=(INT32U)(KXSD9_DATA.X_AXIS/10)%10;  
			                     lcd_char_data[0][3]=(INT32U)(KXSD9_DATA.X_AXIS)%10;   
			                     
			                     // Y AXIS   
			                     lcd_char_data[1][0]=(INT32U)(KXSD9_DATA.Y_AXIS/1000)%10;
			                     lcd_char_data[1][1]=(INT32U)(KXSD9_DATA.Y_AXIS/100)%10;   
			                     lcd_char_data[1][2]=(INT32U)(KXSD9_DATA.Y_AXIS/10)%10;  
			                     lcd_char_data[1][3]=(INT32U)(KXSD9_DATA.Y_AXIS)%10;         
			                     
			                     
			                     // Z AXIS   
			                     lcd_char_data[2][0]=(INT32U)(KXSD9_DATA.Z_AXIS/1000)%10;
			                     lcd_char_data[2][1]=(INT32U)(KXSD9_DATA.Z_AXIS/100)%10;   
			                     lcd_char_data[2][2]=(INT32U)(KXSD9_DATA.Z_AXIS/10)%10;  
			                     lcd_char_data[2][3]=(INT32U)(KXSD9_DATA.Z_AXIS)%10;         



		                     	// AUXOUT AXIS   
			                     lcd_char_data[3][0]=(INT32U)(KXSD9_DATA.AUXOUT/1000)%10;
			                     lcd_char_data[3][1]=(INT32U)(KXSD9_DATA.AUXOUT/100)%10;   
			                     lcd_char_data[3][2]=(INT32U)(KXSD9_DATA.AUXOUT/10)%10;  
			                     lcd_char_data[3][3]=(INT32U)(KXSD9_DATA.AUXOUT)%10;         



					   	/* SHOW X axis DATA */                    
			            		lcd_gotoxy(0,0);
			            		lcd_putsf("X axis:");     
						/* show X axis data on LCD */
				              lcd_putchar(48+lcd_char_data[0][0]);
				              lcd_putchar(48+lcd_char_data[0][1]);
				              lcd_putchar(48+lcd_char_data[0][2]);
				              lcd_putchar(48+lcd_char_data[0][3]);

								
						/* SHOW Y axis DATA */    
			          		lcd_gotoxy(0,1);
			           		lcd_putsf("Y axis:"); 
						/* show Y axis data on LCD */
			                 	lcd_putchar(48+lcd_char_data[1][0]);
			                	lcd_putchar(48+lcd_char_data[1][1]);
			                 	lcd_putchar(48+lcd_char_data[1][2]);
			                 	lcd_putchar(48+lcd_char_data[1][3]);
			                    
			  
						/* SHOW Z axis DATA */    
			            		lcd_gotoxy(0,2);
			            		lcd_putsf("Z axis:"); 					   
						/* show Z axis data on LCD */
			                 	lcd_putchar(48+lcd_char_data[2][0]);
			                 	lcd_putchar(48+lcd_char_data[2][1]);
			                 	lcd_putchar(48+lcd_char_data[2][2]);
			                 	lcd_putchar(48+lcd_char_data[2][3]);
		                    
			  
						/* SHOW AUXOUT  DATA */    
			            		lcd_gotoxy(0,3);
			            		lcd_putsf("AUXOUT :"); 					   
						/* show AUXOUT data on LCD */
			                 	lcd_putchar(48+lcd_char_data[3][0]);
			                 	lcd_putchar(48+lcd_char_data[3][1]);
			                 	lcd_putchar(48+lcd_char_data[3][2]);
			                 	lcd_putchar(48+lcd_char_data[3][3]);
		                    
						
						}
			else  printf("read data fail !! \r\n");
			
			delay_ms(100);			//delay 100 ms 
			
#if EXAMPLE_KXSD9_INIFITE_READ == 1
			}
#endif
               printf("-------------------- 3 - Axis Accelerometer  KXSD9 --------------------\r\n");
	  
}


#endif		//#ifndef __KXSD9_EXAMPLE__
