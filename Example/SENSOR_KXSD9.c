/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: 3 - Axis Accelerometer 
     File Name		: SENSOR_KXSD9.c
     Function		: SENSOR_KXSD9
     Create Date	: 2017/07/12
---------------------------------------------------------------------- */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <delay.h>
#include <datatype_Layer.h>
#include <swi2c_Layer.h>
#include <SENSOR_KXSD9.h>


#define KXSD9_USE_IDEAL_GET_ANGLE				(0)	/*if "1" enable ideal way to calculate the 3-axis angle ; if set "0" disable */
#define KXSD9_USE_ACTUAL_GET_ANGLE				(1)	/*if "1" enable actual way to calculate the 3-axis angle ; if set "0" disable */
#define KXSD9_ENABLE_THE_ROUNDING_CAL			(1)	/*if "1" enable rounding calculate the 3-axis angle ; if set "0" disable */



/********************************************** SYSTEM **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*
	1.reset KXSD9 once  
	2.set the operational bandwidth [LP]  = 1	1	0	[100 Hz]
	3.the full scale sensing range [FS] = 1	1  [+/-2g]
	4.Set MOTLev = 1 & MOTLat = 0
	5.set operation mode -> normal mode
	6.disable CLKhld -> set "0"
	7.Set  MOTIen = 0
	--------------------------
	return ststus
	     0		initial ok!!
	    -1	reset fail
	    -2	set low pass fail
	    -3	set FS , MOTlen , MOTlat  fail
	    -4	set mode & disable CLKhld  fail
	    -5	set MOTlen fail	
	--------------------------
*/
CHAR8S KXSD9_INIT(void)
{
	CHAR8S status = 0; 

		/*1.reset KXSD9 once  */
		status = KXSD9_RESET();
		if(status!=0)
		{
			return -1; /*reset fail*/
		}
		
		delay_ms(5);
		
		/*2.set the operational bandwidth [LP]  = 110[100 Hz]*/
		status = KXSD9_SET_LOW_PASS(KXSD9_LP_100HZ);
		if(status!=0) 
		{
			return -2; /*set low pass fail*/
		}

		delay_ms(1);

		/* 3.the full scale sensing range [FS] = 1	1  [+/-2g]*/
		/* 4.Set MOTLev = 1 & MOTLat = 0*/
		status = KXSD9_SET_FS_MOTLEV_MOTLAT(KXSD9_SET_FS_MOTLEV_LAT);
		if(status!=0) 
		{
			return -3; /*set FS , MOTlen , MOTlat  fail*/
		}

		delay_ms(1);

		/* 5.set operation mode -> normal mode*/
		/* 6.disable CLKhld -> set "0"*/
		status = KXSD9_SET_PWR_MODE(KXSD9_NORMAL_MODE);
		if(status!=0)
		{
			return -4; /*set mode & disable CLKhld  fail*/
		}

		delay_ms(1);
		
		/* 7.Set  MOTIen = 0*/
		status = KXSD9_SET_MOTLEN(KXSD9_DIS_MOTLEN);
		if(status!=0)
		{
			return -5; /*set MOTlen fail*/
		}

		delay_ms(1);

		return 0 ; /* initial ok!!*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*read X,Y,Z,AUXOUT data from*/
CHAR8S KXSD9_READ_AXIS(KXSD9_AXIS *data)
{
	CHAR8S status = 0; 
	CHAR8U read_data[8] = {0};
	
		status = i2c_read_n_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_XOUT_H,8,&read_data[0]);
		if(status!=1) 
		{
			return -1 ; /*read fail*/
		}
		
		data->X_AXIS = (INT16S)( read_data[0]<<4 | (read_data[1]>>4)) ;
		data->Y_AXIS = (INT16S)( read_data[2]<<4 | (read_data[3]>>4)) ;
		data->Z_AXIS = (INT16S)( read_data[4]<<4 | (read_data[5]>>4)) ;
		data->AUXOUT = (INT16S)( read_data[6]<<4 | (read_data[7]>>4)) ;

		return 0 ; /*read success*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*reset KXSD9 -> REG 0X0A write 0xCA.*/
CHAR8S KXSD9_RESET(void)
{
	CHAR8S status = 0; 
	
		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_RESET,0xCA);/*write 0xCA to reset*/
		if(status !=1)
		{
			return -1; /*write fail*/
		}
		return 0; /*write success */
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set the low pass frequency for 0x0C bit 7~5
	 ** use the define 
	KXSD9_LP_NO_FILTER		
	KXSD9_LP_2000HZ		
	KXSD9_LP_1000HZ		
	KXSD9_LP_500HZ	
	KXSD9_LP_100HZ
	KXSD9_LP_50HZ	
*/
CHAR8S KXSD9_SET_LOW_PASS(CHAR8U low_pass_mode)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		/*check low_pass_mode value*/
		if(low_pass_mode != KXSD9_LP_NO_FILTER ||low_pass_mode != KXSD9_LP_2000HZ||\
		low_pass_mode != KXSD9_LP_1000HZ ||low_pass_mode != KXSD9_LP_500HZ ||\
		low_pass_mode != KXSD9_LP_100HZ || low_pass_mode != KXSD9_LP_50HZ) 
		{
			return -1; 	/* error low_pass_mode value.*/
		}

		/*fisrt - load reg 0x0C */
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,&read_data);
		if(status !=1) 
		{
			return -1; 		/*read fail.*/
		}
		
		write_data = read_data & 0x1F; 	/*0x1F mask -> clear bit 7 ~ 5 , remain other bit*/
		write_data|= low_pass_mode ; /* set low pass value*/

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,write_data);
		if(status !=1)
		{
			return -1; 		/*write fail.*/
		}
		return 0 ; /* set low pass filter success!*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set the full scale sensing range of the accelerometer for 0x0c bit1~0 & bit 4 & bit 3*/
