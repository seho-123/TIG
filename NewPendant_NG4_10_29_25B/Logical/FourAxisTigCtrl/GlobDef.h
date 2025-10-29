/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: WeldServ
 * File: GlobDef.h
 * Author: Christopher
 * Created: June 09, 2009
 *******************************************************************/

 
#ifndef GLOBDEF_H_
#define GLOBDEF_H_
#define _WEAK	__attribute__((__weak__))

	 
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <stdarg.h>
#include <..\..\Lib\WSI_types.h>

#define MASTER_MODE_INIT    0
#define MASTER_MODE_SETUP   1
#define MASTER_MODE_ERROR   2
#define MASTER_MODE_ESTOP   3


#define MASTER_MODE_STOP    	10
#define MASTER_MODE_RUN 		11
#define MASTER_MODE_CALIB_STOP 	12
#define MASTER_MODE_CALIB_RUN 	13


#define VECT_TVL 	0
#define VECT_OSC 	1
#define VECT_AVC 	2
#define VECT_AVC2 	3
//#define VECT_WRST 	3
#define VECT_WF		4
#define VECT_WP 	5
#define VECT_WF_1 	6
#define VECT_WP_1 	7
#define VECT_INDX_1 	8
#define VECT_INDX_2 	9
#define VECT_WRST	10
#define VECT_WB_UD		11
#define VECT_WB_LR		12
#define VECT_AUX_WF 	13
#define VECT_AUX_WF_1 	14


#define PROC_TVL 		0
#define PROC_OSC 		1
#define PROC_AVC 		2
#define PROC_WIRE_FEED 	3
#define PROC_WIRE_PULSE 4
#define PROC_TIG_PS 	5
#define PROC_HW_PS 		6
#define PROC_GAS 		7
#define PROC_COOL 		8
#define PROC_INDX 		9
#define PROC_WRST	10
#define PROC_WB_UD		11
#define PROC_WB_LR		12

#define TML_AX_TVL 		0
#define TML_AX_OSC 		1
#define TML_AX_AVC 		2
#define TML_AX_AVC2	    3

#define TML_AX_WF		4
#define TML_AX_WP 		5
#define TML_AX_WF_1		6
#define TML_AX_WP_1 	7

// was added for BPV v2
#define TML_AX_INDX		8
#define TML_AX_INDX_1	9

// was added for 
#define TML_AUX_AX_WF		10
#define TML_AUX_AX_WF_1		11

#define TML_AX_WRST 		12





/* Nieudana Proba bo gdzies TML jest hardcoded
#define TML_AX_OSC 		0
#define TML_AX_AVC 		1
#define TML_AX_WRST		2
#define TML_AX_WF		3
#define TML_AX_TVL 		4
#define TML_AX_WP 		5
#define TML_AX_WF_1		6
#define TML_AX_WP_1 	7


#define PROC_OSC 		0
#define PROC_AVC 		1
#define PROC_WIRE_FEED 	2
#define PROC_TVL 		3
#define PROC_WIRE_PULSE 4
#define PROC_TIG_PS 	5
#define PROC_HW_PS 		6
#define PROC_GAS 		7
#define PROC_COOL 		8
#define PROC_INDX 		9
#define PROC_WRST	10
#define PROC_WB_UD		11
#define PROC_WB_LR		12
*/





#define MODE_SINGLE_TORCH	0
#define MODE_DOUBLE_TORCH	1
#define MODE_DUAL_OSCILATOR	2
#define MODE_DUAL_AVC		3




/* Was good before 16 Jan 15
#define DC_AX_WP  	0
#define DC_AX_WF  	1
#define DC_AX_WP_1  2
#define DC_AX_WF_1  3
*/

#define DC_AX_WF  	0
#define DC_AX_WP  	1
#define DC_AX_WF_1  2
#define DC_AX_WP_1  3

#define DC_INDX_1  	0
#define DC_INDX_2  	1

#define DC_WB_UD	0
#define DC_WB_LR	1



// CALIBRATION
#define CALIB_OSC_SFT_LIMIT  0
#define CALIB_TIG_VOLT_FBK   1
#define CALIB_TVL_SPEED_CMD  2

#define CALIB_TIG_AMPS_CMD   3
#define CALIB_HW_AMPS_CMD    4
#define CALIB_WF_SPEED_CMD   5
#define CALIB_WP_SPEED_CMD   6

#define CALIB_INDX_CMD   	 7





//  PENDANT BUTTONS 

#define BTN_A0   0x1
#define BTN_B0   0x2
#define BTN_C0   0x4
#define BTN_D0   0x8
#define BTN_E0   0x10
#define BTN_F0   0x20

#define BTN_A1   0x40
#define BTN_B1   0x80
#define BTN_C1   0x100
#define BTN_D1   0x200
#define BTN_E1   0x400
#define BTN_F1   0x800


