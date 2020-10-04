/*-----------------------------------------------------------------------
     Creator		: Morris chiou
     Sensor		: 3 - Axis Accelerometer 
     File Name		: SENSOR_KXSD9.h
     Function		: SENSOR_KXSD9
     Create Date	: 2017/07/12
---------------------------------------------------------------------- */
#ifndef __KXSD9_HEADER__ 
#define __KXSD9_HEADER__  


/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/
/* KXSD9 SLAVE ADDRESS*/
	#define KXSD9_ADDR_PIN 	 			(0)				/*PIN8(ADDR) connect  "0" GND or "1" Vcc*/
#if (KXSD9_ADDR_PIN == 1)
	#define KXSD9_SLAVE_ADDRESS 		(0x32)
#elif (KXSD9_ADDR_PIN == 0)
	#define KXSD9_SLAVE_ADDRESS 		(0x30)
#endif
/*--------------------------------- Define SLAVE ADDRESS -------------------------------------*/


/*--------------------------------- Command Register bit -------------------------------------*/
/*High byte for X axis*/
#define KXSD9_REG_XOUT_H				(0x00)			/*Read only*/
/*-----------------------------------------------------------*/
/*Low byte for X axis in 7~4bit , 3~0bit -> reserved*/
#define KXSD9_REG_XOUT_L				(0x01)			/*Read only*/
/*-----------------------------------------------------------*/
/*High byte for Y axis*/
#define KXSD9_REG_YOUT_H				(0x02)			/*Read only*/
/*-----------------------------------------------------------*/
/*Low byte for Y axis in 7~4bit , 3~0bit -> reserved*/
#define KXSD9_REG_YOUT_L				(0x03)			/*Read only*/
/*-----------------------------------------------------------*/
/*High byte for Z axis*/
#define KXSD9_REG_ZOUT_H				(0x04)			/*Read only*/
/*-----------------------------------------------------------*/
/*Low byte for Z axis in 7~4bit , 3~0bit -> reserved*/
#define KXSD9_REG_ZOUT_L				(0x05)			/*Read only*/
/*-----------------------------------------------------------*/
/*Auxiliary out significant high byte.*/
#define KXSD9_REG_AUXOUT_H			(0x06)			/*Read only*/
/*-----------------------------------------------------------*/
/*Auxiliary out significant low byte in 7~4bit*/
#define KXSD9_REG_AUXOUT_L				(0x07)			/*Read only*/
/*-----------------------------------------------------------*/
/*
	wrtie 0xCA to this register , the offset, sensitivity and temperature correction values will be 
	loaded into RAM and used for all further measurements.
	This is also accomplished at power-up by an internal power-up reset circuit.
*/
#define KXSD9_REG_RESET					(0x0A)			/*Write only	, WRITE 0XCA*/
/*-----------------------------------------------------------*/
/*
	default : 0b11100001
	*** bit 3,4 detail see the spec. 
	bit 2 -> set "0"
	bit 1~0 [FS1 FS0]   select the full scale sensing range of the accelerometer.
	    FS1		    FS0			Full Scale Range		   12-bit Sensitivity
		0			0				+/-8 g					205 counts/g
		0			1				+/-6 g					273 counts/g
		1			0				+/-4 g					410 counts/g
		1			1				+/-2 g					819 counts/g
	----------------------------

	bit 3 [MOTLat]	switches the motion wake up response function between latching and non-latching.
	MOTLat = 1 - If MOTIen in CTRL_REGB is ¡§1¡¨ and a motion event happens, 
	the MOTI bit in CTRL_REGA will be set to ¡§1¡¨ and the MOT pin (7) will be pulled high. 
	The part will clear the MOTIen bit in CTRL_REGB and enter full-power mode. 
	To re-enter low power motion wakeup mode, reset the MOTIen bit to ¡§1¡¨. 

	MOTLat = 0 ¡V If MOTIen in CTRL_REGB is ¡§1¡¨ and a motion event happens,
	the MOTI bit in CTRL_REGA will be set to ¡§1¡¨ and the MOT pin (7) will be pulled high.
	The part will remain in low power motion wakeup mode. 
	The MOT pin (7) can be cleared by reading CTRL_REGA. Figure 8 shows the response of the KXSD9 as described above.

	----------------------------
	bit 4 [MOTLev]	set the motion wakeup threshold to an acceleration level
		MOTLev		FS1		FS0			Motion Wake Up Threshold
			0		0		0				+/-6 	g				
			0		0		1				+/-4.5 	g
			0		1		0				+/-3 	g
			0		1		1				+/-1.5 	g
			1		0		0				+/-4 	g
			1		0		1				+/-3 	g
			1		1		0				+/-2 	g
			1		1		1				+/-1 	g
	----------------------------
	bit 7 ~ 5 [LP2 LP1 LP0]	select the operational bandwidth of the accelerometer.
			LP2		LP1		LP0			Filter Corner Frequency
			0		0		0				No Filter
			0		0		1				2000 Hz
			0		1		0				2000 Hz
			0		1		1				2000 Hz
			1		0		0				1000 Hz	
			1		0		1				500   Hz
			1		1		0				100   Hz
			1		1		1				50     Hz
	----------------------------
*/
#define KXSD9_REG_CTRL_REGC			(0x0C)			/*R/W*/
/*-----------------------------------------------------------*/
/*
	default : b01000000
	bit 0,1,3,4	 -> set "0"
	bit 2 [MOTlen]	enables the motion wakeup feature. -> MOT pin
	MOTIen = 1 ¡V the KXSD9 will run in a low power mode until a motion event occurs that causes 
	MOTI in CTRL_REGA and the MOT pin (7) to go high.
	The part then enters normal operation if MOTLat = 1 or remains in low power mode if MOTLat = 0.
	MOTIen = 0 ¡V the KXSD9 is in normal operating mode.

	bit 5 [ST]	activates the """"self-test"""" function for the sensor elements on all three axes. 
	A correctly functioning KXSD9 will increase all channel outputs when Self test = 1 and Enable = 1. This bit can be read or written.

	bit 6 [ENABLE]	powers up the KXSD9 for operation.
	set 1 ¡V normal operation
	set 0 ¡V low-power standby

	bit 7 [CLKhld]	allows the KXSD9 to hold the serial clock, SCL, low in I2C mode to force the transmitter into a wait state during A/D conversions.
	CLKhld = 1 ¡V SCL held low during A/D conversions
	CLKhld = 0 ¡V SCL unaffected
	CLKhld should be set to 0 when Enable is set to 0 (disabled) to prevent potential holding of the CLK line.
*/
#define KXSD9_REG_CTRL_REGB			(0x0D)			/*R/W*/

