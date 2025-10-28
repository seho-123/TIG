#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <math.h>


#ifndef _HMI_M_C
#define _HMI_M_C


#include <..\GlobDef.h>
#include <..\..\Lib\WSI_types.h>
#include <..\..\Lib\WSI_Lib.c>

#include <..\GlobDecl.h>
#include <..\Hardcoded.c>
#include <..\HMI_Cntrl.c>
#include <..\HeadConfig.c>


#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\LCDLib.c> 
#include <..\..\Lib\DrillMenuLib.c> 
#include <..\..\Lib\LoggerLib.c>




_LOCAL plcbit  HMI_WHEEL_NOT_PRESSED	;// 0x1

_LOCAL plcbit  HMI_Btn_HOME   			;//0x2
_LOCAL plcbit  HMI_Btn_START  			;//0x40000000
_LOCAL plcbit  HMI_Btn_STOP   			;//0x4

_LOCAL plcbit  HMI_Btn_ARC_AMPS_NEG   	;//0x20000
_LOCAL plcbit  HMI_Btn_ARC_AMPS_POS   	;//0x200

_LOCAL plcbit  HMI_Btn_TRAVEL_SPEED_NEG   ;//0x40000
_LOCAL plcbit  HMI_Btn_TRAVEL_SPEED_POS   ;//0x400

_LOCAL plcbit  HMI_Btn_WIRE_SPEED_NEG   	;//0x80000
_LOCAL plcbit  HMI_Btn_WIRE_SPEED_POS   	;//0x800

_LOCAL plcbit  HMI_Btn_AVC_VOLTS_NEG   	;//0x100000
_LOCAL plcbit  HMI_Btn_AVC_VOLTS_POS   	;//0x1000

_LOCAL plcbit  HMI_Btn_OSC_SPEED_NEG   	;//0x200000
_LOCAL plcbit  HMI_Btn_OSC_SPEED_POS   	;//0x2000

_LOCAL plcbit  HMI_Btn_OSC_AMPL_NEG   	;//0x400000
_LOCAL plcbit  HMI_Btn_OSC_AMPL_POS   	;//0x4000

_LOCAL plcbit  HMI_Btn_AVC_LOCK   		;//0x800000
_LOCAL plcbit  HMI_Btn_WIRE_DISABLE   	;//0x8000

_LOCAL plcbit  HMI_Btn_UP   			;//0x04000000
_LOCAL plcbit  HMI_Btn_DOWN   			;//0x10000000
_LOCAL plcbit  HMI_Btn_RIGHT   			;//0x02000000
_LOCAL plcbit  HMI_Btn_LEFT   			;//0x20000000
_LOCAL plcbit  HMI_Btn_ENTER   			;//0x08000000



// This is for Android 
UDINT GlblBtnCmd[2];	
UDINT GlblBtnLED[2];
plcbit TestMode;

///////  HMI for File Managment
#define VISIBLE 	0
#define INVISIBLE 	1

_LOCAL UINT FileOper_xCMD;
_LOCAL UINT FileOper_Indx;
_LOCAL plcstring FileOper_FileName[FILE_NAME_MAX];
_LOCAL plcstring FileOper_Msg[FILE_NAME_MAX];
_LOCAL UINT FileOper_Btn;
_LOCAL UINT FileOper_Edit_vis;
_LOCAL UINT FileOper_Edit_lock;
_LOCAL UINT FileOper_Tmr;
_LOCAL UINT FileOper_Btn_vis;

//////////////////////////////////////

_LOCAL EDIT_PARAM_PAGE_typ EditPageData;
//

_LOCAL Units_obj Units[unitMaxIndx];

_LOCAL USINT FileCMD;
_LOCAL plcstring FileExtension[16];

//////////////////////////

_LOCAL plcbit KK_SingleEditor_test;

_LOCAL USINT KK_Test_Tick;

UINT _PageBtnPress[16];

_LOCAL unsigned char LastPageIndx;

_LOCAL FileOpen_typ  _FOpen;
_LOCAL FileDelete_typ _FDelete;
_LOCAL FileClose_typ _FClose;

void DeleteExtension(char *p);


plcbit _btns;

UINT _Single_Editor_vis;

USINT _use_USB_RAM;


void JogMode();
void PageNav();
void PageOperationsCntrl();

void Open_SingleEditor(UINT k);
void Open_SingleEnumEditor(UINT k);
//float GetParamValue(MASTER_typ *m , Scaled_Var_obj* var , UINT segm );

void Page1_RUN();
void Page2_SEGM_EDITOR();
void Page3_LogBook();
void Page4_AplicSetting();
void Page5_GlobalSetting();
void Page6_DrillDown();
void Page7_Diagnostic();
void Page8_Errors();
void Page9_Alt_RUN();
void Page10_Main_Legacy();
void Page11_Calibration();
void Page12_Files();
void Page13_Main();

void Page20_Errors();


/////////////////



_LOCAL HMI_typ * hmi;
_LOCAL HMI_INTRF_typ * intrf;


/// Segments Editor
_LOCAL UINT Page_Step;
_LOCAL plcstring Segm_Var_Name[8][32];
_LOCAL float Segm_Var_Value[8][4];

_LOCAL USINT HorLine_Indx , HorLine_Top_Indx;
_LOCAL USINT VertCol_Indx , VertCol_Lft_Indx;

_LOCAL plcbit VertCol_Left_Btn , VertCol_Right_Btn;
_LOCAL plcbit HorLine_Up_Btn , HorLine_Dwn_Btn;



_LOCAL UINT HorLine_vis[8], VertLine_vis[4] , Segm_Var_vis[8][4];
_LOCAL UINT Segm_Var_Color[8][4] , HorLine_Color[8];
_LOCAL INT  HorLine_ExtraDig[8];
_LOCAL plcstring Segm_Var_Unit[8][16];
_LOCAL UINT Segm_Var_Indx[8];
_LOCAL UINT Segm_Zone_Indx[4];



_LOCAL USINT Completed_Flag[16];

_LOCAL UINT Visible[8], Locked[8];
_LOCAL plcstring Visible_Text[32];
//////////////////
// New LCD Mgr
_LOCAL DINT statusInit;
_LOCAL SerialPortDriver_typ _RS232;



/////  Pendant variable editor

_LOCAL float *Pend_Edit_Var;
_LOCAL plcstring Pend_Edit_Var_Name[64];

