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
#include <stdarg.h>
#include <..\..\Lib\Statelib.h>	
#include <..\..\Lib\WSI_types.h>	


// THIS is PLC Version on Screen
#define  PLC_VERSION_STRING				"HotPulse_5Febr18" //Based ON ; "HotPulse_01Jun15"   WF_MAX = 50 


#define FIFO_DLY 4

#define true 1
#define false 0
#define TRUE 1
#define FALSE 0

//#define SMALL_CPU 0 //comment if CP1585

//General
//-------------------------------------------
#define  TWIRL_SPEED 4 //bigger number means slower, must be greater than 0
#define  KNOB_SENSITIVITY_FACTOR 1  //counts div factor, bigger number means slower changes, must be greater than 0


//Menu LCD
#ifdef SMALL_CPU
#define NUM_LCD 1  //number of LCD
#else
#define NUM_LCD 2  //number of LCD
#endif

//Power Supplies
#define NUM_POWER_SUPPLIES 2  //primary and HW

#define PRIMARY_PS_AMPS_AT_10V 250 //changed from 350 to 250 on 091412 by MAF by request of MS to deal with Miller Dynasty 26% of max minimum current setting.
#define HW_PS_AMPS_AT_10V 320  //changed from 100 to 320 on 091812 by MAF to accommodate the Jackle HW PS which has 0-5V input not 0-10V with a 160A max.  Setting HW_PS_AMPS_AT_10V to 320 and HWAMPS_MAX to 100 will limite the output voltage to 3.125V = 100 amps. //old comment - changed from 100 to 200 on 091412 by MAF to accommodate the Jackle HW PS which has 0-5V input not 0-10V.  Setting HW_PS_AMPS_AT_10V to 200 and HWAMPS_MAX to 100 will limite the output voltage to 5V = 100 amps. 

//Jackle
#define JACKLE_PS_MAX_COMMAND_V 5.0
#define JACKLE_PS_AMPS_AT_MAX_COMMAND_V 160.0
#define JACKLE_PS_MAX_FEEDBACK_V 5.0
#define JACKLE_PS_AMPS_AT_MAX_FEEDBACK_V 160.0

//Accopian
#define ACCOPIAN_PS_MAX_COMMAND_V 10.0
#define ACCOPIAN_PS_AMPS_AT_MAX_COMMAND_V 108.0
#define ACCOPIAN_PS_MAX_FEEDBACK_V 10.0
#define ACCOPIAN_PS_AMPS_AT_MAX_FEEDBACK_V 108.0

#define ACCOPIAN_PS_VOLTS_AT_MAX_CURRENT 50.0
#define ACCOPIAN_PS_VOLTS_AT_MIN_CURRENT 12.0
#define ACCOPIAN_PS_VOLTS_CMD_SLOPE 0.5    //  Volts cmd on analog input / Volts on PWS output 


//Miller
#define MILLER_PS_MAX_COMMAND_V 10.0
#define MILLER_PS_AMPS_AT_MAX_COMMAND_V 250.0
#define MILLER_PS_MAX_FEEDBACK_V 10.0
#define MILLER_PS_AMPS_AT_MAX_FEEDBACK_V 250.0

//GTVI
#define GTVI_PS_MAX_COMMAND_V 10.0
#define GTVI_PS_AMPS_AT_MAX_COMMAND_V 300.0
#define GTVI_PS_MAX_FEEDBACK_V 10.0
#define GTVI_PS_AMPS_AT_MAX_FEEDBACK_V 300.0

//GTVI
#define GTII_PS_MAX_COMMAND_V 10.0
#define GTII_PS_AMPS_AT_MAX_COMMAND_V 300.0
#define GTII_PS_MAX_FEEDBACK_V 10.0
#define GTII_PS_AMPS_AT_MAX_FEEDBACK_V 300.0

//LINCOLN S350
#define LINCS350_PS_MAX_COMMAND_V 8.593846154
//#define LINC_PS_AMPS_AT_MAX_COMMAND_V 333.33
#define LINCS350_PS_AMPS_AT_MAX_COMMAND_V 350.0

#define LINCS350_PS_MAX_FEEDBACK_V 8.593846154
//#define LINC_PS_AMPS_AT_MAX_FEEDBACK_V 333.33
#define LINCS350_PS_AMPS_AT_MAX_FEEDBACK_V 350.0

//LINCOLN S500
#define LINCS500_PS_MAX_COMMAND_V 7.98
//#define LINC_PS_AMPS_AT_MAX_COMMAND_V 333.33
#define LINCS500_PS_AMPS_AT_MAX_COMMAND_V 500.0

#define LINCS500_PS_MAX_FEEDBACK_V 7.98
//#define LINC_PS_AMPS_AT_MAX_FEEDBACK_V 333.33
#define LINCS500_PS_AMPS_AT_MAX_FEEDBACK_V 500.0



//Motors
#define NUM_MOTORS 4  //WF, WP, Rem WP 0, Rem WP 1

//initial start settings
//-------------------------------------------
#define  DISPCODE_INITIAL_SP ArcAmps_Setting
#define  RUN_STATE_INITIAL_SP PostCheck
#define  PRIMARY_PS_TYPE_INITIAL_SP Miller_Type
#define  HOTWIRE_PS_TYPE_INITIAL_SP Jackle_Type
#define  CONTROL_MODE_INITIAL_SP Manual
#define  RUN_MODE_INITIAL_SP WF_And_WP_And_HW_Mode
#define  LOAD_DEFAULTS_MODE_INITIAL_SP No_Load
#define  BOOST_LEVEL_1_TYPE_INITIAL_SP ARC_WF_WP_HW_Boost_Type //changed from ARC_And_WF_Boost_Type to ARC_And_WF_And_WP_And_HW_Boost_Type on 091412 by MAF by request of MS.
#define  BOOST_LEVEL_2_TYPE_INITIAL_SP ARC_WF_WP_HW_Boost_Type //changed from ARC_And_WF_Boost_Type to ARC_And_WF_And_WP_And_HW_Boost_Type on 091412 by MAF by request of MS.


//initial settings
//-------------------------------------------
#define  WPFREQ_INITIAL_SP 20.0 //hz
#define  WFSPEED_INITIAL_SP 45.0 //ipm
#define  ARCAMPS_INITIAL_SP 110.0 //amps
#define  HWAMPS_INITIAL_SP 75.0 //amps