#define BTN_A2   0x1000
#define BTN_B2   0x2000
#define BTN_C2   0x4000
#define BTN_D2   0x8000
#define BTN_E2   0x10000
#define BTN_F2   0x20000


#define BTN_A3   0x40000
#define BTN_B3   0x80000
#define BTN_C3   0x100000
#define BTN_D3   0x200000
#define BTN_E3 	 0x400000
#define BTN_F3   0x800000

#define BTN_A4   0x1000000
#define BTN_B4   0x2000000
#define BTN_C4   0x4000000
#define BTN_D4   0x8000000
#define BTN_E4   0x10000000
#define BTN_F4	 0x20000000


//  ANDROID BUTTONS 

#define ANDR_WHEEL_NOT_PRESSED 0x1

#define ANDR_BTN_HOME   			0x2
#define ANDR_BTN_START  			0x40000000
#define ANDR_BTN_STOP   			0x4

#define ANDR_BTN_ARC_AMPS_NEG   	0x20000
#define ANDR_BTN_ARC_AMPS_POS   	0x200

#define ANDR_BTN_TRAVEL_SPEED_NEG   0x40000
#define ANDR_BTN_TRAVEL_SPEED_POS   0x400

#define ANDR_BTN_WIRE_SPEED_NEG   	0x80000
#define ANDR_BTN_WIRE_SPEED_POS   	0x800

#define ANDR_BTN_AVC_VOLTS_NEG   	0x100000
#define ANDR_BTN_AVC_VOLTS_POS   	0x1000

#define ANDR_BTN_OSC_SPEED_NEG   	0x200000
#define ANDR_BTN_OSC_SPEED_POS   	0x2000

#define ANDR_BTN_OSC_AMPL_NEG   	0x400000
#define ANDR_BTN_OSC_AMPL_POS   	0x4000

#define ANDR_BTN_AVC_LOCK   		0x800000
#define ANDR_BTN_WIRE_DISABLE   	0x8000

#define ANDR_BTN_UP   				0x04000000
#define ANDR_BTN_DOWN   			0x10000000
#define ANDR_BTN_RIGHT   			0x02000000
#define ANDR_BTN_LEFT   			0x20000000
#define ANDR_BTN_ENTER   			0x08000000


/////   Mapping  //////////////
#define UPPER_LEVEL 0
#define LOWER_LEVEL 1
#define DRIVE_SIDE  2


#define PWM_MOD 0

#define WF_ENC  0
#define WP_ENC  1


/////   Mapping  LOWER_LEVEL //////////////

#define ESTOP_STAT_DI 12 // Estop status 

#define TIG_PS_ANALOG 1
#define TIG_PS_DO 0

#define HW_PS_ANALOG 3
#define HW_PS_DO 2

#define LAMP1_ON_DO  	4
#define CAM_1_2_ON_DO  	5


#define COOL_OK_DI   6
#define ARGON_OK_DI   7
#define GP_2_DI   6
#define GP_1_DI   7


#define ARGON_ENABLE_DO 12
#define COOL_ENABLE_DO 	13
#define ESTOP_PERMIT_DO 14
#define ESTOP_RESET_DO 	15




/*******************************/
/*         ALARMS  				****/
/*******************************/

/*

// Alarms Class
#define ERROR_SETUP 		-2
#define ERROR_FATAL 		-1
#define ERROR_MOTOR  		1
#define ERROR_MOTOR_INIT  	2
#define ERROR_TIG_PROC  	3
#define ERROR_COOL_GAS  	4
#define ERROR_PENDANT  		5





// Alarms items
#define ERROR_NONE 0

#define ERROR_TML_1 1 
#define ERROR_TML_2 2
#define ERROR_TML_3 3
#define ERROR_TML_4 4
#define ERROR_TML_5 5
#define ERROR_TML_6 6
#define ERROR_TML_7 7
#define ERROR_TML_8 8
#define ERROR_TML_9 9
#define ERROR_TML_10 10


#define ERROR_FATAL_TML_1 11
#define ERROR_FATAL_TML_2 12
#define ERROR_FATAL_TML_3 13
#define ERROR_FATAL_TML_4 14
#define ERROR_FATAL_TML_5 15
#define ERROR_FATAL_TML_6 16
#define ERROR_FATAL_TML_7 17
#define ERROR_FATAL_TML_8 18
#define ERROR_FATAL_TML_9 19
#define ERROR_FATAL_TML_10 20


#define ERROR_ACP_1  21
#define ERROR_ACP_2  22
#define ERROR_ACP_3  23
#define ERROR_ACP_4  24
#define ERROR_ACP_5  25
#define ERROR_ACP_6  26
#define ERROR_ACP_7  27
#define ERROR_ACP_8  28


#define ERROR_FATAL_ACP_1  31
#define ERROR_FATAL_ACP_2  32
#define ERROR_FATAL_ACP_3  33
#define ERROR_FATAL_ACP_4  34
#define ERROR_FATAL_ACP_5  35
#define ERROR_FATAL_ACP_6  36
#define ERROR_FATAL_ACP_7  37
#define ERROR_FATAL_ACP_8  38

#define ERROR_MOTOR_INIT  39


#define ERROR_ANGLE_NOT_EQ 40

#define ERROR_TIG_TOUCH_TMO 41
#define ERROR_TIG_SP_TMO 	42
#define ERROR_TIG_VOLT_ERR 	43
#define ERROR_TIG_AVC_LIMIT_ERR 	44




#define ERROR_FILE_RW 		45

#define ERROR_BR_HOME  		46

#define ERROR_GAS_TMO 	    47
#define ERROR_COOL_TMO 	    48


#define ERROR_TVL_STALL 	49


#define ERROR_ADROID_COM_LOST  	50


#define ERROR_NONE 0

// Warnings 
#define WARN_AVC_LIMITS  0
#define WARN_OSC_LIMITS  1

*/


