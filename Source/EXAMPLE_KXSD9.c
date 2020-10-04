/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: 3 - Axis Accelerometer 
     File Name		: SENSOR_KXSD9.h
     Function		: SENSOR_KXSD9
     Create Date	: 2017/07/12
---------------------------------------------------------------------- */

#include <mega32a.h>
#include <stdio.h>
#include <delay.h>
#include <math.h>
#include <stdlib.h>
#include <alcd.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_KXSD9.h>


#define KXSD9_GET_MAX_VALUE			(0)			/* rotate KXSD9 & get the Xmax,Xmin ; Ymax,Ymin ; Zmax,Zmin */
#define KXSD9_SHOW_RAW_OR_ANGLE	(0)			/* if set "1" -> show x,y,z axis raw data ; set "0" -> show x,y,z axis angle */
#define KXSD9_TEMPERATURE_COMP		(1)			/* if set "1" -> the angle with temperautre compensation ; set "0" -> the angle without temperautre compensation */


/*------- Define KXSD9 variable ------*/
KXSD9_AXIS 			KXSD9_DATA;	/*X,Y,Z,AUXOUT*/
KXSD9_3AXIS_ANGLE	KXSD9_ANGLE;

#if (KXSD9_GET_MAX_VALUE==1)
KXSD9_AXIS KXSD9_MAX_POS,KXSD9_MAX_NEG;
CHAR8U lcd_char_data_pos[3][4]={0}; 
CHAR8U lcd_char_data_neg[3][4]={0}; 
#endif

/*------- Define KXSD9 variable ------*/

void  EXAMPLE_KXSD9(void);