#define  BOOST_LEVEL_1_ARC_INITIAL_SP 25.0 //in units of variable type
#define  BOOST_LEVEL_2_ARC_INITIAL_SP -15.0 //in units of variable type
#define  BOOST_LEVEL_1_WF_INITIAL_SP -15.0 //in units of variable type
#define  BOOST_LEVEL_2_WF_INITIAL_SP 0.0 //in units of variable type
#define  BOOST_LEVEL_1_WP_INITIAL_SP 0.0 //in units of variable type
#define  BOOST_LEVEL_2_WP_INITIAL_SP 0.0 //in units of variable type
#define  BOOST_LEVEL_1_HW_INITIAL_SP 0.0 //in units of variable type
#define  BOOST_LEVEL_2_HW_INITIAL_SP 0.0 //in units of variable type

#define  WFSTARTDELAY_INITIAL_SP 0.0 //in sec
#define  WPSTARTDELAY_INITIAL_SP 0.0 //in sec
#define  HWSTARTDELAY_INITIAL_SP 0.0 //in sec
#define  ARCSTARTDELAY_INITIAL_SP 0.0 //in sec

#define  WFSTOPDELAY_INITIAL_SP 0.0 //in sec
#define  WPSTOPDELAY_INITIAL_SP 0.0 //in sec
#define  HWSTOPDELAY_INITIAL_SP 0.0 //in sec
#define  ARCSTOPDELAY_INITIAL_SP 0.0 //in sec

#define  WFSTARTRAMP_INITIAL_SP 0.0 //in sec
#define  WPSTARTRAMP_INITIAL_SP 0.0 //in sec
#define  HWSTARTRAMP_INITIAL_SP 0.0 //in sec
#define  ARCSTARTRAMP_INITIAL_SP 2.0 //in sec

#define  WFSTOPRAMP_INITIAL_SP 0.0 //in sec
#define  WPSTOPRAMP_INITIAL_SP 0.0 //in sec
#define  HWSTOPRAMP_INITIAL_SP 0.0 //in sec
#define  ARCSTOPRAMP_INITIAL_SP 3.0 //in sec

#define WF_CAL_SLOPE_INITIAL_SP 1.0 //in units of variable type
#define WP_CAL_SLOPE_INITIAL_SP 1.0 //in units of variable type
#define HW_AMPS_CAL_SLOPE_INITIAL_SP 1.0 //in units of variable type
#define ARC_AMPS_CAL_SLOPE_INITIAL_SP 1.0 //in units of variable type

#define WF_CAL_OFFSET_INITIAL_SP 0.0 //in units of variable type
#define WP_CAL_OFFSET_INITIAL_SP 0.0 //in units of variable type
#define HW_AMPS_CAL_OFFSET_INITIAL_SP 0.0 //in units of variable type
#define ARC_AMPS_CAL_OFFSET_INITIAL_SP 0.0  //in units of variable type


//max settings
//-------------------------------------------
#define  WPFREQ_MAX 50.0 //20.0 //hz
#define  WFSPEED_MAX 400.0 //ipm
#define  ARCAMPS_MAX 250.0 //amps  //changed from 350 to 250 on 091412 by MAF by request of MS to deal with Miller Dynasty 26% of max minimum current setting.
#define  HWAMPS_MAX 100 //amps  //can't be set higher than 100 for Jackle PS otherwise more than 5V will be output

#define  WPFREQ_BOOST_MAX 5.0 //hz
#define  WFSPEED_BOOST_MAX 40.0 //ipm
#define  ARCAMPS_BOOST_MAX 50.0 //amps  //changed to 5 from 35 on 100812 by MAF 
#define  HWAMPS_BOOST_MAX 10.0 //amps

#define  WFSTARTDELAY_MAX 5.0 //in sec
#define  WPSTARTDELAY_MAX 5.0 //in sec
#define  HWSTARTDELAY_MAX 5.0 //in sec
#define  ARCSTARTDELAY_MAX 5.0 //in sec

#define  WFSTOPDELAY_MAX 5.0 //in sec
#define  WPSTOPDELAY_MAX 5.0 //in sec
#define  HWSTOPDELAY_MAX 5.0 //in sec
#define  ARCSTOPDELAY_MAX 5.0 //in sec

#define  WFSTARTRAMP_MAX 5.0 //in sec
#define  WPSTARTRAMP_MAX 5.0 //in sec
#define  HWSTARTRAMP_MAX 5.0 //in sec
#define  ARCSTARTRAMP_MAX 10.0 //in sec  //changed to 10.0 from 5.0 by MAF on 092612

#define  WFSTOPRAMP_MAX 5.0 //in sec
#define  WPSTOPRAMP_MAX 5.0 //in sec
#define  HWSTOPRAMP_MAX 5.0 //in sec
#define  ARCSTOPRAMP_MAX 10.0 //in sec  //changed to 10.0 from 50.0 by MAF on 092612

#define WF_SPEED_CAL_SLOPE_MAX 1.5  //in units of variable type
#define HW_AMPS_CAL_SLOPE_MAX 1.2  //in units of variable type
#define ARC_AMPS_CAL_SLOPE_MAX 1.2  //in units of variable type

#define WF_SPEED_CAL_OFFSET_MAX 20.0  //in units of variable type
#define HW_AMPS_CAL_OFFSET_MAX 20.0  //in units of variable type
#define ARC_AMPS_CAL_OFFSET_MAX 20.0  //in units of variable type

//min settings
//-------------------------------------------
#define  WPFREQ_MIN 5.0 //hz
#define  WFSPEED_MIN 5.0 //ipm
#define  ARCAMPS_MIN 50.0 //amps
#define  HWAMPS_MIN 5.0 //amps

#define  WPFREQ_BOOST_MIN -5.0 //hz
#define  WFSPEED_BOOST_MIN -40.0 //ipm
#define  ARCAMPS_BOOST_MIN -50.0 //amps  //changed to -5 from -35 on 100812 by MAF 
#define  HWAMPS_BOOST_MIN -10.0 //amps

#define  WFSTARTDELAY_MIN 0.0 //in sec
#define  WPSTARTDELAY_MIN 0.0 //in sec
#define  HWSTARTDELAY_MIN 0.0 //in sec
#define  ARCSTARTDELAY_MIN 0.0 //in sec