/************************/
/*** LCD ****************/
// Page  Definition
#define P_INIT 				1

#define P_MODE_1 			20
#define P_MODE_2 			40
#define P_MODE_3 			60
#define P_MODE_4			80


#define P_POP_UP			95

#define P_MENU 				100

#define P_PARAM_WELD 		160


#define P_PERMIT			200

#define P_PARAM 			210

#define P_PARAM_OSC			235
#define P_PARAM_WP			260
#define P_PARAM_WF			285
#define P_PARAM_AVC			310
#define P_PARAM_HW			345
#define P_PARAM_TIG			360
#define P_PARAM_GAS			380
#define P_PARAM_GLOBAL		400
#define P_PARAM_INDX		425
#define P_PARAM_TVL_ROT 	430


#define P_VAR_SETUP 		460

#define P_PAR_REVIEW 		500

#define P_PARAM_APPL 			560

#define P_MODE_CALIB 			590

#define P_MODE_OSC_LIMIT_CALIB 	605

#define P_MODE_TIG_FBK_CALIB 	610

#define P_MODE_WF_CALIB_PAR 	620
#define P_MODE_WF_CALIB 		627
#define P_MODE_WF_CALIB_VER 	635

#define P_MODE_TIG_PS_CALIB_PAR 640
#define P_MODE_TIG_PS_CALIB 	650
#define P_MODE_TIG_PS_CALIB_VER 655

#define P_MODE_HW_PS_CALIB_PAR 	660
#define P_MODE_HW_PS_CALIB 		665
#define P_MODE_HW_PS_CALIB_VER 	675

#define P_MODE_TVL_CALIB_PAR 	680
#define P_MODE_TVL_CALIB 		687
#define P_MODE_TVL_CALIB_VER 	695
#define P_MODE_INDX_CALIB		705

#define P_MODE_TRACK_CALIB_PAR 	710
#define P_MODE_TRACK_CALIB 		716
#define P_MODE_TRACK_CALIB_VER 	723

#define P_LOGS				730
#define P_ERRORS			740

#define P_DIAGNOSTIC		750
#define P_DIAGN_AXIS		760
#define P_DIAGN_BR			770
#define P_DIAGN_TML			780
#define P_DIAGN_PROC		810
#define P_DIAGN_DC_MTR		820
#define P_DIAGN_IO			830

#define P_SEGMENTS			845
#define P_SPEC_OPERATION 	850

#define P_JOBS 				880

#define P_JOB_SAVE_AS 		890
#define P_JOB_LOAD 			900
#define P_JOB_DELETE		910
#define P_JOB_DELETE_CONF	920


#define P_PARAM_WRST 	925

#define P_PARAM_BOOSTS 	945

#define P_PARAM_CONTIN_WELD 960

#define P_LAST_NODE 		999




// EVENTS
#define EVNT_ACKN 					1
#define EVNT_OK 					2
#define EVNT_CANCEL					3
#define EVNT_RESTART 				4
#define EVNT_ACKN_ALL 				7
#define EVNT_GO_TO_DEFAULT			8
#define EVNT_LOAD					9


#define EVNT_LOAD_DEFAULT			10  
#define EVNT_SKIP_SERVO_TEST 		11
#define EVNT_TOGGLE					12
#define EVNT_ADJUSTMENT_MODE		13
#define EVNT_SAVE_JOB				14
#define EVNT_LOAD_JOB				15
#define EVNT_DELETE_JOB				16

#define EVNT_ALL_VECTORS_SIMUL 		20
#define EVNT_ALL_VECTORS_NO_SIMUL   21

#define EVNT_ALL_PARAM_SEGMENT   	22
#define EVNT_ALL_PARAM_NO_SEGMENT   23

#define EVNT_EDIT_PARAM_VAL   		24
#define EVNT_EDIT_PARAM_SETUP   	25
#define EVNT_ENT_BTN_REQ   			26

#define EVNT_OSC_POS_LIMIT_SET   	27
#define EVNT_OSC_NEG_LIMIT_SET   	28

