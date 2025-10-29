
/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: Master
 * File: Master.c
 * Author: wsi
 * Created: September 25, 2012
 ********************************************************************

 Hardware[1].Inp[23] - index 80x switch
 * Implementation of program Master
 ********************************************************************/
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <math.h>



#include <..\GlobDef.h>
#include <..\..\Lib\WSI_Lib.c>

#include <..\GlobDecl.h>
#include <..\Hardcoded.c>
#include <..\HMI_Cntrl.c>
#include <..\HeadConfig.c>


#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\LCDLib.c> 
#include <..\..\Lib\LoggerLib.c>

//#include <..\..\Lib\utilityLib\bgdebug.h>
//#define TEST_ONLY_PLC_ANDROID 
#define HASH_NUM				440
//#define VERSION "UnivCntrl-10-29-19B" was tested by welders
//#define VERSION "UnivCntrl-11-20-19F" // was ready for testing before Thanksgiving but nobody tested it
//#define VERSION "UnivCntrl-01-10-20F" // Successfully tested in January - very good result
//#define VERSION "UnivCntrl-02-14-20F"
//#define VERSION "UnivCntrl-02-17-20B"
//#define VERSION "UnivCntrl-02-27-20B"
//#define VERSION "UnivCntrl-03-13-20Exp"
//#define VERSION "UnivCntrl-04-17-20A" // ESTOP does not interrupt GAS
//#define VERSION "UnivCntrl-05-15-20exp" // was working
//#define VERSION "UnivCntrl-06-12-20exp"  // Max Osc ampit was changed to 3 inch
// New OSC Concept for BPV v2 is possible - make changes in Configure_BPV_v2 in OSC motor def and OSC Process use type 10
//#define VERSION "UnivCntrl-06-16-20F"  //  New Heads: BPV 2.0 MT-BO (replaces "BPV 2.0 MPBO") , BPV 2.0 TT-BO , BPV 2.0 MT-BO_TOS , BPV 2.0 TT-BO_TOS;
//BPV_v2_TT_BO_TOS chyba dziala , przenioslem w desperacji wszystko z MBPOH , trzeba dodac w Main2 wszystko zwiazane z nowymi headami 		
//#define VERSION "UnivCntrl-07-10-20A" 
//#define VERSION "UnivCntrl-07-23-20A" 
//#define VERSION "UnivCntrl-07-27-20A"
//#define VERSION "UnivCntrl-08-20-20F" // good working install on 5 machines in October 2020
//#define VERSION "UnivCntrl-09-14-20B" // new TML organization )comments) - just started
//#define VERSION "UnivCntrl-09-16-20A" // new TML organization (comments) - compiled but never used 
//#define VERSION "UnivCntrl-10-27-20A" // new TML organization (comments) + SweachtHead for WF is fixed in SuperMini- is tested 
//#define VERSION "UnivCntrl-11-19-20F" // the exactly the same as  "UnivCntrl-10-27-20A" but installed on Tyler machine
//#define VERSION "UnivCntrl-11-24-20F" // all as above plus file name fixed
//#define VERSION "UnivCntrl-01-29-21F" // Installed on real machine with SMH

//#define VERSION "UnivCntrl-06-04-21B" // starting - adding weld report HMI

//#define VERSION "UnivCntrl-08-09-21G" // wyjechalo do Occonee - na razie spakowane - dzialalo dobrze byli zadowloeni
//#define VERSION "UnivCntrl-08-26-21A" // proba uzycia CMT

//#define VERSION "UnivCntrl-09-02-21A" // Mode 2 nie dzialal w SuperMini ?? spowrotem Mode 1 //TVL_ROT_Param_Set[i].Mode = 2; //TVL_MODE_BPV_PosMode
// aby dzialal TVL Mode 2,3 - jazda wg position to trzeba dodac w drivie POSTRIGG2=2147483647 , POSTRIGG3=2147483647 , POSTRIGG4=2147483647
//#define VERSION "UnivCntrl-09-21-21F" 
//define VERSION "MIGE_EXP-10-11-21F" 
//#define VERSION "MIGE_EXP-10-13-21F" 
//#define VERSION "MIGE_EXP-10-14-21G"
//#define VERSION "MIGE_EXP-10-15-21F"
//#define VERSION "MIGE_EXP-10-19-21F"
//#define VERSION "UnivCntrl-10-19-21F"
//#define VERSION "UnivCntrl-10-19-21M"
//#define VERSION "UnivCntrl-10-19-21MX"
//#define VERSION "UnivCntrl-11-02-21MX"
//#define VERSION "UnivCntrl-11-10-21MX"  // Zmiany W MIG EXP - 0.5 s delay przy restarcie pojednyczych TML
//#define VERSION "UnivCntrl-11-10-21MX"  // Zmiany W MIG EXP - 0.5 s delay przy restarcie pojednyczych TML

//#define VERSION "UnivCntrl-02-16-22A"  // dziala RUBIX 

//#define VERSION "UnivCntrl-02-23-22A"  // dzial  MINI Standard

//#define VERSION "UnivCntrl-03-01-22F"  // Synch Pulse  MINI Standard & Rubix

//#define VERSION "UnivCntrl-03-24-22F"  // Poprawki w beat logger + Synch Pulse  MINI Standard & Rubix

//#define VERSION "UnivCntrl-04-05-22F"  // MiniHead Omega  +>   Poprawki w beat logger + Synch Pulse  MINI Standard & Rubix
// required -> UnivCntrl_01Apr22_US_4AXIS RUBIX & STND&MiniOmega.m.zip

//#define VERSION "UnivCntrl-04-12-22H" // poprawiony TVL step w Mode 2 i 3
// required -> UnivCntrl_01Apr22_US_4AXIS RUBIX & STND&MiniOmega.m.zip

//#define VERSION "UnivCntrl-04-29-22F" // nowy D-Head
// required -> UnivCntrl_29Apr22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip

//#define VERSION "UnivCntrl-06-23-22F" // nowy D-Head
// required -> UnivCntrl_21June22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip

// this was changed TVL_ROT_Param_Set[i].Mode = 3; to Mode 1 
// in BPV_v2_MPBO head to get infinity welding
//#define VERSION "UnivCntrl-12-05-23F" 
// required -> UnivCntrl_21June22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip

// New Head definition - BPV v2 MT  - like BPV2 but with MT truck - no position mode in TVL
//#define VERSION "UnivCntrl-12-06-23F" 
// required -> UnivCntrl_21June22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip

//#define VERSION "UnivCntrl-12-11-23F" //New Head definition - BPV v1 MT
// required -> UnvCntrl_BPV_11_12_23.m // nowy TVL head
// required -> UnivCntrl_21June22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip

//#define VERSION "UnivCntrl-12-14-23F" //WYWALONY potencjometr z AVC New Head definition - BPV v1 MT
// required -> UnvCntrl_BPV_11_12_23.m // nowy TVL head
// required -> UnivCntrl_21June22_US_4AXIS RUBIX & STND&MiniOmega&DHead.m.zip




//#define VERSION "UnivCntrl-07-12-24A" // dodane BPV_v2_Video i zmiany W HMI Report

//#define VERSION "UnivCntrl-07-23-24A" // dodane BPV_v2_Video i zmiany W HMI Report

//#define VERSION "UnivCntrl-07-31-24F" // all above plus AVC feedback calibration in all legacy heads fixed

//#define VERSION "UnivCntrl-08-01-24G" // Weld Report : Heat Input or Power Ratio

//#define VERSION "UnivCntrl-09-09-24G" // Weld Report : Heat Input or Power Ratio + Limits

//#define VERSION "UnivCntrl-09-09-24R" // Radom - Weld Report : Heat Input or Power Ratio + Limits

//#define VERSION "UnivCntrl-10-18-24A" // new head : ClamShell , small change in reports

//#define VERSION "UnivCntrl-11-07-24A" // New Head KKL
//#define VERSION "UnivCntrl-11-29-24A" // New Head KKL
//#define VERSION "UnivCntrl-12-02-24A" // New Head KKL
//#define VERSION "UnivCntrl-12-03-24A" // New Head KKL
#define VERSION "UnivCntrl-12-04-24F" // New Head KKL - final


/////////////////


_LOCAL DINT 	statusInit;

_LOCAL plcbit   MIG_ESTOP_Rst_Out , MIG_ESTOP_Rst_Btn_Inp , MIG_ESTOP_Rst_PendantBtn;

 
_LOCAL Cfg_typ *cfg;
_LOCAL CurCfg_typ *curCfg;
_LOCAL CurCalib_typ *curCalib, *defCalib;
_LOCAL Scaled_Var_obj *var;

_LOCAL MASTER_typ * m;
_LOCAL SerialPortDriver_typ * rs;
_LOCAL LCD_MGR_obj * lcd;
_LOCAL PENDANT_HARDWARE_obj * pend;
_LOCAL HMI_typ * hmi;
_LOCAL HMI_INTRF_typ * intrf;

_LOCAL LCD_NODE_obj * nodes;
_LOCAL TON_typ	WF_Catch_TMR;
_LOCAL UINT 	WF_RAMPUP_Comp_Step;

_LOCAL TON_typ	DoubleStop_TMR;
_LOCAL plcbit	DoubleStopBtn;
_LOCAL plcbit	DoubleStopBtn_Latch, StopBtn_Latch;   

_LOCAL TON_typ	ESTOP_Dly_Tmr, AndroidConnect_Tmr;
_LOCAL UDINT	GlblBtnLED_Mask;
_LOCAL plcbit	ESTOP_PERMIT;

_LOCAL TON_typ Restart_Tmr;

//_LOCAL UINT CalibStep;
//_LOCAL BOOL CalibStepDone;
prcsSTATUS_Enum _ProcStatus;

_LOCAL INT		ANDROID_Pendant;
_LOCAL plcbit	StartBtn, StopBtn;

_LOCAL TON_typ	WireDisable_TMR;
_LOCAL UINT 	TVL_Latch_Cnt, TVL_Latch_Tm;
_LOCAL UINT 	TVL_Latch_Latched, _TVL_Latch_Latched;

_LOCAL plcbit   SYSTEM_LOCKED;

_LOCAL plcbit	Force_ENTR_LCD, _Force_ENTR_LCD;
_LOCAL UINT 	DebaunceCnt[16], DebounceTickSw, DebounceTickSwMax;
_LOCAL BOOL 	TickSw, _TickSw;
_LOCAL DINT 	WF_TVL_Diff;

INT 			_curZone;

_LOCAL plcbit	FirstScan, RefreshAndroid;

_LOCAL DINT 	_TIG_Mode , _AVC_Mode , _TVL_SYN_MODE , _WF_SYN_MODE , _WP_SYN_MODE ;

BOOL			SendRS232, _SendRS232;
_LOCAL plcstring txt[16][32];
_LOCAL Calib_Param_typ tempCalib;

_LOCAL plcbit	_segmSw;

_LOCAL plcbit	ROM_USB;

//_LOCAL DINT 	_SETUP_HEAD_SETUP;
_LOCAL plcbit	AutoStartEnable;


_LOCAL DINT 	actTicks, actTickBalance, actRot, _actRot, actPathCnt;
_LOCAL DINT 	deltaTick, actTicksForZone, actTicksForHead, actTVL_Dist, encHome_TVL;
_LOCAL DINT 	tickEncoder, _tickEncoder, encDelta;
_LOCAL UINT 	btnProgress, btnProgressMax, btnProgressBias;
_GLOBAL UINT 	curTML_indx , TML_Setup_From_Diagn;
_LOCAL UINT	    stillRun;

float			_TIG_CalibAmp, _HW_CalibAmp;

//_LOCAL Calib_Param_typ tempCalibPar; 
_LOCAL UINT			_ToolHeadID , _ToolHeadID_Sw1 , _ToolHeadID_Sw2;
DINT			_GlblBtnCmd[2];

INT 			_AndroidWheel;
UINT			_AndroidEntrBtn;
_LOCAL plcstring LineTxt[256];
_LOCAL INT		parametersDump_Step, paramIndx;
_LOCAL DTStructure DTStruct;
_LOCAL DTGetTime_typ DTGetTime_1;

UDINT			_datagramCountTotal[2];
UDINT			_datagramPerSec[2];

USINT 			_MappingActive;

UINT			_PLC_Changes;
USINT 			_WireDisabled;
_LOCAL plcbit	AutoStartBtn;
_LOCAL plcbit	Cont_Stop_By_End;

_LOCAL plcbit   TIG_MIG_Sw_Inp;


_LOCAL UINT POPUP_EVENT;

_LOCAL plcbit ESTOP_Inp_Status;



_LOCAL Robot_typ * robot;

_LOCAL UINT KK_ErrorsHold;
_LOCAL UINT KK_Can_Download_Everything;

_LOCAL USINT KK_Was_Cahnged , KK_Catched;
_LOCAL USINT	KK_Life;
_LOCAL BOOL 	KK_AUTO_Test;
_LOCAL plcstring KK_text[64];
_LOCAL INT		KK_AndroidWheelIncr;
_LOCAL INT		KK_Drill, KK_Out, KK_ZONE;
_LOCAL DINT 	KK_actPosProcss , KK_ActEnc;
_LOCAL float	KK_CatchAndroidComErr;
_LOCAL UINT 	KK_CatchAndroidErr;
_LOCAL UINT 	KK_test;


_LOCAL DINT 	myCounter;

_LOCAL USINT   AmpsCalib_step;

_LOCAL UINT TML_Restart_Indx , TML_Restart_Step;
_LOCAL UINT Process_Restart_Indx , Process_Restart_Step;



// TVL_MAPPing
_GLOBAL DINT _deltaAPOS[TML_SERVO_NUM] , _deltaAPOS2[TML_SERVO_NUM];
_GLOBAL UINT TVL_MAP_Indx[TML_SERVO_NUM], TVL_MAP_Indx_New[TML_SERVO_NUM];
_LOCAL  UINT TVL_MAP_Step;
_LOCAL  plcbit TVL_MAP_Allowed;
void TVL_Map_ToFile();



void ProcessEvents_Android(MASTER_typ * m, LCD_NODE_obj * nodes, HMI_typ * hmi, HMI_INTRF_typ * intrf);
void EditParam(UINT paramIndx);
void JogProcessAndroid(HMI_INTRF_typ * intrf, UINT procIndx, float gain);
void TakeNextWeldProgram(HMI_typ * hmi);
void AndroidHelper(MASTER_typ * m, UINT n);

void CalibrationModePendantButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd);
void StopModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd);
void StopCalibrationModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd);
void RunModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd);
void RunCalibrationModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd);


DINT MasterSequence(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn);
DINT MasterSequence_Contin(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn);
DINT MasterSequence_Contin_OneWay(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn);


DINT CalibrationSequence(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn);

void IncrParam(CurCfg_typ * curCfg, UINT param, DINT segm, float gain);
void DecrParam(CurCfg_typ * curCfg, UINT param, DINT segm, float gain);

void Check_parOSC_Excur(CurCfg_typ * curCfg, HMI_typ * hmi, UINT seg);

void Check_parOSC_Amplit(CurCfg_typ * curCfg, HMI_typ * hmi, UINT seg);
void PleaseUpdateHeadSetup(MASTER_typ * m);

void SetBannerTxt(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf);  
float getBtnProgress();
void SetZeroPosition(MASTER_typ * m);
void CreateParamList();
void DeleteExtension(char * p);
void WireDisableInable(plcbit Tmr_Q);

void DumpParamToFile();
void EvaluatePosition();
void Fill_Segments(CurCfg_typ * curCfg, UINT par, UINT seg, UINT Mode, UINT Step);

int WatchArcLinkError(MASTER_typ * m, Robot_typ * robot);
DINT Check_OSC_Amplt_and_Center();

void CalculateRunOverlap();
void CalculateRunOverlap_Cont();
void CalculateRunOverlap_Cont_OneWay();

void RestartStateMach();
plcbit Find_ext_txt(char *name , char* ext);




_LOCAL DINT _NextHeadConfig_Indx;

void _INIT Main2_INIT(void)
{
	int 			i;

	strcpy(PLC_Version_TXT, VERSION);


	LimScal_Tilt.x1 = 0.0;
	LimScal_Tilt.y1 = 0.0;

	LimScal_Tilt.x1 = 4096.0;
	LimScal_Tilt.y1 = 360.0;

	
	//	Program options
	KK_DoNotChangeTMLProgram 	= 0;
	KK_DoNotHashCalculation 	= 1; // always like that
	TVL_MAP_Allowed 			= 0;
	KK_No_TVL_Restart	= 0;
	KK_ManualCalib		= 0;
	KK_TML_PowerON		= 0;
	KK_Functionality	= 0;
	KK_ErrorsHold 		= 0;
	KK_Can_Download_Everything = 0;

	//////////////////////////
	Hold_Estop_ON 		= 0;
	ESTOP_Inp_Status 	= 0;
	TVL_Latch_Tm		= 50;
	ANDROID_Pendant 	= 1;

	//some heads setup in Config
	btnProgressMax		= 50;
	btnProgressBias 	= 10;

	KK_AUTO_Test		= 0;

	DebounceTickSwMax	= 2;


	// Just to see globals in the watch window
	Master[0].Active	= Master[0].Active;

	//LCD_MGR[0].curHead = LCD_MGR[0].curHead; 
	RS232[0].Ident		= RS232[0].Ident;
	Pendant[0].ABEncoder = Pendant[0].ABEncoder;


	ESTOP_Dly_Tmr.PT	= 1000; 					// 1 sec delay off ESTOP
	Hardware[0].Inp[4]	= 1;						// ESTOP

	Master[0].AndIntrf[0].Alive = 0;


	myCounter			= myCounter;

	ReadPermMemory_HS.Go = 0;
	ReadPermMemory_HS.Ready = 0;

	ReadCfgFile_HS.Go = 0;
	ReadCfgFile_HS.Ready = 0;
	
	TimerStart(&DoubleStop_TMR, 1.0);
	TimerExecute(&DoubleStop_TMR);

	
	Hardware[1].Outp[9] = 0; // Lights OFF (NC) on camera
	
	FirstScan = 1;
	RefreshAndroid = 0;

	SYSTEM_LOCKED = 0;

	LoggerInit(&LogMgr, &LogID, "LogBook1", 2000);

	// Very Important - get value from Statis Memory
	_NextHeadConfig_Indx = NextHeadConfig_Indx;
	actTicks			= actTicksPerm;

	if((_NextHeadConfig_Indx < 0) || (_NextHeadConfig_Indx >= HEAD_INVALID)){
		_NextHeadConfig_Indx = BPV_v2;
		NextHeadConfig_Indx = BPV_v2;
	}

	Master[0].ToolHeadID = (UINT)LastHeadDir;


}


void _CYCLIC Main2_CYCLIC(void)
{
int 			i, k;
char			status, _status;
DINT			d;
int 			len;
float 			x;

//plcbit StartBtn , StopBtn;
INT 			tickIncr;
LCD_NODE_obj *	curNode;	

	/////////////////////////////cas
	////  Must be in cyclic
	///  This is real pointer pickup 
	///////////////////////
	// Take pointers from SupeMaster
	m		= pMaster[0];
	lcd 	= pLCD_MGR[0];
	rs		= pRS232[0];
	pend	= pPendant[0];


	if(SYSTEM_LOCKED) // System Locked after Head definition changed - Must reboot
		return;


	// Check if pointers are not null
	if (!m)
		return;
	
	nodes				= m->pNode[0];

	if (!nodes)
		return;

	if (!lcd)
		return;

	if (!rs)
		return;


	if (!pend)
		return;

	if (!m->pCurCfg || !m->pCfg || !m->pCalib || !m->pScaledVar)
		return;

	cfg 				= (Cfg_typ *)		m->pCfg;
	curCfg				= (CurCfg_typ *)	m->pCurCfg;
	curCalib			= (CurCalib_typ *)	m->pCalib;
	defCalib			= (CurCalib_typ *)	m->pDefaultCalib;
	var 				= (Scaled_Var_obj*)	m->pScaledVar;
	robot				= (Robot_typ*)PS_Param_Set[0].pArcLinkRobot;

	hmi 				= &m->Hmi[0];
	intrf				= &m->Intrf[0];

	if (!hmi || !intrf)
		return;

	Hardware[1].Outp[8] = Hardware[1].Outp[9]; // Power on Camera #1
	//////  TEST /////////////
	/*
	if(KK_test){
		KK_test = 0;
		//Errors(&ErrorMgr, ERROR_TIG_PROC, ERROR_ARCLINK_PS_ERR, 123);

		if(Find_ext_txt(File_RW.fileNewName , "_setup"))
			File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;

	}
	*/
	//////////////////////////

	
	///////// Important because HMI in Global setting can change NextHeadConfig_Indx

	// Head type was changed - Load New Head definition
	
	if(hmi->curHeadID != P_PARAM_GLOBAL){

		if(_NextHeadConfig_Indx != NextHeadConfig_Indx){
			_NextHeadConfig_Indx = NextHeadConfig_Indx;

			LogTxt(&LogMgr, "Head was changed");
			
			/*
			STOP_SavingData = 1;
			m->sm.Step = 1001;
			m->LOCKED = 1;
			TML_MTR_CUT_POWER = 1;
			*/

			FirstScan = 1;
			RefreshAndroid = 1;
			TML_MTR_CUT_POWER = 1;
			m->LOCKED = 1;

			m->sm.Step = 250;
		}

	}

	if(FirstScan){
		FirstScan = 0;

		ReadPermMemory_HS.Ready = 0;
		ReadPermMemory_HS.Go = 1;
		m->xCmd = mstrRESTART;

		LogTxt(&LogMgr, "System Restart");

	}
	
	
	//////////////////////////////////////////////////////////////////
	DumpParamToFile();

	if(TVL_MAP_Allowed)
		TVL_Map_ToFile();

	////////////////////////////////////////////////////
	// Task restart from supemaster
	if ((m->xCmd == mstrRESTART))
	{		

		if(m->pNode[0] && ReadPermMemory_HS.Ready)
		{

			sm_init(&m->sm);
			pend->_intr.TickIncr = 2;

			for (i = 0; i < PROCESS_NUM; i++)
				Process[i].ConfigReady = 0;

			//SerialPortInit(rs ,  pend->RSDeviceStr , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
			//SerialPortInit(rs , "IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
			SerialPortInit(rs, "IF6.ST11.IF1", "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");

			//SerialPortInit(rs , "SS1.IF1.ST5.IF1.ST4.IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");

			/** Master reinit **/
			m->varLastNum		= varLAST_VARIABLE;
			m->paramLastNum 	= parLAST_PARAM;
			m->numSegm			= SEGM_NUM_MAX;
			m->paramEnumLastNum = parENUM_LAST_PARAM;
			m->varEnumLastNum	= varENUM_LAST_VARIABLE;
			m->paramIntgLastNum = parINT_LAST_PARAM;
			m->varIntLastNum	= varINT_LAST_VARIABLE;
			m->paramStrLastNum	= parSTRING_LAST_PARAM;
			m->varStrLastNum	= varSTRING_LAST_VARIABLE;

			memset(nodes, 0, sizeof(LCD_NODE_obj) *NODES_NUM_MAX);

			//ScaledValuesCfg(m); // This is pointer population
			HardCodeCFg(m); 						// This is for pamaeters and variable definition	

			CreateParamList();

			SetupNODES_Android(m, nodes, P_LAST_NODE); // this is what will be displayed on LCD - menu etc

			statusInit			= LCD_MGR_Init(lcd, NUM_LINE, NODES_NUM_MAX, m, rs, nodes, P_INIT, 0.05);

			lcd->EditMode		= EDIT_MODE_VALUE;	// Edit only value of parameter on the screen

			//////////////////////////////////////////
			// Assign HMI pointers from external instances			
			for (i = 0; i < 20; i++)
				hmi->pHMI_Lines[i] = HMI_lines[i];

			for (i = 0; i < 4; i++)
				hmi->pMessage[i] = m->AndIntrf[0].Message[i];

			hmi->pBanner		= m->AndIntrf[0].Banner;

			/////////////////////////////////////////
			TimerStart(&hmi->MsgTmr, 0.1);

			if(NextHeadConfig_Indx < U_BEND_HP_ENC)
				NextHeadConfig_Indx  = U_BEND_HP_ENC;

			if(NextHeadConfig_Indx >= HEAD_INVALID)
				NextHeadConfig_Indx= HEAD_INVALID-1;


			///////////////////////////////

			if((strlen(curCfg->NextProgramName) > 31)
				|| (strlen(curCfg->NextProgramName) == 0))
				strcpy(curCfg->NextProgramName, PRG_NO_NEXT);

			///////////////////////////////			
	
			if(ConfigureHead(m, NextHeadConfig_Indx))
			{
				curCalib->GenSetup[SETUP_HEAD_SETUP] = NextHeadConfig_Indx;
				m->xCmd 	= 0;

				if(RefreshAndroid)
					hmi->PLC_Changes = 0x0001; // new curcCfg Loaded 

				RefreshAndroid = 0;
				
				///// pointers to main objects so Cfg file parser can see them //
				m->Objects.pArcLink_IP_Address = (UDINT)ArcLinkPS_IP_Address;
				
				for(i = 0 ; i < TIG_PS_NUM ; i++)
					m->Objects.pPS_Param_Set[i] = &PS_Param_Set[i];
				
				///////
				_TIG_Mode = 0xFFFF;
				_AVC_Mode = 0xFFFF;
				m->Active			= 1;	// Master active
				m->sm.Step			= 0;	// Main state machine reset
				///////////////
				//ReadPermMemory_HS.Go = 1; //plus Parse Cfg File
				STOP_SavingData = 0;

				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

					case BPV_F_HEAD:
					case BPV_v1_MT:		
					case CLAMSHELL_HEAD:	
						
					 	DC_Mtr[m->pCfg->HeadCfg.INDX.DC_A_Indx].msc.Pos = OLD_MTR_POS[INDX_A_LAST_POS];
						DC_Mtr[m->pCfg->HeadCfg.INDX.DC_B_Indx].msc.Pos = OLD_MTR_POS[INDX_B_LAST_POS];

					break;

					case RUBIX:
					
						btnProgressMax		= 30;
						btnProgressBias 	= 30;

					break;

				}	
				
			}

			//_SETUP_HEAD_SETUP	= curCalib->GenSetup[SETUP_HEAD_SETUP];
			//LoggerInit(&LogMgr, &LogID, "LogBook1", 2000);

			ESTOP_PERMIT		= 1;

			///////////////////////////////////////////
			//GB: calculate hash value for the NODES array	
			if (KK_DoNotHashCalculation)
			{

				gHashValuesArray[0] = HASH_NUM;
				gHashValuesArray[1] = gHashValuesArray[0];

			}
			else 
			{

				gHashValuesArray[0] = hash_of_binary_array(nodes, sizeof(LCD_NODE_obj) *NODES_NUM_MAX);
				gHashValuesArray[1] = hash_of_binary_array(cfg, sizeof(Cfg_typ));

			}

			///////////////////////////////////////////
			VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[0] = 0.0;
			VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[1] = 0.0;
			////////////////////////////////////////////
			TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter = curCfg->Param[parAVC_FbkFilter].val[0];			
			TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kpp = (DINT)(curCfg->Param[parAVC_KPP].val[0] *TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kppMax) ;
			TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kip = (DINT)(curCfg->Param[parAVC_KIP].val[0] *TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kipMax) ;
		
		
			for (i = 0; i < TML_SERVO_NUM; i++)
				TML_MTR_INST[i].xCMD = TML_RESTART;

			for (i = 0; i < PROCESS_NUM; i++)
				Process[i].xCMD = prcsRESTART;

			
			intrf->TestMode 	= 1;
			m->WireDisabled 	= 1;

			

		}


	}

#ifndef  MIG_EXP_SYSTEM

	#ifndef CLAMSHELL
	
	if(!Hardware[1].CouplerOK[0]){ 
		Errors(&ErrorMgr,ERROR_HARDWARE, ERROR_MODULE_ERR ,1);
	}
	#endif
	
#endif

#ifdef  MIG_SYSTEM

	#ifndef CLAMSHELL	

	if(!TIG_MIG_Sw_Inp)
		Errors(&ErrorMgr, ERROR_HARDWARE, ERROR_TIG_MIG_SW_ERR, 0);

	#endif

	
	MIG_ESTOP_Rst_PendantBtn = (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP) && Master[0].ESTOP;
	
	MIG_ESTOP_Rst_Out = MIG_ESTOP_Rst_Btn_Inp || MIG_ESTOP_Rst_PendantBtn;
	