#define  WFSTOPDELAY_MIN 0.0 //in sec
#define  WPSTOPDELAY_MIN 0.0 //in sec
#define  HWSTOPDELAY_MIN 0.0 //in sec
#define  ARCSTOPDELAY_MIN 0.0 //in sec

#define  WFSTARTRAMP_MIN 0.0 //in sec
#define  WPSTARTRAMP_MIN 0.0 //in sec
#define  HWSTARTRAMP_MIN 0.0 //in sec
#define  ARCSTARTRAMP_MIN 0.0 //in sec

#define  WFSTOPRAMP_MIN 0.0 //in sec
#define  WPSTOPRAMP_MIN 0.0 //in sec
#define  HWSTOPRAMP_MIN 0.0 //in sec
#define  ARCSTOPRAMP_MIN 0.0 //in sec

#define WF_SPEED_CAL_SLOPE_MIN 0.5  //in units of variable type
#define HW_AMPS_CAL_SLOPE_MIN 0.8  //in units of variable type
#define ARC_AMPS_CAL_SLOPE_MIN 0.8  //in units of variable type

#define WF_SPEED_CAL_OFFSET_MIN -20.0  //in units of variable type
#define HW_AMPS_CAL_OFFSET_MIN -20.0  //in units of variable type
#define ARC_AMPS_CAL_OFFSET_MIN -20.0  //in units of variable type


//increment value settings
//-------------------------------------------
#define  WPFREQ_INCR 1.0
#define  WFSPEED_INCR 5.0 //ipm
#define  ARCAMPS_INCR 5.0 //amps
#define  HWAMPS_INCR 5.0 //amps

#define  BOOST_LEVEL_1_ARC_INCR 5.0 //in units of variable type
#define  BOOST_LEVEL_2_ARC_INCR 5.0 //in units of variable type
#define  BOOST_LEVEL_1_WF_INCR 5.0 //in units of variable type
#define  BOOST_LEVEL_2_WF_INCR 5.0 //in units of variable type
#define  BOOST_LEVEL_1_WP_INCR 1.0 //in units of variable type
#define  BOOST_LEVEL_2_WP_INCR 1.0 //in units of variable type
#define  BOOST_LEVEL_1_HW_INCR 5.0 //in units of variable type
#define  BOOST_LEVEL_2_HW_INCR 5.0 //in units of variable type

#define  WFSTARTDELAY_INCR 0.1 //in sec
#define  WPSTARTDELAY_INCR 0.1 //in sec
#define  HWSTARTDELAY_INCR 0.1 //in sec
#define  ARCSTARTDELAY_INCR 0.1 //in sec

#define  WFSTOPDELAY_INCR 0.1 //in sec
#define  WPSTOPDELAY_INCR 0.1 //in sec
#define  HWSTOPDELAY_INCR 0.1 //in sec
#define  ARCSTOPDELAY_INCR 0.1 //in sec

#define  WFSTARTRAMP_INCR 0.1 //in sec
#define  WPSTARTRAMP_INCR 0.1 //in sec
#define  HWSTARTRAMP_INCR 0.1 //in sec
#define  ARCSTARTRAMP_INCR 0.1 //in sec

#define  WFSTOPRAMP_INCR 0.1 //in sec
#define  WPSTOPRAMP_INCR 0.1 //in sec
#define  HWSTOPRAMP_INCR 0.1 //in sec
#define  ARCSTOPRAMP_INCR 0.1 //in sec

#define WF_SPEED_CAL_SLOPE_INCR 0.01  //in units of variable type
#define HW_AMPS_CAL_SLOPE_INCR 0.01  //in units of variable type
#define ARC_AMPS_CAL_SLOPE_INCR 0.01  //in units of variable type

#define WF_SPEED_CAL_OFFSET_INCR 1.0  //in units of variable type
#define HW_AMPS_CAL_OFFSET_INCR 1.0  //in units of variable type
#define ARC_AMPS_CAL_OFFSET_INCR 1.0  //in units of variable type


//#define ROUND_OF_TO_1_DECIMAL 10
#define ROUND_OF_TO_2_DECIMAL 100

//display strings
//-------------------------------------------

//variables
#define  VAR_STRING_SIZE 12 //include the /n 11+1
#define  VAR_BLINK_STRING         "           "

#define  WPFREQ_DISP_STRING				"WP FREQ:   "
#define  WFSPEED_DISP_STRING			"WIRE SPEED:"
#define  ARCAMPS_DISP_STRING			"ARC AMPS:  "
#define  HWAMPS_DISP_STRING				"HW AMPS:   "

#define  BOOST1VAR_DISP_STRING			"BOOST1 VAR:"
#define  BOOST2VAR_DISP_STRING			"BOOST2 VAR:"

#define  ARC_AMPS_BOOST1SET_DISP_STRING			"ARC B1 SET:"
#define  ARC_AMPS_BOOST2SET_DISP_STRING			"ARC B2 SET:"
#define  WF_SPEED_BOOST1SET_DISP_STRING			"WS B1 SET: "
#define  WF_SPEED_BOOST2SET_DISP_STRING			"WS B2 SET: "
#define  WP_FREQ_BOOST1SET_DISP_STRING			"WP B1 SET: "
#define  WP_FREQ_BOOST2SET_DISP_STRING			"WP B2 SET: "
#define  HW_AMPS_BOOST1SET_DISP_STRING			"HW B1 SET: "
#define  HW_AMPS_BOOST2SET_DISP_STRING			"HW B2 SET: "

#define  WFSTARTDELAY_DISP_STRING		"WS ON DL:  "
#define  WPSTARTDELAY_DISP_STRING		"WP ON DL:  "
#define  HWSTARTDELAY_DISP_STRING		"HW ON DL:  "
#define  ARCSTARTDELAY_DISP_STRING		"ARC ON DL: "

#define  WFSTOPDELAY_DISP_STRING		"WS OFF DL: "
#define  WPSTOPDELAY_DISP_STRING		"WP OFF DL: "
#define  HWSTOPDELAY_DISP_STRING		"HW OFF DL: "
#define  ARCSTOPDELAY_DISP_STRING		"ARC OFF DL:"