/*-----------------------------------------------------------*/
/*
	bit 1[MOTI]	MOTI reports the status of the motion wakeup interrupt. Reading CTRL_REGA clears the MOTI bit and MOT pin (7).
	MOTI = 1 - a motion wake up event has occurred and the MOT pin (7) is high.
	MOTI = 0 - a motion wake up event has not occurred and the MOT pin (7) is low.
*/
#define KXSD9_REG_CTRL_REGA			(0x0E)			/*Read only*/
/*-----------------------------------------------------------*/
/*--------------------------------- Command Register bit -------------------------------------*/


/*--------------------------------- KXSD9  parameter ---------------------------------*/
/* ------------- DEFINE KXSD9 STRUCT   ------------*/
typedef struct KXSD9_DATA
{	
	/*KXSD9 resolution -> 12 bit*/
	INT16S X_AXIS;
	INT16S Y_AXIS;
	INT16S Z_AXIS;	
	INT16S AUXOUT;
}KXSD9_AXIS;
/* ------------- DEFINE KXSD9 STRUCT   ------------*/

/* ------------- DEFINE KXSD9 STRUCT   ------------*/
typedef struct KXSD9_3AXIS_ANGLE_S
{	
	
	FLOAT X_ANGLE;
	FLOAT Y_ANGLE;	
	FLOAT Z_ANGLE;
	CHAR8S X_SIGN;
	CHAR8S Y_SIGN;
	CHAR8S Z_SIGN;
	
}KXSD9_3AXIS_ANGLE;
/* ------------- DEFINE KXSD9 STRUCT   ------------*/





#define KXSD9_LP_NO_FILTER		(0x00<<5)
#define KXSD9_LP_2000HZ			(0x01<<5)
#define KXSD9_LP_1000HZ			(0x04<<5)
#define KXSD9_LP_500HZ			(0x05<<5)
#define KXSD9_LP_100HZ			(0x06<<5)
#define KXSD9_LP_50HZ				(0x07<<5)



