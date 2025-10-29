/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Package: 
 * File: WSI_types.h
 * Author: ckaczkowski
 * Created: 17 June 2014
 *******************************************************************/

#include <bur/plctypes.h>


#ifndef _WSITYPES_H
#define _WSITYPES_H

#define PARAM_NUM_MAX  		256
#define PARAM_INT_NUM_MAX  	8
#define PARAM_ENUM_NUM_MAX  32
#define PARAM_STR_NUM_MAX   16
//#define PARAM_ENUM_NUM_MAX  8
//#define PARAM_STR_NUM_MAX   8
#define VAR_NUM_MAX    		128
#define ENUM_VAR_MAX   		32
#define INT_VAR_MAX    		32
#define STR_VAR_MAX    		64
//#define SEGM_NUM_MAX   		31
#define SEGM_NUM_MAX   		32
#define TXT_RESOURCES_MAX 	64
#define RES_TXT_LEN 		16
#define PAGES_MAX			64


#include <..\..\lib\Statelib.h>

#include <..\..\Universal\SuperDefinition.h>



/////////////////////////////////////////////////////
typedef struct AzzNode_typ
{
	float X;
	float Y;
	
} AzzNode_typ;

typedef struct AzzLinSpline
{
	/* VAR_INPUT (analog) */
	unsigned short NbNodes;
	AzzNode_typ* pNodeVec;
	unsigned char Smooth;
	float Rness;
	signed char DisMode;
	float x;
	/* VAR_OUTPUT (analog) */
	float y;
	unsigned short status;
	/* VAR (analog) */
	INT _i;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Invert;
	plcbit Update;
} AzzLinSpline_typ;


// New Functions from Paul
typedef struct AzzTON {
  BOOL IN;
  TIME PT;
  BOOL Q;
  TIME ET;
  TIME StartTime;
  BOOL M;
  UDINT Restart;
} AzzTON_typ;

typedef struct AzzMovAvgFlt {
	/* VAR_INPUT (analog) */
	float x;
	unsigned long base;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float y;
	/* VAR (analog) */
	double sum_old;
	//signed long sum_old_dbl_2;
	unsigned long p_xold;
	unsigned short i_xold;
	unsigned long base_old;
	unsigned long bootkey_old;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR (digital) */
	plcbit enable_old;
} AzzMovAvgFlt_typ;


typedef struct AzzPIDTune{
	float nothing;
  
} AzzPIDTune_typ;

typedef struct AzzPID{
	float i_sum, d_old, e_fbk;
	DINT last_time;
} AzzPID_typ;

typedef struct azzslimpid_par {
	float Y_max;
	float Y_min;
	float Kp;
	float Tn;
	float Tv;
	float Kfbk;
} azzslimpid_par_typ;

typedef struct AzzSlimPID{
	/* VAR_INPUT (analog) */
	float W;
	float X;
	unsigned long request;
	azzslimpid_par_typ* pPar;
	/* VAR_OUTPUT (analog) */
	float e;
	float Y;
	float dt;
	unsigned short status;
	unsigned short addInfo;
	/* VAR (analog) */
	AzzMovAvgFlt_typ MovAvgFlt_inst;
	AzzPIDTune_typ PIDTune_inst;
	AzzPID_typ PID_inst;
	unsigned long requ_old;
	/* VAR_INPUT (digital) */
	plcbit enable;
	/* VAR (digital) */
	plcbit enable_old;
} AzzSlimPID_typ;

typedef struct AzzRamp{
	/* VAR_INPUT (analog) */
	float x;
	float dy_up;
	float dy_down;
	float y_max;
	float y_min;
	float y_set;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	float y;
	/* VAR (analog) */
	float dt;
  	float y_old;
	//signed long y_dbl_1;
	//signed long y_dbl_2;
	unsigned long timestore1;
	//unsigned long timestore2;
	//unsigned long getDtForceOff;
	//struct SysInfo sysinfo_inst;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit set;
	/* VAR_OUTPUT (digital) */
	plcbit x_reached;
	plcbit max_limit;
	plcbit min_limit;
} AzzRamp_typ;


typedef struct AzzLimScal {
  REAL x, x1, y1, x2, y2, y;
} AzzLimScal_typ;

typedef struct AzzScal{
  REAL x, x1, y1, x2, y2, y;
} AzzScal_typ;


//////////////////////////////////////////
///////////////////////////////////////////////////////////



typedef struct LogItem_typ{

	signed long timeStamp;
	signed short status; 
	unsigned long value; 
	plcstring  txt[32];
	
}LogItem_typ;

typedef struct WarnItem_typ{
	signed short status; 
	plcstring  txt[32];
	
}WarnItem_typ;

typedef struct ERROR_Mgr_typ{
	SINT Error;
	unsigned short lastItemIndx;	
	unsigned short history[128];
	LogItem_typ logs[128];
	UINT warnings;

}ERROR_Mgr_typ;



/*********************************************************************/

/***********   VECTOR STATE        ***************************************/

/*********************************************************************/


#define vectSt_Mode_OFF 0
#define vectSt_Mode_Idle 1
#define vectSt_Mode_Hold 2
#define vectSt_Mode_Home 3
#define vectSt_Mode_Pos 4
#define vectSt_Mode_Vel 5
#define vectSt_Mode_Torq 6
#define vectSt_Mode_Volt 7
#define vectSt_Mode_PT 	 8
#define vectSt_Move_Abs  9
#define vectSt_Move_Rel  10  
#define vectSt_Mode_Custom 11
#define vectSt_Mode_Drive_Independant 12



typedef struct STATE_VECTOR_typ
{ 	
	//plcbit OK;
	float x; 	// value
	float x2; 	// value
	float vel; 	// first derivative
	float acc; 	// second derivative
	float amp; 	// current amps
	float volt; // volts
	float softLimits[2]; // software limits
	//DINT Center;
	float Center;
	signed short Status;
	signed short Mode;
	
}STATE_VECTOR_typ;

typedef struct StateVectorProxy_typ{
	plcstring Name[16];
	UINT Indx;
   /* VAR_INPUT (analog) */	
   UINT simul;
   UINT testMode;
   UINT xCMD;
   UINT cmdWord;
   float fParam[8];
   DINT Param[8];
   unsigned short MCRReg;
   unsigned long StatusReg;
   USINT xCmd_busy;
   float InVal;	
   float maxVel;
   float maxAcc;
   unsigned short curStep;
   signed short Mode;
   /* VAR_OUTPUT (analog) */
   STATE_VECTOR_typ OutVect;
   signed short Status;
   STATE_VECTOR_typ FbkVect;
   DINT actPosDINT;
   INT FbkWarning;
   UDINT VirStatus;
   
	
}StateVectorProxy_typ;

///////////////////////////////////////////////
/* Units enumeration*/

typedef enum UNIT_Enum
{	
	unitIU = 0,
	unitLength = 1,
	unitSpeed = 2,
	unitAcc = 3,
	unitRot = 4,
	unitTime = 5,
	unitAmp = 6,
	unitVolt = 7,
	unitFreq = 8,
	unitPerc = 9,
	unitAngle = 10,
	unitMaxIndx = 11,
	
} UNIT_Enum;


///////////////////////////////////////////////////////////
 
///  Types for NODE variable
 
  typedef struct Scaled_Var_obj{
	 float* pfVal;
	 plcstring name[32];
	 plcstring enumTXT[32+1];
	 plcstring VarName[32];
	 UINT Indx;
	 
	 UNIT_Enum	unitType;
	 UINT  PlcUnitIndx;
	 UINT  DefaultUnit;
	 int precision;
	 UINT segm;
	 UINT type;
	 
}Scaled_Var_obj;

 typedef struct Integer_Var_obj{
	 DINT* pVal;
	 plcstring name[32];
	 plcstring enumTXT[32+1];
	 plcstring VarName[32];
	 UINT Indx;

	 DINT Min;
	 DINT Max;
	 DINT DefaultVal;
	 UINT segm;
	 
 }Integer_Var_obj; 

 typedef struct String_Var_obj{
	 plcstring* pStr;
	 plcstring name[32];
	 plcstring enumTXT[32+1];
	 plcstring VarName[32];
	 UINT Indx;

	 UINT Len;
	 UINT Type;

	 UINT segm; // can be used in future 
	 
 }String_Var_obj; 

 
 typedef struct Enum_Var_obj{
	 UDINT pVal;
	 plcstring name[32];
	 plcstring enumTXT[32+1];
	 plcstring VarName[32];
	 UINT Indx;
	 UINT TxtIndx; // Index in Cfg.

	 DINT Min;
	 DINT Max;
	 UINT segm;
	 
 } Enum_Var_obj;
  
 typedef struct SegmentsTable_obj{ 	
	UINT segm_num;
	UINT param_segm_num;
	UINT ParamIndx[PARAM_NUM_MAX]; //PARAM_NUM_MAX
 }SegmentsTable_obj;
 
  typedef struct Unit_Scale_obj{
 
	 float gain;
	 float offset;
	 
	 INT extraDigits;
 
	 float incrGain;
	 float incrOffset;
 
	 STRING txt[8];
 
  }Unit_Scale_obj;
 
  typedef struct Units_obj{
	 UNIT_Enum	unitType;
	 Unit_Scale_obj unitScale[SUB_UNIT_NUM];
  }Units_obj;



/***********************************************************************************/

/***********   Conifg & Current Config   Types        ***************************************/

/***********************************************************************************/


/* Datatypes and datatypes of function blocks */
typedef struct Cfg_Param_typ
{	
	UNIT_Enum unit;
	float min;
	float max;
	float incrMin;
	float incrMax;
	float defaultVal;
	
	
} Cfg_Param_typ;

typedef struct Cfg_Param_INT_typ{

	DINT min;
	DINT max;
	DINT defaultVal;
	
}Cfg_Param_INT_typ;

typedef struct Cfg_Param_STR_typ{
	UINT len;
	plcstring defaultStr[32];
	
}Cfg_Param_STR_typ;


typedef struct PAGE_DICT_typ{
	UINT indx;
	plcstring enumTXT[32+1];
	
}PAGE_DICT_typ;




typedef struct CurCfg_Param_typ
{	float val[SEGM_NUM_MAX+1];
	float incr;
	float min;
	float max;
	SINT  segmYes;
} CurCfg_Param_typ;

typedef struct CurCfg_Param_INT_typ
{	DINT val[SEGM_NUM_MAX+1];
	float incr;
	float min;
	float max;
	SINT  segmYes;
} CurCfg_Param_INT_typ;

typedef struct CurCfg_Param_STR_typ
{	
	plcstring str[32];
	UINT len;
	UINT type;
} CurCfg_Param_STR_typ;


/* Datatypes and datatypes of function blocks */
typedef struct Calib_Param_typ
{	
	UINT num;
	float val[10];
	
} Calib_Param_typ;


typedef struct CurCfg_typ
{	
	UDINT Signature;
	plcstring WeldProgramName[16];
	CurCfg_Param_typ Param[PARAM_NUM_MAX];
	CurCfg_Param_INT_typ Param_INT[PARAM_INT_NUM_MAX];
	CurCfg_Param_INT_typ Param_ENUM[PARAM_ENUM_NUM_MAX];
	CurCfg_Param_STR_typ Param_STR[PARAM_STR_NUM_MAX]; //  Not Segmented - could be added in the future
	
	plcstring NextProgramName[32];
	plcstring CurProgramName[32];
	plcstring FreeText[63];
	USINT PleaseUpdate;
	USINT Dirty;
} CurCfg_typ;