_LOCAL DINT Pend_Edit_Segm;
_LOCAL UINT Pend_Edit_vis;

////////////////////////



UINT _virt_Oper_Pend_vis;


void _INIT ProgramInit(void)
{
UINT i;

	
	File_RW.DO_NOT_Del_Extension = 1; 
	gFileList_Indx = 0;
	
	Pend_Edit_Segm = 1;
	DrillDwnHMI.UserLevel = 1000;

	strcpy(ESAB_WebPage , "192.168.10.122");

	HMI_Intr.GlblBtnCmd[0] = 0;
	Master[0].Active = Master[0].Active;

	MIG_PEND_ShowUp_Box_vis = 1 ; //INVISIBLE;

	KK_NewHMI;
	FileOper_FileName;
	FileOper_Btn;
	FileOper_Edit_vis;
	FileOper_Msg;
	FileOper_Edit_lock;
	FileOper_xCMD = 100;

	////////////
	Master[1].pCfg = &Cfg;
	Master[1].pCurCfg = &CurCfg;
	Master[1].pCalib = &CurCalib;
	Master[1].pDefaultCalib = &DefaultCalib;
	Master[1].pScaledUnitIndx = curScaledUnitIndx;
	Master[1].pScaledVar = (UDINT)&Cfg.ScaledVar;
	Master[1].pSegmentTable = (UDINT)&SegmentsTable;
	Master[1].pUnits = Units;

	Master[1].varLastNum		= varLAST_VARIABLE;
	Master[1].paramLastNum 	= parLAST_PARAM;
	Master[1].numSegm			= SEGM_NUM_MAX;
	Master[1].paramEnumLastNum = parENUM_LAST_PARAM;
	Master[1].varEnumLastNum	= varENUM_LAST_VARIABLE;
	Master[1].paramIntgLastNum = parINT_LAST_PARAM;
	Master[1].varIntLastNum	= varINT_LAST_VARIABLE;
	Master[1].paramStrLastNum	= parSTRING_LAST_PARAM;
	Master[1].varStrLastNum	= varSTRING_LAST_VARIABLE;

	
	//////////////////////////////////////////
	SetupNODES_Android(&Master[1], NODES_M, P_LAST_NODE); 

	// Assign HMI pointers from external instances			
	for (i = 0; i < 20; i++)
		Master[1].Hmi[0].pHMI_Lines[i] = HMI_lines[i];//HMI_lines[i]; //HMI_lines2[i];

	for (i = 0; i < 4; i++)
		Master[1].Hmi[0].pMessage[i] = DrillDwnHMI.Message[i]; //Master[0].AndIntrf[0].Message[i];

	Master[1].Hmi[0].pBanner = DrillDwnHMI.Banner; //Master[0].AndIntrf[0].Banner;
	DrillDwnHMI.pActiveBanner = Master[1].Hmi[0].ActiveBanner;
	//////////////////

	strcpy(DrillDwnHMI.Banner , "New DrillDwn");	
	
	DrillDwnHMI.editPageData = &EditPageData;
	DrillDwnMenu_INIT(&DrillDwnHMI , &Master[1], NODES_M , &Cfg , &CurCfg , &Master[0].Hmi[0]);

	Virt_Oper_Pend_vis = 1; // INVISIBLE

	////  Single Param Editor	
	SingleParamEditorHMI.editPageData = &EditPageData;
	SINGLE_EDITOR_INIT(&SingleParamEditorHMI, &Master[1], &Cfg , &CurCfg);

	for(i = 0 ; i < 16 ; i++)
		ReadVar[i].pMaster = &Master[1];
	//////////////////////////////
	
	SingleEnumParamEditorHMI.Enum_Editor_vis = 1; // INVISIBLE
	
}

void _CYCLIC ProgramCyclic(void)
{
UINT i;
plcbit btns;

	
	btns = Pend_Btn_Left && Pend_Btn_Right;
	
	if(btns && !_btns){

		if(Virt_Oper_Pend_vis == 1){ // INVISIBLE 		

			Virt_Oper_Pend_vis = 0; // VISIBLE
		}else{

			Virt_Oper_Pend_vis = 1; // INVISIBLE 
			HMI_Jog_Select = 0;	

		}		
	}

	////

		if(_virt_Oper_Pend_vis != Virt_Oper_Pend_vis){

			SingleParamEditorHMI.Editor_vis = 1; // INVISIBLE
			DrillDwnHMI.Editor_vis = 1; // INVISIBLE

		}


	///
	_btns = btns;
	_virt_Oper_Pend_vis = Virt_Oper_Pend_vis;

	JogMode();
	
	//////////////////////////////////////////////////////////


	/////////////////////////////////
	hmi = &Master[1].Hmi[0];	
	intrf = &Master[1].Intrf[0];

	hmi->curHeadID		= DrillDwnHMI.curHeadID;
	hmi->curNodeID		= DrillDwnHMI.curNodeID;
	
	hmi->UserLevel = DrillDwnHMI.UserLevel;

	intrf->LCDEntr = DrillDwnHMI.LCDEntr;
	intrf->TickPlus = DrillDwnHMI.TickPlus;
	intrf->TickMinus = DrillDwnHMI.TickMinus;

	//DrillDwnHMI.FiredEvent = 0;
	//DrillDwnHMI.PleaseUpdate = 0;

	
	if(DrillDwnHMI.PageHeadClick){
		LogMgr.Cmd.LastLogs = 1;

		// Go to main Drill DOwn Entry
		if(DrillDwnHMI.curHeadID == DrillDwnHMI.curNodeID){

			switch(DrillDwnHMI.curHeadID){

			case P_LOGS:
			case P_ERRORS:
			case P_DIAGNOSTIC:

			break;

			default:

				PageBtnPress[10] = 1;

			break;
				
			

			}

		}

	}

	////

	FilesOperation_Srv();

	////////////////////////////////
	// Refresh Data after edit
	if(SingleParamEditorHMI.Editor_vis && !_Single_Editor_vis)
			Page_Step = 0;

	_Single_Editor_vis = SingleParamEditorHMI.Editor_vis;

	///////////////////////////////

	DrillDwnMenu_MGR(&DrillDwnHMI);	

	if(DrillDwnHMI.FiredEvent)
		hmi->FiredEvent = DrillDwnHMI.FiredEvent;

	if(DrillDwnHMI.PleaseUpdate)
		hmi->PleaseUpdate = DrillDwnHMI.PleaseUpdate;
	
	EvaluateHMI_Android(&Master[1], NODES_M, hmi, intrf);

	///////////////////////////////////
	switch(hmi->xCMD){

		case IDLE_CMD:
		case JUMP_CMD:		
		case UPDATE_CMD:			
		case  SHOW_POPUP_CMD:
		break;

		case JUMP2ID_CMD:

			DrillDwnHMI.curHeadID = hmi->Node2JumpID;
			DrillDwnHMI.curTopNodeID = hmi->Node2JumpID;
			DrillDwnHMI.curNodeID = hmi->Node2JumpID;
			//DrillDwnHMI.xCMD = hmi->xCMD;

			KK_Test_Tick++;
		break;

	}
	hmi->xCMD = 0;
	////////////////////////////////
	
	DrillDwnHMI.PopUpMsg_Event = POPUP_EVENT;
	DrillDwnHMI.FiredEvent = 0;
	DrillDwnHMI.PleaseUpdate = 0;

	///////////////////////////////////////
	////  Single Param Editor

	PageOperationsCntrl();
	
	SINGLE_EDITOR_MGR(&SingleParamEditorHMI);

	SINGLE_ENUM_EDITOR_MGR(&SingleEnumParamEditorHMI);
	
	/////////////////////////////
	
	PageNav();

	/*********RESET BUTTONS *********/	
	for(i = 0 ; i < 32 ; i++){ 
		Screen.btn[i] = 0;
	}	
	
	for(i = 0 ; i < 32 ; i++){ 
		Screen.btnBar[i] = 0;
	}
	
	Screen.btn_NO = 0;
	Screen.btn_YES= 0;
	Screen.Update = 0;	

	

	
}