#endif		


	if(ProjectConfig_Hrdw_ERROR){
		Errors(&ErrorMgr, ERROR_SETUP, ERROR_PROJECT_HRDW_ERR, 0);
	}

	/////////////////////////////////////////////////////
	
	EvaluatePosition();

	//////////////////////////
	// Special trick

	if((intrf->GlblBtnCmd[0] & ANDR_BTN_HOME) &&  (intrf->GlblBtnCmd[0] & ANDR_BTN_START)
		&&  (intrf->GlblBtnCmd[0] & ANDR_BTN_AVC_LOCK)) 
		KK_Functionality = 1;
	
	if((intrf->GlblBtnCmd[0] & ANDR_BTN_HOME) &&  (intrf->GlblBtnCmd[0] & ANDR_BTN_STOP)) 
		KK_Functionality = 0;

	//////////////////////////
	// Trigger ESTOP when TML error
	if (KK_TML_PowerON || Hold_Estop_ON){
		
		Hardware[1].Outp[7] = 1; // Four AXIS start Inable and Power to TML motors
		
		if(m->pESTOP_Out)
			*m->pESTOP_Out = 1;

	}
	else {

		
		Hardware[1].Outp[7] = !TML_MTR_CUT_POWER; // Four AXIS start Inable and Power to TML motors
		
		if(m->pESTOP_Out)
			*m->pESTOP_Out = !TML_MTR_CUT_POWER;
	}


	if (curCfg->Param[parLCD_BtnMode].val[0] > 0.0)
		lcd->EntrBtnPressReq = 1; // LCD ENtr button should be hold to edit
	else 
		lcd->EntrBtnPressReq = 0;



	intrf->GlblBtnCmd[0] = (m->AndIntrf[0].GlblBtnCmd[0] &0xFFFFFFFE); // first bit is wheel is not pressed so we mask it;	

	if (intrf->GlblBtnCmd[0] == 0)
		btnProgress = 0;

	intrf->Wheel		= m->AndIntrf[0].Wheel;

	intrf->WheelDelta	= m->AndIntrf[0].Wheel - _AndroidWheel;
	_AndroidWheel		= m->AndIntrf[0].Wheel;


	intrf->LCDEntr		= (m->AndIntrf[0].EntrBtn > 0);
	intrf->TickPlus 	= ((intrf->WheelDelta > 0) || ((intrf->GlblBtnCmd[0] &ANDR_BTN_UP) &&
		 !intrf->_intr._GlblBtnCmd[0]));
	intrf->TickMinus	= ((intrf->WheelDelta < 0) || ((intrf->GlblBtnCmd[0] &ANDR_BTN_DOWN) &&
		 !intrf->_intr._GlblBtnCmd[0]));

	intrf->LCDEntr_PosEdge = (m->AndIntrf[0].EntrBtn && !_AndroidEntrBtn);
	intrf->LCDEntr_NegEdge = (!m->AndIntrf[0].EntrBtn && _AndroidEntrBtn);
	_AndroidEntrBtn 	= m->AndIntrf[0].EntrBtn;

	m->AndIntrf[0].GlblBtnLED[0] = intrf->GlblBtnLED[0];

	if(intrf->TickPlus)
		myCounter++;

	////////////////////////////////////////	
	hmi->curHeadID		= m->AndIntrf[0].curHeadID;
	hmi->curNodeID		= m->AndIntrf[0].curLineID;


	hmi->FiredEvent 	= m->AndIntrf[0].FiredEvent;
	hmi->PleaseUpdate	= m->AndIntrf[0].PleaseUpdate;
	hmi->UserLevel		= m->AndIntrf[0].UserLevel;
	m->AndIntrf[0].FiredEvent = 0;


	/////////////////////////////////////////
	// Security for edit on node
	if (hmi->curHeadID < P_LAST_NODE)
	{ // on one of drill down page

		if (hmi->UserLevel < nodes[hmi->curNodeID].MinUserLevel)
			intrf->LCDEntr = 0;


	}

	/////////////////////////////////
	EvaluateHMI_Android(m, nodes, hmi, intrf);


	//////////////////////////////////////////////

	// Special Setup for particular Heads

	switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){


		case BPV_v2:
		case BPV_v2_MPBO:
		case BPV_v2_MPGH:
		case BPV_v2_MPGV:
			
		case BPV_v2_TT_BO:
		case BPV_v2_MT_BO:
		case BPV_v2_MT_BO_TOS:
		case BPV_v2_TT_BO_TOS:
		// dodane 8July24
		case BPV_v2_Video:	

		// Dodane 12Dec23	
		case BPV_v2_MT:
					

			WRST_TML_Param_Set[0].MappingActive = (curCalib->GenSetup[SETUP_MAPPING_ON] > 0)? 1:0;			

		break;


		default:			

			curCalib->GenSetup[SETUP_MAPPING_ON] = 0;

			WRST_TML_Param_Set[0].MappingActive = 0;


		break;


	}	

	///////////////////////////////////////////////

	switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

		case CLAMSHELL_HEAD:
		case  BPV_v2_Video:

			if(TVL_ROT_Param_Set[0].TotalRescaleFactor != 0.0)	{

				/*
				LimScal_Tilt.x = (float)TML_MTR_INST[TML_AX_AVC].AnInp[1];
				AzzLimScal(&LimScal_Tilt);
				x = LimScal_Tilt.y - curCalib->GenFloatValue[SETUP_FLOAT_TILT_OFFSET];
				*/
				

				//xsprintf(m->AndIntrf[0].MasterModTXT, "Tilt:%1.1f  %d/%d TVL:%d / Enc:%d " ,x  , File_RW._int.PathDoneCnt,
										//(UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0], TVL_ROT_Param_Set[0].EncPos_FBK, actTicks);
										
				//xsprintf(m->AndIntrf[0].MasterModTXT, "T:%1.1f W:%1.1f  %d/%d TVL:%d / Enc:%d " ,x , TML_MTR_INST[TML_AX_WRST].PosFBK , File_RW._int.PathDoneCnt,
					 	//(UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0], TVL_ROT_Param_Set[0].EncPos_FBK, actTicks);	

						
				xsprintf(m->AndIntrf[0].MasterModTXT, "WA:%1.1f	%d/%d TVL:%d / Enc:%d "  , TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx].PosFBK , File_RW._int.PathDoneCnt,
										(UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0], TVL_ROT_Param_Set[0].EncPos_FBK, actTicks);	

			}

		break;


		default:


			if(TVL_ROT_Param_Set[0].TotalRescaleFactor != 0.0)	{		

				if(WRST_TML_Param_Set[0].MappingActive){
					//xsprintf(m->AndIntrf[0].MasterModTXT, "A:%2.1f/%2.1f / TVL:%d / Enc:%d / I:%3.2f ", 1.0/TVL_ROT_Param_Set[0].TotalRescaleFactor, WRST_TML_Param_Set[0].actAngleFromMapping,
					//xsprintf(m->AndIntrf[0].MasterModTXT, "A:%2.1f/%2.1f / TVL:%d / Enc:%d / I:%3.2f ",WRST_TML_Param_Set[0].actAngle , WRST_TML_Param_Set[0].actAngleFromMapping,
					 //TVL_ROT_Param_Set[0].EncPos_FBK, actTicks,VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].FbkVect.x - VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].FbkVect.x );
					xsprintf(m->AndIntrf[0].MasterModTXT, "A:%2.1f/%2.1f / TVL:%d / Enc:%d/ %d;%d  ",WRST_TML_Param_Set[0].actAngle , WRST_TML_Param_Set[0].actAngleFromMapping,
					 TVL_ROT_Param_Set[0].EncPos_FBK, actTicks,File_RW._int.PathDoneCnt, (UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0]);

				}else{

					switch(curCalib->GenSetup[SETUP_INDX_ENBL]) {

					case 1: 

						xsprintf(m->AndIntrf[0].MasterModTXT, "%3.2f %d/%d TVL:%d / Enc:%d / I:%3.2f ", 1.0/TVL_ROT_Param_Set[0].TotalRescaleFactor ,File_RW._int.PathDoneCnt,
					 	(UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0], TVL_ROT_Param_Set[0].EncPos_FBK, actTicks,VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].FbkVect.x - VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].FbkVect.x );	
					

					break;
					
					default: 

						xsprintf(m->AndIntrf[0].MasterModTXT, "%3.2f %d/%d TVL:%d / Enc:%d ", 1.0/TVL_ROT_Param_Set[0].TotalRescaleFactor ,File_RW._int.PathDoneCnt,
					 	(UINT) curCfg->Param[parGEN_WeldProgramRepeat].val[0], TVL_ROT_Param_Set[0].EncPos_FBK, actTicks);	
					

					break;						

					}
				}
			}



		break;




	}

			

	

	 // m->curSegm[FR_SEGM]
	ProcessEvents_Android(m, nodes, hmi, intrf);

	m->AndIntrf[0].MsgTmr_Done = !hmi->MsgTmr.Q;
	m->AndIntrf[0].MsgTmr_Duration = (UINT) (hmi->MsgTmr.PT / 1000.0);
	m->AndIntrf[0].MasterMode = m->Mode;

	if((hmi->curHeadID != P_PARAM_TIG) 
		&& (hmi->curHeadID != P_PARAM_AVC)
		&& (hmi->curHeadID != P_PARAM))
		ChangeSegmentList();

	if (_PLC_Changes != hmi->PLC_Changes)
	{ // New flag
		m->AndIntrf[0].PLC_Changes = hmi->PLC_Changes; // New flag,
		_PLC_Changes		= hmi->PLC_Changes;

	}

	else if (m->AndIntrf[0].PLC_Changes == 0)
	{ // Changes accepted
		hmi->PLC_Changes	= 0;
		_PLC_Changes		= hmi->PLC_Changes;

	}



	if (!m->AndIntrf[0].xCMD)
	{

		if ((hmi->Node2JumpID >= P_MENU) && hmi->xCMD)
		{

			m->AndIntrf[0].xCMD = hmi->xCMD;
			m->AndIntrf[0].JumpLineID = hmi->JumpLineID;
			m->AndIntrf[0].Node2JumpID = hmi->Node2JumpID;

			hmi->xCMD			= 0;
			hmi->JumpLineID 	= 0;
			hmi->Node2JumpID	= 0;
		}
		else 
		{
			if(hmi->xCMD){				
				m->AndIntrf[0].xCMD = hmi->xCMD;
				hmi->xCMD = 0;
			}
		}
	}

	// No Buttons pressed when pop up message is on
	// I removed it because I could not evaluate holding buttons - 8 November 2017
	/*
	if (m->AndIntrf[0].MsgTmr_Done)
	{
		intrf->GlblBtnCmd[0] = 0;
		m->AndIntrf[0].GlblBtnCmd[0] = 0;
	}
	*/

	///////////////////
	// Buttons
	m->AndIntrf[0].GlblBtnLED[0] = 0;

	if (intrf->TestMode)
		m->AndIntrf[0].GlblBtnLED[0] |= 0x1;

	if (!m->WireDisabled)
		m->AndIntrf[0].GlblBtnLED[0] |= 0x2;

	if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0])
		m->AndIntrf[0].GlblBtnLED[0] |= 0x4;

	if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
		m->AndIntrf[0].GlblBtnLED[0] |= 0x8;

	if (!m->ToolHeadID)
		m->AndIntrf[0].GlblBtnLED[0] |= 0x10;
	else 
		m->AndIntrf[0].GlblBtnLED[0] |= 0x20;


	m->AndIntrf[0].GlblBtnLED[1] = 0;

	switch (curCalib->GenSetup[SETUP_OSC_EXC_MODE])
	{
		case 1:
			m->AndIntrf[0].GlblBtnLED[1] |= 0x1;
			break; // Dwell In

		case 2:
			m->AndIntrf[0].GlblBtnLED[1] |= 0x2;
			break; // Dwell Out		

		default:
			break; // Excursion time
	}

	//m->AndIntrf[0].curNumSegments = m->numZone;
	//m->AndIntrf[0].maxNumSegments = FR_SEGM_NUM_MAX;
	m->AndIntrf[0].curNumSegments = m->curSegm[ZONE];
	m->AndIntrf[0].maxNumSegments = m->numZone + 1;

	Master_numZone = m->numZone;

	hmi->xCMD = 0;


	/********************************************************/

	if (m->ESTOP && lcd->Intr.SlowBlink)
		strcpy(lcd->Banner, "	ESTOP !!!!! ");



	if (Force_ENTR_LCD)
	{
		intrf->LCDEntr		= 1;
		lcd->EditMode		= EDIT_MODE_VALUE;
		lcd->Intr.editStep	= 3;
	}

	if (Force_ENTR_LCD && !_Force_ENTR_LCD)
		intrf->LCDEntr_PosEdge = 1;

	else if (!Force_ENTR_LCD && _Force_ENTR_LCD)
		intrf->LCDEntr_NegEdge = 1;

	_Force_ENTR_LCD 	= Force_ENTR_LCD;

	Force_ENTR_LCD		= 0;

	if (!ANDROID_Pendant)
	{

		if (statusInit == 1)
		{

			lcd->Intrf.WheelDelta = intrf->WheelDelta;

			lcd->Intrf.LCDEntr	= intrf->LCDEntr;
			lcd->Intrf.LCDEntr_NegEdge = intrf->LCDEntr_NegEdge;
			lcd->Intrf.LCDEntr_PosEdge = intrf->LCDEntr_PosEdge;

			lcd->Intrf.TickMinus = intrf->TickMinus;
			lcd->Intrf.TickPlus = intrf->TickPlus;

			status				= LCD_MGR_Srv(lcd);

			// Trigger sending to screen every second loop
			if (SendRS232)
			{

				if (!rs->txBufRequest)
					rs->txBufRequest = 1;

			}

			SendRS232			= !SendRS232;
			/****************************************/
		}
	}

	/************************************************************/
	

	if (m->LOCKED && (m->sm.Step < 230))
	{

		for (i = 0; i < PROCESS_NUM; i++)
			Process[i].xCMD = prcsHalt;

		for (i = 0; i < TML_SERVO_NUM; i++)
			TML_MTR_INST[i].xCMD = TML_RESET_ONLY_CMD;


		m->Mode 			= MASTER_MODE_INIT;

		STOP_SavingData = 1;
		m->sm.Step			= 230;
	}


	////////////////////
	// Android connection
	if (ANDROID_Pendant)
	{

		if ((m->sm.Step > 1))
		{ // Allow init first

			if (_datagramCountTotal[0] != datagramCountTotal[0])
				TimerStart(&AndroidConnect_Tmr, 1.5);
				//TimerStart(&AndroidConnect_Tmr, 1.0);

			if (TimerIsTimeOut(&AndroidConnect_Tmr))
			{

				AndroidConnect_Tmr.IN = 0;
				AndroidConnect_Tmr.Q = 0;

				if (m->AndIntrf[0].Alive)
				{
					Errors(&ErrorMgr, ERROR_PENDANT, ERROR_ADROID_COM_LOST, 0);
					LogTxt(&LogMgr, "Pendant com. error");
					TML_MTR_CUT_POWER = 1;
					
					KK_CatchAndroidComErr = AndroidConnect_Tmr.ET;
					KK_CatchAndroidErr	= 1;
				}

				m->AndIntrf[0].Alive = 0;

			}

		}
		else 
		{

			AndroidConnect_Tmr.IN = 0;
			AndroidConnect_Tmr.Q = 0;


		}

	}

	///////////////////////////////////////////////
	//	ESTOP and ERROS handling
	
	if (m->pESTOP_Inp)
	{

#ifdef TEST_ONLY_PLC_ANDROID
		(*m->pESTOP_Inp)	= 1;					// Remove KK
#endif		
		if(!(*m->pESTOP_Inp))		
		{			
			//ESTOP_Inp_Status = 1;
			m->ESTOP			= 1;
			ESTOP_Dly_Tmr.IN	= 0;
			ESTOP_Dly_Tmr.Q		= 0;
			Ready2StoreLastPos = 0;

			if ((m->sm.Step > 5 && m->sm.Step < 190) || (m->sm.Step == 0) || (m->sm.Step == 1))
			{
				Errors(&ErrorMgr,ERROR_ESTOP,ERROR_ESTOP_ERR,1);
				TML_MTR_CUT_POWER = 1;
				m->sm.Step	= 190;
			}

			// This is only when you have to clear Estop then clear error - not opposite
			if(!ErrorMgr.Error)
				Errors(&ErrorMgr,ERROR_ESTOP,ERROR_ESTOP_ERR,1);
			


		}
		else {

			ESTOP_Inp_Status = 0;

			if(ProjectConfig == project_US_Orbital) 
				m->ESTOP = TML_MTR_CUT_POWER;
			
			else{

				//ESTOP_Dly_Tmr.IN	= !TML_MTR_CUT_POWER;								
				//m->ESTOP = !ESTOP_Dly_Tmr.Q;

				m->ESTOP = 0;

			}
		}	


	}


	if (ErrorMgr.Error && !m->LOCKED && !KK_ErrorsHold)
	{


		if (m->sm.Step > 5 && m->sm.Step < 190)
		{ // Give chance to clear error by step first steps and download TML programs 


			// Always Coolant On
			if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
				Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;

			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;

			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsHalt;
			m->Mode 			= MASTER_MODE_ERROR;

			Ready2StoreLastPos = 0;
			m->sm.Step			= 200;

		}

	}

	////////////////////////////////////////////////////////////////
	// Extra Imperatives
	//curCfg->Param[parTVL_RunDist].val[0] = (float)curCalib->GenSetup[SETUP_TVL_DISTANCE];
	///////////////////////////////////////////////////////////////
	KK_HomeBtn			= (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_HOME));
	KK_StopBtn			= (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP));

	if((intrf->GlblBtnCmd[0] & ANDR_BTN_HOME) && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP))
		Weld_Report_Status = 0;

	// MAIN STATES //
	switch (m->sm.Step)
	{
		case 0:	sm_text("Start", &m->sm);	

			Ready2StoreLastPos = 0;

			if (ANDROID_Pendant)
			{ // start only when is android connection				

				if (_datagramCountTotal[0] == datagramCountTotal[0]){
					//TML_MTR_CUT_POWER = 1;
					break;
				}

			}		

			if(intrf->GlblBtnCmd[0] & (0xFFFFFFFF
					-ANDR_BTN_STOP -0x04000000 - 0x10000000 - 0x20000000 - 0x02000000 - 0x08000000 )){			
						
				strcpy(hmi->pMessage[0], " DO NOT HOLD BUTTON");
				strcpy(hmi->pMessage[1], " ");
				strcpy(hmi->pMessage[2], " ");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 1.0);				

				break;

			}
			else if (ErrorMgr.Error && !KK_Functionality && !KK_ErrorsHold)
			{
				
				m->Mode 			= MASTER_MODE_ERROR;
				break;
			}
			
			else if (m->ESTOP)
			{
				
				m->Mode 			= MASTER_MODE_ESTOP;

								
				break;
			}			
			
			hmi->Node2JumpID = P_MODE_1;
			//hmi->JumpLineID = 0;
			//hmi->xCMD = JUMP2ID_CMD;
			hmi->DynPageNodeIndx = 0xFFFF;
			lcd->StaticMessageVisible = 0;
			ParametrAssignToPointers(m, m->curSegm[ZONE]);
			intrf->ModeLED = 1;
			PleaseUpdateHeadSetup(m);
			AckAllErrors(&ErrorMgr);
			TML_MTR_CUT_POWER = 0;

			
			/*
			for (i = 0; i < PROCESS_NUM; i++)
				Process[i].xCMD = prcsRESTART;

			for (i = 0; i < TML_SERVO_NUM; i++)
			{

				TML_MTR_INST[i].xCMD = TML_RESTART;
				TML_MTR_INST[i]._int.restartCnt = 0;
				TML_MTR_INST[i]._int.Init = 0;
			}
			*/
			

			TML_Restart_Step = 1;
			Process_Restart_Step = 0;

			m->SeqStep = 0;
			m->Mode = MASTER_MODE_INIT;

			if(curCalib->GenSetup[SETUP_INDX_ENBL] == 0 
				|| curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // Not Installed or OSC Index

				if( m->pCfg->HeadCfg.INDX.VECT_A_Indx > 0)
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].simul = 1;

				if( m->pCfg->HeadCfg.INDX.VECT_B_Indx > 0)
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].simul = 1;

				if(m->pCfg->HeadCfg.INDX.TML_A_Indx > 0)
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx].Disable = 1;

				if(m->pCfg->HeadCfg.INDX.TML_B_Indx > 0)
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx].Disable = 1;
			

			}else{


				if( m->pCfg->HeadCfg.INDX.VECT_A_Indx > 0)
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].simul = 0;

				if( m->pCfg->HeadCfg.INDX.VECT_B_Indx > 0)
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].simul = 0;

				if(m->pCfg->HeadCfg.INDX.TML_A_Indx > 0)
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx].Disable = 0;

				if(m->pCfg->HeadCfg.INDX.TML_B_Indx > 0)
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx].Disable = 0;



			}

			// Coolant always ON
			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsForceON;
			curTML_indx = 0;
			TimerStart(&m->sm.Tmr1, 4.0); // Time to Init TML servo
			//Hold_Estop_ON = 0;
			m->LOCKED = 0;
			m->sm.Step++;
			
		break;

		case 1:	sm_text("Check TML", &m->sm);
		

			//TML_STOP_SEND_ERROR = 0;

			if(!((TML_Restart_Step == 0) && (Process_Restart_Step == 0)))
				break;
			

						
			if (TimerIsTimeOut(&m->sm.Tmr1))
			{ // Check why TML did not start

				status				= 0;
				_status 			= status;

				for (i = 0; i < TML_SERVO_NUM; i++)
				{

					if (TML_MTR_INST[i].pState && !TML_MTR_INST[i].Disable)
					{


						if (TML_MTR_INST[i].xCMD == TML_WAIT_FOR_DOWNLOAD)
						{

							LogTxt(&LogMgr, "Wrong Setup %s ", TML_MTR_INST[i].Name);

							if (_status < 2)
								status = 2;


						}

						else if (TML_MTR_INST[i]._int.restartCnt > 10)
						{

							LogTxt(&LogMgr, "CAN com. Err. %s ", TML_MTR_INST[i].Name);
							if(TML_MTR_INST[i].ErrorReg)
								Errors(&ErrorMgr ,ERROR_MOTOR , ERROR_DRV_01 + i , 17);
							else
								Errors(&ErrorMgr ,ERROR_MOTOR , ERROR_DRV_01 + i , 18);

							if (_status < 1)
								status = 1;

						}
						else if (!TML_MTR_INST[i]._int.Init)
						{

#ifndef TEST_ONLY_PLC_ANDROID

							if (TML_MTR_INST[i].xCMD)
								LogTxt(&LogMgr, "Not Comun. %s ", TML_MTR_INST[i].Name);
							else 
								LogTxt(&LogMgr, "Not Init. %s ", TML_MTR_INST[i].Name);

							if(TML_MTR_INST[i].ErrorReg)
								//Errors(&ErrorMgr ,ERROR_MOTOR , ERROR_TML_1 + i , TML_MTR_INST[i].ErrorReg);
								Errors(&ErrorMgr ,ERROR_MOTOR , ERROR_DRV_01 + i , 17);
							else
								Errors(&ErrorMgr ,ERROR_MOTOR , ERROR_DRV_01 + i , 18);

							if (_status < 1)
								status = 1;

#endif
						}

						_status 			= status;

					}

				}

			}
			else 
			{


				status				= 1;


				for (i = 0; i < TML_SERVO_NUM; i++)
				{

					if (TML_MTR_INST[i].pState)
					{

						if (!TML_MTR_INST[i]._int.Init)
							status = 0;

					}

				}

				if (status)
				{
					m->sm.Step			= 5;
					break;
				}
				else 
					break;

			}

			if (KK_DoNotChangeTMLProgram)
			{
				if (status == 2)
					status = 0;
			}

			KK_actPosProcss = status;

			switch (status)
			{
				case 1:

					
					//Errors(&ErrorMgr, ERROR_MOTOR, ERROR_TML_1, 10);

					// Display which one has error in previous step

					if(!KK_ErrorsHold){
						
						TML_MTR_CUT_POWER = 1;					
						m->sm.Step = 200;
					}
					else{

						if (ANDROID_Pendant){

							m->sm.Step = 5;

						}
						else 
						{

							TimerStart(&m->sm.Tmr1, 5.0); // Time to Init Arclink						
							m->sm.Step	= 49;

						}


					}	
					
				break;

				case 2:

					if(!KK_ErrorsHold){
						
						TML_MTR_CUT_POWER = 1;
						m->LOCKED = 1;
					}
					else{

						if (ANDROID_Pendant){

							m->sm.Step = 5;

						}
						else 
						{

							TimerStart(&m->sm.Tmr1, 5.0); // Time to Init Arclink						
							m->sm.Step	= 49;

						}
					}	
					
				break;

				default:
					
					if (ANDROID_Pendant){

						m->sm.Step = 5;

					}
					else 
					{

						TimerStart(&m->sm.Tmr1, 5.0); // Time to Init Arclink						
						m->sm.Step	= 49;

					}

				break;
			}

			break;

		case 5:	sm_text("W: Pendant ", &m->sm);

			 TML_STOP_SEND_ERROR = 0;
			
			//if(datagramCountTotal[0] > 100)
#ifndef TEST_ONLY_PLC_ANDROID

			if (m->AndIntrf[0].Alive)
#else

				if(1)
#endif
				{

					KK_AUTO_Test		= 0;

					TimerStart(&DoubleStop_TMR, 0.1);
					ParametrAssignToPointers(m, m->curSegm[ZONE]);

					m->SubSeqStep = 1;

					TimerStart(&m->sm.Tmr1, 60.0);		// Time to Init Arclink
					m->sm.Step			= 49;

				}
		break;

		case 49: sm_text("W: Arclink PS Rdy", &m->sm);

			//Hold_Estop_ON = 0;

			//if(strlen(ArcLinkPS_IP_Address) == 0){
			if(PS_Param_Set[0].Mode == 0){ // NO Arclink

				SetTVL_Pos_Feedback();
				
				MV[0].xCMD = MV_RESTART_CMD;
			
				for (i = 0; i < PROCESS_NUM; i++)					
					Process[i].fParam[0] = 0.0;
				
				if(!intrf->GlblBtnCmd[0])
					m->sm.Step = 50;
				
				break;

			}
			else if (robot)
			{ // Arclink connection

				if (robot->Cntrl.Fbk.Status & PS_Ready){
					SetTVL_Pos_Feedback();
					TVL_Latch_Latched = 0;
					
					MV[0].xCMD = MV_RESTART_CMD;

					for (i = 0; i < PROCESS_NUM; i++)					
						Process[i].fParam[0] = 0.0;
					
					if(!intrf->GlblBtnCmd[0])
						m->sm.Step = 50;

				}				
				else if(TimerIsTimeOut(&m->sm.Tmr1)){

					Errors(&ErrorMgr,ERROR_TIG_PROC, ERROR_ARCLINK_COM_LOST, 1 );						
					LogTxt(&LogMgr , "Arclink TimeOut "  );	

				}
				
			}
			else {
				SetTVL_Pos_Feedback();
				TVL_Latch_Latched = 0;
			
				MV[0].xCMD = MV_RESTART_CMD;

				for (i = 0; i < PROCESS_NUM; i++)					
					Process[i].fParam[0] = 0.0;
				
				if(!intrf->GlblBtnCmd[0])
					m->sm.Step = 50;
			}

		break;

		// Watch for button pressed 
		case 50: sm_text("Normal Stop", &m->sm);

			Ready2StoreLastPos = 1;

			if(PS_Param_Set[0].Mode){
				if (WatchArcLinkError(m, robot))
					break;
			}

			m->Mode = MASTER_MODE_STOP;

			if (_curZone != m->curSegm[ZONE])
				ParametrAssignToPointers(m, m->curSegm[ZONE]);

			_curZone = m->curSegm[ZONE];
			StopModeAndroidButtons(m, hmi, intrf, lcd);
			StartBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START));
			StopBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP));
			/// was added on March 28 , 2019///
			if(StopBtn){
				TVL_Latch_Latched = 0;
				_TVL_Latch_Latched	= TVL_Latch_Latched;

			}
			///////
			TVL_ROT_Param_Set[0].HeadSwitch = (curCfg->Param_ENUM[parENUM_PathMode].val[0] == 0.0)? 1:0;
			TVL_ROT_Param_Set[0].Extras = (UINT)(curCalib->GenSetup[SETUP_EXTRAS]);
					

			switch(curCalib->GenSetup[SETUP_CONTIN_WELD]){

				case 1: // CONTINUE ONE WAY 

					MasterSequence_Contin_OneWay(m, hmi, StartBtn, StopBtn);
					
				break;

				case 2: // CONTINUE BOTH DIRECTION

					MasterSequence_Contin(m, hmi, StartBtn, StopBtn);
			
				break;

				default: // STANDARD

					MasterSequence(m, hmi, StartBtn, StopBtn);

				break;



			}
	

						
			hmi->CalibMode = 0; // No Calibration	

			if(m->SeqStep > 0)
				m->sm.Step = 51;

		break;

		case 51:
			sm_text("Normal Run", &m->sm);


			if(PS_Param_Set[0].Mode){
				if (WatchArcLinkError(m, robot))
					break;
			}

			m->Mode = MASTER_MODE_RUN;

			if (_curZone != m->curSegm[ZONE])
				ParametrAssignToPointers(m, m->curSegm[ZONE]);

			_curZone = m->curSegm[ZONE];

			//if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped )
			if ((Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				|| (m->TestMode))
				RunModeAndroidButtons(m, hmi, intrf, lcd);
			else 
				StopModeAndroidButtons(m, hmi, intrf, lcd);

			StartBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START));
			StopBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP));

			//MasterSequence(m, hmi, StartBtn, StopBtn);
			
			


			switch(curCalib->GenSetup[SETUP_CONTIN_WELD]){


				case 1: // CONTINUE ONE WAY 

					MasterSequence_Contin_OneWay(m, hmi, StartBtn, StopBtn);
					
				break;

				case 2: // CONTINUE BOTH DIRECTION

					Weld_Report_Status = 2; // Always ready to acknowledge
					MasterSequence_Contin(m, hmi, StartBtn, StopBtn);
			
				break;				

				default: // STANDARD

					MasterSequence(m, hmi, StartBtn, StopBtn);

				break;



			}

			
			

			if (m->SeqStep == 0)
				m->sm.Step = 50;

		break;

		case 100:	sm_text("Calibration Stop", &m->sm);

			Ready2StoreLastPos = 1;
		
			m->Mode = MASTER_MODE_CALIB_STOP;
			StopCalibrationModeAndroidButtons(m, hmi, intrf, lcd);
			StartBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START));
			StopBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP));

			switch (CalibrationSequence(m, hmi, StartBtn, StopBtn))
			{
				case 0: // Not Ready						
				break;

				case 1: //	Stop						
				break;

				case 2: //	Run					
					m->sm.Step = 101;
				break;
			}

		break;

		case 101: sm_text("Calibration Run", &m->sm);
		
			m->Mode = MASTER_MODE_CALIB_RUN;
			RunCalibrationModeAndroidButtons(m, hmi, intrf, lcd);
			StartBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START));
			StopBtn = (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP));

			switch (CalibrationSequence(m, hmi, StartBtn, StopBtn))
			{
				case 0: // Not Ready
					m->sm.Step = 100;
				break;

				case 1: //	Stop				
					m->sm.Step = 100;
				break;

				case 2: //	Run 				
				break;
			}

		break;

		////////////////////////////////////////
		////////////////////////////////////////					
		case 190: sm_text("ESTOP Catched!", &m->sm);
		
			m->Mode = MASTER_MODE_ESTOP;

			TVL_Latch_Latched = 0;

			LogMgr.Cmd.PauseLog = 0;

			/*
			hmi->Node2JumpID = P_INIT;
			hmi->JumpLineID = 0;
			hmi->xCMD = JUMP2ID_CMD;
			hmi->DynPageNodeIndx = P_INIT;
			*/
			LogTxt(&LogMgr, " ESTOP Pressed");

			for (i = 0; i < PROCESS_NUM; i++){

				if(i == m->pCfg->HeadCfg.GAS.PROC_Indx)
					Process[i].xCMD = prcsSTOP;
				else
					Process[i].xCMD = prcsHalt;
				
			}	

			Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTOP;
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // remain gas
			m->SeqStep = 0;
			ParametrAssignToPointers(m, m->curSegm[ZONE]);		

			TimerStart(&m->sm.Tmr1, 2.5);
			
			m->sm.Step++;
			
		break;

		case 191: sm_text("Wait: ESTOP", &m->sm);


			if(!TimerIsTimeOut(&m->sm.Tmr1))
				break;
			

			if (!m->ESTOP)
			{
				
				TVL_Latch_Latched = 0;
				
				if(!intrf->GlblBtnCmd[0]){		

					if(ErrorMgr.Error){

						if(KK_Functionality)
							m->sm.Step = 0;
						else	
							m->sm.Step = 200;

					}
					else{

#ifdef TML_CMD_LOGS

						memset(&Cmd_Log_Mgr , 0 , sizeof(Cmd_Log_Mgr));
						Cmd_Log_Mgr.AxisID_filter = AxisID_filter;
#endif						
					
						m->sm.Step = 0;

					}

				}else {

					strcpy(hmi->pMessage[0], " DO NOT HOLD BUTTON");
					strcpy(hmi->pMessage[1], " ");
					strcpy(hmi->pMessage[2], "It is recovery");
					strcpy(hmi->pMessage[3], "after ESTOP");
					TimerStart(&hmi->MsgTmr, 1.0);	

				}

			}
			else if(m->pESTOP_Out){ // For US Orbital only - allow to clear ESTOP
				TML_MTR_CUT_POWER = 0;
				TML_STOP_SEND_ERROR = 1;


			}
			else
				TML_STOP_SEND_ERROR = 1;
				

		break;

		////////////////////////////////////////
		////////////////////////////////////////
		case 200: sm_text("ERROR !", &m->sm);
		
			m->Mode = MASTER_MODE_ERROR;

			if (m->sm.Step != m->sm.oldStep)
			{

				if (!m->ESTOP)
					hmi->DynPageNodeIndx = P_ERRORS;

				LogTxt(&LogMgr, " Get ERROR ");

			}
			else if (ErrorMgr.Error <= ERROR_FATAL)
			{
				hmi->DynPageNodeIndx = P_ERRORS;
				m->sm.Step			= 255;

			}
			else if (!ErrorMgr.Error)
			{
				m->Mode 			= MASTER_MODE_INIT;		


				if(!(intrf->GlblBtnCmd[0] & (0xFFFFFFFF -ANDR_BTN_STOP))){

					//////////////////////////
					if (robot)
					{ // Arclink connection

						if (robot->Cntrl.Fbk.Error > 0)
							robot->Cntrl.Cmd.RST = 1;

					}
					/////////////////////////				
					TimerStart(&m->sm.Tmr1, 3.0);
					
					TML_MTR_CUT_POWER = 0;
					
					TVL_Latch_Latched = 0;

					//for (i = 0; i < TML_SERVO_NUM; i++)
						//TML_MTR_INST[i].xCMD = TML_RESTART;					
					
					
					m->sm.Step++;

				}
				
			}
			/*
			else if(intrf->GlblBtnCmd[0] & (0xFFFFFFFF - ANDR_BTN_ENTER
					-ANDR_BTN_STOP -0x04000000 - 0x10000000 - 0x20000000 - 0x02000000 - 0x08000000   )){ // all butons excludes navigation buttons
					
					strcpy(hmi->pMessage[0], " DO NOT HOLD BUTTON");
					strcpy(hmi->pMessage[1], "");
					strcpy(hmi->pMessage[2], "It is ERROR state");
					strcpy(hmi->pMessage[3], "");
					TimerStart(&hmi->MsgTmr, 1.0);	
					
			}
			*/
			else if (!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] & ANDR_BTN_STOP))
			{				
					AckAllErrors(&ErrorMgr);				
			
			}

			if(m->ESTOP)
				m->sm.Step = 190;
				
			
		break;

		case 201: sm_text("No Errors wait", &m->sm);

#ifdef MIG_EXP_SYSTEM

			if(Kick_TML_Cmd != 0)
				break;