typedef struct Head_TVL_Cfg_typ{

	UINT PROC_Indx; 
	UINT VECT_Indx; 
	UINT TML_Indx;
	UINT DC_Indx;
	
	DINT Enc_Dir;
	float EncPerTick;
	float EncPerTick_SecEnc;
	//float EncNominator;
	double EncScale;
	//DINT EncDenominator;	
	DINT PinsPerRot;
	float DistPerTick;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;
	
}Head_TVL_Cfg_typ;

typedef struct Head_WF_Cfg_typ{

	UINT PROC_Indx; 
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT VECT_Aux_A_Indx; 	
	UINT VECT_Aux_B_Indx;
	UINT TML_Aux_A_Indx;
	UINT TML_Aux_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;
	
	
}Head_WF_Cfg_typ;

typedef struct Head_WP_Cfg_typ{

	UINT PROC_Indx; 	
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;
	
}Head_WP_Cfg_typ;


typedef struct Head_OSC_Cfg_typ{
	
	UINT PROC_Indx; 
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;

	float WheelJog_Gain1;
	float WheelJog_Gain2;
	UINT  WhelJog_Mode1;
	UINT  WhelJog_Mode2;
	UINT  WhelJog_OnlyWhenRun;

	UINT Limits_Harcoded;
	UINT DwellDir; 	
	
}Head_OSC_Cfg_typ;

typedef struct Head_WRST_Cfg_typ{
	
	UINT PROC_Indx; 
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;

	//float WheelJog_Gain1;
	//float WheelJog_Gain2;
	//UINT  WhelJog_Mode1;
	//UINT  WhelJog_Mode2;
	//UINT  WhelJog_OnlyWhenRun;

	UINT Limits_Harcoded;
	//UINT DwellDir; 	
	
}Head_WRST_Cfg_typ;


typedef struct Head_AVC_Cfg_typ{

	UINT PROC_Indx; 
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;
	
}Head_AVC_Cfg_typ;

typedef struct Head_INDX_Cfg_typ{

	UINT PROC_Indx; 
	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
	
	UINT TML_A_Indx;
	UINT TML_B_Indx;
	UINT DC_A_Indx;
	UINT DC_B_Indx;

	float BtnJog_Gain;
	UINT  BtnJog_Mode;
	
}Head_INDX_Cfg_typ;

typedef struct WB_Cfg_typ{

	UINT PROC_Indx; 
	UINT VECT_Indx; 
		
}WB_Cfg_typ;

typedef struct LampOnOFF_Cfg_typ{

	UINT VECT_A_Indx; 	
	UINT VECT_B_Indx;
		
}LampOnOFF_Cfg_typ;

typedef struct SwitchHead_Cfg_typ{

	UINT VECT_Indx; 	
	UINT TML_Indx;

	UINT VECT_Indx_2; 	
	UINT TML_Indx_2;
		
}SwitchHead_Cfg_typ;




typedef struct Head_GAS_Cfg_typ{

	UINT PROC_Indx; 
		
}Head_GAS_Cfg_typ;

typedef struct Head_COOL_Cfg_typ{

	UINT PROC_Indx; 
		
}Head_COOL_Cfg_typ;

typedef struct Head_TIG_Cfg_typ{

	UINT PROC_Indx; 
	UINT Arclink; 
		
}Head_TIG_Cfg_typ;

typedef struct Head_HW_Cfg_typ{

	UINT PROC_Indx; 
		
}Head_HW_Cfg_typ;

typedef struct Head_GEN_Cfg_typ{

	UINT Torch_Mode;
	UINT AVC_Mode;
	UINT TML_Setup_AutoLoad;
	
}Head_GEN_Cfg_typ;


typedef struct Head_Cfg_typ{

	Head_TVL_Cfg_typ TVL;	
	Head_WF_Cfg_typ WF;
	Head_WP_Cfg_typ WP;
	Head_OSC_Cfg_typ OSC;
	Head_WRST_Cfg_typ WRST;
	Head_AVC_Cfg_typ AVC;
	Head_COOL_Cfg_typ COOL;
	Head_TIG_Cfg_typ TIG;
	Head_HW_Cfg_typ HW;
	Head_GAS_Cfg_typ GAS;
	Head_GEN_Cfg_typ GEN;
	Head_INDX_Cfg_typ INDX;
	WB_Cfg_typ WB[WIRE_BLK_NUM];
	LampOnOFF_Cfg_typ LampOnOFF;
	SwitchHead_Cfg_typ SwitchHead;
	//UINT EvalateTVLservo_outputs;

}Head_Cfg_typ;
	


// This structure holds information for editable values and any values for HMI with their phisical location
// General rule is : if 	'' plcstring VarName[32]'' is empty use " UINT Indx" as index in proper(type determined)  cfg array from:
/*
	Cfg_Param_typ Param[PARAM_NUM_MAX];
	Cfg_Param_INT_typ Param_INT[PARAM_INT_NUM_MAX];
	Cfg_Param_INT_typ Param_ENUM[PARAM_ENUM_NUM_MAX];
	Cfg_Param_STR_typ Param_STR[PARAM_STR_NUM_MAX];
*/	

typedef struct Cfg_typ
{	
	// This declare - where to find instave of proper type , if can be segmented , and how to display on HMI 	
	Scaled_Var_obj ScaledVar[PARAM_NUM_MAX + VAR_NUM_MAX];
	Enum_Var_obj EnumVar[PARAM_ENUM_NUM_MAX + ENUM_VAR_MAX];
	Integer_Var_obj IntVar[PARAM_INT_NUM_MAX + INT_VAR_MAX];
	String_Var_obj StringVar[PARAM_STR_NUM_MAX + STR_VAR_MAX];

	// this declare how to edit proper type - it means : low up limit , increments etc.
	Cfg_Param_typ Param[PARAM_NUM_MAX];
	Cfg_Param_INT_typ Param_INT[PARAM_INT_NUM_MAX];
	Cfg_Param_INT_typ Param_ENUM[PARAM_ENUM_NUM_MAX];
	Cfg_Param_STR_typ Param_STR[PARAM_STR_NUM_MAX];

	

	// text resources for enumrated txt or any string
	plcstring TXT_Resources[TXT_RESOURCES_MAX][RES_TXT_LEN];
	// Dictionary of head nodes
	PAGE_DICT_typ PagesDict[PAGES_MAX];

	Head_Cfg_typ HeadCfg;
	
	
} Cfg_typ;

typedef struct CurMap_typ{

	float x ;
	float y ;
	float z ;
	float amp;
	float volt;
	float angle ;

}CurMap_typ;


typedef struct CurCalib_typ
{	
	UINT num;
	Calib_Param_typ Points[10];
	DINT GenSetup[32];
	float GenFloatValue[32];
	DINT  GenDintValue[32];	
	CurMap_typ Map[64];	
} CurCalib_typ;





///////////////////////////////////////////////////////////


/***********************************************/

/****        GENERIC PROCESS					****/

/************************************************/


/******   Generic Process Command *************/
#define prcsIDLE  		0
#define prcsSTART  		1
#define prcsSTOP   		2
#define prcsJOGpos 		3
#define prcsJOGneg 		4
#define prcsSTEPpos   	5
#define prcsSTEPneg   	6
#define prcsRESTART  	7
#define prcsForceON  		8
#define prcsHalt   		9
#define prcsRefrCalib   	10
#define prcsCalib   		11
#define prcsJump2Run   	12
#define prcsRetract   		13
#define prcsDirectCntrl 	14
#define prcsQuickSTOP   	15
#define prcsAlligment   	16
#define prcsQuickStart   	17
#define prcsSetHomePos   	18
#define prcsGoToPos   		19





/******   Generic Vector Command *************/
#define VECT_UpdateSignFilter		1	
#define VECT_ClearSignFilter		2
#define VECT_FullSignFilter			3
#define VECT_Set_Gain				4
#define VECT_Set_Gain_Zero			5
#define VECT_Set_PID_Param			6
#define VECT_Set_SATP				7
#define VECT_Set_SATP_Zero			8

#define VECT_Set_Param_0			10
#define VECT_Set_Param_1			11
#define VECT_Set_Param_2			12
#define VECT_Set_Param_3			13
#define VECT_Set_Param_4			14
#define VECT_Set_Param_5			15
#define VECT_Set_Param_6			16
#define VECT_Set_Param_7			17
#define VECT_Set_MaxError			18

#define VECT_Set_CmdWord			20

#define VECT_OSC_Start_Cmd			40
#define VECT_OSC_Stop_Cmd			41
#define VECT_OSC_Update_Cmd			42
#define VECT_OSC_Stop2_Cmd			43


#define VECT_Move_Abs_Cmd			22
#define VECT_Move_Rel_Cmd			23
#define VECT_Move_Stop_Cmd			24

#define VECT_ARC_Stop_Cmd			25
#define VECT_ARC_Start_Cmd			26

#define VECT_ZERO_AMPS_Cmd			27
#define VECT_DEFAULT_AMPS_Cmd		28

#define VECT_RESTART_SRV_Cmd		29

#define VECT_Set_KPP				30
#define VECT_Set_KPP_Zero			31

#define VECT_Upd_CACC				32
#define VECT_Upd_CSPD				33



#define VECT_Set_Act_Pos			35

#define VECT_Set_Fbk_1			    36
#define VECT_Set_Fbk_2			    37

#define VECT_Set_OUT0				50
#define VECT_Rst_OUT0				51
#define VECT_Set_OUT1				52
#define VECT_Rst_OUT1				53
#define VECT_Set_OUT4				54
#define VECT_Rst_OUT4				55

#define VECT_Set_FFL				60

#define VECT_Refresh_Mode			70



#define VECT_Set_OSC_Max 		80
#define VECT_Set_OSC_Min 		81
#define VECT_Set_OSC_Time		82
#define VECT_Set_OSC_DwellIn 	83
#define VECT_Set_OSC_DwellOut 	84
#define VECT_Set_OSC_Steer		85 
#define VECT_Set_OSC_Cmd_Param 	86
#define VECT_Set_OSC_Jog_Volt	87
#define VECT_Set_OSC_Jog_Speed	88
#define VECT_Set_OSC_Jog_Step	89
#define VECT_Set_OSC_Amplit 	90
#define VECT_Set_OSC_AllParam 	91
#define VECT_Set_OSC_Home 		92
#define VECT_Set_All_Run_Param 	93



/******   Generic Process Status *************/

typedef enum prcsSTATUS_Enum
{	
	prcsSTATUS_StartDly = 10,
	prcsSTATUS_RampUp 	= 11,
	prcsSTATUS_Run 		= 12,
	prcsSTATUS_StopDly 	= 13,
	prcsSTATUS_RampDwn 	= 14,
	prcsSTATUS_Stopped  = 15,
	prcsSTATUS_Fault  	= 16,
	prcsSTATUS_Jog  	= 17,
	prcsSTATUS_Step  	= 18,
	prcsSTATUS_Init  	= 19,
	prcsSTATUS_ForceON  = 20,
	prcsSTATUS_Retract 	= 21,
	prcsSTATUS_AVC_OK   = 22,
	prcsSTATUS_AVC_Hold = 23,
	prcsSTATUS_DirectCntrl = 24,
	prcsSTATUS_ForceCalib  = 25,
	prcsSTATUS_ReturnDly  	= 27,
	prcsSTATUS_Return  		= 28,
	prcsSTATUS_Alligment	= 29,
	
}prcsSTATUS_Enum;

/*********** Gen Process Types **********/