void PageNav(){
USINT i;

	if(visChangePage == 255){

		// return from Diagnostic or Set Limits page
		if((NewPageReq_Cmd ==  20) // Erros page
			//|| ((NewPageReq_Cmd ==  30)) // Edit Param Page
			|| ((NewPageReq_Cmd ==  200))){ // StanBy

			if(NewPageReq_Cmd == visPage){
				
				NewPageReq_Cmd = LastPageIndx; // return to last page

				if(visPage == 20){ // Erros page
					
					// Restart Step
					MIG_PEND_Intrf.curHeadID = P_ERRORS;
					MIG_PEND_Intrf.curLineID = P_ERRORS;

					
					
				}

			}			

		}
			
		// Navigate from Splash Screen
		if(visPage == 0)
			NewPageReq_Cmd = 1;

		// Watch main menu pressed	
		for(i = 1; i < 16 ; i++){
			
			if(PageBtnPress[i] && !_PageBtnPress[i]){
				NewPageReq_Cmd = i;
				EditBox_Indx = 0;
			}
			else {

				if(i== visPage )
					PageBtnPress[i] = 1;
				else
					PageBtnPress[i] = 0;
			}	

			
			
			_PageBtnPress[i] = PageBtnPress[i]; 
		}				
		

		if(NewPageReq_Cmd > 0){	
			
			visChangePage = NewPageReq_Cmd;

			LastPageIndx = visPage;

			// 
			Page_Step = 0;
			FileOper_xCMD = 0;
			//
			
			Virt_Oper_Pend_vis = 1; // INVISIBLE


			///  Stop any motion

			if((NewPageReq_Cmd == 20)
				|| (LastPageIndx == 20)){ // Jump to or from  Calibration
				
				
				///////////////

			}			

			for(i = 1 ; i < 16 ; i++)
				PageBtnPress[i] = 0;	

			if(NewPageReq_Cmd < 16)
				PageBtnPress[NewPageReq_Cmd] = 1;

			//Screen_sm.Step = 0;
						
		}

		///////////////////////
		//if(_PageBtnPress[i] != PageBtnPress[i]){

			
			switch(NewPageReq_Cmd){


				case 3: // LogBook
					
					DrillDwnHMI.curTopNodeID = P_LOGS;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 4: // Application Param
					
					DrillDwnHMI.curTopNodeID = P_PARAM_APPL;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 5: // Application Param
					
					DrillDwnHMI.curTopNodeID = P_PARAM_GLOBAL;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;


				case 6: // Application Param
					
					DrillDwnHMI.curTopNodeID = P_MENU;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 7: // Diagnostics
					
					DrillDwnHMI.curTopNodeID = P_DIAGNOSTIC;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 8: // Errors
					
					DrillDwnHMI.curTopNodeID = P_ERRORS;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 11: // Calibration
					
					DrillDwnHMI.curTopNodeID = P_MODE_CALIB;
					DrillDwnHMI.curNodeID = DrillDwnHMI.curTopNodeID;
					DrillDwnHMI.curHeadID =  DrillDwnHMI.nodes[DrillDwnHMI.curTopNodeID].HeadID;
					DrillDwnHMI.RefreshPage = 1;

				break;

				case 12: // Files Managment

					//if(hmi->ROM_USB)
					if(Use_USB_RAM)
						strcpy(File_RW.deviceName, "USB_WeldPrograms");
					else
						strcpy(File_RW.deviceName, "WeldPrograms");

					strcpy(File_RW.MsgText , "");
					File_RW.CMD = F_READ_DIR_FILELIST;

				break;


				case 1:
				case 2:	
				case 9:	

					DrillDwnHMI.curTopNodeID = 0;
					DrillDwnHMI.curHeadID = 0;
					DrillDwnHMI.curNodeID = 0;

				break;


			}
		//}
		/////////////////////////

		NewPageReq_Cmd = 0;
	
	}

	
}