#endif

			if (TimerIsTimeOut(&m->sm.Tmr1))
			{
				/*
				hmi->Node2JumpID	= P_MODE_1;
				hmi->JumpLineID 	= 0;
				hmi->xCMD			= JUMP2ID_CMD;
				*/

				/*
				TVL_Latch_Latched = 0;

				for (i = 0; i < TML_SERVO_NUM; i++)
					TML_MTR_INST[i].xCMD = TML_RESTART;
				*/	

				m->sm.Step			= 0;

			}

		break;

		////////////////////////////////////////////
		/////  New Head Configuration Selected
		case 230: sm_text("Please Clear Estop", &m->sm);

			/*
			Hold_Estop_ON = 1;
			TML_MTR_CUT_POWER = 0;

			if (!ESTOP_Inp_Status)
				m->sm.Step++;
			*/

			//TML_MTR_CUT_POWER = 1;
			//TML_STOP_SEND_ERROR = 1;
			//m->LOCKED = 1;	

			
			m->LOCKED = 1;
			TML_MTR_CUT_POWER = 1;
			TML_STOP_SEND_ERROR = 1;

			if(m->ESTOP
				|| (ProjectConfig == project_FourAxisTig)){
			
				LogTxt(&LogMgr, "Checking TML Setup");			
				
				m->sm.Step++;

			}
			
		break;

		case 231: sm_text("Check (2) TML", &m->sm);


			if(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START)){
						
						
				for (i = 0; i < TML_SERVO_NUM; i++){

					TML_MTR_INST[i]._int.restartCnt = 0;
					TML_MTR_INST[i]._int.Init = 0;
					TML_MTR_INST[i].xCMD = TML_RESTART;

				}

				TimerStart(&m->sm.Tmr1, 4.0);
			
				m->sm.Step++;

				
			}
		
			else if(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP)){

				m->sm.Step = 999;

			}
				
			
		break;

		case 232: sm_text("Check TML (3)", &m->sm);

		
			if(ESTOP_Inp_Status){
				m->sm.Step = 230;
				break;
			}
			

			if (TimerIsTimeOut(&m->sm.Tmr1))
			{

				status	= 0;

				for (i = 0; i < TML_SERVO_NUM; i++)
				{

					if (TML_MTR_INST[i].pState && !TML_MTR_INST[i].Disable)
					{

						if (status == 0)
						{ // Catch only first

							if(TML_MTR_INST[i].xCMD)
							{
								/*
								if (((TML_MTR_INST[i].xCMD == TML_WAIT_FOR_DOWNLOAD) || (TML_MTR_INST[i].xCMD <= 9))
									&& (!TML_MTR_INST[i]._cfg.NotAutoDownload))
								{ // Reset was blocked
									curTML_indx = i;
									status = 1;
								}
								*/

								if ((TML_MTR_INST[i].xCMD == TML_WAIT_FOR_DOWNLOAD) || (TML_MTR_INST[i].xCMD <= 9))
									
								{ 
									curTML_indx = i;
									if((!TML_MTR_INST[i]._cfg.NotAutoDownload) || KK_Can_Download_Everything ){
										//curTML_indx = i;
										status = 1;
									}
									else{

										LogTxt(&LogMgr, "Change %s Setup only in Diagn. ", TML_MTR_INST[curTML_indx].Name);
										
										strcpy(hmi->pMessage[0], "Setup Change of");
										strcpy(hmi->pMessage[1], TML_MTR_INST[curTML_indx].Name);
										xsprintf(hmi->pMessage[2], "can be done");
										strcpy(hmi->pMessage[3], "only in Diagnostic");
										TimerStart(&hmi->MsgTmr, 3.0);

										status = 2;

									}
										
								}
							}

						}
					}

				}

				if (status == 1)
					m->sm.Step++;
					//m->sm.Step = 1999;

				else if (status == 2)
					m->sm.Step = 230;
					//m->sm.Step = 1999;

				else 
				{
					m->sm.Step = 246;
				}

			}

		break;

		case 233: sm_text("Start Download", &m->sm);

			
			if(ESTOP_Inp_Status){
				m->sm.Step = 230;
				break;
			}

			m->LOCKED = 1;
			TML_MTR_CUT_POWER = 1;
			TML_STOP_SEND_ERROR = 1;
				

			if(m->ESTOP
				|| (ProjectConfig == project_FourAxisTig)){

				if(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_START)){

					/*
					m->LOCKED = 1;
					TML_MTR_CUT_POWER = 1;
					TML_STOP_SEND_ERROR = 1;
					*/
				
					if(TML_Setup_From_Diagn)
						m->LOCKED = 1;

					if (File_RW.CMD == 0)
					{
						TML_MTR_INST[curTML_indx].xCMD = TML_DOWNL;
						TimerStart(&m->sm.Tmr1, 30.0);
						m->sm.Step++;
					}
					else 
					{
						LogTxt(&LogMgr, "File Manager busy");
						m->sm.Step			= 1;
					}

				}
				else if(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP)){

					m->sm.Step = 999;

				}
			}
			

		break;

		case 234: sm_text("Wait Downloading", &m->sm);
			
			if(ESTOP_Inp_Status){
				m->sm.Step = 230;
				break;
			}
			
			if (TML_MTR_INST[curTML_indx].xCMD == 107)
			{
				TimerStart(&m->sm.Tmr1, 4.0);
				m->sm.Step = 235;				
			}
			else if (TML_MTR_INST[curTML_indx].xCMD == 106)
			{
				TimerStart(&m->sm.Tmr1, 4.0);
				m->sm.Step = 236;
			}
			else if ((TML_MTR_INST[curTML_indx].xCMD == TML_DOWNL_DONE)
				|| (TML_MTR_INST[curTML_indx].xCMD == TML_IDLE)){

				LogTxt(&LogMgr, "%s Setup is Uploaded" ,TML_MTR_INST[curTML_indx].Name );
				m->sm.Step = 245;

			}
			
			else if (TimerIsTimeOut(&m->sm.Tmr1)){

				LogTxt(&LogMgr, "%s Load Setup TimeOut" ,TML_MTR_INST[curTML_indx].Name );
				
				strcpy(hmi->pMessage[0], "Setup Download To");
				strcpy(hmi->pMessage[1], TML_MTR_INST[curTML_indx].Name);
				xsprintf(hmi->pMessage[2], "is Timeouted");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 3.0);
				
				m->sm.Step = 246;

			}

		break;

		case 235:  sm_text("Wait Downloading1", &m->sm);

			
			if(ESTOP_Inp_Status){
				m->sm.Step = 230;
				break;
			}
			
			
			if (TML_MTR_INST[curTML_indx].xCMD != 107)
			{
				m->sm.Step = 234;
			}
			else if (TimerIsTimeOut(&m->sm.Tmr1))
			{
				//TML_MTR_INST[curTML_indx].xCMD = TML_DOWNL;
				LogTxt(&LogMgr, "Setup Transm. Error %s", TML_MTR_INST[curTML_indx].Name);
				m->sm.Step = 230;
			}

		break;

		case 236:

			
			if(ESTOP_Inp_Status){
				m->sm.Step = 230;
				break;
			}
		
			
			if (TML_MTR_INST[curTML_indx].xCMD != 106)
			{
				m->sm.Step = 234;
			}
			else if (TimerIsTimeOut(&m->sm.Tmr1))
			{
				LogTxt(&LogMgr, "Setup Transm. Error %s", TML_MTR_INST[curTML_indx].Name);
				m->sm.Step = 230;
			}

		break;

		case 245: // Try again setup checkout

			if(TML_Setup_From_Diagn){
				TML_Setup_From_Diagn = 0;
				m->sm.Step = 222; // Got o to step 0
				//m->sm.Step = 1000; // Freeze and wait for Restart
			}	
			else	
				m->sm.Step = 231;
		break;

		case 246: // Restart Whole system <Manually			

			/*
			if(TML_Setup_From_Diagn){
				m->sm.Step = 1000; // Freeze and wait for Restart
			}	
			else	
				m->sm.Step = 999; // Freeze and wait for Restart
			*/
			
			TML_Setup_From_Diagn = 0;
			//m->sm.Step = 222; // Got o to step 0
			m->LOCKED = 0;	
			m->sm.Step = 0; 
			//m->xCmd = 1; //Restart entire system

			TML_Setup_From_Diagn = 0;
		break;


		case 250: // SYSTEM MUST BE RESTARTED = Power Cyclic

			sm_text("Head Restarted 0", &m->sm);

			LogTxt(&LogMgr, "Heasd Setup Change - must reboot");
										
			strcpy(hmi->pMessage[0], "Head Setup");
			strcpy(hmi->pMessage[1], "Changed");
			xsprintf(hmi->pMessage[2], "Please");
			strcpy(hmi->pMessage[3], "Power OFF/ON");
			TimerStart(&hmi->MsgTmr, 3.0);

			TML_MTR_CUT_POWER = 1;

			m->sm.Step++;

		break;

		case 251:

			sm_text("Head Restarted 1", &m->sm);
			
			SYSTEM_LOCKED = 1;

		break;
	
		case 999:  sm_text("Dead End 999", &m->sm);

		break;

		case 1000:  sm_text("Dead End 1000", &m->sm);

		break;

		case 1001:  sm_text("Dead End 1001", &m->sm);

		break;
		
	}

		

		// Head Selection
		// FWD Direction  - Head B	m->ToolHeadID = 1
		if (m->ToolHeadID == 0)
		{
			Hardware[1].Outp[5] = 1;	// Select A Head Signal
			Hardware[1].Outp[0] = 1;	// Weld Angle	

			//TVL
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[1] = 0; // Select Calibration & Direction

			

			if(curCalib->GenSetup[SETUP_WF_WP_SWAP] == 0){ 

				//WF
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1] = 0; // Select Calibration

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx > 0)
					WIRE_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx];				
				else
					WIRE_Param_Set[0].pVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx != m->pCfg->HeadCfg.WF.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_B_Indx].InVal = 0.0;
				}

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx > 0)
					WIRE_Param_Set[0].pAuxVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx];	
				else
					WIRE_Param_Set[0].pAuxVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx != m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx].InVal = 0.0;
				}
				

				//WP
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[1] = 0; // Select Calibration

				if(m->pCfg->HeadCfg.WP.VECT_A_Indx > 0)
					WIRE_Param_Set[1].pVect = &VectorStateMach[m->pCfg->HeadCfg.WP.VECT_A_Indx];
				else
					WIRE_Param_Set[1].pVect = 0;

				if(m->pCfg->HeadCfg.WP.VECT_A_Indx != m->pCfg->HeadCfg.WP.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WP.VECT_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WP.VECT_B_Indx].InVal = 0.0;
				}

				
				
			}
			else{
				
				//WF
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1] = 1; // Select Calibration

				if(m->pCfg->HeadCfg.WF.VECT_B_Indx > 0)
					WIRE_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_B_Indx];
				else
					WIRE_Param_Set[0].pVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx != m->pCfg->HeadCfg.WF.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_A_Indx > 0 )
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].InVal = 0.0;
				}

				
				if(m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx > 0)
					WIRE_Param_Set[0].pAuxVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx];					
				else
					WIRE_Param_Set[0].pAuxVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx != m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx].InVal = 0.0;
				}
			

				//WP
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[1] = 1; // Select Calibration

				if(m->pCfg->HeadCfg.WP.VECT_B_Indx > 0)
					WIRE_Param_Set[1].pVect = &VectorStateMach[m->pCfg->HeadCfg.WP.VECT_B_Indx];
				else
					WIRE_Param_Set[1].pVect = 0;

				if(m->pCfg->HeadCfg.WP.VECT_A_Indx != m->pCfg->HeadCfg.WP.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WP.VECT_A_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WP.VECT_A_Indx].InVal = 0.0;
				}


			}		


			//OSC
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[1] = 0; // Select Calibration
			OSC_TML_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx];

			if (m->pCfg->HeadCfg.OSC.VECT_A_Indx != m->pCfg->HeadCfg.OSC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].Mode = vectSt_Mode_Idle;

			//  AVC
			if (m->pCfg->HeadCfg.AVC.VECT_A_Indx != m->pCfg->HeadCfg.AVC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_B_Indx].Mode = vectSt_Mode_Idle;

			
			switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

				case BPV_v2_MPGH:
				case BPV_v2_MPGV:	
				case BPV_v2_MT_BO_TOS:
				case BPV_v2_TT_BO_TOS:		
				
					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[0] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[1] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];	

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];
					
				break;

				default:

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[0] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[1] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];


				break;


			}			

			//AVC
			AVC_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_A_Indx];

			if (m->pCfg->HeadCfg.AVC.VECT_A_Indx != m->pCfg->HeadCfg.AVC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_B_Indx].Mode = vectSt_Mode_Idle;

			/*
			if(m->pCfg->HeadCfg.EvalateTVLservo_outputs){
				
				if(TML_MTR_INST[TML_AX_TVL].DigOut_stat[0]){

					if(VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD == 0){
						
						VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD = VECT_Set_OUT0;
						//_ToolHeadID 		= m->ToolHeadID;

					}

				}
			}
			*/
			////////////////////////
			if(m->pCfg->HeadCfg.SwitchHead.VECT_Indx > 0 ){

				if(m->pCfg->HeadCfg.SwitchHead.TML_Indx > 0 ){
				
					if(TML_MTR_INST[m->pCfg->HeadCfg.SwitchHead.TML_Indx -1].DigOut_stat[0]){
					//if(_ToolHeadID_Sw1 != m->ToolHeadID){	

						if(VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx-1].xCMD == 0){
							
							VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx-1].xCMD = VECT_Set_OUT0;
							//_ToolHeadID_Sw1 		= m->ToolHeadID;

						}

					}
				}
			}

			/*
			if(m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2 > 0 ){

				if(m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2 > 0 ){
				
					if(TML_MTR_INST[m->pCfg->HeadCfg.SwitchHead.TML_Indx_2 -1].DigOut_stat[0]){
					//if(_ToolHeadID_Sw2 != m->ToolHeadID){

						if(VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2-1].xCMD == 0){
							
							VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2-1].xCMD = VECT_Set_OUT0;
							//_ToolHeadID_Sw2 		= m->ToolHeadID;
							//_ToolHeadID 		= m->ToolHeadID;

						}

					}
				}
			}
			*/
			/////////////////////
			

		}

		// REV Direction  - Head A	m->ToolHeadID = 0
		else 
		{

			Hardware[1].Outp[5] = 0;	// Select B Head Signal
			Hardware[1].Outp[0] = 0;	// Weld Angle	

			//TVL
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[1] = 0x1 + 0x2; // Select Calibration & Direction

			
			if(curCalib->GenSetup[SETUP_WF_WP_SWAP] == 0){

				//WF
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1] = 1; // Select Calibration

				if(m->pCfg->HeadCfg.WF.VECT_B_Indx > 0)
					WIRE_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_B_Indx];				
				else
					WIRE_Param_Set[0].pVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx != m->pCfg->HeadCfg.WF.VECT_B_Indx){	
					
					if(m->pCfg->HeadCfg.WF.VECT_A_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].InVal = 0.0;
				}

				if(m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx > 0)
					WIRE_Param_Set[0].pAuxVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx];
				else
					WIRE_Param_Set[0].pAuxVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx != m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx].InVal = 0.0;

				}

				//WP
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[1] = 1; // Select Calibration

				if(m->pCfg->HeadCfg.WP.VECT_B_Indx > 0)
					WIRE_Param_Set[1].pVect = &VectorStateMach[m->pCfg->HeadCfg.WP.VECT_B_Indx];
				else
					WIRE_Param_Set[1].pVect = 0;


				if(m->pCfg->HeadCfg.WP.VECT_A_Indx != m->pCfg->HeadCfg.WP.VECT_B_Indx){	
					
					if(m->pCfg->HeadCfg.WP.VECT_A_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WP.VECT_A_Indx].InVal = 0.0;
				}

				
			}
			else{
				
				//WF
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1] = 0; // Select Calibration

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx > 0)
					WIRE_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx];				
				else
					WIRE_Param_Set[0].pVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_A_Indx != m->pCfg->HeadCfg.WF.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_B_Indx].InVal = 0.0;
				}

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx > 0)
					WIRE_Param_Set[0].pAuxVect = &VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx];
				else
					WIRE_Param_Set[0].pAuxVect = 0;

				if(m->pCfg->HeadCfg.WF.VECT_Aux_A_Indx != m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx){
					
					if(m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WF.VECT_Aux_B_Indx].InVal = 0.0;
				}

				//WP
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[1] = 0; // Select Calibration

				if(m->pCfg->HeadCfg.WP.VECT_A_Indx > 0)				
					WIRE_Param_Set[1].pVect = &VectorStateMach[m->pCfg->HeadCfg.WP.VECT_A_Indx];
				else
					WIRE_Param_Set[1].pVect = 0;	

				if(m->pCfg->HeadCfg.WP.VECT_A_Indx != m->pCfg->HeadCfg.WP.VECT_B_Indx){
					
					if(m->pCfg->HeadCfg.WP.VECT_B_Indx > 0)
						VectorStateMach[m->pCfg->HeadCfg.WP.VECT_B_Indx].InVal = 0.0;	
				}

			}

			//OSC
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[1] = 1; // Select Calibration
			OSC_TML_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx];

			if (m->pCfg->HeadCfg.OSC.VECT_A_Indx != m->pCfg->HeadCfg.OSC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].Mode = vectSt_Mode_Idle;

			// AVC
			if (m->pCfg->HeadCfg.AVC.VECT_A_Indx != m->pCfg->HeadCfg.AVC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_A_Indx].Mode = vectSt_Mode_Idle;


			switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

				case BPV_v2_MPGH:
				case BPV_v2_MPGV:	
				case BPV_v2_MT_BO_TOS:
				case BPV_v2_TT_BO_TOS:	

					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[0] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[1] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];	

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[2];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[3];


				break;


				default:

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[0] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[2];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[1] =
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[3];	


				break;

			}
				


			//AVC
			AVC_Param_Set[0].pVect = &VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_B_Indx];


			if (m->pCfg->HeadCfg.AVC.VECT_A_Indx != m->pCfg->HeadCfg.AVC.VECT_B_Indx)
				VectorStateMach[m->pCfg->HeadCfg.AVC.VECT_A_Indx].Mode = vectSt_Mode_Idle;

			/*
			if(m->pCfg->HeadCfg.EvalateTVLservo_outputs){
				
				if(!TML_MTR_INST[TML_AX_TVL].DigOut_stat[0]){

					if(VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD == 0){
						
						VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD = VECT_Rst_OUT0;
						//_ToolHeadID 		= m->ToolHeadID;

					}

				}
			}
			*/
			////////////////////////
			if(m->pCfg->HeadCfg.SwitchHead.VECT_Indx > 0 ){

				if(m->pCfg->HeadCfg.SwitchHead.TML_Indx > 0 ){
				
					if(!TML_MTR_INST[m->pCfg->HeadCfg.SwitchHead.TML_Indx -1].DigOut_stat[0]){
					//if(_ToolHeadID_Sw1 != m->ToolHeadID){
						

						if(VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx-1].xCMD == 0){
							
							VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx-1].xCMD = VECT_Rst_OUT0;
							//_ToolHeadID_Sw1 		= m->ToolHeadID;
							//_ToolHeadID 		= m->ToolHeadID;

						}

					}
				}
			}

			/*
			if(m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2 > 0 ){

				if(m->pCfg->HeadCfg.SwitchHead.TML_Indx_2 > 0 ){
				
					if(!TML_MTR_INST[m->pCfg->HeadCfg.SwitchHead.TML_Indx_2 -1].DigOut_stat[0]){
					//if(_ToolHeadID_Sw2 != m->ToolHeadID){

						if(VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2-1].xCMD == 0){
							
							VectorStateMach[m->pCfg->HeadCfg.SwitchHead.VECT_Indx_2-1].xCMD = VECT_Rst_OUT0;
							//_ToolHeadID_Sw2 		= m->ToolHeadID;
							//_ToolHeadID 		= m->ToolHeadID;

						}

					}
				}
			}
			*/
			/////////////////////
			

		}



		
		if (m->ToolHeadID != _ToolHeadID){
			_ToolHeadID 		= m->ToolHeadID;

			if(m->pCfg->HeadCfg.GEN.Torch_Mode == MODE_DUAL_OSCILATOR)
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsRESTART;	

			if(m->pCfg->HeadCfg.GEN.AVC_Mode == MODE_DUAL_AVC)
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsRESTART;	

		}

		
		// On page calibration No limits and Force ToolID assigments
		if (hmi->curHeadID == P_MODE_OSC_LIMIT_CALIB)
		{

			switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

				case BPV_v2_MPGH:
				case BPV_v2_MPGV:	
				case BPV_v2_MT_BO_TOS:
				case BPV_v2_TT_BO_TOS:	
				
					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];
				
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[2];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[3];

					

				break;

				default:

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];

					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[0] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[2];
					VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_B_Indx].OutVect.softLimits[1] =
					defCalib->Points[CALIB_OSC_SFT_LIMIT].val[3];

				break;

			}			

		}		

		///////////////////////////////////////////////////////////////
		FinalPos			= StartPos + (DINT)
		curCfg->Param[parTVL_RunDist].val[0];


		//////////
		intrf->_intr._GlblBtnCmd[0] = (intrf->GlblBtnCmd[0] & (0xFFFFFFFF - ANDR_BTN_HOME)) ;
		intrf->_intr._GlblBtnCmd[1] = intrf->GlblBtnCmd[1];


		// THIS MUST BE RUN //
		// This is for pipe compensation
		
		if(m->FrameDia > 0.0)
			curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] = m->FrameDia ;
		
		if(hmi->curHeadID != P_MODE_TVL_CALIB){


			if(WRST_TML_Param_Set[0].MappingActive > 0)
				TVL_ROT_Param_Set[0].TotalRescaleFactor = WRST_TML_Param_Set[0].TVLSpeedOverride;
			
			else if((curCfg->Param[parTVL_TrackDia].val[0]> 0.0) && (curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA]> 0.0))
				TVL_ROT_Param_Set[0].TotalRescaleFactor = curCfg->Param[parTVL_TrackDia].val[0] / curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA];

			else
				TVL_ROT_Param_Set[0].TotalRescaleFactor = 1.0;

		}
		else 
			TVL_ROT_Param_Set[0].TotalRescaleFactor = 1.0;

		sm_serv(&m->sm);
		TimerExecute(&ESTOP_Dly_Tmr);
		TimerExecute(&DoubleStop_TMR);
		LoggerSrv(&LogMgr);


		//AndroidHelper(m , 0);		
		SetBannerTxt(m, hmi, intrf);
		TimerExecute(&AndroidConnect_Tmr);	

		RestartStateMach();

		_datagramCountTotal[0] = datagramCountTotal[0];
		_datagramPerSec[0]	= datagramPerSec[0];

		if (btnProgress < btnProgressMax)
			btnProgress++;


		KK_Life++;

		
		/////////////////////////////////////////////////////	

		LastHeadDir = (USINT)Master[0].ToolHeadID;

		if(Ready2StoreLastPos){

			// Store last Position
			if (TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_A_Indx]._int.Init &&
				 (TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_A_Indx]._cfg.absFbk != 2))
				OLD_MTR_POS[OSC_LAST_POS] = AxisCh[m->pCfg->HeadCfg.OSC.TML_A_Indx].curPos;

			if (TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx]._int.Init &&
				 (TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absFbk != 2))
				OLD_MTR_POS[WRST_LAST_POS] = AxisCh[m->pCfg->HeadCfg.WRST.TML_A_Indx].curPos;	

			if (TML_MTR_INST[m->pCfg->HeadCfg.TVL.TML_Indx]._int.Init &&
				 (TML_MTR_INST[m->pCfg->HeadCfg.TVL.TML_Indx]._cfg.absFbk != 2))
				OLD_MTR_POS[TVL_LAST_POS] = AxisCh[m->pCfg->HeadCfg.TVL.TML_Indx].curPos;

			switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

				
				case CLAMSHELL_HEAD:										 	
				case BPV_F_HEAD:	
				//dodane 6Dec23	
				case BPV_v1_MT:		

					OLD_MTR_POS[INDX_A_LAST_POS] = DC_Mtr[m->pCfg->HeadCfg.INDX.DC_A_Indx].msc.Pos;
					OLD_MTR_POS[INDX_B_LAST_POS] = DC_Mtr[m->pCfg->HeadCfg.INDX.DC_B_Indx].msc.Pos; 

					//curCalib->GenSetup[SETUP_MAPPING_ON] = MAPPING_OFF;	
					//WRST_TML_Param_Set[0].MappingActive = 0;

				break;

				case BPV_v2:
				case BPV_v2_MPBO:
				case BPV_v2_MPGH:
				case BPV_v2_MPGV:	
				// dodane 8July24
				case BPV_v2_Video:					
				//dodane 6Dec23
				case BPV_v2_MT:				

					if (TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx]._int.Init &&
						(TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx]._cfg.absFbk != 2))
						OLD_MTR_POS[INDX_A_LAST_POS] = AxisCh[m->pCfg->HeadCfg.INDX.TML_A_Indx].curPos;


					if (TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx]._int.Init &&
						(TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx]._cfg.absFbk != 2))
						OLD_MTR_POS[INDX_B_LAST_POS] = AxisCh[m->pCfg->HeadCfg.INDX.TML_B_Indx].curPos;

				break;
			

				default:
					

				break;	

			}

		}


		//////////
		if(m->WireDisabled != _WireDisabled)
			LogTxt(&LogMgr, "Wire Disable Change %d" , (DINT)m->WireDisabled);

		_WireDisabled = m->WireDisabled;
		
	}



	/*********************************************************/
	/****	Process parameters assigments	************************/
	DINT MasterSequence(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn)
	{
	int 			i;
	char			status;
	DINT			ret;
	DINT			d;
	USINT			newFile;

	//Cfg_typ *cfg;
	CurCfg_typ *	curCfg;

	//CurCalib_typ *curCalib , *defCalib;
	//Scaled_Var_obj *var;
	//cfg = (Cfg_typ*) m->pCfg;
	curCfg				= (CurCfg_typ *)
	m->pCurCfg;

	//curCalib = (CurCalib_typ*) m->pCalib;
	//defCalib = (CurCalib_typ*) m->pDefaultCalib;
	//var = (Scaled_Var_obj*) m->pScaledVar;
	m->TestMode 		= intrf->TestMode;

	ret 				= 0;

	if((hmi->curHeadID == 0xFFFE)){ // Screen RUn or Alter Run
		
		if(intrf->GlblBtnCmd[0] &ANDR_BTN_UP){

			AVC_Param_Set[0].BoostCmd = 1;
			PS_Param_Set[0].BoostCmd = 1;
			TVL_ROT_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[1].BoostCmd = 1;
			
		}
		else if(intrf->GlblBtnCmd[0] &ANDR_BTN_DOWN){

			AVC_Param_Set[0].BoostCmd = 2;
			PS_Param_Set[0].BoostCmd = 2;
			TVL_ROT_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[1].BoostCmd = 2;

		}
		else{

			AVC_Param_Set[0].BoostCmd = 0;
			PS_Param_Set[0].BoostCmd = 0;
			TVL_ROT_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[1].BoostCmd = 0;


		}
	}
	else{

		AVC_Param_Set[0].BoostCmd = 0;
		PS_Param_Set[0].BoostCmd = 0;
		TVL_ROT_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[1].BoostCmd = 0;

	}
	
	/////////
	if (StopBtn)
	{
		AutoStartBtn		= 0;

		if (TimerIsTimeOut(&DoubleStop_TMR))
			TimerStart(&DoubleStop_TMR, 0.5);
		else 
			DoubleStopBtn_Latch = 1;
	}

	if (StopBtn)
		StopBtn_Latch = 1;

	

	if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
	{
		if(m->SubSeqStep == 1 )// Weld to Target
		{
			if(abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0]) < 10)
				m->SubSeqStep = 2;
		}
	}
	
	if ((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) ||
	(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_ReturnDly)){

		if (DoubleStopBtn_Latch)
			m->SubSeqStep = 0;
	}
	DoubleStopBtn_Latch = 0;

	if(curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 3) // Continues welding
		m->SubSeqStep = 0;
		

	if (StopBtn_Latch)
		Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // No Auto Stop and Not Rewrap

	//////////////////////////
	////////////////////////////
	if (m->SeqStep > 0)
		GlobalTimer += 0.0512; // Timer based on task class


	//////////////

	

		//	 MASTER SEQUENCE  //
	switch (m->SeqStep)
	{
		case 0:

			//////////////////////////////////////
			if(curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				File_RW._int.PathDoneCnt = 0;
			
			// Reset Disable when was in Test Mode
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;
			StopBtn_Latch = 0;

			// AVC Locked is OFF

			//if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;

			if(StartBtn && !Check_OSC_Amplt_and_Center())
				break;

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Halt Stop
				break;
			}

			status = 1;

			if (//(Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped))
				//(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped))
				status = 0;

			if (status)
			{

				if (!ErrorMgr.Error)
				{
					ret = 1;		// Raedy To Run

					if((intrf->GlblBtnCmd[0] &ANDR_BTN_HOME) && StartBtn){

						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; //only rewrap
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
						m->SeqStep = 210;
						
					}

					//if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd != XML_CMD_READ))
					//else if ((StartBtn || AutoStartBtn) && ((Weld_Report_Status < 2) || ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0))) //Weld_Report_Mode
					else if ((StartBtn || AutoStartBtn) && ((Weld_Report_Start_Enable) || ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0))) //Weld_Report_Mode
					{
						Weld_Report_Status = 1;
						
						AutoStartBtn = 0;

						if (m->SubSeqStep == 2)
						{ // Rewrap							

							if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
								TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);

							else 
								TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep = 150;

						}
						else 
						{
						
						GlobalTimer 		= 0;

						LogTxt(&LogMgr, "PreProcess start");

						if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
							TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);
						else 
							TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep++;

						}
					}

					/*
					else if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd == XML_CMD_READ)){


						strcpy(hmi->pMessage[0], " FILE OPER. \n ");
						strcpy(hmi->pMessage[1], File_RW.fileName);
						strcpy(hmi->pMessage[2], "Is Loading");
						strcpy(hmi->pMessage[3] ,"Wait for finish");
						
						Set_Message(hmi , 0.1);



					}
					*/
				}

			}
			else 
			{
				if (StopBtn)
				{
					LogTxt(&LogMgr, "Some Prcs aborted");
					m->SeqStep = 254;
				}

			}

		break;

		case 1:
		// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep = 254;
			}
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;
			
			else 
			{
				if (!m->TestMode)
				{
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
				}

				if (m->TestMode)
				{
				
					//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable |= 0x02 ;
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 1;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable |= 0x02;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable |= 0x02;

				}
				else 
				{

					if ((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run) &&
					(Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status == prcsSTATUS_Run))
					{
						//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD ;
						Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 0;
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
						Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;

					}
					else 
						break;

				}

				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsSTART;

				//if(!m->TestMode)
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsForceON;

				LogTxt(&LogMgr, "ARC start");

				m->SeqStep++;

			}

		break;

		case 2:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep			= 254;
			}

			///////////////////////////
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if ((Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_Run)
					|| (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_OK))
			{


				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTART;
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 0; // Start with ramp 
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsSTART;

				/*	TVL */
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;

#ifdef EXTENDED_LOGS			
				LogTxt(&LogMgr, "M:TVL%d Start: %.2f" , (UDINT) m->ToolHeadID , ABS_TVL_Pos);
#endif
				StartPos = 0;				
				
				CalculateRunOverlap();		


				if (!m->WireDisabled)
				{
					// if prcsStart then Param[0] = 0 is set in Wire process - jus in case
					//Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;
					//Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;
				}

				LogTxt(&LogMgr, "ARC established");

				TimerStart(&WireDisable_TMR, 3.0);

				m->SeqStep++;

			}

		break;

		case 3:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			WireDisableInable(WireDisable_TMR.Q);
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			ret = 3; //  Run	


			if (curCfg->Param_ENUM[parENUM_WeldProgramLinkMode].val[0] > 0.0)
			{

				if(curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				{

					if (strlen(curCfg->NextProgramName) > 0)
					{

						if (!strstr(curCfg->NextProgramName, "NONE"))
						{ // Not None

							// It should be some additional condition when to load new file
							if (actRot != _actRot){
								//TakeNextWeldProgram(hmi);
								//newFile 			= 1;

							}

						}

					}

				}

			}	
			
			_actRot = actRot;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].subStatus == 1)
			{ // End of travel reached
				//LogTxt(&LogMgr, "Path Done");
				//LogTxt(&LogMgr, "Path Done: %.2f" , ABS_TVL_Pos);
				Weld_Report_Status = 2;
				m->SeqStep = 200;
			}

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Process stop");
				//Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTOP;
				m->SeqStep = 200;

			}
			else if (ErrorMgr.Error || m->ESTOP)
			{
				LogTxt(&LogMgr, "Process Error");
				m->SeqStep = 100;
			}

		break;

		case 100:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;

			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsHalt;
			switch (ErrorMgr.Error)
			{
				case ERROR_FATAL:
					m->SeqStep++;
				break;

				case ERROR_MOTOR:
					m->SeqStep++;
				break;

				case ERROR_MOTOR_INIT:
					m->SeqStep++;
				break;

				case ERROR_TIG_PROC:
					m->SeqStep++;
				break;

				default:
					m->SeqStep++;
				break;
			}

		break;

		case 101:
			ret = 4; // Preparig To Stop in Halt Mode	

			if (StopBtn)
			{
				for (i = 0; i < PROCESS_NUM; i++)
				{
					if (i != PROC_COOL)
						Process[i].xCMD = prcsHalt;
				}
				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{
				LogTxt(&LogMgr, "Prc Final stopped");
				m->SeqStep = 254;
			}

		break;

		case 150:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				break;
			}

			if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			/*	TVL */
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
			StartPos = 0;
			
			CalculateRunOverlap();
						
			m->SeqStep = 202;
			
		break;

		
		case 200:
						
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			for (i = 0; i < PROCESS_NUM; i++)
			{
				if ((i != PROC_COOL)) //&&( i != m->pCfg->HeadCfg.TVL.PROC_Indx))
					Process[i].xCMD = prcsSTOP;

			}	

			// Dodane - Nie ma Xcmd Stop w TRVL_BPV_POS
			if((TVL_ROT_Param_Set[0].Mode == 2) // with only position
				|| (TVL_ROT_Param_Set[0].Mode == 3)) // with interrupt
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = 0;
							
///////////////////////////////////////////////////////////////////////////
/// Nowosc Index wczesniej

			if((curCalib->GenSetup[SETUP_EXTRAS]&0x4)){
				
				switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

						
						default:

							if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


								if(INDEX_Param_Set[0].pStepDist){

									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPneg; // Quick Step with delay

									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

									KK_Catched = 133;

									//m->SeqStep++;
								}
								//else
									//m->SeqStep = 203;					
						

							}
							
							else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){
						
										
						
								if(INDEX_Param_Set[0].pStepDist){
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
			
									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
									
									m->SeqStep++;
			
								}
								//else
									//m->SeqStep = 203;


							}
							//else
								//m->SeqStep = 203;
								

						break;

					}

			}

/////////////////////////////////////////////////////////////////////////
#ifdef EXTENDED_LOGS

			LogTxt(&LogMgr, "Path %d Done: %.2f" , (UDINT)File_RW._int.PathDoneCnt, ABS_TVL_Pos);
#else
			LogTxt(&LogMgr, "Path %d Done" , (UDINT)File_RW._int.PathDoneCnt);

#endif

			//TimerStart(&WF_Catch_TMR, curCfg->Param[parWF_RampDwn].val[0]);		
			m->SeqStep++;
				
		break;

			
		case 201:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "Quick stop %.2f" , ABS_TVL_Pos);
#else 
				LogTxt(&LogMgr, "Quick stop");
#endif
				m->SeqStep = 254;			// Quick Stop
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped){ // stopped by end of travel				
				status = 0;
			}	
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			//if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				//status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{

#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "M:All%d Stop: %.2f" ,(UDINT) m->ToolHeadID, ABS_TVL_Pos);				

#endif				

				if(!(curCalib->GenSetup[SETUP_EXTRAS]&0x4)){
					
					switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

						/*
						case BPV_v2_MPGH:
						case BPV_v2_MPGV:

						
						case BPV_v2_MPBO:

							if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


								if(INDEX_Param_Set[0].pStepDist){

									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

									KK_Catched = 133;

									m->SeqStep++;
								}
								else
									m->SeqStep = 203;					
						

							}
							else
								m->SeqStep = 203;



						break;
						*/

						default:


							if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


								if(INDEX_Param_Set[0].pStepDist){

									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

									KK_Catched = 133;

									m->SeqStep++;
								}
								else
									m->SeqStep = 203;					
						

							}
							
							else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){				
										
						
								if(INDEX_Param_Set[0].pStepDist){
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
			
									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
									
									m->SeqStep++;
			
								}
								else
									m->SeqStep = 203;


							}
							else
								m->SeqStep = 203;
								

						break;

					}

					}
				
			}

		break;


		case 202:
				
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if(Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Status == prcsSTATUS_Stopped
				&& Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped){

				TimerStart(&m->sm.Tmr1, 0.1);
				m->SeqStep++;

			}
			
		break;

		case 203:
			// Always Coolant On
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped){

				LogTxt(&LogMgr, "M: Cyc. Time %.2f" , GlobalTimer);
				m->SeqStep++;
			}	

		break;

		

		case 204:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				//m->SeqStep = 205;			// Normal Stop
				break;
			}

			// Check if Index move done
			if (1)
			{

				
				if((abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3]) < 10)
				//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped)
					&& (m->SubSeqStep == 2)) // IMPORTANT - head swap is only when was rewrap before
				{

					m->SubSeqStep = 1;

					if (curCfg->Param_ENUM[parENUM_PathMode].val[0] == 0.0)
						m->ToolHeadID = !m->ToolHeadID;

					if (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0)
					{ // Relative Move
						if (TVL_ROT_Param_Set[0].pEncoder)
							OLD_MTR_POS[TVL_ENC_HOME] = *TVL_ROT_Param_Set[0].pEncoder;
					}
					AutoStartEnable = 1;
					m->SeqStep++;

				}
				
				else 
				{
					m->SeqStep = 254;
				}			


			}

		break;

		case 205:			
			
			File_RW._int.PathDoneCnt++;
			newFile = 0;

			if (curCfg->Param_ENUM[parENUM_WeldProgramLinkMode].val[0] > 0.0)
			{

				if (curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				{

					if (strlen(curCfg->NextProgramName) > 0)
					{

						if (!strstr(curCfg->NextProgramName, "NONE"))
						{ // Not None
							TakeNextWeldProgram(hmi);
							newFile 			= 1;

						}

					}

				}

				else 
				{

					if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
					{

						if (strlen(curCfg->NextProgramName) > 0)
						{

							if (!strstr(curCfg->NextProgramName, "NONE"))
							{ // Not None
								TakeNextWeldProgram(hmi);
								newFile 			= 1;
							}							
								
						}
						
					}

				}

			}

			if (newFile)
				m->SeqStep = 206;
			else 
				m->SeqStep = 207;

		break;

		case 206:
			if (File_RW.CMD == 0)
			{

				if (File_RW.Error)
					m->SeqStep = 206;

				else 
				{
					AutoStartEnable = 1;
					m->SeqStep = 207;

				}
			}

		break;

		case 207:
			
			AutoStartEnable = 1;

			if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
			{
				File_RW._int.PathDoneCnt = (UINT)
				curCfg->Param[parGEN_WeldProgramRepeat].val[0];

				AutoStartEnable 	= 0;
			}

			m->SeqStep++;
			
		break;

		case 208:
			///////////////////////////////////
			//Auto Start		
			if (curCalib->GenSetup[SETUP_AUTO_START] && AutoStartEnable)
			{
				TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]);
				m->SeqStep = 220;

			}
			else 
				m->SeqStep = 254;

		break;

		case 210:

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				
			}
			else if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) // stopped by end of travel	
				m->SeqStep = 0;	
				
		break;

		case 220:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				break;
			}
			else if(!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			GlobalTimer = 0;

			//WF_TVL_Diff = 0;	
			LogTxt(&LogMgr, "Auto Repeat");
			AutoStartBtn = 1;
			m->SeqStep = 0;
			
		break;

		case 254:
			
			for (i = 0; i < PROCESS_NUM; i++)
			{ // beside PROC_COOL
				if (i != PROC_COOL)
					Process[i].xCMD = prcsHalt;
			}

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // STop without ramp 
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP; // return to center position
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			m->SeqStep++;
			
		break;

		default:


			if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				for (i = 0; i < PROCESS_NUM; i++)
				{ // beside PROC_COOL	
					if (i!= PROC_COOL && i!= PROC_OSC)
						Process[i].xCMD = prcsHalt;
				}

				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
				m->SeqStep			= 0;

			}

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped)
				m->SeqStep = 0;

			if (m->SeqStep == 0)
			{
				// Load new AVC param from new file
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter =	curCfg->Param[parAVC_FbkFilter].val[0];
			}

			

		break;
			
		}

		//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped))
		//StopBtn_Latch = 0;
		////////////////////////////
		TimerExecute(&WireDisable_TMR);
		TimerExecute(&WF_Catch_TMR);

		return ret;
	}


		/*********************************************************/
			/****	Process parameters assigments	************************/

	DINT MasterSequence_Contin_OneWay_dobra_alebezWeldIndex(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn)
	{
	int 			i;
	char			status;
	DINT			ret;
	DINT			d;
	USINT			newFile;

	//Cfg_typ *cfg;
	CurCfg_typ *	curCfg;

	//CurCalib_typ *curCalib , *defCalib;
	//Scaled_Var_obj *var;
	//cfg = (Cfg_typ*) m->pCfg;
	curCfg				= (CurCfg_typ *)
	m->pCurCfg;

	//curCalib = (CurCalib_typ*) m->pCalib;
	//defCalib = (CurCalib_typ*) m->pDefaultCalib;
	//var = (Scaled_Var_obj*) m->pScaledVar;
	m->TestMode 		= intrf->TestMode;

	ret 				= 0;

	if((hmi->curHeadID == 0xFFFE)){ // Screen RUn or Alter Run
		
		if(intrf->GlblBtnCmd[0] &ANDR_BTN_UP){

			AVC_Param_Set[0].BoostCmd = 1;
			PS_Param_Set[0].BoostCmd = 1;
			TVL_ROT_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[1].BoostCmd = 1;
			
		}
		else if(intrf->GlblBtnCmd[0] &ANDR_BTN_DOWN){

			AVC_Param_Set[0].BoostCmd = 2;
			PS_Param_Set[0].BoostCmd = 2;
			TVL_ROT_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[1].BoostCmd = 2;

		}
		else{

			AVC_Param_Set[0].BoostCmd = 0;
			PS_Param_Set[0].BoostCmd = 0;
			TVL_ROT_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[1].BoostCmd = 0;


		}
	}
	else{

		AVC_Param_Set[0].BoostCmd = 0;
		PS_Param_Set[0].BoostCmd = 0;
		TVL_ROT_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[1].BoostCmd = 0;

	}
	
	/////////
	/////////
	//(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP)
	/*
	if (StopBtn)
	{
		AutoStartBtn		= 0;

		if (TimerIsTimeOut(&DoubleStop_TMR))
			TimerStart(&DoubleStop_TMR, 0.5);
		else 
			DoubleStopBtn_Latch = 1;
	}

	if (StopBtn)
		StopBtn_Latch = 1;

	*/
	

	if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
	{
		if(m->SubSeqStep == 1 )// Weld to Target
		{
			if(abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0]) < 10)
				m->SubSeqStep = 2;
		}
	}
	
	if ((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) ||
	(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_ReturnDly)){

		if(DoubleStopBtn_Latch){
			if(m->SubSeqStep > 0){
				AutoStartBtn = 0;
				m->SubSeqStep = 0;
			}	
			else
				m->SubSeqStep = 1;

		}
	}
	DoubleStopBtn_Latch = 0;

	if(curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 3) // Continues welding
		m->SubSeqStep = 0;
		

	if (StopBtn_Latch)
		Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // No Auto Stop and Not Rewrap

	//////////////////////////
	////////////////////////////
	if (m->SeqStep > 0)
		GlobalTimer += 0.0512; // Timer based on task class


	//////////////

	

		//	 MASTER SEQUENCE  //
	switch (m->SeqStep)
	{
		case 0:

			Cont_Stop_By_End = 0;
			//////////////////////////////////////
			if(curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				File_RW._int.PathDoneCnt = 0;
			
			// Reset Disable when was in Test Mode
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;
			StopBtn_Latch = 0;

			// AVC Locked is OFF

			//if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;

			if(StartBtn && !Check_OSC_Amplt_and_Center())
				break;

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Halt Stop
				break;
			}

			status = 1;

			if (//(Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped))
				status = 0;

			if (status)
			{

				if (!ErrorMgr.Error)
				{
					ret = 1;		// Raedy To Run

					if((intrf->GlblBtnCmd[0] &ANDR_BTN_HOME) && StartBtn){

						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; //only rewrap
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
						m->SeqStep = 210;
						
					}

					//if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd != XML_CMD_READ))
					//if (StartBtn || AutoStartBtn)
					else if ((StartBtn || AutoStartBtn) && ((Weld_Report_Status < 2) || ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0)))
					{
						Weld_Report_Status = 1;
						AutoStartBtn = 0;

						if (m->SubSeqStep == 2)
						{ // Rewrap							

							if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
								TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);

							else 
								TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep = 150;

						}
						else 
						{
						
						GlobalTimer 		= 0;

						LogTxt(&LogMgr, "PreProcess start");

						if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
							TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);
						else 
							TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep++;

						}
					}

					/*
					else if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd == XML_CMD_READ)){


						strcpy(hmi->pMessage[0], " FILE OPER. \n ");
						strcpy(hmi->pMessage[1], File_RW.fileName);
						strcpy(hmi->pMessage[2], "Is Loading");
						strcpy(hmi->pMessage[3] ,"Wait for finish");
						
						Set_Message(hmi , 0.1);



					}
					*/
				}

			}
			else 
			{
				if (StopBtn)
				{
					LogTxt(&LogMgr, "Some Prcs aborted");
					m->SeqStep = 254;
				}

			}

		break;

		case 1:
		// Always Coolant On

			Cont_Stop_By_End = 0;
		
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep = 254;
			}
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;
			
			else 
			{
				if (!m->TestMode)
				{
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
				}

				if (m->TestMode)
				{
				
					//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable |= 0x02 ;
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 1;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable |= 0x02;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable |= 0x02;

				}
				else 
				{

					if ((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run) &&
					(Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status == prcsSTATUS_Run))
					{
						//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD ;
						Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 0;
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
						Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;

					}
					else 
						break;

				}

				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsSTART;

				//if(!m->TestMode)
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsForceON;

				LogTxt(&LogMgr, "ARC start");

				m->SeqStep++;

			}

		break;

		case 2:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep			= 254;
			}

			///////////////////////////
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if ((Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_Run)
					|| (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_OK))
			{


				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTART;
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 0; // Start with ramp 
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsSTART;

				/*	TVL */
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;

#ifdef EXTENDED_LOGS			
				LogTxt(&LogMgr, "M:TVL%d Start: %.2f" , (UDINT) m->ToolHeadID , ABS_TVL_Pos);
#endif
				StartPos = 0;				
				
				CalculateRunOverlap_Cont_OneWay();		


				if (!m->WireDisabled)
				{
					// if prcsStart then Param[0] = 0 is set in Wire process - jus in case
					//Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;
					//Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;
				}

				LogTxt(&LogMgr, "ARC established");

				TimerStart(&WireDisable_TMR, 3.0);

				m->SeqStep++;

			}

		break;

		case 3:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			WireDisableInable(WireDisable_TMR.Q);
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			ret = 3; //  Run	

			//if (actRot != _actRot)
				//TakeNextWeldProgram(hmi);

			_actRot = actRot;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].subStatus == 1)
			{ // End of travel reached
				//LogTxt(&LogMgr, "Path Done");
				//LogTxt(&LogMgr, "Path Done: %.2f" , ABS_TVL_Pos);

				Weld_Report_Status = 2;

				m->SeqStep = 200;

							
			}

			if (StopBtn)
			{
				/*
				LogTxt(&LogMgr, "Process stop");
				//Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTOP;
				m->SeqStep = 200;
				*/
				if(Cont_Stop_By_End){
					LogTxt(&LogMgr, "Process stop by Operator");
					Cont_Stop_By_End = 0;
					m->SeqStep = 200;
				}
				else
					Cont_Stop_By_End = 1;

			}
			else if (ErrorMgr.Error || m->ESTOP)
			{
				LogTxt(&LogMgr, "Process Error");
				m->SeqStep = 100;
			}

		break;

		case 100:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;

			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsHalt;
			switch (ErrorMgr.Error)
			{
				case ERROR_FATAL:
					m->SeqStep++;
				break;

				case ERROR_MOTOR:
					m->SeqStep++;
				break;

				case ERROR_MOTOR_INIT:
					m->SeqStep++;
				break;

				case ERROR_TIG_PROC:
					m->SeqStep++;
				break;

				default:
					m->SeqStep++;
				break;
			}

		break;

		case 101:
			ret = 4; // Preparig To Stop in Halt Mode	

			if (StopBtn)
			{
				for (i = 0; i < PROCESS_NUM; i++)
				{
					if (i != PROC_COOL)
						Process[i].xCMD = prcsHalt;
				}
				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{
				LogTxt(&LogMgr, "Prc Final stopped");
				m->SeqStep = 254;
			}

		break;

		case 150:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				break;
			}

			if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			/*	TVL */
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
			StartPos = 0;
			
			CalculateRunOverlap_Cont_OneWay();
						
			m->SeqStep = 202;
			
		break;

		
		case 200:
						
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			for (i = 0; i < PROCESS_NUM; i++)
			{
				if ((i != PROC_COOL)) //&&( i != m->pCfg->HeadCfg.TVL.PROC_Indx))
					Process[i].xCMD = prcsSTOP;

			}			