typedef struct GenProcess_typ
  {   
	  /* VAR_IN_OUT (analog and digital) */
	  //CurCfg_typ* pCurCfg; /* pointer to permanent memory with parameters */
	 // StateVectorProxy_typ *pVect; /* pointer to Vector State generator array */
	 INT ConfigReady;
	  plcstring Name[16]; /* Name of process */
	  USINT Disable;
	  /* VAR_INPUT (analog) */
	  //unsigned short VectStateNum; /* size of vectors array for process */	  
	  USINT xCMD; /* command from master */
	  float fParam[4]; /* command parameters */
	  DINT	Param[4]; /* command parameters */
	  /* VAR_OUTPUT (analog) */
	  prcsSTATUS_Enum Status; /* current status */
	  INT subStatus;
	  float Fbk[10]; /* current status */
	  INT FbkWarning;
	  /* VAR (digital) */
	  USINT _step; /* internal use */
	   /* VAR_INPUT (analog) */
	   INT TestMode;
	  
  } GenProcess_typ;
  
 ///////////////////////////////////////////////////////////

 
 /***********************************************/
 
 /****		   Hardware 							 ****/
 
 /************************************************/
 
 
 typedef struct X20MM4456_HARDWARE_obj
 {	 
	 
	 UDINT SerialNumber;
	 UINT ModuleID;
	 UINT HardwareVariant;
	 UINT FirmwareVersion;
 
	 
 
	 UINT PeriodDurationPWM;
	 BOOL UnderVoltageError;
	 BOOL VolatgeWarning;
	 BOOL OverVoltageError;
	 BOOL OvertempeatureError;
	 
	 INT PulseWidthPWM[4];
	 
	 BOOL StartLatch[4];
	 BOOL DitherDisable[4];
	 BOOL ClearError[4];
	 BOOL ShowMeanCurrent[4];
	 BOOL ResetCounter[4];
 
	 INT Counter[4];
	 INT CounterLatch[4];
 
	 
	 BOOL PWMError[4];
	 BOOL CurrentError[4];
	 BOOL OverCurrentError[4];
 
 
 }X20MM4456_HARDWARE_obj;		 
 
 typedef struct PENDANT_HARDWARE_intr {

		DINT TickIncr;
		DINT _ABEncoder;
		plcbit _Inp[16];
		
}PENDANT_HARDWARE_intr;

 
 typedef struct PENDANT_HARDWARE_obj
 {	 
	 plcstring RSDeviceStr[32];
	 UDINT SerialNumber;
	 UINT ModuleID;
	 UINT HardwareVariant;
	 UINT FirmwareVersion;
 
	 DINT ABEncoder;
	 plcbit NotWheelReset;
	 plcbit Outp[16];
	 plcbit Inp[16];

	 PENDANT_HARDWARE_intr _intr;  
 
	 
 }PENDANT_HARDWARE_obj;
 
 
 typedef struct HARDWARE_typ
 {
	//PENDANT_HARDWARE_obj Pend;
	X20MM4456_HARDWARE_obj PWM[HRD_PWM_MOD_NUM];
	INT Encoder[HRD_ENCODER_NUM];	
	plcbit Inp[HRD_DIG_INP_NUM];
	plcbit Outp[HRD_DIG_OUTP_NUM];
	INT Anal_Inp[HRD_AN_INP_NUM];
	INT Anal_Outp[HRD_AN_OUTP_NUM];
	plcbit CouplerOK[HRD_COUPLER_NUM];
	
 }HARDWARE_typ; 


///////////////////////////////////////////////////////////


/***********************************************/

/****        MASTER							****/

/************************************************/ 

 #define INTERF_NUM 2

// Command to master	
 #define mstrRESTART 1 // restart Master
 
 typedef struct MASTER_MSC_typ
 {
	INT _ABEncoder;
	
 }MASTER_MSC_typ;	


   typedef struct HMI_INTRF_intr_typ
 {
	INT _Wheel;
	UDINT _GlblBtnCmd[2];
	UINT _BtnCmd;
 }HMI_INTRF_intr_typ;
   
  typedef struct HMI_INTRF_typ
 {
	USINT Mode;
	USINT ModeLED;
	//USINT TestMode;
	USINT LCD_ENTR_LED;
	
	
	UINT BtnCmd; 
	UINT BtnLED;

	// This is for Android 
	UDINT GlblBtnCmd[2];	
	UDINT GlblBtnLED[2];
	plcbit TestMode;
	//
  
	plcbit LCDEntr;
	plcbit LCDEntr_PosEdge;
	plcbit LCDEntr_NegEdge;


	plcbit TickPlus;
	plcbit TickMinus;
  
	INT Wheel;
	INT WheelDelta;
	UINT tempIndx;
	TON_typ Tmr;

	HMI_INTRF_intr_typ _intr;	
	
 }HMI_INTRF_typ;

  typedef struct ANDROID_INTRF_typ
 {
 	/*
	//UINT Mode;
	UDINT TestMode; //  read only
	UDINT MasterMode; // read only
	UDINT UserLevel; // write only
	
	// This is from Buttons and encoder wheel 
	UDINT GlblBtnCmd[2]; // write only	
	UDINT GlblBtnLED[2]; // read only
	UDINT EntrBtn; // write only
	UDINT TickPlus; // write only
	UDINT TickMinus; // write only
	DINT Wheel;	// write only	
	/////
	
	// Screen View and Control
	UDINT FiredEvent; // write only
	UDINT PleaseUpdate;// write only
	UDINT curHeadID; // write only
	UDINT curLineID; // write only	
	
	
	UDINT Node2JumpID; // read only
	UDINT JumpLineID; // read only
	UDINT xCMD; // read only
	

	UDINT MsgTmr_Done; // read only
	UDINT MsgTmr_Duration; // read only
	plcstring Message[4][20+1];// read only

	plcstring Banner[32+1];// read only

	UDINT Alive ; // Write only - heartbeat from HMI

	DINT curNumSegments; // read only
	DINT maxNumSegments; // read only
	

	plcstring MasterModTXT[20+1]; // read only - Master Mode as Text
	plcstring UserLevelTXT[20+1]; // read only - USER Text as Text

	*/


	//UINT Mode;
	UINT TestMode; //  read only
	UINT MasterMode; // read only
	UINT UserLevel; // write only
	
	// This is from Buttons and encoder wheel 
	UDINT GlblBtnCmd[2]; // write only	
	UDINT GlblBtnLED[2]; // read only
	UINT EntrBtn; // write only
	UINT TickPlus; // write only
	UINT TickMinus; // write only
	INT Wheel;	// write only	
	/////
	
	// Screen View and Control
	UDINT FiredEvent; // write only
	UINT PleaseUpdate;// write only
	UINT curHeadID; // write only
	UINT curLineID; // write only	
	
	
	UINT Node2JumpID; // read only
	UINT JumpLineID; // read only
	UINT xCMD; // read only
	UINT PLC_Changes;// read only and clear by android
	/*

	0x0001 - new curCfg loaded
	0x0002 - new units loaded
	0x0004 - new segmented list
	0x0008 - new segement max

	*/
	

	UINT MsgTmr_Done; // read only
	plcstring Message[4][20+1];// read only
	UDINT MsgTmr_Duration; // read only

	plcstring Banner[64+1];// read only

	UINT Alive ; // Write only - heartbeat from HMI

	INT curNumSegments; // read only
	INT maxNumSegments; // read only
	

	plcstring MasterModTXT[64+1]; // read only - Master Mode as Text
	plcstring UserLevelTXT[20+1]; // read only - USER Text as Text
	
	
 }ANDROID_INTRF_typ;
   
  
  typedef struct HMI_typ
 {
 	// 
	UINT DynPageNodeIndx; // write by master
	plcstring ActiveBanner[32]; // write by master
	plcstring *pHMI_Lines[20]; // write by master
	
	//

	UINT curHeadID; //  -write by HMi device
	UINT curNodeID; //  -write by HMi device
	UDINT FiredEvent; //-write by HMi device
	UINT PleaseUpdate; // -write by HMi device - index of editing item

	
	plcstring* pBanner;  // Pointers to string - text write by HMI view control
	plcstring* pMessage[4]; // Pointers to string  - text write by HMI view control	
	USINT EditMode; // write by HMI view control
	UINT  UserLevel; // write by HMI view control

	UINT Node2JumpID; // write by HMI view control
	UINT JumpLineID; // write by HMI view control
	UINT xCMD; // write by HMI view control
	UINT PLC_Changes;// write by HMI  and clear by android

	/*
	0x0001 - new curCfg loaded
	0x0002 - new units loaded
	0x0004 - new segmented list
	0x0008 - new segement max
	*/

	// for internal use
	USINT FastBlink;
	USINT SlowBlink;
	UINT oldDynPageNodeIndx; // 
	plcbit DynPageActive;
	Calib_Param_typ tempCalib;
	float oldK1;
	float oldK2;
	INT Indx;
	USINT selectNextWeldProgram_step;
	UINT CalibStep;
	BOOL CalibStepDone;
	SINT CalibMode;
	UINT CalibPoint[8];
	float CalibPointCmd[8];
	prcsSTATUS_Enum _ProcStatus;
	TON_typ MsgTmr;
	plcbit ROM_USB;
	
	
 }HMI_typ;

 typedef struct Objects_Pointers_typ{

	 UDINT pArcLink_IP_Address;
	UDINT pPS_Param_Set[TIG_PS_NUM];	

 }Objects_Pointers_typ;
 
  
typedef struct MASTER_typ
{
   USINT xCmd;	
   USINT Active;
   Cfg_typ* pCfg;
   CurCfg_typ* pCurCfg;
   CurCalib_typ* pCalib;
   CurCalib_typ* pDefaultCalib;
   Objects_Pointers_typ Objects;
   
   USINT LOCKED;
   //ERROR_Mgr_typ *pLog;
   //GenProcess_typ *pProc;
   UDINT pUnits;
   UDINT pScaledVar;
   INT *pScaledUnitIndx;
   UDINT *pSegmentTable;
   DINT curSegm[SEGM_TYPE_NUM];
   //unsigned short ProcNum;
   HMI_INTRF_typ Intrf[INTERF_NUM];
   UDINT pNode[2];
   UINT nodesNum[2];
   HMI_typ Hmi[INTERF_NUM];
   SM_obj sm; 
   UINT ToolHeadID;
   USINT Mode;
   INT TestMode;
   INT CalibType;
   ANDROID_INTRF_typ AndIntrf[INTERF_NUM];
   USINT WireDisabled;
   UINT SeqStep;
   UINT SubSeqStep;
   plcbit ESTOP;
   plcbit *pESTOP_Inp;
   plcbit *pESTOP_Out;
   plcstring Password[16];

   INT numSegm;
   INT numZone;
  // DINT EncPerTick;
   INT ticksPerSegm;
   INT ticksPerZone;
   float FrameDia;

   //UINT OscNum;
   //DINT directTVL;
  // DINT HomePos_TVL;
  // DINT HomeZone_TVL;
    
   UINT paramLastNum;
   UINT varLastNum ;
   UINT paramEnumLastNum;
   UINT varEnumLastNum ;
   UINT paramIntgLastNum;
   UINT varIntLastNum ;
   UINT paramStrLastNum;
   UINT varStrLastNum ;
	   
}MASTER_typ;



/***********************************************/

/********			 Generic Motor 	    **********/

/***********************************************/
typedef struct GEN_MTR_msc_type
{	

		AzzRamp_typ Ramp;		
		
		AzzSlimPID_typ PID;
		azzslimpid_par_typ PIDParameters;
	

		/*
		LCRRamp_typ Ramp;		
		
		LCRSlimPID_typ PID;
		lcrslimpid_par_typ PIDParameters;
		*/

		DINT Pos;
		INT _pos;
		float _vel;
		float _amp;
		float _volt;
		DINT deltaPos;


		//float kVel;
		//float kAcc;
		float kPos;

		
} GEN_MTR_msc_type;	