void PageOperationsCntrl(){
UINT i;



	if(Virt_Oper_Pend_vis == 1){ // INVISIBLE - Start STop Pendant invisible

		
		switch (visPage)
		{

			case 1: // RUN

				Page1_RUN();

			break;

			case 2: // Segments Editor

				Page2_SEGM_EDITOR();				

			break;

			case 3:

				Page3_LogBook();

			break;

			case 4:

				Page4_AplicSetting();

			break;

			case 5:

				Page5_GlobalSetting();

			break;

			case 6:

				Page6_DrillDown();

			break;

			case 7:

				Page7_Diagnostic();

			break;

			case 8:

				Page8_Errors();

			break;

			
			case 9:  // ALT_RUN page			

				Page9_Alt_RUN();				

			break;

			case 10:

				Page10_Main_Legacy();

			break;

			case 11:

				Page11_Calibration();

			break;

			case 12:

				Page12_Files();

			break;
			
			
			case 13: // New Main

				Page13_Main();

			break;

			case 20:

				Page20_Errors();

			break;			

			default:

				//for(i = 0 ; i < MAX_READ_VAR ; i++)
					//VarBtnPress[i] = 0; 

			break;
		}

		/////////////////////////////		
		
		
		for(i = 0 ; i < MAX_READ_VAR ; i++){
			Open_SingleEditor(i);	
			Open_SingleEnumEditor(i);

		}

		
		
	

	}
	 
}



void JogMode(){
UINT indx;


	//HMI_Intr.GlblBtnLED[0] =  Master[0].AndIntrf[0].GlblBtnLED[0];	
	HMI_Intr.GlblBtnLED[0] =  Master[1].AndIntrf[0].GlblBtnLED[0];
	
	DrillDwnHMI.StatusLED[0] = (HMI_Intr.GlblBtnLED[0] & 0x1)? 1:0; // Test Mode
	DrillDwnHMI.StatusLED[1] = (HMI_Intr.GlblBtnLED[0] & 0x2)? 1:0; // Wire Disabled
	DrillDwnHMI.StatusLED[2] = (HMI_Intr.GlblBtnLED[0] & 0x4)? 1:0; // AVC Locked
	DrillDwnHMI.StatusLED[3] = (HMI_Intr.GlblBtnLED[0] & 0x8)? 1:0; // Gas ON	
	DrillDwnHMI.StatusLED[4] = (HMI_Intr.GlblBtnLED[0] & 0x10)? 1:0; // Head A/B
	DrillDwnHMI.StatusLED[5] = (HMI_Intr.GlblBtnLED[0] & 0x20)? 1:0; // Head B/A

	HMI_Intr.GlblBtnCmd[0] = 0;	
	if(Pend_Btn_Stop)		
		HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_STOP;

	if(Pend_Btn_Start)
		HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_START;

	//!!!!!!!
	if(Virt_Oper_Pend_vis == 1) // INVISIBLE - Pendant Start STop Page invisible
		return;

	/////////////////////////////////////////////////////////////////
	
	if(Pend_Edit_Segm < 0)
		Pend_Edit_Segm = 0;
	else if(Pend_Edit_Segm > SEGM_NUM_MAX)
		Pend_Edit_Segm = SEGM_NUM_MAX - 1;


	/// Show Selected values
	/*

	if(CurCfg.Param[parTIG_AmpSP].segmYes)
		Pend_Edit_Var = &CurCfg.Param[parTIG_AmpSP].val[Pend_Edit_Segm];
	else
		Pend_Edit_Var = &CurCfg.Param[parTIG_AmpSP].val[0];

	xsprintf(Pend_Edit_Var_Name,"Name: %s", Cfg.ScaledVar[parTIG_AmpSP].name);	
	
	*/
	
	/////////////////////////////////////////////////////////////////////

	Pend_Edit_vis = 0; // VISIBLE
	
	Visible[0] = VISIBLE; // Left Edit value
	Visible[1] = VISIBLE; // Center Edit value
	Visible[2] = VISIBLE; // Right Edit value
	
	Visible[3] = VISIBLE; // Left Current Value
	Visible[4] = VISIBLE; // Center Current Value
	Visible[5] = VISIBLE; // Right Current Value

	Visible[6] = INVISIBLE; // Big Current Value in Center + Text

	Visible_Text[0] = 0;
	
	switch(HMI_Jog_Select){


		case 1:	// ARC AMPS

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_ARC_AMPS_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_ARC_AMPS_POS;


			///////////////////////
			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parWF_SpeedSP], 2);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 3);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurWF_Vel], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurArc_Volt], 0);

		break;


		case 2:	// TRAVEL SPEED

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_TRAVEL_SPEED_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_TRAVEL_SPEED_POS;

			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTVL_VelSP], 1);
			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parTVL_VelSP], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurTVL_Vel], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[parAVC_VoltSP], 0);

		break;		


		case 3:	// WIRE SPEED

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_WIRE_SPEED_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_WIRE_SPEED_POS;

			
			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parWF_SpeedSP], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parWF_SpeedSP], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurWF_Vel], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurArc_Volt], 0);
			

		break;

		case 4:	// AVC VOLT

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_AVC_VOLTS_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_AVC_VOLTS_POS;

			
			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parAVC_VoltSP], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parAVC_VoltSP], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parWF_SpeedSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurArc_Volt], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurWF_Vel], 0);
	

		break;


		case 5:	// OSC SPEED

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_OSC_SPEED_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_OSC_SPEED_POS;

			
			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parOSC_Excur], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parOSC_Excur], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varOSC_curExcurTm], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurArc_Volt], 0);
		

		break;

		case 6:	// OSC AMPL

			if(Pend_Btn_Left)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_OSC_AMPL_NEG;
			else if(Pend_Btn_Right)
				HMI_Intr.GlblBtnCmd[0] += ANDR_BTN_OSC_AMPL_POS;

			
			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parOSC_Amplit], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parOSC_Amplit], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varOSC_CurPos], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurArc_Volt], 0);
			

		break;

		default:

			
			if(Pend_Btn_Left)
				HMI_Intr.Wheel ++;
			else if(Pend_Btn_Right)
				HMI_Intr.Wheel --;

			//ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[varOSC_Centr], 1);

			ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parTIG_AmpSP], 1);
			ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parAVC_VoltSP], 1);	

			ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurTIG_Amp], 0);
			ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varOSC_Centr], 0);
			ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurArc_Volt], 0);

			Visible[1] = INVISIBLE; // Center Edit value
			Visible[4] = INVISIBLE; // Center Current Value
			Visible[6] = VISIBLE; // Big Current Value in Center + Text
			strcpy(Visible_Text , " OSC Center Steering");

			

			Pend_Edit_vis = 1; // INVISIBLE
			HMI_Jog_Select = 0;

		break;

	}	
				
	Open_SingleEditor(0);	
	Open_SingleEditor(1);
	Open_SingleEditor(2);
	
}