#ifdef EXTENDED_LOGS

			LogTxt(&LogMgr, "Path %d Done: %.2f" , (UDINT)File_RW._int.PathDoneCnt, ABS_TVL_Pos);
#else
			LogTxt(&LogMgr, "Path %d Done" , (UDINT)File_RW._int.PathDoneCnt);

#endif

			//TimerStart(&WF_Catch_TMR, curCfg->Param[parWF_RampDwn].val[0]);		
			m->SeqStep++;
				
		break;

			
		case 201:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "Quick stop %.2f" , ABS_TVL_Pos);
#else 
				LogTxt(&LogMgr, "Quick stop");
#endif
				m->SeqStep = 254;			// Quick Stop
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped){ // stopped by end of travel				
				status = 0;
			}	
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			//if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				//status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{

#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "M:All%d Stop: %.2f" ,(UDINT) m->ToolHeadID, ABS_TVL_Pos);				

#endif				
				switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

					/*
					case BPV_v2_MPGH:
					case BPV_v2_MPGV:

					case BPV_v2_MPBO:

						if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


							if(INDEX_Param_Set[0].pStepDist){

								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
								else
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

								KK_Catched = 133;

								m->SeqStep++;
							}
							else
								m->SeqStep = 203;					
					

						}
						else
							m->SeqStep = 203;



					break;
					*/

					default:



						if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


							if(INDEX_Param_Set[0].pStepDist){

								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
								else
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

								KK_Catched = 133;

								m->SeqStep++;
							}
							else
								m->SeqStep = 203;					
					

						}
						
						else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){
					
									
					
							if(INDEX_Param_Set[0].pStepDist){
								Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
		
								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
								else
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
								
								m->SeqStep++;
		
							}
							else
								m->SeqStep = 203;


						}
						else
							m->SeqStep = 203;
							

					break;

				}
				
			}

		break;


		case 202:
				
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if(Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Status == prcsSTATUS_Stopped
				&& Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped){

				TimerStart(&m->sm.Tmr1, 0.1);
				m->SeqStep++;

			}
			
		break;

		case 203:
			// Always Coolant On
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped){

				LogTxt(&LogMgr, "M: Cyc. Time %.2f" , GlobalTimer);
				m->SeqStep++;
			}	

		break;

		

		case 204:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				//m->SeqStep = 205;			// Normal Stop
				break;
			}

			// Check if Index move done
			if (1)
			{

				
				if((abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3]) < 10)
				//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped)
					&& (m->SubSeqStep == 2)) // IMPORTANT - head swap is only when was rewrap before
				{

					m->SubSeqStep = 1;

					//if (curCfg->Param_ENUM[parENUM_PathMode].val[0] == 0.0)
						//m->ToolHeadID = !m->ToolHeadID;

					if (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0)
					{ // Relative Move
						if (TVL_ROT_Param_Set[0].pEncoder)
							OLD_MTR_POS[TVL_ENC_HOME] = *TVL_ROT_Param_Set[0].pEncoder;
					}
					AutoStartEnable = 1;
					m->SeqStep++;

				}
				
				else 
				{
					m->SeqStep = 254;
				}			


			}

		break;

		case 205:			
			
			File_RW._int.PathDoneCnt++;
			newFile = 0;
			/*
			if (curCfg->Param_ENUM[parENUM_WeldProgramLinkMode].val[0] > 0.0)
			{

				if (curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				{

					if (strlen(curCfg->NextProgramName) > 0)
					{

						if (!strstr(curCfg->NextProgramName, "NONE"))
						{ // Not None
							TakeNextWeldProgram(hmi);
							newFile 			= 1;

						}

					}

				}

				else 
				{

					if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
					{

						if (strlen(curCfg->NextProgramName) > 0)
						{

							if (!strstr(curCfg->NextProgramName, "NONE"))
							{ // Not None
								TakeNextWeldProgram(hmi);
								newFile 			= 1;
							}							
								
						}
						
					}

				}

			}
			*/

			if (newFile)
				m->SeqStep = 206;
			else 
				m->SeqStep = 207;

		break;

		case 206:
			if (File_RW.CMD == 0)
			{

				if (File_RW.Error)
					m->SeqStep = 206;

				else 
				{
					AutoStartEnable = 1;
					m->SeqStep = 207;

				}
			}

		break;

		case 207:
			
			AutoStartEnable = 1;

			/*
			if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
			{
				File_RW._int.PathDoneCnt = (UINT)
				curCfg->Param[parGEN_WeldProgramRepeat].val[0];

				AutoStartEnable 	= 0;
			}
			*/

			m->SeqStep++;
			
		break;

		case 208:
			///////////////////////////////////
			//Auto Start		
			if (curCalib->GenSetup[SETUP_AUTO_START] && AutoStartEnable)
			{
				TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]);
				m->SeqStep = 220;

			}
			else 
				m->SeqStep = 254;

		break;

		case 210:

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				
			}
			else if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) // stopped by end of travel	
				m->SeqStep = 0;	
				
		break;

		case 220:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				break;
			}
			else if(!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			GlobalTimer = 0;

			//WF_TVL_Diff = 0;	
			LogTxt(&LogMgr, "Auto Repeat");

			if(Cont_Stop_By_End)
				AutoStartBtn = 0;
			else	
				AutoStartBtn = 1;
			m->SeqStep = 0;
			
		break;

		case 254:
			
			for (i = 0; i < PROCESS_NUM; i++)
			{ // beside PROC_COOL
				if (i != PROC_COOL)
					Process[i].xCMD = prcsHalt;
			}

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // STop without ramp 
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP; // return to center position
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			m->SeqStep++;
			
		break;

		default:


			if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				for (i = 0; i < PROCESS_NUM; i++)
				{ // beside PROC_COOL	
					if (i!= PROC_COOL && i!= PROC_OSC)
						Process[i].xCMD = prcsHalt;
				}

				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
				m->SeqStep			= 0;

			}

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped)
				m->SeqStep = 0;

			if (m->SeqStep == 0)
			{
				// Load new AVC param from new file
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter =	curCfg->Param[parAVC_FbkFilter].val[0];
			}

			

		break;
			
		}

		//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped))
		//StopBtn_Latch = 0;
		////////////////////////////
		TimerExecute(&WireDisable_TMR);
		TimerExecute(&WF_Catch_TMR);

		return ret;
	}

	DINT MasterSequence_Contin_OneWay(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn)
	{
	int 			i;
	char			status;
	DINT			ret;
	DINT			d;
	USINT			newFile;

	//Cfg_typ *cfg;
	CurCfg_typ *	curCfg;

	//CurCalib_typ *curCalib , *defCalib;
	//Scaled_Var_obj *var;
	//cfg = (Cfg_typ*) m->pCfg;
	curCfg				= (CurCfg_typ *)
	m->pCurCfg;

	//curCalib = (CurCalib_typ*) m->pCalib;
	//defCalib = (CurCalib_typ*) m->pDefaultCalib;
	//var = (Scaled_Var_obj*) m->pScaledVar;
	m->TestMode 		= intrf->TestMode;

	ret 				= 0;

	if((hmi->curHeadID == 0xFFFE)){ // Screen RUn or Alter Run
		
		if(intrf->GlblBtnCmd[0] &ANDR_BTN_UP){

			AVC_Param_Set[0].BoostCmd = 1;
			PS_Param_Set[0].BoostCmd = 1;
			TVL_ROT_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[1].BoostCmd = 1;
			
		}
		else if(intrf->GlblBtnCmd[0] &ANDR_BTN_DOWN){

			AVC_Param_Set[0].BoostCmd = 2;
			PS_Param_Set[0].BoostCmd = 2;
			TVL_ROT_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[1].BoostCmd = 2;

		}
		else{

			AVC_Param_Set[0].BoostCmd = 0;
			PS_Param_Set[0].BoostCmd = 0;
			TVL_ROT_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[1].BoostCmd = 0;


		}
	}
	else{

		AVC_Param_Set[0].BoostCmd = 0;
		PS_Param_Set[0].BoostCmd = 0;
		TVL_ROT_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[1].BoostCmd = 0;

	}
	
	/////////
	/////////
	//(!intrf->_intr._GlblBtnCmd[0] && (intrf->GlblBtnCmd[0] &ANDR_BTN_STOP)
	/*
	if (StopBtn)
	{
		AutoStartBtn		= 0;

		if (TimerIsTimeOut(&DoubleStop_TMR))
			TimerStart(&DoubleStop_TMR, 0.5);
		else 
			DoubleStopBtn_Latch = 1;
	}

	if (StopBtn)
		StopBtn_Latch = 1;

	*/
	

	if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
	{
		if(m->SubSeqStep == 1 )// Weld to Target
		{
			//if(abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0]) < 8){
			if(abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0]) < 16){	// was changed July 23 2024
				m->SubSeqStep = 2;
				Weld_Report_Status = 2; // Should be reported
				LogTxt(&LogMgr, "Stopped < 1-4");
			}
			
			//if(abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0]) < 10)
				//m->SubSeqStep = 2;
		}
	}
	
	if ((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) ||
	(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_ReturnDly)){

		if(DoubleStopBtn_Latch){
			if(m->SubSeqStep > 0){
				AutoStartBtn = 0;
				m->SubSeqStep = 0;
			}	
			else
				m->SubSeqStep = 1;

		}
	}
	DoubleStopBtn_Latch = 0;

	if(curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 3) // Continues welding
		m->SubSeqStep = 0;
		

	if (StopBtn_Latch)
		Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // No Auto Stop and Not Rewrap

	//////////////////////////
	////////////////////////////
	if (m->SeqStep > 0)
		GlobalTimer += 0.0512; // Timer based on task class


	//////////////

	

		//	 MASTER SEQUENCE  //
	switch (m->SeqStep)
	{
		case 0:

			Cont_Stop_By_End = 0;
			//////////////////////////////////////
			if(curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				File_RW._int.PathDoneCnt = 0;
			
			// Reset Disable when was in Test Mode
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;
			StopBtn_Latch = 0;

			// AVC Locked is OFF

			//if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;

			if(StartBtn && !Check_OSC_Amplt_and_Center())
				break;

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Halt Stop
				break;
			}

			status = 1;

			if (//(Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped))
				status = 0;

			if (status)
			{

				if (!ErrorMgr.Error)
				{
					ret = 1;		// Raedy To Run

					if((intrf->GlblBtnCmd[0] &ANDR_BTN_HOME) && StartBtn){

						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; //only rewrap
						Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
						m->SeqStep = 210;
						
					}

					//if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd != XML_CMD_READ))
					//if (StartBtn || AutoStartBtn)
					//else if ((StartBtn || AutoStartBtn) && ((Weld_Report_Status < 2) || ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0)))
					else if ((StartBtn || AutoStartBtn) && ((Weld_Report_Start_Enable) || ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0)))
					{
						Weld_Report_Status = 1;
						AutoStartBtn = 0;

						if (m->SubSeqStep == 2)
						{ // Rewrap							

							if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
								TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);

							else 
								TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep = 150;

						}
						else 
						{
						
						GlobalTimer 		= 0;

						LogTxt(&LogMgr, "PreProcess start");

						if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
							TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);
						else 
							TimerStart(&m->sm.Tmr1, 0.1);

							m->SeqStep++;

						}
					}

					/*
					else if ((StartBtn || AutoStartBtn) && (XMLHelper.xCmd == XML_CMD_READ)){


						strcpy(hmi->pMessage[0], " FILE OPER. \n ");
						strcpy(hmi->pMessage[1], File_RW.fileName);
						strcpy(hmi->pMessage[2], "Is Loading");
						strcpy(hmi->pMessage[3] ,"Wait for finish");
						
						Set_Message(hmi , 0.1);



					}
					*/
				}

			}
			else 
			{
				if (StopBtn)
				{
					LogTxt(&LogMgr, "Some Prcs aborted");
					m->SeqStep = 254;
				}

			}

		break;

		case 1:
		// Always Coolant On

			Cont_Stop_By_End = 0;
		
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep = 254;
			}
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;
			
			else 
			{
				if (!m->TestMode)
				{
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
				}

				if (m->TestMode)
				{
				
					//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable |= 0x02 ;
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 1;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable |= 0x02;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable |= 0x02;

				}
				else 
				{

					if ((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run) &&
					(Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status == prcsSTATUS_Run))
					{
						//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD ;
						Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 0;
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
						Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;

					}
					else 
						break;

				}

				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsSTART;

				//if(!m->TestMode)
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsForceON;

				LogTxt(&LogMgr, "ARC start");

				m->SeqStep++;

			}

		break;

		case 2:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep			= 254;
			}

			///////////////////////////
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if ((Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_Run)
					|| (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_OK))
			{


				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTART;
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 0; // Start with ramp 
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsSTART;

				/*	TVL */
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;

#ifdef EXTENDED_LOGS			
				LogTxt(&LogMgr, "M:TVL%d Start: %.2f" , (UDINT) m->ToolHeadID , ABS_TVL_Pos);
#endif
				StartPos = 0;				
				
				CalculateRunOverlap_Cont_OneWay();		


				if (!m->WireDisabled)
				{
					// if prcsStart then Param[0] = 0 is set in Wire process - jus in case
					//Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;
					//Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 0; // start with full ramp
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;
				}

				LogTxt(&LogMgr, "ARC established");

				TimerStart(&WireDisable_TMR, 3.0);

				m->SeqStep++;

			}

		break;

		case 3:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			WireDisableInable(WireDisable_TMR.Q);
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			ret = 3; //  Run	

			//if (actRot != _actRot)
				//TakeNextWeldProgram(hmi);

			_actRot = actRot;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].subStatus == 1)
			{ // End of travel reached
				//LogTxt(&LogMgr, "Path Done");
				//LogTxt(&LogMgr, "Path Done: %.2f" , ABS_TVL_Pos);

				Weld_Report_Status = 2;			

				m->SeqStep = 200;

							
			}

			if (StopBtn)		{

				
				/*
				LogTxt(&LogMgr, "Process stop");
				//Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTOP;
				m->SeqStep = 200;
				*/
				if(Cont_Stop_By_End){
					LogTxt(&LogMgr, "Process stop by Operator");
					Cont_Stop_By_End = 0;
					m->SeqStep = 200;
				}
				else
					Cont_Stop_By_End = 1;

			}
			else if (ErrorMgr.Error || m->ESTOP)
			{
				LogTxt(&LogMgr, "Process Error");
				m->SeqStep = 100;
			}

		break;

		case 100:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;

			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsHalt;
			switch (ErrorMgr.Error)
			{
				case ERROR_FATAL:
					m->SeqStep++;
				break;

				case ERROR_MOTOR:
					m->SeqStep++;
				break;

				case ERROR_MOTOR_INIT:
					m->SeqStep++;
				break;

				case ERROR_TIG_PROC:
					m->SeqStep++;
				break;

				default:
					m->SeqStep++;
				break;
			}

		break;

		case 101:
			ret = 4; // Preparig To Stop in Halt Mode	

			if (StopBtn)
			{
				for (i = 0; i < PROCESS_NUM; i++)
				{
					if (i != PROC_COOL)
						Process[i].xCMD = prcsHalt;
				}
				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{
				LogTxt(&LogMgr, "Prc Final stopped");
				m->SeqStep = 254;
			}

		break;

		case 150:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				break;
			}

			if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			/*	TVL */
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;
			StartPos = 0;
			
			CalculateRunOverlap_Cont_OneWay();
						
			m->SeqStep = 202;
			
		break;

		
		case 200:
						
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			for (i = 0; i < PROCESS_NUM; i++)
			{
				if((i != PROC_COOL))
					//&&( i != m->pCfg->HeadCfg.TVL.PROC_Indx))
					Process[i].xCMD = prcsSTOP;

			}		

			// Dodane - Nie ma Xcmd Stop w TRVL_BPV_POS
			if((TVL_ROT_Param_Set[0].Mode == 2) // with only position
				|| (TVL_ROT_Param_Set[0].Mode == 3)) // with interrupt
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = 0;
				
///////////////////////////////////////////////////////////////////////////
/// Nowosc Index wczesniej

			if((curCalib->GenSetup[SETUP_EXTRAS]&0x4)){
				
				switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

						
						default:

							if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


								if(INDEX_Param_Set[0].pStepDist){

									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPneg; // Quick Step with delay

									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

									KK_Catched = 133;

									//m->SeqStep++;
								}
								//else
									//m->SeqStep = 203;					
						

							}
							
							else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){
						
										
						
								if(INDEX_Param_Set[0].pStepDist){
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
			
									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
									
									m->SeqStep++;
			
								}
								//else
									//m->SeqStep = 203;


							}
							//else
								//m->SeqStep = 203;
								

						break;

					}

			}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#ifdef EXTENDED_LOGS

			LogTxt(&LogMgr, "Path %d Done: %.2f" , (UDINT)File_RW._int.PathDoneCnt, ABS_TVL_Pos);
#else
			LogTxt(&LogMgr, "Path %d Done" , (UDINT)File_RW._int.PathDoneCnt);

#endif

			//TimerStart(&WF_Catch_TMR, curCfg->Param[parWF_RampDwn].val[0]);		
			m->SeqStep++;
				
		break;

			
		case 201:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "Quick stop %.2f" , ABS_TVL_Pos);
#else 
				LogTxt(&LogMgr, "Quick stop");
#endif
				m->SeqStep = 254;			// Quick Stop
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped){ // stopped by end of travel				
				status = 0;
			}				

			if (Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Status != prcsSTATUS_Stopped){ 			
				status = 0;
			}	
			
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			//if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				//status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{

#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "M:All%d Stop: %.2f" ,(UDINT) m->ToolHeadID, ABS_TVL_Pos);				

#endif		
				if(!(curCalib->GenSetup[SETUP_EXTRAS]&0x4)){
					
					switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

						
						default:



							if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc


								if(INDEX_Param_Set[0].pStepDist){

									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

									KK_Catched = 133;

									m->SeqStep++;
								}
								else
									m->SeqStep = 203;					
						

							}
							
							else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){
						
										
						
								if(INDEX_Param_Set[0].pStepDist){
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
			
									if(curCalib->GenSetup[SETUP_INDX_DIR])
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
									else
										Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
									
									m->SeqStep++;
			
								}
								else
									m->SeqStep = 203;


							}
							else
								m->SeqStep = 203;
								

						break;

					}
					}

				m->SeqStep++;
				
			}

		break;


		case 202:
				
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if(Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Status == prcsSTATUS_Stopped
				&& Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped){

				TimerStart(&m->sm.Tmr1, 0.1);
				m->SeqStep++;

			}
			
		break;

		case 203:
			// Always Coolant On
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped){

				LogTxt(&LogMgr, "M: Cyc. Time %.2f" , GlobalTimer);
				m->SeqStep++;
			}	

		break;

		

		case 204:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Quick Stop
				//m->SeqStep = 205;			// Normal Stop
				break;
			}

			// Check if Index move done
			if (1)
			{

				
				if((abs(m->curSegm[ACT_TVL_DIST] - Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3]) < 10)
				//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped)
					&& (m->SubSeqStep == 2)) // IMPORTANT - head swap is only when was rewrap before
				{

					m->SubSeqStep = 1;

					//if (curCfg->Param_ENUM[parENUM_PathMode].val[0] == 0.0)
						//m->ToolHeadID = !m->ToolHeadID;

					if (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0)
					{ // Relative Move
						if (TVL_ROT_Param_Set[0].pEncoder)
							OLD_MTR_POS[TVL_ENC_HOME] = *TVL_ROT_Param_Set[0].pEncoder;
					}
					AutoStartEnable = 1;
					m->SeqStep++;

				}
				
				else 
				{
					m->SeqStep = 254;
				}			


			}

		break;

		case 205:			
			
			File_RW._int.PathDoneCnt++;
			newFile = 0;
			/*
			if (curCfg->Param_ENUM[parENUM_WeldProgramLinkMode].val[0] > 0.0)
			{

				if (curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				{

					if (strlen(curCfg->NextProgramName) > 0)
					{

						if (!strstr(curCfg->NextProgramName, "NONE"))
						{ // Not None
							TakeNextWeldProgram(hmi);
							newFile 			= 1;

						}

					}

				}

				else 
				{

					if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
					{

						if (strlen(curCfg->NextProgramName) > 0)
						{

							if (!strstr(curCfg->NextProgramName, "NONE"))
							{ // Not None
								TakeNextWeldProgram(hmi);
								newFile 			= 1;
							}							
								
						}
						
					}

				}

			}
			*/

			if (newFile)
				m->SeqStep = 206;
			else 
				m->SeqStep = 207;

		break;

		case 206:
			if (File_RW.CMD == 0)
			{

				if (File_RW.Error)
					m->SeqStep = 206;

				else 
				{
					AutoStartEnable = 1;
					m->SeqStep = 207;

				}
			}

		break;

		case 207:
			
			AutoStartEnable = 1;

			/*
			if (File_RW._int.PathDoneCnt > (UINT)curCfg->Param[parGEN_WeldProgramRepeat].val[0])
			{
				File_RW._int.PathDoneCnt = (UINT)
				curCfg->Param[parGEN_WeldProgramRepeat].val[0];

				AutoStartEnable 	= 0;
			}
			*/

			m->SeqStep++;
			
		break;

		case 208:
			///////////////////////////////////
			//Auto Start		
			if (curCalib->GenSetup[SETUP_AUTO_START] && AutoStartEnable)
			{
				TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]);
				m->SeqStep = 220;

			}
			else 
				m->SeqStep = 254;

		break;

		case 210:

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				
			}
			else if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) // stopped by end of travel	
				m->SeqStep = 0;	
				
		break;

		case 220:
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep = 254;			// Quick Stop
				break;
			}
			else if(!TimerIsTimeOut(&m->sm.Tmr1))
				break;

			GlobalTimer = 0;

			//WF_TVL_Diff = 0;	
			LogTxt(&LogMgr, "Auto Repeat");

			if(Cont_Stop_By_End)
				AutoStartBtn = 0;
			else	
				AutoStartBtn = 1;
			m->SeqStep = 0;
			
		break;

		case 254:
			
			for (i = 0; i < PROCESS_NUM; i++)
			{ // beside PROC_COOL
				if (i != PROC_COOL)
					Process[i].xCMD = prcsHalt;
			}

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // STop without ramp 
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP; // return to center position
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			m->SeqStep++;
			
		break;

		default:


			if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				for (i = 0; i < PROCESS_NUM; i++)
				{ // beside PROC_COOL	
					if (i!= PROC_COOL && i!= PROC_OSC)
						Process[i].xCMD = prcsHalt;
				}

				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
				m->SeqStep			= 0;

			}

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped)
				m->SeqStep = 0;

			if (m->SeqStep == 0)
			{
				// Load new AVC param from new file
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter =	curCfg->Param[parAVC_FbkFilter].val[0];
			}

			

		break;
			
		}

		//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped))
		//StopBtn_Latch = 0;
		////////////////////////////
		TimerExecute(&WireDisable_TMR);
		TimerExecute(&WF_Catch_TMR);

		return ret;
	}

	
	DINT MasterSequence_Contin(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn)
	{
	int 			i;
	char			status;
	DINT			ret;
	DINT			d;
	USINT			newFile;

	//Cfg_typ *cfg;
	CurCfg_typ *	curCfg;

	//CurCalib_typ *curCalib , *defCalib;
	//Scaled_Var_obj *var;
	//cfg = (Cfg_typ*) m->pCfg;
	curCfg				= (CurCfg_typ *)
	m->pCurCfg;

	//curCalib = (CurCalib_typ*) m->pCalib;
	//defCalib = (CurCalib_typ*) m->pDefaultCalib;
	//var = (Scaled_Var_obj*) m->pScaledVar;
	m->TestMode 		= intrf->TestMode;

	ret 				= 0;

	if((hmi->curHeadID == 0xFFFE)){ // Screen RUn or Alter Run
		
		if(intrf->GlblBtnCmd[0] &ANDR_BTN_UP){

			AVC_Param_Set[0].BoostCmd = 1;
			PS_Param_Set[0].BoostCmd = 1;
			TVL_ROT_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[0].BoostCmd = 1;
			WIRE_Param_Set[1].BoostCmd = 1;
			
		}
		else if(intrf->GlblBtnCmd[0] &ANDR_BTN_DOWN){

			AVC_Param_Set[0].BoostCmd = 2;
			PS_Param_Set[0].BoostCmd = 2;
			TVL_ROT_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[0].BoostCmd = 2;
			WIRE_Param_Set[1].BoostCmd = 2;

		}
		else{

			AVC_Param_Set[0].BoostCmd = 0;
			PS_Param_Set[0].BoostCmd = 0;
			TVL_ROT_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[0].BoostCmd = 0;
			WIRE_Param_Set[1].BoostCmd = 0;


		}
	}
	else{

		AVC_Param_Set[0].BoostCmd = 0;
		PS_Param_Set[0].BoostCmd = 0;
		TVL_ROT_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[0].BoostCmd = 0;
		WIRE_Param_Set[1].BoostCmd = 0;

	}
	
	/////////
	if (StopBtn)
	{
		AutoStartBtn		= 0;

		if (TimerIsTimeOut(&DoubleStop_TMR))
			TimerStart(&DoubleStop_TMR, 0.5);
		else 
			DoubleStopBtn_Latch = 1;
	}

	if (StopBtn)
		StopBtn_Latch = 1;

	
	m->SubSeqStep = 3; // Only Information that target is reached

	/*
	if ((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) ||
	(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_ReturnDly)){

		if (DoubleStopBtn_Latch)
			m->SubSeqStep = 0;
	}
	DoubleStopBtn_Latch = 0;
	

	if(curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 3) // Continues welding
		m->SubSeqStep = 0;
		

	if (StopBtn_Latch)
		Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // No Auto Stop and Not Rewrap

	*/	

	//////////////////////////
	////////////////////////////
	if (m->SeqStep > 0)
		GlobalTimer += 0.0512; // Timer based on task class


	//////////////

	

		//	 MASTER SEQUENCE  //
	switch (m->SeqStep)
	{
		case 0:

			Cont_Stop_By_End = 0;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0; // AVC UnLocked

			//////////////////////////////////////
			if(curCfg->Param[parGEN_WeldProgramRepeat].val[0] == 0.0)
				File_RW._int.PathDoneCnt = 0;
			
			// Reset Disable when was in Test Mode
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;
			StopBtn_Latch = 0;

			// AVC Locked is OFF

			//if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;

			if(StartBtn && !Check_OSC_Amplt_and_Center())
				break;

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");
				m->SeqStep = 254;			// Halt Stop
				break;
			}

			status = 1;

			if (//(Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped) ||
				(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped))
				status = 0;

			if (status)
			{

				if (!ErrorMgr.Error)
				{
					ret = 1;		// Raedy To Run

					if (StartBtn)
					//if (StartBtn || AutoStartBtn)
					{

						AutoStartBtn = 0;					
						
						GlobalTimer 		= 0;

						LogTxt(&LogMgr, "PreProcess start");

						if (curCalib->GenSetup[SETUP_SOUND_DELAY] || curCalib->GenSetup[SETUP_AUTO_START])
							TimerStart(&m->sm.Tmr1, curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);
						else 
							TimerStart(&m->sm.Tmr1, 0.1);

						m->SeqStep++;

						
					}
				}

			}
			else 
			{
				if (StopBtn)
				{
					LogTxt(&LogMgr, "Some Prcs aborted");
					m->SeqStep = 254;
				}

			}

		break;

		case 1:

			Cont_Stop_By_End = 0;
		// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep = 254;
			}
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if (!TimerIsTimeOut(&m->sm.Tmr1))
				break;
			
			else 
			{

				/*
				if (!m->TestMode)
				{
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
				}
				*/

				if (m->TestMode)
				{
				
					//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable |= 0x02 ;
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 1;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable |= 0x02;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable |= 0x02;

				}
				else 
				{

					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;

					if ((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run) &&
					(Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status == prcsSTATUS_Run))
					{
						//Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Disable &= 0xFD ;
						Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 0;
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Disable &= 0xFD;
						Process[m->pCfg->HeadCfg.HW.PROC_Indx].Disable &= 0xFD;

					}
					else 
						break;

				}

				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsSTART;

				//if(!m->TestMode)
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsForceON;

				LogTxt(&LogMgr, "ARC start");

				m->SeqStep++;

			}

		break;

		case 2:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 2; // Preparig To Run	

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Some Prcs aborted");
				m->SeqStep			= 254;
			}

			///////////////////////////
			else if (ErrorMgr.Error)
				m->SeqStep = 100;
			
			else if ((Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_Run)
					|| (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_OK))
			{


				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTART;
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 0; // Start with ramp 
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsSTART;

				/*	TVL */
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = m->SubSeqStep;
				
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;

#ifdef EXTENDED_LOGS			
				LogTxt(&LogMgr, "M:TVL%d Start: %.2f" , (UDINT) m->ToolHeadID , ABS_TVL_Pos);
#endif
				StartPos = 0;				
				
				CalculateRunOverlap_Cont();	

				
				if (!m->WireDisabled)
				{
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;
				}

				LogTxt(&LogMgr, "ARC established");

				TimerStart(&WireDisable_TMR, 3.0);

				m->SeqStep++;

			}

		break;

		case 3:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			WireDisableInable(WireDisable_TMR.Q);
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].Param[0] = Process[m->pCfg->HeadCfg.OSC.PROC_Indx].subStatus;
			ret = 3; //  Run	

			/*
			if (actRot != _actRot)
				TakeNextWeldProgram(hmi);
			*/
			_actRot = actRot;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].subStatus == 1)
			{ 
				if(Cont_Stop_By_End){
				
					m->SeqStep = 240; // Regular Stop
				}
				else{
				
					m->SeqStep = 200;

				}
				
			}

			if (StopBtn) // Stop by operator
			{
				if(Cont_Stop_By_End){
					LogTxt(&LogMgr, "Process stop by Operator");
					Cont_Stop_By_End = 0;
					m->SeqStep = 240;
				}
				else
					Cont_Stop_By_End = 1;
					

			}
			else if (ErrorMgr.Error || m->ESTOP)
			{
				LogTxt(&LogMgr, "Process Error");
				m->SeqStep = 100;
			}

		break;

		case 100:
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;

			//Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsHalt;
			switch (ErrorMgr.Error)
			{
				case ERROR_FATAL:
					m->SeqStep++;
				break;

				case ERROR_MOTOR:
					m->SeqStep++;
				break;

				case ERROR_MOTOR_INIT:
					m->SeqStep++;
				break;

				case ERROR_TIG_PROC:
					m->SeqStep++;
				break;

				default:
					m->SeqStep++;
				break;
			}

		break;

		case 101:
			ret = 4; // Preparig To Stop in Halt Mode	

			if (StopBtn)
			{
				for (i = 0; i < PROCESS_NUM; i++)
				{
					if (i != PROC_COOL)
						Process[i].xCMD = prcsHalt;
				}
				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{
				LogTxt(&LogMgr, "Prc Final stopped");
				m->SeqStep = 254;
			}

		break;

		case 199: // waiting for signal from OSC


			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[2] == 0.0) // pick is reached
			{ 
				
				m->SeqStep = 200;
				
			}

			if (StopBtn)
			{
				
				LogTxt(&LogMgr, "Process stop");

				//m->SeqStep = 254;
				
				//m->SeqStep = 200;

				LogTxt(&LogMgr, "Process stop by Operator");

				Cont_Stop_By_End = 0;

				m->SeqStep = 240;
				

			}
			else if (ErrorMgr.Error || m->ESTOP)
			{
				LogTxt(&LogMgr, "Process Error");
				m->SeqStep = 100;
			}


		break;
				
		case 200: // Contin Stop
						
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}


			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 1; // AVC Locked
			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[1] = 1; // Run Reduce Profile

			Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 2; // skip delays & Contin_Ramp
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickSTOP;

			Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 2; // skip delays & Contin_Ramp
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsQuickSTOP;

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 4; // Contin_Ramp
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsQuickSTOP;

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP;
			
			
#ifdef EXTENDED_LOGS

			LogTxt(&LogMgr, "Path %d Done: %.2f" , (UDINT)File_RW._int.PathDoneCnt, ABS_TVL_Pos);
