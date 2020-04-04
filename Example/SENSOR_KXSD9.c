/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: 3 - Axis Accelerometer 
     File Name		: SENSOR_KXSD9.c
     Function		: SENSOR_KXSD9
     Create Date	: 2017/07/12
---------------------------------------------------------------------- */
#ifndef __KXSD9_FUNCTION__
#define __KXSD9_FUNCTION__ 

#include <stdio.h>
#include <math.h>
#include <delay.h>

#include "SENSOR_KXSD9.h"
#include "Porting_Layer.h"


//********************************************* SYSTEM *************************************************
//--------------------------------------------------------------------------------------------------
// 1.reset KXSD9 once  
// 2.set the operational bandwidth [LP]  = 1	1	0	[100 Hz]
// 3.the full scale sensing range [FS] = 1	1  [+/-2g]
// 4.Set MOTLev = 1 & MOTLat = 0
// 5.set operation mode -> normal mode
// 6.disable CLKhld -> set "0"
// 7.Set  MOTIen = 0
//--------------------------
//return ststus
//     0	initial ok!!
//    -1	reset fail
//    -2	set low pass fail
//    -3	set FS , MOTlen , MOTlat  fail
//    -4	set mode & disable CLKhld  fail
//    -5	set MOTlen fail	
//--------------------------
CHAR8S KXSD9_INIT(void)
{
		CHAR8S status = 0; 

		// 1.reset KXSD9 once  
		status = KXSD9_RESET();
		if(status!=0) return -1; //reset fail
		delay_ms(5);
		
		// 2.set the operational bandwidth [LP]  = 1	1	0	[100 Hz]
		status = KXSD9_SET_LOW_PASS(KXSD9_LP_100HZ);
		if(status!=0) return -2; //set low pass fail
		delay_ms(1);

		// 3.the full scale sensing range [FS] = 1	1  [+/-2g]
		// 4.Set MOTLev = 1 & MOTLat = 0
		status = KXSD9_SET_FS_MOTLEV_MOTLAT(KXSD9_SET_FS_MOTLEV_LAT);
		if(status!=0) return -3; //set FS , MOTlen , MOTlat  fail
		delay_ms(1);

		// 5.set operation mode -> normal mode
		// 6.disable CLKhld -> set "0"
		status = KXSD9_SET_PWR_MODE(KXSD9_NORMAL_MODE);
		if(status!=0) return -4; //set mode & disable CLKhld  fail
		delay_ms(1);
		
		// 7.Set  MOTIen = 0
		status = KXSD9_SET_MOTLEN(KXSD9_DIS_MOTLEN);
		if(status!=0) return -5; //set MOTlen fail
		delay_ms(1);

		return 0 ; // initial ok!!
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//read X,Y,Z,AUXOUT data from
CHAR8S KXSD9_READ_AXIS(KXSD9_AXIS *data)
{
	CHAR8S status = 0; 
	CHAR8U read_data[8] = {0};
	
	status = i2c_read_n_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_XOUT_H,8,&read_data[0]);
	if(status!=1) return -1 ; //read fail

	data->X_AXIS = (INT16S)( read_data[0]<<4 | (read_data[1]>>4)) ;
	data->Y_AXIS = (INT16S)( read_data[2]<<4 | (read_data[3]>>4)) ;
	data->Z_AXIS = (INT16S)( read_data[4]<<4 | (read_data[5]>>4)) ;
	data->AUXOUT = (INT16S)( read_data[6]<<4 | (read_data[7]>>4)) ;
	return 0 ; // read success
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//reset KXSD9 -> REG 0X0A write 0xCA.
CHAR8S KXSD9_RESET(void)
{
	CHAR8S status = 0; 
	
		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_RESET,0xCA);//write 0xCA to reset
		if(status !=1) return -1; //write fail
		return 0; //write success 
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//set the low pass frequency for 0x0C bit 7~5
// ** use the define 
//KXSD9_LP_NO_FILTER		
//KXSD9_LP_2000HZ		
//KXSD9_LP_1000HZ		
//KXSD9_LP_500HZ	
//KXSD9_LP_100HZ
//KXSD9_LP_50HZ	
CHAR8S KXSD9_SET_LOW_PASS(CHAR8U low_pass_mode)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		//check low_pass_mode value
		if(low_pass_mode != KXSD9_LP_NO_FILTER ||low_pass_mode != KXSD9_LP_2000HZ||\
		low_pass_mode != KXSD9_LP_1000HZ ||low_pass_mode != KXSD9_LP_500HZ ||\
		low_pass_mode != KXSD9_LP_100HZ || low_pass_mode != KXSD9_LP_50HZ) return -1; 	// error low_pass_mode value.
	
		//fisrt - load reg 0x0C 
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,&read_data);
		if(status !=1) return -1; 		//read fail.
		
		write_data = read_data & 0x1F; 	// 0x1F mask -> clear bit 7 ~ 5 , remain other bit
		write_data|= low_pass_mode ; // set low pass value

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,write_data);
		if(status !=1) return -1; 		//write fail.
		return 0 ; // set low pass filter success!

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//set the full scale sensing range of the accelerometer for 0x0c bit1~0 & bit 4 & bit 3
// ** use the define  KXSD9_SET_FS_MOTLEV_LAT
CHAR8S KXSD9_SET_FS_MOTLEV_MOTLAT(CHAR8U fs_motlev_motlat)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;
	
		//didn't check the fs_motlev_motlat value!

		//fisrt - load reg 0x0C
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,&read_data);
		if(status !=1) return -1; 		//read fail.
		
		write_data = read_data & 0XE0; 	// 0xE0 mask -> clear bit 4~0, remain other bit
		write_data|= fs_motlev_motlat ; // set fs , MOTlev , MOTlat

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGC,write_data);
		if(status !=1) return -1;		//write fail.
		return 0 ; // set fs , MOTlev , MOTlat success!

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//set enables the motion wakeup feature.  for 0x0D bit 2
// ** use the define 
// KXSD9_DIS_MOTLEN
// KXSD9_EN_MOTLEN	
CHAR8S KXSD9_SET_MOTLEN(CHAR8U motlen)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		//check motlen
		if(motlen!= KXSD9_DIS_MOTLEN || motlen!= KXSD9_EN_MOTLEN ) return -1; //error motlen value
	
		//fisrt - load reg 0x0D 
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,&read_data);
		if(status !=1) return -1; 		//read fail.
		
		write_data = read_data & 0xFB; 	// 0xFB mask -> clear bit 2 , remain other bit
		write_data|= motlen ; // set motlen value

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,write_data);
		if(status !=1) return -1; 		//write fail.
		return 0 ; // set motlen success!

}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//set the full scale sensing range of the accelerometer for 0x0D  bit6 &  bit7(disable CLKhld)
//**use  define
// KXSD9_NORMAL_MODE		
// KXSD9_PWN_DOWN_MODE	
CHAR8S KXSD9_SET_PWR_MODE(CHAR8U mode)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0,write_data = 0;

		//check mode
		if(mode!= KXSD9_NORMAL_MODE || mode!= KXSD9_PWN_DOWN_MODE ) return -1; //error mode value
	
		//fisrt - load reg 0x0D 
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,&read_data);
		if(status !=1) return -1; 		//read fail.
		
		write_data = read_data & 0x3F; 	// 0x3F mask -> clear bit 6,7 , remain other bit
		write_data|= mode ; // set mode value

		status = i2c_write_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGB,write_data);
		if(status !=1) return -1; 		//write fail.
		return 0 ; // set mode success!
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//check MOT pin or REGA[0x0E] bit 1
//MOTI = 1 - a motion wake up event has occurred and the MOT pin (7) is high.
//MOTI = 0 - a motion wake up event has not occurred and the MOT pin (7) is low.

//if return   0 -> motion active
//if return -2 -> not motion active
//if return -1 -> read error
CHAR8S KXSD9_CHK_MOTION(void)
{
	CHAR8S status = 0 ;
	CHAR8U read_data = 0;

		//read reg 0x0E
		status = i2c_read_1_byte_data(KXSD9_SLAVE_ADDRESS,KXSD9_REG_CTRL_REGA,&read_data);
		if(status !=1) return -1; //read fail.

		//check if bit 1 set "1" -> motion occour !
		if(read_data & 0x02) return 0; // motion active
		else return -2; //not motion active
}
//--------------------------------------------------------------------------------------------------
//********************************************* SYSTEM *************************************************



#endif 		//#ifndef __KXSD9_FUNCTION__