void FilesOperation_Srv(){
/*
	FileOper_FileName;
	FileOper_Btn;
	FileOper_Edit_vis;
	FileOper_Msg;
	FileOper_Edit_lock;
*/

	if(_use_USB_RAM != Use_USB_RAM){
		
		if(Use_USB_RAM)
			strcpy(File_RW.deviceName, "USB_WeldPrograms");
		else
			strcpy(File_RW.deviceName, "WeldPrograms");

		memset(gNewFileList , 0 , sizeof (gNewFileList));
		strcpy(File_RW.MsgText , "");
		File_RW.CMD = F_READ_DIR_FILELIST;

		_use_USB_RAM = Use_USB_RAM;

	}
		

	switch(FileOper_xCMD){

		case 0: 

		break;

		/////////////////////////////////////////////////////
		////////   READ /////////////////////////////////////
		case 1:	

			if(File_RW.CMD){

			}

			else if(FileOper_Indx < 500){

				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				strcpy(FileOper_Msg , "Load File \n Please Confirm !!!");
				
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 1;
				FileOper_xCMD++;

			}
			else			
				FileOper_xCMD = 100;
			
		break;


		case 2:	

			if(FileOper_Btn == 1){ // OK

				if(FileOper_Indx < 500){

					if(strlen(gNewFileList[FileOper_Indx])){
						strcpy( File_RW.fileName ,  gNewFileList[FileOper_Indx]);
						File_RW.CMD = F_LOAD_CURCFG;

						FileOper_Btn_vis = INVISIBLE;
						strcpy(FileOper_Msg , "Please WAIT");
						FileOper_xCMD ++;
					}

				}
			}
			else if(FileOper_Btn == 2) // Cancel			
				FileOper_xCMD = 100;
			
		break;

		case 3:			

			if(!File_RW.CMD){

				if(File_RW.Error){

					strcpy(FileOper_FileName , File_RW.MsgText);
					strcpy(FileOper_Msg , "File Load Error !!!");				

				}
				else{

					strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
					strcpy(FileOper_Msg , "File Loaded \n Success !!!");

				}

				FileOper_Btn_vis = INVISIBLE;
				FileOper_Tmr = 30;
				FileOper_xCMD ++;
			}
						
		break;


		case 4:
			
			if(!FileOper_Tmr)
				FileOper_xCMD = 100;

			FileOper_Tmr--;

		break;

		

		/////////////////////////////////////////////////////
		////////   SAVE PROGRAM AS  /////////////////////////

/*		
case F_SAVE_AS_CURCFG_BIN: WriteMode = F_MODE_BINARY ; break;  - 46

case F_SAVE_AS_FULL_BIN: WriteMode = F_MODE_BINARY_FULL ; break; - 48	

case F_SAVE_AS_CALIB_BIN: WriteMode = F_MODE_BINARY_CALIB ; break; - 49

case F_SAVE_AS_CURCFG: WriteMode = F_MODE_XML ; break; // To dziwne - 7
*/
		
		case 10: 

			
 			if(File_RW.CMD) break;

			FileCMD = F_SAVE_AS_CURCFG_BIN;

			if(FileOper_Indx < 500){

				//strcpy(FileOper_FileName ," New File Name ");
				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				
				strcpy(FileOper_Msg , "Save Params As .setup \n Please Confirm !!!");
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 0;

				//strcpy(FileExtension , "_setup");
				strcpy(FileExtension , ".setup");
				FileOper_xCMD = 20;

			}
			else			
				FileOper_xCMD = 100;
			
		break;

		case 11: 

			if(File_RW.CMD) break;

			FileCMD = F_SAVE_AS_FULL_BIN;

			if(FileOper_Indx < 500){

				//strcpy(FileOper_FileName ," New File Name ");
				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				
				strcpy(FileOper_Msg , "Save Params & Calibr. As .state \n Please Confirm !!!");
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 0;

				//strcpy(FileExtension , "_state");
				strcpy(FileExtension , ".state");
				FileOper_xCMD = 20;

			}
			else			
				FileOper_xCMD = 100;
			
		break;

		case 12: 

			if(File_RW.CMD) break;

			FileCMD = F_SAVE_AS_CALIB_BIN;

			if(FileOper_Indx < 500){

				//strcpy(FileOper_FileName ," New File Name ");
				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				
				strcpy(FileOper_Msg , "Save ONLY Calibration As .calib \n Please Confirm !!!");
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 0;

				//strcpy(FileExtension , "_calib");
				strcpy(FileExtension , ".calib");
				FileOper_xCMD = 20;

			}
			else			
				FileOper_xCMD = 100;
			
		break;
		
		case 13: 

			if(File_RW.CMD) break;

			FileCMD = F_SAVE_AS_CURCFG;

			if(FileOper_Indx < 500){

				//strcpy(FileOper_FileName ," New File Name ");
				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				
				strcpy(FileOper_Msg , "Save Welding Params As .prg \n Please Confirm !!!");
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 0;

				//strcpy(FileExtension , "_prg");
				strcpy(FileExtension , "");
				FileOper_xCMD = 20;

			}
			else			
				FileOper_xCMD = 100;
			
		break;

		/////////////////////////////////////////////////////
		case 20:


			if(FileOper_Btn == 1){ // OK

			if(FileOper_Indx < 500){

					DeleteExtension(FileOper_FileName);
					strcat(FileOper_FileName , FileExtension);

					if(strlen(FileOper_FileName)){
						
						_FOpen.enable		= 1;
						_FOpen.pDevice	= (UDINT)File_RW.deviceName;
						_FOpen.pFile = (UDINT)FileOper_FileName;
						_FOpen.mode	  = FILE_RW;
						
						FileOper_Btn_vis = INVISIBLE;
						strcpy(FileOper_Msg , "Please WAIT");
						FileOper_xCMD ++;
					}

				}
			}
			else if(FileOper_Btn == 2) // Cancel			
				FileOper_xCMD = 100;		



		break;


		case 21:	


			FileOpen(&_FOpen);

			File_RW._int.dwIdent = _FOpen.ident;
			File_RW._int.wStatus = _FOpen.status;

			/* status (20708 -> File doesn't exist) */

			if (File_RW._int.wStatus == 20708)
			{

				FileOper_xCMD = 25;
				
			}
			// File Found
			else if (File_RW._int.wStatus == 0){				
				
				_FClose.enable	 = 1;
				_FClose.ident 	 = File_RW._int.dwIdent;

				FileOper_xCMD++;

			}
			else if (File_RW._int.wStatus != 65535)
		   	{
			  File_RW._int.byErrorLevel = 7;
			 File_RW._int.step  = 254; // Error	

			 FileOper_xCMD = 100;
			  
			  if (File_RW._int.wStatus == 20799)
			  {
				 File_RW._int.wError = FileIoGetSysError();
			  }
			}		

		break;


		case 22:

			FileClose(&_FClose);
			
			File_RW._int.dwIdent = _FClose.ident;
			File_RW._int.wStatus = _FClose.status;

			if (File_RW._int.wStatus == 0)
			{
				strcpy(FileOper_Msg , "File Exists Do you want OVER WRITE ?");
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 0;

				FileOper_Btn = 0;
		
				FileOper_xCMD ++;		
					 
					 
			}					
		   	else if (File_RW._int.wStatus != 65535)
		   	{
			  File_RW._int.byErrorLevel = 7;
			 File_RW._int.step  = 254; // Error	

			 FileOper_xCMD = 100;
			  
			  if (File_RW._int.wStatus == 20799)
			  {
				 File_RW._int.wError = FileIoGetSysError();
			  }
			}				


		break;


		case 23:

			if(FileOper_Btn == 1){ // OK

				FileOper_Btn_vis = INVISIBLE;
				strcpy(FileOper_Msg , "Please WAIT");

				_FDelete.enable	 = 1;
				_FDelete.pDevice = (UDINT) File_RW.deviceName;
				_FDelete.pName	=  (UDINT) FileOper_FileName;				

				FileOper_xCMD++;
				
				
			}
			else if(FileOper_Btn == 2) // Cancel			
				FileOper_xCMD = 100;

		break;


		case 24:

			FileDelete(&_FDelete);
			/* Get status */

			File_RW._int.wStatus = _FDelete.status;

			if (File_RW._int.wStatus == 0)
			{
						
				FileOper_xCMD = 25;		
					 
					 
			}					
		   	else if (File_RW._int.wStatus != 65535)
		   	{
			  File_RW._int.byErrorLevel = 7;
			 	File_RW._int.step  = 254; // Error	

				FileOper_xCMD = 100;
			  
			  if (File_RW._int.wStatus == 20799)
			  {
				 File_RW._int.wError = FileIoGetSysError();
			  }
			}		


		break;		
		/////////////////////////////////////////////////////


		case 25: 

			if(FileOper_Btn == 3){

				FileOper_xCMD = 100;

			}// OK			
			else if(FileOper_Btn == 1){ // OK

				if(FileOper_Indx < 500){

					
					
					if(strlen(FileOper_FileName)){
						strcpy( File_RW.fileNewName ,	FileOper_FileName);
						
						File_RW.CMD = FileCMD;								    

						FileOper_Btn_vis = INVISIBLE;
						strcpy(FileOper_Msg , "Please WAIT");
						//strcpy(FileOper_Msg , File_RW.fileNewName);
						FileOper_xCMD ++;
					}

				}
			}
			else if(FileOper_Btn == 2) // Cancel			
				FileOper_xCMD = 100;
			
		break;

		case 26: 
			

			if(!File_RW.CMD){

				if(File_RW.Error){

					strcpy(FileOper_FileName , File_RW.MsgText);
					strcpy(FileOper_Msg , "File Save Error !!!");				

				}
				else{

					strcpy(FileOper_FileName , File_RW.MsgText);
					strcpy(FileOper_Msg , "Program: ");					
					strcat(FileOper_Msg , File_RW.fileNewName);					
					strcat(FileOper_Msg , "\nSaved Success !!!");

				}

				FileOper_Btn_vis = INVISIBLE;
				FileOper_Tmr = 30;
				FileOper_xCMD ++;
			}
						
		break;


		case 27:
			
			if(!FileOper_Tmr){

				if(0)
					strcpy(File_RW.deviceName, "USB_WeldPrograms");
				else
					strcpy(File_RW.deviceName, "WeldPrograms");

				strcpy(File_RW.MsgText , "");
						
				File_RW.CMD = F_READ_DIR_FILELIST;
			
				FileOper_xCMD = 100;

			}

			FileOper_Tmr--;

		break;

		
		/////////////////////////////////////////////////////
		////////   DELETE /////////////////////////////////////
		case 50: 

			if(File_RW.CMD) break;

			if(FileOper_Indx < 500){

				strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
				strcpy(FileOper_Msg , "Delete File \n Please Confirm !!!");
				
				FileOper_Edit_vis = VISIBLE;
				FileOper_Btn_vis = VISIBLE;
				FileOper_Edit_lock = 1;
				FileOper_xCMD++;

			}
			else			
				FileOper_xCMD = 100;
			
		break;


		case 51: 

			if(FileOper_Btn == 1){ // OK

				if(FileOper_Indx < 500){

					if(strlen(gNewFileList[FileOper_Indx])){
						strcpy( File_RW.fileName ,	gNewFileList[FileOper_Indx]);
						File_RW.CMD = F_DELETE;

						FileOper_Btn_vis = INVISIBLE;
						strcpy(FileOper_Msg , "Please WAIT");
						FileOper_xCMD ++;
					}

				}
			}
			else if(FileOper_Btn == 2) // Cancel			
				FileOper_xCMD = 100;
			
		break;

		case 52: 		

			if(!File_RW.CMD){

				if(File_RW.Error){

					strcpy(FileOper_FileName , File_RW.MsgText);
					strcpy(FileOper_Msg , "File Delete Error !!!");				

				}
				else{

					strcpy(FileOper_FileName , gNewFileList[FileOper_Indx]);
					strcpy(FileOper_Msg , "File Deleted \n Success !!!");

				}

				FileOper_Btn_vis = INVISIBLE;
				FileOper_Tmr = 30;
				FileOper_xCMD ++;
			}
						
		break;


		case 53:
			
			if(!FileOper_Tmr)
				FileOper_xCMD = 100;

			FileOper_Tmr--;

		break;


		/////////////////////////////////////////////////////	

		default:

			FileOper_xCMD = 0;
			FileOper_Btn = 0;
			FileOper_Edit_vis = INVISIBLE;
			FileOper_Btn_vis = INVISIBLE;
			FileOper_Edit_lock = 1;

		break;


	}
	
}