#else
			LogTxt(&LogMgr, "Path %d Done" , (UDINT)File_RW._int.PathDoneCnt);

#endif

			//TimerStart(&WF_Catch_TMR, curCfg->Param[parWF_RampDwn].val[0]);		
			m->SeqStep++;
				
		break;

			
		case 201:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "Quick stop %.2f" , ABS_TVL_Pos);
#else 
				LogTxt(&LogMgr, "Quick stop");
#endif
				//m->SeqStep = 254;			// Quick Stop
		
					
				LogTxt(&LogMgr, "Process stop by Operator");

				Cont_Stop_By_End = 0;
				
				m->SeqStep = 240;			

				break;

				
				
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped){ // stopped by end of travel				
				status = 0;
			}	
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped){
				status = 0;
			}	

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped){
				//status = 0;
			}

			if (!((Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped)
				 || (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Retract))){
				//status = 0;
			}
	

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped){
				status = 0;
			}

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped){
				//status = 0;
			}

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped){
				//status = 0;
			}

			if (status)
			{

#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "M:All%d Stop: %.2f" ,(UDINT) m->ToolHeadID, ABS_TVL_Pos);				

#endif				
				switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

					/*
					case BPV_v2:	
					case BPV_v2_MPBO:
					case BPV_v2_MPGH:
					case BPV_v2_MPGV:
											


						if(*INDEX_Param_Set[0].pStepDist > 0.0){

							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

							if(curCalib->GenSetup[SETUP_INDX_DIR])
								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
							else
								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) ;


							
							if(m->ToolHeadID == 0){ // Head A make Rewrap so change len of beat B

								if(curCalib->GenDintValue[1]!= 0){		

									if(curCalib->GenDintValue[1] < -30)
										curCalib->GenDintValue[1] = -30;
									
									else if(curCalib->GenDintValue[1] > 30)
										curCalib->GenDintValue[1] = 30;
								
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] += curCalib->GenDintValue[1]; // Run Length correction

								
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; // Rewrap only
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART; 

								}

							}else{

								if(curCalib->GenDintValue[0]!= 0){	

									if(curCalib->GenDintValue[0] < -30)
										curCalib->GenDintValue[0] = -30;
									
									else if(curCalib->GenDintValue[0] > 30)
										curCalib->GenDintValue[0] = 30;
								
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] += curCalib->GenDintValue[0]; // Run Length correction

								
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; // Rewrap only
									Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART; 

								}
							}							

							KK_Catched = 133;

							
						}						

					break;
					*/

					default:

						if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // In OSc

							/*
							if(INDEX_Param_Set[0].pStepDist){

								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
								else
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);

								KK_Catched = 133;

								m->SeqStep++;
							}
							else
								m->SeqStep = 203;	

							*/

								if(*INDEX_Param_Set[0].pStepDist > 0.0){

								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
								else
									Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) ;


								
								if(m->ToolHeadID == 0){ // Head A make Rewrap so change len of beat B

									if(curCalib->GenDintValue[1]!= 0){		

										if(curCalib->GenDintValue[1] < -30)
											curCalib->GenDintValue[1] = -30;
										
										else if(curCalib->GenDintValue[1] > 30)
											curCalib->GenDintValue[1] = 30;
									
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] += curCalib->GenDintValue[1]; // Run Length correction

									
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; // Rewrap only
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART; 

									}

								}else{

									if(curCalib->GenDintValue[0]!= 0){	

										if(curCalib->GenDintValue[0] < -30)
											curCalib->GenDintValue[0] = -30;
										
										else if(curCalib->GenDintValue[0] > 30)
											curCalib->GenDintValue[0] = 30;
									
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] += curCalib->GenDintValue[0]; // Run Length correction

									
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 2; // Rewrap only
										Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART; 

									}
								}							

								KK_Catched = 133;

								
							}						
					

						}
						
						else if(curCalib->GenSetup[SETUP_INDX_ENBL] == 1){
					
									
					
							if(INDEX_Param_Set[0].pStepDist){
								Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsSTEPpos;
		
								if(curCalib->GenSetup[SETUP_INDX_DIR])
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist);
								else
									Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
								
								m->SeqStep++;
		
							}
							else
								m->SeqStep = 203;


						}
						else
							m->SeqStep = 203;

					break;

				}


				//m->SeqStep = 203;				
				m->SeqStep = 230;
				
			}

		break;		
		

		/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		case 230: // Start New Path


			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				//m->SeqStep = 254;			// Quick Stop

				LogTxt(&LogMgr, "Process stop by Operator");

				Cont_Stop_By_End = 0;

				m->SeqStep = 240;
				
				break;
			}


			if(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				break;

			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				break;
			

			// New Path in New Direction

			m->ToolHeadID = !m->ToolHeadID;		
			
			CalculateRunOverlap_Cont();				
					
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 4; // Contin_Ramps
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsQuickStart;

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJump2Run;

			Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Param[1] = 0; // return from Reduce Profile

			Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0; // AVC UnLocked	

			
			if (!m->WireDisabled){
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickStart;
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 2; // skip delays & Contin Ramps
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsQuickStart;
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 2; // skip delays & Contin Ramps
			}

#ifdef EXTENDED_LOGS			
			LogTxt(&LogMgr, "M:TVL%d Start: %.2f" , (UDINT) m->ToolHeadID , ABS_TVL_Pos);
#endif
			StartPos = 0;			

			m->SeqStep = 3;
			
		break;		

		///////////////   Stop By Operator
		//////////////////////////////////////////////////////////////////////////////
		case 240: // Stop by Btn
						
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			for (i = 0; i < PROCESS_NUM; i++)
			{
				if ((i != PROC_COOL)) //&&( i != m->pCfg->HeadCfg.TVL.PROC_Indx))
					Process[i].xCMD = prcsSTOP;

			}

			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // Regular stop no retract a moze byc z 1 wtedy rewrap
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 1; // Regular stop no retract
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 1; // Regular stop no retract
						
#ifdef EXTENDED_LOGS

			LogTxt(&LogMgr, "Path %d Done: %.2f" , (UDINT)File_RW._int.PathDoneCnt, ABS_TVL_Pos);
#else
			LogTxt(&LogMgr, "Path %d Done" , (UDINT)File_RW._int.PathDoneCnt);

#endif

			//TimerStart(&WF_Catch_TMR, curCfg->Param[parWF_RampDwn].val[0]);		
			m->SeqStep++;
				
		break;

			
		case 241:
			
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			ret = 5; // Preparing To Stop	

			if (StopBtn)
			{
#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "Quick stop %.2f" , ABS_TVL_Pos);
#else 
				LogTxt(&LogMgr, "Quick stop");
#endif
				m->SeqStep = 254;			// Quick Stop
			}

			status = 1;

			if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped){ // stopped by end of travel				
				status = 0;
			}	
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			//if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				//status = 0;

			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
				status = 0;

			if (status)
			{

#ifdef EXTENDED_LOGS

				LogTxt(&LogMgr, "M:All%d Stop: %.2f" ,(UDINT) m->ToolHeadID, ABS_TVL_Pos);				

#endif				
				m->SeqStep++;
				
			}

		break;


		case 242:
				
			// Always Coolant On
			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if (StopBtn)
			{
				LogTxt(&LogMgr, "Quick stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if(Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Status == prcsSTATUS_Stopped
				&& Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped){

				TimerStart(&m->sm.Tmr1, 0.1);
				m->SeqStep++;

			}
			
		break;

		case 243:
			// Always Coolant On
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				m->SeqStep			= 254;			// Quick Stop
				break;
			}

			if (!m->TestMode)
			{
				if (Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status != prcsSTATUS_Run)
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
			}

			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped){

				LogTxt(&LogMgr, "M: Cyc. Time %.2f" , GlobalTimer);

				if(Cont_Stop_By_End)
					m->ToolHeadID = !m->ToolHeadID;


				if(Cont_Stop_By_End){
					m->SeqStep++;
				}
				else{

					m->SeqStep = 0;
				}
					
			}	

		break;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////


		case 244:


			switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){

					/*
					case BPV_v2_MPGH:
					case BPV_v2_MPGV:

					case BPV_v2_MPBO:						
					*/

					default:

						if(*INDEX_Param_Set[0].pStepDist > 0.0){

							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTEPpos;

							if(curCalib->GenSetup[SETUP_INDX_DIR])
								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) * (-1.0);
							else
								Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[1] = (*INDEX_Param_Set[0].pStepDist) ;
	

							KK_Catched = 135;

							
						}						

					break;


					

				}

				m->SeqStep = 0;

		break;


		/////////////////////////////////////////////////////////////////////////////


		case 254: // Quick End of Welding
			
			for (i = 0; i < PROCESS_NUM; i++)
			{ // beside PROC_COOL
				if (i != PROC_COOL)
					Process[i].xCMD = prcsHalt;
			}

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // STop without ramp 
			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP; // return to center position
			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
			m->SeqStep++;
			
		break;

		default:


			if(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 3) // AVC Mode NOT LOCKED !
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] = 0;
			
			if (StopBtn)
			{
				LogTxt(&LogMgr, "Halt stop");

				for (i = 0; i < PROCESS_NUM; i++)
				{ // beside PROC_COOL	
					if (i!= PROC_COOL && i!= PROC_OSC)
						Process[i].xCMD = prcsHalt;
				}

				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // maintain gas
				m->SeqStep			= 0;

			}

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped)
				m->SeqStep = 0;

			if (m->SeqStep == 0)
			{
				// Load new AVC param from new file
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter =	curCfg->Param[parAVC_FbkFilter].val[0];
			}

			

		break;
			
		}

		//if((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped))
		//StopBtn_Latch = 0;
		////////////////////////////
		TimerExecute(&WireDisable_TMR);
		TimerExecute(&WF_Catch_TMR);

		return ret;
	}


void ProcessEvents_Android(MASTER_typ * m, LCD_NODE_obj * nodes, HMI_typ * hmi, HMI_INTRF_typ * intrf)
{
USINT			status;
UINT			where, indx, seg;
Scaled_Var_obj * pVar;
int 			i, k;
float			x, y, z, a, A, b, B;
CurCfg_typ *	curCfg;

UINT segm;
float pos;

//float TVL_StopDly , TVL_RmpDwn ,TVL_Vel , TVL_Vel_Fin , TVL_ReWrap;	
//float WF_StopDly , WF_RmpDwn;
curCfg				= m->pCurCfg;

	if (!curCfg)
		return;


	if (File_RW.NewFileLoaded)
	{

		File_RW.NewFileLoaded = 0;

		hmi->PLC_Changes	|= (0x0001 + 0x0002 + 0x0004 + 0x0008); // New CurCfg and New segment List

		PleaseUpdateHeadSetup(m);

		_TIG_Mode = 0xFFFF;
		_AVC_Mode = 0xFFFF;

		// Force limits from Cfg in curCfg for enumerated values
		for (i = 0; i < m->paramEnumLastNum; i++)
		{
			curCfg->Param_ENUM[i].min = cfg->Param_ENUM[i].min;
			curCfg->Param_ENUM[i].max = cfg->Param_ENUM[i].max;

		}


	}

	switch (hmi->FiredEvent)
	{

			case EVNT_SET_INDX_ZERO:


			switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

				case BPV_F_HEAD:
				case CLAMSHELL_HEAD:
				//dodane 6Dec23					
				case BPV_v1_MT:	

					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].Param[0] = 0;
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].xCMD = VECT_Set_Act_Pos;

					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].Param[0] = 0;
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].xCMD = VECT_Set_Act_Pos;		

					strcpy(hmi->pMessage[0], "	INDEX ");
					strcpy(hmi->pMessage[1], "	Zero ");
					xsprintf(hmi->pMessage[2], "is Set");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 0.1);

				break;


				case BPV_v2:
				case BPV_v2_Video:
				case BPV_v2_MPBO:
				case BPV_v2_MPGH:
				case BPV_v2_MPGV:
				//dodane 6Dec23	
				case BPV_v2_MT:		
				
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx]._cfg.absLastPos = 0; //OLD_MTR_POS[INDX_B_LAST_POS];
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx]._cfg.absFbk = 2;
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_A_Indx].xCMD = 1;

					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx]._cfg.absLastPos = 0; //OLD_MTR_POS[INDX_B_LAST_POS];
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx]._cfg.absFbk = 2;
					TML_MTR_INST[m->pCfg->HeadCfg.INDX.TML_B_Indx].xCMD = 1;

					strcpy(hmi->pMessage[0], "	INDEX ");
					strcpy(hmi->pMessage[1], "	Zero ");
					xsprintf(hmi->pMessage[2], "is Set");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 0.1);


				break;


				default:

					strcpy(hmi->pMessage[0], "	INDEX ");
					strcpy(hmi->pMessage[1], "	is Not");
					xsprintf(hmi->pMessage[2], "Allowed");
					strcpy(hmi->pMessage[3], " in this head ");
					TimerStart(&hmi->MsgTmr, 0.1);


				break;


			}	

				


			break;
			
			//////////////////////////////////////////////////////////////////
			case EVNT_FILL_MODE:
				
				curCalib->GenSetup[SETUP_OSC_EXC_MODE] ++;

				if (curCalib->GenSetup[SETUP_OSC_EXC_MODE] > 2)
					curCalib->GenSetup[SETUP_OSC_EXC_MODE] = 0;

			break;

			////////////////////////////////////////////////////////////////////
			case EVENT_WELD_WIZARD_DONE:
				
				strcpy(hmi->pMessage[0], "	  WIZARD");
				strcpy(hmi->pMessage[1], "	Calculated ");
				xsprintf(hmi->pMessage[2], "New Timing");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 0.1);
				
			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_STEP_FWD:
				
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTEPpos;

				
				

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_STEP_REV:
				
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTEPneg;

				

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_TEST_MODE_ON_OFF:
				
				if (m->SeqStep == 0)
				{
					if (intrf->TestMode != 0)
						intrf->TestMode = 0;
					else 
						intrf->TestMode = 1;
				}

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_SWAP_HEAD:
				
				if (m->SeqStep != 0) // No Head Changes on run
					break;

				if (m->ToolHeadID != 0)
					m->ToolHeadID = 0;
				else 
					m->ToolHeadID = 1;

				if(curCalib->GenSetup[SETUP_TVL_ENC_SET]){
					if (TVL_ROT_Param_Set[0].pEncoder)
						OLD_MTR_POS[TVL_ENC_HOME] = *TVL_ROT_Param_Set[0].pEncoder;
				}

				m->SubSeqStep = 1;
				
			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_SAVE_CURCFG:
				
				if (File_RW.CMD == 0)
				{
					File_RW.CMD = F_SAVE_CURCFG;

					strcpy(hmi->pMessage[0], "	  Parameters  ");
					strcpy(hmi->pMessage[1], "	 ");
					strcpy(hmi->pMessage[2], "		SAVED ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.0);

				}

			break;

			////////////////////////////////////////////////////////////////////

			case EVNT_POPUP_OK:

				switch(POPUP_EVENT){

					case EVNT_SET_HOME:

						SetZeroPosition(m);					

						strcpy(hmi->pMessage[0], "Segm and TVL Zero ");
						strcpy(hmi->pMessage[1], " are Set ");
						xsprintf(hmi->pMessage[2], "");
						strcpy(hmi->pMessage[3], "");
						TimerStart(&hmi->MsgTmr, 0.5);
						//KK_test = 2;					

					break;

					case EVNT_OSC_ALLIGMENT:

						if(OSC_TML_Param_Set[0].PosFbkIndx == 2)
							pos = OSC_TML_Param_Set[0].pVect->FbkVect.x2 ;

						else
							pos = OSC_TML_Param_Set[0].pVect->FbkVect.x ;

						
						if((*OSC_TML_Param_Set[0].pPrimAmpl) > (OSC_TML_Param_Set[0].pVect->OutVect.softLimits[1] - OSC_TML_Param_Set[0].pVect->OutVect.softLimits[0]))
							(*OSC_TML_Param_Set[0].pPrimAmpl) = (OSC_TML_Param_Set[0].pVect->OutVect.softLimits[1] - OSC_TML_Param_Set[0].pVect->OutVect.softLimits[0]);

						
						if((pos  + (*OSC_TML_Param_Set[0].pPrimAmpl)/2.0) > OSC_TML_Param_Set[0].pVect->OutVect.softLimits[1]){
							
							(*OSC_TML_Param_Set[0].pPrimAmpl) = (OSC_TML_Param_Set[0].pVect->OutVect.softLimits[1] - pos )*2.0;									
						}

						else if((pos  - (*OSC_TML_Param_Set[0].pPrimAmpl)/2) < OSC_TML_Param_Set[0].pVect->OutVect.softLimits[0]){
							
							(*OSC_TML_Param_Set[0].pPrimAmpl) = (pos  - OSC_TML_Param_Set[0].pVect->OutVect.softLimits[0])*2.0;
							
						}

						// Check if Amplitude not negative
						if((*OSC_TML_Param_Set[0].pPrimAmpl) < 0)
							(*OSC_TML_Param_Set[0].pPrimAmpl) = 0;


					break;

				}

				POPUP_EVENT = 0;	

			break;

			case EVNT_POPUP_CANCEL:

				switch(POPUP_EVENT){

					case EVNT_SET_HOME:

						//KK_test = 3;

					break;

					case EVNT_OSC_ALLIGMENT:

						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsAlligment;

					break;

				}				
				POPUP_EVENT = 0;
				
			break;

			
			case EVNT_SET_HOME:
				
				if (m->SeqStep == 0)				
				{
					POPUP_EVENT = EVNT_SET_HOME;

					if (TVL_ROT_Param_Set[0].pEncoder)
						OLD_MTR_POS[TVL_ENC_HOME] = *TVL_ROT_Param_Set[0].pEncoder;
					
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].Param[0] = 0;
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_A_Indx].xCMD = VECT_Set_Act_Pos;

					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].Param[0] = 0;
					VectorStateMach[m->pCfg->HeadCfg.INDX.VECT_B_Indx].xCMD = VECT_Set_Act_Pos;		
												
					strcpy(hmi->pMessage[0], "TVL Zero is Set. ");
					strcpy(hmi->pMessage[1], "Do you want to Set ");
					xsprintf(hmi->pMessage[2], "Segment Zero");
					strcpy(hmi->pMessage[3], " as well ?");
					hmi->xCMD = SHOW_POPUP_CMD;					
					
				}else{

					strcpy(hmi->pMessage[0], "  Sorry,");
					strcpy(hmi->pMessage[1], " I am not");
					xsprintf(hmi->pMessage[2], "  ready");
					strcpy(hmi->pMessage[3], "");
					TimerStart(&hmi->MsgTmr, 0.5);

				}

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_TEST_OSC:
				
				if (m->SeqStep == 0)
				{							

					if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped)
					{
						if(Check_OSC_Amplt_and_Center()){
							
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // Start without ramp 
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJump2Run;

						}
					}
					else if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
					{
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // Stop without ramp 
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP;

					}					

				}

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_GAS_ON_OFF:
				
				if (m->Mode != MASTER_MODE_RUN)
				{
					if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Stopped)
						Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsForceON;
					
					else if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run)
						Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsHalt;
				}

			break;

			////////////////////////////////////////////////////////////////////
			case EVNT_GO_CALIB_MODE:
				
				if((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped) ||
					(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
					(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped) ||
					(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped) ||
					(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped) ||
					(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped))
					status = 0;

				if (status && (m->sm.Step == 50))
				{
					m->sm.Step = 100;
				}

			break;

			////////////////////////////////////////////////////////////////////
		case EVNT_RETURN_CALIB_MODE:
			
			for (i = 0; i < PROCESS_NUM; i++)
			{ // beside PROC_COOL
				if (i != PROC_COOL)
					Process[i].xCMD = prcsHalt;
			}

			Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP; // rmaintain gas
			m->sm.Step = 0;
			
		break;

		/////////////////////////////////////////////////////////////////////////////////
		case EVNT_OSC_POS_LIMIT_SET:
			
			if (m->pCfg->HeadCfg.OSC.Limits_Harcoded)
			{

				strcpy(hmi->pMessage[0], " OSC Limits  ");
				strcpy(hmi->pMessage[1], " are hardcoded  ");
				strcpy(hmi->pMessage[2], " Operation ");
				strcpy(hmi->pMessage[3], " denied");
				TimerStart(&hmi->MsgTmr, 1.5);

				break;

			}

			if((curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_TT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGH)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGV) ){


					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1] = TML_MTR_INST[m->pCfg->HeadCfg.TVL.TML_Indx].PosFBK;

					
					strcpy(hmi->pMessage[0], " TVL (OSC) Positive  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

					break;
			

			}			

			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[1] == 0)
			{

				
				if(1)
				{				

					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1] = TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_A_Indx].PosFBK + OSC_TML_Param_Set[0].MaxOscAmpl/2.0 ;

					if (m->pCfg->HeadCfg.OSC.TML_A_Indx == m->pCfg->HeadCfg.OSC.TML_B_Indx) // Only One OSC
						curCalib->Points[CALIB_OSC_SFT_LIMIT].val[3] = curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1];								
					

					strcpy(hmi->pMessage[0], " OSC Positive  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

				}
				else 
				{
					strcpy(hmi->pMessage[0], " Positive is ");
					strcpy(hmi->pMessage[1], "	 lower ");
					strcpy(hmi->pMessage[2], " than negative");
					strcpy(hmi->pMessage[3], "	TRY AGAIN ");
					TimerStart(&hmi->MsgTmr, 2.0);

				}

			}
			else 
			{

				//if (TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_B_Indx].PosFBK > curCalib->Points[CALIB_OSC_SFT_LIMIT].val[2])
				if(1)
				{
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[3] = TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_B_Indx].PosFBK + OSC_TML_Param_Set[0].MaxOscAmpl/2.0;

					if (m->pCfg->HeadCfg.OSC.TML_A_Indx == m->pCfg->HeadCfg.OSC.TML_B_Indx) // Only One OSC
						curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1] = curCalib->Points[CALIB_OSC_SFT_LIMIT].val[3];

					strcpy(hmi->pMessage[0], " OSC Positive  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

				}
				else 
				{
					strcpy(hmi->pMessage[0], " Positive is ");
					strcpy(hmi->pMessage[1], "	 lower ");
					strcpy(hmi->pMessage[2], " than negative");
					strcpy(hmi->pMessage[3], "	TRY AGAIN ");
					TimerStart(&hmi->MsgTmr, 2.0);

				}

			}

		break;

		/////////////////////////////////////////////////////////////////////////			
		case EVNT_OSC_NEG_LIMIT_SET:


			if (m->pCfg->HeadCfg.OSC.Limits_Harcoded)
			{

				strcpy(hmi->pMessage[0], " OSC Limits  ");
				strcpy(hmi->pMessage[1], " are hardcoded  ");
				strcpy(hmi->pMessage[2], " Operation ");
				strcpy(hmi->pMessage[3], " denied");
				TimerStart(&hmi->MsgTmr, 1.5);

				break;

			}

			if((curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_TT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGH)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGV)
				 ){


					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0] = TML_MTR_INST[m->pCfg->HeadCfg.TVL.TML_Indx].PosFBK;

					
					strcpy(hmi->pMessage[0], " TVL (OSC) Negative  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

					break;
			

			}	
			
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[1] == 0)
			{

				//if (TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_A_Indx].PosFBK < curCalib->Points[CALIB_OSC_SFT_LIMIT].val[1])
				if(1)
				{
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0] = TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_A_Indx].PosFBK - OSC_TML_Param_Set[0].MaxOscAmpl/2.0;

					if (m->pCfg->HeadCfg.OSC.TML_A_Indx == m->pCfg->HeadCfg.OSC.TML_B_Indx) // Only One OSC
						curCalib->Points[CALIB_OSC_SFT_LIMIT].val[2] = curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0];

					strcpy(hmi->pMessage[0], " OSC Negative  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

				}
				else 
				{
					strcpy(hmi->pMessage[0], " Negative is ");
					strcpy(hmi->pMessage[1], "	 greater ");
					strcpy(hmi->pMessage[2], " than positive");
					strcpy(hmi->pMessage[3], "	TRY AGAIN ");
					TimerStart(&hmi->MsgTmr, 2.0);

				}
			}
			else 
			{

				//if (TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_B_Indx].PosFBK < curCalib->Points[CALIB_OSC_SFT_LIMIT].val[3])
				if(1)
				{
					curCalib->Points[CALIB_OSC_SFT_LIMIT].val[2] = TML_MTR_INST[m->pCfg->HeadCfg.OSC.TML_B_Indx].PosFBK - OSC_TML_Param_Set[0].MaxOscAmpl/2.0;

					if (m->pCfg->HeadCfg.OSC.TML_A_Indx == m->pCfg->HeadCfg.OSC.TML_B_Indx) // Only One OSC
						curCalib->Points[CALIB_OSC_SFT_LIMIT].val[0] = curCalib->Points[CALIB_OSC_SFT_LIMIT].val[2];

					strcpy(hmi->pMessage[0], " OSC Negative  ");
					strcpy(hmi->pMessage[1], " Software  ");
					strcpy(hmi->pMessage[2], " Limit SET ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.5);

				}
				else 
				{
					strcpy(hmi->pMessage[0], " Negative is ");
					strcpy(hmi->pMessage[1], "	 greater ");
					strcpy(hmi->pMessage[2], " than positive");
					strcpy(hmi->pMessage[3], "	TRY AGAIN ");
					TimerStart(&hmi->MsgTmr, 2.0);
				}

			}

		break;

		///////////////////////////////////////////////////

		case EVNT_VER_TIG_PS_CALIB:
			
			hmi->CalibStep = 4;
			hmi->CalibStepDone = 1;
			strcpy(hmi->pHMI_Lines[0], "Ready To Test");

			//memcpy(&hmi->tempCalib , &curCalib->Points[CALIB_TIG_AMPS_CMD] , sizeof(tempCalib));
		break;

		case EVNT_VER_HW_PS_CALIB:
			
			hmi->CalibStep = 4;
			hmi->CalibStepDone = 1;
			strcpy(hmi->pHMI_Lines[0], "Ready To Test");

			//memcpy(&hmi->tempCalib , &curCalib->Points[CALIB_HW_AMPS_CMD] , sizeof(tempCalib));
		break;

		case EVNT_VER_TVL_CALIB:
			
			hmi->CalibStep = 6;
			hmi->CalibStepDone = 1;
			strcpy(hmi->pHMI_Lines[0], "Ready To Test");

			//memcpy(&hmi->tempCalib , &curCalib->Points[CALIB_TVL_SPEED_CMD] , sizeof(tempCalib));
		break;

		case EVNT_VER_WF_CALIB:
			
			hmi->CalibStep = 6;
			hmi->CalibStepDone = 1;
			strcpy(hmi->pHMI_Lines[0], "Ready To Test");

			//memcpy(&hmi->tempCalib , &curCalib->Points[CALIB_WF_SPEED_CMD] , sizeof(tempCalib));
		break;

		////////////////////////////////////////////////////////////////////
		case EVNT_SAVE_SETUP_BIN:
			
			if (File_RW.CMD == 0)
			{
				File_RW.CMD 		= F_SAVE_SETUP_BIN;
				
				strcpy(hmi->pMessage[0], " Setup As Binary");
				strcpy(hmi->pMessage[1], "");
				strcpy(hmi->pMessage[2], "	   SAVED ");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 1.0);

			}

		break;

		case EVNT_SAVE_SETUP_AS_BIN:
			
			if (File_RW.CMD == 0)
			{

				if (strlen(curCfg->CurProgramName) == 0)
					strcpy(curCfg->CurProgramName, "Default");

				strcpy(File_RW.fileNewName, curCfg->CurProgramName);


				if(Find_ext_txt(File_RW.fileNewName , "_state"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;

				if(Find_ext_txt(File_RW.fileNewName , "_calib"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;
					
				if(!Find_ext_txt(File_RW.fileNewName , "_setup"))
					strcat(File_RW.fileNewName, "_setup");

				File_RW.CMD = F_SAVE_AS_CURCFG_BIN;

				strcpy(hmi->pMessage[0], " Setup ");
				strcpy(hmi->pMessage[1], " As Binary ");
				strcpy(hmi->pMessage[2], "	   SAVED ");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 1.0);

			}

		break;

		case EVNT_SAVE_FULL_AS_BIN:
			
			if (File_RW.CMD == 0)
			{

				if (strlen(curCfg->CurProgramName) == 0)
					strcpy(curCfg->CurProgramName, "Default");

				strcpy(File_RW.fileNewName, curCfg->CurProgramName);


				if(Find_ext_txt(File_RW.fileNewName , "_setup"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;

				if(Find_ext_txt(File_RW.fileNewName , "_calib"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;


				if(!Find_ext_txt(File_RW.fileNewName , "_state"))
					strcat(File_RW.fileNewName, "_state");

				File_RW.CMD = F_SAVE_AS_FULL_BIN;

				strcpy(hmi->pMessage[0], "Machine State");
				strcpy(hmi->pMessage[1], " As Binary ");
				strcpy(hmi->pMessage[2], "	   SAVED ");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 1.0);

			}

		break;

		case EVNT_SAVE_CALIB_AS_BIN:
			
			if (File_RW.CMD == 0)
			{

				if (strlen(curCfg->CurProgramName) == 0)
					strcpy(curCfg->CurProgramName, "Default");

				strcpy(File_RW.fileNewName, curCfg->CurProgramName);


				if(Find_ext_txt(File_RW.fileNewName , "_setup"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;

				if(Find_ext_txt(File_RW.fileNewName , "_state"))
					File_RW.fileNewName[strlen(File_RW.fileNewName) - 6] = 0;


				if(!Find_ext_txt(File_RW.fileNewName , "_calib"))
					strcat(File_RW.fileNewName, "_calib");

				File_RW.CMD = F_SAVE_AS_CALIB_BIN;

				strcpy(hmi->pMessage[0], " Calibration");
				strcpy(hmi->pMessage[1], " As Binary ");
				strcpy(hmi->pMessage[2], "	   SAVED ");
				strcpy(hmi->pMessage[3], " ");
				TimerStart(&hmi->MsgTmr, 1.0);

			}

		break;
			
	}	



	// Temporary Patch it should be on event process switch
	if ((intrf->WheelDelta != 0) || ((intrf->GlblBtnCmd[0] &ANDR_BTN_RIGHT) && !intrf->_intr._GlblBtnCmd[0]) ||
	((intrf->GlblBtnCmd[0] &ANDR_BTN_LEFT) && !intrf->_intr._GlblBtnCmd[0]))
	{
		if (curCfg->Param_ENUM[parENUM_WF_WP_Relation].val[0] > 0)
		{
			for (i = 0; i < m->numZone; i++)
			curCfg->Param[parWP_SpeedSP].val[i] = curCfg->Param[parWF_SpeedSP].val[i] *0.31552314834734205493308631414126;
		}

	}

// Check Updated Values
	if (hmi->PleaseUpdate)
	{

		if (!intrf->TickPlus && !intrf->TickMinus)
			return;

		curCfg->Dirty		= 1;

		if (!m->pScaledVar)
		return;

		pVar =(Scaled_Var_obj *)m->pScaledVar;


		//seg = pVar[indx].segm;
		//seg = 0;
		seg = (hmi->PleaseUpdate & 0xFC00) / 1024;

		if ((! (curCalib->GenSetup[SETUP_WHERE_FILL] == 1) && (hmi->curHeadID == 65535))) //Not only Keyboard && Segment Page
		Fill_Segments(curCfg, hmi->PleaseUpdate & 0x3FF, seg, curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);


		switch (hmi->PleaseUpdate & 0x3FF)
		{
			case parWF_SpeedSP:
				
				if (curCfg->Param_ENUM[parENUM_WF_WP_Relation].val[0] > 0)
				{
					for (i = 0; i < m->numZone; i++)
						curCfg->Param[parWP_SpeedSP].val[i] = curCfg->Param[parWF_SpeedSP].val[i] *0.31552314834734205493308631414126;
				}

			break;

			case parOSC_InitAmplit:

				/*

				if(curCfg->Param[parOSC_InitAmplit].val[seg] > curCfg->Param[parOSC_Amplit].val[seg] ){

					curCfg->Param[parOSC_InitAmplit].val[seg] = curCfg->Param[parOSC_Amplit].val[seg];

					strcpy(hmi->pMessage[0], " OSC Init   ");
					strcpy(hmi->pMessage[1], " Amplitude ");
					strcpy(hmi->pMessage[2], " bigger then ");
					strcpy(hmi->pMessage[3], " Primery Amplitude");
					TimerStart(&hmi->MsgTmr, 1.5);


				}
				*/
			break;

			case parOSC_FinalAmplit:

				/*

				if(curCfg->Param[parOSC_FinalAmplit].val[seg] > curCfg->Param[parOSC_Amplit].val[seg] ){

					curCfg->Param[parOSC_FinalAmplit].val[seg] = curCfg->Param[parOSC_Amplit].val[seg];

					strcpy(hmi->pMessage[0], " OSC Final   ");
					strcpy(hmi->pMessage[1], " Amplitude ");
					strcpy(hmi->pMessage[2], " bigger then ");
					strcpy(hmi->pMessage[3], " Primery Amplitude");
					TimerStart(&hmi->MsgTmr, 1.5);


				}
				*/
			break;

			case parOSC_Amplit:
			Check_parOSC_Amplit(curCfg, hmi, seg);

				/*
				if(curCfg->Param[parOSC_Amplit].val[seg] < curCfg->Param[parOSC_FinalAmplit].val[seg] ){

					curCfg->Param[parOSC_Amplit].val[seg] = curCfg->Param[parOSC_FinalAmplit].val[seg];

					strcpy(hmi->pMessage[0], " OSC Primery	 ");
					strcpy(hmi->pMessage[1], " Amplitude ");
					strcpy(hmi->pMessage[2], " smaller then ");
					strcpy(hmi->pMessage[3], " Final Amplitude");
					TimerStart(&hmi->MsgTmr, 1.5);


				}

				if(curCfg->Param[parOSC_Amplit].val[seg] < curCfg->Param[parOSC_InitAmplit].val[seg] ){

					curCfg->Param[parOSC_Amplit].val[seg] = curCfg->Param[parOSC_InitAmplit].val[seg];

					strcpy(hmi->pMessage[0], " OSC Primery	 ");
					strcpy(hmi->pMessage[1], " Amplitude ");
					strcpy(hmi->pMessage[2], " smaller then ");
					strcpy(hmi->pMessage[3], " Init Amplitude");
					TimerStart(&hmi->MsgTmr, 1.5);


				}


				if(curCfg->Param[parOSC_Excur].val[seg] > 0 ){
					
					x = curCfg->Param[parOSC_Amplit].val[seg] / curCfg->Param[parOSC_Excur].val[seg];					

					if(x > curCfg->Param[parOSC_MaxTvlVel].val[0] ){

						curCfg->Param[parOSC_Amplit].val[seg] = curCfg->Param[parOSC_MaxTvlVel].val[0] * curCfg->Param
					[parOSC_Excur].val[seg];

						strcpy(hmi->pMessage[0], " OSC	 ");
						strcpy(hmi->pMessage[1], " Amplitude ");
						strcpy(hmi->pMessage[2], " was");
						strcpy(hmi->pMessage[3], " too high");
						TimerStart(&hmi->MsgTmr, 1.5);
						
					}	
						
				}

				*/
			break;

			case parOSC_Excur:
				
				Check_parOSC_Excur(curCfg, hmi, seg);

				/*

				if((curCfg->Param[parOSC_Excur].val[seg] > 0)  
					&& (curCfg->Param[parOSC_MaxTvlVel].val[0] > 0)){
					
					x = curCfg->Param[parOSC_Amplit].val[seg] / curCfg->Param[parOSC_Excur].val[seg];

					if(x >curCfg->Param[parOSC_MaxTvlVel].val[0] ){

						curCfg->Param[parOSC_Excur].val[seg] = curCfg->Param[parOSC_Amplit].val[seg] /		curCfg->Param
					[parOSC_MaxTvlVel].val[0];

						strcpy(hmi->pMessage[0], " OSC	 ");
						strcpy(hmi->pMessage[1], " Excurtion ");
						strcpy(hmi->pMessage[2], " was");
						strcpy(hmi->pMessage[3], " too small");
						TimerStart(&hmi->MsgTmr, 1.5);
						
					}	
						
				}

				*/
			break;

			case parTVL_InitVel:
				
			if (curCfg->Param[parTVL_InitVel].val[0] > curCfg->Param[parTVL_VelSP].val[seg])
			{
				curCfg->Param[parTVL_InitVel].val[0] = curCfg->Param[parTVL_VelSP].val[seg];

				strcpy(hmi->pMessage[0], " TVL Init   ");
				strcpy(hmi->pMessage[1], " Speed ");
				strcpy(hmi->pMessage[2], " bigger then ");
				strcpy(hmi->pMessage[3], " Primery Speed");
				TimerStart(&hmi->MsgTmr, 1.5);

			}

			break;

			case parTVL_FinalVel:
				
				if (curCfg->Param[parTVL_FinalVel].val[0] > curCfg->Param[parTVL_VelSP].val[seg])
				{
					curCfg->Param[parTVL_FinalVel].val[0] = curCfg->Param[parTVL_VelSP].val[seg];

					strcpy(hmi->pMessage[0], " TVL Final   ");
					strcpy(hmi->pMessage[1], " Speed ");
					strcpy(hmi->pMessage[2], " bigger then ");
					strcpy(hmi->pMessage[3], " Primery Speed");
					TimerStart(&hmi->MsgTmr, 1.5);
				}

			break;

			case parTVL_VelSP:
				
				if (curCfg->Param[parTVL_VelSP].val[seg] < curCfg->Param[parTVL_InitVel].val[0])
				{
					curCfg->Param[parTVL_VelSP].val[seg] = curCfg->Param[parTVL_InitVel].val[0];

					strcpy(hmi->pMessage[0], " TVL Primery	 ");
					strcpy(hmi->pMessage[1], " Speed ");
					strcpy(hmi->pMessage[2], " smaller then ");
					strcpy(hmi->pMessage[3], " Init Speed");
					TimerStart(&hmi->MsgTmr, 1.5);

				}

				if (curCfg->Param[parTVL_VelSP].val[seg] < curCfg->Param[parTVL_FinalVel].val[0])
				{
					curCfg->Param[parTVL_VelSP].val[seg] = curCfg->Param[parTVL_FinalVel].val[0];

					strcpy(hmi->pMessage[0], " TVL Primery	 ");
					strcpy(hmi->pMessage[1], " Speed ");
					strcpy(hmi->pMessage[2], " smaller then ");
					strcpy(hmi->pMessage[3], " Final Speed");
					TimerStart(&hmi->MsgTmr, 1.5);

				}

			break;

			case parAVC_FbkFilter:
				
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.signFilter =	curCfg->Param[parAVC_FbkFilter].val[0];
				VectorStateMach[VECT_AVC].xCMD = VECT_UpdateSignFilter;
				
			break;

			case parAVC_DB_Window:
				
				VectorStateMach[VECT_AVC].fParam[0] = curCfg->Param[parAVC_DB_Window].val[0];
				VectorStateMach[VECT_AVC].xCMD = VECT_Set_MaxError;
			
			break;

			case parAVC_KIP:
			case parAVC_KPP:
			
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kpp = (DINT)(curCfg->Param[parAVC_KPP].val[0] *TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kppMax);
				TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kip = (DINT)(curCfg->Param[parAVC_KIP].val[0] *TML_MTR_INST[m->pCfg->HeadCfg.AVC.TML_A_Indx]._cfg.kipMax);
			
				VectorStateMach[VECT_AVC].xCMD = VECT_Set_PID_Param;
				
			break;

			case parTVL_RunDist:
				
				PleaseUpdateHeadSetup(m);
				
			break;

			case parTVL_TrackDia:
				
			break;
			
		}


		////////   Parameter Leaders /////////////////////

		
		if (hmi->curNodeID == P_PARAM_WELD)
		{

			switch (indx)
			{
				case parAVC_Upslope:
				break;

				case parWF_RampUp:
				break;

				case parWF_RampDwn:
				break;

				case parAVC_DownSlope:
				break;
			}

		}
	}

	hmi->FiredEvent = 0;
	
}



void EditParam(UINT paramIndx)
{

	if (hmi->curNodeID >= P_MENU)
	{
		strcpy(hmi->ActiveBanner, "");
		return;
	}

	GlblBtnLED_Mask 	= intrf->GlblBtnCmd[0];

	Force_ENTR_LCD		= 1;
	hmi->DynPageNodeIndx = P_POP_UP;

	nodes[P_POP_UP].var.Var1ID = paramIndx;

	if (curCfg->Param[paramIndx].segmYes)
		var[paramIndx].segm = m->curSegm[ZONE];

	return;

}


void JogProcessAndroid(HMI_INTRF_typ * intrf, UINT procIndx, float gain)
{

	Process[procIndx].xCMD = prcsDirectCntrl;
	Process[procIndx].fParam[0] = gain;

	return;
}



void TakeNextWeldProgram(HMI_typ * hmi)
{


	if((strlen(curCfg->NextProgramName) > 31)
		|| (strlen(curCfg->NextProgramName) == 0))
		strcpy(curCfg->NextProgramName, PRG_NO_NEXT);

	

	//if (strlen(curCfg->NextProgramName) && strcmp(curCfg->NextProgramName, PRG_NO_NEXT))
	if(strcmp(curCfg->NextProgramName, PRG_NO_NEXT))
	{

		strcpy(File_RW.deviceName, "WeldPrograms");

		strcpy(File_RW.fileName, curCfg->NextProgramName);

		DeleteExtension(File_RW.fileName);

		if (File_RW.Mode == F_MODE_XML)
			strcat(File_RW.fileName, PRG_FILE_EXT);

		File_RW.CMD 		= F_LOAD_CURCFG;


		if (curCfg->Dirty)
		{
			strcpy(hmi->pMessage[0], "Changes are LOST !!!");
			xsprintf(hmi->pMessage[1], " %s", File_RW.fileName);
			strcpy(hmi->pMessage[2], "	is");
			strcpy(hmi->pMessage[3], " Loaded ");
			TimerStart(&hmi->MsgTmr, 1.0);
		}
		else 
		{
			strcpy(hmi->pMessage[0], " NEXT Program ");
			xsprintf(hmi->pMessage[1], " %s", File_RW.fileName);
			strcpy(hmi->pMessage[2], "	is");
			strcpy(hmi->pMessage[3], " Loaded ");
			TimerStart(&hmi->MsgTmr, 1.0);
		}

	}


}


void AndroidHelper(MASTER_typ * m, UINT n)
{

	if (!m || n > 1)
		return;

	/*
	m->AndIntrf[n].MasterMode = m->Mode;



	switch(m->AndIntrf[n].UserLevel){


	case USER_OPERATOR: strcpy(m->AndIntrf[n].UserLevelTXT , "OPERATOR"); break;

	case USER_ADJUSTER: strcpy(m->AndIntrf[n].UserLevelTXT , "ADJUSTER"); break;

	case USER_MANAGER:	strcpy(m->AndIntrf[n].UserLevelTXT , "MANAGER"); break;

	case USER_JA:		strcpy(m->AndIntrf[n].UserLevelTXT , "DESIGNER"); break;


	default: strcpy(m->AndIntrf[n].UserLevelTXT , "WRONG LEVEL"); break;


	}


	switch(m->AndIntrf[n].MasterMode){


	case MASTER_MODE_STOP: strcpy(m->AndIntrf[n].MasterModTXT , "IDLE"); break;

	case MASTER_MODE_RUN: strcpy(m->AndIntrf[n].MasterModTXT , "RUN"); break;

	case MASTER_MODE_CALIB_STOP:	strcpy(m->AndIntrf[n].MasterModTXT , "CALIBR. IDLE"); break;

	case MASTER_MODE_CALIB_RUN:	strcpy(m->AndIntrf[n].MasterModTXT , "CALIBR. RUN"); break;

	case MASTER_MODE_ERROR:			strcpy(m->AndIntrf[n].MasterModTXT , "ERROR"); break;

	case MASTER_MODE_ESTOP: 		strcpy(m->AndIntrf[n].MasterModTXT , "ESTOP"); break;


	default: strcpy(m->AndIntrf[n].MasterModTXT , "INITIAL"); break;



	}

	*/
}


void StopModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd)
{
	UINT			i;
	UINT			par;
	CurCfg_typ *	curCfg;

	curCfg				= m->pCurCfg;

	if (!curCfg)
		return;


	////////////
	switch (intrf->GlblBtnCmd[0])
	{
		case ANDR_BTN_HOME:	
			
		break;

		case ANDR_BTN_ARC_AMPS_NEG:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
					strcpy(hmi->ActiveBanner, "WB#1 JOG REV");
					JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_UD].PROC_Indx,(-1.0));
							
			}else if(!Process[PROC_INDX].Disable){
			
				

				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){
		
					
					case BPV_v2:
					case BPV_v2_Video:	
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:
					//dodane 6Dec23
					case BPV_v2_MT:	
						
				
						strcpy(hmi->ActiveBanner, " INDX JOG DWN ");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.INDX.PROC_Indx,
						m->pCfg->HeadCfg.INDX.BtnJog_Gain * getBtnProgress () * (-1.0));


					break;


					case BPV_v2_MT_BO:
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO:
					case BPV_v2_TT_BO_TOS:
					
						strcpy(hmi->ActiveBanner, "INDEX Not installed ");		

					break;


					default:

						strcpy(hmi->ActiveBanner, "INDEX JOG REV ");				
				
						if(hmi->curHeadID != P_MODE_INDX_CALIB)
							Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 0; // Both head Jogging
							
						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsJOGpos;
						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[0] = (-1.0);


					break;


				}	

			}
			else{

				strcpy(hmi->ActiveBanner, "INDEX Process Disabled ");	

			}
			
		
		break;

		case ANDR_BTN_ARC_AMPS_POS:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#1 JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_UD].PROC_Indx,(1.0));
					
			}else if(!Process[PROC_INDX].Disable){
			
				

				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){
		
					
					case BPV_v2:
					case BPV_v2_Video:	
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:	
					//dodane 6Dec23
					case BPV_v2_MT:		
				
						strcpy(hmi->ActiveBanner, " INDX JOG UP ");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.INDX.PROC_Indx,
						m->pCfg->HeadCfg.INDX.BtnJog_Gain * getBtnProgress ());


					break;


					case BPV_v2_MT_BO:
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO:
					case BPV_v2_TT_BO_TOS:
					
						strcpy(hmi->ActiveBanner, "INDEX Not installed ");		

					break;


					default:

						strcpy(hmi->ActiveBanner, "INDEX JOG FWD ");

						if(hmi->curHeadID != P_MODE_INDX_CALIB)
							Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 0; // Both head Jogging
							
						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].xCMD = prcsJOGpos;					
						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].fParam[0] = 1.0;


					break;


				}	

			}
			else{

				strcpy(hmi->ActiveBanner, "INDEX Process Disabled ");	

			}
		
		break;

		case ANDR_BTN_TRAVEL_SPEED_NEG:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#2 JOG REV");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_LR].PROC_Indx,(-1.0));
					
			}else{
			
				strcpy(hmi->ActiveBanner, " TRAVEL JOG REV ");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.TVL.PROC_Indx,
				m->pCfg->HeadCfg.TVL.BtnJog_Gain * getBtnProgress() * (-1.0));

				if (_TVL_Latch_Latched && TVL_Latch_Latched)
				{
					TVL_Latch_Latched	= 0;
					_TVL_Latch_Latched	= TVL_Latch_Latched;
					TVL_Latch_Cnt		= 0;
				}

				if (!TVL_Latch_Latched)
				{
					if (TVL_Latch_Cnt < TVL_Latch_Tm)
						TVL_Latch_Cnt++;
					else 
						TVL_Latch_Latched = 1;
				}

			}

		break;

		case ANDR_BTN_TRAVEL_SPEED_POS:



			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#2 JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_LR].PROC_Indx,(1.0));
					
					
			}else{
			
				strcpy(hmi->ActiveBanner, " TRAVEL JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.TVL.PROC_Indx,
				m->pCfg->HeadCfg.TVL.BtnJog_Gain * getBtnProgress());

				if (_TVL_Latch_Latched && TVL_Latch_Latched)
				{
					TVL_Latch_Latched	= 0;
					_TVL_Latch_Latched	= TVL_Latch_Latched;
					TVL_Latch_Cnt		= 0;
				}

				if (!TVL_Latch_Latched)
				{
					if (TVL_Latch_Cnt < TVL_Latch_Tm)
						TVL_Latch_Cnt++;
					else 
						TVL_Latch_Latched = 2;
				}

			}

		break;

		case ANDR_BTN_WIRE_SPEED_NEG:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				
				///  LAMPS ON / OFF
				if (!intrf->_intr._GlblBtnCmd[0]){				
					Hardware[1].Outp[9] = !Hardware[1].Outp[9];

					
					if(Hardware[1].Outp[9]){

						/* was only for Super Mini Head
						
						if(VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].xCMD == 0)
							VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].xCMD = VECT_Set_OUT0;

						if(VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].xCMD == 0)
							VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].xCMD = VECT_Set_OUT0;

						*/	

						
						if(m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx > 0){ // Zero means - do not apply
							
							if(VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx - 1].xCMD == 0)
								VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx -1].xCMD = VECT_Set_OUT0;
							
						}

						if(m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx > 0){ // Zero means - do not apply
						
							if(VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx-1].xCMD == 0)
								VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx-1].xCMD = VECT_Set_OUT0;
							
						}					

					}
					else{

						
						if(m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx > 0){ // Zero means - do not apply
							
							if(VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx - 1].xCMD == 0)
								VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_A_Indx -1].xCMD = VECT_Rst_OUT0;
							
						}

						if(m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx > 0){ // Zero means - do not apply

							if(VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx-1].xCMD == 0)
								VectorStateMach[m->pCfg->HeadCfg.LampOnOFF.VECT_B_Indx-1].xCMD = VECT_Rst_OUT0;
							
						}					

						/* was only for Super Mini Head
						if(VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].xCMD == 0)
							VectorStateMach[m->pCfg->HeadCfg.OSC.VECT_A_Indx].xCMD = VECT_Rst_OUT0;
						
						if(VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].xCMD == 0)
							VectorStateMach[m->pCfg->HeadCfg.WF.VECT_A_Indx].xCMD = VECT_Rst_OUT0;
						*/

					}
					
				}
					
					
			}else{
			
				strcpy(hmi->ActiveBanner, "WIRE JOG REV");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WF.PROC_Indx,
				m->pCfg->HeadCfg.WF.BtnJog_Gain * getBtnProgress() * (-1.0));

			}
		
		break;

		case ANDR_BTN_WIRE_SPEED_POS:

			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				// Camera View 
				if (!intrf->_intr._GlblBtnCmd[0])
					MV[0].xCMD = MV_LOAD_NEXT_CMD;
					
					
			}else{
			
				strcpy(hmi->ActiveBanner, "WIRE JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WF.PROC_Indx,
				m->pCfg->HeadCfg.WF.BtnJog_Gain * getBtnProgress());


			}
		
		break;

		case ANDR_BTN_AVC_VOLTS_NEG:
			
			strcpy(hmi->ActiveBanner, " AVC JOG DOWN ");
			JogProcessAndroid(intrf, m->pCfg->HeadCfg.AVC.PROC_Indx,
			m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress() * (-1.0));
		
		break;

		case ANDR_BTN_AVC_VOLTS_POS:
			
			strcpy(hmi->ActiveBanner, " AVC JOG UP ");
			JogProcessAndroid(intrf, m->pCfg->HeadCfg.AVC.PROC_Indx,
			m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress());
		
		break;

		case ANDR_BTN_OSC_SPEED_NEG:
			
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
			{
				strcpy(hmi->ActiveBanner, "OSC	TIME Minus");

				if (m->AndIntrf[0].GlblBtnLED[1] &0x1)
					par = parOSC_Dwell_In;

				else if (m->AndIntrf[0].GlblBtnLED[1] &0x2)
					par = parOSC_Dwell_Out;

				else 
					par = parOSC_Excur;


				if (!intrf->_intr._GlblBtnCmd[0])
				{
					DecrParam(curCfg, par, m->curSegm[ZONE], 1.0);

					if (par == parOSC_Excur)
						Check_parOSC_Excur(curCfg, hmi, m->curSegm[ZONE]);

					if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
						Fill_Segments(curCfg, par, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);

				}

			}
			else if(!Process[PROC_WRST].Disable){
				
				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

					/*
					case BPV_v2:
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:

						strcpy(hmi->ActiveBanner, " INDX JOG DWN ");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.INDX.PROC_Indx,
						m->pCfg->HeadCfg.INDX.BtnJog_Gain * getBtnProgress () * (-1.0));

					break;	
					*/
					
					case BPV_v2:
					case BPV_v2_Video:	
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:
					case BPV_v2_MT_BO:
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO:
					case BPV_v2_TT_BO_TOS:	
					//dodane 6Dec23
					case BPV_v2_MT:		

						strcpy(hmi->ActiveBanner, " WRST JOG Left ");	

						Process[m->pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
						Process[m->pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = m->pCfg->HeadCfg.WRST.BtnJog_Gain * getBtnProgress () * (-1.0);

					break;


					default:

						strcpy(hmi->ActiveBanner, "WRST JOG Left");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.WRST.PROC_Indx,
						m->pCfg->HeadCfg.WRST.BtnJog_Gain * getBtnProgress () * (-1.0));


					break;

				}			
				
								
			}
			else{

				strcpy(hmi->ActiveBanner, "WRST Process Disabled ");	

			}
			

		break;

		case ANDR_BTN_OSC_SPEED_POS:
			
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
			{

				strcpy(hmi->ActiveBanner, "OSC	TIME plus");

				if (m->AndIntrf[0].GlblBtnLED[1] &0x1)
					par = parOSC_Dwell_In;

				else if (m->AndIntrf[0].GlblBtnLED[1] &0x2)
					par = parOSC_Dwell_Out;
				
				else 
					par = parOSC_Excur;


				if (!intrf->_intr._GlblBtnCmd[0])
				{
					IncrParam(curCfg, par, m->curSegm[ZONE], 1.0);

					if (par == parOSC_Excur)
						Check_parOSC_Excur(curCfg, hmi, m->curSegm[ZONE]);

					if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
						Fill_Segments(curCfg, par, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);
				}
			}
			else if(!Process[PROC_WRST].Disable){

				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

					/*
					case BPV_v2:
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:

						strcpy(hmi->ActiveBanner, " INDX JOG UP ");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.INDX.PROC_Indx,
						m->pCfg->HeadCfg.INDX.BtnJog_Gain * getBtnProgress ());

					break;	
					*/
					
					case BPV_v2:
					case BPV_v2_Video:
					case BPV_v2_MPBO:
					case BPV_v2_MPGV:
					case BPV_v2_MPGH:
					case BPV_v2_MT_BO:
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO:
					case BPV_v2_TT_BO_TOS:
					//dodane 6Dec23
					case BPV_v2_MT:		

						strcpy(hmi->ActiveBanner, " WRST JOG Right  ");	

						Process[m->pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
						Process[m->pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = m->pCfg->HeadCfg.WRST.BtnJog_Gain * getBtnProgress ();
					break;


					default:

						strcpy(hmi->ActiveBanner, " WRST JOG Right ");

						JogProcessAndroid(intrf, m->pCfg->HeadCfg.WRST.PROC_Indx,
						m->pCfg->HeadCfg.WRST.BtnJog_Gain * getBtnProgress ());


					break;

				}

								
			}
			else{

				strcpy(hmi->ActiveBanner, "WRST Process Disabled ");	

			}

		break;

		case ANDR_BTN_OSC_AMPL_NEG:
			
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
			{
				strcpy(hmi->ActiveBanner, "BTN_OSC_AMPL_NEG ");

				if (!intrf->_intr._GlblBtnCmd[0])
					DecrParam(curCfg, parOSC_Amplit, m->curSegm[ZONE], 1.0);

				Check_parOSC_Amplit(curCfg, hmi, m->curSegm[0]);
			}
			else if ((Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped) ||
			(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
			{
				strcpy(hmi->ActiveBanner, "OSC JOG LEFT");

				switch (m->pCfg->HeadCfg.OSC.BtnJog_Mode)
				{
					case BTN_JOG_MODE_SINGLE:
						
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,
						m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress () * (-1.0));
						
					break;

					case BTN_JOG_MODE_DUAL:
						
						if (m->ToolHeadID)
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress());
						else 
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress() * (-1.0));

					break;

					case BTN_JOG_MODE_DUAL_INV:
						
						if (!m->ToolHeadID)
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress());
						else 
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress() * (-1.0));

					break;
				}

				
			}

		break;

		case ANDR_BTN_OSC_AMPL_POS:
				
			if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
			{

				strcpy(hmi->ActiveBanner, "ANDR_BTN_OSC_AMPL_POS");

				if (!intrf->_intr._GlblBtnCmd[0])
					IncrParam(curCfg, parOSC_Amplit, m->curSegm[ZONE], 1.0);

				Check_parOSC_Amplit(curCfg, hmi, m->curSegm[0]);
			}
			else if ((Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped) ||
			(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
			{
				strcpy(hmi->ActiveBanner, "OSC JOG RIGHT");

				switch (m->pCfg->HeadCfg.OSC.BtnJog_Mode)
				{
					case BTN_JOG_MODE_SINGLE:
						
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,
						m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress ());
						
					break;

					case BTN_JOG_MODE_DUAL:
						
						if (m->ToolHeadID)
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress() * (-1.0));
						else 
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress());

					break;

					case BTN_JOG_MODE_DUAL_INV:
						
						if (!m->ToolHeadID)
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress() * (-1.0));
						else 
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.BtnJog_Gain * getBtnProgress());

					break;
				}

			}

		break;

		case ANDR_BTN_AVC_LOCK:
			
			strcpy(hmi->ActiveBanner, "AVC LOCK ON / OFF");
		
			if (curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 2)
			{ // AVC Mode NOT Lock

				if (!intrf->_intr._GlblBtnCmd[0])
				{

					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] =	!Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0]; // AVC Hold

				}

			}

			
		break;

		case ANDR_BTN_WIRE_DISABLE:
			
			strcpy(hmi->ActiveBanner, "WIRE ON/OFF");

			if (!intrf->_intr._GlblBtnCmd[0])
			{
				if (m->WireDisabled != 0)
					m->WireDisabled = 0;
				else 
					m->WireDisabled = 1;

				LogTxt(&LogMgr, "Wire Disable Event %d" , (DINT)m->WireDisabled);
			}

		break;

		case ANDR_BTN_STOP:
			
			for (i = 0; i < PROCESS_NUM; i++)
				Process[i].fParam[0] = 0.0;

			TVL_Latch_Latched = 0;
			_TVL_Latch_Latched = TVL_Latch_Latched;
			TVL_Latch_Cnt = 0;
			
		break;

		default:
			
			for (i = 0; i < PROCESS_NUM; i++)
			{
				if (i != m->pCfg->HeadCfg.OSC.PROC_Indx)
					Process[i].fParam[0] = 0.0;
			}

			_TVL_Latch_Latched = TVL_Latch_Latched;
			TVL_Latch_Cnt = 0;
		break;
	}

	if (! (intrf->GlblBtnCmd[0] & (ANDR_BTN_OSC_AMPL_NEG + ANDR_BTN_OSC_AMPL_POS)))
		Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = 0.0;


	if((curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MT_BO_TOS)
		|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_TT_BO_TOS)
		|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGH)
		|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGV))
		TVL_Latch_Latched = 0;
		

	switch (TVL_Latch_Latched)
	{
		case 1:
			JogProcessAndroid(intrf, m->pCfg->HeadCfg.TVL.PROC_Indx, -24.0);
		break;

		case 2:
			JogProcessAndroid(intrf, m->pCfg->HeadCfg.TVL.PROC_Indx, 24.0);
		break;
	}

	//	OScilation Center Jog	
	if ((intrf->WheelDelta != 0) && (hmi->curHeadID == 0xFFFE))
	{ // only jog on Run and Alternate Run screen

		if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Init)
		{

			if ((Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped) ||
			(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
			{

				if (!m->pCfg->HeadCfg.OSC.WhelJog_OnlyWhenRun)
				{

					switch (m->pCfg->HeadCfg.OSC.WhelJog_Mode1)
					{
						case WHL_JOG_MODE_SINGLE:
							
							JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);

						break;

						case WHL_JOG_MODE_STEP:
							
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJOGpos;
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain1;

						break;

						case WHL_JOG_MODE_DUAL:
							
							if (m->ToolHeadID)
								JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);
							else 
								JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta * (-1.0));

						break;

						case WHL_JOG_MODE_DUAL_INV:
							
							if (m->ToolHeadID)
								JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);
							else 
								JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx, m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta * (-1.0));

						break;
					}
				}
					
			}
			else 
			{

				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJOGpos;

				switch (m->pCfg->HeadCfg.OSC.WhelJog_Mode2)
				{
					case WHL_JOG_MODE_SINGLE:
					case WHL_JOG_MODE_STEP:
						
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
					
					break;

					case WHL_JOG_MODE_DUAL:
						
						if (m->ToolHeadID)
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
						else 
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2 * (-1.0);

					break;

					case WHL_JOG_MODE_DUAL_INV:
						
						if (!m->ToolHeadID)
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
						else 
							Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2 * (-1.0);

					break;
				}

			}

		}
	}

	
}


void StopCalibrationModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd)
{

	StopModeAndroidButtons(m, hmi, intrf, lcd);

	return;

}


void RunModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd)
{
UINT			i;
UINT			par;
CurCfg_typ *	curCfg;
curCfg				= m->pCurCfg;

	if (!curCfg)
		return;


 //if((Weld_Report_Status != 1) ||  ((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0)){
 if(((curCalib->GenSetup[SETUP_EXTRAS]&0x1) == 0)){
 	
	switch (intrf->GlblBtnCmd[0])
	{
		case ANDR_BTN_ARC_AMPS_NEG:		


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#1 JOG REV");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_UD].PROC_Indx,(-1.0));
							
			}else{
				
				strcpy(hmi->ActiveBanner, " ARC AMPS minus ");
				
				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parTIG_AmpSP , -1 );
			}	
			
		break;

		case ANDR_BTN_ARC_AMPS_POS:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#1 JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_UD].PROC_Indx,(1.0));
							
			}else{
				
				strcpy(hmi->ActiveBanner, " ARC AMPS plus ");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parTIG_AmpSP , 1 );
			}
		
		break;

		case ANDR_BTN_TRAVEL_SPEED_NEG:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#2 JOG REV");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_LR].PROC_Indx,(-1.0));
					
			}else{	
				
				strcpy(hmi->ActiveBanner, " TRAVEL SPEED minus ");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parTVL_VelSP , -1 );

			}
		
		break;

		case ANDR_BTN_TRAVEL_SPEED_POS:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				strcpy(hmi->ActiveBanner, "WB#2 JOG FWD");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.WB[WB_LR].PROC_Indx,(1.0));
					
			}else{
				
				strcpy(hmi->ActiveBanner, " TRAVEL SPEED plus");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parTVL_VelSP , 1 );

			}
			
		break;

		case ANDR_BTN_WIRE_SPEED_NEG:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				Hardware[1].Outp[9] = 1; // Lights OFF (NC) on camera
					
					
			}else{
			
				strcpy(hmi->ActiveBanner, "WIRE SPEED minus");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parWF_SpeedSP , -1 );


			}
		
		break;

		case ANDR_BTN_WIRE_SPEED_POS:


			if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
				Hardware[1].Outp[9] = 0; // Lights ON (NC) on camera
					
					
			}else{
			
				strcpy(hmi->ActiveBanner, "WIRE SPEED plus");

				if(!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parWF_SpeedSP , 1 );

			}
		
		break;

		case ANDR_BTN_AVC_VOLTS_NEG:


			if(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_Hold )
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].fParam[0] = m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress() * (-1.0);
				
			
			else if (m->TestMode){

				strcpy(hmi->ActiveBanner, " AVC JOG DOWN ");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.AVC.PROC_Indx, m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress() * (-1.0));


			}
			else{

				strcpy(hmi->ActiveBanner, "AVC VOLT minus");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parAVC_VoltSP , -1 );
				
			}

		break;

		case ANDR_BTN_AVC_VOLTS_POS:


			if(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_AVC_Hold )
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].fParam[0] = m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress();
			
			else if (m->TestMode){

				strcpy(hmi->ActiveBanner, " AVC JOG UP ");
				JogProcessAndroid(intrf, m->pCfg->HeadCfg.AVC.PROC_Indx, m->pCfg->HeadCfg.AVC.BtnJog_Gain * getBtnProgress());



			}
			else{

				strcpy(hmi->ActiveBanner, "AVC VOLT plus");

				if (!intrf->_intr._GlblBtnCmd[0])
					ChangeParam(parAVC_VoltSP , 1 );
				
			}

		break;

		case ANDR_BTN_OSC_SPEED_NEG:
			strcpy(hmi->ActiveBanner, "OSC	TIME minus");


			if(m->pCfg->HeadCfg.OSC.DwellDir){


				if (m->AndIntrf[0].GlblBtnLED[1] &0x2){					
					
					par = parOSC_Dwell_Out;
				}
				else if (m->AndIntrf[0].GlblBtnLED[1] &0x1){				
					
					par = parOSC_Dwell_In;
				}
				else 
					par = parOSC_Excur;



			}
			else{

				if (m->AndIntrf[0].GlblBtnLED[1] &0x1){					
					
					par = parOSC_Dwell_In;
				}
				else if (m->AndIntrf[0].GlblBtnLED[1] &0x2){				
					
					par = parOSC_Dwell_Out;
				}
				else 
					par = parOSC_Excur;

			}	


			

			if (!intrf->_intr._GlblBtnCmd[0])
			{
				DecrParam(curCfg, par, m->curSegm[ZONE], 1.0);

				if (par == parOSC_Excur)
					Check_parOSC_Excur(curCfg, hmi, m->curSegm[ZONE]);

				if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
					Fill_Segments(curCfg, par, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);


			}

		break;

		case ANDR_BTN_OSC_SPEED_POS:
			strcpy(hmi->ActiveBanner, "OSC	TIME plus");

			if(m->pCfg->HeadCfg.OSC.DwellDir){
			

				if (m->AndIntrf[0].GlblBtnLED[1] &0x2){ 				
					
					par = parOSC_Dwell_Out;
				}
				else if (m->AndIntrf[0].GlblBtnLED[1] &0x1){				
					
					par = parOSC_Dwell_In;
				}
				else 
					par = parOSC_Excur;



			}
			else{

				if (m->AndIntrf[0].GlblBtnLED[1] &0x1){ 				
					
					par = parOSC_Dwell_In;
				}
				else if (m->AndIntrf[0].GlblBtnLED[1] &0x2){				
					
					par = parOSC_Dwell_Out;
				}
				else 
					par = parOSC_Excur;

			}	

			
			if (!intrf->_intr._GlblBtnCmd[0])
			{
				IncrParam(curCfg, par, m->curSegm[ZONE], 1.0);

				if (par == parOSC_Excur)
					Check_parOSC_Excur(curCfg, hmi, m->curSegm[ZONE]);

				if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
					Fill_Segments(curCfg, par, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);

			}

		break;

		case ANDR_BTN_OSC_AMPL_NEG:
			strcpy(hmi->ActiveBanner, "OSC AMPLITUDE minus");

			if (!intrf->_intr._GlblBtnCmd[0])
			{
				DecrParam(curCfg, parOSC_Amplit, m->curSegm[ZONE], 1.0);

				Check_parOSC_Amplit(curCfg, hmi, m->curSegm[ZONE]);

				if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
					Fill_Segments(curCfg, parOSC_Amplit, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL], m->sm.Step);

			}

		break;

		case ANDR_BTN_OSC_AMPL_POS:
			strcpy(hmi->ActiveBanner, "OSC AMPLITUDE plus");

			if (!intrf->_intr._GlblBtnCmd[0])
			{
				IncrParam(curCfg, parOSC_Amplit, m->curSegm[ZONE], 1.0);

				Check_parOSC_Amplit(curCfg, hmi, m->curSegm[ZONE]);

			if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
				Fill_Segments(curCfg, parOSC_Amplit, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL],	m->sm.Step);

			}

		break;

		case ANDR_BTN_AVC_LOCK:
			strcpy(hmi->ActiveBanner, "AVC LOCK ON / OFF");

			if (curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != 2)
			{ // AVC Mode NOT Lock

				if (!intrf->_intr._GlblBtnCmd[0])
				{

					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0] =	!Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0]; // AVC Hold

				}

			}

		break;

	
		case ANDR_BTN_WIRE_DISABLE:
			strcpy(hmi->ActiveBanner, "WIRE ON/OFF");

			if (!intrf->_intr._GlblBtnCmd[0])
			{

				if (m->WireDisabled != 0)
					m->WireDisabled = 0;
				else 
					m->WireDisabled = 1;

				LogTxt(&LogMgr, "Wire Disable Event %d" , (DINT)m->WireDisabled);
			}

		break;
		

		default:
			
			for (i = 0; i < PROCESS_NUM; i++)
			Process[i].fParam[0] = 0.0;

		break;
	}

}
 else{

	switch (intrf->GlblBtnCmd[0])
	{
		case ANDR_BTN_WIRE_DISABLE:
			strcpy(hmi->ActiveBanner, "WIRE ON/OFF");

			if (!intrf->_intr._GlblBtnCmd[0])
			{

				if (m->WireDisabled != 0)
					m->WireDisabled = 0;
				else 
					m->WireDisabled = 1;

				LogTxt(&LogMgr, "Wire Disable Event %d" , (DINT)m->WireDisabled);
			}

		break;

		default:
			
			for (i = 0; i < PROCESS_NUM; i++)
			Process[i].fParam[0] = 0.0;

		break;

	}

 }


//	OScilation Center Jog	
if ((intrf->WheelDelta != 0) && (hmi->curHeadID == 0xFFFE))
{ // only jog on Run and Alternate Run screen

	if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Init)
	{

		if ((Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped) ||
		(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
		{

			switch (m->pCfg->HeadCfg.OSC.WhelJog_Mode1)
			{
				case WHL_JOG_MODE_SINGLE:
					
					JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,
					m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);
					
				break;

				case WHL_JOG_MODE_STEP:
					
					Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJOGpos;
					Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)
					intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain1;
				
				break;

				case WHL_JOG_MODE_DUAL:
					
					if (m->ToolHeadID)
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);
					else 
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta * (-1.0));

				break;

				case WHL_JOG_MODE_DUAL_INV:
					
					if (m->ToolHeadID)
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta);
					else 
						JogProcessAndroid(intrf, m->pCfg->HeadCfg.OSC.PROC_Indx,m->pCfg->HeadCfg.OSC.WheelJog_Gain1 * (float) intrf->WheelDelta * (-1.0));

				break;
				}

			
		}
		else 
		{

			Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsJOGpos;

			switch (m->pCfg->HeadCfg.OSC.WhelJog_Mode2)
			{
				case WHL_JOG_MODE_SINGLE:
				case WHL_JOG_MODE_STEP:
					
					Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
				break;

				case WHL_JOG_MODE_DUAL:
					
					if (m->ToolHeadID)
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
					else 
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2 * (-1.0);

				break;

				case WHL_JOG_MODE_DUAL_INV:
					
					if (!m->ToolHeadID)
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2;
					else 
						Process[m->pCfg->HeadCfg.OSC.PROC_Indx].fParam[0] = (float)intrf->WheelDelta * m->pCfg->HeadCfg.OSC.WheelJog_Gain2 * (-1.0);

				break;
				}

			}

		}
	}

}


void RunCalibrationModeAndroidButtons(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf, LCD_MGR_obj * lcd)
{
	StopModeAndroidButtons(m, hmi, intrf, lcd);
	return;
}