#define EVNT_SET_AMERICAN_UNIT   	29
#define EVNT_SET_NORMAL_UNIT   		30
#define EVNT_LOAD_DEFAULT_CALIB		31

#define EVNT_GO_CALIB_MODE			33
#define EVNT_RETURN_CALIB_MODE		34


#define EVNT_SET_TVL_CALIB_POINT		35
#define EVNT_SET_TIG_PS_CALIB_POINT		36
#define EVNT_SET_HW_PS_CALIB_POINT		37
#define EVNT_SET_WF_CALIB_POINT			38
#define EVNT_SET_TIG_FBK_CALIB_POINT	39

#define EVNT_VER_TVL_CALIB				40
#define EVNT_VER_TIG_PS_CALIB			41
#define EVNT_VER_HW_PS_CALIB			42
#define EVNT_VER_WF_CALIB				43

#define EVNT_SET_HOME					44
#define EVNT_TEST_OSC					45
#define EVNT_GAS_ON_OFF					46
#define EVNT_SAVE_CURCFG				47
#define EVNT_SAVE_SETUP_BIN				55
#define EVNT_SAVE_SETUP_AS_BIN			56
#define EVNT_SAVE_FULL_AS_BIN			57
#define EVNT_SAVE_CALIB_AS_BIN			58


#define EVNT_STEP_FWD					48
#define EVNT_STEP_REV					49

#define EVNT_SWAP_HEAD					50
#define EVNT_TEST_MODE_ON_OFF			51


#define EVNT_JOG_DIAGN_FWD				60
#define EVNT_JOG_DIAGN_REV				61

#define EVNT_FILL_MODE					70
#define EVNT_SET_INDX_ZERO			  	75

#define EVNT_POPUP_OK					100
#define EVNT_POPUP_CANCEL			  	101

#define EVNT_OSC_ALLIGMENT			  	105


#define EVENT_WELD_WIZARD_DONE			255


//  HEAD DEFINITION 

#define BTN_JOG_MODE_SINGLE 	0
#define BTN_JOG_MODE_DUAL   	1
#define BTN_JOG_MODE_DUAL_INV   2

#define WHL_JOG_MODE_SINGLE 	0
#define WHL_JOG_MODE_STEP   	1
#define WHL_JOG_MODE_DUAL	   	2
#define WHL_JOG_MODE_DUAL_INV   3



// OLD MOTOR POSITION
#define OSC_LAST_POS		0
#define TVL_LAST_POS  		1
#define TVL_ENC_HOME  		2
#define TVL_ZONE_HOME  		3
#define INDX_A_LAST_POS  	4
#define INDX_B_LAST_POS  	5
#define WRST_LAST_POS  		6
#define WRST1_LAST_POS  	7
#define WRST2_LAST_POS  	8



// GEN SETUP

#define SETUP_PIN_PER_ROT 	0
#define SETUP_SEGM_NO		1
#define SETUP_AUTO_START  	2
#define SETUP_HEAD_SETUP  	3
#define SETUP_AUTO_FILL  	4
#define SETUP_OSC_EXC_MODE  5
#define SETUP_WHERE_FILL  	6
#define SETUP_SOUND_DELAY	7
#define SETUP_INDX_DIR		8
#define SETUP_INDX_ENBL		9
#define SETUP_ZONE_CENTR	10
#define SETUP_TWO_TORCH		11
#define SETUP_TWO_UP		12
#define SETUP_TVL_ENC_SET	13
#define SETUP_TVL_SEC_ENC	14
#define SETUP_AVC_LEGACY	15
#define SETUP_WF_WP_SWAP	16
#define SETUP_POLE_ELEMENTS	17
#define SETUP_MAPPING_ON	18
#define SETUP_CONTIN_WELD	19
#define SETUP_WF_RAMP		20
#define SETUP_EXTRAS		21
#define SETUP_LOG_OPTION	22




#define MAPPING_OFF		0x0
#define MAPPING_ON		0x1
#define MAPPING_SEGM	0x2
#define MAPPING_AVC_FF	0x4




#define SETUP_FLOAT_AUTOSTART_DLY	0
#define SETUP_FLOAT_SOUND_TM		1
#define SETUP_FLOAT_TRACK_DIA		2

#define SETUP_FLOAT_TILT_OFFSET		3

#define SETUP_FLOAT_POWER_RATIO_LO	4
#define SETUP_FLOAT_POWER_RATIO_HI	5

#define SETUP_FLOAT_HEAT_INPUT_LO	6
#define SETUP_FLOAT_HEAT_INPUT_HI	7