#define  WFSTARTRAMP_DISP_STRING		"WS UPSLT:  "
#define  WPSTARTRAMP_DISP_STRING		"WP UPSLT:  "
#define  HWSTARTRAMP_DISP_STRING		"HW UPSLT:  "
#define  ARCSTARTRAMP_DISP_STRING		"ARC UPSLT: "

#define  WFSTOPRAMP_DISP_STRING			"WS DWNSLT: "
#define  WPSTOPRAMP_DISP_STRING			"WP DWNSLT: "
#define  HWSTOPRAMP_DISP_STRING			"HW DWNSLT: "
#define  ARCSTOPRAMP_DISP_STRING		"ARC DWNSLT:"

#define  ERRORCODE_ACK_STRING			"ERROR CODE:"
#define  PRIMARY_PS_TYPE_DISP_STRING	"PR PS TYPE:"
#define  HOTWIRE_PS_TYPE_DISP_STRING	"HW PS TYPE:"
#define  CONTROL_MODE_DISPLAY_STRING	"CNTL MODE: "
#define  RUN_MODE_DISPLAY_STRING		"RUN MODE:  "

#define  MENU_MODE_DISPLAY_STRING		"MENU MODE: "

#define  LOAD_DEFAULTS_MODE_DISPLAY_STRING	"LOAD DFLTS:"

#define  WFCAL_SLOPE_DISP_STRING			"WS SLOPE:  "
#define  HWCAL_SLOPE_DISP_STRING			"HW SLOPE:  "
#define  ARCCAL_SLOPE_DISP_STRING           "ARC SLOPE: "

#define  WFCAL_OFFSET_DISP_STRING			"WS OFFSET: "
#define  HWCAL_OFFSET_DISP_STRING			"HW OFFSET: "
#define  ARCCAL_OFFSET_DISP_STRING          "ARC OFFSET:"


//status
#define STATUS_DISP_STRING "STATUS:" //7 chars

#define ESTOP_ACTIVE_STRING						"ESTOP ACTIVE"  //12 chars
#define EDIT_MODE_STRING						"EDIT  MODE  "  //12 chars
#define POST_CHECK_DISP_STRING					"POST CHECK  "  //12 chars
#define READY_DISP_STRING						"READY       "  //12 chars
#define JOGGING_DISP_STRING						"JOGGING     "  //12 chars
#define ARC_ENABLED_DISP_STRING					"ARC ENABLE  "  //12 chars
#define REM_ENABLED_DISP_STRING					"REM ENABLE  "  //12 chars
#define WF_RUNNING_DISP_STRING					"WS          "  //12 chars
#define HW_RUNNING_DISP_STRING					"HW          "  //12 chars
#define WP_RUNNING_DISP_STRING					"WP          "  //12 chars
#define WF_AND_WP_RUNNING_DISP_STRING			"WS/WP       "  //12 chars
#define WF_AND_HW_RUNNING_DISP_STRING			"WS/HW       "  //12 chars
#define WP_AND_HW_RUNNING_DISP_STRING			"WP/HW       "  //12 chars
#define WF_AND_WP_AND_HW_RUNNING_DISP_STRING	"WS/WP/HW    "  //12 chars
#define ERROR_DISP_STRING						"ERROR       "  //12 chars

#define ARC_DISP_STRING			"ARC"
#define NO_ARC_DISP_STRING		"   "
#define REM_DISP_STRING			"REM"
#define NO_REM_DISP_STRING		"   "
#define WF_DISP_STRING			"WS"
#define NO_WF_DISP_STRING		"  "
#define WP_DISP_STRING			"WP"
#define NO_WP_DISP_STRING		"  "
#define HW_DISP_STRING			"HW"
#define NO_HW_DISP_STRING		"  "


//errors
#define NO_ERROR_STRING					"NO ERROR"  //8 chars
#define TEST_ERROR_STRING				"TEST    "  //8 chars
#define WF_0_MOTOR_HIGH_AMPS_STRING		"WF  MT A"  //8 chars
#define WP_0_MOTOR_HIGH_AMPS_STRING		"WP  MT A"  //8 chars
#define WF_1_MOTOR_HIGH_AMPS_STRING		"WF  MT B"  //8 chars
#define WP_1_MOTOR_HIGH_AMPS_STRING		"WP  MT B"  //8 chars

#define HARDWARE_NOT_ATTACHED_STRING	"HARDWARE"  //8 chars
#define RESET_IN_PROGRESS_STRING	    "RESET IP"  //8 chars


//Primary Power Supply Types
#define  MILLER_PS_TYPE_DISP_STRING			"  MILLER"  //8 chars
#define  GTII_PS_TYPE_DISP_STRING			"    GTII"  //8 chars
#define  GTVI_PS_TYPE_DISP_STRING			"    GTVI"  //8 chars
#define  LINCS350_PS_TYPE_DISP_STRING		"LINCS350"  //8 chars
#define  LINCS500_PS_TYPE_DISP_STRING		"LINCS500"  //8 chars


//Hotwire Power Supply Types
#define  JACKLE_PS_TYPE_DISP_STRING		"  JACKLE"  //8 chars
#define  ACOPIAN_PS_TYPE_DISP_STRING	" ACOPIAN"  //8 chars

//Run Modes
#define  WF_RUN_MODE_DISP_STRING				"      WS"  //8 chars
#define  WP_RUN_MODE_DISP_STRING				"      WP"  //8 chars
#define  HW_RUN_MODE_DISP_STRING				"      HW"  //8 chars
#define  WP_AND_HW_RUN_MODE_DISP_STRING			"   WP/HW"  //8 chars
#define  WF_AND_HW_RUN_MODE_DISP_STRING			"   WS/HW"  //8 chars
#define  WF_AND_WP_RUN_MODE_DISP_STRING			"   WS/WP"  //8 chars
#define  WF_AND_WP_AND_HW_RUN_MODE_DISP_STRING	"WS/WP/HW"  //8 chars


//Menu Modes
#define  USER_MENU_MODE_DISP_STRING				"    USER"  //8 chars
#define  CAL_MENU_MODE_DISP_STRING				"     CAL"  //8 chars
#define  BOOST_MENU_MODE_DISP_STRING			"   BOOST"  //8 chars
#define  CONFIG_MENU_MODE_DISP_STRING			"  CONFIG"  //8 chars