DINT CalibrationSequence(MASTER_typ * m, HMI_typ * hmi, plcbit StartBtn, plcbit StopBtn)
{
UINT			i, status;
DINT			ret;

//Cfg_typ *cfg;
CurCfg_typ *	curCfg;
CurCalib_typ *	curCalib, *defCalib;

	//Scaled_Var_obj *var;
	//cfg = (Cfg_typ*) m->pCfg;
	curCfg	= (CurCfg_typ *)m->pCurCfg;
	curCalib = (CurCalib_typ *)	m->pCalib;
	defCalib = (CurCalib_typ *)	m->pDefaultCalib;

	//var = (Scaled_Var_obj*) m->pScaledVar;
	///////////////////////////////	
	// Button Start and Stop
	ret = 1; // Stop

	switch (m->CalibType)
	{
	case CALIB_TVL:
		
		if ((Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_Stopped) ||
		(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
		{

			if (StartBtn)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsCalib;

				

		}
		

		if(StopBtn)
			CalibrTMR.IN = 0;
		
		else if (StartBtn){
				
			if(Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status == prcsSTATUS_ForceCalib){

				if(!CalibrTMR.IN)
					TimerStart(&CalibrTMR, 600.0);

			}
			
		}

		switch (hmi->CalibStep)
		{
			
			case 1:
			case 2: // Low Vel
			
				//if(StartBtn || KK_Functionality){
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[1] &0x2)
				{

					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTVL_CalibVel1].val[0], &curCalib->Points[CALIB_TVL_SPEED_CMD], CALIB_2POINTS);
					hmi->CalibPoint[hmi->CalibStep] = 0;
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1];

				}
				else 
				{

					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTVL_CalibVel1].val[0], &curCalib->Points[CALIB_TVL_SPEED_CMD], CALIB_2POINTS_4OFF);
					hmi->CalibPoint[hmi->CalibStep] = 4;
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1];

				}

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			case 3:
			case 4:
				
				// High Vel
				//if(StartBtn || KK_Functionality){
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[1] &0x2)
				{

					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTVL_CalibVel2].val[0], &curCalib->Points[CALIB_TVL_SPEED_CMD], CALIB_2POINTS);
					hmi->CalibPoint[hmi->CalibStep] = 2;
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1];

				}
				else 
				{
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTVL_CalibVel2].val[0], &curCalib->Points[CALIB_TVL_SPEED_CMD], CALIB_2POINTS_4OFF);
					hmi->CalibPoint[hmi->CalibStep] = 6;
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1];
				}

				//}
				if (KK_ManualCalib)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			default:
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].fParam[1] = 0.0;
			break;
		}

	break;

	///////////////////////////////////////////
	case CALIB_WF:
		
		if((Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped) ||
		(Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
		{
			if (StartBtn)
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsCalib;
		}

		if(TimerIsTimeOut(&CalibrTMR))
		{
			CalibrTMR.IN = 0;
			CalibrTMR.Q = 0;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
		}

		switch (hmi->CalibStep)
		{
		
			case 1:
			case 3: // Low Vel
			
				if (StartBtn)
					TimerStart(&CalibrTMR, curCfg->Param[parGEN_CalibTime1].val[0]);

				//if(StartBtn || KK_Functionality){
				if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1])
				{
					hmi->CalibPoint[hmi->CalibStep] = 0;
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parWF_CalibVel1].val[0], &curCalib->Points[CALIB_WF_SPEED_CMD], CALIB_2POINTS);
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1];
				}
				else 
				{
					hmi->CalibPoint[hmi->CalibStep] = 4;
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parWF_CalibVel1].val[0], &curCalib->Points[CALIB_WF_SPEED_CMD], CALIB_2POINTS_4OFF);
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1];
				}

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			case 2:
			case 4: // High Vel
			
				if (StartBtn)
					TimerStart(&CalibrTMR, curCfg->Param[parGEN_CalibTime2].val[0]);

				//if(StartBtn || KK_Functionality){
				if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[1])
				{
					hmi->CalibPoint[hmi->CalibStep] = 2;
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parWF_CalibVel2].val[0], &curCalib->Points[CALIB_WF_SPEED_CMD], CALIB_2POINTS);
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1];
				}
				else 
				{
					hmi->CalibPoint[hmi->CalibStep] = 6;
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parWF_CalibVel2].val[0], &curCalib->Points[CALIB_WF_SPEED_CMD], CALIB_2POINTS_4OFF);
					hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1];
				}

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			default:
				
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].fParam[1] = 0.0;
				
			break;
		}

	break;

	//////////////////////////////////////////////////////////////////
	case CALIB_TIG_PS:
		
		if ((Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status == prcsSTATUS_Stopped) ||
		(Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
		{
			if (StartBtn){

				switch(curCfg->Param_ENUM[parENUM_TIG_Cal_Mode].val[0]){


					case 1: // Load Bank
						
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsCalib;
				
					break;

					default: // Arc start

						if(AmpsCalib_step == 0)													
							AmpsCalib_step = 1;

					break;


				}
				
				



			}
		}


		switch(AmpsCalib_step){


			 case 0:

			 break;


			 case 1:


			 	if (ErrorMgr.Error){
					 AmpsCalib_step = 100;
					 break;

			 	}

				else if(StopBtn){
					
					AmpsCalib_step = 10;
					break;
				}
				

			 	if (!m->TestMode)
				{
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTART;
					Process[m->pCfg->HeadCfg.COOL.PROC_Indx].xCMD = prcsSTART;
				}

				if (m->TestMode)
				{				
					
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 1;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
					
				}
				else 
				{

					if ((Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status == prcsSTATUS_Run) &&
					(Process[m->pCfg->HeadCfg.COOL.PROC_Indx].Status == prcsSTATUS_Run))
					{
						Process[m->pCfg->HeadCfg.AVC.PROC_Indx].TestMode = 0;						
						Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsCalib;

					}
					else 
						break;

				}

				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsSTART;

				AmpsCalib_step++;


			 break;


			 case 2:

		 	
				if (ErrorMgr.Error){
					 AmpsCalib_step = 100;
					 break;

			 	}

				else if(StopBtn){
					
					AmpsCalib_step = 10;
					break;
				}
				 
				 if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status == prcsSTATUS_Run)
				 {
	 
	 				/* AVC */
					 Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsSTART;
					 Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Param[0] = 1; // Start without ramp 
					 
					 /*  TVL */
					 Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[2] = 0; // No Auto stop
					 Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsSTART;

					AmpsCalib_step++;
				 }
				 
			 break;


			 case 3:

			 	if (ErrorMgr.Error){
					
					 AmpsCalib_step = 100;
					
			 	}

				else if(StopBtn){
					
					AmpsCalib_step = 10;
				}


			 break;


			 case 10: // Stop sequence


				if (ErrorMgr.Error){
					 AmpsCalib_step = 100;
					 break;

			 	}
			 	
				Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsQuickSTOP;
				Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;
				Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
				
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;

				AmpsCalib_step++;
			 	
				

			 break;


			 case 11:


			 	if (ErrorMgr.Error){
					 AmpsCalib_step = 100;
					 break;

			 	}

				else if(StopBtn){
					
					AmpsCalib_step = 100;
					break;
				}


				status = 1;
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
					status = 0;

				if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
					status = 0;

				if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
					status = 0;

				
				if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
					status = 0;

				//if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
					//status = 0;

				if(status)								
					AmpsCalib_step = 0;

			 break;


			 
			case 100:
		
		
				if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
				   Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;

				if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
				   Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;

				if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
				   Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;


				if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
				   Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;  


				if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
				   Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsSTOP;


				AmpsCalib_step = 0;


			break;


			default:
				
				AmpsCalib_step = 0;

			break;


		}

		switch (hmi->CalibStep)
		{
			case 1:
				//if(StartBtn || KK_Functionality){
				hmi->CalibPoint[hmi->CalibStep] = 0;
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTIG_CalibAmp1].val[0], &curCalib->Points[CALIB_TIG_AMPS_CMD],	CALIB_2POINTS);
				hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1];

				//}
				if(KK_ManualCalib)
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			case 2:
				//if(StartBtn || KK_Functionality){
				hmi->CalibPoint[hmi->CalibStep] = 2;
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1] =	getFromCalib(curCfg->Param[parTIG_CalibAmp2].val[0], &curCalib->Points[CALIB_TIG_AMPS_CMD],	CALIB_2POINTS);
				hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1];

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			default:
				
				Process[m->pCfg->HeadCfg.TIG.PROC_Indx].fParam[1] = 0.0;
				
			break;
		}

	break;

	//////////////////////////////////////////////////////////////////
	case CALIB_HW_PS:
		if ((Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status == prcsSTATUS_Stopped) ||
		(Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
		{
			if (StartBtn)
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsCalib;
		}

		switch (hmi->CalibStep)
		{
			case 1:
				//if(StartBtn || KK_Functionality){
				hmi->CalibPoint[hmi->CalibStep] = 0;
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parHW_CalibAmp1].val[0], &curCalib->Points[CALIB_HW_AMPS_CMD], CALIB_2POINTS);
				hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1];

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			case 2:
				//if(StartBtn || KK_Functionality){
				hmi->CalibPoint[hmi->CalibStep] = 2;
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1] = getFromCalib(curCfg->Param[parHW_CalibAmp2].val[0], &curCalib->Points[CALIB_HW_AMPS_CMD], CALIB_2POINTS);
				hmi->CalibPointCmd[hmi->CalibStep] = Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1];

				//}
				if (KK_ManualCalib)
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1] = KK_ManualCalib_set_value;

			break;

			default:
				
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].fParam[1] = 0.0;
				
			break;
		}

	break;

	///////////////////////////////////////////		
	}

	status = 1;

	if (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.TIG.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;

	if (Process[m->pCfg->HeadCfg.GAS.PROC_Indx].Status != prcsSTATUS_Stopped)
		status = 0;
	
		//intrf->GlblBtnLED[0] += BTN_B0; //SEQ_STOP_LED 
		if (StopBtn)
		{ // Stop Button

			if (!status)
			{

				if(AmpsCalib_step == 0){ // No TIG calibration in ARC mode
					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.OSC.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.AVC.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.TIG.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsHalt;
					Process[m->pCfg->HeadCfg.GAS.PROC_Indx].xCMD = prcsHalt;	

				}

			}

		}
		ret = 2;	

	TimerExecute(&CalibrTMR);

	if (m->CalibType == CALIB_NONE)
		ret = 0;

	return ret;
}


void IncrParam(CurCfg_typ * curCfg, UINT param, DINT segm, float gain)
{
int i;

	if (curCfg->Param[param].segmYes)
	{
		i = segm;

		if (i > SEGM_NUM_MAX)
			i = SEGM_NUM_MAX;
	}
	else 
		i = 0;

	curCfg->Param[param].val[i] += curCfg->Param[param].incr * gain;

	if (curCfg->Param[param].val[i] > curCfg->Param[param].max)
		curCfg->Param[param].val[i] = curCfg->Param[param].max;

	return;

}


void DecrParam(CurCfg_typ * curCfg, UINT param, DINT segm, float gain)
{
	int i;


	if (curCfg->Param[param].segmYes)
	{
		i = segm;

		if (i > SEGM_NUM_MAX)
			i = SEGM_NUM_MAX;
	}
	else 
		i = 0;

	curCfg->Param[param].val[i] -= curCfg->Param[param].incr * gain;

	if (curCfg->Param[param].val[i] < curCfg->Param[param].min)
		curCfg->Param[param].val[i] = curCfg->Param[param].min;

	return;
}


void Check_parOSC_Amplit(CurCfg_typ * curCfg, HMI_typ * hmi, UINT seg)
{
float x;
UINT _seg;

	if(curCfg->Param[parOSC_FinalAmplit].segmYes)
		_seg = seg;
	else
		_seg = 0; 

		
	if(curCfg->Param[parOSC_Amplit].val[seg] < curCfg->Param[parOSC_FinalAmplit].val[_seg] ){

		curCfg->Param[parOSC_Amplit].val[seg] = curCfg->Param[parOSC_FinalAmplit].val[_seg];

		strcpy(hmi->pMessage[0], " OSC Primery	 ");
		strcpy(hmi->pMessage[1], " Amplitude ");
		strcpy(hmi->pMessage[2], " smaller then ");
		strcpy(hmi->pMessage[3], " Final Amplitude");
		TimerStart(&hmi->MsgTmr, 1.5);


	}

	if(curCfg->Param[parOSC_InitAmplit].segmYes)
		_seg = seg;
	else
		_seg = 0; 

	if(curCfg->Param[parOSC_Amplit].val[seg] < curCfg->Param[parOSC_InitAmplit].val[_seg] ){

		curCfg->Param[parOSC_Amplit].val[seg] = curCfg->Param[parOSC_InitAmplit].val[_seg];

		strcpy(hmi->pMessage[0], " OSC Primery	 ");
		strcpy(hmi->pMessage[1], " Amplitude ");
		strcpy(hmi->pMessage[2], " smaller then ");
		strcpy(hmi->pMessage[3], " Init Amplitude");
		TimerStart(&hmi->MsgTmr, 1.5);


	}

	if (curCfg->Param[parOSC_Excur].val[seg] > 0)
	{

		x = curCfg->Param[parOSC_Amplit].val[seg] / curCfg->Param[parOSC_Excur].val[seg];

		if (x > curCfg->Param[parOSC_MaxTvlVel].val[0])
		{
			curCfg->Param[parOSC_Amplit].val[seg] =
			curCfg->Param[parOSC_MaxTvlVel].val[0] *curCfg->Param[parOSC_Excur].val[seg];

			strcpy(hmi->pMessage[0], " OSC	 ");
			strcpy(hmi->pMessage[1], " Amplitude ");
			strcpy(hmi->pMessage[2], " was");
			strcpy(hmi->pMessage[3], " too high");
			TimerStart(&hmi->MsgTmr, 1.5);
		}

	}
}


void Check_parOSC_Excur(CurCfg_typ * curCfg, HMI_typ * hmi, UINT seg)
{
float x;

	if ((curCfg->Param[parOSC_Excur].val[seg] > 0) && (curCfg->Param[parOSC_MaxTvlVel].val[0] > 0))
	{
		x = curCfg->Param[parOSC_Amplit].val[seg] / curCfg->Param[parOSC_Excur].val[seg];

		if (x > curCfg->Param[parOSC_MaxTvlVel].val[0])
		{
			curCfg->Param[parOSC_Excur].val[seg] =
			curCfg->Param[parOSC_Amplit].val[seg] / curCfg->Param[parOSC_MaxTvlVel].val[0];

			strcpy(hmi->pMessage[0], " OSC	 ");
			strcpy(hmi->pMessage[1], " Excurtion ");
			strcpy(hmi->pMessage[2], " was");
			strcpy(hmi->pMessage[3], " too small");
			TimerStart(&hmi->MsgTmr, 1.5);
		}

	}
	return;
}


void Fill_Segments(CurCfg_typ * curCfg, UINT par, UINT seg, UINT Mode, UINT Step)
{
float x;
int i;

	if (!curCfg->Param[par].segmYes)
		return;

	switch (Mode)
	{
		case 1:
			
			for (i = seg; i < SEGM_NUM_MAX; i++)
				curCfg->Param[par].val[i] = curCfg->Param[par].val[seg];

		break;

		case 2:
			
			if (Step == 51)
			{ // RUN
				for (i = seg; i < SEGM_NUM_MAX; i++)
					curCfg->Param[par].val[i] = curCfg->Param[par].val[seg];
			}
			
		break;

		default:
			// OFF
		break;
	}

	return;
}


void SetBannerTxt(MASTER_typ * m, HMI_typ * hmi, HMI_INTRF_typ * intrf)
{

	if (!m || !hmi || !intrf)
	return;

	if (!hmi->pBanner)
	return;


	if(m->sm.Step == 251){ // Head was changed

		strcpy(hmi->pBanner ,"Head Setup Changed - Must Restart!!!");
		
		return;

	}

	
	if(TVL_MAP_Step > 0)
		strcpy(hmi->pBanner ,"REC. ");
	else {

		if((curCalib->GenSetup[SETUP_EXTRAS]&0x1) != 0)
			strcpy(hmi->pBanner ,"Sup.//");
		else
			strcpy(hmi->pBanner ," ");

	}


	if((curCalib->GenSetup[SETUP_EXTRAS]&0x1) != 0){

		if(Weld_Report_Status == 2)
			strcat(hmi->pBanner , "W:Report !!!  ");

	}	
	
	//strcpy(hmi->pBanner, "");

#ifdef UNIV_SYSTEM
	if (!Hardware[1].CouplerOK[0])
	{

#ifndef TEST_ONLY_PLC_ANDROID
		strcat(hmi->pBanner, "Ext. Suitcase Err ");
		return;

#endif

	}
	else 
#endif


	if(m->sm.Step == 999){
		xsprintf(hmi->pBanner + strlen(hmi->pBanner)," TML Ref. Setup Finished - Restart Power ");
		return;
	}
	
	else if(m->sm.Step == 1000){
		xsprintf(hmi->pBanner + strlen(hmi->pBanner),"TML Setup Fin. Restart or Start New One ");
		return;
	}	
	else if(m->sm.Step == 1001){
		xsprintf(hmi->pBanner + strlen(hmi->pBanner),"New Head Selected Restart Power ");
		return;
	}		
	else if ((m->sm.Step >= 230) && (m->sm.Step <= 236)){

		switch (m->sm.Step)
		{	

			
			case 230:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Clear ESTOP to Refresh TML Setup ");
				
			break;

			case 231:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Press START for TML Check ");
				
			break;

			case 232:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Wait!! Checking TML  %3.1f",  ((float) (m->sm.Tmr1.PT - m->sm.Tmr1.ET)) / 1000.0);
				
			break;

			case 233:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Press START Btn for %s  Setup Download " , TML_MTR_INST[curTML_indx].Name);
				
			break;

			
			case 234:
			case 235:
			case 236:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Wait!! TML Setup %s %d %3.1f", TML_MTR_INST[curTML_indx].Name, TML_MTR_INST[curTML_indx].xCMD, (float) m->sm.Tmr1.ET / 1000.0);
			
			break;

			default:
				
				xsprintf(hmi->pBanner + strlen(hmi->pBanner), "Wait!! Checking TML");
			
			break;
		}

		return;

	}	
	else if ((VectorStateMach[VECT_TVL].FbkVect.Mode == vectSt_Mode_Vel) && VectorStateMach[VECT_TVL].FbkWarning)
	{
		if(VectorStateMach[VECT_TVL].FbkWarning == 2)
			strcat(hmi->pBanner, "Max Cmd TVL - slow down !!!");
		else
			strcat(hmi->pBanner, "TVL Limits are reached !!!");
		
		return;

	}
	else if ((ErrorMgr.Error != 0) && (!m->LOCKED))
	{

		switch (ErrorMgr.Error)
		{
			case ERROR_FATAL:
				strcat(hmi->pBanner, "FATAL ERR MUST RBT !!!");
			break;

			default:
				if((m->sm.Step == 191)
					||(m->sm.Step == 200)) // waiting ESTOP recovery
					strcat(hmi->pBanner, "Do NOT Hold BUTTON when clear error !!!");
				else	
					strcat(hmi->pBanner, "	ERROR  !!!");
			break;
		}

		return;
	}
	else if (m->ESTOP)
	{

		if(m->pESTOP_Out){

			if(*m->pESTOP_Out)
				strcat(hmi->pBanner, " ESTOP !!! ");
			else
				strcat(hmi->pBanner, " ESTOP *** ");
				
		}
		else			
			strcat(hmi->pBanner, " ESTOP !!! ");
		
		return;

	}
	else if (intrf->GlblBtnCmd[0])
	{

		strcat(hmi->pBanner, hmi->ActiveBanner);

		return;

	}

	switch (m->sm.Step)
	{

		case 1:

			xsprintf(hmi->pBanner , "Check TML Ax: %d  ", (DINT)TML_Restart_Indx);


		break;


		case 49:

			strcat(hmi->pBanner, "Wait for ArcLink");			

		break;


		case 50:
			
			if ((Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Stopped) ||
			(Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_DirectCntrl))
			{
				strcat(hmi->pBanner, "  STOP ");
				
				if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 2)
					strcat(hmi->pBanner, "Cont.");	
				else if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 1)
					strcat(hmi->pBanner, "Cont. One Way");
					
			}
			else if (Process[m->pCfg->HeadCfg.OSC.PROC_Indx].Status == prcsSTATUS_Run)
				strcat(hmi->pBanner, " OSC Test ");

		break;

		case 51:
			
			switch (m->SeqStep)
				{
				case 1:
					
					xsprintf(hmi->pBanner + strlen(hmi->pBanner), "START !!! %3.1f ",((float) (m->sm.Tmr1.PT - m->sm.Tmr1.ET)) / 1000.0);
					
				break;

				case 2:
					
					strcat(hmi->pBanner, " Wait ARC ");
				
				break;

				case 3:
					
					strcat(hmi->pBanner, " RUN ");

					

					if(curCalib->GenSetup[SETUP_CONTIN_WELD]> 0){					
						

						if(Cont_Stop_By_End)
							strcat(hmi->pBanner, "To STOP !!");	
						else {

							if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 2)
								strcat(hmi->pBanner, "Cont.");	
							else 
								strcat(hmi->pBanner, "Cont. One Way");

						}					
								

					}
					
				break;

				case 150:
					
					xsprintf(hmi->pBanner + strlen(hmi->pBanner), "REWRAP !!! %3.1f ",	((float) (m->sm.Tmr1.PT - m->sm.Tmr1.ET)) / 1000.0);
					
				break;

				case 201:
					
					switch (Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Status)
					{
						case prcsSTATUS_StopDly:
							strcat(hmi->pBanner, "TVL Stop Dly");
						break;

						case prcsSTATUS_RampDwn:
							strcat(hmi->pBanner, "TVL Ramp Dwn");
						break;

						case prcsSTATUS_ReturnDly:
							strcat(hmi->pBanner, "TVL Ret Dly");
						break;

						case prcsSTATUS_Return:
							strcat(hmi->pBanner, "TVL Return");
						break;

						default:
							strcat(hmi->pBanner, "Ramp Down");
						break;
					}

				break;


				case 202:

					strcat(hmi->pBanner, "Wait Index");
					
				break;

				case 203:
					
					strcat(hmi->pBanner, "Wait Head Return");
					
				break;

				case 205:
					
					strcat(hmi->pBanner, "Wait New File Load");
					
				break;

				case 210:
					
					strcat(hmi->pBanner, "Going to Start Pos.");

				break;

				case 220:
					
					xsprintf(hmi->pBanner + strlen(hmi->pBanner), "AUTO START !!! %3.1f ",	((float) (m->sm.Tmr1.PT - m->sm.Tmr1.ET)) / 1000.0);

				break;

				case 255:
					
					strcat(hmi->pBanner, "Wait OSC Stop");
					
				break;

				default:
					
					strcat(hmi->pBanner, " Stopping ");
					if(curCalib->GenSetup[SETUP_CONTIN_WELD]> 0){

						if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 2)
							strcat(hmi->pBanner, "Cont.");	
						else 
							strcat(hmi->pBanner, "Cont. One Way");
						
						//strcat(hmi->pBanner, "Cont.");	


					}
					
				break;
			}

		break;

		case 100:
			/*
			if(hmi->curHeadID < 0xFFFE)			
				strcat (hmi->pBanner , hmi->ActiveBanner);
			else */
			strcat(hmi->pBanner, " Calib.STOP ");
			
		break;

		case 101:
			/*if(hmi->curHeadID < 0xFFFE)			
				strcat (hmi->pBanner , hmi->ActiveBanner);
			else */
			strcat(hmi->pBanner, " Calib.RUN ");
			
		break;

		case 220:
			
			strcat(hmi->pBanner, " TML Firm. ");
		
		break;

		case 230:
			
			strcpy(hmi->pBanner, " NEW HEAD CONFIGURATION was Selected");
		
		break;

		default:
			
			//strcat(hmi->pBanner, " INIT ");
			strcat(hmi->pBanner, m->sm.Text);
		
		break;
	}

	switch (m->SubSeqStep)
	{
		default:
		break;

		case 1:
			strcat(hmi->pBanner, " >>T");
		break;

		case 2:
			strcat(hmi->pBanner, " >>R");
		break;
	}

	/*
	if(intrf->TestMode) 	
	strcat(hmi->pBanner , " TEST");

	if(m->WireDisabled) 	
	strcat(hmi->pBanner , "/WIRE OFF");

	if(Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Param[0])
	strcat(hmi->pBanner , "/LOCKED");
	*/
	
	return;

}


float getBtnProgress()
{

	return (float) (btnProgress + btnProgressBias) / (float) (btnProgressMax + btnProgressBias);

}


void SetZeroPosition(MASTER_typ * m)
{

	if (TVL_ROT_Param_Set[0].pEncoder){
		OLD_MTR_POS[TVL_ZONE_HOME] = *TVL_ROT_Param_Set[0].pEncoder;

		Process[PROC_WRST].xCMD = prcsRetract;

	}

	

return;

}


void CreateParamList()
{
UINT i;

	i = 0;
	ParamList.Index[i++] = parGEN_WeldProgramRepeat;

	// All parameters from segment editor	
	ParamList.Index[i++] = parTIG_AmpSP;	
	ParamList.Index[i++] = parTIG_AmpInSP;
	ParamList.Index[i++] = parTIG_AmpOutSP;
	ParamList.Index[i++] = parTIG_BackAmp;
	ParamList.Index[i++] = parTIG_PulseFreq;
	ParamList.Index[i++] = parTIG_PulseWidth;
	
	ParamList.Index[i++] = parAVC_VoltSP;
	ParamList.Index[i++] = parAVC_VoltInSP;
	ParamList.Index[i++] = parAVC_VoltOutSP;
	ParamList.Index[i++] = parAVC_VoltBackSP;

	ParamList.Index[i++] = parTVL_VelSP;
	ParamList.Index[i++] = parTVL_VelInSP;
	ParamList.Index[i++] = parTVL_VelOutSP;
	ParamList.Index[i++] = parTVL_VelBackSP;
	
	ParamList.Index[i++] = parWF_SpeedSP;
	ParamList.Index[i++] = parWF_SpeedInSP;
	ParamList.Index[i++] = parWF_SpeedOutSP;
	ParamList.Index[i++] = parWF_BkgSpeedSP;
	
	ParamList.Index[i++] = parWP_SpeedSP;
	ParamList.Index[i++] = parWP_SpeedInSP;
	ParamList.Index[i++] = parWP_SpeedOutSP;
	ParamList.Index[i++] = parWP_BkgSpeedSP;
	
	ParamList.Index[i++] = parHW_AmpSP;
	ParamList.Index[i++] = parOSC_Amplit;
	ParamList.Index[i++] = parOSC_Excur;
	ParamList.Index[i++] = parOSC_Dwell_In;
	ParamList.Index[i++] = parOSC_Dwell_Out;


	// All parameters  from Timing Calculation
	ParamList.Index[i++] = parAVC_Upslope;
	ParamList.Index[i++] = parAVC_StopDly;
	ParamList.Index[i++] = parAVC_DownSlope;

	ParamList.Index[i++] = parTVL_StartDly;
	ParamList.Index[i++] = parTVL_RampUp;
	ParamList.Index[i++] = parTVL_StopDly;
	ParamList.Index[i++] = parTVL_RampDwn;
	ParamList.Index[i++] = parTVL_RewrapDly;
	ParamList.Index[i++] = parTVL_RunDist;
	ParamList.Index[i++] = parTVL_OverlapDist;


	ParamList.Index[i++] = parWF_StartDly;
	ParamList.Index[i++] = parWF_RampUp;
	ParamList.Index[i++] = parWF_StopDly;
	ParamList.Index[i++] = parWF_RampDwn;
	ParamList.Index[i++] = parWF_RetractDly;

	ParamList.Index[i++] = parTIG_StartDly;
	ParamList.Index[i++] = parTIG_RampUp;
	ParamList.Index[i++] = parTIG_StopDly;
	ParamList.Index[i++] = parTIG_RampDwn;

	ParamList.Index[i++] = parOSC_StartDly;
	ParamList.Index[i++] = parOSC_RampUp;
	ParamList.Index[i++] = parOSC_StopDly;
	ParamList.Index[i++] = parOSC_RampDwn;

	ParamList.Index[i++] = parWP_StartDly;
	ParamList.Index[i++] = parWP_RampUp;
	ParamList.Index[i++] = parWP_StopDly;
	ParamList.Index[i++] = parWP_RampDwn;

	ParamList.Index[i++] = parHW_RampUpDly;
	ParamList.Index[i++] = parHW_RampUp;
	ParamList.Index[i++] = parHW_RampDwnDly;
	ParamList.Index[i++] = parHW_RampDwn;
	ParamList.Index[i++] = parGAS_PostPurgeTm;

	ParamList.IndexNum	= i;
	
	i = 0;
	ParamList.Index_Enum[i++] = parENUM_WeldProgramLinkMode;
	ParamList.Index_Enum[i++] = parENUM_PathMode;
	//ParamList.Index_Enum[i++] = parENUM_WeldProgramCase;

	ParamList.IndexNum_Enum = i;

	i = 0;
	ParamList.Index_Int[i++] = parINT_NumZone;

	ParamList.IndexNum_Int = i;

}


void DeleteExtension(char * p)
{
UINT len, i;

	len = strlen(p);

	for (i = 0; i < len; i++)
	{
		if (p[i] == '.')
		{
			p[i]= 0;
			break;
		}
	}

	return;

}


void WireDisableInable(plcbit Tmr_Q)
{
	if (!m->WireDisabled)
	{

		if (Tmr_Q)
		{
			if ((Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Stopped)
				|| (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Retract)
				|| (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_RampDwn)
				|| (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_StopDly)){


					switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x4){

						case 0:  // No Ramps

							Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsJump2Run;

						break;

						default:
							
							Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 1; // Skip all delays - regular ramps
							Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsQuickStart;

						break;
					}

			}

			if ((Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped)
				|| (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Retract)
				|| (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_RampDwn)
				|| (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_StopDly)){

					switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x4){

						case 0:  // No Ramps

							Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsJump2Run;

						break;

						default:
							
							Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 1; // Skip all delays - regular ramps
							Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickStart;

						break;
					}

			}
				
			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Run)
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsJump2Run;

		}
		else 
		{
			
			if(Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;

		}

	}
	else 
	{
		
		if ((Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Run)
			|| (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_StartDly)
			|| (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_RampUp)){


			switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x3){



				case 0x0: // No ramp , no retract
				case 0x1: // No Ramp , Retract

					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
				
				break;

				
				case 0x2: // Ramp , No Retract
				case 0x3: // Ramp , Retract

					Process[m->pCfg->HeadCfg.WP.PROC_Indx].Param[0] = 1; // Skip all delays , use ramp
					Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsQuickSTOP;

				break;
				
			}
		}		

		
		if ((Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Run)
			|| (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_StartDly)
			|| (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_RampUp)){
		
			switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x3){


				case 0x0: // No ramp , no retract

					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			
				break;

				case 0x1: // No Ramp , Retract

					Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 3; // Skip all delays , ramps , only retract
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickSTOP;
					
				break;

				case 0x2: // Ramp , No Retract

					Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 4; // Skip all delays , ramps , only retract
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickSTOP;

				break;

				case 0x3: // Ramp , Retract

					Process[m->pCfg->HeadCfg.WF.PROC_Indx].Param[0] = 1; // Skip all delays , use ramp
					Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickSTOP;

				break;			

				
			}

		}


		if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsQuickSTOP;

	}

	return;
}


/*
prcsSTATUS_Retract
prcsSTATUS_RampDwn
prcsSTATUS_ReturnDly
prcsSTATUS_StopDly
*/

/* old one replaced 01 Apr2020
void WireDisableInable(plcbit Tmr_Q)
{
	if (!m->WireDisabled)
	{

		if (Tmr_Q)
		{
			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsJump2Run;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsJump2Run;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsJump2Run;

		}
		else 
		{
			
			if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsSTART;

			if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsSTART;

			if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status == prcsSTATUS_Stopped)
				Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTART;

		}

	}
	else 
	{
		// Nowe
		if (Process[m->pCfg->HeadCfg.WP.PROC_Indx].Status == prcsSTATUS_Run)
			Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsQuickSTOP;

		//Process[m->pCfg->HeadCfg.WP.PROC_Indx].xCMD = prcsHalt;
		if (Process[m->pCfg->HeadCfg.WF.PROC_Indx].Status != prcsSTATUS_Stopped) //Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsHalt;
			Process[m->pCfg->HeadCfg.WF.PROC_Indx].xCMD = prcsQuickSTOP;


		if (Process[m->pCfg->HeadCfg.HW.PROC_Indx].Status != prcsSTATUS_Stopped)
			Process[m->pCfg->HeadCfg.HW.PROC_Indx].xCMD = prcsSTOP;

	}

	return;
}

*/

void DumpParamToFile()
{

char tmpTxt[32];
int len;

	switch (parametersDump_Step)
	{
		case 0:

						
		break;

		case 1:
			
			if (File_RW.CMD == 0)
			{

				strcpy(File_RW.deviceName, "WeldPrograms");
				File_RW._int.wStatus = 0;

				DTGetTime_1.enable	= 1;
				DTGetTime(&DTGetTime_1);
				DT_TO_DTStructure(DTGetTime_1.DT1, (UDINT) (&DTStruct));

				xsprintf(File_RW.fileNewName, "%s_%d_%d_%d.csv", curCfg->CurProgramName, 
				DTStruct.hour, DTStruct.minute, DTStruct.second);

				File_RW._int.wStatus = 0;
				File_RW.CMD 		= F_OPEN_ONLY;
				parametersDump_Step++;

			}

		break;

		case 2:
			
			if (File_RW.CMD == 0)
			{

				File_RW.pMem		= (UDINT)
				LineTxt;
				File_RW.Offset		= 0;
				File_RW.Len 		= 0;
				
				//paramIndx			= parOSC_StartDly;				
				paramIndx			= parLCD_BtnMode;
				


				/* status (20708 -> File doesn't exist) */
				if (File_RW._int.wStatus == 20708)
				{
					File_RW.CMD 		= F_CREATE_ONLY;
					parametersDump_Step++;
				}
				else if (!File_RW.Error)
				{
					File_RW.CMD 		= F_DELETE;
				}
				else 
					parametersDump_Step = 0;

			}

		break;

		case 3:
			
			if (File_RW.CMD == 0)
			{
				ascDTStructure((UDINT) &DTStruct, tmpTxt, 25);

				xsprintf(LineTxt, " Program: %s -  Stored at: %s \n\n\r////// Parameters ///// \n\r  ",	curCfg->CurProgramName, tmpTxt);

				//ascDTStructure((UDINT) &DTStruct, LineTxt + strlen(LineTxt), 25);
				len = strlen(LineTxt);

				//LineTxt[len++]= 0x0D; //CR
				//LineTxt[len++]= 0x0D; // CR
				File_RW.Len 		= strlen(LineTxt);
				File_RW.CMD 		= F_WRITE_ONLY;
				parametersDump_Step++;

			}

		break;

		case 4:
			
			if (File_RW.CMD == 0)
			{

				if (paramIndx < parLAST_PARAM)
				{

					File_RW.Offset		+= File_RW.Len;

					if (strlen(cfg->ScaledVar[paramIndx].name) > 0)
					{
						xsprintf(LineTxt, "Par. %d ,   (%s) , [ %s ] = %s \n\r ", paramIndx, cfg->ScaledVar[paramIndx].enumTXT, cfg->ScaledVar[paramIndx].name, GetScaledVal(lcd, &cfg->ScaledVar[paramIndx], 0));
					}
					else 
						xsprintf(LineTxt, "Par. %d:///	available\n\r ", paramIndx);

					File_RW.Len 		= strlen(LineTxt);
					File_RW.CMD 		= F_WRITE_ONLY;
					paramIndx++;


				}
				else 
				{
					File_RW.Offset		+= File_RW.Len;

					xsprintf(LineTxt, "//////  Text Resources	///////// \n\r");
					File_RW.Len 		= strlen(LineTxt);
					File_RW.CMD 		= F_WRITE_ONLY;

					paramIndx			= 0;
					parametersDump_Step++;
				}

			}

		break;

		case 5:
			
			if (File_RW.CMD == 0)
			{

				if (paramIndx < TXT_RESOURCES_MAX)
				{
					if (strlen(cfg->TXT_Resources[paramIndx]) > 0)
					{

						File_RW.Offset		+= File_RW.Len;
						xsprintf(LineTxt, "Txt Res. %d , [ %s ] \n\r", paramIndx, cfg->TXT_Resources[paramIndx]);

						File_RW.Len 		= strlen(LineTxt);
						File_RW.CMD 		= F_WRITE_ONLY;

					}

					paramIndx++;
				}
				else 
				{
					File_RW.Offset		+= File_RW.Len;

					xsprintf(LineTxt, "//////	Error Text	 ///////// \n\r");
					File_RW.Len 		= strlen(LineTxt);
					File_RW.CMD 		= F_WRITE_ONLY;

					paramIndx			= 0;
					parametersDump_Step++;

				}

			}

		break;

		case 6:
			
			if (File_RW.CMD == 0)
			{
				if (paramIndx < 128)
				{
					if (strlen(ErrorMgr.logs[paramIndx].txt) > 0)
					{
						File_RW.Offset		+= File_RW.Len;
						xsprintf(LineTxt, "Err %d , [ %s ] \n\r", paramIndx, ErrorMgr.logs[paramIndx].txt);

						File_RW.Len 		= strlen(LineTxt);
						File_RW.CMD 		= F_WRITE_ONLY;
					}

						paramIndx++;
					}
				else 
				{

					paramIndx			= 0;
					parametersDump_Step = 10;

				}

			}

		break;

		case 10:
			
			File_RW.CMD = F_CLOSE_ONLY;
			parametersDump_Step = 0;
		
		break;

	}

}