// Head Type
#define U_BEND_HP_PINS  			0
#define U_BEND_HP_ENC 				1
#define F_HEAD_HP_ENCODER			2
#define BPV_F_HEAD 					3
#define OMEGA_16INCH_HP_ENCODER 	4
#define OMEGA_10INCH_HP_ENCODER 	5
#define OMEGA_8INCH_HP_ENCODER 		6
#define F_HEAD_HP_MINI_ENC 			7
#define SUPER_MINI 					8
#define BPV_FACE_OSC 				9
#define U_BEND_HP_ENC_BEMF 			10
#define F_HEAD_HP_ENC_BEMF			11
#define BPV_FACE_OSC_ENC 			12
#define BPV_FACE_OSC_EXP 			13
#define M_94						14

#define BPV_v2						15  // call - BPV v2 TT BO IND
#define BPV_v2_MPBO					16  // call - BPV v2 MT BO IND
#define BPV_v2_MPGH					17  // call - BPV v2 TT BO TOS IND  // TOS - TVL & OSC swap
#define BPV_v2_MPGV					18  // call - BPV v2 MT BO TOS IND  // TOS - TVL & OSC swap
			
#define BPV_v2_TT_BO				19  // call - BPV v2 TT BO AWF
#define BPV_v2_MT_BO				20  // call - BPV v2 MT BO AWF
#define BPV_v2_TT_BO_TOS			21  // call - BPV v2 TT BO TOS AWF
#define BPV_v2_MT_BO_TOS			22  // call - BPV v2 MT BO TOS AWF 

#define RUBIX						23   
#define F_HEAD_MINI_STND 			24 
#define F_HEAD_MINI_OMEGA 			25
#define D_HEAD_HP					26

#define BPV2_FACE_OSC_EXP 			27

#define BPV_v2_MT		 			28
#define BPV_v1_MT		 			29

#define BPV_v2_Video		 		30

#define CLAMSHELL_HEAD		 		31
#define CHERRYPOINT_BP1_HEAD		32
#define CHERRYPOINT_BP2_HEAD		33

#define CHERRYPOINT_MT_BP2_HEAD		34


#define MIG_TIG_HEAD				35

#define HEAD_INVALID				36


#define ZONE 			1
#define ZONE_RAW 		2
//#define HEAD_SEGM 		3
#define ACT_TVL_DIST 	4
#define WF_STOP_CATCH 	5


// Calibration Types

#define CALIB_NONE 		0
#define CALIB_TVL  		1
#define CALIB_TIG_PS  	2
#define CALIB_HW_PS  	3
#define CALIB_WF  		4
#define CALIB_OSC_LIMIT 5


/***************/
#define NUM_LINE 		3


// ENUMERATION TEXT 


char  CAM_TYPE_TXT[5][RES_TXT_LEN]=
{ 
	{"AGRES"}, 
	{"HARMON"},
	{"TRAPEZ"},
	{"NO OSCIL"},
	{"Cam 5"},
};



char  JOBLINK_MODE_TXT[2][RES_TXT_LEN]=
{ 
	{"NO"},
	{"YES"},
	
};

char  PATH_MODE_TXT[2][RES_TXT_LEN]=
{ 
	{"YES"},
	{"NO"},
	
};

char  JOB_CASE_TXT[6][RES_TXT_LEN]=
{ 
	{"Raw"},
	{"Generic"},
	{"#1"},
	{"#2"},
	{"#3"},
	{"#4"},
	
};

char  AVC_MODE_TXT[4][RES_TXT_LEN]=
{ 
	{"Cont."},
	{"Bck Lock"},
	{"Frg  Lock"},
	{"Lock"},	
		
};

char  TIG_MODE_TXT[4][RES_TXT_LEN]=
{ 
	{"Straight"},
	{"Pulse Arc"},
	{"Sync Pulse"},
	{"Sync In/Out"},
		
};

char  SYNCH_OPTION_TXT[3][RES_TXT_LEN]=
{ 
	{"Straight"},
	{"Sync Pulse"},
	{"Sync In/Out"},
		
};


char  TVL_MODE_TXT[4][RES_TXT_LEN]=
{ 
	{"Overlap Relativ"},
	{"Overlap On A"},
	{"Overlap On B"},
	{"Continues"},
	
};




static const  char  PROCESS_STATUS_TXT[12][RES_TXT_LEN]=
{ 
	{"Strt Dly"},
	{"Rmp UP"},
	{"RUN"},
	{"Stop Dly"},
	{"Rmp Dwn"},
	{"STOP"},
	{"FAULT"},
	{"JOG"},
	{"STEP"},
	{"INIT"},
	{"FORCE_ON"},
	{"RETRACT"},
	{"AVC_OK"},
	{"AVC_HOLD"},	
	{"DirCntrl"},
	{"Force_CALIB"},

	
};


static const  char  AXIS_STATUS_TXT[11][RES_TXT_LEN]=
{ 
	{"OFF"},
	{"Idle"},
	{"Hold"},
	{"Home"},
	{"Pos "},
	{"Vel"},
	{"Torq "},
	{"Volt "},
	{"PT"},
	{"Abs"},
	{"Rel"},
		
};