/* ** use the define  KXSD9_SET_FS_MOTLEV_LAT*/
CHAR8S KXSD9_SET_FS_MOTLEV_MOTLAT(CHAR8U fs_motlev_motlat)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;
	
		/*didn't check the fs_motlev_motlat value!*/

		/*fisrt - load reg 0x0C*/
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,&read_data);
		if(status !=1) 
		{
			return -1; 		/*read fail.*/
		}
		
		write_data = read_data & 0XE0; 	/* 0xE0 mask -> clear bit 4~0, remain other bit*/
		write_data|= fs_motlev_motlat ;/* set fs , MOTlev , MOTlat*/

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,write_data);
		if(status !=1) 
		{
			return -1;		/*write fail.*/
		}
		
		return 0 ; /* set fs , MOTlev , MOTlat success!*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set enables the motion wakeup feature.  for 0x0D bit 2
	** use the define 
	KXSD9_DIS_MOTLEN
	KXSD9_EN_MOTLEN
*/
CHAR8S KXSD9_SET_MOTLEN(CHAR8U motlen)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		/*check motlen*/
		if(motlen!= KXSD9_DIS_MOTLEN || motlen!= KXSD9_EN_MOTLEN ) 
		{
			return -1; /*error motlen value*/
		}

		/*fisrt - load reg 0x0D */
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,&read_data);
		if(status !=1) 
		{
			return -1; 		/*read fail.*/
		}

		write_data = read_data & 0xFB; 	/* 0xFB mask -> clear bit 2 , remain other bit*/
		write_data|= motlen ; /* set motlen value*/

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,write_data);
		if(status !=1)
		{
			return -1; 		/*write fail.*/
		}
		return 0 ; /* set motlen success!*/

}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set the full scale sensing range of the accelerometer for 0x0D  bit6 &  bit7(disable CLKhld)
	**use  define
	KXSD9_NORMAL_MODE		
	KXSD9_PWN_DOWN_MODE	