typedef struct GEN_MTR_obj
{

	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;
	float actPos;
	float actVel;
	float fParam[2];

	struct Hrdw
	{	
		UINT *pPeriodDurationPWM;
		INT *pPulseWidthPWM;
		BOOL *pClearError;
		INT *pEncFbk;
		INT *pAnalogFbk;
		
		
	}Hrdw;	
	
	struct Cfg{
		DINT units;
		float enc_dir;
		float mtr_dir;
		float rev_scale;
		//float loopPeriod;
				
	}Cfg;

	GEN_MTR_msc_type msc;

	/*
	struct msc
	{	

		LCRRamp_typ Ramp;
		//LCRPT2o_typ Trajectory;
		//HCRFIFO_typ FIFODelay;
		//float FIFODelay[FIFO_DLY+1];
		
		LCRSlimPID_typ PID;
		lcrslimpid_par_typ PIDParameters;

		DINT Pos;
		INT _pos;
		float _vel;
		float _amp;
		float _volt;
		DINT deltaPos;

		//float kVel;
		//float kAcc;
		float kPos;

		
	} msc;	
	*/

}GEN_MTR_obj;

///////////////////////////////////////////////////////////

typedef struct Cfg_DC2DIRMTR_obj{

	float MtrDir;
	BOOL *pQ1_output;
	BOOL *pQ2_output;
	BOOL *pQ3_output;
	BOOL *pQ4_output;
	
}Cfg_DC2DIRMTR_obj;

typedef	struct msc_DC2DIRMTR_obj
{		
	UINT sw_step;
	
} msc_DC2DIRMTR_obj;	

typedef struct DC2DIRMTR_obj
{
	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;	
	float fParam[2];
	Cfg_DC2DIRMTR_obj Cfg;
	msc_DC2DIRMTR_obj msc;

}DC2DIRMTR_obj;

typedef struct TML_OUTPUTS_Cfg_obj{

	float MtrDir;
	UINT TML_Vect_Indx;
	UINT TML_Indx;
	UINT Out1_Indx;
	UINT Out2_Indx;
	

}TML_OUTPUTS_Cfg_obj;

typedef	struct TML_OUTPUTS_msc_obj
{		
	UINT sw_step;
	
} TML_OUTPUTS_msc_obj;

typedef struct TML_OUTPUTS_obj
{
	UDINT pState;
	plcstring Name[16];
	USINT Disable;
	USINT xCmd;	
	float fParam[2];
	TML_OUTPUTS_Cfg_obj Cfg;
	TML_OUTPUTS_msc_obj msc;

}TML_OUTPUTS_obj;


//////////////////////////////////////////////////////////


/***********************************************/

/****        B&R 	Screen   						****/

/************************************************/

 //*===COLORS===*/
#define TRANSP						0xFF
#define GREEN						10
#define DARK_GREEN					2
#define BLUE						1
#define BABY_BLUE					11
#define TEAL						168
#define RED							0x33
#define YELLOW						46
#define BLACK						0
#define WHITE						15
#define GRAY						7
#define DARK_NAVY					219
#define DARK_GRAY					8
#define LIGHT_GRAY					227
#define CYAN						196
#define MAGENTA						21
#define BROWN						86
#define LIGHT_BLUE					52


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
	USINT AccessLevel;
	unsigned short menubtnColor[10];
  } SCREEN_obj;	
///////////////////////////////////////////////////////////


/***********************************************/

/****        Technosoft Servos					****/

/************************************************/


// Commands definition
#define TML_IDLE 0
#define TML_RESTART 1
// 2 - 13 reserved for TML_RESTART
#define TML_WAIT_FOR_DOWNLOAD 15
// 14 reserved for TML_RESTART
#define TML_HOLD_MODE 17
// 15 - 19 reserved for TML_HOLD_MODE
#define TML_POS_MODE 20
// 21 - 29 reserved for TML_POS_MODE
#define TML_VEL_MODE 30
// 31 - 39 reserved for TML_VEL_MODE
#define TML_TORQ_MODE 40
// 41 - 49 reserved for TML_TORQ_MODE
#define TML_VOLT_MODE 50
// 51 - 59 reserved for TML_VOLT_MODE
#define TML_PT_MODE 60
// 61 - 79 reserved for TML_PT_MODE
#define TML_CUST_MODE 80
// 81 - 89 reserved for TML_PT_MODE
#define TML_OSC_CUST_CMD 90
// 90 - 99 reserved for TML_PT_MODE
#define TML_DOWNL			100	

/// New Osc

#define TML_SET_P_Max 		110
#define TML_SET_P_Min 		111
#define TML_SET_P_Time		112
#define TML_SET_P_DwellIn 	113
#define TML_SET_P_DwellOut 	114
#define TML_SET_P_Steer		115 
#define TML_SET_P_Jog		117
#define TML_SET_P_Amplit 	118
#define TML_SET_Cmd_Word 	119


#define TML_SET_Cmd_Param 	120
#define TML_SET_Home 		121
#define TML_SET_Home2 		122




#define TML_KPP				140
#define TML_KIP				141
#define TML_KIP_Zero		145
#define TML_KPP_Zero		146
#define TML_SATP			150
#define TML_SATP_Zero		151

#define TML_UpdateFilter	160	
#define TML_ClearFilter		161	
#define TML_FullFilter		162
#define TML_SetGain			170


#define TML_MOVE_REL_CMD 175
// 175 - 190 reserved for TML_MOVE_REL_CMD

#define TML_RESET_ONLY_CMD 185

#define TML_ARC_CUST_CMD 190

#define TML_SET_OUT0_CMD 200
#define TML_RST_OUT0_CMD 201
#define TML_SET_OUT1_CMD 202
#define TML_RST_OUT1_CMD 203

#define TML_AXIS_ON_CMD  210
#define TML_AXIS_OFF_CMD 211

#define TML_MOVE_ABS_CMD 220

#define TML_UPD_CACC 230
#define TML_UPD_CSPD 231

#define TML_SET_SCR_1 235 //select Feedback 1
#define TML_SET_SCR_2 240 //select Feedback 2



#define TML_SetGain_Zero	173


////////


#define TML_DOWNL_DONE	222	
#define TML_ERROR			255

// Status Mask
#define ERR_CANbus_MSK 			0x0001
#define ERR_ShortCircuit_MSK 	0x0002
#define ERR_InvalidSetup_MSK 	0x0004
#define ERR_Cntrl_MSK 			0x0008
#define ERR_RS232_MSK 			0x0010

//#define SREG_Fault_MSK 				0x80000000
#define SREG_Fault_MSK 				0x80000000

#define SREG_MotionCompleted_MSK 	0x00000400


//  Enumeration for new style OSC Parameters
#define V_Param_Cmd_Param	0
#define V_Param_Time 		1
#define V_Param_DwellIn 	2
#define V_Param_DwellOut 	3


#define V_fParam_Amplit 	0
#define V_fParam_Max 		1 
#define V_fParam_Min 		2
#define V_fParam_Jog		3
#define V_fParam_Steer 		4
#define V_fParam_HomePos 	5




/******  TML STatus enumeration    *****/

typedef enum TML_STAT_Enum
{	
	TML_STAT_DRIVE_ERR = 	-4,
	TML_STAT_VER_ERR   =	-3,
	TML_STAT_INIT  	   =	-2,
	TML_HEARTBEAT_ERR  = 	-1,
	TML_STAT_DEAD  	   =	0,
	TML_STAT_READY 	   =	1,
	TML_STAT_IDLE 	   =	2,
	TML_STAT_HOLD_INIT 	   =	3,
	TML_STAT_HOLD_OK 	   =	4,
	TML_STAT_POS_MODE_INIT  =	5,
	TML_STAT_POS_MODE_OK    =	6,
	TML_STAT_VEL_MODE_INIT  =	7,
	TML_STAT_VEL_MODE_OK 	=	8,
	TML_STAT_TORQ_MODE_INIT =	9,
	TML_STAT_TORQ_MODE_OK 	=	10,
	TML_STAT_VOLT_MODE_INIT =	11,
	TML_STAT_VOLT_MODE_OK 	=	12,
	TML_STAT_PT_MODE_INIT 	=	13,
	TML_STAT_PT_MODE_OK 	=	14,
	TML_STAT_CUSTOM_INIT 	   =	15,
	TML_STAT_CUSTOM_OK 	   =	16,
	TML_STAT_DRIVE_SELF_CNTRL 	   =	17,
	
}TML_STAT_Enum;	

typedef struct TML_typ
{	
	StateVectorProxy_typ* pState; 
	plcstring Name[16];
	USINT Disable;
	unsigned long  ProgramVersion;
	unsigned long  FirmwareVersion;
	
	float PosFBK;
	USINT xCMD;
	TML_STAT_Enum Status;
	unsigned long StatusReg;
	unsigned short ErrorReg;
	unsigned short MCRReg;	
	plcbit Error;
	plcbit EnableInp;
	unsigned short AnInp[2];
	plcbit DigInp[4];
	plcbit DigOut_stat[3];
	
	// DigInp[2] = LSP
	// DigInp[3] = LSN
	// AnInp[0] = REF
	// AnInp[1] = FDBK
	struct _cfg{
		DINT units;
		float rev_motor;
		float rev_scale;
		float actPos2_scale;
		UINT  absFbk;
		DINT absLastPos;
		DINT *pAbsLastPos;
		float loopPeriod;
		float maxVel;
		float maxAcc;
		float signFilter;
		//DINT  satp;
		DINT  satpMax;
		DINT  kip;
		DINT  kipMax;
		DINT  kpp;
		DINT  kppMax;
		DINT  SATSMax;
		plcstring programName[32];
		float HeartBeat;
		float PVTPeriod;
		UINT MtrType;
		//float sftLimitPos;
		//float sftLimitNeg;
		UINT NotAutoDownload;
		
		UDINT Appl_ID_adr;
		unsigned short AnalLimitPos;
		unsigned short AnalLimitNeg;
		
	}_cfg;
	
	struct _int{
		DINT _pos;
		DINT _deltaPos;
		UINT _posCnt;
		float _floatPos;
		float _vel;
		float _amp;
		float _volt;
		float kVel;
		float kAcc;
		float kPos;
		float kVolt;
		UINT PVTPeriod;
		SINT Init;
		float startPos;
		UINT restartCnt;
		UINT cnt;
		//UINT msgProcess;
		UINT KickCntr;
		DINT const_PT;
		TON_typ dly_Tmr;
			
	}_int;
	
} TML_typ;


typedef struct TML_CHAN_HRDW_typ
{
	USINT AxisNum;
	
	USINT TXCountReadBack;
	//
	USINT RXDataSize0;
	USINT RXCount;
	USINT buff[4];
	UDINT RXIdent0;
	UINT  RXData0Word0[4];
	//	
	USINT TXCount;
	USINT TXDataSize;
	UDINT TXIdent;
	UINT  TXDataWord0[4];
	
	USINT _oldRxCount;
	USINT _indx;

}TML_CHAN_HRDW_typ;

typedef struct TML_Axis_typ
{	unsigned short AxisID;
	unsigned long  Version;  
	unsigned long  ProgramVersion;
	unsigned long  FirmwareVersion;
	USINT  Flag;
	USINT  AxisState;
	unsigned short DriveStatusRegisterL;
	unsigned short DriveStatusRegisterH;
	unsigned short DriveErrorRegister;
	unsigned short PVTstatus;
	unsigned short DataLow;
	unsigned short DataHigh;
	unsigned short VarAddress;
	unsigned short SATS;
	signed long	   curPos;	
	signed long	   _curPos;
	signed long	   _lastPos;
	unsigned short pointer;
	short deltaAPOS;
	//unsigned long  CmdWord;
	unsigned short PointCounter;
	TON_typ HeartBeat;
	TON_typ InitTmr;
	
	
	
} TML_Axis_typ;