static const  char  TML_STATUS_TXT[20][RES_TXT_LEN]=
{ 
	{"Drive Err"},
	{"Ver Err"},
	{"Init"},
	{"Hrt Beat Err"},
	{"DEAD"},
	{"Ready"},
	{"IDLE"},
	{"HOLD Init"},
	{"HOLD OK"},
	{"POS M Init"},
	{"POS M OK"},

	{"VEL M Init"},
	{"VEL M OK"},
	{"TRQ M Init"},
	{"TRG M OK"},
	{"VOLT M Init"},
	{"VOLT M OK"},
	{"PT M INIT"},
	{"PT M OK"},
		
};

static const  char  TIG_CAL_MODE_TXT[2][RES_TXT_LEN]=
{ 
	{"Arc Start"},
	{"Load Bank"},
			
};


static const  char  ON_OFF_TXT[2][RES_TXT_LEN]=
{ 
	{"OFF"},
	{"ON"},
			
};
static const  char  HI_LO_TXT[2][RES_TXT_LEN]=
{ 
	{"Low"},
	{"High"},
			
};

static const  char  OSC_MODE_TXT[4][RES_TXT_LEN]=
{ 
	{"CONTIN."},
	{"STEPOVER"},
	{"SPIRAL"},	
	{"NO OSCIL."},	
};
static const  char  OSC_SYNCH_TXT[5][RES_TXT_LEN]=
{ 
	{"CONTIN."},
	{"DWELL IN"},
	{"DWELL OUT"},
	{"DWELL IN/OUT"},
	{"EXCURSION"},
};
	
static const  char  WF_AUX_TXT[3][RES_TXT_LEN]=
{ 
	{"NOT INSTALLED"},
	{"AUX OFF"},
	{"AUX ON"},
};



static const char LINE_EMPTY[1] = {0x00}; 





/*********************************/

 
 /*****************************/
 /* Constants */

#define FULL_ROT 65536 





/////////////////////////////////////////////////////

//                       PARAMETERS
 
 /* Datatypes and datatypes of function blocks */
