/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Package: UniversalController
 * File: SuperDefinition.h
 * Author: ckaczkowski
 * Created: June 18, 2014
 *******************************************************************/

//#ifndef _SUPERDEFINITION_H
//#define _SUPERDEFINITION_H


#include <bur/plctypes.h>

#include <..\..\Lib\WSI_types.h>
#include <..\..\Lib\LoggerLib.h>
#include <..\..\Lib\arclink\arclink.h>


#define FULL_HEAD_LIST    0
//#define CHERRYPOINT_LIST  1

//#define EXTENDED_LOGS 1
//#define TML_CMD_LOGS 1

// HARDWARE SELCTION
//#define UNIV_SYSTEM 1   // MPC
//#define CMT_SYSTEM 2	  // CMT
#define MIG_SYSTEM 3      // NG4 , CAN network , HP PS  
//#define MIG_EXP_SYSTEM 4 // NG4 Ethernet to Can converter
//#define NG4_TIG_SYSTEM 5
//////////////////////
// aby dziala TIG w NG4 trzeba odkomentowac
// MIG_SYSTEM , MIG_EXP_SYSTEM i NG4_TIG_SYSTEM

#define MIG_PENDANT 20

//#define SYSTEM_SIMULATION 1

//#define CLAMSHELL 1 // nie potrzebne
//#define CHERRYPOINT 2 // nie potrzebne

#define MASTER_NUM   2 // Number of Masters run simultaneously
#define TASKS_NUM 5 // Number of tasks (programs) in PLC beside supermaster

#define HRD_COUPLER_NUM 	3

#define HRD_ENCODER_NUM 	8
#define HRD_AN_INP_NUM  	8
#define HRD_AN_OUTP_NUM 	8
#define HRD_DIG_INP_NUM  	24
#define HRD_DIG_OUTP_NUM  	24
#define HRD_PWM_MOD_NUM  	1

#define USER_OPERATOR 0
#define USER_ADJUSTER 1
#define USER_MANAGER  2
#define USER_JA		  10

#define SEGM_NUM		8
#define SEGM_TYPE_NUM	6


#define BR_SERVO_NUM 	6
#define TML_SERVO_NUM 	16
#define TML_MGR_NUM 	2
#define GEN_MTR_NUM     6
#define DC2DIRMTR_NUM	2
#define TML_RLY_NUM 	6


#define PROCESS_NUM		16
#define VECTORS_NUM		16

#define CAM_NUM			3


#define OSC_NUM 		3
#define WRST_NUM 		3
#define AVC_NUM 		3
#define WIRE_BLK_NUM 	3
#define ROT_NUM 		3
#define TVL_NUM 		3
#define WIRE_PULS_NUM	3
#define WIRE_FEED_NUM	3
#define TIG_PS_NUM		3
#define HW_PS_NUM		3
#define GAS_NUM			3
#define COOL_NUM        3
#define INDEX_NUM       3
#define MV_NUM			1


#define ALARMS_NUM      64
#define SUB_UNIT_NUM	4


#define NODES_NUM_MAX 1024

#define WB_UD	0
#define WB_LR	1


///////////////
// VNC HMI
#define MAX_READ_VAR 16

///////////////
/*******************************/
/*         ALARMS  				****/
/*******************************/


// Alarms Class
#define ERROR_SETUP 		-2
#define ERROR_FATAL 		-1
#define ERROR_MOTOR  		1
#define ERROR_MOTOR_INIT  	2
#define ERROR_TIG_PROC  	3
#define ERROR_COOL_GAS  	4
#define ERROR_TVL  			5
#define ERROR_PENDANT  		6
#define ERROR_ESTOP  		10
#define ERROR_HARDWARE  	11





// Alarms items
#define ERROR_NONE 0


#define ERROR_DRV_01 1 
#define ERROR_DRV_11 11 
#define ERROR_DRV_21 21
#define ERROR_DRV_31 31
#define ERROR_DRV_41 41


/*


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

*/



//#define ERROR_ANGLE_NOT_EQ 40

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

#define ERROR_ARCLINK_COM_LOST  	51
#define ERROR_ARCLINK_PS_ERR  		52

#define ERROR_PROJECT_HRDW_ERR  	53

#define ERROR_ESTOP_ERR  	54

#define ERROR_MODULE_ERR  	55

#define ERROR_TIG_MIG_SW_ERR  	56


#define ERROR_NONE 0

// Warnings 
#define WARN_AVC_LIMITS  0
#define WARN_OSC_LIMITS  1



//#endif