//Control Modes
#define  MANUAL_CONTROL_MODE_DISP_STRING	"  MANUAL"  //8 chars
#define  AUTOMATIC_CONTROL_MODE_DISP_STRING	"    AUTO"  //8 chars
#define  FourAxis_CONTROL_MODE_DISP_STRING	"FourAxis"  //8 chars


//Control Modes
#define  NO_LOAD_DEFAULTS_MODE_DISP_STRING	"      NO"  //8 chars
#define  YES_LOAD_DEFAULTS_MODE_DISP_STRING	"     YES"  //8 chars


//Boost Var Types
#define  ARC_BOOST_TYPE_DISP_STRINGG                    "       A"  //8 chars
#define  WF_BOOST_TYPE_DISP_STRING                      "      WS"  //8 chars
#define  WP_BOOST_TYPE_DISP_STRING                      "      WP"  //8 chars
#define  HW_BOOST_TYPE_DISP_STRING                      "      HW"  //8 chars

#define  ARC_WF_BOOST_TYPE_DISP_STRING                  "    A/WS"  //8 chars
#define  ARC_WP_BOOST_TYPE_DISP_STRING                  "    A/WP"  //8 chars
#define  ARC_HW_BOOST_TYPE_DISP_STRING                  "    A/HW"  //8 chars
#define  WF_WP_BOOST_TYPE_DISP_STRING                   "   WS/WP"  //8 chars
#define  WF_HW_BOOST_TYPE_DISP_STRING                   "   WS/HW"  //8 chars
#define  WP_HW_BOOST_TYPE_DISP_STRING                   "   WP/HW"  //8 chars

#define  ARC_WF_WP_BOOST_TYPE_DISP_STRING				" A/WS/WP"  //8 chars
#define  ARC_WF_HW_BOOST_TYPE_DISP_STRING				" A/WS/HW"  //8 chars
#define  ARC_WP_HW_BOOST_TYPE_DISP_STRING				" A/WP/HW"  //8 chars
#define  WF_WP_HW_BOOST_TYPE_DISP_STRING            	"WS/WP/HW"  //8 chars

#define  ARC_WF_WP_HW_BOOST_TYPE_DISP_STRING			"ALL VARS"  //8 chars

//-------------------------------------------




//misc settings
//-------------------------------------------
#define  USEC_PER_SEC 1000000.0
#define  USEC_PER_MSEC 1000.0
#define  SEC_PER_MIN 60.0

#define  WF_INCHES_PER_REV 2.225 //inches per rev
#define  WF_ENCODER_PER_REV 2000.0
#define  WF_GBOX_NUM 86.0  
#define  WF_GBOX_DEN 1.0 

#define  WP_ENCODER_PER_REV 2000.0
#define  WP_GBOX_NUM 44.0  
#define  WP_GBOX_DEN 10.0 


//motor settings
#define  DEFAULT_WF_CURRENT_LIMIT 3.0
#define  DEFAULT_WP_CURRENT_LIMIT 3.0

#define DEFAULT_WF_OVERCURRENT_CYCLES_LIMIT 5
#define DEFAULT_WP_OVERCURRENT_CYCLES_LIMIT 5

#define WF_JOG_SPEED 150.0  //ipm default was set to 75
#define WF_REV_JOG_SPEED 50.0 //ipm default was set to 20


#define MAX_ANALOG_OUT_INT_VALUE 32767 //2^15-1
#define MAX_ANALOG_OUT_VALUE 10.0

//-------------------------------------------



typedef enum {
	  
	WpFreq_Setting,
	WfSpeed_Setting,
	ArcAmps_Setting,
	HwAmps_Setting,
	  
	BoostLevel_1_Variable_Setting,
	  
	ArcAmpsBoostLevel_1_Setting,
	WfSpeedBoostLevel_1_Setting,
	WpFreqBoostLevel_1_Setting,
	HwAmpsBoostLevel_1_Setting,
  
	BoostLevel_2_Variable_Setting,
  
	ArcAmpsBoostLevel_2_Setting,
	WfSpeedBoostLevel_2_Setting,
	WpFreqBoostLevel_2_Setting,
	HwAmpsBoostLevel_2_Setting,
	  
	ArcStartDelay_Setting,
	WfStartDelay_Setting,
	WpStartDelay_Setting,
	HwStartDelay_Setting,
	  
	ArcStopDelay_Setting,
	WfStopDelay_Setting,
	WpStopDelay_Setting,
	HwStopDelay_Setting,
	  
	ArcStartRamp_Setting,
	WfStartRamp_Setting,
	WpStartRamp_Setting,
	HwStartRamp_Setting,
	  
	ArcStopRamp_Setting,
	WfStopRamp_Setting,
	WpStopRamp_Setting,
	HwStopRamp_Setting,
	  
	ArcCalSlope_Setting,
	WfCalSlope_Setting,
	HwCalSlope_Setting,

	ArcCalOffset_Setting,
	WfCalOffset_Setting,
	HwCalOffset_Setting,
	  
	PrimaryPowerSupplyType_Setting,
	HotwirePowerSupplyType_Setting,
	ControlMode_Setting,
	RunMode_Setting,
	  
	LoadDefaults_Setting,
	
	MenuMode_Setting,
		
	ErrorCode_Ack,	

	PLC_Version,

	NEXT_Project_Setting,

	Last_Display_Setting_Code,
	  
} Display_Setting_Code;
  
typedef enum {
	  
	PostCheck,
	Ready,
	Arc_Enabled,
	Starting,
	Jogging,
	JoggingRev,
	Running,
	RemoteRunning,
	Error
	  
} Run_State;  



typedef enum {
	  
	Stopped_Motor_State,
	Running_Motor_State,
	Jogging_Motor_State,
	JoggingRev_Motor_State,
	Error_Motor_State

} Motor_State;  
  

typedef enum {
	  
	Stopped_Power_Supply_State,
	Running_Power_Supply_State,
	Error_Power_Supply_State 
		  
} Power_Supply_State;  


typedef enum {
	  
	No_Error = 0,
	Test_Error = 1,
	WF_0_Motor_High_Amps_Error = 2,
	WP_0_Motor_High_Amps_Error = 3,
	WF_1_Motor_High_Amps_Error = 4,
	WP_1_Motor_High_Amps_Error = 5,
	Hardware_Not_Attached = 6,
	  
} Error_Code;  
  