typedef enum SCALED_VAR_Enum
{	

	 
	parLCD_BtnMode = 0,

	// First most importatnt values
	parTIG_AmpSP, 
	parTIG_AmpInSP,
	parTIG_AmpOutSP,
	parTIG_BackAmp,
	parTIG_PulseFreq,
	parTIG_PulseWidth,	
	parAVC_VoltSP,	
	parAVC_VoltInSP,
	parAVC_VoltOutSP,
	parAVC_VoltBackSP,
	parTVL_VelSP,
	parTVL_VelInSP,
	parTVL_VelOutSP,
	parTVL_VelBackSP,
	parWF_SpeedSP, 
	parWF_SpeedInSP,
	parWF_SpeedOutSP,
	parWF_BkgSpeedSP, 
	parWP_SpeedSP, 
	parWP_SpeedInSP,
	parWP_SpeedOutSP,
	parWP_BkgSpeedSP,
	parHW_AmpSP,
	parOSC_Amplit,		
	parOSC_Excur,
	parOSC_Dwell_In,
	parOSC_Dwell_Out,
	///////////////////////
	parSpare1,
	parSpare2,
	parSpare3,
	parOSC_Indx_Vel,
	parOSC_StartDly,
	parOSC_RampUp,
	//parOSC_Amplit,
	parOSC_InitAmplit,
	parOSC_FinalAmplit,
	parOSC_StopDly,
	parOSC_RampDwn,
	parOSC_RampUpdate,
	//parOSC_Excur,
	//parOSC_Dwell_In,
	//parOSC_Dwell_Out,
	parOSC_MaxTvlVel,
	parOSC_JogSpeed,
	parOSC_JogSpeedDirCntrl,
	parOSC_Indx,
	parOSC_Period,
	
	parAVC_DB_Window,		
	parAVC_TouchVolt,	
	parAVC_TouchTm,
	parAVC_TRetractTm,
	parAVC_TouchRepeat,
	parAVC_VoltInit,
	//parAVC_VoltSP,
	//parAVC_BkgVoltSP,
	parAVC_KissTm,
	parAVC_TouchVel,
	parAVC_ToSPVel,

	parAVC_DownSlope,
	parAVC_VoltFinal,
	parAVC_HoldTm,	
	parAVC_GoBackTm,

	parAVC_VSPTmOut,

	parAVC_StartDly,	
	parAVC_StopDly,
	
	parAVC_Upslope,
	parAVC_MaxVoltError,
	parAVC_Limits_Mask_Tm,
	parAVC_JogSpeedDirCntrl,
	parAVC_LiftSpeedDirCntrl,
	parAVC_FbkFilter,
	parAVC_KPP,
	parAVC_KIP,
	parAVC_BB_Integr,
	parAVC_spare1,

	parWF_SpeedInit, 
	parWF_SpeedFinal,

	//parWF_SpeedSP, 
	parWF_RampUp,
	parWF_RampDwn,
	parWF_RampUpdate, 
	parWF_StartDly,
	parWF_StopDly,

	parWF_RetractDly,
	parWF_RetractSpeed,
	parWF_RetracDist,
	
	parWF_Boost1SP,
	parWF_Boost2SP,
	parWF_CalibVel1,
	parWF_CalibVel2,
	parWF_JogSpeed,
	parWF_JogSpeedDirCntrl,
	
	parWF_Spare1,
	parWF_Spare2,
	
	parWP_SpeedInit,
	parWP_SpeedFinal,

	//parWP_SpeedSP, 
	parWP_RampUp,
	parWP_RampDwn,
	parWP_RampUpdate, 
	parWP_StartDly,
	parWP_StopDly,
	parWP_Boost1SP,
	parWP_Boost2SP,
	parWP_JogSpeed,
	parWP_JogSpeedDirCntrl,
	parWP_Spare1,
	parWP_Spare2,
	parWP_Spare3,
	parWP_Spare4,
	parWP_Spare5,
	
	//parTIG_AmpSP, 
	//parTIG_BackAmp,
	//parTIG_PulseFreq,
	//parTIG_PulseWidth,

	parTIG_StartAmp,
	parTIG_EndAmp,
	parTIG_RampUp,
	parTIG_RampDwn,
	parTIG_StartDly,
	parTIG_StopDly,
	parTIG_CalibAmp1,
	parTIG_CalibAmp2,			
	parTIG_AmpSP_Boost,
	parTIG_Boost1,
	parTIG_Boost2,

	//parHW_AmpSP,
	parHW_VoltSP,
	parHW_StartAmp,
	parHW_EndAmp,
	parHW_RampUp,
	parHW_RampDwn,
	parHW_RampUpdate, 
	parHW_RampUpDly,
	parHW_RampDwnDly,
	parHW_CalibAmp1,
	parHW_CalibAmp2,
	parHW_AmpSP_Boost,
	parHW_Spare1,
	parHW_Spare2,

	//parTVL_VelSP,
	parTVL_InitVel,
	parTVL_FinalVel,
	parTVL_RampUp,
	parTVL_RampDwn,
	parTVL_RampUpdate, 
	parTVL_StartDly,
	parTVL_StopDly,
	parTVL_Jog2Stop,
	parTVL_JogSpeedDirCntrl,
	parTVL_RunDist,
	
	parTVL_RewrapDly,
	//parTVL_RewrapDist,
	parTVL_OverlapDist,
	parTVL_RewrapVel,
	
	parTVL_CalibVel1,
	parTVL_CalibVel2,
	parTVL_TrackDia,
	parTVL_SpeedTolerane,
	parTVL_Boost1,
	parTVL_Boost2,
			
	parGAS_PrePurgeTm,
	parGAS_PostPurgeTm,
	parGAS_Tmo,
	parGAS_Spare1,
	parGAS_Spare2,

	parCOOL_Tmo,
	parCOOL_Spare1,
	parCOOL_Spare2,

	parGEN_CalibDist,
	parGEN_CalibTime,
	parGEN_AmpRead,

	parGEN_WeldProgramRepeat,

	/* It was for weld wizard
	parGEN_UpSlope,
	parGEN_DownSlope,
	parGEN_RampUp,
	parGEN_RampDown,

	parGEN_T0_TVL_StartDly,
	parGEN_T1_WF_StartDly,
	parGEN_T3_DS_StartDly,
	parGEN_T4_TVL_StoptDly,
	parGEN_GAS_PostPurgeTm,
	*/

	parGEN_spare1,
	
	parCONTIN_TVL_RampDwn,
	parCONTIN_TVL_RampUp,
	parCONTIN_WF_RampDwn,
	parCONTIN_WF_RampUp,
	parCONTIN_WP_RampDwn,
	parCONTIN_WP_RampUp,
	
	parGEN_Radius,
	parGEN_RadSpeed_Boost,

	// Extra AVC for Bang-Bang regulation	
	parAVC_BB_Vel,
	parAVC_BB_Gain,
	parAVC_LU_TM,

	parGEN_CalibTime1,
	parGEN_CalibTime2,
	parGEN_CalibTime3,
	parGEN_CalibTime4,	

	parGEN_CalibDist1,
	parGEN_CalibDist2,
	parGEN_CalibDist3,
	parGEN_CalibDist4,

	parGEN_CalibAmp1,
	parGEN_CalibAmp2,

	parINDX_StepDist,
	parINDX_StepVel,
	parINDX_JogSpeedDirCntrl,
	parINDX_StepAfterBeats,
	parINDX_Spare1,
	parINDX_Spare2,
	parINDX_Spare3,
	parINDX_Spare4,

	parWRST_JogSpeedDirCntrl,
	parWRST_Spare1,
	
	parTVL_POS_OFFSET,
	parTVL_POS_PREDICT,
	
	parGEN_CalibDist_2,

	parAVC_Boost1,
	parAVC_Boost2,

	parCONTIN_TIG_RampDwn,
	parCONTIN_TIG_RampUp,
	parCONTIN_TIG_Percent,
	parCONTIN_TIG_Dwell,
		
	parLAST_PARAM  , // This must be last with higher number

	///////////

	varCurTVL_Vel ,
	varCurTVL_Vel_Calc,
	varTVL_Vel_Cmd,
	varCurWF_Vel,
	varWF_Vel_Cmd,  
	varCurWFDist,
	varCurWP_Vel,
	varWP_Vel_Cmd, 
	varCurArc_Volt,
	varArc_Volt_Cmd,
	varCurTIG_Amp,
	varTIG_Amp_Cmd,
	varCurHW_Amp, 
	varHW_Amp_Cmd, 
	varCurHW_Volt, 
	varHW_Volt_Cmd, 
	varOSC_Centr,
	varOSC_CurPos,
	varOSC_CurRawPos,
	varOSC_Amplit_Cmd,
	varOSC_curExcurTm,
	varOSC_ExcurTm_Cmd,
	varOSC_Dwell_In_Cmd,
	varOSC_Dwell_Out_Cmd,
	varINDX_A_Pos,
	varINDX_B_Pos,
	varINDX_Dif_Pos,
	varGlobTimer, 

	varLAST_VARIABLE , // This must be last with higher number
		
	
} SCALED_VAR_Enum;