*/
CHAR8S KXSD9_SET_PWR_MODE(CHAR8U mode)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		/*check mode*/
		if(mode!= KXSD9_NORMAL_MODE || mode!= KXSD9_PWN_DOWN_MODE )
		{
			return -1; /*error mode value*/
		}
		
		/*fisrt - load reg 0x0D */
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,&read_data);
		if(status !=1) 
		{
			return -1;		/*read fail.*/
		}
		
		write_data = read_data & 0x3F; 		/*0x3F mask -> clear bit 6,7 , remain other bit*/
		write_data|= mode ; 	/* set mode value*/

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,write_data);
		if(status !=1)
		{
			return -1; 		/*write fail.*/
		}
		
		return 0; 	/*set mode success!*/
}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	check MOT pin or REGA[0x0E] bit 1
	MOTI = 1 - a motion wake up event has occurred and the MOT pin (7) is high.
	MOTI = 0 - a motion wake up event has not occurred and the MOT pin (7) is low.

	if return   0 -> motion active
	if return -2 -> not motion active
	if return -1 -> read error
*/
CHAR8S KXSD9_CHK_MOTION(void)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0;

		/*read reg 0x0E*/
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGA,&read_data);
		if(status !=1)
		{
			return -1; /*read fail.*/
		}

		/*check if bit 1 set "1" -> motion occour !*/
		if(read_data & 0x02) 
		{
			return 0;/*motion active*/
		}
		else 
		{
			return -2; /*not motion active*/
		}
}
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM **************************************************/