typedef struct TML_MGR_typ
{
	UDINT pSendData[2]; // two channels
	UDINT pRecData[2]; // two channels
	TML_CHAN_HRDW_typ hrdw[2];
	UDINT Firmware[2];
	USINT BlockTrans[2];
	TML_Axis_typ *pAxis[2];
	TML_typ *pTML[2];
	UDINT pLog;


}TML_MGR_typ;



typedef struct RS_Channel_typ{
	USINT Status;
	USINT Data[32];
	UINT Len;
	UDINT Cntr;
}RS_Channel_typ;

///////////////////////////////////////////////////////////


/***********************************************/

/********			 Files Operetion	    **********/

/***********************************************/

#define PRG_FILE_EXT ".prg"
#define PRG_NO_NEXT "None"



#define F_READ 				1
#define F_WRITE 			2
#define F_DELETE			3
#define F_FREE_MEM 			4

#define F_LOAD_CURCFG		5
#define F_SAVE_CURCFG		6
#define F_SAVE_AS_CURCFG	7

#define F_RENAME			8
#define F_COPY				9
#define F_GET_NEXT_FILE		10	
#define F_SET_NEXT_FILE		11	
#define F_READ_FILE_TO_TMP 	12
#define F_SAVE_AS_TMP 		13

#define F_READ_DIR 			20
#define F_READ_FILE_INF 	21
#define F_READ_DIR_FILELIST 22

#define F_OPEN_ONLY 		30
#define F_CREATE_ONLY 		31
#define F_WRITE_ONLY 		32
#define F_CLOSE_ONLY 		33

#define F_READ_FILES_CHAIN  40
#define F_EDIT_CHAIN  		41

#define F_SAVE_CURCFG_BIN		45
#define F_SAVE_AS_CURCFG_BIN	46
#define F_SAVE_SETUP_BIN		47
#define F_SAVE_AS_FULL_BIN		48
#define F_SAVE_AS_CALIB_BIN		49


#define F_MODE_BINARY		0
#define F_MODE_XML			1
#define F_MODE_BINARY_FULL	2
#define F_MODE_BINARY_CALIB	3



typedef struct XML_RW_HS_INT_typ
{
	UINT step;
	USINT          byErrorLevel;
	UINT           wStatus;
	UINT           wError;
	UDINT          dwIdent;
	UDINT 		   memlng;	
	
	FileOpen_typ   FOpen;
	FileClose_typ  FClose;
	FileCreate_typ FCreate;
	FileRead_typ   FRead;
	FileWrite_typ  FWrite;
	FileDelete_typ FDelete;
	DirInfo_typ	   FDirInfo;
	DirRead_typ	   FFileInfo;

	FileRename_typ FRename;
	FileCopy_typ   FCopy;

	UDINT OffsetToWeldProgramLinkMode;
	UDINT OffsetToWeldProgramRepeat;	

	UINT  PathDoneCnt;
	
		

}XML_RW_HS_INT_typ;

typedef struct FILE_SCHEDULE_typ
{
	plcstring nextFileName[32];
	DINT WeldProgramLinkMode;
	float WeldProgramRepeat;	
	
}FILE_SCHEDULE_typ;


typedef struct FILE_RW_typ
{

	USINT CMD;
	plcbit Done;
	plcbit Error;
	plcbit FileLocked;
	plcbit NewFileLoaded;
	USINT Mode;

	//plcstring varName[32];
	plcstring fileName[64];
	plcstring fileNewName[64];
	
	plcstring deviceName[32];
	plcstring targetDeviceName[32];

	plcstring MsgText[128];
	FILE_SCHEDULE_typ FileSchedule;
	
	
	CurCfg_typ curCfgTemp;
	fiDIR_READ_DATA  FileInfo[1];

	TON_typ Tmr;
	

	UDINT pMem;
	UDINT Len;
	UDINT Offset;
	
	USINT DO_NOT_Del_Extension;

	XML_RW_HS_INT_typ _int;


}FILE_RW_typ;



////////////////////////////////////////////////////////////////
////////////   XML Helper /////////////////////////////////////////

#define WAIT 			0 /*state to wait for commands*/
#define CREATE_WRITER 	1 /*state to create a writer to a memory area*/
#define START_DOC 		2 /*state to create a new XML document*/
#define START_ELEM 		3 /*state to start a new element*/
#define WRITE_ATTR 		4 /*state to write an attribute*/
#define WRITE_ELEM_TEXT 5 /*state to write a text in the current element*/
#define WRITE_CDATA 	6 /*state to write a CDATA section*/
#define WRITE_PI 		7 /*state to write a process Instruction*/
#define WRITE_COMMENT 	8 /*state to write a comment*/
#define CLOSE_ELEM 		9 /*state to close an element*/
#define CLOSE_DOC 		10 /*state to close the document*/
#define GET_MEMINFO 	11 /*state to to get the adress of the XML data*/
#define CLOSE_WRITER 	12 /*state to close the XML writer*/
#define ERROR 			13/*state when an error occured*/

#define CREATE_READER   50 /*state to create a reader to a memory area*/
#define READ_NODE 		51 /*state to read a Node*/
#define READ_ATTR 		52 /*state to read an attribute*/
#define CLOSE_READER 	53 /*state to close the reader*/

 
#define XML_CMD_WRITE 			1 /*write parameters from list to xml file*/
#define XML_CMD_READ 			2 /*read parameters from list to xml file*/
#define XML_CMD_CLOSE_WRITER 	3 /*close memory xml writer*/
#define XML_CMD_CLOSE_READER 	4 /*close memory xml writer*/
#define XML_CMD_SET_NEXT_PROGR  5 /* Set Next Program */
#define XML_CMD_GET_NEXT_PROGR  6 /* Set Next Program */



 
  
 typedef struct XML_Para_typ{
 
	 UDINT ident; /*parameter for the ident*/
	 plcstring	 attributeName[32]; /*name of the attribute to be written*/
	 plcstring	 attributeValue[32]; /*value of the attribute to be written*/
	 plcstring	 elementText[32]; /*text to be written in the element */
	 plcstring	 elementName[32]; /*name of the element to be written*/
	 plcstring	 cDATA[32]; /*data to be written in the xml document*/
	 plcstring	 piTarget[32]; /*target of the Process Instruction*/
	 plcstring	 piValue[32] ; /*value of the Process Instruction*/
	 plcstring	 comment[32]; /*comment that has to be written*/

 
 }XML_Para_typ;

 typedef struct xmlAttribute_typ{
 	
	plcstring name[64]; //name of the attribute
	plcstring value[64]; //value of the attribute

 }xmlAttribute_typ;


typedef struct xmlElement_typ{
	
	plcstring 	name[32]; //name of the XML element
	plcstring 	value[32]; //value of the XML element
	float 		fValue;
	UDINT 		type ; //type of the XML element
	UDINT 		debth; //debth of the XML element*
	UDINT 		attributeCount;
	xmlAttribute_typ attributes[2]; //attributes of the XML element
	
}xmlElement_typ;


typedef struct ReaderPara_typ {
	
	 UDINT 				ident;  //variable with the ident  
	 plcstring 			nodeName[64];  //variable with the name of a XML node  
	 plcstring 			nodeValue[64];  //variable with the value of a XML node  
	 UDINT 				nodeType;  //variable with the node type  
	 UDINT 				nodeDepth;  //variable that shows the depth of the XML node  
	 plcbit 			empty;  //variable to check if the the XML node is empty  
	 UDINT 				attributeCount;  //variable with the number of attributes  
	 UDINT 				attributeIndex;  //variable with the attribute index to read  
	 plcstring 			attributeName[64];  //variable with the name of the attribute  
	 plcstring 			attributeValue[64];  //variable with the value of the attribute  
	 xmlElement_typ 	xmlElements[256];  //variable with XML element information  
	 USINT 				elementIndex;  //variable with the index of the element to be read  
	 
}ReaderPara_typ;

 
 typedef struct XML_FUB_typ
 {	  
		xmlCreateMemoryWriter_typ	 xmlCreateMemoryWriter_0; /*function block that creates a writer to a memory area for newly created XML data*/
		xmlWriteStartDocument_typ	 xmlWriteStartDocument_0; /*function block that creates a new XML document*/
		xmlWriteStartElement_typ	 xmlWriteStartElement_0; /*function block that starts a new XML element*/
		xmlWriteAttribute_typ		 xmlWriteAttribute_0; /*function block that writes an XML attribute for the current XML element*/
		xmlWriteElementText_typ	 	 xmlWriteElementText_0; /*function block that writes a text in the current XML element*/
		xmlWriteCData_typ			 xmlWriteCData_0; /*function block that writes a CDATA section*/
		xmlWritePI_typ 			     xmlWritePI_0; /*function block that writes a process instruction*/
		xmlWriteComment_typ		     xmlWriteComment_0; /*function block that writes a comment at the current position in the XML document*/
		xmlWriteEndElement_typ 	     xmlWriteEndElement_0; /*function block that closes the open XML element*/
		xmlWriteEndDocument_typ	     xmlWriteEndDocument_0; /*function block that completes the XML document*/
		xmlGetMemoryInfo_typ		 xmlGetMemoryInfo_0; /*function block that creates a pointer to the memory area with the newly created XML data and the size of the data*/
		xmlCloseMemoryWriter_typ	 xmlCloseMemoryWriter_0; /*function block that releases the resources required by the XML writer*/

		xmlCreateMemoryReader_typ	xmlCreateMemoryReader_0; /*function block that creates a reader for a memory area with XML data*/
		xmlReadNextNode_typ 		xmlReadNextNode_0; /*function block that reads the next XML node*/
		xmlReadAttributeNr_typ 		xmlReadAttributeNr_0; /*function block that reads the XML attribute with the specified index of the current XML node*/
		xmlCloseMemoryReader_typ 	xmlCloseMemoryReader_0; /*function block that releases the resources required by the XML reader*/

 }XML_FUB_typ;
 
 typedef struct XML_Helper_typ{
	 UINT	 			xCmd; /*variable with the commands*/
	 UINT 				Auto;
	 XML_Para_typ 		para;  /*variable with the parameters*/
	 ReaderPara_typ 	para_read; /*variable with the parameters*/
	 XML_FUB_typ  		fub; /*variable with the Function Blocks*/
	 UINT				state ; /*variable with the actual state*/
	 UINT				runState; /*variable with the actual auto State*/
	 UINT 				errorState;
	 UDINT				pMemory;
	 UDINT      		MemorySize;
	 plcstring 			NextProgramName[32];
	 float              WeldProgramRepeat;
 }XML_Helper_typ;

  typedef struct ParamList_typ {

	UINT Index[128];
	UINT IndexNum;

	UINT Index_Int[32];
	UINT IndexNum_Int;

	UINT Index_Enum[32];
	UINT IndexNum_Enum;

 }ParamList_typ;
  

/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////




/***********************************************/

/******	SERIAL PORT & LCD DISPALY Lib       ********/

/***********************************************/

#define DRILLMENU_LINE_NUM   8

#define SUB_UNIT_NUM	4
#define NUM_LINE 		3

// Methods Enumeration

#define IDLE_CMD 		0
#define JUMP_CMD 		1
#define JUMP2ID_CMD 	2
#define UPDATE_CMD 		3

#define SHOW_POPUP_CMD 	100

// MCVCommands