/*---------------------------------------------------------------*/
/*
	bit 4 [MOTLev]	set the motion wakeup threshold to an acceleration level
		MOTLev		FS1		FS0			Motion Wake Up Threshold
			0		0		0				+/-6 g				
			0		0		1				+/-4.5 g
			0		1		0				+/-3 g
			0		1		1				+/-1.5 g
			1		0		0				+/-4 g
			1		0		1				+/-3 g
			1		1		0				+/-2 g
			1		1		1				+/-1 g
	----------------------------
		    FS1		    FS0				Full Scale Range		   12-bit Sensitivity
			0			0					+/-8 g				205 counts/g
			0			1					+/-6 g				273 counts/g
			1			0					+/-4 g				410 counts/g
			1			1					+/-2 g				819 counts/g
	--------------------------
*/
#define KXSD9_FS_8G						(0x00<<0)
#define KXSD9_FS_6G						(0x01<<0)
#define KXSD9_FS_4G						(0x02<<0)
#define KXSD9_FS_2G						(0x03<<0)

#define KXSD9_MOTLEV_0					(0x0<<4)		
#define KXSD9_MOTLEV_1					(0x1<<4)	

#define KXSD9_MOTLAT_0					(0x0<<3)		
#define KXSD9_MOTLAT_1					(0x1<<3)	

/**** USE THIS DEFINF for  KXSD9_SET_FS_MOTLEV() function.*/
#define KXSD9_SET_FS_MOTLEV_LAT 		( KXSD9_FS_2G | KXSD9_MOTLEV_0 | KXSD9_MOTLAT_0)
/*---------------------------------------------------------------*/

#define KXSD9_NORMAL_MODE				(0x01<<6)	/*note: bit7 ->0   disable  CLKhld*/
#define KXSD9_PWN_DOWN_MODE			(0x00<<6)	/*note: bit7 ->0	disable  CLKhld*/

#define KXSD9_DIS_MOTLEN				(0x00<<2)	/*normal mode*/
#define KXSD9_EN_MOTLEN					(0x01<<1)	/*run in low power mode & until a motion event occour -> REGA [MOTI]bit7 = 1*/
															/*if motlat = 1 -> enter normal mode*/
															/*if motlat = 0 -> keep low power mode*/
/*---------------------------------------------------------------*/

/*-------------------------------*/
#define KXSD9_TEMP_VAR_X_AXIS						(0.5)				/* temperatrue increase 1C  -> increase 0.5mg *//* see spec page2 */
#define KXSD9_TEMP_VAR_Y_AXIS						(0.5)				/* temperatrue increase 1C  -> increase 0.5mg *//* see spec page2 */
#define KXSD9_TEMP_VAR_Z_AXIS						(3)					/* temperatrue increase 1C  -> increase 3mg *//* see spec page2 */

#define KXSD9_TEMP_VAR_X_SENSITIVY_SHIFT		(0.0001)				/*temperatrue increase 1C shift 0.01% *//* see spec page2 */
#define KXSD9_TEMP_VAR_Y_SENSITIVY_SHIFT		(0.0001)				/*temperatrue increase 1C shift 0.01% *//* see spec page2 */
#define KXSD9_TEMP_VAR_Z_SENSITIVY_SHIFT		(0.0004)				/*temperatrue increase 1C shift 0.04% *//* see spec page2 */

#define KXSD9_G_STILL_MAX							(1.05)				/*this value is get from test *//* unit : g */
#define KXSD9_G_STILL_MIN							(0.82)				/*this value is get from test *//* unit : g */



/* every axis zero offset*/
/* ideal zero offset */
#define KXSD9_IDEAL_X_ZERO_OFFSET			(2048)	/* see sepc page 9*/
#define KXSD9_IDEAL_Y_ZERO_OFFSET			(2048)	/* see sepc page 9*/
#define KXSD9_IDEAL_Z_ZERO_OFFSET			(2048)	/* see sepc page 9*/

/* real zero offset */
#define KXSD9_REAL_X_ZERO_OFFSET				(2020)	/* the sepc max ~ min range : 1843 ~ 2253 , ideal=2048 */ /* see sepc page 2*/
#define KXSD9_REAL_Y_ZERO_OFFSET				(2139)	/* the sepc max ~ min range : 1843 ~ 2253 , ideal=2048 */ /* see sepc page 2*/
#define KXSD9_REAL_Z_ZERO_OFFSET				(2203)	/* the sepc max ~ min range : 1843 ~ 2253 , ideal=2048 */ /* see sepc page 2*/
/*-------------------------------*/