/********************************************** ADVANCED APPLICATIONS **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the 3axis angle degree without temperautre compensation
*/
CHAR8S KXSD9_GET_ANG(KXSD9_AXIS AXIS_RAW,KXSD9_3AXIS_ANGLE *AXIS_ANGLE)
{
	FLOAT x_raw_acc,y_raw_acc,z_raw_acc;
	FLOAT x_degree,y_degree,z_degree;
	FLOAT check_g;	
	INT16U x_fix,y_fix,z_fix;

	
		/* calibrate the data */ 	

		/* ideal calculate way*/
		#if (KXSD9_USE_IDEAL_GET_ANGLE==1 && KXSD9_USE_ACTUAL_GET_ANGLE==0)
		{
			x_raw_acc=((FLOAT)AXIS_RAW.X_AXIS - KXSD9_IDEAL_X_ZERO_OFFSET)/KXSD9_FS_2G_IDEAL_RESOLUTION;
			
			y_raw_acc=((FLOAT)AXIS_RAW.Y_AXIS - KXSD9_IDEAL_Y_ZERO_OFFSET)/KXSD9_FS_2G_IDEAL_RESOLUTION;
			
			z_raw_acc=((FLOAT)AXIS_RAW.Z_AXIS - KXSD9_IDEAL_Z_ZERO_OFFSET)/KXSD9_FS_2G_IDEAL_RESOLUTION;
		}
		#endif


		/* real calculate way without temperautre compensation */	
		#if (KXSD9_USE_ACTUAL_GET_ANGLE==1 && KXSD9_USE_IDEAL_GET_ANGLE==0)
		{
			/**** formula : acc = get_rawdata /resolution ; unit = g ; 1g = 9.8 m/(s^2) */
			/* this use full-scale 2g . */
			
			/* calculate the X-axis acceleration value */
			if(AXIS_RAW.X_AXIS>= KXSD9_REAL_X_ZERO_OFFSET) /* if X-axis raw data >=KXSD9_REAL_X_ZERO_OFFSET */
			{	
				x_raw_acc=((FLOAT)AXIS_RAW.X_AXIS - KXSD9_REAL_X_ZERO_OFFSET)/KXSD9_FS_2G_REAL_XPOS_RESOLUTION; /*  unit = g*/
			}
			else if(AXIS_RAW.X_AXIS< KXSD9_REAL_X_ZERO_OFFSET) /* if X-axis raw data <KXSD9_REAL_X_ZERO_OFFSET */
			{	
				x_raw_acc=(KXSD9_REAL_X_ZERO_OFFSET-(FLOAT)AXIS_RAW.X_AXIS )/KXSD9_FS_2G_REAL_XNEG_RESOLUTION; /*  unit = g*/
			}

			/* calculate the Y-axis acceleration value */		
			if(AXIS_RAW.Y_AXIS>= KXSD9_REAL_Y_ZERO_OFFSET) /* if Y-axis raw data >=KXSD9_REAL_Y_ZERO_OFFSET */
			{
				y_raw_acc=((FLOAT)AXIS_RAW.Y_AXIS - KXSD9_REAL_Y_ZERO_OFFSET)/KXSD9_FS_2G_REAL_YPOS_RESOLUTION; /*  unit = g*/
			}
			else if(AXIS_RAW.Y_AXIS< KXSD9_REAL_Y_ZERO_OFFSET)/* if Y-axis raw data <KXSD9_REAL_Y_ZERO_OFFSET */
			{ 
				y_raw_acc=(KXSD9_REAL_Y_ZERO_OFFSET-(FLOAT)AXIS_RAW.Y_AXIS)/KXSD9_FS_2G_REAL_YNEG_RESOLUTION; /*  unit = g*/
			}

			/* calculate the Z-axis acceleration value */	
			if(AXIS_RAW.Z_AXIS>= KXSD9_REAL_Z_ZERO_OFFSET) /* if Z-axis raw data >=KXSD9_REAL_Z_ZERO_OFFSET */
			{
				z_raw_acc=((FLOAT)AXIS_RAW.Z_AXIS - KXSD9_REAL_Z_ZERO_OFFSET)/KXSD9_FS_2G_REAL_ZPOS_RESOLUTION; /*  unit = g*/
			}
			else if(AXIS_RAW.Z_AXIS< KXSD9_REAL_Z_ZERO_OFFSET)/* if Z-axis raw data <KXSD9_REAL_Z_ZERO_OFFSET */
			{
				z_raw_acc=(KXSD9_REAL_Z_ZERO_OFFSET-(FLOAT)AXIS_RAW.Z_AXIS)/KXSD9_FS_2G_REAL_ZNEG_RESOLUTION; /*  unit = g */
			}
		}
		
		#endif
		
		/* check the KXSD9 is still? */
		check_g= sqrt(pow(x_raw_acc,2) +  pow(y_raw_acc,2) + pow(z_raw_acc,2));   /* if check_g = KXSD9_G_STILL_MIN ~ KXSD9_G_STILL_MAX is OK */
		if((check_g<KXSD9_G_STILL_MIN) || (check_g > KXSD9_G_STILL_MAX))		/* the KXSD9 is not still */
		{				
			return -1; /* the sensor is not still */
		}
		else  /*the KXSD9 is still */ /* calcualte the angle */	
		{
			/* calculate the X-axis degree */
			AXIS_ANGLE->X_ANGLE=atan(x_raw_acc/sqrt(pow(y_raw_acc,2) + pow(z_raw_acc,2))) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */

			/* calculate the Y-axis degree */
			AXIS_ANGLE->Y_ANGLE=atan(y_raw_acc/sqrt(pow(x_raw_acc,2) + pow(z_raw_acc,2))) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */

			/* calculate the Z-axis degree */
			AXIS_ANGLE->Z_ANGLE=atan(sqrt(pow(x_raw_acc,2) + pow(y_raw_acc,2))/z_raw_acc) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */


			#if (KXSD9_ENABLE_THE_ROUNDING_CAL ==1)		
			if(fabs(AXIS_ANGLE->X_ANGLE) - fabs(floor(AXIS_ANGLE->X_ANGLE)) >0.4)
			{
				AXIS_ANGLE->X_ANGLE = floor(AXIS_ANGLE->X_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->X_ANGLE = floor(AXIS_ANGLE->X_ANGLE);
			}

			if(fabs(AXIS_ANGLE->Y_ANGLE) - fabs(floor(AXIS_ANGLE->Y_ANGLE)) >0.4)
			{
				AXIS_ANGLE->Y_ANGLE = floor(AXIS_ANGLE->Y_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->Y_ANGLE = floor(AXIS_ANGLE->Y_ANGLE);
			}

			if(fabs(AXIS_ANGLE->Z_ANGLE) - fabs(floor(AXIS_ANGLE->Z_ANGLE)) >0.4)
			{
				AXIS_ANGLE->Z_ANGLE = floor(AXIS_ANGLE->Z_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->Z_ANGLE = floor(AXIS_ANGLE->Z_ANGLE);
			}	
			#endif
	
			return 1; /*get angle success! */			
		}




}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the 3axis angle degree with temperautre compensation
*/
/* temperature_data unit : 1 degree */
FLOAT check_g_max=1,check_g_min=1;
CHAR8S KXSD9_GET_ANG_WITH_TEMPERAUTRE(KXSD9_AXIS AXIS_RAW,KXSD9_3AXIS_ANGLE *AXIS_ANGLE,INT16S temperature_data)
{
	FLOAT x_raw_acc,y_raw_acc,z_raw_acc;
	FLOAT x_degree,y_degree,z_degree;
	FLOAT x_temp_comp,y_temp_comp,z_temp_comp;
	FLOAT x_zero_offset_temp_comp,y_zero_offset_temp_comp,z_zero_offset_temp_comp;
	FLOAT check_g;
	INT16S temperature_deff;
	INT16U x_fix,y_fix,z_fix;

		/*check temperature_data */
		/*avoid unexpected error */
		if((temperature_data <-40) ||(temperature_data>85))
		{
			temperature_data = 25;
		}


		/* Calculate the temperautre compensation	 */	
		temperature_deff = temperature_data -25 ; /* SEPC -> temperature 25 degree as a reference */
		x_temp_comp = (FLOAT)temperature_deff * KXSD9_FS_2G_REAL_X_RESOLUTION_1mg * KXSD9_TEMP_VAR_X_AXIS;
		y_temp_comp = (FLOAT)temperature_deff * KXSD9_FS_2G_REAL_Y_RESOLUTION_1mg * KXSD9_TEMP_VAR_Y_AXIS;
		z_temp_comp = (FLOAT)temperature_deff * KXSD9_FS_2G_REAL_Z_RESOLUTION_1mg * KXSD9_TEMP_VAR_Z_AXIS;
	
		/* calculate the new zero offset for each axis */
		x_zero_offset_temp_comp = KXSD9_REAL_X_ZERO_OFFSET + x_temp_comp;
		y_zero_offset_temp_comp = KXSD9_REAL_Y_ZERO_OFFSET + y_temp_comp;
		z_zero_offset_temp_comp = KXSD9_REAL_Z_ZERO_OFFSET + z_temp_comp;


		/* calibrate the data */ 	
		/* real calculate way with temperautre compensation */	
		#if (KXSD9_USE_ACTUAL_GET_ANGLE==1 && KXSD9_USE_IDEAL_GET_ANGLE==0)
		{
			/**** formula : acc = get_rawdata /resolution ; unit = g ; 1g = 9.8 m/(s^2) */
			/* this use full-scale 2g . */
			
			/* calculate the X-axis acceleration value */
			if(AXIS_RAW.X_AXIS>= x_zero_offset_temp_comp) /* if X-axis raw data >=KXSD9_REAL_X_ZERO_OFFSET */
			{	
				x_raw_acc=((FLOAT)AXIS_RAW.X_AXIS - x_zero_offset_temp_comp )/KXSD9_FS_2G_REAL_XPOS_RESOLUTION; /*  unit = g*/

			}
			else if(AXIS_RAW.X_AXIS< x_zero_offset_temp_comp) /* if X-axis raw data <KXSD9_REAL_X_ZERO_OFFSET */
			{	
				x_raw_acc=(x_zero_offset_temp_comp -(FLOAT)AXIS_RAW.X_AXIS )/KXSD9_FS_2G_REAL_XNEG_RESOLUTION; /*  unit = g*/
			}

			/* calculate the Y-axis acceleration value */		
			if(AXIS_RAW.Y_AXIS>= y_zero_offset_temp_comp ) /* if Y-axis raw data >=KXSD9_REAL_Y_ZERO_OFFSET */
			{
				y_raw_acc=((FLOAT)AXIS_RAW.Y_AXIS - y_zero_offset_temp_comp)/KXSD9_FS_2G_REAL_YPOS_RESOLUTION; /*  unit = g*/
			}
			else if(AXIS_RAW.Y_AXIS< y_zero_offset_temp_comp)/* if Y-axis raw data <KXSD9_REAL_Y_ZERO_OFFSET */
			{ 
				y_raw_acc=(y_zero_offset_temp_comp-(FLOAT)AXIS_RAW.Y_AXIS)/KXSD9_FS_2G_REAL_YNEG_RESOLUTION; /*  unit = g*/
			}

			/* calculate the Z-axis acceleration value */	
			if(AXIS_RAW.Z_AXIS>= z_zero_offset_temp_comp) /* if Z-axis raw data >=KXSD9_REAL_Z_ZERO_OFFSET */
			{
				z_raw_acc=((FLOAT)AXIS_RAW.Z_AXIS - z_zero_offset_temp_comp)/KXSD9_FS_2G_REAL_ZPOS_RESOLUTION; /*  unit = g*/
			}
			else if(AXIS_RAW.Z_AXIS< z_zero_offset_temp_comp)/* if Z-axis raw data <KXSD9_REAL_Z_ZERO_OFFSET */
			{
				z_raw_acc=(z_zero_offset_temp_comp-(FLOAT)AXIS_RAW.Z_AXIS)/KXSD9_FS_2G_REAL_ZNEG_RESOLUTION; /*  unit = g */
			}

			/* calculate the Sensitivity with  temperautre compensation*/
			x_raw_acc = x_raw_acc + (KXSD9_TEMP_VAR_X_SENSITIVY_SHIFT *temperature_deff);
			y_raw_acc = y_raw_acc + (KXSD9_TEMP_VAR_Y_SENSITIVY_SHIFT *temperature_deff);
			z_raw_acc = z_raw_acc + (KXSD9_TEMP_VAR_Z_SENSITIVY_SHIFT *temperature_deff);			
	
		}	
		#endif

		/* check the KXSD9 is still? */
		check_g= sqrt(pow(x_raw_acc,2) +  pow(y_raw_acc,2) + pow(z_raw_acc,2));   /* if check_g = KXSD9_G_STILL_MIN ~ KXSD9_G_STILL_MAX is OK */
		if((check_g<KXSD9_G_STILL_MIN) || (check_g > KXSD9_G_STILL_MAX))		/* the KXSD9 is not still */
		{					

			return -1; /* the sensor is not still */
		}
		else  /*the KXSD9 is still */ /* calcualte the angle */	
		{
			
			/* calculate the X-axis degree */
			AXIS_ANGLE->X_ANGLE=atan(x_raw_acc/sqrt(pow(y_raw_acc,2) + pow(z_raw_acc,2))) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */

			/* calculate the Y-axis degree */
			AXIS_ANGLE->Y_ANGLE=atan(y_raw_acc/sqrt(pow(x_raw_acc,2) + pow(z_raw_acc,2))) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */

			/* calculate the Z-axis degree */
			AXIS_ANGLE->Z_ANGLE=atan(sqrt(pow(x_raw_acc,2) + pow(y_raw_acc,2))/z_raw_acc) * (180 /3.14159); /* (180 /3.14159) = 180/PI -> turn to angle */


			#if (KXSD9_ENABLE_THE_ROUNDING_CAL==1)
			if(fabs(AXIS_ANGLE->X_ANGLE) - fabs(floor(AXIS_ANGLE->X_ANGLE)) >0.4)
			{
				AXIS_ANGLE->X_ANGLE = floor(AXIS_ANGLE->X_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->X_ANGLE = floor(AXIS_ANGLE->X_ANGLE);
			}


			if(fabs(AXIS_ANGLE->Y_ANGLE) - fabs(floor(AXIS_ANGLE->Y_ANGLE)) >0.4)
			{
				AXIS_ANGLE->Y_ANGLE = floor(AXIS_ANGLE->Y_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->Y_ANGLE = floor(AXIS_ANGLE->Y_ANGLE);
			}


			if(fabs(AXIS_ANGLE->Z_ANGLE) - fabs(floor(AXIS_ANGLE->Z_ANGLE)) >0.4)
			{
				AXIS_ANGLE->Z_ANGLE = floor(AXIS_ANGLE->Z_ANGLE) +1;
			}
			else
			{
				AXIS_ANGLE->Z_ANGLE = floor(AXIS_ANGLE->Z_ANGLE);
			}	
			#endif

			return 1; /*get angle success! */			
		}


}
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	find the each axis (X,Y,Z) max value
	POS_AXIS_RAW -> max positive
	NEG_AXIS_RAW-> max negative

	POS_AXIS_RAW-> X_AXIS   max Xaxis positive data
	POS_AXIS_RAW-> Y_AXIS   max Yaxis positive data	
	POS_AXIS_RAW-> Z_AXIS   max Zaxis positive data	

	NEG_AXIS_RAW-> X_AXIS   max Xaxis negative data
	NEG_AXIS_RAW-> Y_AXIS   max Yaxis negative data	
	NEG_AXIS_RAW-> Z_AXIS   max Zaxis negative data		
*/
/*********** Test way **********
   Rotate slowly 360 degree twice for 1. & 2. way.
   1.Rotate the X -axis , get the Z-axis & Y-axis  pos max & neg max .
   1.Rotate the Z -axis , get the X-axis  pos max & neg max .
*********** Test way ***********/

/* initial the max pos , max neg value . To find the +-g max value */
/*
	pos -> +g max value;
	neg -> -g max value
*/
INT16S x_max_pos=2048,y_max_pos=2048,z_max_pos=2048;
INT16S x_max_neg=2048,y_max_neg=2048,z_max_neg=2048;
void KXSD9_GET_3AXIS_MAX(KXSD9_AXIS *POS_AXIS_RAW,KXSD9_AXIS *NEG_AXIS_RAW,KXSD9_AXIS RAW_DATA)
{


	/* find the X pos max data */
	if(RAW_DATA.X_AXIS>=x_max_pos)
	{
		x_max_pos =RAW_DATA.X_AXIS;
		POS_AXIS_RAW->X_AXIS=x_max_pos;
	}
	
	/* find the Y pos max data */
	if(RAW_DATA.Y_AXIS>=y_max_pos)
	{
		y_max_pos =RAW_DATA.Y_AXIS;
		POS_AXIS_RAW->Y_AXIS=y_max_pos;
	}

	/* find the Z pos max data */
	if(RAW_DATA.Z_AXIS>=z_max_pos)
	{
		z_max_pos =RAW_DATA.Z_AXIS;
		POS_AXIS_RAW->Z_AXIS=z_max_pos;
	}


	/* find the X neg max data */
	if(RAW_DATA.X_AXIS<x_max_neg)
	{
		x_max_neg =RAW_DATA.X_AXIS;
		NEG_AXIS_RAW->X_AXIS=x_max_neg;
	}	

	/* find the Y neg max data */
	if(RAW_DATA.Y_AXIS<y_max_neg)
	{
		y_max_neg =RAW_DATA.Y_AXIS;
		NEG_AXIS_RAW->Y_AXIS=y_max_neg;
	}

	/* find the Z neg max data */
	if(RAW_DATA.Z_AXIS<z_max_neg)
	{
		z_max_neg =RAW_DATA.Z_AXIS;
		NEG_AXIS_RAW->Z_AXIS=z_max_neg;
	}
	

}
/*--------------------------------------------------------------------------------------------------*/
/********************************************** ADVANCED APPLICATIONS **************************************************/