typedef enum {
	  
	ARC_Boost_Type,
	WF_Boost_Type,
	WP_Boost_Type,
	HW_Boost_Type,
	ARC_WF_Boost_Type,
	ARC_WP_Boost_Type,
	ARC_HW_Boost_Type,
	WF_WP_Boost_Type,
	WF_HW_Boost_Type,
	WP_HW_Boost_Mode,
	ARC_WF_WP_Boost_Type,
	ARC_WF_HW_Boost_Type,
	ARC_WP_HW_Boost_Type,
	WF_WP_HW_Boost_Type,
	ARC_WF_WP_HW_Boost_Type
	  
} Boost_Type;  
  
typedef enum {
	  
	ARC_Boost_Check_Type,
	WF_Boost_Check_Type,
	WP_Boost_Check_Type,
	HW_Boost_Check_Type
	  
} BoostCheck_Type;  
  
typedef enum {
	  
	Level1,
	Level2
	  
} BoostLevel_Type;  
  
typedef enum {
	  
	Miller_Type,
	GTII_Type,
	GTVI_Type,
	LINCS350_Type,
	LINCS500_Type
	  
} Primary_PS_Type;  


typedef enum {
	  
	Jackle_Type,
	Acopian_Type
	  
} Hotwire_PS_Type;  


typedef enum {
	  
	Manual,
	Automatic,
	FourAxis
	  
} Control_Mode;  

typedef enum {
	  
	User,
	Boost,
	Calibration,
	Configuration
	  
} Menu_Mode;  

  
typedef enum {
	  
	No_Load,
	Yes_Load
	  
} Load_Defaults_Mode;  

typedef enum {
	  
	WF_Mode,
	WP_Mode,
	HW_Mode,
	WF_And_WP_Mode,
	WF_And_HW_Mode,
	WP_And_HW_Mode,
	WF_And_WP_And_HW_Mode
	  
} Run_Mode;  
 
typedef enum {
	  
	PrimaryPS=0,
	HotwirePS=1,
	  
} PowerSupplies;   


typedef enum {

	 /* 
	Wf_Motor=0,
	Wp_Motor=1,
	Remote_Wp_0_Motor=2,
	Remote_Wp_1_Motor=3,
	*/

	Wf_0_Motor=0,
	Wp_0_Motor=1,
	Wf_1_Motor=2,
	Wp_1_Motor=3,
		  
} Motors;   


#ifdef SMALL_CPU

//B&R Prototype Hardware Input/Output Enumerations 
// small CPU - single suitcase

typedef enum {
	
	WF_Button=0,
	Arc_Button=1,
	Boost_1_Button=2,
	Boost_2_Button=3, 
	Jog_Button=4,
	GTII_WFAuto_Input=5, 	//RemoteSeqTest=5, change to GTII WF Auto Input
	RemoteSeqStart=6, 		//GTVI In Sequence
	RemoteSeqTest=7, 		//GVTI In Test Mode
	RemoteWfEnable=8, 		//if true use wpr0 else wpr1 (applies to automatic mode only) GTVI WF On
	RemoteRevDir=9, 		//RemoteWfEnable=9,  GTVI in FWD Direction *****inverted in automation studios***
	GTII_SeqStart_Input=10, //Spare10_DI=10,GTI Seq Start Input
	GTII_SeqStop_Input=11,	//GTII Seq Stop Switch Input
	MenuButton=12, 			//LCD Menu Button
	  
} Inputs;  

typedef enum {
	  
	PrimaryPS_Enable=0,
	Remote_Fwd_Rev_Sol_And_Rel=1,
	HotwirePS_Enable=2,
	GTII_WFAuto_Latch=3,
	GTII_SeqStart_Latch=4,
	GTII_SeqStop_Latch=5,
	Spare5_DO=6,
	Spare6_DO=7,
	  
} Outputs;  

//End B&R Prototype Hardware Input/Output Enumerations

#else

//New B&R Suitcase Hardware Input/Output Enumerations
// X20CP1585 CPU -  suitcase with remotes

typedef enum {

	GTII_SeqStop_Input=0,	//GTII Seq Stop Switch Input
	GTII_SeqStart_Input=1, 	//Spare10_DI=10,GTI Seq Start Input
	GTII_WFAuto_Input=2, 	//RemoteSeqTest=5, change to GTII WF Auto Input
	//spare3
	//spare4
	ControlModule_Estop_Active=5,
	RemoteSeqStart=6, 		//GTVI In Sequence
	RemoteSeqTest=7, 		//GVTI In Test Mode
	RemoteWfEnable=8, 		//if true use wpr0 else wpr1 (applies to automatic mode only) GTVI WF On
	RemoteRevDir=9, 		//RemoteWfEnable=9,  GTVI in FWD Direction *****inverted in automation studios***
	//spare10
	//spare11
	//MenuButton=12, 			//LCD Menu Button
	//spare13
	//spare14
	//spare15
	//spare16
	//spare17
	//spare18
	//spare19
	//spare20
	//spare21
	Jog_Button=22,
	Arc_Button=23,
	WF_Button=24,
	Boost_1_Button=25,
	Boost_2_Button=26,
	WF_Suitcase_EstopReset=27,
	WF_Suitcase_MenuButton=28,
	//spare29
	  
} Inputs;  

typedef enum {
	  
	PrimaryPS_Enable=0,
	Remote_Fwd_Rev_Sol_And_Rel=1,
	HotwirePS_Enable=2,
	GTII_WFAuto_Latch=3,
	GTII_SeqStart_Latch=4,
	GTII_SeqStop_Latch=5,	
	Estop_Reset=6,	
	//spare7,
	Arc_On_PL=8,
	Estop_Good_PL=9,
	//spare10
	//spare11
  
} Outputs;  
//End new B&R Suitcase Hardware Input/Output Enumerations

#endif
 
typedef enum {
	  
	PrimaryPS_AO=0,
	HotwirePS_AO=1,
	AcopianVoltCmd_AO=2,
	Spare3_AO=3,
	  
} AnalogOutputs;  
 