/*-------------------------------*/
/* ideal gravity value : 1g = ??? value */
#define KXSD9_FS_2G_IDEAL_RESOLUTION		(819)	/* for each aixs (X,Y,Z)  ; 1g = 819 value */ /* see sepc page 2*/
#define KXSD9_FS_4G_IDEAL_RESOLUTION		(410)	/* for each aixs (X,Y,Z)  ; 1g = 410 value */ /* see sepc page 2*/
#define KXSD9_FS_6G_IDEAL_RESOLUTION		(273)	/* for each aixs (X,Y,Z)  ; 1g = 273 value */ /* see sepc page 2*/
#define KXSD9_FS_8G_IDEAL_RESOLUTION		(205)	/* for each aixs (X,Y,Z)  ; 1g = 205 value */ /* see sepc page 2*/


#if 1
/* real gravity value : 1g = ??? value */
/* 
X-axis +1g value : KXSD9_FS_2G_REAL_XPOS_RESOLUTION
X-axis -1g value : KXSD9_FS_2G_REAL_XPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_XPOS_RESOLUTION		(2879 - KXSD9_REAL_X_ZERO_OFFSET)		/* this is a real data form rotation test */	/*X +1g =859 */	
#define KXSD9_FS_2G_REAL_XNEG_RESOLUTION		(KXSD9_REAL_X_ZERO_OFFSET-1161)		/* this is a real data form rotation test */	/*X -1g =859 */	

#define KXSD9_FS_2G_REAL_X_RESOLUTION 			(2879 - KXSD9_REAL_X_ZERO_OFFSET)		/* this is a real data form rotation test */	/*X +1g =859 */
#define KXSD9_FS_2G_REAL_X_RESOLUTION_1mg	(KXSD9_FS_2G_REAL_X_RESOLUTION /1000) 	/* X axis 1mg */

/* 
Y-axis +1g value : KXSD9_FS_2G_REAL_YPOS_RESOLUTION
Y-axis -1g value : KXSD9_FS_2G_REAL_YPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_YPOS_RESOLUTION		(3019 - KXSD9_REAL_Y_ZERO_OFFSET)		/* this is a real data form rotation test */	/*Y +1g = 880 */	
#define KXSD9_FS_2G_REAL_YNEG_RESOLUTION		(KXSD9_REAL_Y_ZERO_OFFSET-1259)		/* this is a real data form rotation test */	/*Y -1g = 880 */	

#define KXSD9_FS_2G_REAL_Y_RESOLUTION 			(3019 - KXSD9_REAL_Y_ZERO_OFFSET)		/* this is a real data form rotation test */	/*Y +1g = 880 */	
#define KXSD9_FS_2G_REAL_Y_RESOLUTION_1mg	(KXSD9_FS_2G_REAL_Y_RESOLUTION /1000) 	/*Y axis 1mg */


/* 
Z-axis +1g value : KXSD9_FS_2G_REAL_ZPOS_RESOLUTION
Z-axis -1g value : KXSD9_FS_2G_REAL_ZPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_ZPOS_RESOLUTION		(3047 - KXSD9_REAL_Z_ZERO_OFFSET)		/* this is a real data form rotation test */	/*Z +1g = 844 */	
#define KXSD9_FS_2G_REAL_ZNEG_RESOLUTION		(KXSD9_REAL_Z_ZERO_OFFSET-1359)		/* this is a real data form rotation test */	/*Z -1g = 844 */	

#define KXSD9_FS_2G_REAL_Z_RESOLUTION 			(3047 - KXSD9_REAL_Z_ZERO_OFFSET)		/* this is a real data form rotation test */	/*Y +1g = 844 */	
#define KXSD9_FS_2G_REAL_Z_RESOLUTION_1mg	(KXSD9_FS_2G_REAL_Z_RESOLUTION /1000) 	/*Z axis 1mg */