void  EXAMPLE_KXSD9(void)
{
	CHAR8S status = 0;
    	CHAR8U lcd_char_data[4][4]={0}; 
	INT16S temperature ;
    	CHAR8U getmax_char_data[2][4]={0}; 	

		
		printf("-------------------- 3 - Axis Accelerometer  KXSD9 --------------------\r\n");
              /* 3 - Axis Accelerometer  KXSD9 ;*/
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



	/* get the max data for each axis */
	#if (KXSD9_GET_MAX_VALUE==1)
		while(1)
		{
			status = KXSD9_READ_AXIS(&KXSD9_DATA);	/* get the X,Y,Z-axis raw data */
			if(status ==0)
			{
				
				/* calculate the max raw data */
				KXSD9_GET_3AXIS_MAX(&KXSD9_MAX_POS,&KXSD9_MAX_NEG,KXSD9_DATA);
				{
					/*X POS MAX AXIS*/   
					lcd_char_data_pos[0][0]=(INT32U)(KXSD9_MAX_POS.X_AXIS/1000)%10;
					lcd_char_data_pos[0][1]=(INT32U)(KXSD9_MAX_POS.X_AXIS/100)%10;   
					lcd_char_data_pos[0][2]=(INT32U)(KXSD9_MAX_POS.X_AXIS/10)%10;  
					lcd_char_data_pos[0][3]=(INT32U)(KXSD9_MAX_POS.X_AXIS)%10;   
					                     
					/*Y POS MAX AXIS*/   
					lcd_char_data_pos[1][0]=(INT32U)(KXSD9_MAX_POS.Y_AXIS/1000)%10;
					lcd_char_data_pos[1][1]=(INT32U)(KXSD9_MAX_POS.Y_AXIS/100)%10;   
					lcd_char_data_pos[1][2]=(INT32U)(KXSD9_MAX_POS.Y_AXIS/10)%10;  
					lcd_char_data_pos[1][3]=(INT32U)(KXSD9_MAX_POS.Y_AXIS)%10;         		                     
					                     
					/*Z POS MAX AXIS*/   
					lcd_char_data_pos[2][0]=(INT32U)(KXSD9_MAX_POS.Z_AXIS/1000)%10;
					lcd_char_data_pos[2][1]=(INT32U)(KXSD9_MAX_POS.Z_AXIS/100)%10;   
					lcd_char_data_pos[2][2]=(INT32U)(KXSD9_MAX_POS.Z_AXIS/10)%10;  
					lcd_char_data_pos[2][3]=(INT32U)(KXSD9_MAX_POS.Z_AXIS)%10;         
				}

				{
					/*X NEG MAX AXIS*/
					lcd_char_data_neg[0][0]=(INT32U)(KXSD9_MAX_NEG.X_AXIS/1000)%10;
					lcd_char_data_neg[0][1]=(INT32U)(KXSD9_MAX_NEG.X_AXIS/100)%10;   
					lcd_char_data_neg[0][2]=(INT32U)(KXSD9_MAX_NEG.X_AXIS/10)%10;  
					lcd_char_data_neg[0][3]=(INT32U)(KXSD9_MAX_NEG.X_AXIS)%10;   
					                     
					/*Y NEG MAX AXIS*/   
					lcd_char_data_neg[1][0]=(INT32U)(KXSD9_MAX_NEG.Y_AXIS/1000)%10;
					lcd_char_data_neg[1][1]=(INT32U)(KXSD9_MAX_NEG.Y_AXIS/100)%10;   
					lcd_char_data_neg[1][2]=(INT32U)(KXSD9_MAX_NEG.Y_AXIS/10)%10;  
					lcd_char_data_neg[1][3]=(INT32U)(KXSD9_MAX_NEG.Y_AXIS)%10;         		                     
					                     
					/*Z NEG MAX AXIS*/   
					lcd_char_data_neg[2][0]=(INT32U)(KXSD9_MAX_NEG.Z_AXIS/1000)%10;
					lcd_char_data_neg[2][1]=(INT32U)(KXSD9_MAX_NEG.Z_AXIS/100)%10;   
					lcd_char_data_neg[2][2]=(INT32U)(KXSD9_MAX_NEG.Z_AXIS/10)%10;  
					lcd_char_data_neg[2][3]=(INT32U)(KXSD9_MAX_NEG.Z_AXIS)%10;         
				}


				{
					/* SHOW X axis POS MAX DATA */                    
					lcd_gotoxy(0,0);
					lcd_putsf("Xp:");     
					/* show X axis data on LCD */
					lcd_putchar(48+lcd_char_data_pos[0][0]);
					lcd_putchar(48+lcd_char_data_pos[0][1]);
					lcd_putchar(48+lcd_char_data_pos[0][2]);
					lcd_putchar(48+lcd_char_data_pos[0][3]);
					
					/* SHOW X axis NEG MAX DATA */                    
					lcd_gotoxy(8,0);
					lcd_putsf("Xn:");     
					/* show X axis data on LCD */
					lcd_putchar(48+lcd_char_data_neg[0][0]);
					lcd_putchar(48+lcd_char_data_neg[0][1]);
					lcd_putchar(48+lcd_char_data_neg[0][2]);
					lcd_putchar(48+lcd_char_data_neg[0][3]);
				}


				{
					/* SHOW Y axis POS MAX DATA */    
					lcd_gotoxy(0,1);
					lcd_putsf("Yp:"); 
					/* show Y axis data on LCD */
					lcd_putchar(48+lcd_char_data_pos[1][0]);
					lcd_putchar(48+lcd_char_data_pos[1][1]);
					lcd_putchar(48+lcd_char_data_pos[1][2]);
					lcd_putchar(48+lcd_char_data_pos[1][3]);

					/* SHOW Y axis NEG MAX DATA */    
					lcd_gotoxy(8,1);
					lcd_putsf("Yn:"); 
					/* show Y axis data on LCD */
					lcd_putchar(48+lcd_char_data_neg[1][0]);
					lcd_putchar(48+lcd_char_data_neg[1][1]);
					lcd_putchar(48+lcd_char_data_neg[1][2]);
					lcd_putchar(48+lcd_char_data_neg[1][3]);
				}	                    
				  

								
				{
					/* SHOW Z axis POS MAX DATA */    
					lcd_gotoxy(0,2);
					lcd_putsf("Zp:"); 					   
					/* show Z axis data on LCD */
					lcd_putchar(48+lcd_char_data_pos[2][0]);
					lcd_putchar(48+lcd_char_data_pos[2][1]);
					lcd_putchar(48+lcd_char_data_pos[2][2]);
					lcd_putchar(48+lcd_char_data_pos[2][3]);

					/* SHOW Z axis NEG MAX DATA */    
					lcd_gotoxy(8,2);
					lcd_putsf("Zn:"); 					   
					/* show Z axis data on LCD */
					lcd_putchar(48+lcd_char_data_neg[2][0]);
					lcd_putchar(48+lcd_char_data_neg[2][1]);
					lcd_putchar(48+lcd_char_data_neg[2][2]);
					lcd_putchar(48+lcd_char_data_neg[2][3]);
				}

				delay_ms(80);	/* tiny delay */
			}
					
		}
#endif

		while(1)
		{
			/* read the axis data */		
			status = KXSD9_READ_AXIS(&KXSD9_DATA);
			if(status ==0)
			{

		#if (KXSD9_SHOW_RAW_OR_ANGLE==1)
		
				printf("read XYZ,AUXOUT success !!\r\n");	
				printf("X axis 	data = %d \r\n",KXSD9_DATA.X_AXIS);							
				printf("Y axis 	data = %d \r\n",KXSD9_DATA.Y_AXIS);		
				printf("Z axis 	data = %d \r\n",KXSD9_DATA.Z_AXIS);		
				printf("AUXOUT  data = %d \r\n",KXSD9_DATA.AUXOUT);			


				/*X AXIS*/   
				lcd_char_data[0][0]=(INT32U)(KXSD9_DATA.X_AXIS/1000)%10;
				lcd_char_data[0][1]=(INT32U)(KXSD9_DATA.X_AXIS/100)%10;   
				lcd_char_data[0][2]=(INT32U)(KXSD9_DATA.X_AXIS/10)%10;  
				lcd_char_data[0][3]=(INT32U)(KXSD9_DATA.X_AXIS)%10;   
			                     
				/*Y AXIS*/   
				lcd_char_data[1][0]=(INT32U)(KXSD9_DATA.Y_AXIS/1000)%10;
				lcd_char_data[1][1]=(INT32U)(KXSD9_DATA.Y_AXIS/100)%10;   
				lcd_char_data[1][2]=(INT32U)(KXSD9_DATA.Y_AXIS/10)%10;  
				lcd_char_data[1][3]=(INT32U)(KXSD9_DATA.Y_AXIS)%10;         		                     
			                     
				/*Z AXIS*/   
				lcd_char_data[2][0]=(INT32U)(KXSD9_DATA.Z_AXIS/1000)%10;
				lcd_char_data[2][1]=(INT32U)(KXSD9_DATA.Z_AXIS/100)%10;   
				lcd_char_data[2][2]=(INT32U)(KXSD9_DATA.Z_AXIS/10)%10;  
				lcd_char_data[2][3]=(INT32U)(KXSD9_DATA.Z_AXIS)%10;         

				/*AUXOUT AXIS*/   
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
				
		#else
		
		#if(KXSD9_TEMPERATURE_COMP==1)	
			
				temperature = 29; 
				/* calculate the 3axis angle degree with temperautre compensation */			
				status=KXSD9_GET_ANG_WITH_TEMPERAUTRE(KXSD9_DATA,&KXSD9_ANGLE,temperature);	
				if(status!=1)
				{
					printf("the KXSD9 is shaking or moving!\r\n");	
				}		
		#else
				/* calculate the 3axis angle degree without temperautre compensation */					
				status=KXSD9_GET_ANG(KXSD9_DATA,&KXSD9_ANGLE);			
				if(status!=1)
				{
					printf("the KXSD9 is shaking or moving!\r\n");	
				}
				
		#endif

				
				printf("read XYZ & calculate the each axis degree success !!\r\n");	
				printf("Xangle = %f \r\n",KXSD9_ANGLE.X_ANGLE);							
				printf("Yangle = %f \r\n",KXSD9_ANGLE.Y_ANGLE);							
				printf("Zangle = %f \r\n",KXSD9_ANGLE.Z_ANGLE);							
						
							
				/* --------- x_degree bolck --------- */
				if(KXSD9_ANGLE.X_ANGLE<0)/* x_degree < 0 */
				{				
					/* Temperautre */
					lcd_char_data[0][0] = (CHAR8U)(abs(KXSD9_ANGLE.X_ANGLE/100))%10;	/* use abs() */
					lcd_char_data[0][1] = (CHAR8U)(abs(KXSD9_ANGLE.X_ANGLE/10))%10;		/* use abs() */
					lcd_char_data[0][2] = (CHAR8U)(abs(KXSD9_ANGLE.X_ANGLE))%10;		/* use abs() */  
					lcd_char_data[0][3] = (CHAR8U)(abs(KXSD9_ANGLE.X_ANGLE*10))%10;		/* use abs() */   
					lcd_char_data[0][4] = (CHAR8U)(abs(KXSD9_ANGLE.X_ANGLE*100))%10; 	/* use abs() */  		
				}
				else /* x_degree >= 0 */
				{
					/* x_degree */
					lcd_char_data[0][0] = (CHAR8U)(KXSD9_ANGLE.X_ANGLE/100)%10;     
					lcd_char_data[0][1] = (CHAR8U)(KXSD9_ANGLE.X_ANGLE/10)%10;
					lcd_char_data[0][2] = (CHAR8U)(KXSD9_ANGLE.X_ANGLE)%10;  
					lcd_char_data[0][3] = (CHAR8U)(KXSD9_ANGLE.X_ANGLE*10)%10;   
					lcd_char_data[0][4] = (CHAR8U)(KXSD9_ANGLE.X_ANGLE*100)%10;   						
				}
				
				/* --------- y_degree bolck --------- */
				if(KXSD9_ANGLE.Y_ANGLE<0)/* y_degree < 0 */
				{				
					/* y_degree */
					lcd_char_data[1][0] = (CHAR8U)(abs(KXSD9_ANGLE.Y_ANGLE/100))%10;	/* use abs() */
					lcd_char_data[1][1] = (CHAR8U)(abs(KXSD9_ANGLE.Y_ANGLE/10))%10;		/* use abs() */
					lcd_char_data[1][2] = (CHAR8U)(abs(KXSD9_ANGLE.Y_ANGLE))%10;			/* use abs() */  
					lcd_char_data[1][3] = (CHAR8U)(abs(KXSD9_ANGLE.Y_ANGLE*10))%10;		/* use abs() */   
					lcd_char_data[1][4] = (CHAR8U)(abs(KXSD9_ANGLE.Y_ANGLE*100))%10; 	/* use abs() */  		
				}
				else /* y_degree >= 0 */
				{
					/* y_degree */
					lcd_char_data[1][0] = (CHAR8U)(KXSD9_ANGLE.Y_ANGLE/100)%10;     
					lcd_char_data[1][1] = (CHAR8U)(KXSD9_ANGLE.Y_ANGLE/10)%10;
					lcd_char_data[1][2] = (CHAR8U)(KXSD9_ANGLE.Y_ANGLE)%10;  
					lcd_char_data[1][3] = (CHAR8U)(KXSD9_ANGLE.Y_ANGLE*10)%10;   
					lcd_char_data[1][4] = (CHAR8U)(KXSD9_ANGLE.Y_ANGLE*100)%10;   						
				}
			                     
				/* --------- z_degree bolck --------- */
				if(KXSD9_ANGLE.Z_ANGLE<0)/* z_degree < 0 */
				{				
					/* z_degree */
					lcd_char_data[2][0] = (CHAR8U)(abs(KXSD9_ANGLE.Z_ANGLE/100))%10;	/* use abs() */
					lcd_char_data[2][1] = (CHAR8U)(abs(KXSD9_ANGLE.Z_ANGLE/10))%10;		/* use abs() */
					lcd_char_data[2][2] = (CHAR8U)(abs(KXSD9_ANGLE.Z_ANGLE))%10;		/* use abs() */  
					lcd_char_data[2][3] = (CHAR8U)(abs(KXSD9_ANGLE.Z_ANGLE*10))%10;		/* use abs() */   
					lcd_char_data[2][4] = (CHAR8U)(abs(KXSD9_ANGLE.Z_ANGLE*100))%10; 	/* use abs() */  		
				}
				else /* z_degree >= 0 */
				{
					/* z_degree */
					lcd_char_data[2][0] = (CHAR8U)(KXSD9_ANGLE.Z_ANGLE/100)%10;     
					lcd_char_data[2][1] = (CHAR8U)(KXSD9_ANGLE.Z_ANGLE/10)%10;
					lcd_char_data[2][2] = (CHAR8U)(KXSD9_ANGLE.Z_ANGLE)%10;  
					lcd_char_data[2][3] = (CHAR8U)(KXSD9_ANGLE.Z_ANGLE*10)%10;   
					lcd_char_data[2][4] = (CHAR8U)(KXSD9_ANGLE.Z_ANGLE*100)%10;   						
				}




				/* SHOW X axis DATA */                    
				lcd_gotoxy(0,0);
				lcd_putsf("Xang:");     
	           	 	if(KXSD9_ANGLE.X_ANGLE<0)
	            		{
	            			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	           		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   			
				/* show X axis data on LCD */
				lcd_putchar(48+lcd_char_data[0][0]);
				lcd_putchar(48+lcd_char_data[0][1]);
				lcd_putchar(48+lcd_char_data[0][2]);
				lcd_putchar(46);    /* print "."  */					
				lcd_putchar(48+lcd_char_data[0][3]);
				lcd_putchar(48+lcd_char_data[0][4]);


				/* SHOW Y axis DATA */                    
				lcd_gotoxy(0,1);
				lcd_putsf("Yang:");     
	           	 	if(KXSD9_ANGLE.Y_ANGLE<0)
	            		{
	            			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	           		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   				
				/* show Y axis data on LCD */
				lcd_putchar(48+lcd_char_data[1][0]);
				lcd_putchar(48+lcd_char_data[1][1]);
				lcd_putchar(48+lcd_char_data[1][2]);
				lcd_putchar(46);    /* print "."  */					
				lcd_putchar(48+lcd_char_data[1][3]);
				lcd_putchar(48+lcd_char_data[1][4]);



				/* SHOW z axis DATA */                    
				lcd_gotoxy(0,2);
				lcd_putsf("Zang:");     
	           	 	if(KXSD9_ANGLE.Z_ANGLE<0)
	            		{
	            			lcd_putchar(45);		/* LCD show "-"  mean negative */
				}                             
	           		else
				{
					lcd_putchar(32);		/* LCD show " "  mean positive */
				}   			
				/* show Z axis data on LCD */
				lcd_putchar(48+lcd_char_data[2][0]);
				lcd_putchar(48+lcd_char_data[2][1]);
				lcd_putchar(48+lcd_char_data[2][2]);
				lcd_putchar(46);    /* print "."  */					
				lcd_putchar(48+lcd_char_data[2][3]);
				lcd_putchar(48+lcd_char_data[2][4]);

	

		#endif
						
			}
			else 
			{
				printf("read data fail !! \r\n");
			}
			
			delay_ms(100);	/*delay 100 ms */
			

		}

               printf("-------------------- 3 - Axis Accelerometer  KXSD9 --------------------\r\n");
	  
}