typedef enum {
	  
	Spare0_AI=0,
	Spare1_AI=1,
	Spare2_AI=2,
	Spare3_AI=3,
	  
} AnalogInputs;  




 /* Datatypes */

typedef struct MENU_obj 
{
	INT Counter;
	BOOL MenuButton;
	INT LastCounter; 
	BOOL Blink;
	BOOL Edit;
	Display_Setting_Code DisplayCode;

	Menu_Mode MenuMode;

	BOOL ArcDisplayBlinkEnable;
	BOOL RemDisplayBlinkEnable;
	BOOL WfDisplayBlinkEnable;
	BOOL WpDisplayBlinkEnable;
	BOOL HwDisplayBlinkEnable;

}MENU_obj;


typedef struct MACHINE_obj
{

	Primary_PS_Type PrimaryPSType;
	Hotwire_PS_Type HotwirePSType;
	Control_Mode ControlMode;
	//Run_State RunState;
	Run_Mode RunMode;
	Load_Defaults_Mode LoadDefaultsMode;

    Boost_Type Boost1Type;
    Boost_Type Boost2Type;

	UINT ErrorCode;
	BOOL AckErrors;

	BOOL EditMode;
	BOOL EstopFault;
	

    BOOL BoostLevel1Active;
    BOOL BoostLevel2Active;
    
    BOOL SystemNeedsReset;
	
}MACHINE_obj;



/*
Serial Port Driver
==================
*/

/*
typedef struct SerialPortDriver_typ{
	FRM_xopen_typ	Open; 							
	XOPENCONFIG		OpenConfig; 						
	FRM_gbuf_typ	GetBuffer; 				
	FRM_rbuf_typ	Release; 					 
	FRM_robuf_typ	ReleaseOutput; 			 
	FRM_write_typ	Write; 				 
	FRM_read_typ	Read; 	
	FRM_mode_typ	Mode;
	FRM_close_typ 	Close;
		
	UINT 			StatusOpen;
	UINT 			StatusWrite;
	UINT 			StatusRead;
	UINT 			StatusGetBuffer;
	UINT 			StatusReleaseOutputBuffer; 
	UINT 			StatusMode; 
	UINT 			StatusClose; 

	UDINT 			Ident; 								 
	UDINT* 			pSendBuffer;
	UDINT*   		pReadBuffer; 
	UINT   			SendLength;
	UINT   			ReadLength;
	UINT   			l; 

	UINT			txRequest;
	UINT			rxReady;
	UINT			txMessageCounter;
	UINT			rxMessageCounter;
	UINT 			txError;
	UINT 			rxError;
	UINT 			initError;

	STRING 			sDevice[32];
	STRING 			sMode[64]; 				
	STRING 			sModeNew[64];
	USINT			_parity;
	UINT    		txDataLen;

	USINT			txData[256];
	plcstring		rxData[256];
	STRING 			sInput[64];
				     	
}SerialPortDriver_typ;


//prototypes
void SerialPortInit(SerialPortDriver_typ *p, char *deviceStr, char *baudRateStr ) ;
void SerialPortClose(SerialPortDriver_typ *p) ;
void SerialPortCyclic(SerialPortDriver_typ *p) ;

typedef struct SCREEN_obj
{	
	 unsigned short SetPage;
	 unsigned short ReadPage;
	 unsigned short GotoScreen;
	 unsigned short ReturnScreen;
	 unsigned short Index;
	 unsigned short NavMenu;
	 unsigned short Update;
	 plcbit btn_YES;
	 plcbit btn_NO;
	 plcbit btn[32];
	 plcbit btnBar[32];
	 plcbit btnMenu[10];
	 plcbit btnSave;
	 plcbit btnIndex;
	 unsigned short vis[32];
	 unsigned short visBar[32];
	 unsigned short visSave;
	 unsigned short colorBackground;
	 unsigned short color[8];
	 unsigned short colorBar[8];
	 unsigned short colorSave;
	 signed long Param[10];
	 float fParam[10];
	 unsigned short Language;
	 unsigned short Units;
	 unsigned short AngleUnits;
	 float UnitsFactor;
	 float UnitsOffset;
	 unsigned long LifeSign;
	 unsigned char AccessLevel;
	 unsigned short menubtnColor[10];
	 
}SCREEN_obj;		

*/
typedef struct USERVALUES_obj
{	

	//general
	float WF_Speed;
	float WP_Freq;
	float ARC_Amps;
	float HW_Amps;

	//boost
    

    float WF_Speed_Boost_Level_1;
    float WF_Speed_Boost_Level_2;

	float WP_Freq_Boost_Level_1;
	float WP_Freq_Boost_Level_2;

	float ARC_Amps_Boost_Level_1;
	float ARC_Amps_Boost_Level_2;

	float HW_Amps_Boost_Level_1;
	float HW_Amps_Boost_Level_2;

	Boost_Type Boost1Type;
    Boost_Type Boost2Type;

    //delays
    float WF_On_Delay_Time;
	float WP_On_Delay_Time;
	float ARC_On_Delay_Time;
	float HW_On_Delay_Time;

	float WF_Off_Delay_Time;
	float WP_Off_Delay_Time;
	float HW_Off_Delay_Time;
	float ARC_Off_Delay_Time;
	
	//ramps
    float WF_On_Ramp_Time;
	float WP_On_Ramp_Time;
	float ARC_On_Ramp_Time;
	float HW_On_Ramp_Time;

	float WF_Off_Ramp_Time;
	float WP_Off_Ramp_Time;
	float HW_Off_Ramp_Time;
	float ARC_Off_Ramp_Time;
	
	
	//cal factors
	float WF_Speed_Cal_Slope;
    float WF_Speed_Cal_Offset;

	float WP_Speed_Cal_Slope;
    float WP_Speed_Cal_Offset;

	float ARC_Amps_Cal_Slope;
	float ARC_Speed_Cal_Offset;

	float HW_Amps_Cal_Slope;
	float HW_Speed_Cal_Offset;

	//control
	Primary_PS_Type PrimaryPSType;
	Hotwire_PS_Type HotwirePSType;
	Control_Mode ControlMode;
	Run_Mode RunMode;

	BOOL FirstTimeThrough;  //if 0 then load defaults automatically


}USERVALUES_obj;	