/*-------------------------------*/	
#else
/* real gravity value : 1g = ??? value */
/* 
X-axis +1g value : KXSD9_FS_2G_REAL_XPOS_RESOLUTION
X-axis -1g value : KXSD9_FS_2G_REAL_XPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_XPOS_RESOLUTION	(2879 - KXSD9_IDEAL_X_ZERO_OFFSET)	/* this is a real data form rotation test */
#define KXSD9_FS_2G_REAL_XNEG_RESOLUTION	(KXSD9_IDEAL_X_ZERO_OFFSET-1161)		/* this is a real data form rotation test */

/* 
Y-axis +1g value : KXSD9_FS_2G_REAL_YPOS_RESOLUTION
Y-axis -1g value : KXSD9_FS_2G_REAL_YPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_YPOS_RESOLUTION	(3019 - KXSD9_IDEAL_Y_ZERO_OFFSET)	/* this is a real data form rotation test */
#define KXSD9_FS_2G_REAL_YNEG_RESOLUTION	(KXSD9_IDEAL_Y_ZERO_OFFSET-1259)		/* this is a real data form rotation test */

/* 
Z-axis +1g value : KXSD9_FS_2G_REAL_ZPOS_RESOLUTION
Z-axis -1g value : KXSD9_FS_2G_REAL_ZPOS_RESOLUTION
*/
#define KXSD9_FS_2G_REAL_ZPOS_RESOLUTION	(3047 - KXSD9_IDEAL_Y_ZERO_OFFSET)	/* this is a real data form rotation test */
#define KXSD9_FS_2G_REAL_ZNEG_RESOLUTION	(KXSD9_IDEAL_Y_ZERO_OFFSET-1359)		/* this is a real data form rotation test */
/*-------------------------------*/	
#endif


/*--------------------------------- KXSD9  parameter ---------------------------------*/


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
CHAR8S KXSD9_INIT(void);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*read X,Y,Z,AUXOUT data from*/
CHAR8S KXSD9_READ_AXIS(KXSD9_AXIS * data);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*reset KXSD9 -> REG 0X0A write 0xCA.*/
CHAR8S KXSD9_RESET(void);
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
CHAR8S KXSD9_SET_LOW_PASS(CHAR8U low_pass_mode);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*set the full scale sensing range of the accelerometer for 0x0c bit1~0 & bit 4 & bit 3 */
/* ** use the define  KXSD9_SET_FS_MOTLEV_LAT */
CHAR8S KXSD9_SET_FS_MOTLEV_MOTLAT(CHAR8U fs_motlev_motlat);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set enables the motion wakeup feature.  for 0x0D bit 2
	 ** use the define 
	KXSD9_DIS_MOTLEN
	KXSD9_EN_MOTLEN	
*/
CHAR8S KXSD9_SET_MOTLEN(CHAR8U motlen);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	set the full scale sensing range of the accelerometer for 0x0D  bit6 &  bit7(disable CLKhld)
	**use  define
	KXSD9_NORMAL_MODE		
	KXSD9_PWN_DOWN_MODE	
*/
CHAR8S KXSD9_SET_PWR_MODE(CHAR8U mode);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	check MOT pin or REGA[0x0E] bit 1
	MOTI = 1 - a motion wake up event has occurred and the MOT pin (7) is high.
	MOTI = 0 - a motion wake up event has not occurred and the MOT pin (7) is low.

	if return 0 -> motion active
	if return -2 -> not motion active
	if return -1 -> read error
*/
CHAR8S KXSD9_CHK_MOTION(void);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** SYSTEM **************************************************/



/********************************************** ADVANCED APPLICATIONS **************************************************/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the 3axis angle degree without temperautre compensation
*/
CHAR8S KXSD9_GET_ANG(KXSD9_AXIS AXIS_RAW,KXSD9_3AXIS_ANGLE *AXIS_ANGLE);
/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
/*
	calculate the 3axis angle degree with temperautre compensation 
*/
/* temperature_data unit : 1 degreeC  */
CHAR8S KXSD9_GET_ANG_WITH_TEMPERAUTRE(KXSD9_AXIS AXIS_RAW,KXSD9_3AXIS_ANGLE *AXIS_ANGLE,INT16S temperature_data);
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
void KXSD9_GET_3AXIS_MAX(KXSD9_AXIS *POS_AXIS_RAW,KXSD9_AXIS *NEG_AXIS_RAW,KXSD9_AXIS RAW_DATA);
/*--------------------------------------------------------------------------------------------------*/
/********************************************** ADVANCED APPLICATIONS **************************************************/


#endif 		/*#ifndef __KXSD9_HEADER__*/ 