#define MV_RESTART_CMD 		1
#define MV_LOAD_NEXT_CMD 	2



// Variable type 

#define T_NATIVE		0x0 // native PVI type
#define T_INTEGER		0x1
#define T_ENUM			0x2
#define T_SCALED		0x4   // This cover Float
#define T_STRING		0x8

// Extra Atrubutes for displaying can be added
#define T_ATR_ENUM_TXT		0x1000
#define T_ATR_BLINK			0x2000
#define T_ATR_FAST_BLINK    0x4000
#define T_ATR_TXT_HIDDEN    0x8000

#define VAR_READONLY    0x10
#define VAR_CURCFG		0x20


// Variable type 
/*
#define T_BLINK			1
#define T_FAST_BLINK    2
#define T_ENUM			4
#define T_SCALED		8
#define T_STRING		16
#define T_STRING_HIDDEN	32

*/

#if 0

///////////////////
// Node type Old Type

// Non Additive
#define N_STAT 		0  //display as normal, no icon, focus box skips
#define N_EDIT		1  //display as normal, edit icon, focus box, enter jumps to edit page (with type)
#define N_JUMP 		2  //display as normal, jump icon, focus box, enter jumps to new node
#define N_SCROLL_PG 4  //not for Android pendant, treat as N_SCROLL  
#define N_SCROLL 	8  //display as normal, no edit icon, focus box, ne edit, no jump


// Additive
#define N_EVENT 	0x10 //can be combined with N_STAT, N_EDIT, N_JUMP, N_SCROLL_PG, N_SCROLL (set event value into fire event) 
#define N_EDIT_IN_PLACE 0x20 // not focus scroll , send index , encoder and enter Btn state to plc, plc updates node text

// Additive but Obsolete - do NOT use it in Android
#define N_EDIT_DIS 	0x40 //not for Android pendant, ignore
#define N_NO_PREFIX	0x80 //not for Android pendant, ignore
#define N_USER_EDIT	0x100 //not for Android pendant, ignore
#define N_ONLY_LCD	0x200 //not for Android pendant, ignore
////////////////////////////////////////////////////////////

#endif


///////////////////
// Node type  New Style

// Non Additive
#define N_STAT 		0  //display as normal, no icon, focus box skips
#define N_EDIT		1  //display as normal, edit icon, focus box, enter jumps to edit page (with type)
#define N_JUMP 		2  //display as normal, jump icon, focus box, enter jumps to new node
#define N_SCROLL_PG 4  //not for Android pendant, treat as N_SCROLL  
#define N_SCROLL 	8  //display as normal, no edit icon, focus box, ne edit, no jump


// Additive
#define N_EVENT 	0x10 //can be combined with N_STAT, N_EDIT, N_JUMP, N_SCROLL_PG, N_SCROLL (set event value into fire event) 
#define N_EVENT_PROMPT_YES_NO 0x20 // like N_EVENT but ask YES NO before send event
#define N_EVENT_PROMPT_OK 0x40  // like N_EVENT but ask for  YES before send event
#define N_EDIT_IN_PLACE 0x80 // not focus scroll , send index , encoder and enter Btn state to plc, plc updates node text



// Additive but Obsolete - do NOT use it in Android
#define N_EDIT_DIS 	0x1000 //not for Android pendant, ignore
#define N_NO_PREFIX	0x2000 //not for Android pendant, ignore
#define N_USER_EDIT	0x4000 //not for Android pendant, ignore
#define N_ONLY_LCD	0x8000 //not for Android pendant, ignore
////////////////////////////////////////////




// Edit Mode
#define EDIT_VAL 	0
#define EDIT_UNIT 	1
#define EDIT_MIN 	2
#define EDIT_MAX 	3
#define EDIT_INCR 	4

#define EDIT_MODE_FULL 0
#define EDIT_MODE_VALUE 1
#define EDIT_MODE_UNIT  2
#define EDIT_MODE_MIN   3
#define EDIT_MODE_MAX   4
#define EDIT_MODE_INCR  5






typedef struct SerialPortDriver{
	 FRM_xopen_typ	 Open;							 
	 XOPENCONFIG	 OpenConfig;						 
	 FRM_gbuf_typ	 GetBuffer; 			 
	 FRM_rbuf_typ	 Release;					  
	 FRM_robuf_typ	 ReleaseOutput; 		  
	 FRM_write_typ	 Write; 			  
	 FRM_read_typ	 Read;	 
	 FRM_mode_typ	 Mode;
	 FRM_close_typ	 Close;
		 
	 UINT			 StatusOpen;
	 UINT			 StatusWrite;
	 UINT			 StatusRead;
	 UINT			 StatusGetBuffer;
	 UINT			 StatusReleaseOutputBuffer; 
	 UINT			 StatusMode; 
	 UINT			 StatusClose; 
 
	 UDINT			 Ident; 							  
	 UDINT* 		 pSendBuffer;
	 UDINT* 		 pReadBuffer; 
	 UINT			 SendLength;
	 UINT			 ReadLength;
	 UINT			 l; 
 
	 UINT			 txRequest;
	 UINT			 txBufRequest;
	 UINT			 txDataLen;
	 UINT			 rxReady;
	 UINT			 txMessageCounter;
	 UINT			 rxMessageCounter;
	 UINT			 txError;
	 UINT			 rxError;
	 UINT			 initError;
 
	 STRING 		 sDevice[32];
	 STRING 		 sMode[64]; 			 
	 STRING 		 sModeNew[64];
	 USINT			 _parity;
 
	 plcstring		 txData[256];
	 plcstring		 rxData[256];
	 STRING 		 sInput[64];
						 
 }SerialPortDriver_typ;
 /***************************************/

 
  
 
 
 
  
#define UNIT_IU  		0 
#define UNIT_mm   		1 
#define UNIT_inch   	2 
#define UNIT_cm   		3
 
   
#define UNIT_IU   			0 
#define UNIT_mm_per_sec   	1 
#define UNIT_inch_per_min   2 
#define UNIT_inch_per_sec   3 
 
  
#define UNIT_IU   			0 
#define UNIT_mm_per_sec2   	1 
#define UNIT_inch_per_sec2  2
#define UNIT_cm_per_sec2  	3
 
 
#define UNIT_IU   	0 
#define UNIT_RPS   	1 
#define UNIT_RPM  	2 
#define UNIT_RPh  	3 
 
 
#define UNIT_IU   	0 
#define UNIT_sec   	1 
#define UNIT_min  	2
#define UNIT_h  	3
 
 
#define UNIT_IU   	0 
#define UNIT_Amp	1 
#define UNIT_kAmp  	2
#define UNIT_mAmp  	3
 
 
#define UNIT_IU   	0 
#define UNIT_Volt   1 
#define UNIT_kVolt  2 
#define UNIT_mVolt  3
 
#define UNIT_IU   	0 
#define UNIT_Hz   	1 
#define UNIT_kHz  	2	 
#define UNIT_mHz  	3
 
#define UNIT_IU   		0 
#define UNIT_PERCENT   	1 
 
#define UNIT_IU   	0
#define UNIT_DEG   	1 
#define UNIT_RAD   	2 
 

  
  typedef struct VAR_obj{
  
	  UINT Type;
	  plcstring Format[64];
	  //plcstring Var1Name[32];	
	
	  UINT Var1ID;
	
  }VAR_obj;
  
  
  /* Datatypes */
  typedef struct LCD_NODE_obj
  { 
  
	  UINT	ID;
	  UINT	HeadID;
	  UINT  JumpNodeID;
	  
	  UINT  Type;
	  UDINT pHeadNode;
	  UINT	MinUserLevel;
	  //UDINT pPrevNode;
	  //UDINT pNextNode;
	  UDINT pJumpNode;
	  UINT JumpLineID;
	  UINT Event;
	  VAR_obj var;
	  //void (*lcdCall)();
  
  
  }LCD_NODE_obj;
 
  
  
  
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
  
#define LCD_SET_BAUDRATE		10
  
  
  typedef struct LCD_CMD_typ{
	  UINT len;   
	  USINT cmd[6];
  }LCD_CMD_typ;   
  
  
  static const	LCD_CMD_typ  LCD_CMD[11]=
  { 
	  {3, {0xFE,0x45,0x00}},  //LCD_SET_CURSOR
	  {2, {0xFE,0x4B}}, 	  // LCD_BLINK_CURSOR_ON
	  {2, {0xFE,0x4C}}, 	  // LCD_BLINK_CURSOR_OFF
	  {2, {0xFE,0x47}}, 	  // LCD_UNDERLINE_ON
	  {2, {0xFE,0x48}}, 	  // LCD_UNDERLINE_OFF
	  {3, {0xFE,0x52}}, 	  // LCD_SET_CONTRAST
	  {3, {0xFE,0x53}}, 	  // LCD_SET_BACKLIGHT	  
	  {2, {0xFE,0x41}}, 	  // LCD_TURN_ON_DISPLAY
	  {2, {0xFE,0x42}}, 	  // LCD_TURN_OFF_DISPLAY
	  {2, {0xFE,0x51}}, 	  // LCD_CLEAR_DISPLAY
	  {3, {0xFE,0x61}}, 	  // LCD_SET_BAUDRATE
  
  };
  
  
 
  typedef struct LCD_MGR_INTR_TYP
  {
	  UINT clk;
	  UINT keyPos;
	  USINT editStep;
	  plcbit SlowBlink;
	  plcbit FastBlink;
	  float _slowBlink;
	  float _fastBlink;
	  //SM_obj sm[NUM_LINE];
	  USINT displStep;
	  plcstring LineTXT[NUM_LINE][32];
	  LCD_NODE_obj *pNodes[NUM_LINE];
	  float tasktime;
	  USINT WhatToEdit;
	  USINT scrollDir;
	  //INT _wheel;
  
  
  }LCD_MGR_INTR_TYP;
  
 
   typedef struct LCD_MGR_INTRF_TYP
  {
	  plcbit LCDEntr;
	  plcbit LCDEntr_NegEdge;
	  plcbit LCDEntr_PosEdge;
	  plcbit TickPlus;
	  plcbit TickMinus;
	  //INT Wheel;
	  INT WheelDelta;
  
  
  }LCD_MGR_INTRF_TYP;
 
typedef struct LCD_MGR_obj
{ 
	USINT StaticMessageVisible;
	UINT  UserLevel;
	plcstring KeyPad[20+1];
	MASTER_typ *pMaster;
	LCD_NODE_obj *pMyNodes; // this is pointer to Nodes memory
	//Units_obj *pUnits;
	//UINT *pScaledUnitIndx;
	LCD_MGR_INTR_TYP Intr;
	LCD_MGR_INTRF_TYP Intrf;
	USINT EditMode;	
	LCD_NODE_obj *pNode2Jump;
	UINT Node2JumpID;
	UDINT pNode2Jump_old;
	plcstring Banner[32];
	plcstring Message[NUM_LINE+1][32];
	UINT xCMD;
	USINT IntrfCmdDisable;
	USINT EntrBtnPressReq;
	UINT LineID;
	UINT JumpLineID;
	SerialPortDriver_typ *pDevice;
	unsigned cursPos;
	UDINT FiredEvent;
	UINT curHeadID;
	//UINT curLineID[NUM_LINE];
	UINT curNodeID;
	TON_typ MsgTmr;
	//TON_typ WarnTmr;

	//UINT paramLastNum;
   	//UINT varLastNum ;
	UINT lineNum;
	//UINT segmNum;
	//UINT numZone;
	UINT nodesNum;

	UINT KK_test[NUM_LINE];
	
}LCD_MGR_obj;




 /******************************/
 /*   PENDANT    *****************/
 /** Inputs Outputs mapping **/
 
 /**  On left side*/