void Open_SingleEditor(UINT k){

	if(k >= MAX_READ_VAR)
		return;

	if(VarBtnPress[k]){
		VarBtnPress[k] = 0;

		EditPageData.var = ReadVar[k].var;
		EditPageData.Segm_Indx = ReadVar[k].Segm_Indx;

		  
		if(ReadVar[k].var->Indx < parLAST_PARAM){
			SingleParamEditorHMI.Editor_vis = 0; // VISIBLE
			SingleParamEditorHMI.RefreshEditor = 1;	
		

			if(CurCfg.Param[ReadVar[k].var->Indx].segmYes){
				SingleParamEditorHMI.editPageData->Segm_Indx_Locked = 0;
			}
			else{
				SingleParamEditorHMI.editPageData->Segm_Indx_Locked = 1;
				SingleParamEditorHMI.editPageData->Segm_Indx = 0;
			}

		}

		
 	}
}

void Open_SingleEnumEditor(UINT k){

	if(k >= MAX_READ_VAR)
		return;

	if(VarEnumBtnPress[k]){
		VarEnumBtnPress[k] = 0;

		SingleEnumParamEditorHMI.ParamEnum_Indx = ReadVar_Enum[k].ParamEnum_Indx;		
		SingleEnumParamEditorHMI.Param_Enum_Segm = ReadVar_Enum[k].Param_Enum_Segm;

		SingleEnumParamEditorHMI.Enum_Editor_vis = 0; // VISIBLE
		SingleEnumParamEditorHMI.RefreshEditor = 1;	

		
		if(CurCfg.Param_ENUM[ReadVar_Enum[k].ParamEnum_Indx].segmYes)
			SingleEnumParamEditorHMI.Segm_Indx_Locked = 0;
		else{
			SingleEnumParamEditorHMI.Segm_Indx_Locked = 1;
			SingleEnumParamEditorHMI.Param_Enum_Segm = 0;
		}
		
	}
}						