typedef enum INT_VAR_Enum
{	

	parINT_NumZone = 0, 
			
	parINT_LAST_PARAM , // This must be last with higher number 
		
	varINT_PIN_PER_ROT,
	varINT_SEGM_NO,
	varINT_ZONE_NO,
	varINT_curSegm,
	varINT_curZone,
	varINT_StartPos,
	varINT_FinalPos,
	varINT_CurTVLPos,
	varINT_LAST_VARIABLE, // This must be last with higher number	

}INT_VAR_Enum;


typedef enum ENUM_VAR_Enum
{	
	
	parENUM_OSC_CAM = 0,  

	parENUM_WeldProgramLinkMode,

	parENUM_PathMode,

	//parENUM_WeldProgramCase,
	parENUM_ARCLINK_MODE,

	parENUM_AVC_Mode,
	
	parENUM_TIG_Mode,

	parENUM_WF_WP_Relation,

	parENUM_TVL_MODE,	

	parENUM_TVL_SYN_MODE,	

	parENUM_WF_SYN_MODE,	
	
	parENUM_WP_SYN_MODE,

	parENUM_TIG_Cal_Mode,

	parENUM_OSC_Mode,
	
	parENUM_OSC_Synch,

	parENUM_WF_AUX,
	
	parENUM_LAST_PARAM,	 // This must be last with higher number	
	
	varENUM_LAST_VARIABLE,   // This must be last with higher number	

}ENUM_VAR_Enum;

typedef enum STRING_VAR_Enum
{	
	parSTRING_FreeTXT,
	parSTRING_LAST_PARAM,	 // This must be last with higher number	

	varSTRING_NextProgramName,
	varSTRING_Spare1 ,
	varSTRING_curProgramName ,
	varSTRING_Spare2 ,
	varSTRING_HMI_0,
	varSTRING_HMI_1,
	varSTRING_HMI_2,
	varSTRING_HMI_3,
	varSTRING_HMI_4,
	varSTRING_HMI_5,
	varSTRING_HMI_6,
	varSTRING_HMI_7,
	varSTRING_HMI_8,
	varSTRING_HMI_9,
	varSTRING_HMI_10,
	varSTRING_HMI_11,
	varSTRING_HMI_12,
	varSTRING_HMI_13,
	varSTRING_HMI_14,
	varSTRING_HMI_15,
	varSTRING_HMI_16,
	varSTRING_HMI_17,
	varSTRING_HMI_18,
	varSTRING_HMI_19,
	varSTRING_UserName ,
	varSTRING_PlcVersion ,
	varSTRING_FileName ,
	varSTRING_MasteStepTXT,
	varSTRING_LAST_VARIABLE,   // This must be last with higher number	

}STRING_VAR_Enum;



typedef enum RES_TXT_Enum
{	
	resTXT_OSC_CAM = 0,
	resTXT_ON_OFF = 5,	
	resWeldProgramName = 7,
	resWelProgramLinkMode = 8,
	resPathMode = 10,
	resWeldProgramCase = 12,
	resAVCMode = 18,
	resTIGMode = 22,
	resTVLMode = 26,
	resSYNCOption = 30,
	resTigCalMode = 33,
	resTXT_OSC_Mode = 35,	
	resTXT_OSC_Synch = 39,	
	resTXT_WF_AUX = 44,
	resLAST_TXT = 47,	

}RES_TXT_Enum;


 

 
	 
#endif /* GLOBDEF_H_ */