#define  BTN_A 	0
#define  BTN_A_MSK 	0x0001

#define  BTN_B	1
#define  BTN_A_MSK 	0x0002

#define	 BTN_C	2
#define  BTN_A_MSK 	0x0004

#define  BTN_D	3
#define  BTN_A_MSK 	0x0008

#define  BTN_E	4
#define  BTN_A_MSK 	0x0010

#define  BTN_F	5
#define  BTN_A_MSK 	0x0020

 
 /* On right side*/
#define BTN_GLB_D		6
#define BTN_GLB_D_MSK 	0x0008

#define BTN_GLB_C		7
#define BTN_GLB_C_MSK 	0x0004


#define LCD_ENTR	12

/***** Multiplex  **********/
/** Multiplex  On right side */ 

/*
#define TEACH_P 	11
#define TEACH_FIN	10
#define SPARE_1		9
#define SPARE_2		8
*/
#define BTN_GLB_A 		11
#define BTN_GLB_A_MSK 	0x0001

#define BTN_GLB_B		10
#define BTN_GLB_B_MSK 	0x0002

#define BTN_GLB_E		9
#define BTN_GLB_E_MSK 	0x0010

#define BTN_GLB_F		8
#define BTN_GLB_F_MSK 	0x0020


 
/** Mode Selector */ 
#define MODE_A_SEL 	8	
#define MODE_B_SEL 	9
#define MODE_C_SEL 	10
#define MODE_D_SEL	11
  
 /*****************/
 /* Outputs Mapping*/
 
#define ENTR_LED	0

#define BTN_A_LED	1
#define BTN_A_LED_MSK 0x0001

#define BTN_B_LED	2
#define BTN_B_LED_MSK 0x0002

#define BTN_C_LED	3
#define BTN_C_LED_MSK 0x0004

#define BTN_D_LED	4
#define BTN_D_LED_MSK 0x0008

#define BTN_E_LED	5
#define BTN_E_LED_MSK 0x0010

#define BTN_F_LED	6
#define BTN_F_LED_MSK 0x0020

 
#define BTN_GLB_D_LED 		7
#define BTN_GLB_D_LED_MSK 0x8

#define BTN_GLB_C_LED 	8
#define BTN_GLB_C_LED_MSK 0x4

 
#define MODE_LED	9
#define MODE_LED_MSK	0x0004

 
#define BTN_GLB_E_LED	10 
#define BTN_GLB_E_LED_MSK	0x10

#define BTN_GLB_B_LED	11
#define BTN_GLB_B_LED_MSK	0x2

#define BTN_GLB_A_LED 	12
#define BTN_GLB_A_LED_MSK	0x1

//old like mine
/*
#define SEL_0_OUT	13
#define SEL_1_OUT	14

#define BTN_GLB_F_LED	15
#define BTN_GLB_F_LED_MSK	0x20
*/

// new

#define BTN_GLB_F_LED	13
#define BTN_GLB_F_LED_MSK	0x0020

#define SEL_0_OUT	14
#define SEL_1_OUT	15





/**********************************/
/**** PROCESS SPECIFIC STRUCTURES */

typedef struct PS_Param_typ{

	GenProcess_typ *pPrc;	
	Calib_Param_typ *pCalibPoints;

	UDINT pArcLinkRobot;
	USINT Mode;
	USINT BoostCmd;
	USINT Test;

	INT *pAnal_Output;
	INT *pAnal_Output_neg;
	plcbit *pOutput;
		
	float *pStartDly;
	float *pRampUp;
	float *pStopDly;
	float *pRampDwn;
	float *pAmpSP;
	float *pAmpInSP;
	float *pAmpOutSP;
	float *pStartAmp;
	float *pEndAmp;
	float *pAmpBoost;

	DINT *pMode;

	float *pBackAmp;
	float *pPulseFreq;
	float *pPulseWidth;	

	float *pBoost1;
	float *pBoost2;


	float *pContin_RampDwn;
	float *pContin_Dwell;
	float *pContin_RampUp;
	float *pContin_Amp_Reduce;
		
	float rawCmdMax;
	float AmpCMD_Max;
	
	
}PS_Param_typ; 

typedef struct INDEX_Param_typ{

	GenProcess_typ *pPrc;	
	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	Calib_Param_typ *pCalibPoints;

	USINT Mode;
	
	float *pStepDist;
	float *pStepVel;
	float *pJogSpeedDirCntrl;

	float MaxSpeed;
		
}INDEX_Param_typ; 




typedef struct AVC_Param_typ{

	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	GenProcess_typ *pPrc;
	Calib_Param_typ *pCalibPoints;

	USINT Mode;
	USINT BoostCmd;

	float extVoltSP;	

	float *pTouchVolt;
	float *pTouchTm;
	float *pRetractTm;
	float *pTouchRepeat;
	
	float *pVoltSP;	
	float *pVoltInSP;
	float *pVoltOutSP;
	float *pBkgVoltSP;
	float *pVoltSPTimeOut;	
	
	float *pTouchVel;
	float *pToSPVel;
	float *pLU_Tm;
	float *pKiss_Tm;

	float *pVoltInit;
	float *pFinalVolt;
	//float *pHoldAVCTm;
	float *pRampDwn;
	float *pHoldTm;
	float *pGoBackTm;

	float *pStartDly;
	float *pStopDly;
	
	float *pRampUp;
	
	float *pMaxVolrError;	

	float *pLimitsFilterTm;

	float *pJogSpeedDirCntrl;	
	float *pLiftSpeedDirCntrl;

	DINT *pMode;

	float *pBB_Window;
	float *pBB_Vel;
	float *pBB_Gain;
	float *pBB_Integr;

	float *pBoost1;
	float *pBoost2;
	
	float MaxSpeedJog;
	float maxBEMFspeed;

	
}AVC_Param_typ;

typedef struct OSC_TML_Param_typ{
	
	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	GenProcess_typ *pPrc;

	float MaxOscAmpl;
	float mm2ticks;
	float SpeedFactor;

	USINT Mode;
	//float GlbDirection;

	TML_typ *pTML;
	float *pStartDly;
	float *pRampUp;
	float *pPrimAmpl;
	float *pInitAmpl;
	float *pFinalAmpl;
	float *pStopDly;
	float *pRampDwn;
	float *pRampUpdate;
	float *pFreq;
	float *pDwell_In;
	float *pDwell_Out;
	DINT  *pCamType;
	float *pJogSpeed;
	float *pJogSpeedDirCntrl;
	DINT *pOSC_Mode;
	DINT *pOSC_Synch;
	float *pIndexDist;
	float *pPeriod;
	float *pIndxVel;
	UINT PosFbkIndx;
	
}OSC_TML_Param_typ;

typedef struct WRST_TML_Param_typ{
	
	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	GenProcess_typ *pPrc;

	TML_typ *pTML;

	USINT Mode;

	float MaxAmpl;
	float GlbDirection;

	float TVLSpeedOverride;
	USINT MappingActive;
	USINT StopEvaluateAngle;
	float actAngle;
	USINT actIndx;
	float actAngleFromMapping;
	
	float *pJogSpeedDirCntrl;
	//float *pAVC_FF_Gain;
	//float *pAVC_FF_T;
	float *pTVL_Pos_Offset;
	float *pTVL_Pos_Predict;

	float *pRadius;
	float *pRadSpeed_Boost;
	
	
}WRST_TML_Param_typ;


typedef struct GAS_Param_typ {
	
	GenProcess_typ *pPrc;

	USINT Mode;

	plcbit *pSensInput;
	plcbit *pOutput;
	
	float *pStartDly;
	float *pStopDly;
	float *pTmo;
	
}GAS_Param_typ;

typedef struct HW_PS_Param_typ{

	GenProcess_typ *pPrc;
	Calib_Param_typ *pCalibPoints;

	INT *pAnal_Output;	
	
	plcbit *pOutput;
		
	float *pStartDly;
	float *pRampUp;
	float *pStopDly;
	float *pRampDwn;
	float *pAmpSP;
	float *pStartAmp;
	float *pEndAmp;
	float *pAmpBoost;
	float *pRampUpdate;
	float *pVoltSP;

	float rawCmdMax;
	float AmpCMD_Max;
	
}HW_PS_Param_typ; 

typedef struct WIRE_Param_typ {

	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	

	StateVectorProxy_typ *pAuxVect;
	unsigned short AuxVectStateNum; /* size of vectors array for process */
	
	GenProcess_typ *pPrc;
	
	Calib_Param_typ *pCalibPoints;

	USINT Mode;
	USINT BoostCmd;
	
	UINT *pSynchMode;
	UINT *pSynchClock;
	
	DINT *pAuxMtrMode;
	
	float *pStartDly;
	float *pRampUp;
	float *pSpeedSp;
	float *pSpeedInSp;
	float *pSpeedOutSp;
	float *pBkgSpeedSp;
	float *pSpeedInit;
	float *pSpeedFinal;
	float *pStopDly;
	float *pRampDwn;
	float *pRampUpdate;

	float *pBoost1SP;
	float *pBoost2SP;
	float *pJogSpeed;
	float *pJogSpeedDirCntrl;

	float *pRetracDly;
	float *pRetracDist;
	float *pRetracSpeed;

	float *pContinRampDwn;
	float *pContinRampUp;
	
	float *pCalibTime;
	float MaxSpeedJog;

	
}WIRE_Param_typ;

typedef struct WIREBLOCK_Param_typ{

	
	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	GenProcess_typ *pPrc;
	
	float *pJogSpeed;
	float MaxSpeedJog;

}WIREBLOCK_Param_typ;


 typedef struct TVLROT_Param_typ {

	StateVectorProxy_typ *pVect;
	unsigned short VectStateNum; /* size of vectors array for process */	  
	GenProcess_typ *pPrc;
	Calib_Param_typ *pCalibPoints;

	USINT Mode;
	USINT BoostCmd;

	UINT *pSynchMode;
	UINT *pSynchClock;

	float *pInitVel;
	float *pPrimVel;
	float *pPrimInVel;
	float *pPrimOutVel;
	float *pPrimBackVel;
	float *pFinalVel;
	float *pJog2Stop;
	float *pStartDly;
	float *pRampUp;
	float *pStopDly;
	float *pRampDwn;
	float *pRampUpdate;
	float *pJogSpeedDirCntrl;
	float *pTrackDia;
	float *pVelTolerance;

	float *pRewrapDly;
	float *pRewrapVel;

	float *pBoost1;
	float *pBoost2;

	float *pContinRampDwn;
	float *pContinRampUp;

	DINT  *pEncoder;
	DINT  *pHomePos;
	float  EncPerTick;
	DINT  TicksAtHighVel;
	DINT  EncPos_FBK;
	float  EncPos_FBK_Float;
	//DINT  EncPos_FBK_Raw;
	DINT  EncDir;
	UINT Extras;
	USINT HeadSwitch;   
	//float DistPerTick;	
	float TotalRescaleFactor;
	SINT VectModeForJog;
	float MaxSpeedJog;
				
}TVLROT_Param_typ;


typedef struct MV_intr_typ {

	USINT _prevMode;

}MV_intr_typ;


typedef struct MV_typ {

	UINT xCMD;
	DINT param[2];
	MV_intr_typ _intr;
		
}MV_typ;


////////////////////////////////////////////////////

typedef struct Cmd_Log_typ{

TIME TimeStamp;
UINT AxisID;
//UINT Channel;
UINT OpCode;
//UINT Address; 
UDINT OpData1;
UDINT OpData2;
//USINT MessageLength;
//USINT DataType;

}Cmd_Log_typ;