void TVL_Map_ToFile()
{

char tmpTxt[32];
int len;

	

	switch (TVL_MAP_Step)
	{
		case 0:

			if(KK_HomeBtn)
				TVL_MAP_Step++;
			
		break;

		case 1:
			
			if (File_RW.CMD == 0)
			{

				strcpy(File_RW.deviceName, "TEMP");
				File_RW._int.wStatus = 0;

				DTGetTime_1.enable	= 1;
				DTGetTime(&DTGetTime_1);
				DT_TO_DTStructure(DTGetTime_1.DT1, (UDINT) (&DTStruct));

				xsprintf(File_RW.fileNewName, "TVL_MAP_%d_%d_%d.csv", DTStruct.hour, DTStruct.minute, DTStruct.second);

				File_RW._int.wStatus = 0;
				//File_RW.CMD 		= F_OPEN_ONLY;
				File_RW.CMD 		= F_CREATE_ONLY;
				//TVL_MAP_Step++;
				TVL_MAP_Step = 3;

			}

		break;

		case 2:
			
			if (File_RW.CMD == 0)
			{

				File_RW.pMem		= (UDINT)LineTxt;
				File_RW.Offset		= 0;
				File_RW.Len 		= 0;
				


				/* status (20708 -> File doesn't exist) */
				if (File_RW._int.wStatus == 20708)
				{
					File_RW.CMD 		= F_CREATE_ONLY;
					TVL_MAP_Step++;
				}
				else if (!File_RW.Error)
				{
					File_RW.CMD 		= F_DELETE;
				}
				else 
					TVL_MAP_Step = 0;

			}

		break;

		case 3:
			
			if (File_RW.CMD == 0)
			{

				File_RW.pMem		= (UDINT)LineTxt;
				File_RW.Offset		= 0;
				File_RW.Len 		= 0;
				
				ascDTStructure((UDINT) &DTStruct, tmpTxt, 25);

				xsprintf(LineTxt, " TVL Mapping : Stored at: %s \n\n\r Id , Tick , TVL Set Speed , Mtr Delta , Encoder Delta  \n\r  ", tmpTxt);
				len = strlen(LineTxt);

				File_RW.Len 		= strlen(LineTxt);
				File_RW.CMD 		= F_WRITE_ONLY;

				TVL_MAP_Indx[0]		= 0;
				TVL_MAP_Indx_New[0] = 0;
				TVL_MAP_Step++;

			}

		break;

		case 4:


			if(KK_StopBtn)
				TVL_MAP_Step = 10;
			
			else if (File_RW.CMD == 0)
			{
			

				if (TVL_MAP_Indx_New[0])
				{

					TVL_MAP_Indx_New[0] = 0;

					File_RW.Offset		+= File_RW.Len;		//TVL_MAP_Indx[0], m->curSegm[ZONE]			

					//xsprintf(LineTxt, " %d,%d, %4.2f ,%d,%d,%d\n ", TVL_MAP_Indx[0] ,actTicks , VectorStateMach[VECT_TVL].InVal ,  AxisCh[TML_AX_TVL].curPos , _deltaAPOS[0] , _deltaAPOS2[0]  );
					xsprintf(LineTxt, " %d,%d, %4.2f ,%d,%d\n ", TVL_MAP_Indx[0] ,actTicks , VectorStateMach[VECT_TVL].InVal  , _deltaAPOS[0] , _deltaAPOS2[0]  );
					//xsprintf(LineTxt, " %d,%d,%d,%d,%d\n\r ", TVL_MAP_Indx[0], m->curSegm[ZONE]  ,  AxisCh[TML_AX_TVL].curPos , _deltaAPOS[0] , _deltaAPOS2[0]  );
					File_RW.Len 		= strlen(LineTxt);
					File_RW.CMD 		= F_WRITE_ONLY;
					

				}
			}

		break;
		

		case 10:
			
			File_RW.CMD = F_CLOSE_ONLY;
			TVL_MAP_Step = 0;
		
		break;

	}

}



void EvaluatePosition()
{int type;


	
	 //VERSION with encoder

	//tickEncoder = (AxisCh[m->pCfg->HeadCfg.TVL.TML_Indx].curPos  - encHome);			
	tickEncoder = (VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].actPosDINT - OLD_MTR_POS[TVL_ZONE_HOME]);

		
	if(m->pCfg->HeadCfg.TVL.EncScale != 0.0){

		//ABS_TVL_Pos = (float)(tickEncoder * m->pCfg->HeadCfg.TVL.EncDenominator) / m->pCfg->HeadCfg.TVL.EncNominator;
		
		ABS_TVL_Pos = (float)((double)tickEncoder * m->pCfg->HeadCfg.TVL.EncScale);		

		// Tak bylo przed 4 June 2021 
		//actTicks = (DINT)ABS_TVL_Pos; 

		if(TVL_ROT_Param_Set[0].EncPerTick > 0)		
			actTicks = (DINT)((tickEncoder / TVL_ROT_Param_Set[0].EncPerTick)+0.5);
		else
			actTicks = (DINT)(ABS_TVL_Pos +0.5); 

		
		actTicks = actTicks * m->pCfg->HeadCfg.TVL.Enc_Dir;

	}
	else { //old code

		if(TVL_ROT_Param_Set[0].EncPerTick > 0)		
			actTicks = (DINT)(tickEncoder / TVL_ROT_Param_Set[0].EncPerTick);
			
		actTicks = actTicks * m->pCfg->HeadCfg.TVL.Enc_Dir;

		ABS_TVL_Pos = (float)tickEncoder / TVL_ROT_Param_Set[0].EncPerTick;


	}	

			


	if (m->ticksPerSegm < 1)
		m->ticksPerSegm = 1;

	if (m->ticksPerZone < 1)
		m->ticksPerZone = 1;

	if(m->pCfg->HeadCfg.TVL.PinsPerRot) // static configuration
		curCalib->GenSetup[SETUP_PIN_PER_ROT] = m->pCfg->HeadCfg.TVL.PinsPerRot;

	m->curSegm[ACT_TVL_DIST] = TVL_ROT_Param_Set[0].EncPos_FBK;

	if(curCalib->GenSetup[SETUP_PIN_PER_ROT] > 0)
		actRot = actTicks / curCalib->GenSetup[SETUP_PIN_PER_ROT]; 

	//m->curSegm[FR_SEGM] = (actTicks / m->ticksPerSegm);
	//m->curSegm[HEAD_SEGM] = m->curSegm[ACT_TVL_DIST] / m->ticksPerSegm;

	actTicksForZone = actTicks;
	//actTicksForZone = KK_ActEnc;

	// Zone Position depends on head direction
	if (!m->ToolHeadID){
		
	}
	else 
	{
		
		switch (curCalib->GenSetup[SETUP_TWO_TORCH])
		{
			default: // One Torch
				actTicksForZone = actTicksForZone *(-1);
			break;

			case 1: // Two Torches
				actTicksForZone = (curCalib->GenSetup[SETUP_PIN_PER_ROT] / 2) -actTicksForZone; // shift by 180 deg
			break;
		}

	}

	//////////////////////////////////////////////////

	switch(curCalib->GenSetup[SETUP_TWO_UP]){

		case 0: // One Up

			type = 0;


		break;


		case 1:

			if(curCalib->GenSetup[SETUP_ZONE_CENTR])
				type = 2; //  Gravity Centric
			else
				type = 1; // Start segm Centric


		break;


	}

	// KK - sprawdzic czy to cos pieprzy
	if(m->numZone <= 0)
		m->numZone = 1;

	if(curCalib->GenSetup[SETUP_PIN_PER_ROT] <= 0)
		curCalib->GenSetup[SETUP_PIN_PER_ROT] = 1;
		
	switch(type){

		case 0: // One Up
	
			m->ticksPerZone = (INT)(ceilf((float)curCalib->GenSetup[SETUP_PIN_PER_ROT]/(float)m->numZone));

			actTicksForZone = actTicksForZone%curCalib->GenSetup[SETUP_PIN_PER_ROT];
			if(actTicksForZone < 0)
				actTicksForZone += curCalib->GenSetup[SETUP_PIN_PER_ROT];

			m->curSegm[ZONE] = actTicksForZone / m->ticksPerZone;
			

		break;


		case 1: // Two up - Star Segment Centric

			m->ticksPerZone = (INT)(ceilf((float)curCalib->GenSetup[SETUP_PIN_PER_ROT]/((float)m->numZone * 2.0)));

				actTicksForZone = (actTicksForZone)%(curCalib->GenSetup[SETUP_PIN_PER_ROT]);
				if(actTicksForZone < 0)
					actTicksForZone += curCalib->GenSetup[SETUP_PIN_PER_ROT];

				m->curSegm[ZONE] = actTicksForZone / (m->ticksPerZone); 
							
				m->curSegm[ZONE_RAW] = m->curSegm[ZONE] ;
				
				if(m->curSegm[ZONE] >= m->numZone )
					m->curSegm[ZONE] = (m->numZone * 2 -1) - m->curSegm[ZONE];
				
		break;

		case 2: // Two up - Gravity Centric

			if(m->numZone < 2.0)
				m->curSegm[ZONE] = 1;
			
			else {
				m->ticksPerZone = (INT)(ceilf((float)curCalib->GenSetup[SETUP_PIN_PER_ROT]/((float)m->numZone * 2.0 - 2.0)));

				actTicksForZone = (actTicksForZone + m->ticksPerZone/2)%(curCalib->GenSetup[SETUP_PIN_PER_ROT]);
				if(actTicksForZone < 0)
					actTicksForZone += curCalib->GenSetup[SETUP_PIN_PER_ROT];

				m->curSegm[ZONE] = actTicksForZone / (m->ticksPerZone); 
				
				m->curSegm[ZONE_RAW] = m->curSegm[ZONE] ;
				
				if(m->curSegm[ZONE] >= m->numZone )
					m->curSegm[ZONE] = (m->numZone * 2 - 2) - m->curSegm[ZONE];  

			}
			
		break;

	}

	if (m->curSegm[ZONE] < 0)
		m->curSegm[ZONE] = 0;;
	
	m->curSegm[ZONE] ++;
		
	////////////////////////
	if (m->curSegm[ZONE] > m->numZone)
		m->curSegm[ZONE] = m->numZone;

	//////////Zone calculating from Mapping /////////////
	//(curCalib->GenSetup[SETUP_MAPPING_ON] > 0)? 1:0;


	if(curCalib->GenSetup[SETUP_MAPPING_ON] != _MappingActive)
		PleaseUpdateHeadSetup(m);
	
	if((curCalib->GenSetup[SETUP_MAPPING_ON]&MAPPING_SEGM)){
	

		if(curCalib->GenSetup[SETUP_POLE_ELEMENTS] < 2)
			curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 2;
		
		else if(curCalib->GenSetup[SETUP_POLE_ELEMENTS] > 32)
			curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 32;

		m->numZone = curCalib->GenSetup[SETUP_POLE_ELEMENTS] * 2;
		m->curSegm[ZONE] = WRST_TML_Param_Set[0].actIndx +1; 

	}
	
	_MappingActive = curCalib->GenSetup[SETUP_MAPPING_ON]; 
	
	//////////////////////////////////////////
	actTicksPerm = actTicks;

}


int WatchArcLinkError(MASTER_typ * m, Robot_typ * robot)
{
	int ret 			= 0;

	if(robot)
	{ // Arclink connection

		if(robot->Cntrl.Fbk.Error)
		{
			if(robot->Cntrl.Fbk.Error & ArcLink_Com_Failed){
				
				Errors(&ErrorMgr, ERROR_TIG_PROC, ERROR_ARCLINK_COM_LOST, (UDINT) robot->Cntrl.Fbk.Error);
				LogTxt(&LogMgr, "Arclink Com Err %d", (UDINT) robot->Cntrl.Fbk.Error);

			}else{
				
				Errors(&ErrorMgr, ERROR_TIG_PROC, ERROR_ARCLINK_PS_ERR, (UDINT) robot->Cntrl.Fbk.Error);
				LogTxt(&LogMgr, "Arclink Err %d", (UDINT) robot->Cntrl.Fbk.Error);

			}	
			
			ret = 1;

		}

	}

	return ret;
}

void ChangeSegmentList(){
UINT _TVL_MODE , _WF_MODE , _WP_MODE;


	if(!curCfg)
		return;
	//////////////////////////////////////////

	if((curCfg->Param_ENUM[parENUM_TIG_Mode].val[0] != _TIG_Mode)
	|| (curCfg->Param_ENUM[parENUM_AVC_Mode].val[0] != _AVC_Mode)
	||(_TVL_SYN_MODE != curCfg->Param_ENUM[parENUM_TVL_SYN_MODE].val[0])
	||(_WF_SYN_MODE != curCfg->Param_ENUM[parENUM_WF_SYN_MODE].val[0])
	||(_WP_SYN_MODE != curCfg->Param_ENUM[parENUM_WP_SYN_MODE].val[0])
	){

		if(TVL_ROT_Param_Set[0].pSynchMode)
			_TVL_MODE = curCfg->Param_ENUM[parENUM_TVL_SYN_MODE].val[0];
		else
			_TVL_MODE = 0;

		if(WIRE_Param_Set[0].pSynchMode)
			_WF_MODE = curCfg->Param_ENUM[parENUM_WF_SYN_MODE].val[0];
		else
			_WF_MODE = 0;

		if(WIRE_Param_Set[1].pSynchMode)
			_WP_MODE = curCfg->Param_ENUM[parENUM_WP_SYN_MODE].val[0];
		else
			_WP_MODE = 0;
		

		switch(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0]){

			default: // Straight

				curCfg->Param[parTIG_AmpSP].segmYes = 1;
				curCfg->Param[parTIG_AmpInSP].segmYes = 0;
				curCfg->Param[parTIG_AmpOutSP].segmYes = 0;
				curCfg->Param[parTIG_BackAmp].segmYes = 0;
				curCfg->Param[parTIG_PulseFreq].segmYes = 0;
				curCfg->Param[parTIG_PulseWidth].segmYes = 0;

				curCfg->Param[parAVC_VoltSP].segmYes = 1;				
				curCfg->Param[parAVC_VoltInSP].segmYes = 0;
				curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
				curCfg->Param[parAVC_VoltBackSP].segmYes = 0;

				/*
				curCfg->Param[parTVL_VelSP].segmYes = 1;
				curCfg->Param[parTVL_VelInSP].segmYes = 0;
				curCfg->Param[parTVL_VelOutSP].segmYes = 0;
				curCfg->Param[parTVL_VelBackSP].segmYes = 0;

				curCfg->Param[parWP_SpeedSP].segmYes = 1;
				curCfg->Param[parWP_SpeedInSP].segmYes = 0;
				curCfg->Param[parWP_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWP_BkgSpeedSP].segmYes = 0;

				curCfg->Param[parWF_SpeedSP].segmYes = 1;
				curCfg->Param[parWF_SpeedInSP].segmYes = 0;
				curCfg->Param[parWF_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWF_BkgSpeedSP].segmYes = 0;
				*/

			break;


			case 1: // Pulse
		
				curCfg->Param[parTIG_AmpSP].segmYes = 1;
				curCfg->Param[parTIG_AmpInSP].segmYes = 0;
				curCfg->Param[parTIG_AmpOutSP].segmYes = 0;
				curCfg->Param[parTIG_BackAmp].segmYes = 1;
				curCfg->Param[parTIG_PulseFreq].segmYes = 1;
				curCfg->Param[parTIG_PulseWidth].segmYes = 1;


				switch(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0]){

					default:

					curCfg->Param[parAVC_VoltSP].segmYes = 1;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;	


					break;


					case 1: // BackGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 1;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;

					break;

					case 2: // ForeGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;


					break;

					case 3: // Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;


					break;


				}					

					
			break;

			case 2: // Synch

				curCfg->Param[parTIG_AmpSP].segmYes = 1;
				curCfg->Param[parTIG_AmpInSP].segmYes = 0;
				curCfg->Param[parTIG_AmpOutSP].segmYes = 0;
				curCfg->Param[parTIG_BackAmp].segmYes = 1;
				curCfg->Param[parTIG_PulseFreq].segmYes = 0;
				curCfg->Param[parTIG_PulseWidth].segmYes = 0;
					
				switch(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0]){

					default:

					curCfg->Param[parAVC_VoltSP].segmYes = 1;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;	


					break;


					case 1: // BackGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 1;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;

					break;

					case 2: // ForeGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;


					break;

					case 3: // Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;


					break;


				}
				

			break;


			case 3: // Synch In/Out

				curCfg->Param[parTIG_AmpSP].segmYes = 0;
				curCfg->Param[parTIG_AmpInSP].segmYes = 1;
				curCfg->Param[parTIG_AmpOutSP].segmYes = 1;
				curCfg->Param[parTIG_BackAmp].segmYes = 1;
				curCfg->Param[parTIG_PulseFreq].segmYes = 0;
				curCfg->Param[parTIG_PulseWidth].segmYes = 0;
					
				switch(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0]){

					default:

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 1;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 1;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;	


					break;


					case 1: // BackGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 1;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 1;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;

					break;

					case 2: // ForeGround Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 1;


					break;

					case 3: // Locked

					curCfg->Param[parAVC_VoltSP].segmYes = 0;				
					curCfg->Param[parAVC_VoltInSP].segmYes = 0;
					curCfg->Param[parAVC_VoltOutSP].segmYes = 0;
					curCfg->Param[parAVC_VoltBackSP].segmYes = 0;


					break;


					}

				
				
			break;
			
		}


		switch(_TVL_MODE){

			default: // Straight

				curCfg->Param[parTVL_VelSP].segmYes = 1;
				curCfg->Param[parTVL_VelInSP].segmYes = 0;
				curCfg->Param[parTVL_VelOutSP].segmYes = 0;
				curCfg->Param[parTVL_VelBackSP].segmYes = 0;

			break;


			case 1:
			
				curCfg->Param[parTVL_VelSP].segmYes = 1;
				curCfg->Param[parTVL_VelInSP].segmYes = 0;
				curCfg->Param[parTVL_VelOutSP].segmYes = 0;
				curCfg->Param[parTVL_VelBackSP].segmYes = 1;

			break;


			case 2:

				curCfg->Param[parTVL_VelSP].segmYes = 0;
				curCfg->Param[parTVL_VelInSP].segmYes = 1;
				curCfg->Param[parTVL_VelOutSP].segmYes = 1;
				curCfg->Param[parTVL_VelBackSP].segmYes = 1;


			break;

		}


		
		switch(_WF_MODE){

			default: // Straight

				curCfg->Param[parWF_SpeedSP].segmYes = 1;
				curCfg->Param[parWF_SpeedInSP].segmYes = 0;
				curCfg->Param[parWF_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWF_BkgSpeedSP].segmYes = 0;

			break;


			case 1:
			
				curCfg->Param[parWF_SpeedSP].segmYes = 1;
				curCfg->Param[parWF_SpeedInSP].segmYes = 0;
				curCfg->Param[parWF_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWF_BkgSpeedSP].segmYes = 1;

			break;

			break;


			case 2:

				curCfg->Param[parWF_SpeedSP].segmYes = 0;
				curCfg->Param[parWF_SpeedInSP].segmYes = 1;
				curCfg->Param[parWF_SpeedOutSP].segmYes = 1;
				curCfg->Param[parWF_BkgSpeedSP].segmYes = 1;

			break;


			break;

		}

		switch(_WP_MODE){

			default: // Straight

				curCfg->Param[parWP_SpeedSP].segmYes = 1;
				curCfg->Param[parWP_SpeedInSP].segmYes = 0;
				curCfg->Param[parWP_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWP_BkgSpeedSP].segmYes = 0;

			break;


			case 1:
			
				curCfg->Param[parWP_SpeedSP].segmYes = 1;
				curCfg->Param[parWP_SpeedInSP].segmYes = 0;
				curCfg->Param[parWP_SpeedOutSP].segmYes = 0;
				curCfg->Param[parWP_BkgSpeedSP].segmYes = 1;

			break;

			break;


			case 2:

				curCfg->Param[parWP_SpeedSP].segmYes = 0;
				curCfg->Param[parWP_SpeedInSP].segmYes = 1;
				curCfg->Param[parWP_SpeedOutSP].segmYes = 1;
				curCfg->Param[parWP_BkgSpeedSP].segmYes = 1;

			break;


			break;

		}

		hmi->PLC_Changes |= 0xF;
		KK_Was_Cahnged = 1;
	}

	_TIG_Mode = curCfg->Param_ENUM[parENUM_TIG_Mode].val[0];

	_AVC_Mode = curCfg->Param_ENUM[parENUM_AVC_Mode].val[0];

	_TVL_SYN_MODE = curCfg->Param_ENUM[parENUM_TVL_SYN_MODE].val[0];

	_WF_SYN_MODE = curCfg->Param_ENUM[parENUM_WF_SYN_MODE].val[0];

	_WP_SYN_MODE = curCfg->Param_ENUM[parENUM_WP_SYN_MODE].val[0];

	return;


}


void BumpParam(UINT param , SINT incr ){

	if(incr > 0)
		IncrParam(curCfg, param, m->curSegm[ZONE], 1.0);
	else
		DecrParam(curCfg, param, m->curSegm[ZONE], 1.0);

	if (! (curCalib->GenSetup[SETUP_WHERE_FILL] == 2)) //Not only Editor 
		Fill_Segments(curCfg, param, m->curSegm[ZONE], curCalib->GenSetup[SETUP_AUTO_FILL],m->sm.Step);


	return;

}

void ChangeParam(UINT param , SINT incr ){


	switch(param){

		////////////////////////////////////////////
		case parTIG_AmpSP:


			switch(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0]){

				default: // Straight

					BumpParam(parTIG_AmpSP , incr) ;
				
				break;


				case 1: // Pulse
				case 2: // Synch

					BumpParam(parTIG_AmpSP , incr) ;
					BumpParam(parTIG_BackAmp , incr) ;
		

				break;


				case 3: //Synch In/Out

					BumpParam(parTIG_AmpInSP , incr) ;
					BumpParam(parTIG_AmpOutSP , incr) ;
					BumpParam(parTIG_BackAmp , incr) ;
		
				break;


			}				
	
		break;
		///////////////////////////////////////

		////////////////////////////////////////////
		case parTVL_VelSP:


			switch(curCfg->Param_ENUM[parENUM_TVL_SYN_MODE].val[0]){

				default: // Straight

					BumpParam(parTVL_VelSP , incr) ;
				
				break;


				case 1: // Synch

					if(curCfg->Param[parTVL_VelSP].val[m->curSegm[ZONE]] != 0.0)
						BumpParam(parTVL_VelSP , incr) ;

					if(curCfg->Param[parTVL_VelBackSP].val[m->curSegm[ZONE]] != 0.0)
						BumpParam(parTVL_VelBackSP , incr) ;
		

				break;


				case 2: //Synch In/Out

					if(curCfg->Param[parTVL_VelInSP].val[m->curSegm[ZONE]] != 0.0)
						BumpParam(parTVL_VelInSP , incr) ;

					if(curCfg->Param[parTVL_VelOutSP].val[m->curSegm[ZONE]] != 0.0)						
						BumpParam(parTVL_VelOutSP , incr) ;

					if(curCfg->Param[parTVL_VelBackSP].val[m->curSegm[ZONE]] != 0.0)
						BumpParam(parTVL_VelBackSP , incr) ;
		
				break;


			}				
				
		break;
		///////////////////////////////////////

		////////////////////////////////////////////
		case parWF_SpeedSP:


			switch(curCfg->Param_ENUM[parENUM_WF_SYN_MODE].val[0]){

				default: // Straight

					BumpParam(parWF_SpeedSP , incr) ;
				
				break;


				case 1: // Synch

					BumpParam(parWF_SpeedSP , incr) ;
					BumpParam(parWF_BkgSpeedSP , incr) ;
		

				break;


				case 2: //Synch In/Out

					BumpParam(parWF_SpeedInSP , incr) ;
					BumpParam(parWF_SpeedOutSP , incr) ;
					BumpParam(parWF_BkgSpeedSP , incr) ;
		
				break;


			}			
	
		break;
		///////////////////////////////////////

		////////////////////////////////////////////
		case parWP_SpeedSP:


			switch(curCfg->Param_ENUM[parENUM_WP_SYN_MODE].val[0]){

				default: // Straight

					BumpParam(parWP_SpeedSP , incr) ;
				
				break;
				
				case 1: // Synch

					BumpParam(parWP_SpeedSP , incr) ;
					BumpParam(parWP_BkgSpeedSP , incr) ;
		

				break;

				case 2: //Synch In/Out

					BumpParam(parWP_SpeedInSP , incr) ;
					BumpParam(parWP_SpeedOutSP , incr) ;
					BumpParam(parWP_BkgSpeedSP , incr) ;
		
				break;


			}			
	
		break;
		///////////////////////////////////////

		////////////////////////////////////////////
		case parAVC_VoltSP:


			switch(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0]){

				case 0: // Continue					

					switch(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0]){


						default:

							BumpParam(parAVC_VoltSP , incr) ;
							

						break;

						case 1: //Pulse
						case 2: // Synch	
						
							BumpParam(parAVC_VoltSP , incr) ;
							BumpParam(parAVC_VoltBackSP , incr) ;

						break;


						case 3: // Synch In/Out

							BumpParam(parAVC_VoltInSP , incr) ;
							BumpParam(parAVC_VoltOutSP , incr) ;
							BumpParam(parAVC_VoltBackSP , incr) ;
							
						break;
						
					}
				
				break;


				case 1: // Background Locked

					switch(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0]){


						default:

							BumpParam(parAVC_VoltSP , incr) ;
							

						break;

						case 1: //Pulse
						case 2: // Synch	
						
							BumpParam(parAVC_VoltSP , incr) ;
							
						break;


						case 3: // Synch In/Out

							BumpParam(parAVC_VoltInSP , incr) ;
							BumpParam(parAVC_VoltOutSP , incr) ;
														
						break;
						
					}
					
					
				break;

				case 2: // ForeGround Locked

					switch(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0]){


						default:

							BumpParam(parAVC_VoltSP , incr) ;
							

						break;

						case 1: //Pulse
						case 2: // Synch	
						case 3: // Synch In/Out
						
							BumpParam(parAVC_VoltBackSP , incr) ;

						break;
						
					}				

				break;

			}			
	
		break;
		///////////////////////////////////////

	}


	return;
}

void CalculateRunOverlap(){


	switch (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0])
	{
	
		default: // relative
			
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
			
			if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Rewrap
			else
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap

		break;

		case 1: // Side A overlap
			
			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Position to	Run

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;			

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] - curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Run
				
				else					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm)* (-1); //Position to	Run
					
			}
			
		break;

		case 2: // Side B overlap						

			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]- curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Run

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0; //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm) ; //Distance to Run;
				
				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)									
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
				
				else								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = 0; //Position to	Run
					
			}
			
		break;
	}

}

void SetTVL_Pos_Feedback(){

	if(curCalib->GenSetup[SETUP_TVL_SEC_ENC]){

		VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].fParam[6] = 1.0;
		VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD = VECT_Set_Param_6;

	
		TVL_ROT_Param_Set[0].EncPerTick = m->pCfg->HeadCfg.TVL.EncPerTick_SecEnc; 

	}
	else{

		VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].fParam[6] = 0.0;
		VectorStateMach[m->pCfg->HeadCfg.TVL.VECT_Indx].xCMD = VECT_Set_Param_6;
	
		TVL_ROT_Param_Set[0].EncPerTick = m->pCfg->HeadCfg.TVL.EncPerTick;
	}


return;

}


DINT Check_OSC_Amplt_and_Center(){
float pos;

	if(!OSC_TML_Param_Set[0].pVect || !OSC_TML_Param_Set[0].pPrimAmpl )
		return 0;

	if(OSC_TML_Param_Set[0].PosFbkIndx == 2)
		pos = OSC_TML_Param_Set[0].pVect->FbkVect.x2 ;

	else
		pos = OSC_TML_Param_Set[0].pVect->FbkVect.x ;
	

	if(((pos  + (*OSC_TML_Param_Set[0].pPrimAmpl)/2.0) > (OSC_TML_Param_Set[0].pVect->OutVect.softLimits[1] + 0.5))
		|| ((pos  - (*OSC_TML_Param_Set[0].pPrimAmpl)/2.0) < (OSC_TML_Param_Set[0].pVect->OutVect.softLimits[0] - 0.5))){		

		strcpy(hmi->pMessage[0], "OSC Limits problem ");
		strcpy(hmi->pMessage[1], " ");
		xsprintf(hmi->pMessage[2], "Correct Amplitude");
		strcpy(hmi->pMessage[3], " or Position ?");
		
		//TimerStart(&hmi->MsgTmr, 1.0);
		
		hmi->xCMD = SHOW_POPUP_CMD;	
		POPUP_EVENT = EVNT_OSC_ALLIGMENT;

		return 0;
								
	}
	return 1;			
}

void CalculateRunOverlap_Cont(){

DINT mode;

	if((curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0 )
		|| (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0 ))
		mode = 1; // Side A overlap
	else
		mode = curCfg->Param_ENUM[parENUM_TVL_MODE].val[0];
	

	switch (mode)
	{
	
		default: // relative
			
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
			
			if((curCalib->GenSetup[SETUP_TWO_UP]> 0)&& 0)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Rewrap
			else
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap

		break;

		case 1: // Side A overlap
			
			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Position to	Run

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0)&& 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;			

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0)&& 0)				
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] - curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Run
				
				else					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm)* (-1); //Position to	Run
					
			}
			
		break;

		case 2: // Side B overlap						

			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]- curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Run

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0)&& 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0; //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm) ; //Distance to Run;
				
				if((curCalib->GenSetup[SETUP_TWO_UP]> 0)&& 0)								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
				
				else								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = 0; //Position to	Run
					
			}
			
		break;
	}

}

void CalculateRunOverlap_Cont_OneWay(){

DINT mode;

	if((curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0 )
		|| (curCfg->Param_ENUM[parENUM_TVL_MODE].val[0] == 0 ))
		mode = 1; // Side A overlap
	else
		mode = curCfg->Param_ENUM[parENUM_TVL_MODE].val[0];
	

	switch (mode)
	{
	
		default: // relative
			
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
			
			if((curCalib->GenSetup[SETUP_TWO_UP]> 0) || 1)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Rewrap
			else
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap

		break;

		case 1: // Side A overlap
			
			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Position to	Run

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0) || 1)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;			

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0) || 1)					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] - curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Run
				
				else					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm)* (-1); //Position to	Run
					
			}
			
		break;

		case 2: // Side B overlap						

			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]- curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Run

				if((curCalib->GenSetup[SETUP_TWO_UP]> 0) || 1)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0; //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm) ; //Distance to Run;
				
				if((curCalib->GenSetup[SETUP_TWO_UP]> 0) || 1)								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
				
				else								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = 0; //Position to	Run
					
			}
			
		break;
	}

/*
	{
	
		default: // relative
			
			Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
			
			if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Rewrap
			else
				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap

		break;

		case 1: // Side A overlap
			
			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Position to	Run

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] + curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0;			

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0] - curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Position to	Run
				
				else					
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm)* (-1); //Position to	Run
					
			}
			
		break;

		case 2: // Side B overlap						

			if (!m->ToolHeadID){

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]- curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm); //Distance to	Run

				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = 0; //Position to	Rewrap
				else
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to  Rewrap
								
			}
			else{

				Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[3] = ((DINT) (curCfg->Param[parTVL_OverlapDist].val[0]) *m->ticksPerSegm) ; //Distance to Run;
				
				if(curCalib->GenSetup[SETUP_TWO_UP]> 0)									
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = ((DINT) (curCfg->Param[parTVL_RunDist].val[0]) *m->ticksPerSegm); //Distance to	Run
				
				else								
					Process[m->pCfg->HeadCfg.TVL.PROC_Indx].Param[0] = 0; //Position to	Run
					
			}
			
		break;
	}
*/
}


void RestartStateMach(){
int i;


#ifdef 	MIG_EXP_SYSTEM


	


	switch(TML_Restart_Step){


		case 0:

		break;

		case 1:

			TML_Restart_Indx = 0;

			Kick_TML_Cmd = 1;


			TML_MTR_INST[TML_AX_WRST].Disable = 1; 
			VectorStateMach[VECT_WRST].simul = 1;
			VectorStateMach[VECT_WRST].testMode = 1;

			TimerStart(&Restart_Tmr, 0.5);

			TML_Restart_Step++;

		break;


		case 2:

			if(!TimerIsTimeOut(&Restart_Tmr))
				break;

			if(TML_Restart_Indx < TML_SERVO_NUM ){

				if(!TML_MTR_INST[TML_Restart_Indx].Disable && TML_MTR_INST[TML_Restart_Indx].pState
					&&(Kick_TML_Cmd == 0)){				
					
					TML_MTR_INST[TML_Restart_Indx].xCMD = TML_RESTART;
					TML_Restart_Step++;


				}
				else
					TML_Restart_Indx++;


			}
			else
				TML_Restart_Step = 10;



		break;

		case 3:

			if((TML_MTR_INST[TML_Restart_Indx].xCMD == 0)
				|| (TML_MTR_INST[TML_Restart_Indx].xCMD == 255) // Error
				|| (TML_MTR_INST[TML_Restart_Indx].xCMD == TML_WAIT_FOR_DOWNLOAD)
				){

				TML_Restart_Indx++;
				Kick_TML_Cmd = 1;

				TimerStart(&Restart_Tmr, 0.5);
				TML_Restart_Step = 2;
				//TML_Restart_Step = 4;
			}
				
		break;


		case 4:

				

		break;
		

		case 10:


			for (i = 0; i < PROCESS_NUM; i++){

				if(i == m->pCfg->HeadCfg.GAS.PROC_Indx)
					Process[i].xCMD = prcsSTOP;
				else									
					Process[i].xCMD = prcsRESTART;

			}


			TML_Restart_Step = 0;

			

		break;


	}


	TimerExecute(&Restart_Tmr);
#else 

/*
	
for (i = 0; i < PROCESS_NUM; i++)
	Process[i].xCMD = prcsRESTART;

for (i = 0; i < TML_SERVO_NUM; i++)
{

	TML_MTR_INST[i].xCMD = TML_RESTART;
	TML_MTR_INST[i]._int.restartCnt = 0;
	TML_MTR_INST[i]._int.Init = 0;
}
*/



	switch(TML_Restart_Step){


		case 0:

		break;

		case 1:

			TML_Restart_Indx = 0;

			TML_Restart_Step++;

			
		break;


		case 2:

			if(TML_Restart_Indx < TML_SERVO_NUM ){

				if(!TML_MTR_INST[TML_Restart_Indx].Disable && TML_MTR_INST[TML_Restart_Indx].pState){				
					
					TML_MTR_INST[TML_Restart_Indx].xCMD = TML_RESTART;
					TML_Restart_Step++;


				}
				else
					TML_Restart_Indx++;


			}
			else
				TML_Restart_Step = 10;



		break;

		case 3:

			TML_Restart_Indx++;
			TML_Restart_Step = 2;


		break;

		case 10:


			for (i = 0; i < PROCESS_NUM; i++){

				if(i == m->pCfg->HeadCfg.GAS.PROC_Indx)
					Process[i].xCMD = prcsSTOP;
				else									
					Process[i].xCMD = prcsRESTART;

			}


			TML_Restart_Step = 0;

			

		break;


	}

 
	switch(Process_Restart_Step){




	}

#endif	

}


plcbit Find_ext_txt(char *name , char* ext){
UINT len , ext_len;

		len = strlen(name);

		ext_len = strlen(ext);
		
		if(len >= ext_len){

			if(strcmp(&name[len-ext_len] ,ext) == 0)
				return 1;
			else
				return 0;
				
		}
		else
			return 0;

}