typedef struct PWM_MODULE_STATUS_obj
{
	BOOL UnderVoltageError;
	BOOL VolatgeWarning;
	BOOL OverVoltageError;
	BOOL OvertempeatureError;
	
}PWM_MODULE_STATUS_obj;


typedef struct HARDWARE_obj
{	
	BOOL Inputs[32];
	BOOL Outputs[12];
	INT AnalogOutputs[4];
	INT AnalogInputs[4];
	PWM_MODULE_STATUS_obj PWM_Status;
	
}HARDWARE_obj;	


typedef struct MTR_HARDWARE_obj
{	
	UINT PeriodDurationPWM;
	INT PulseWidthPWM;
	INT Counter;
	INT CurrentMeasurement;

	BOOL PWMError;
	BOOL CurrentError;
	BOOL OverCurrentError;
	BOOL ClearError;
	
	BOOL StartCurrentMeasurement;
	

	BOOL ResetEncoder;
	

}MTR_HARDWARE_obj;		


typedef struct MTR_HARDWARE_CONFIG_obj
{	

	float CalSlope;
	float CalOffset;

	float  EngUnit;  //encoder pulses per engineering unit (inch, rotation)
	INT    MtrDir;  // Motor direction - was
	INT    EncDir;  // Motor direction - was
	
	//float TaskTimeInSec;

	UINT CyclesInOvercurrent;

	UINT CyclesInOvercurrentLimit;
	
	float CurrentLimit;
	
}MTR_HARDWARE_CONFIG_obj;		



typedef struct MTR_MISC_obj
{	

	AzzRamp_typ Ramp;
	//LCRPT2o_typ Trajectory;
	//HCRFIFO_typ FIFODelay;
	//float FIFODelay[FIFO_DLY+1];
	
	AzzSlimPID_typ PID;
	azzslimpid_par_typ PIDParameters;

	INT LastPosition;
	INT CurrentPosition;
	DINT DeltaPosition;

	
} MTR_MISC_obj;	



typedef struct MTR_COMMAND_obj
{	

	SM_obj command_sm;

	BOOL Start;
	BOOL Stop;
	BOOL Reset;

} MTR_COMMAND_obj;		


typedef struct MTR_PARAMETERS_obj
{	
	float SpeedSetpoint; //ipm, hz
	float OnRampTime;
	float OffRampTime;
	float OnDelayTime;
	float OffDelayTime;

	float BoostLevel1SetPoint; //ipm, hz
	float BoostLevel2SetPoint; //ipm, hz

} MTR_PARAMETERS_obj;	


typedef struct MTR_obj
{	
	MTR_HARDWARE_obj Hardware;
	MTR_PARAMETERS_obj Parameters;
	MTR_COMMAND_obj Command;
	MTR_MISC_obj Misc;

	MTR_HARDWARE_CONFIG_obj HardwareConfig;

	
	float CurrentVelocity;
	float CommandVelocity;
	
	UINT Status;

} MTR_obj;		




typedef struct PS_HARDWARE_obj
{	

	float AnologCommandVoltage;  //analog output command voltage to PS
	BOOL Enable;

}PS_HARDWARE_obj;		


typedef struct PS_HARDWARE_CONFIG_obj
{	
	float CalSlope;
	float CalOffset;
	float AmpsPerVoltCommand;  //amps per volt of command
	float AmpsPerVoltFeedback;  //amps per volt of feedback
	
	//float TaskTimeInSec;

}PS_HARDWARE_CONFIG_obj;		



typedef struct PS_MISC_obj
{	
	AzzRamp_typ Ramp;
	//LCRPT2o_typ Trajectory;
	//HCRFIFO_typ FIFODelay;
	//float FIFODelay[FIFO_DLY+1];

} PS_MISC_obj;	



typedef struct PS_COMMAND_obj
{	

	SM_obj command_sm;

	BOOL Start;
	BOOL Stop;
	BOOL Reset;

} PS_COMMAND_obj;		


typedef struct PS_PARAMETERS_obj
{	
	float AmpsSetpoint;
	float OnRampTime;
	float OffRampTime;
	float OnDelayTime;
	float OffDelayTime;

	float BoostLevel1SetPoint; //amps
	float BoostLevel2SetPoint; //amps

} PS_PARAMETERS_obj;	


typedef struct PS_obj
{	
	PS_HARDWARE_obj Hardware;
	PS_PARAMETERS_obj Parameters;
	PS_COMMAND_obj Command;
	PS_MISC_obj Misc;

	PS_HARDWARE_CONFIG_obj HardwareConfig;


	float CurrentAmps;
	float CommandAmps;

	UINT Status;

} PS_obj;		

#if 0
/*******************************/
/****** LCD Commands **********/

#define LCD_SET_CURSOR 			0
#define LCD_BLINK_CURSOR_ON 	1
#define LCD_BLINK_CURSOR_OFF 	2
#define LCD_UNDERLINE_ON		3
#define LCD_UNDERLINE_OFF		4
#define LCD_SET_CONTRAST		5
#define LCD_SET_BACKLIGHT		6

#define LCD_TURN_ON_DISPLAY		7
#define LCD_TURN_OFF_DISPLAY	8
#define LCD_CLEAR_DISPLAY		9


typedef struct LCD_CMD_typ{
	UINT len;	
	char cmd[6];
}LCD_CMD_typ;	


static const  LCD_CMD_typ  LCD_CMD[10]=
{ 
	{3, {0xFE,0x45,0x00}}, 	//LCD_SET_CURSOR
	{2, {0xFE,0x4B}}, 		// LCD_BLINK_CURSOR_ON
	{2, {0xFE,0x4C}}, 		// LCD_BLINK_CURSOR_OFF
	{2, {0xFE,0x47}}, 		// LCD_UNDERLINE_ON
	{2, {0xFE,0x48}}, 		// LCD_UNDERLINE_OFF
	{3, {0xFE,0x52}}, 		// LCD_SET_CONTRAST
	{3, {0xFE,0x53}}, 		// LCD_SET_BACKLIGHT	
	{2, {0xFE,0x41}}, 		// LCD_TURN_ON_DISPLAY
	{2, {0xFE,0x42}}, 		// LCD_TURN_OFF_DISPLAY
	{2, {0xFE,0x51}}, 		// LCD_CLEAR_DISPLAY

};



#endif 0
	 
#endif /* GLOBDEF_H_ */