typedef struct Cmd_Log_Mgr_typ{

	USINT PauseLogging;
	UINT AxisID_filter;
	UINT curIndx;
	Cmd_Log_typ logs[128];
	
}Cmd_Log_Mgr_typ;


/////////////////////////////////////////////////////
///   New Path Generator created March 2019 by CK
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////

typedef struct HCRMoveParamAzz_type
{
	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HCRMoveParamAzz_type;


typedef struct segment_type{

	float s;
	float a;
	float v;
	
}segment_type;


typedef struct HCRSPFazz_Internal_type
{
	plcstring statusTxt[81];
	UINT step;
	
	float S0;
	float S1;
	float S2;
	float S3;

	float _s_set;
	float _v_set;
	float _a_set;
	UINT _uSec;
	UINT uSec;	
	UINT indx;

	plcbit _enable;
	plcbit _Stop;
	plcbit _Set_home;
	plcbit _Init;
	plcbit _Brake;

	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;

	float TaskPeriod;

	segment_type segm[4];
	
}HCRSPFazz_Internal_type;

typedef struct HCRMoveParaAzz_type
{
	float t_Jolt;
	float vC_pos;
	float vC_neg;
	float aAcc_pos;
	float aDec_pos;
	float aAcc_neg;
	float aDec_neg;
} HCRMoveParaAzz_type;


typedef struct HCRSPFazz_type
{
	/* VAR_INPUT (analog) */
	//struct HCRMovePara_typ* pMove_Para;
	struct  HCRMoveParamAzz_type* pMove_Para;
	float s_home;
	float s_end;
	/* VAR_OUTPUT (analog) */
	float s_set;
	float v_set;
	float a_set;
	float elapseTime;
	signed char MoveDir;
	signed char ApproachDir;
	unsigned short status;
	/* VAR (analog) */
	struct HCRSPFazz_Internal_type Intr;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit Stop;
	plcbit Set_home;
	plcbit Init;
	plcbit Brake;
	/* VAR_OUTPUT (digital) */
	plcbit Reached;
} HCRSPFazz_type;

void HCRSPFazz(HCRSPFazz_type* inst);

//////  From old Mark Federer cod


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



/////////////////////////////////////////////////////////////
///  NEW Drill Down HMI Manager

/*
typedef struct UnitConv_obj{

	UINT step;
	UINT status;
	
	ArUnitConvConvert_typ Converter;		

}UnitConv_obj;
*/

typedef struct SceenEvent_typ{


}SceenEvent_typ;

typedef struct EDIT_PARAM_PAGE_typ{

	UINT Param_Indx;
	UINT Segm_Indx;

	plcstring ParamName[64];
	plcstring UnitsName[32];


	float MainVal_Disp;
	float MinVal_Disp;
	float MaxVal_Disp;
	float Incr_Disp;

	UINT UnitTxt_Indx;

	// Helpinh HMI object
	TON_typ *pMsgTmr;
	UINT Confrim_Btn;
	UINT Confrim_Btn_vis;
	UINT MsgBox_vis;
	/////////////

	
	UINT PrecDigits;
	float MinVal_Cfg;
	float MaxVal_Cfg;
	float MinIncr_Cfg;
	float MaxIncr_Cfg;
	float defaultVal;
	plcbit LoadDefault_Btn;

	UINT Segm_Indx_Locked;

	Unit_Scale_obj *pScale;	
	Scaled_Var_obj* var;

}EDIT_PARAM_PAGE_typ;

typedef struct READ_VAR_typ{

	MASTER_typ *pMaster;


	UINT Param_Indx;
	UINT Segm_Indx;

	plcstring ParamName[64];
	plcstring UnitsName[32];
	//plcstring EnumTxt[64];


	float MainVal_Disp;
	float MinVal_Disp;
	float MaxVal_Disp;
	float Incr_Disp;

	UINT UnitTxt_Indx;

	// Helpinh HMI object
	//TON_typ *pMsgTmr;
	//UINT Confrim_Btn;
	//UINT Confrim_Btn_vis;
	//UINT MsgBox_vis;
	/////////////

	
	UINT PrecDigits;
	float MinVal_Cfg;
	float MaxVal_Cfg;
	float MinIncr_Cfg;
	float MaxIncr_Cfg;
	float defaultVal;
	plcbit LoadDefault_Btn;

	Unit_Scale_obj *pScale;	
	Scaled_Var_obj* var;	

}READ_VAR_typ;

	
typedef struct DRILL_DWN_MGR_obj
{ 
	UINT  UserLevel;

	MASTER_typ *pMaster;
	LCD_NODE_obj *nodes; // this is pointer to Nodes memory
	Cfg_typ *cfg;
	CurCfg_typ *curCfg;

	HMI_typ *pT_Pend_HMI;

	EDIT_PARAM_PAGE_typ *editPageData;
	plcbit ShowPopUpMsg;
	UINT StatusLED[8];

	plcstring Message[4][20+1];// read only
	plcstring Banner[64+1];// read only
	plcstring *pActiveBanner;

	//UINT EditPageReq_Cmd; // New
	plcbit Save_Exit_Cmd; // New
	plcbit Cancel_Return_Cmd; // New
	plcbit Refresh_Edit_Window_Cmd; // Nie potrzebny
	plcbit Enum_Save_Exit_Cmd; // New
	plcbit Enum_Cancel_Return_Cmd; // New

	plcbit ClearMsgBox_vis_Cmd;
	
	plcstring TxtLines[DRILLMENU_LINE_NUM+1][64];
	plcstring VarLines[DRILLMENU_LINE_NUM+1][64];
	plcstring UnitLines[DRILLMENU_LINE_NUM+1][64];

	UINT TxtLines_vis[DRILLMENU_LINE_NUM+1];
	UINT LongTxtLines_vis[DRILLMENU_LINE_NUM+1];
	UINT TwoTxtLines_vis[DRILLMENU_LINE_NUM+1];

	UINT TxtLineHighLight[DRILLMENU_LINE_NUM+1];
	UINT LineAtr[DRILLMENU_LINE_NUM+1];
	UINT LineAtr_vis[DRILLMENU_LINE_NUM+1];
	plcbit LineClick[DRILLMENU_LINE_NUM+1];
	plcbit PrevClick;
	plcbit NextClick;
	plcbit PrevPageClick;
	plcbit NextPageClick;
	plcbit PageHeadClick;
	
	
	UINT curActiveLine;
	UINT lineNum;
	UINT curlineNum;
	UINT curTopNodeID;

	// HMI Interface
	plcbit LCDEntr;
	//plcbit LCDEntr_PosEdge;
	//plcbit LCDEntr_NegEdge;

	plcbit TickPlus;
	plcbit TickMinus;

	///

	UINT Ticks_vis;

	
	UINT TxtIndex;
	UINT xCMD;
	
	UINT LineID;
	
	UINT JumpLineID;
	UDINT FiredEvent;
	UDINT PendingFiredEvent;
	UDINT PopUpMsg_Event;
	UINT PleaseUpdate;
	UINT curHeadID;
	UINT curNodeID;
	TON_typ MsgTmr;
	
	//UINT segmNum;
	//UINT numZone;
	UINT nodesNum;

	UINT Editor_vis;
	UINT Enum_Editor_vis;
	
	//UnitConv_obj UnitConverter;
	plcbit RefreshPage;
	plcbit RefreshEditor;
	

	USINT RefreshCntr;
	

	UINT KK_test[NUM_LINE];
	
}DRILL_DWN_MGR_obj;


typedef struct SINGLE_EDITOR_MGR_obj
{ 
	UINT  UserLevel;

	MASTER_typ *pMaster;
	//LCD_NODE_obj *nodes; // this is pointer to Nodes memory
	Cfg_typ *cfg;
	CurCfg_typ *curCfg;

	//HMI_typ *pT_Pend_HMI;

	EDIT_PARAM_PAGE_typ *editPageData;
	//plcbit ShowPopUpMsg;
	//UINT StatusLED[8];

	//plcstring Message[4][20+1];// read only
	//plcstring Banner[64+1];// read only
	//plcstring *pActiveBanner;

	//UINT EditPageReq_Cmd; // New
	plcbit Save_Exit_Cmd; // New
	plcbit Cancel_Return_Cmd; // New
	//plcbit Refresh_Edit_Window_Cmd; // Nie potrzebny
	plcbit Enum_Save_Exit_Cmd; // New
	plcbit Enum_Cancel_Return_Cmd; // New

	//plcbit ClearMsgBox_vis_Cmd;

	/*
	plcstring TxtLines[DRILLMENU_LINE_NUM+1][64];
	plcstring VarLines[DRILLMENU_LINE_NUM+1][64];
	plcstring UnitLines[DRILLMENU_LINE_NUM+1][64];

	UINT TxtLines_vis[DRILLMENU_LINE_NUM+1];
	UINT LongTxtLines_vis[DRILLMENU_LINE_NUM+1];
	UINT TwoTxtLines_vis[DRILLMENU_LINE_NUM+1];

	UINT TxtLineHighLight[DRILLMENU_LINE_NUM+1];
	UINT LineAtr[DRILLMENU_LINE_NUM+1];
	UINT LineAtr_vis[DRILLMENU_LINE_NUM+1];
	plcbit LineClick[DRILLMENU_LINE_NUM+1];
	plcbit PrevClick;
	plcbit NextClick;
	plcbit PrevPageClick;
	plcbit NextPageClick;
	plcbit PageHeadClick;
	
	
	UINT curActiveLine;
	UINT lineNum;
	UINT curlineNum;
	UINT curTopNodeID;

	// HMI Interface
	plcbit LCDEntr;
	//plcbit LCDEntr_PosEdge;
	//plcbit LCDEntr_NegEdge;
	*/

	plcbit TickPlus;
	plcbit TickMinus;

	UINT Ticks_vis;

	///

	/*
	UINT TxtIndex;
	UINT xCMD;
	
	UINT LineID;
	
	UINT JumpLineID;
	UDINT FiredEvent;
	UDINT PendingFiredEvent;
	UDINT PopUpMsg_Event;
	UINT PleaseUpdate;
	UINT curHeadID;
	UINT curNodeID;
	TON_typ MsgTmr;
	
	//UINT segmNum;
	//UINT numZone;
	UINT nodesNum;
	*/
	UINT Editor_vis;
	UINT Enum_Editor_vis;
	
	//UnitConv_obj UnitConverter;
	//plcbit RefreshPage;
	plcbit RefreshEditor;
	

	USINT RefreshCntr;
	

	UINT KK_test[NUM_LINE];
	
}SINGLE_EDITOR_MGR_obj;

typedef struct READ_ENUM_VAR_typ{

	UINT ParamEnum_Indx;
	UINT Param_Enum_Segm;

	plcstring ParamName[64];
	plcstring EnumTxt[64];
	
	//plcstring EnumList[16][RES_TXT_LEN];

}READ_ENUM_VAR_typ;


typedef struct SINGLE_ENUM_EDITOR{

	USINT step;
	UINT ParamEnum_Indx;
	UINT Param_Enum_Segm;

	
	
	plcstring ParamName[64];
	plcstring EnumList[16][RES_TXT_LEN];
	UINT EnumList_Indx;

	UINT EnumList_Indx_Max;


	plcbit Save_Exit_Cmd; // New
	plcbit Cancel_Return_Cmd; // New

	plcbit TickPlus;
	plcbit TickMinus;

	plcbit LoadDefaultBtn;

	USINT Segm_Indx_Locked;

	UINT Enum_Editor_vis;
	plcbit RefreshEditor;

}SINGLE_ENUM_EDITOR;
#endif