void Page1_RUN(){ 

	ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parAVC_VoltSP], 1);
	ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parTIG_AmpSP], 1);
	ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parWF_SpeedSP], 1);	

	ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurArc_Volt], 0);
	ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurTIG_Amp], 0);
	ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurWF_Vel], 0);

	ReadScaledVar(&ReadVar[6], &Cfg.ScaledVar[parTVL_VelSP], 1);
	ReadScaledVar(&ReadVar[7], &Cfg.ScaledVar[parOSC_Excur], 1);
	ReadScaledVar(&ReadVar[8], &Cfg.ScaledVar[parWP_SpeedSP], 1);	

	ReadScaledVar(&ReadVar[9], &Cfg.ScaledVar[varCurTVL_Vel], 0);
	ReadScaledVar(&ReadVar[10], &Cfg.ScaledVar[varOSC_Amplit_Cmd], 0);
	ReadScaledVar(&ReadVar[11], &Cfg.ScaledVar[varOSC_Centr], 0);
	


};
void Page2_SEGM_EDITOR(){ 

UINT i , k , m , n;
UINT i_max , k_max;

Unit_Scale_obj *scale;
Scaled_Var_obj *var;


	switch(Page_Step){

		case 0:

			m = 0;
			for(i = 0 ; i < PARAM_NUM_MAX ; i++){
				
				if(CurCfg.Param[i].segmYes){
					SegmentsTable.ParamIndx[m++] = i;	

				}

			}

			SegmentsTable.param_segm_num = m;
			SegmentsTable.segm_num = Master[1].numZone;

			VertCol_Lft_Indx = Master[1].curSegm[ZONE]/4;

			if(Master[1].curSegm[ZONE]%4 > 0)
				VertCol_Indx = Master[1].curSegm[ZONE]%4 - 1;

			Page_Step++;
			

		break;

		case 1:

			

			//SegmentsTable.param_segm_num = m;

			////////////////////////////////

			i_max = (SegmentsTable.param_segm_num < 8)? SegmentsTable.param_segm_num:8;
			k_max = (SegmentsTable.segm_num < 4)? SegmentsTable.segm_num:4;

			if(i_max > 0){

				if(HorLine_Indx >= i_max)
					HorLine_Indx = i_max-1;
			}
			else
				HorLine_Indx = 0;

			if(k_max > 0){

				if(VertCol_Indx >= k_max)
					VertCol_Indx = k_max-1;

			}else
				VertCol_Indx = 0;

			
			if(HorLine_Top_Indx >= (SegmentsTable.param_segm_num - 8))								
				HorLine_Top_Indx = (SegmentsTable.param_segm_num - 8);

			if(VertCol_Lft_Indx >= (SegmentsTable.segm_num - 4))
				VertCol_Lft_Indx = (SegmentsTable.segm_num - 4);


			////////////////////////////////					

			if(SegmentsTable.segm_num < 4 )
				VertCol_Lft_Indx = 0;

			if(SegmentsTable.param_segm_num < 8){							
				HorLine_Top_Indx = 0;

				for(i = 0 ; i < 8 ; i++){

					HorLine_Color[i] = (i == HorLine_Indx)? 1:0;

					if(i < SegmentsTable.param_segm_num){
						HorLine_vis[i] = VISIBLE;

						for(k = 0; k < 4 ; k++){

							if(k < SegmentsTable.segm_num)
								Segm_Var_vis[i][k] = VISIBLE;
							else
								Segm_Var_vis[i][k] = INVISIBLE;

						}	

					}
					else{
						
						HorLine_vis[i] = INVISIBLE;

						for(k = 0; k < 4 ; k++){

							Segm_Var_vis[i][k] = INVISIBLE;

						}
					
					}

				}

			}
			else{

				

				for(i = 0; i < 8 ; i++){
					
					HorLine_Color[i] = (i == HorLine_Indx)? 1:0;

					for(k = 0; k < 4 ; k++){

						if(k < SegmentsTable.segm_num)
							Segm_Var_vis[i][k] = VISIBLE;
						else
							Segm_Var_vis[i][k] = INVISIBLE;

					}
					
					HorLine_vis[i] = VISIBLE;

				}
			}
				
			/////////////

			m = HorLine_Top_Indx; // Horizontal

				
			for(i = 0 ; i < 8 ; i++){ // Fill Veritical

				if(m >= PARAM_NUM_MAX)
					continue;						

				var = &Cfg.ScaledVar[SegmentsTable.ParamIndx[m]];
				scale = &Units[var->unitType].unitScale[curScaledUnitIndx[var->Indx]];
				HorLine_ExtraDig[i] = Cfg.ScaledVar[SegmentsTable.ParamIndx[m]].precision + scale->extraDigits;

				strcpy(Segm_Var_Unit[i] , scale->txt);
				Segm_Var_Indx[i] = SegmentsTable.ParamIndx[m];
				
				n = VertCol_Lft_Indx; // vertical
				
				for(k = 0 ; k < 4 ; k++){ // Fill Horiz								

					if( (i == HorLine_Indx) )
						//|| (k == VertCol_Indx))
						Segm_Var_Color[i][k] = 2; // Grey
					else
						Segm_Var_Color[i][k] = 0;	

					if(n <= SEGM_NUM_MAX ){
						//Segm_Var_Value[i][k] = CurCfg.Param[SegmentsTable.ParamIndx[m]].val[n++];
			
						Segm_Var_Value[i][k] = GetParamValue(&Master[1], &Cfg.ScaledVar[SegmentsTable.ParamIndx[m]] , ++n); // important segment plus 1

					}


					
				}
				strcpy(Segm_Var_Name[i], Cfg.ScaledVar[SegmentsTable.ParamIndx[m++]].name);
											
			}

			//for( i = 0 ; i < 8 ; i++)
				
			//////////////////////////////////////////////////////////////
			Segm_Var_Color[HorLine_Indx][VertCol_Indx] = 1; // Selected

			for(k = 0; k < 4 ; k++)
				Segm_Zone_Indx[k]= VertCol_Lft_Indx + k + 1; //important segment plus 1

			
			if(((HorLine_Indx + HorLine_Top_Indx) < PARAM_NUM_MAX)
				&& ((VertCol_Indx + VertCol_Lft_Indx) < SEGM_NUM_MAX)){

				m = SegmentsTable.ParamIndx[HorLine_Indx + HorLine_Top_Indx];

				ReadVar[0].var = &Cfg.ScaledVar[m];
				ReadVar[0].Segm_Indx = VertCol_Indx + VertCol_Lft_Indx + 1;	//important segment plus 1				
				
			
			}

			Page_Step++;

		break;


		case 2:
			
			if(HorLine_Up_Btn){
				HorLine_Up_Btn = 0;
				Page_Step = 1;

				if(HorLine_Indx == 0){

					if(HorLine_Top_Indx > 0)
						HorLine_Top_Indx--;

				}
				else
					HorLine_Indx--;
			}

			if(HorLine_Dwn_Btn){
				HorLine_Dwn_Btn = 0;
				Page_Step = 1;

				if(HorLine_Indx >= 7)
					HorLine_Top_Indx++;
				else
					HorLine_Indx++;
			}

			/////

			if(VertCol_Left_Btn){
				VertCol_Left_Btn = 0;
				Page_Step = 1;

				if(VertCol_Indx == 0){

					if(VertCol_Lft_Indx > 0)
						VertCol_Lft_Indx--;

				}
				else
					VertCol_Indx--;
			}

			if(VertCol_Right_Btn){
				VertCol_Right_Btn = 0;
				Page_Step = 1;

				if(VertCol_Indx >= 3 )
					VertCol_Lft_Indx ++;
				else
					VertCol_Indx++;
			}


		break;


		default:


		break;			


	}

};
void Page3_LogBook(){ };
void Page4_AplicSetting(){ };
void Page5_GlobalSetting(){ };
void Page6_DrillDown(){ };
void Page7_Diagnostic(){ };
void Page8_Errors(){ };
void Page9_Alt_RUN(){

	ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parAVC_VoltSP], 1);
	ReadScaledVar(&ReadVar[1], &Cfg.ScaledVar[parTIG_AmpSP], 1);
	ReadScaledVar(&ReadVar[2], &Cfg.ScaledVar[parWF_SpeedSP], 1);	

	ReadScaledVar(&ReadVar[3], &Cfg.ScaledVar[varCurArc_Volt], 0);
	ReadScaledVar(&ReadVar[4], &Cfg.ScaledVar[varCurTIG_Amp], 0);
	ReadScaledVar(&ReadVar[5], &Cfg.ScaledVar[varCurWF_Vel], 0);

	ReadScaledVar(&ReadVar[6], &Cfg.ScaledVar[parTVL_VelSP], 1);
	ReadScaledVar(&ReadVar[7], &Cfg.ScaledVar[parOSC_Excur], 1);
	ReadScaledVar(&ReadVar[8], &Cfg.ScaledVar[parOSC_Amplit], 1);	

	ReadScaledVar(&ReadVar[9], &Cfg.ScaledVar[varCurTVL_Vel], 0);
	ReadScaledVar(&ReadVar[10], &Cfg.ScaledVar[varOSC_curExcurTm], 0);
	ReadScaledVar(&ReadVar[11], &Cfg.ScaledVar[varOSC_Amplit_Cmd], 0);
	


};
void Page10_Main_Legacy(){ };
void Page11_Calibration(){ };
void Page12_Files(){ };
void Page13_Main(){


	Locked[0] = (Master[1].SeqStep == 0)? 0:1; // Lock some changes on page
	
	
	ReadEnumVar(&ReadVar_Enum[0] , parENUM_ARCLINK_MODE , 0);
	ReadEnumVar(&ReadVar_Enum[1] , parENUM_AVC_Mode , 0);
	ReadEnumVar(&ReadVar_Enum[2] , parENUM_TIG_Mode , 0);
	ReadEnumVar(&ReadVar_Enum[3] , parENUM_TVL_MODE , 0);
	ReadEnumVar(&ReadVar_Enum[4] , parENUM_TVL_SYN_MODE , 0);

	ReadScaledVar(&ReadVar[0], &Cfg.ScaledVar[parTVL_RunDist], 0);

	
	if(Completed_Flag[0]){
		Completed_Flag[0] = 0;
		Page_Step = 0;
		PleaseUpdateHeadSetup(&Master[1]);

	}


}

void Page20_Errors(){ };


/////////////////////////
void DeleteExtension(char *p){

UINT len , i;

	len = strlen(p);

	for(i=0 ; i < len ; i++){

		if(p[i]=='.'){

			p[i] = 0;
			break;

		}

	}

	return;

}



#endif


