/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Package: Robot
 * File: Hardcoded.c
 * Author: wsi
 * Created: March 08, 2013
 *******************************************************************/
#ifndef _HARDCODED_C
#define _HARDCODED_C



#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobDef.h>
#include <..\GlobDecl.h>
#include <..\..\Lib\LCDLib.c> 



//#include <string.h>

/**
char line[6][4] = {	{0xFF,0xFF,0xFF,0x00},
					{0xFF,0xFF,0xFF,0x00},
					{0xFF,0xFF,0xFF,0x00},
					{0xFF,0xFF,0xFF,0x00},
					{0xFF,0xFF,0xFF,0x00},
					{0xFF,0xFF,0xFF,0x00} }; 

char emptyLine[21] = { 0x20 , 0x00 };

**/
	
static	plcstring txt_example[32];
static	plcstring txt_hidden_example[32];


void HardCodeCFg(MASTER_typ *m);


void SetupNODES_Android(MASTER_typ *m , UDINT pNode ,UINT nodeNum );


void SetPageDict(PAGE_DICT_typ* dict , UINT indx , char* pTxt);


void SetupNODES_Android(MASTER_typ *m , UDINT pNode ,UINT nodeNum ){

int i, k , h , n;
char status;
Cfg_typ *cfg;
CurCfg_typ *curCfg;
CurCalib_typ *curCalib;
Scaled_Var_obj *var , *tempVar;

Enum_Var_obj  *enumVar;
Integer_Var_obj *intVar;

LCD_NODE_obj *node , *tempNode;	
//LCD_MGR_obj *lcd;

		
	if(!m)
		return;

	if(!pNode)
		return;


	node = (LCD_NODE_obj*)pNode; 
	cfg = (Cfg_typ*) m->pCfg;
	curCfg = (CurCfg_typ*) m->pCurCfg;
	var = (Scaled_Var_obj*) m->pScaledVar;
	curCalib = (CurCalib_typ*) m->pCalib;

	enumVar = cfg->EnumVar;

	if(!cfg || ! curCfg || !var || !curCalib)
		return;

	
	/************************************/

	n = 0;
	
	///// MENU ////////////
	/** P_MENU **/
	i = P_MENU;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MENU , "P_MENU");
	

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL   ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = varSTRING_curProgramName;
	strcpy(node[i++].var.Format, "Cur Program: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = varSTRING_NextProgramName;
	strcpy(node[i++].var.Format, "Next Program: %s");	
	

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_EDIT_PARAM_VAL;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Parameters Value");	

	
	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_EDIT_PARAM_VAL;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_BOOSTS];
	strcpy(node[i++].var.Format, "Parameters Boost");	

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_EDIT_PARAM_VAL;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_CONTIN_WELD];
	strcpy(node[i++].var.Format, "Parameters Contin. Weld");	

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_PAR_REVIEW];
	strcpy(node[i++].var.Format, "Timing Review");


	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_VAR_SETUP];
	strcpy(node[i++].var.Format, "Var Unit Setup");
	


	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_SPEC_OPERATION];
	strcpy(node[i++].var.Format, "Special Operation");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = varSTRING_PlcVersion;
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL + N_EVENT ;
	node[i].Event = EVNT_TOGGLE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "COOL PWR is: %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "COOL Sensor is: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "GAS Sensor is: %s");	

	////

	/*
	node[i].pHeadNode	= (UDINT)&node[P_MENU];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_SEGMENTS];
	strcpy(node[i++].var.Format, "Segments Setup");
	*/
			

	/** P_MODE_CALIB**/
	i = P_MODE_CALIB;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_CALIB , "P_MODE_CALIB");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_RETURN_CALIB_MODE;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	strcpy(node[i++].var.Format, "TIG PS Calib.");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	strcpy(node[i++].var.Format, "HW PS Calib");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_OSC_LIMIT_CALIB];
	strcpy(node[i++].var.Format, "OSC Limit Setup");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	strcpy(node[i++].var.Format, "ARC Volt FBK Calib");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	strcpy(node[i++].var.Format, "TVL Calib");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_GO_CALIB_MODE;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_WF_CALIB_PAR];
	strcpy(node[i++].var.Format, "WFeed Calib");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_VER_TIG_PS_CALIB;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TIG_PS_CALIB_VER];
	strcpy(node[i++].var.Format, "TIG PS Test");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_VER_HW_PS_CALIB;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_HW_PS_CALIB_VER];
	strcpy(node[i++].var.Format, "HW PS Test");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_VER_TVL_CALIB;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TVL_CALIB_VER];
	strcpy(node[i++].var.Format, "TVL Test");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_CALIB];
	node[i].Type = N_JUMP + N_EVENT;
	node[i].Event = EVNT_VER_WF_CALIB;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_WF_CALIB_VER];
	strcpy(node[i++].var.Format, "WFeed Test");

	
	/** P_PARAM **/
	i = P_PARAM;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM , "P_PARAM");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");
	
		
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_GAS];
	strcpy(node[i++].var.Format, "Param. GAS/COOL");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_AVC];
	strcpy(node[i++].var.Format, "Param. AVC");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_TIG];
	strcpy(node[i++].var.Format, "Param. TIG");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_OSC];
	strcpy(node[i++].var.Format, "Param. OSC");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_TVL_ROT];
	strcpy(node[i++].var.Format, "Param.TVL");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_WF];
	strcpy(node[i++].var.Format, "Param. WF");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_WP];
	strcpy(node[i++].var.Format, "Param. WP");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_HW];
	strcpy(node[i++].var.Format, "Param. HW");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_WRST];
	strcpy(node[i++].var.Format, "Param. WRIST");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM_INDX];
	strcpy(node[i++].var.Format, "Param. INDX");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WeldProgramLinkMode;
	//strcpy(node[i++].var.Format, "Load Next Prg.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_PathMode;
	//strcpy(node[i++].var.Format, "Swap Head: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_WeldProgramRepeat;
	//strcpy(node[i++].var.Format, "Repeat: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;	
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "** SYNCH. options**");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_TIG_Mode;
	//strcpy(node[i++].var.Format, "TIG Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_AVC_Mode;
	//strcpy(node[i++].var.Format, "AVC Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_TVL_SYN_MODE;
	//strcpy(node[i++].var.Format, "TVL Synch. Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WF_SYN_MODE;
	//strcpy(node[i++].var.Format, "WF Synch. Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WP_SYN_MODE;
	//strcpy(node[i++].var.Format, "WP Synch. Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	i = P_PARAM_APPL;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_APPL , "P_PARAM_APPL");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RunDist;
	//strcpy(node[i++].var.Format, "RUN Dist: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;	
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); // Zone editor

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;	
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); // Track Dia

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_TrackDia;
	//strcpy(node[i++].var.Format, "Tungsten Track Dia: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WeldProgramLinkMode;
	//strcpy(node[i++].var.Format, "Load Next Prg.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_PathMode;
	//strcpy(node[i++].var.Format, "Swap Head: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_WeldProgramRepeat;
	//strcpy(node[i++].var.Format, "Repeat: %s"); 	
	strcpy(node[i++].var.Format, "%name%: %s");	


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;	
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); // Weld Report ON / OFF

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;	
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); // TVL Reverse RampDown ON / OFF

	/*
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_APPL];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;	
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); // Set Tilt Zero
	*/
	

	/** P_PARAM_WRST **/
	i = P_PARAM_WRST;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_WRST , "P_PARAM_WRST");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWRST_JogSpeedDirCntrl;
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_RadSpeed_Boost;
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_POS_PREDICT;
	strcpy(node[i++].var.Format, "%name%: %s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WRST];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

		
	/** P_PARAM_OSC **/
	i = P_PARAM_OSC;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_OSC , "P_PARAM_OSC");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Amplit;
	//strcpy(node[i++].var.Format, "OSC WIDTH %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Excur;
	//strcpy(node[i++].var.Format, "Excurs Tm.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Dwell_In;
	//strcpy(node[i++].var.Format, "Dwell In: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Dwell_Out;
	//strcpy(node[i++].var.Format, "Dwell Out: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_InitAmplit;
	//strcpy(node[i++].var.Format, "OSC INI W %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_FinalAmplit;
	//strcpy(node[i++].var.Format, "OSC FIN W %s");
	strcpy(node[i++].var.Format, "%name%: %s");


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_OSC_Mode;	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_OSC_Synch;	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Indx;
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Period;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_Indx_Vel;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampUpdate;
	//strcpy(node[i++].var.Format, "RampUpd.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_JogSpeed;
	//strcpy(node[i++].var.Format, "Steer Speed: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Cntrl: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_OSC_CAM;	
	//strcpy(node[i++].var.Format, "Cam.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_MaxTvlVel;
	//strcpy(node[i++].var.Format, "Max OSC Vel: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_StartDly;
	//strcpy(node[i++].var.Format, "OSC DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampUp;
	//strcpy(node[i++].var.Format, "O UPSCLOPE %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_StopDly;
	//strcpy(node[i++].var.Format, "O DS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_OSC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampDwn;
	//strcpy(node[i++].var.Format, "O DWNSLOP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	/** P_PARAM_TVL_ROT **/
	i = P_PARAM_TVL_ROT;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_TVL_ROT , "P_PARAM_TVL_ROT");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_VelSP;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_VelInSP;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T PRI IN SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_VelOutSP;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T PRI OUT SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_VelBackSP;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T BCKG SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_OverlapDist;
	//strcpy(node[i++].var.Format, "OVERLAP DIST: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_InitVel;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T INI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_FinalVel;
	node[i].var.Type = T_SCALED;
	//strcpy(node[i++].var.Format, "T FIN SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RampUpdate;
	//strcpy(node[i++].var.Format, "RampUpd.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Cntrl: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RewrapVel;
	//strcpy(node[i++].var.Format, "REWRP SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_TVL_MODE;
	//strcpy(node[i++].var.Format, "TVL Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_Jog2Stop;
	//strcpy(node[i++].var.Format, "Jog2Stop: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_SpeedTolerane;
	//strcpy(node[i++].var.Format, "Vel Toler: %s");
	strcpy(node[i++].var.Format, "%name%: %s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_CalibVel1;
	//strcpy(node[i++].var.Format, "Cal Vel1: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_CalibVel2;
	//strcpy(node[i++].var.Format, "Cal Vel2: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_TrackDia;
	//strcpy(node[i++].var.Format, "Tungsten Track Dia: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_StartDly;
	//strcpy(node[i++].var.Format, "TVL DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RampUp;
	//strcpy(node[i++].var.Format, "T_UPSLOPE %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_StopDly;
	//strcpy(node[i++].var.Format, "T DS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RampDwn;
	//strcpy(node[i++].var.Format, "T DWNSLOP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RewrapDly;
	//strcpy(node[i++].var.Format, "REWRP DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TVL_ROT];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RunDist;
	//strcpy(node[i++].var.Format, "RUN Dist: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	
	/** P_PARAM_WP **/
	i = P_PARAM_WP;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_WP , "P_PARAM_WP");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_SpeedSP;
	//strcpy(node[i++].var.Format, "WP FRG SP %s");
	strcpy(node[i++].var.Format, "%name% %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_SpeedInSP;
	//strcpy(node[i++].var.Format, "WP FRG IN SP %s");
	strcpy(node[i++].var.Format, "%name% %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_SpeedOutSP;
	//strcpy(node[i++].var.Format, "WP FRG OUT SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_BkgSpeedSP;
	//strcpy(node[i++].var.Format, "WP BKG SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_SCROLL;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_SpeedInit;
	//strcpy(node[i++].var.Format, "WP INI SP %s");
	strcpy(node[i++].var.Format, "%name% %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_SpeedFinal;
	//strcpy(node[i++].var.Format, "WP FIN SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_RampUpdate;
	//strcpy(node[i++].var.Format, "RampUpdate: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	/*
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_Boost1SP;
	//strcpy(node[i++].var.Format, "Boost1: %s");
	strcpy(node[i++].var.Format, "%name% %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_Boost2SP;
	//strcpy(node[i++].var.Format, "Boost2: %s");
	strcpy(node[i++].var.Format, "%name% %s");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_JogSpeed;
	//strcpy(node[i++].var.Format, "Jog Vel.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Cntrl: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WF_WP_Relation;
	//strcpy(node[i++].var.Format, "WP Auto Calc.: %s");
	strcpy(node[i++].var.Format, "%name% %s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_StartDly;
	//strcpy(node[i++].var.Format, "WP DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_RampUp;
	//strcpy(node[i++].var.Format, "WP UPSLOP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_StopDly;
	//strcpy(node[i++].var.Format, "WP DS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WP];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_RampDwn;
	//strcpy(node[i++].var.Format, "WP DWNSLP %s");	
	strcpy(node[i++].var.Format, "%name%: %s");	


	/** P_PARAM_WF **/
	i = P_PARAM_WF;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_WF , "P_PARAM_WF");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_SpeedSP;
	//strcpy(node[i++].var.Format, "WF FRG SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_SpeedInSP;
	//strcpy(node[i++].var.Format, "WF FRG IN SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_SpeedOutSP;
	//strcpy(node[i++].var.Format, "WF FRG OUT SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_BkgSpeedSP;
	//strcpy(node[i++].var.Format, "WF BKG SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_SpeedInit;
	//strcpy(node[i++].var.Format, "WF INI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_SpeedFinal;
	//strcpy(node[i++].var.Format, "WF FIN SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RetractSpeed;
	//strcpy(node[i++].var.Format, "WF RET SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RetracDist;
	//strcpy(node[i++].var.Format, "WF UP LEN %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RampUpdate;
	//strcpy(node[i++].var.Format, "RampUpdate: %s");
	strcpy(node[i++].var.Format, "%name%: %s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_WF_AUX; 
	strcpy(node[i++].var.Format, "%name%: %s");


	/*
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_Boost1SP;
	//strcpy(node[i++].var.Format, "Boost1: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_Boost2SP;
	//strcpy(node[i++].var.Format, "Boost2: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_JogSpeed;
	//strcpy(node[i++].var.Format, "Jog Vel.: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Cntrl: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
			
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_CalibVel1;
	//strcpy(node[i++].var.Format, "Cal.Vel1: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_CalibVel2;
	//strcpy(node[i++].var.Format, "Cal.Vel2: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_StartDly;
	//strcpy(node[i++].var.Format, "WF DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RampUp;
	//strcpy(node[i++].var.Format, "WF UPSLOP %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_StopDly;
	//strcpy(node[i++].var.Format, "WF DS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RampDwn;
	//strcpy(node[i++].var.Format, "WF DWNSLP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_WF];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RetractDly;
	//strcpy(node[i++].var.Format, "WFRET DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	

	/** P_PARAM_AVC **/
	i = P_PARAM_AVC;			h=varSTRING_HMI_0;	
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_AVC , "P_PARAM_AVC");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltSP;
	//strcpy(node[i++].var.Format, "PRI VOLTS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltInSP;
	//strcpy(node[i++].var.Format, "PRI IN VOLTS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltOutSP;
	//strcpy(node[i++].var.Format, "PRI OUT VOLTS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltBackSP;
	//strcpy(node[i++].var.Format, "BACK VOLTS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_AVC_Mode;
	//strcpy(node[i++].var.Format, "Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "** ARC Start/Stop **");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_TouchVolt; 
	//strcpy(node[i++].var.Format, "TOUCH V %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltInit; 
	//strcpy(node[i++].var.Format, "AVC INI V %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_TouchVel;
	//strcpy(node[i++].var.Format, "AVC SRI %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_KissTm;
	//strcpy(node[i++].var.Format, "Touch Time %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_ToSPVel;
	//strcpy(node[i++].var.Format, "AVC LU SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_LU_TM;
	//strcpy(node[i++].var.Format, "AVC LU TM %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VoltFinal;
	//strcpy(node[i++].var.Format, "FIN VOLTS %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_GoBackTm;
	//strcpy(node[i++].var.Format, "AVC UP TM %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_LiftSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "AVC UP SP %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_TouchTm;
	//strcpy(node[i++].var.Format, "Touch TMO: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_TRetractTm;
	//strcpy(node[i++].var.Format, "Retract: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_TouchRepeat;
	//strcpy(node[i++].var.Format, "Repeat Cnt: %s");
	strcpy(node[i++].var.Format, "%name%: %s");


	/*
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_StartDly;
	//strcpy(node[i++].var.Format, "V UPS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_VSPTmOut;
	//strcpy(node[i++].var.Format, "Init Volt Tmo: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_MaxVoltError;
	//strcpy(node[i++].var.Format, "Max Volt Err: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Cntrl: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	/*
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_Limits_Mask_Tm;
	strcpy(node[i++].var.Format, "Lim Filt: %s");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_FbkFilter;
	//strcpy(node[i++].var.Format, "Fbk Filtr: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_KPP;
	//strcpy(node[i++].var.Format, "KPP: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_KIP;
	//strcpy(node[i++].var.Format, "KIP: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_DB_Window;
	//strcpy(node[i++].var.Format, "DB Window: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_BB_Vel;
	//strcpy(node[i++].var.Format, "BB Speed: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_BB_Gain;
	//strcpy(node[i++].var.Format, "BB Gain: %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_BB_Integr;
	strcpy(node[i++].var.Format, "%name%: %s");	
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");	


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_Upslope;
	//strcpy(node[i++].var.Format, "V UPSLOPE %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_StopDly;
	//strcpy(node[i++].var.Format, "V DS DLY %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_DownSlope;
	//strcpy(node[i++].var.Format, "V DWNSLOP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_AVC];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_HoldTm;
	//strcpy(node[i++].var.Format, "AVCUP DLY %s");		
	strcpy(node[i++].var.Format, "%name%: %s");	


	/** P_PARAM_TIG **/
	i = P_PARAM_TIG;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_TIG , "P_PARAM_TIG");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_AmpSP;
	//strcpy(node[i++].var.Format, "PRI AMPS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_AmpInSP;
	//strcpy(node[i++].var.Format, "PRI IN AMPS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_AmpOutSP;
	//strcpy(node[i++].var.Format, "PRI OUT AMPS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_BackAmp;
	//strcpy(node[i++].var.Format, "BKG AMPS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_PulseFreq;
	//strcpy(node[i++].var.Format, "Pulse Freq. %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_PulseWidth;
	//strcpy(node[i++].var.Format, "Pulse Width %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_TIG_Mode;	
	//strcpy(node[i++].var.Format, "Mode: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_StartAmp;
	//strcpy(node[i++].var.Format, "INI AMPS %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_EndAmp;
	//strcpy(node[i++].var.Format, "FIN AMPS %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_MANAGER;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_AmpSP_Boost;
	//strcpy(node[i++].var.Format, "Amp Boost: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_ENUM;
	node[i].var.Var1ID = parENUM_ARCLINK_MODE;	
	//strcpy(node[i++].var.Format, "ArcLink: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_MANAGER;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_CalibAmp1;
	//strcpy(node[i++].var.Format, "Cal. Amp1: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_MANAGER;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_CalibAmp2;
	//strcpy(node[i++].var.Format, "Cal. Amp2: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_StartDly;
	//strcpy(node[i++].var.Format, "I DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_RampUp;
	//strcpy(node[i++].var.Format, "I UPSLOPE %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_StopDly;
	//strcpy(node[i++].var.Format, "I DS DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_TIG];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_RampDwn;
	//strcpy(node[i++].var.Format, "I DOWNSLOP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	/** P_PARAM_HW **/
	i = P_PARAM_HW;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_HW , "P_PARAM_HW");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_AmpSP;
	//strcpy(node[i++].var.Format, "HW PRI A %s");
	strcpy(node[i++].var.Format, "%name%: %s");
		
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_StartAmp;
	//strcpy(node[i++].var.Format, "HW INI A %s");
	strcpy(node[i++].var.Format, "%name%: %s");
		
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_EndAmp;
	//strcpy(node[i++].var.Format, "HW FIN A %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_VoltSP;
	//strcpy(node[i++].var.Format, "Prim Volt: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_MANAGER;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_AmpSP_Boost;
	//strcpy(node[i++].var.Format, "Amp Boost: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_CalibAmp1;
	//strcpy(node[i++].var.Format, "Cal. Amp1: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_CalibAmp2;
	//strcpy(node[i++].var.Format, "Cal. Amp2: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampUpDly;
	//strcpy(node[i++].var.Format, "HW US DLY %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampUp;
	//strcpy(node[i++].var.Format, "HW UPSLOP %s"); 
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampDwnDly;
	//strcpy(node[i++].var.Format, "HW DS DLY %s"); 	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_HW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampDwn;
	//strcpy(node[i++].var.Format, "HW DWNSLOP %s"); 
	strcpy(node[i++].var.Format, "%name%: %s");


	/** P_PARAM_BOOSTS **/
	i = P_PARAM_BOOSTS;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_BOOSTS , "P_PARAM_BOOSTS");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, " *BOOST 1 - UP Key* ");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_Boost1;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_Boost1;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_Boost1;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_Boost1SP;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_Boost1SP;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, " *BOOST 2 - Down Key* ");
	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_Boost2;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_Boost2;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");


	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_Boost2;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_Boost2SP;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_BOOSTS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_Boost2SP;
	//strcpy(node[i++].var.Format, "T PRI SP %s");
	strcpy(node[i++].var.Format, "%name%: %s");	


	/** P_PARAM_CONTIN_WELD **/
	i = P_PARAM_CONTIN_WELD;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_CONTIN_WELD , "P_PARAM_CONTIN_WELD");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** Path Correction");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "%s");  

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "%s");  


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** TVL in Contin. Weld");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TVL_RampDwn;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TVL_RampUp;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** OSC Ramps");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampDwn;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampUp;
	strcpy(node[i++].var.Format, "%name%: %s");	


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** TIG AMP in Contin. Weld");	


	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TIG_Percent;
	strcpy(node[i++].var.Format, "%name%: %s");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TIG_RampDwn;
	strcpy(node[i++].var.Format, "%name%: %s");	

	/*
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TIG_Dwell;
	strcpy(node[i++].var.Format, "%name%: %s");	
	*/

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_TIG_RampUp;
	strcpy(node[i++].var.Format, "%name%: %s");	
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** WF in Contin. Weld");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_WF_RampDwn;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_WF_RampUp;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "*** WP in Contin. Weld");	

	
	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_WP_RampDwn;
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_CONTIN_WELD];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCONTIN_WP_RampUp;
	strcpy(node[i++].var.Format, "%name%: %s");	
	
	

	/** P_PARAM_INDX **/
	i = P_PARAM_INDX;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_HW , "P_PARAM_INDX");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parINDX_StepDist;
	//strcpy(node[i++].var.Format, "Step Dist. %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parINDX_StepAfterBeats;
	//strcpy(node[i++].var.Format, "Step After Beats %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parINDX_StepVel;
	//strcpy(node[i++].var.Format, "Step Vel. %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parINDX_JogSpeedDirCntrl;
	//strcpy(node[i++].var.Format, "Jog Speed %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	/*
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "JOG ONLY A");	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "JOG ONLY B"); 

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_INDX];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_SET_INDX_ZERO;
	strcpy(node[i++].var.Format, "Set Zero"); 
*/

	
	/** P_PARAM_GAS **/
	i = P_PARAM_GAS;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_GAS , "P_PARAM_GAS");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_PARAM];
	strcpy(node[i++].var.Format, "Return ");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGAS_PrePurgeTm;
	//strcpy(node[i++].var.Format, "PREPURGE %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode 	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "******* MSC. *******");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGAS_Tmo;
	//strcpy(node[i++].var.Format, "Argon TMO: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parCOOL_Tmo;
	//strcpy(node[i++].var.Format, "Cool TMO: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_SCROLL + N_NO_PREFIX;
	node[i].MinUserLevel = 5;
	strcpy(node[i++].var.Format, "**** CALCULATED ****");
	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GAS];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = 5;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGAS_PostPurgeTm;
	//strcpy(node[i++].var.Format, "PostPurge %s");	
	strcpy(node[i++].var.Format, "%name%: %s");	


	/** P_VAR_SETUP **/
	i = P_VAR_SETUP;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_VAR_SETUP , "P_VAR_SETUP");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");

	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_AmpRead;
	//strcpy(node[i++].var.Format, "Cal.Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_CalibDist;
	//strcpy(node[i++].var.Format, "Cal.Dist: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_CalibDist_2;
	//strcpy(node[i++].var.Format, "Cal.Dist: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGEN_CalibTime;
	//strcpy(node[i++].var.Format, "Cal.Time: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	

	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varOSC_Centr;
	//strcpy(node[i++].var.Format, "actOSC Centr: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varOSC_CurPos;
	//strcpy(node[i++].var.Format, "actOSC Pos: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = varHW_Amp_Cmd;
	//strcpy(node[i++].var.Format, "actHW Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurArc_Volt;
	//strcpy(node[i++].var.Format, "actTIG Volt: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurTIG_Amp;
	//strcpy(node[i++].var.Format, "actTIG Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varTIG_Amp_Cmd;
	//strcpy(node[i++].var.Format, "cmdTIG Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurTVL_Vel;
	//strcpy(node[i++].var.Format, "actTVL Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurWF_Vel;
	//strcpy(node[i++].var.Format, "actWF Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurWP_Vel;
	//strcpy(node[i++].var.Format, "actWP Freq: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_VAR_SETUP];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurWFDist;
	//strcpy(node[i++].var.Format, "actWF Dist: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	
	
	/** P_PARAM_GLOBAL **/
	i = P_PARAM_GLOBAL;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_PARAM_GLOBAL , "P_PARAM_GLOBAL");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "%s");

		
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "%s");

	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// Use Mapping
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");


	/// Auto Start
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
	
	// Auto Start Time
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	
	// Single Start Sound ON / OFF
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// Start Sound Time
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	
	// INDEX Enable
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// INDEX Direction
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	//SETUP_ZONE_CENTR
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// SETUP_TWO_TORCH
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// SETUP_ONE_UP
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	//SETUP_CONTIN_WELD
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	//SETUP_TVL_ENC_SET
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	// SETUP_TVL_SEC_ENC
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, " ******** ");


	/// HEAD TYPE	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	/// Legacy AVC	

	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	/// WF Swap	
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	/// WF Stop Ramp
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	/// WF Start Ramp
	h=varSTRING_HMI_0; 
	node[i].pHeadNode	= (UDINT)&node[P_PARAM_GLOBAL];
	node[i].Type = N_SCROLL  + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
		
	
	// P_MODE_WF_CALIB_PAR
	i= P_MODE_WF_CALIB_PAR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_WF_CALIB_PAR , "P_MODE_WF_CALIB_PAR");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parWF_CalibVel1;
	//strcpy(node[i++].var.Format, "Low Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parWF_CalibVel2;
	//strcpy(node[i++].var.Format, "High Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist;
	//strcpy(node[i++].var.Format, "Length: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist_2;
	//strcpy(node[i++].var.Format, "Length: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		
	

	// P_MODE_WF_CALIB
	i= P_MODE_WF_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_WF_CALIB , "P_MODE_WF_CALIB");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist1;
	//strcpy(node[i++].var.Format, "(1)Length at Head A Low Vel.:  %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist2;
	//strcpy(node[i++].var.Format, "(2)Length at Head A High Vel.:  %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist3;
	//strcpy(node[i++].var.Format, "(3)Length at Head B Low Vel.:  %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist4;
	//strcpy(node[i++].var.Format, "(4)Length at Head B High Vel.:  %s");
	strcpy(node[i++].var.Format, "%name%: %s");	
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, " %s");

	// P_MODE_WF_CALIB_VER
	i= P_MODE_WF_CALIB_VER;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_WF_CALIB_VER , "P_MODE_WF_CALIB_VER");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	strcpy(node[i++].var.Format, "Repeat !!!");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
		

	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	


	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_WF_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	strcpy(node[i++].var.Format, "Save Calibration !!!");
		
	
	//P_MODE_TIG_PS_CALIB_PAR
	i= P_MODE_TIG_PS_CALIB_PAR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TIG_PS_CALIB_PAR , "P_MODE_TIG_PS_CALIB_PAR");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTIG_CalibAmp1;
	//strcpy(node[i++].var.Format, "Low Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTIG_CalibAmp2;
	//strcpy(node[i++].var.Format, "High Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_ENUM;	
	node[i].var.Var1ID = parENUM_TIG_Cal_Mode;
	strcpy(node[i++].var.Format, "%name%: %s");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
	
	//P_MODE_TIG_PS_CALIB
	i= P_MODE_TIG_PS_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TIG_PS_CALIB , "P_MODE_TIG_PS_CALIB");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibAmp1;
	//strcpy(node[i++].var.Format, "(1)Low Amps:  %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibAmp2;
	//strcpy(node[i++].var.Format, "(2)High Amps: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, " %s");
	
	

	// P_MODE_TIG_PS_CALIB_VER
	i= P_MODE_TIG_PS_CALIB_VER;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TIG_PS_CALIB_VER , "P_MODE_TIG_PS_CALIB_VER");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	strcpy(node[i++].var.Format, "Repeat !!!");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_PS_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	strcpy(node[i++].var.Format, "Save Calibration !!!");	


	//P_MODE_HW_PS_CALIB_PAR
	i= P_MODE_HW_PS_CALIB_PAR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_HW_PS_CALIB_PAR , "P_MODE_HW_PS_CALIB_PAR");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parHW_CalibAmp1;
	//strcpy(node[i++].var.Format, "Low Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parHW_CalibAmp2;
	//strcpy(node[i++].var.Format, "High Amp: %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
	
	//P_MODE_HW_PS_CALIB
	i= P_MODE_HW_PS_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_HW_PS_CALIB , "P_MODE_HW_PS_CALIB");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibAmp1;
	//strcpy(node[i++].var.Format, "(1)Low Amps:  %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibAmp2;
	//strcpy(node[i++].var.Format, "(2)High Amps: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, " %s");
	
	

	// P_MODE_HW_PS_CALIB_PAR
	i= P_MODE_HW_PS_CALIB_VER;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_HW_PS_CALIB_VER , "P_MODE_HW_PS_CALIB_VER");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	strcpy(node[i++].var.Format, "Repeat !!!");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_HW_PS_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	strcpy(node[i++].var.Format, "Save Calibration !!!");
	

	// P_MODE_OSC_LIMIT_CALIB
	i= P_MODE_OSC_LIMIT_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_OSC_LIMIT_CALIB , "P_MODE_OSC_LIMIT_CALIB");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_OSC_LIMIT_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_SWAP_HEAD;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	
	strcpy(node[i++].var.Format, "%s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_OSC_LIMIT_CALIB];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = varOSC_CurRawPos;	
	//strcpy(node[i++].var.Format, "actPos FBK: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_OSC_LIMIT_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OSC_POS_LIMIT_SET;
	strcpy(node[i++].var.Format, "Store Pos Limit");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_OSC_LIMIT_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OSC_NEG_LIMIT_SET;
	strcpy(node[i++].var.Format, "Store Neg Limit");



	// P_MODE_TIG_FBK_CALIB
	i= P_MODE_TIG_FBK_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TIG_FBK_CALIB , "P_MODE_TIG_FBK_CALIB");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = varCurArc_Volt;
	//strcpy(node[i++].var.Format, "act Arc: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL +   N_EDIT_IN_PLACE ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_SET_TIG_FBK_CALIB_POINT;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, " ******** ");


	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, " ******** ");
	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TIG_FBK_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_LOAD_DEFAULT_CALIB;
	strcpy(node[i++].var.Format, "Load Default Calibr. !!!");


	// P_MODE_INDX_CALIB
	i= P_MODE_INDX_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_INDX_CALIB , "P_MODE_INDX_CALIB");

	/*
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_CALIB];
	strcpy(node[i++].var.Format, "Cancel/Return ");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_MODE_INDX_CALIB];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "JOG BOTH"); 	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_INDX_CALIB];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "JOG A"); 	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_INDX_CALIB];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "JOG B"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_INDX_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_SET_INDX_ZERO;
	strcpy(node[i++].var.Format, "Set Zero"); 
	


	// P_MODE_TVL_CALIB_PAR
	i= P_MODE_TVL_CALIB_PAR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TVL_CALIB_PAR , "P_MODE_TVL_CALIB_PAR");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_CalibVel1;
	//strcpy(node[i++].var.Format, "Low Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_CalibVel2;
	//strcpy(node[i++].var.Format, "High Vel: %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist;
	//strcpy(node[i++].var.Format, "Dist.: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibDist_2;
	//strcpy(node[i++].var.Format, "Dist.: %s");	
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	




	// P_MODE_TVL_CALIB
	i= P_MODE_TVL_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TVL_CALIB , "P_MODE_TVL_CALIB");

	/*
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MODE_TVL_CALIB_PAR];
	strcpy(node[i++].var.Format, "Cancel/Return ");
	*/

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");		

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibTime1;
	//strcpy(node[i++].var.Format, "(1)Time Low Vel. FWD:   %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibTime2;
	//strcpy(node[i++].var.Format, "(2)Time Low Vel. REV:   %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibTime3;
	//strcpy(node[i++].var.Format, "(3)Time High Vel. FWD:   %s");	
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_CalibTime4;
	//strcpy(node[i++].var.Format, "(4)Time High Vel. REV:   %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, " %s");	
	

	// P_MODE_TVL_CALIB_VER
	i= P_MODE_TVL_CALIB_VER;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TVL_CALIB_VER , "P_MODE_TVL_CALIB_VER");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_CANCEL;
	strcpy(node[i++].var.Format, "Repeat !!!");	//ActPos:

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	
		

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

		
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TVL_CALIB_VER];
	node[i].Type = N_SCROLL +  N_EVENT ;
	node[i].Event = EVNT_OK;
	//node[i].var.Type = T_STRING;
	//node[i].var.Var1ID = (h++);
	//strcpy(node[i++].var.Format, "%s");	
	strcpy(node[i++].var.Format, "Save Calibration !!!");	

	///////////////////////////////////////

	i= P_MODE_TRACK_CALIB_PAR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TRACK_CALIB_PAR , "P_MODE_TRACK_CALIB_PAR");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parGEN_Radius;
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_EDIT;
	node[i].var.Type = T_SCALED;	
	node[i].var.Var1ID = parTVL_POS_OFFSET; 
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	


	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_PAR];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ACKN;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");	


	// P_MODE_TRACK_CALIB_PAR
	i= P_MODE_TRACK_CALIB;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TRACK_CALIB, "P_MODE_TRACK_CALIB");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ACKN;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");
	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ACKN_ALL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 
	
	

	// P_MODE_TRACK_CALIB_VER
	i= P_MODE_TRACK_CALIB_VER;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_MODE_TRACK_CALIB_VER , "P_MODE_TRACK_CALIB_VER");

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_CANCEL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ACKN;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");

	
	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);  
	strcpy(node[i++].var.Format, "%s");
	

	node[i].pHeadNode	= (UDINT)&node[P_MODE_TRACK_CALIB_VER];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s"); 
		
	/////////////////////////////////////
	
	// P_SPEC_OPERATION
	i = P_SPEC_OPERATION;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_SPEC_OPERATION , "P_SPEC_OPERATION");
	
	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, " Return !!!");	

	
	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_SET_AMERICAN_UNIT;
	strcpy(node[i++].var.Format, "Set Default Units !");

	
	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_SET_NORMAL_UNIT;
	strcpy(node[i++].var.Format, "Set SI Units !!!");

	
	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_LOAD_DEFAULT;
	strcpy(node[i++].var.Format, "Load Defaults !!!");

	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_LOAD_DEFAULT_CALIB;
	strcpy(node[i++].var.Format, "Load Default Calib !");

	
	node[i].pHeadNode	= (UDINT)&node[P_SPEC_OPERATION];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ENT_BTN_REQ;
	strcpy(node[i++].var.Format, "Enter Btn Req!!");

	
	
	
	/** P_DIAGNOSTIC **/	
	 i = P_DIAGNOSTIC;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGNOSTIC , "P_DIAGNOSTIC");

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = varSTRING_MasteStepTXT;
	strcpy(node[i++].var.Format, "M: %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGN_PROC];
	strcpy(node[i++].var.Format, "Diagn PROCESS"); 

	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGN_TML];
	strcpy(node[i++].var.Format, "Diagn TML Servo");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGN_DC_MTR];
	strcpy(node[i++].var.Format, "Diagn DC Mtrs");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGN_AXIS];
	strcpy(node[i++].var.Format, "Diagn Axises ");

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGN_IO];
	strcpy(node[i++].var.Format, "Diagn I/O");
	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGNOSTIC];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_RESTART ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	 
	strcpy(node[i++].var.Format, "RESTART !!!");



	/** P_DIAGN_TML **/
	i = P_DIAGN_TML;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGN_TML , "P_DIAGN_TML");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGNOSTIC];
	strcpy(node[i++].var.Format, " Return !!!");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];	
	node[i].Type =    N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	 
	strcpy(node[i++].var.Format,"%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "Stat: %s ");


	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL +	N_EVENT ;
	node[i].Event = EVNT_TOGGLE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");


	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "EnInp: %s");  

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s "); // Inp 0 & 1

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s "); //Inp 2 & 3

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s "); //Out 0 & 1

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s "); //Out 4

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "xCMD: %s ");


	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s "); //Fbk: 

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EVENT;
	strcpy(node[i++].var.Format, "Jog Fwd");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EVENT;
	strcpy(node[i++].var.Format, "Jog Rev");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_OK;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);		
	strcpy(node[i++].var.Format, "Set ABS Home  !!!");
	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_LOAD;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "LOAD Setup !!!");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_TML];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_RESTART;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);		
	strcpy(node[i++].var.Format, "RESTART !!!");
	

	/** P_DIAGN_PROC **/
	i = P_DIAGN_PROC;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGN_PROC , "P_DIAGN_PROC");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGNOSTIC];
	strcpy(node[i++].var.Format, " Return !!!");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];	
	node[i].Type =    N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	 
	strcpy(node[i++].var.Format,"%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "Stat: %s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];
	node[i].Type = N_SCROLL +	N_EVENT ;
	node[i].Event = EVNT_TOGGLE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_PROC];
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_RESTART ;
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "RESTART !!!");
	

	/** P_DIAGN_DC_MTR **/
	i = P_DIAGN_DC_MTR;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGN_DC_MTR , "P_DIAGN_DC_MTR");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGNOSTIC];
	strcpy(node[i++].var.Format, " Return !!!");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];	
	node[i].Type =    N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	 
	strcpy(node[i++].var.Format,"%s");


	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_SCROLL +	N_EVENT ;
	node[i].Event = EVNT_TOGGLE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");
	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "Fbk: %s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_SCROLL + N_EVENT;
	strcpy(node[i++].var.Format, "Jog Fwd");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_DC_MTR];
	node[i].Type = N_SCROLL + N_EVENT;
	strcpy(node[i++].var.Format, "Jog Rev");


	/** P_DIAGN_AXIS **/
	i = P_DIAGN_AXIS;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGN_AXIS , "P_DIAGN_AXIS");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGNOSTIC];
	strcpy(node[i++].var.Format, " Return !!!");
		
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];	
	node[i].Type =    N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	 
	strcpy(node[i++].var.Format,"%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];
	node[i].Type = N_SCROLL;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];
	node[i].Type = N_SCROLL ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];
	node[i].Type = N_SCROLL ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_AXIS];
	node[i].Type = N_SCROLL ;
	node[i].var.Type = T_STRING;
	node[i].var.Var1ID = (h++);
	strcpy(node[i++].var.Format, "%s ");


	/** P_DIAGN_IO **/
	i = P_DIAGN_IO;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_DIAGN_IO , "P_DIAGN_IO");
	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_DIAGNOSTIC];
	strcpy(node[i++].var.Format, " Return !!!");	
	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");

	
	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	

	node[i].pHeadNode	= (UDINT)&node[P_DIAGN_IO];
	node[i].Type = N_SCROLL;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	
	

	
	/** P_LOGS **/	
	 i = P_LOGS;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_LOGS , "P_LOGS");
	
	
	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");


	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	


	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	


	node[i].pHeadNode	= (UDINT)&node[P_LOGS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "%s");	
	
		
	/** P_ERRORS **/	
	 i = P_ERRORS;			h=varSTRING_HMI_0;
	SetPageDict(&cfg->PagesDict[n++] , P_ERRORS , "P_ERRORS");
	

	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;	 
	strcpy(node[i++].var.Format, "%s");

	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "  %s");

	
	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "  %s");

	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];
	node[i].Type = N_SCROLL + N_EDIT_IN_PLACE;
	node[i].var.Var1ID = (h++);
	node[i].var.Type = T_STRING;
	strcpy(node[i++].var.Format, "  %s");


	node[i].pHeadNode	= (UDINT)&node[P_ERRORS];	
	node[i].Type = N_SCROLL + N_EVENT;
	node[i].Event = EVNT_ACKN_ALL;
	strcpy(node[i++].var.Format, " ACK All Errors !!!");		
			
	
	//////////////////////////////////////////////////////////
	//////////    FOR TESTING ONLY -


		
	/** P_PAR_REVIEW **/
	i = P_PAR_REVIEW;			h=varSTRING_HMI_0; 
	SetPageDict(&cfg->PagesDict[n++] , P_PAR_REVIEW , "P_PAR_REVIEW");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_JUMP;
	node[i].JumpLineID = 0;
	node[i].pJumpNode = (UDINT)&node[P_MENU];
	strcpy(node[i++].var.Format, "Return ");

	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** AVC *****");	


	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_StartDly;
	//strcpy(node[i++].var.Format, "AVC UpslopeDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_Upslope;
	//strcpy(node[i++].var.Format, "AVC Upslope %s");
	strcpy(node[i++].var.Format, "%name%: %s");	

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_StopDly;
	//strcpy(node[i++].var.Format, "AVC StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_DownSlope;
	//strcpy(node[i++].var.Format, "AVC DownSlope %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parAVC_HoldTm;
	//strcpy(node[i++].var.Format, "AVC HoldTm %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** TVL *****"); 

	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_StartDly;
	//strcpy(node[i++].var.Format, "TVL_StartDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RampUp;
	//strcpy(node[i++].var.Format, "TVL RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_StopDly;
	//strcpy(node[i++].var.Format, "TVL StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RampDwn;
	//strcpy(node[i++].var.Format, "TVL RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_RewrapDly;
	//strcpy(node[i++].var.Format, "TVL RewrapDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTVL_OverlapDist;
	//strcpy(node[i++].var.Format, "TVL Overlap %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** TIG AMP *****");	

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_StartDly;
	//strcpy(node[i++].var.Format, "TIG StartDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_RampUp;
	//strcpy(node[i++].var.Format, "TIG RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_StopDly;
	//strcpy(node[i++].var.Format, "TIG StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parTIG_RampDwn;
	//strcpy(node[i++].var.Format, "TIG RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** OSC *****"); 

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_StartDly;
	//strcpy(node[i++].var.Format, "OSC StartDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampUp;
	//strcpy(node[i++].var.Format, "OSC RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_StopDly;
	//strcpy(node[i++].var.Format, "OSC StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parOSC_RampDwn;
	//strcpy(node[i++].var.Format, "OSC RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** WF *****");

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_StartDly;
	//strcpy(node[i++].var.Format, "parWF StartDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RampUp;
	//strcpy(node[i++].var.Format, "WF RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_StopDly;
	//strcpy(node[i++].var.Format, "WF StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWF_RampDwn;
	//strcpy(node[i++].var.Format, "WF RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** WP *****");

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_StartDly;
	//strcpy(node[i++].var.Format, "WP StartDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_RampUp;
	//strcpy(node[i++].var.Format, "WP RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_StopDly;
	//strcpy(node[i++].var.Format, "WP StopDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parWP_RampDwn;
	//strcpy(node[i++].var.Format, "WP RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** HW *****");

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampUpDly;
	//strcpy(node[i++].var.Format, "HW RampUpDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampUp;
	//strcpy(node[i++].var.Format, "HW RampUp %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampDwnDly;
	//strcpy(node[i++].var.Format, "HW RampDwnDly %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parHW_RampDwn;
	//strcpy(node[i++].var.Format, "HW RampDwn %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_SCROLL;
	strcpy(node[i++].var.Format, "**** GAS *****"); 

	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGAS_PrePurgeTm;
	//strcpy(node[i++].var.Format, "GAS PrePurgeTm %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	
	node[i].pHeadNode	= (UDINT)&node[P_PAR_REVIEW];
	node[i].Type = N_EDIT;
	node[i].MinUserLevel = USER_OPERATOR;
	node[i].var.Type = T_SCALED;
	node[i].var.Var1ID = parGAS_PostPurgeTm;
	//strcpy(node[i++].var.Format, "GAS PostPurgeTm %s");
	strcpy(node[i++].var.Format, "%name%: %s");
	

	//////////////////////////////////////////////////////////

			
	m->nodesNum[0] = nodeNum;	

	/////////////////////////
	// Populate nodes ID //////
	for(i = 0 ; i < nodeNum ; i ++){
		node[i].ID = (UINT)i;

		// Sanitary - remove old absolete attributes - just in case
		node[i].Type = node[i].Type & (0xFFFF - (N_EDIT_DIS + N_NO_PREFIX + N_USER_EDIT + N_ONLY_LCD));

	}

	
	// Populate HeadID
	for(i = 0 ; i < nodeNum ; i ++){

		if(	node[i].pHeadNode){
			tempNode  = (LCD_NODE_obj*)node[i].pHeadNode;

			if(tempNode)
				node[i].HeadID = tempNode->ID;
		}
		else
			node[i].pHeadNode = 0;
	}	

	// Populate JumpNodeID	
	for(i = 0 ; i < nodeNum ; i ++){
		
		tempNode = (LCD_NODE_obj*)node[i].pJumpNode;
		
		if(tempNode)
			node[i].JumpNodeID = tempNode->ID;
		

	}	
	

}



void HardCodeCFg(MASTER_typ *m){

int i, k;
char status;
Cfg_typ *cfg;
CurCfg_typ *curCfg;
Scaled_Var_obj *var;
CurCalib_typ *defCalib , *calib;




	if(!m)
		return;

	cfg = (Cfg_typ*) m->pCfg;
	curCfg = (CurCfg_typ*) m->pCurCfg;
	var = (Scaled_Var_obj*) m->pScaledVar;
	defCalib =  (CurCalib_typ*) m->pDefaultCalib;	
	calib =	(CurCalib_typ*) m->pCalib;


	if(!cfg || ! curCfg || !var || !defCalib || !calib)
		return;

	

	//////////////////////////////////////////////////////
	//   Button Mode when edit value  - 1.- hold button
	i = parLCD_BtnMode;
	strcpy(var[i].name , "Edit Button Mode");
	strcpy(var[i].enumTXT, "parLCD_BtnMode");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 2;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;

	
	//////////////////////////////////////////////////////
	//   OSCILLATION 


	i = parOSC_Indx_Vel;
	strcpy(var[i].name , "OSC Index Speed");
	strcpy(var[i].enumTXT, "parOSC_Indx_Vel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_sec;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.01 * in_2_mm / 1.0;
	cfg->Param[i].max = 1.0 * in_2_mm / 1.0;
	cfg->Param[i].incrMin = 0.01 * in_2_mm / 1.0;	
	cfg->Param[i].incrMax = 5 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.25 * in_2_mm / 1.0;
	var[i].segm = 0;

	i = parOSC_StartDly;
	strcpy(var[i].name , "OSC START DLY");
	strcpy(var[i].enumTXT, "parOSC_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;	
	var[i].segm = 0;
		

	i = parOSC_RampUp;
	strcpy(var[i].name , "OSC UPSLOPE TM");
	strcpy(var[i].enumTXT, "parOSC_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 4.0;
	var[i].segm = 0;

	i = parOSC_Amplit;
	strcpy(var[i].name , "OSC WIDTH");
	strcpy(var[i].enumTXT, "parOSC_Amplit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	// Was changed to 3 inch 12 June 2020 by Mark request
	cfg->Param[i].max = 3.0 * 25.4;
	cfg->Param[i].incrMin = 0.01 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 0.35 * in_2_mm ; 
	var[i].segm = 1;
	

	i = parOSC_FinalAmplit;
	strcpy(var[i].name , "OSC FINAL WIDTH");
	strcpy(var[i].enumTXT, "parOSC_FinalAmplit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 25.4;
	cfg->Param[i].incrMin = 0.05 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 0.05 * in_2_mm ; // 1/4 " = 6.35 mm
	var[i].segm = 0;


	i = parOSC_InitAmplit;
	strcpy(var[i].name , "OSC INITIAL WIDTH");
	strcpy(var[i].enumTXT, "parOSC_InitAmplit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 25.4;
	cfg->Param[i].incrMin = 0.05 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 0.0 * in_2_mm ; // 1/4 " = 6.35 mm
	var[i].segm = 0;

	i = parOSC_Indx;
	strcpy(var[i].name , "OSC Index");
	strcpy(var[i].enumTXT, "parOSC_Indx");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = -25.4;
	cfg->Param[i].max = 25.4;
	cfg->Param[i].incrMin = 0.01 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 0.25 * in_2_mm ; // 1/4 " = 6.35 mm
	var[i].segm = 0;

	i = parOSC_Period;
	strcpy(var[i].name , "OSC Period");
	strcpy(var[i].enumTXT, "parOSC_Period");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType =  unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 100000.0;
	cfg->Param[i].incrMin = 1;
	cfg->Param[i].incrMax = 1;
	cfg->Param[i].defaultVal = 40;
	var[i].segm = 0;
	

	i = parOSC_StopDly;
	strcpy(var[i].name , "OSC DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parOSC_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 7.0;
	var[i].segm = 0;
	
	i = parOSC_RampDwn;
	strcpy(var[i].name , "OSC DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parOSC_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;

	i = parOSC_RampUpdate;
	strcpy(var[i].name , "Osc Ramp Update");
	strcpy(var[i].enumTXT, "parOSC_RampUpdate");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.1;
	var[i].segm = 0;

	i = parOSC_Excur;
	strcpy(var[i].name , "Osc Excursion");
	strcpy(var[i].enumTXT, "parOSC_Excur");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.05;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 1;
	

	i = parOSC_Dwell_In;
	strcpy(var[i].name , "Osc Dwell In");
	strcpy(var[i].enumTXT, "parOSC_Dwell_In");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 6.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 1;

	i = parOSC_Dwell_Out;
	strcpy(var[i].name , "Osc Dwell Out");
	strcpy(var[i].enumTXT, "parOSC_Dwell_Out");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 6.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 1;
	
	i = parOSC_JogSpeedDirCntrl;
	strcpy(var[i].name , "OSC Jog Speed");
	strcpy(var[i].enumTXT, "parOSC_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.75;
	var[i].segm = 0;

	i = parOSC_JogSpeed;
	strcpy(var[i].name , "OSC Jog Speed");
	strcpy(var[i].enumTXT, "parOSC_JogSpeed");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.3;
	var[i].segm = 0;

	i = parOSC_MaxTvlVel;
	strcpy(var[i].name , "Max OSC Speed");
	strcpy(var[i].enumTXT, "parOSC_MaxTvlVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_sec;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.25 * in_2_mm / 1.0;
	cfg->Param[i].max = 3.0 * in_2_mm / 1.0;
	cfg->Param[i].incrMin = 0.05 * in_2_mm / 1.0;	
	cfg->Param[i].incrMax = 4 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.25 * in_2_mm / 0.5;
	var[i].segm = 0;
	
	
	//////////////////////////////////////////////////////
	//  AVC
	

	i = parAVC_TouchVolt;
	strcpy(var[i].name , "AVC TOUCH VOLT");
	strcpy(var[i].enumTXT, "parAVC_TouchVolt");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 18.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.6;
	var[i].segm = 0;


	i = parAVC_TouchTm;
	strcpy(var[i].name , "AVC Touch TMO");
	strcpy(var[i].enumTXT, "parAVC_TouchTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 5.8;
	var[i].segm = 0;

	i = parAVC_TRetractTm;
	strcpy(var[i].name , "AVC Retract Tm");
	strcpy(var[i].enumTXT, "parAVC_TRetractTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.15;
	var[i].segm = 0;

	i = parAVC_TouchRepeat;
	strcpy(var[i].name , "AVC Retry Cnt");
	strcpy(var[i].enumTXT, "parAVC_TouchRepeat");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;

	
	i = parAVC_VoltInit;
	strcpy(var[i].name , "AVC INITIAL VOLT");
	strcpy(var[i].enumTXT, "parAVC_VoltInit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 18.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 7.0; 
	var[i].segm = 0;
	

	i = parAVC_VoltSP;
	strcpy(var[i].name , "PRIMARY ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_VoltSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 30.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 1;

	i = parAVC_VoltInSP;
	strcpy(var[i].name , "PRIMARY In ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_VoltInSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 30.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 1;

	i = parAVC_VoltOutSP;
	strcpy(var[i].name , "PRIMARY Out ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_VoltOutSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 30.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 1;

	 i = parAVC_VoltBackSP;
	strcpy(var[i].name , "BACKGRD ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_VoltBackSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 30.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 1;


	i = parAVC_Boost1;
	strcpy(var[i].name , "Boost1 ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_Boost1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].max = 30.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;

	i = parAVC_Boost2;
	strcpy(var[i].name , "Boost2 ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_Boost2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;	
	cfg->Param[i].max = 30.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;
	

	i = parAVC_VSPTmOut;
	strcpy(var[i].name , "AVC Prim Volt Tmo");
	strcpy(var[i].enumTXT, "parAVC_VSPTmOut");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10;	
	var[i].segm = 0;

	i = parAVC_TouchVel;
	strcpy(var[i].name , "AVC SLOW RUN IN SP");
	strcpy(var[i].enumTXT, "parAVC_TouchVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.15;
	var[i].segm = 0;

	i = parAVC_ToSPVel;
	strcpy(var[i].name , "AVC LIFT UP SPEED");
	strcpy(var[i].enumTXT, "parAVC_ToSPVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parAVC_LU_TM;
	strcpy(var[i].name , "AVC LU TM");
	strcpy(var[i].enumTXT, "parAVC_LU_TM");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 2;
	cfg->Param[i].min = 0.01;
	cfg->Param[i].max = 2.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.2;
	var[i].segm = 0;

	i = parAVC_KissTm;
	strcpy(var[i].name , "AVC Kiss TM");
	strcpy(var[i].enumTXT, "parAVC_KissTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 2;
	cfg->Param[i].min = 0.005;
	cfg->Param[i].max = 2.0;
	cfg->Param[i].incrMin = 0.005;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.01;
	var[i].segm = 0;
		
	i = parAVC_Upslope;
	strcpy(var[i].name , "VOLT UPSLOPE TM");
	strcpy(var[i].enumTXT, "parAVC_Upslope");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 30.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;

	i = parAVC_DownSlope;
	strcpy(var[i].name , "VOLT DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parAVC_DownSlope");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 20.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;

	i = parAVC_HoldTm;
	strcpy(var[i].name , "AVC RETRACT DLY");
	strcpy(var[i].enumTXT, "parAVC_HoldTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 0;

	i = parAVC_VoltFinal;
	strcpy(var[i].name , "FINAL ARC VOLTS");
	strcpy(var[i].enumTXT, "parAVC_VoltFinal");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 18.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 9.0;	
	var[i].segm = 0;

	i = parAVC_GoBackTm;
	strcpy(var[i].name , "AVC RETRACT TM");
	strcpy(var[i].enumTXT, "parAVC_GoBackTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 5.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;	


	i = parAVC_StartDly;
	strcpy(var[i].name , "VOLT UPSLOPE DLY");
	strcpy(var[i].enumTXT, "parAVC_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	
	i = parAVC_StopDly;
	strcpy(var[i].name , "VOLT DWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parAVC_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;


	i = parAVC_MaxVoltError;
	strcpy(var[i].name , "AVC Max Volt Err");
	strcpy(var[i].enumTXT, "parAVC_MaxVoltError");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;	
	var[i].segm = 0;

	i = parAVC_Limits_Mask_Tm;
	strcpy(var[i].name , "AVC Limits Filter Tm");
	strcpy(var[i].enumTXT, "parAVC_Limits_Mask_Tm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 3;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 2.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.2;
	cfg->Param[i].defaultVal = 0.1;
	var[i].segm = 0;


	i = parAVC_JogSpeedDirCntrl;
	strcpy(var[i].name , "AVC Jog Speed");
	strcpy(var[i].enumTXT, "parAVC_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.75;
	var[i].segm = 0;

	
	i = parAVC_LiftSpeedDirCntrl;
	strcpy(var[i].name , "AVC RETRACT SP");
	strcpy(var[i].enumTXT, "parAVC_LiftSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.35;
	var[i].segm = 0;


	i = parAVC_FbkFilter;
	strcpy(var[i].name , "AVC Sign Filter");
	strcpy(var[i].enumTXT, "parAVC_FbkFilter");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parAVC_KPP;
	strcpy(var[i].name , "AVC KPP");
	strcpy(var[i].enumTXT, "parAVC_KPP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parAVC_KIP;
	strcpy(var[i].name , "AVC KIP");
	strcpy(var[i].enumTXT, "parAVC_KIP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.1;
	var[i].segm = 0;

	i = parAVC_BB_Vel;
	strcpy(var[i].name , "AVC BB Speed");
	strcpy(var[i].enumTXT, "parAVC_BB_Vel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.15;
	var[i].segm = 0;


	i = parAVC_BB_Integr;
	strcpy(var[i].name , "AVC BB Integr");
	strcpy(var[i].enumTXT, "parAVC_BB_Integr");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.25;
	var[i].segm = 0;

	i = parAVC_BB_Gain;
	strcpy(var[i].name , "AVC BB Gain");
	strcpy(var[i].enumTXT, "parAVC_BB_Gain");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parAVC_DB_Window;
	strcpy(var[i].name , "AVC DB Window");
	strcpy(var[i].enumTXT, "parAVC_DB_Window");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 2.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.5;
	cfg->Param[i].defaultVal = 0.3;	
	var[i].segm = 0;

	//////////////////////////////////////////////////////
	//  Wire Feed
	i = parWF_StartDly;
	strcpy(var[i].name , "WF START DLY");
	strcpy(var[i].enumTXT, "parWF_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 5.0;
	var[i].segm = 0;

	i = parWF_RampUp;
	strcpy(var[i].name , "WF UPSLOPE TM");
	strcpy(var[i].enumTXT, "parWF_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 4.0;
	var[i].segm = 0;

	i = parWF_SpeedSP;
	strcpy(var[i].name , "WF PRIMARY SPEED");
	strcpy(var[i].enumTXT, "parWF_SpeedSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 100.0 * in_min_2_mm_s;
	var[i].segm = 1;

	i = parWF_SpeedInSP;
	strcpy(var[i].name , "WF PRIMARY IN SPEED");
	strcpy(var[i].enumTXT, "parWF_SpeedInSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 100.0 * in_min_2_mm_s;
	var[i].segm = 1;

	i = parWF_SpeedOutSP;
	strcpy(var[i].name , "WF PRIMARY OUT SPEED");
	strcpy(var[i].enumTXT, "parWF_SpeedOutSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 100.0 * in_min_2_mm_s;
	var[i].segm = 1;

	i = parWF_BkgSpeedSP;
	strcpy(var[i].name , "WF BKG SPEED");
	strcpy(var[i].enumTXT, "parWF_BkgSpeedSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 100.0 * in_min_2_mm_s;
	var[i].segm = 1;

	i = parWF_SpeedInit;
	strcpy(var[i].name , "WF INITIAL SPEED");
	strcpy(var[i].enumTXT, "parWF_SpeedInit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal =0.0 * in_min_2_mm_s;
	var[i].segm = 0;


	i = parWF_SpeedFinal;
	strcpy(var[i].name , "WF FINAL SPEED");
	strcpy(var[i].enumTXT, "parWF_SpeedFinal");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0 * in_min_2_mm_s;
	var[i].segm = 0;

	i = parWF_StopDly;
	strcpy(var[i].name , "WF DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parWF_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	i = parWF_RampDwn;
	strcpy(var[i].name , "WF DOWNSLOPE TIME");
	strcpy(var[i].enumTXT, "parWF_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parWF_RampUpdate;
	strcpy(var[i].name , "WF Ramp Update");
	strcpy(var[i].enumTXT, "parWF_RampUpdate");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.1;

	i = parWF_Boost1SP;
	strcpy(var[i].name , "WF Boost1");
	strcpy(var[i].enumTXT, "parWF_Boost1SP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;


	i = parWF_Boost2SP;
	strcpy(var[i].name , "WF Boots2");
	strcpy(var[i].enumTXT, "parWF_Boost2SP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 500 * in_min_2_mm_s;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;


	 i = parWF_JogSpeed;
	strcpy(var[i].name , "WF Jog ???");
	strcpy(var[i].enumTXT, "parWF_JogSpeed");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parWF_JogSpeedDirCntrl;
	strcpy(var[i].name , "WF Jog Speed");
	strcpy(var[i].enumTXT, "parWF_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.9;
	var[i].segm = 0;

	
	i = parWF_RetractDly;
	strcpy(var[i].name , "WF RETRACT DLY");
	strcpy(var[i].enumTXT, "parWF_RetractDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 120.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 9.0;
	var[i].segm = 0;


	i = parWF_RetractSpeed;
	strcpy(var[i].name , "WF RETRACT SPEED");
	strcpy(var[i].enumTXT, "parWF_RetractSpeed");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.25;
	var[i].segm = 0;


	i = parWF_RetracDist;
	strcpy(var[i].name , "WF RETRACT LENGTH");
	strcpy(var[i].enumTXT, "parWF_RetracDist");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 25.4 * 2;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.2 * in_2_mm;
	var[i].segm = 0;

	i = parWF_CalibVel1;
	strcpy(var[i].name , "WF Calib Speed 1");
	strcpy(var[i].enumTXT, "parWF_CalibVel1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 127.0;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 50.0 * in_min_2_mm_s;
	var[i].segm = 0;

	i = parWF_CalibVel2;
	strcpy(var[i].name , "WF Calib Speed 2");
	strcpy(var[i].enumTXT, "parWF_CalibVel2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	
	cfg->Param[i].max = 127.0;
	cfg->Param[i].incrMin = 1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 100.0 * in_min_2_mm_s;
	var[i].segm = 0;
		
	
	
	//////////////////////////////////////////////////////
	//	Wire Pulse
	
	
	i = parWP_StartDly;
	strcpy(var[i].name , "WP START DELAY");
	strcpy(var[i].enumTXT, "parWP_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 5.0;
	var[i].segm = 0;

	i = parWP_RampUp;
	strcpy(var[i].name , "WP UPSLOPE TM");
	strcpy(var[i].enumTXT, "parWP_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;

	i = parWP_SpeedSP;
	strcpy(var[i].name , "WP PRIMARY SPEED");
	strcpy(var[i].enumTXT, "parWP_SpeedSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 13.2;
	var[i].segm = 1;

	i = parWP_SpeedInSP;
	strcpy(var[i].name , "WP PRIMARY IN SPEED");
	strcpy(var[i].enumTXT, "parWP_SpeedInSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 13.2;
	var[i].segm = 1;

	i = parWP_SpeedOutSP;
	strcpy(var[i].name , "WP PRIMARY OUT SPEED");
	strcpy(var[i].enumTXT, "parWP_SpeedOutSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 13.2;
	var[i].segm = 1;

	i = parWP_BkgSpeedSP;
	strcpy(var[i].name , "WP BKG SPEED");
	strcpy(var[i].enumTXT, "parWP_BkgSpeedSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 13.2;
	var[i].segm = 1;
	
	
	i = parWP_SpeedInit;
	strcpy(var[i].name , "WP INITIAL SPEED");
	strcpy(var[i].enumTXT, "parWP_SpeedInit");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 25.0;
	cfg->Param[i].incrMin = 0.25;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;


	i = parWP_SpeedFinal;
	strcpy(var[i].name , "WP FINAL SPEED");
	strcpy(var[i].enumTXT, "parWP_SpeedFinal");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 25.0;
	cfg->Param[i].incrMin = 0.25;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parWP_StopDly;
	strcpy(var[i].name , "WP DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parWP_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	i = parWP_RampDwn;
	strcpy(var[i].name , "WP DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parWP_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;

	i = parWP_RampUpdate;
	strcpy(var[i].name , "WP Ramp Update");
	strcpy(var[i].enumTXT, "parWP_RampUpdate");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.1;	
	var[i].segm = 0;
	
	i = parWP_Boost1SP;
	strcpy(var[i].name , "WP Boot1");
	strcpy(var[i].enumTXT, "parWP_Boost1SP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;


	i = parWP_Boost2SP;
	strcpy(var[i].name , "WP Boot2");
	strcpy(var[i].enumTXT, "parWP_Boost2SP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 40.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;
	
	
	i = parWP_JogSpeed;
	strcpy(var[i].name , "WP Jog");
	strcpy(var[i].enumTXT, "parWP_JogSpeed");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].max = 25.0;
	cfg->Param[i].incrMin = 0.25;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 6.0;
	var[i].segm = 0;

	i = parWP_JogSpeedDirCntrl;
	strcpy(var[i].name , "WP Jog Cntrl");
	strcpy(var[i].enumTXT, "parWP_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;
	

	//////////////////////////////
	//	TIG Power Supply
	
	i = parTIG_AmpSP;
	strcpy(var[i].name , "PRIMARY ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_AmpSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 140.0;
	var[i].segm = 1;

	i = parTIG_AmpInSP;
	strcpy(var[i].name , "PRIMARY In ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_AmpInSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 140.0;
	var[i].segm = 1;


	i = parTIG_AmpOutSP;
	strcpy(var[i].name , "PRIMARY Out ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_AmpOutSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 140.0;
	var[i].segm = 1;


	i = parTIG_BackAmp;
	strcpy(var[i].name , "BACKGND ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_BackAmp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 110.0;
	var[i].segm = 1;


	i = parTIG_Boost1;
	strcpy(var[i].name , "Boost1 ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_Boost1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].max = 500.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;

	i = parTIG_Boost2;
	strcpy(var[i].name , "Boost2 ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_Boost2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].max = 500.0;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 1;

	i = parTIG_PulseFreq;
	strcpy(var[i].name , "ARC AMPS FREQ.");
	strcpy(var[i].enumTXT, "parTIG_PulseFreq");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 1;
	
	i = parTIG_PulseWidth;
	strcpy(var[i].name , "ARC PULSE DUTY");
	strcpy(var[i].enumTXT, "parTIG_PulseWidth");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.001;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.001;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 1;		

	i = parTIG_StartAmp;
	strcpy(var[i].name , "INITIAL ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_StartAmp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 45.0;
	var[i].segm = 0;

	
	i = parTIG_EndAmp;
	strcpy(var[i].name , "FINAL ARC AMPS");
	strcpy(var[i].enumTXT, "parTIG_EndAmp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;


	i = parTIG_RampUp;
	strcpy(var[i].name , "ARC AMP UPSLOPE TM");
	strcpy(var[i].enumTXT, "parTIG_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;

	i = parTIG_RampDwn;
	strcpy(var[i].name , "AMPS DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parTIG_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 9.0;
	var[i].segm = 0;
	
	i = parTIG_StartDly;
	strcpy(var[i].name , "ARC AMP START DLY");
	strcpy(var[i].enumTXT, "parTIG_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	i = parTIG_StopDly;
	strcpy(var[i].name , "AMPS DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parTIG_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parTIG_CalibAmp1;
	strcpy(var[i].name , "TIG Calib Amp 1");
	strcpy(var[i].enumTXT, "parTIG_CalibAmp1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;

	i = parTIG_CalibAmp2;
	strcpy(var[i].name , "TIG Calib Amp 2");
	strcpy(var[i].enumTXT, "parTIG_CalibAmp2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 100.0;
	var[i].segm = 0;

	i = parTIG_AmpSP_Boost;
	strcpy(var[i].name , "TIG Amp Boost");
	strcpy(var[i].enumTXT, "parTIG_AmpSP_Boost");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 2;
	cfg->Param[i].min = -0.5;
	cfg->Param[i].max = 0.5;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;


	//////////////////////////////
	//	HW Power Supply
	
	i = parHW_AmpSP;
	strcpy(var[i].name , "HW PRIMARY AMPS");
	strcpy(var[i].enumTXT, "parHW_AmpSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 80.0;
	var[i].segm = 1;

	i = parHW_VoltSP;
	strcpy(var[i].name , "HW Volt SP");
	strcpy(var[i].enumTXT, "parHW_VoltSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;

	i = parHW_StartAmp;
	strcpy(var[i].name , "HW INITIAL AMPS");
	strcpy(var[i].enumTXT, "parHW_StartAmp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;
	
	i = parHW_EndAmp;
	strcpy(var[i].name , "HW FINAL AMPS");
	strcpy(var[i].enumTXT, "parHW_EndAmp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 100.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;

	i = parHW_RampUp;
	strcpy(var[i].name , "HW UPSLOPE TM");
	strcpy(var[i].enumTXT, "parHW_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;

	i = parHW_RampDwn;
	strcpy(var[i].name , "HW DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parHW_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.5;
	var[i].segm = 0;


	i = parHW_RampUpDly;
	strcpy(var[i].name , "HW UPSLOPE DLY ");
	strcpy(var[i].enumTXT, "parHW_RampUpDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 8.0;
	var[i].segm = 0;

	i = parHW_RampDwnDly;
	strcpy(var[i].name , "HW DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parHW_RampDwnDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;


	i = parHW_CalibAmp1;
	strcpy(var[i].name , "HW Calib Amp 1");
	strcpy(var[i].enumTXT, "parHW_CalibAmp1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 160.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 25.0;
	var[i].segm = 0;


	i = parHW_CalibAmp2;
	strcpy(var[i].name , "HW Calib Amp 2");
	strcpy(var[i].enumTXT, "parHW_CalibAmp2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 160.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;	
	var[i].segm = 0;

		
	i = parHW_AmpSP_Boost;
	strcpy(var[i].name , "HW Amp Boost");
	strcpy(var[i].enumTXT, "parHW_AmpSP_Boost");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 2;
	cfg->Param[i].min = -0.5;
	cfg->Param[i].max = 0.5;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;
	

	/////////////////////////////
	// TVL
	

	i = parTVL_VelSP;
	strcpy(var[i].name , "TVL PRIMARY SPEED");
	strcpy(var[i].enumTXT, "parTVL_VelSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.5 * in_min_2_mm_s;	
	var[i].segm = 1;

	i = parTVL_VelInSP;
	strcpy(var[i].name , "TVL IN PRIMARY SPEED");
	strcpy(var[i].enumTXT, "parTVL_VelInSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.5 * in_min_2_mm_s;	
	var[i].segm = 1;

	i = parTVL_VelOutSP;
	strcpy(var[i].name , "TVL Out PRIMARY SPEED");
	strcpy(var[i].enumTXT, "parTVL_VelOutSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.5 * in_min_2_mm_s;	
	var[i].segm = 1;

	i = parTVL_VelBackSP;
	strcpy(var[i].name , "TVL Bckg SPEED");
	strcpy(var[i].enumTXT, "parTVL_VelBackSP");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.5 * in_min_2_mm_s;	
	var[i].segm = 1;


	i = parTVL_Boost1;
	strcpy(var[i].name , "TVL Boost1 SPEED");
	strcpy(var[i].enumTXT, "parTVL_Boost1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0;	
	var[i].segm = 1;


	i = parTVL_Boost2;
	strcpy(var[i].name , "TVL Boost2 SPEED");
	strcpy(var[i].enumTXT, "parTVL_Boost2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].min = -cfg->Param[i].max;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0;	
	var[i].segm = 1;

	i = parTVL_InitVel;
	strcpy(var[i].name , "TVL INITIAL SPEED");
	strcpy(var[i].enumTXT, "parTVL_InitVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max =  50.0* in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 0.0 * in_min_2_mm_s;
	var[i].segm = 0;
	
	i = parTVL_FinalVel;
	strcpy(var[i].name , "TVL FINAL SPEED");
	strcpy(var[i].enumTXT, "parTVL_FinalVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max =  50.0 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 1.0 * in_min_2_mm_s;	
	var[i].segm = 0;
		
	
	i = parTVL_StartDly;
	strcpy(var[i].name , "TVL START DLY");
	strcpy(var[i].enumTXT, "parTVL_StartDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 5.0;
	var[i].segm = 0;

	i = parTVL_RampUp;
	strcpy(var[i].name , "TVL UPSLOPE TM");
	strcpy(var[i].enumTXT, "parTVL_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;	

	i = parTVL_StopDly;
	strcpy(var[i].name , "TVL DOWNSLOPE DLY");
	strcpy(var[i].enumTXT, "parTVL_StopDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 0;

	i = parTVL_RampDwn;
	strcpy(var[i].name , "TVL DOWNSLOPE TM");
	strcpy(var[i].enumTXT, "parTVL_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parTVL_RampUpdate;
	strcpy(var[i].name , "TVL Ramp Update");
	strcpy(var[i].enumTXT, "parTVL_RampUpdate");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.1;
	var[i].segm = 0;

	i = parTVL_JogSpeedDirCntrl;
	strcpy(var[i].name , "TVL Jog Speed");	
	strcpy(var[i].enumTXT, "parTVL_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;

	i = parTVL_RunDist;
	//strcpy(var[i].name , "RUN Segments");
	strcpy(var[i].name , "RUN Dist.");
	strcpy(var[i].enumTXT, "parTVL_RunDist");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType =  unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 100000.0;
	cfg->Param[i].incrMin = 1;
	cfg->Param[i].incrMax = 1000;
	cfg->Param[i].defaultVal = 40;
	var[i].segm = 0;

	i = parTVL_RewrapDly;
	strcpy(var[i].name , "TVL REWRAP DLY");
	strcpy(var[i].enumTXT, "parTVL_RewrapDly");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 300.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	i = parTVL_OverlapDist;
	strcpy(var[i].name , "TVL Overlap ");
	strcpy(var[i].enumTXT, "parTVL_OverlapDist");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType =  unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;
	cfg->Param[i].min = -300000.0;
	cfg->Param[i].max = 300000.0;
	cfg->Param[i].incrMin = 1;
	cfg->Param[i].incrMax = 1;
	cfg->Param[i].defaultVal = 0;
	var[i].segm = 0;	
	
	i = parTVL_RewrapVel;
	strcpy(var[i].name , "TVL REWRAP SP");
	strcpy(var[i].enumTXT, "parTVL_RewrapVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 110.0 * in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 10.0;
	var[i].segm = 0;

	i = parTVL_Jog2Stop;
	strcpy(var[i].name , "TVL Jog2Stop");
	strcpy(var[i].enumTXT, "parTVL_Jog2Stop");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 2;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 4.5;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 1.5;	
	var[i].segm = 0;	
	
	i = parTVL_CalibVel1;
	strcpy(var[i].name , "TVL Calib Speed 1");
	strcpy(var[i].enumTXT, "parTVL_CalibVel1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 1.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.0 * in_min_2_mm_s; 
	var[i].segm = 0;

	i = parTVL_CalibVel2;
	strcpy(var[i].name , "TVL Calib Speed 2");
	strcpy(var[i].enumTXT, "parTVL_CalibVel2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 1.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 100.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 6.0 * in_min_2_mm_s; 
	var[i].segm = 0;	

	i = parTVL_TrackDia;
	strcpy(var[i].name , "Tungsten Track Dia");
	strcpy(var[i].enumTXT, "parTVL_TrackDia");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 1.0 * in_2_mm;
	cfg->Param[i].max = 200.0 * in_2_mm;
	cfg->Param[i].incrMin = 0.01 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 4.5 * in_2_mm; 
	var[i].segm = 0;	
	
	i = parTVL_SpeedTolerane;
	strcpy(var[i].name , "TVL Speed Tolerance");
	strcpy(var[i].enumTXT, "parTVL_SpeedTolerane");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 5.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 5.0;
	var[i].segm = 0;

	///////////////////////////////
	
	i = parCONTIN_TVL_RampDwn;
	strcpy(var[i].name , "CONTIN_TVL_RampDwn");
	strcpy(var[i].enumTXT, "parCONTIN_TVL_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_TVL_RampUp;
	strcpy(var[i].name , "CONTIN_TVL_RampUp");
	strcpy(var[i].enumTXT, "parCONTIN_TVL_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;	

	i = parCONTIN_WF_RampDwn;
	strcpy(var[i].name , "CONTIN_WF_RampDwn");
	strcpy(var[i].enumTXT, "parCONTIN_WF_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_WF_RampUp;
	strcpy(var[i].name , "CONTIN_WF_RampUp");
	strcpy(var[i].enumTXT, "parCONTIN_WF_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;	

	i = parCONTIN_WP_RampDwn;
	strcpy(var[i].name , "CONTIN_WP_RampDwn");
	strcpy(var[i].enumTXT, "parCONTIN_WP_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_WP_RampUp;
	strcpy(var[i].name , "CONTIN_WP_RampUp");
	strcpy(var[i].enumTXT, "parCONTIN_WP_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;	


	i = parCONTIN_TIG_RampUp;
	strcpy(var[i].name , "parCONTIN_TIG_RampUp");
	strcpy(var[i].enumTXT, "parCONTIN_TIG_RampUp");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_TIG_RampDwn;
	strcpy(var[i].name , "parCONTIN_TIG_RampDwn");
	strcpy(var[i].enumTXT, "parCONTIN_TIG_RampDwn");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_TIG_Dwell;
	strcpy(var[i].name , "parCONTIN_TIG_Dwell");
	strcpy(var[i].enumTXT, "parCONTIN_TIG_Dwell");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parCONTIN_TIG_Percent;
	strcpy(var[i].name , "parCONTIN_TIG_Percent");
	strcpy(var[i].enumTXT, "parCONTIN_TIG_Percent");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = -0.8;
	cfg->Param[i].max = 0.8;
	cfg->Param[i].incrMin = 0.005;
	cfg->Param[i].incrMax = 0.01;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;
	
	/////////////////////////////
	// GAS
	
	i = parGAS_PrePurgeTm;
	strcpy(var[i].name , "GAS PREPURGE TM");
	strcpy(var[i].enumTXT, "parGAS_PrePurgeTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 60.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;

	i = parGAS_PostPurgeTm;
	strcpy(var[i].name , "GAS POSTPURGE TM");
	strcpy(var[i].enumTXT, "parGAS_PostPurgeTm");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 180.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 12.0;
	var[i].segm = 0;
	

	i = parGAS_Tmo;
	strcpy(var[i].name , "Gas On/Off TimeOut");
	strcpy(var[i].enumTXT, "parGAS_Tmo");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 5.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;


	/////////////////////////////
	// COOLANT
	

	i = parCOOL_Tmo;
	strcpy(var[i].name , "Cool On/Off TimeOut");
	strcpy(var[i].enumTXT, "parCOOL_Tmo");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 5.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 3.0;
	var[i].segm = 0;


	///// INDX /////

	i = parINDX_StepDist;
	strcpy(var[i].name , "INDX Step Distance");
	strcpy(var[i].enumTXT, "parINDX_StepDist");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0 * in_2_mm;
	cfg->Param[i].max = 200.0 * in_2_mm;
	cfg->Param[i].incrMin = 0.01 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 4.5 * in_2_mm; 
	var[i].segm = 0;	

	i = parINDX_StepVel;
	strcpy(var[i].name , "INDX Step Vel");
	strcpy(var[i].enumTXT, "parINDX_StepVel");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;
	
	/*
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	cfg->Param[i].min = 1.0 *  in_min_2_mm_s;
	cfg->Param[i].max = 10.0 *  in_min_2_mm_s;
	cfg->Param[i].incrMin = 0.1 * in_min_2_mm_s;
	cfg->Param[i].incrMax = 10.0 * cfg->Param[i].incrMin;
	cfg->Param[i].defaultVal = 3.0 * in_min_2_mm_s; 
	var[i].segm = 0;
	*/

	
	i = parINDX_JogSpeedDirCntrl;
	strcpy(var[i].name , "INDX Jog Speed");	
	strcpy(var[i].enumTXT, "parINDX_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 0;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.01;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 1.0;
	var[i].segm = 0;


	i = parINDX_StepAfterBeats;
	strcpy(var[i].name , "Step After Beats");
	strcpy(var[i].enumTXT, "parINDX_StepAfterBeats");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 99.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;	
	var[i].segm = 0;


	//////  WRIST	
	i = parWRST_JogSpeedDirCntrl;
	strcpy(var[i].name , "Weld Angle Jog Speed");
	strcpy(var[i].enumTXT, "parWRST_JogSpeedDirCntrl");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 0.75;
	var[i].segm = 0;


	//////  parTVL_POS_OFFSET
	i = parTVL_POS_OFFSET;
	strcpy(var[i].name , "TVL Pos Offset");
	strcpy(var[i].enumTXT, "parTVL_POS_OFFSET");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = -200.0 * in_2_mm;
	cfg->Param[i].max = 200.0 * in_2_mm;
	cfg->Param[i].incrMin = 0.01 * in_2_mm;
	cfg->Param[i].incrMax = 1.0 * in_2_mm;
	cfg->Param[i].defaultVal = 0; 
	var[i].segm = 0;

	i = parTVL_POS_PREDICT;
	strcpy(var[i].name , "TVL POS Predict Time");
	strcpy(var[i].enumTXT, "parTVL_POS_PREDICT");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 3.0;
	cfg->Param[i].incrMin = 0.1;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.1;

	////////////////////////////////////

	i = parGEN_CalibDist;
	strcpy(var[i].name , "Calib Dist");
	strcpy(var[i].enumTXT, "parGEN_CalibDist");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;

	i = parGEN_CalibDist_2;
	strcpy(var[i].name , "Calib Dist 2");
	strcpy(var[i].enumTXT, "parGEN_CalibDist_2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;

	i = parGEN_CalibDist1;
	strcpy(var[i].name , "Calib Dist1");
	strcpy(var[i].enumTXT, "parGEN_CalibDist1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;

	i = parGEN_CalibDist2;
	strcpy(var[i].name , "Calib Dist2");
	strcpy(var[i].enumTXT, "parGEN_CalibDist2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;

	i = parGEN_CalibDist3;
	strcpy(var[i].name , "Calib Dist3");
	strcpy(var[i].enumTXT, "parGEN_CalibDist3");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;

	i = parGEN_CalibDist4;
	strcpy(var[i].name , "Calib Dist4");
	strcpy(var[i].enumTXT, "parGEN_CalibDist4");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;
		

	i = parGEN_CalibTime;
	strcpy(var[i].name , "Calib Time");
	strcpy(var[i].enumTXT, "parGEN_CalibTime");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000000.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parGEN_CalibTime1;
	strcpy(var[i].name , "Calib Time1");
	strcpy(var[i].enumTXT, "parGEN_CalibTime1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000000.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parGEN_CalibTime2;
	strcpy(var[i].name , "Calib Time2");
	strcpy(var[i].enumTXT, "parGEN_CalibTime2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000000.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parGEN_CalibTime3;
	strcpy(var[i].name , "Calib Time3");
	strcpy(var[i].enumTXT, "parGEN_CalibTime3");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000000.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;

	i = parGEN_CalibAmp1;
	strcpy(var[i].name , "GEN Calib Amp 1");
	strcpy(var[i].enumTXT, "parGEN_CalibAmp1");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 1.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 25.0;
	var[i].segm = 0;

	i = parGEN_CalibAmp2;
	strcpy(var[i].name , "GEN Calib Amp 2");
	strcpy(var[i].enumTXT, "parGEN_CalibAmp2");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 1.0;
	cfg->Param[i].max = 500.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 25.0;
	var[i].segm = 0;

	i = parGEN_CalibTime4;
	strcpy(var[i].name , "Calib Time4");
	strcpy(var[i].enumTXT, "parGEN_CalibTime4");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000000.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 10.0; 
	cfg->Param[i].defaultVal = 0.0;
	var[i].segm = 0;
	
	i = parGEN_AmpRead;
	strcpy(var[i].name , "Amp Meas.");
	strcpy(var[i].enumTXT, "parGEN_AmpRead");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;	
	cfg->Param[i].min = 5.0;
	cfg->Param[i].max = 200.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 10.0;
	cfg->Param[i].defaultVal = 50.0;
	var[i].segm = 0;

	i = parGEN_WeldProgramRepeat;
	strcpy(var[i].name , "Weld Program Repeat");
	strcpy(var[i].enumTXT, "parGEN_WeldProgramRepeat");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitIU;
	var[i].DefaultUnit = unitIU;
	var[i].precision = 0;	
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 99.0;
	cfg->Param[i].incrMin = 1.0;
	cfg->Param[i].incrMax = 1.0;
	cfg->Param[i].defaultVal = 0.0;	
	var[i].segm = 0;	

	i = parGEN_RadSpeed_Boost;
	strcpy(var[i].name , "TVL Radius Vel Boost");
	strcpy(var[i].enumTXT, "parGEN_RadSpeed_Boost");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitPerc;
	var[i].DefaultUnit = UNIT_PERCENT;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 10.0;
	cfg->Param[i].incrMin = 0.05;
	cfg->Param[i].incrMax = 0.1;
	cfg->Param[i].defaultVal = 2.0;
	var[i].segm = 0;


	i = parGEN_Radius;
	strcpy(var[i].name , "Pole Radius");
	strcpy(var[i].enumTXT, "parGEN_Radius");
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;
	cfg->Param[i].min = 0.0;
	cfg->Param[i].max = 1000000;
	cfg->Param[i].incrMin = 0.254 *1;
	cfg->Param[i].incrMax = 25.4;
	cfg->Param[i].defaultVal = 0.0; 
	var[i].segm = 0;
	

	/////////////////////////////////////
	////////   Scaled variables

		
	i = varCurTVL_Vel;
	strcpy(var[i].enumTXT, "varCurTVL_Vel");
	strcpy(var[i].name , "TVL Vel 1FBK");	
	var[i].pfVal = &Process[PROC_TVL].Fbk[0];
	strcpy(var[i].VarName , "Process[0].Fbk[0]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;		

	i = varCurTVL_Vel_Calc;
	strcpy(var[i].enumTXT, "varCurTVL_Vel_Calc");
	strcpy(var[i].name , "TVL Vel FBK Calc");
	var[i].pfVal = &Process[PROC_TVL].Fbk[4];
	strcpy(var[i].VarName , "Process[0].Fbk[4]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 2;	

	i = varTVL_Vel_Cmd; 
	strcpy(var[i].enumTXT, "varTVL_Vel_Cmd");
	strcpy(var[i].name , "TVL Vel Cmd");
	var[i].pfVal = &Process[PROC_TVL].Fbk[1];
	strcpy(var[i].VarName , "Process[0].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;	

	i = varCurWF_Vel;
	strcpy(var[i].enumTXT, "varCurWF_Vel");
	strcpy(var[i].name , "WF Vel FBK");
	var[i].pfVal = &Process[PROC_WIRE_FEED].Fbk[0];
	strcpy(var[i].VarName , "Process[3].Fbk[0]");
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;

	i = varWF_Vel_Cmd; 
	strcpy(var[i].enumTXT, "varWF_Vel_Cmd");
	strcpy(var[i].name , "WF Vel Cmd");
	var[i].pfVal = &Process[PROC_WIRE_FEED].Fbk[1];
	strcpy(var[i].VarName , "Process[3].Fbk[1]");
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitSpeed;
	var[i].DefaultUnit = UNIT_inch_per_min;
	var[i].precision = 0;
	
	i = varCurWFDist;
	strcpy(var[i].enumTXT, "varCurWFDist");
	strcpy(var[i].name , "act WF Len");
	var[i].pfVal = &Process[PROC_WIRE_FEED].Fbk[4];
	strcpy(var[i].VarName , "Process[3].Fbk[4]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 0;
	

	i = varCurWP_Vel;
	strcpy(var[i].enumTXT, "varCurWP_Vel");
	strcpy(var[i].name , "WP Freq FBK");
	var[i].pfVal = &Process[PROC_WIRE_PULSE].Fbk[0];
	strcpy(var[i].VarName , "Process[4].Fbk[0]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;
	
	i = varWP_Vel_Cmd; 
	strcpy(var[i].enumTXT, "varWP_Vel_Cmd");
	strcpy(var[i].name , "WP Freq Cmd");
	var[i].pfVal = &Process[PROC_WIRE_PULSE].Fbk[1];
	strcpy(var[i].VarName , "Process[4].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitFreq;
	var[i].DefaultUnit = UNIT_Hz;
	var[i].precision = 1;

	i = varCurArc_Volt;
	strcpy(var[i].enumTXT, "varCurArc_Volt");
	strcpy(var[i].name , "Arc Volt FBK");
	var[i].pfVal = &Process[PROC_AVC].Fbk[0];
	strcpy(var[i].VarName , "Process[2].Fbk[0]");
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 1;

	i = varArc_Volt_Cmd;
	strcpy(var[i].enumTXT, "varArc_Volt_Cmd");
	strcpy(var[i].name , "Arc Volt Cmd");
	var[i].pfVal = &Process[PROC_AVC].Fbk[1];
	strcpy(var[i].VarName , "Process[2].Fbk[1]");
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 1;

	i = varCurTIG_Amp;
	strcpy(var[i].enumTXT, "varCurTIG_Amp");
	strcpy(var[i].name , "TIG Amp FBK");
	var[i].pfVal = &Process[PROC_TIG_PS].Fbk[0];
	strcpy(var[i].VarName , "Process[5].Fbk[0]");
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 1;

	i = varTIG_Amp_Cmd;
	strcpy(var[i].enumTXT, "varTIG_Amp_Cmd");
	strcpy(var[i].name , "TIG Amp CMD");	
	var[i].pfVal = &Process[PROC_TIG_PS].Fbk[1];
	strcpy(var[i].VarName , "Process[5].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;

	i = varCurHW_Amp;
	strcpy(var[i].enumTXT, "varCurHW_Amp");
	strcpy(var[i].name , "HW Amp FBK");
	var[i].pfVal = &Process[PROC_HW_PS].Fbk[0];	
	strcpy(var[i].VarName , "Process[6].Fbk[0]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;

	i = varHW_Amp_Cmd;
	strcpy(var[i].enumTXT, "varHW_Amp_Cmd");
	strcpy(var[i].name , "HW Amp CMD");
	var[i].pfVal = &Process[PROC_HW_PS].Fbk[1];	
	strcpy(var[i].VarName , "Process[6].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitAmp;
	var[i].DefaultUnit = UNIT_Amp;
	var[i].precision = 0;

	i = varCurHW_Volt; // new
	strcpy(var[i].enumTXT, "varCurHW_Volt");
	strcpy(var[i].name , "HW Volt FBK");
	var[i].pfVal = &Process[PROC_HW_PS].Fbk[2];	
	strcpy(var[i].VarName , "Process[6].Fbk[2]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 1;

	i = varHW_Volt_Cmd; // new
	strcpy(var[i].enumTXT, "varHW_Volt_Cmd");
	strcpy(var[i].name , "HW Volt CMD");
	var[i].pfVal = &Process[PROC_HW_PS].Fbk[3];	
	strcpy(var[i].VarName , "Process[6].Fbk[3]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitVolt;
	var[i].DefaultUnit = UNIT_Volt;
	var[i].precision = 1;

	i = varOSC_CurPos;
	strcpy(var[i].enumTXT, "varOSC_CurPos");
	strcpy(var[i].name , "act OSC Pos");
	var[i].pfVal = &Process[PROC_OSC].Fbk[0];
	strcpy(var[i].VarName , "Process[1].Fbk[0]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;

	i = varOSC_Centr;
	strcpy(var[i].enumTXT, "varOSC_Centr");
	strcpy(var[i].name , "act OSC Centr");
	var[i].pfVal = &Process[PROC_OSC].Fbk[1];
	strcpy(var[i].VarName , "Process[1].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;

	
	i = varOSC_CurRawPos;
	strcpy(var[i].enumTXT, "varOSC_CurRawPos");
	strcpy(var[i].name , "act OSC Pos");
	var[i].pfVal = &Process[PROC_OSC].Fbk[2];
	strcpy(var[i].VarName , "Process[1].Fbk[2]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 2;

	i = varOSC_Amplit_Cmd;
	strcpy(var[i].enumTXT, "varOSC_Amplit_Cmd");
	strcpy(var[i].name , "OSC Amplit Cmd");
	var[i].pfVal = &Process[PROC_OSC].Fbk[3];
	strcpy(var[i].VarName , "Process[1].Fbk[3]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 1;

	i = varOSC_curExcurTm;
	strcpy(var[i].enumTXT, "varOSC_curExcurTm");
	strcpy(var[i].name , "act Excur Tm");
	var[i].pfVal = &Process[PROC_OSC].Fbk[4];
	strcpy(var[i].VarName , "Process[1].Fbk[4]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;

	i = varOSC_ExcurTm_Cmd;
	strcpy(var[i].enumTXT, "varOSC_ExcurTm_Cmd");
	strcpy(var[i].name , "OSC Excur Tm Cmd");
	var[i].pfVal = &Process[PROC_OSC].Fbk[5];
	strcpy(var[i].VarName , "Process[1].Fbk[5]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;

	i = varOSC_Dwell_In_Cmd;
	strcpy(var[i].enumTXT, "varOSC_Dwell_In_Cmd");
	strcpy(var[i].name , "varOSC Dwell In Cmd");
	var[i].pfVal = &Process[PROC_OSC].Fbk[6];
	strcpy(var[i].VarName , "Process[1].Fbk[6]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;

	i = varOSC_Dwell_Out_Cmd;
	strcpy(var[i].enumTXT, "varOSC_Dwell_Out_Cmd");
	strcpy(var[i].name , "Dwell_Out_Cmd Cmd");
	var[i].pfVal = &Process[PROC_OSC].Fbk[7];
	strcpy(var[i].VarName , "Process[1].Fbk[7]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 1;

	
	i = varINDX_A_Pos;
	strcpy(var[i].enumTXT, "varINDX_A_Pos");
	strcpy(var[i].name , "act Indx A Pos");
	var[i].pfVal = &Process[PROC_INDX].Fbk[0];
	strcpy(var[i].VarName , "Process[10].Fbk[0]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 2;

	i = varINDX_B_Pos;
	strcpy(var[i].enumTXT, "varINDX_B_Pos");
	strcpy(var[i].name , "act Indx B Pos");
	var[i].pfVal = &Process[PROC_INDX].Fbk[1];
	strcpy(var[i].VarName , "Process[10].Fbk[1]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 2;

	i = varINDX_Dif_Pos;
	strcpy(var[i].enumTXT, "varINDX_Dif_Pos");
	strcpy(var[i].name , "act Indx Dif Pos");
	var[i].pfVal = &Process[PROC_INDX].Fbk[2];
	strcpy(var[i].VarName , "Process[10].Fbk[2]"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitLength;
	var[i].DefaultUnit = UNIT_inch;
	var[i].precision = 3;
	

	i = varGlobTimer;
	strcpy(var[i].enumTXT, "varGlobTimer");
	strcpy(var[i].name , "Timer");
	var[i].pfVal = &GlobalTimer;
	strcpy(var[i].VarName , "GlobalTimer"); 
	var[i].type = VAR_READONLY;
	var[i].PlcUnitIndx = unitIU;
	var[i].unitType = unitTime;
	var[i].DefaultUnit = UNIT_sec;
	var[i].precision = 0;

	

	////////////////////////////
	
	i = parENUM_OSC_CAM;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_OSC_CAM");
	strcpy(cfg->EnumVar[i].name , "OSC Cam Type ");
	cfg->EnumVar[i].TxtIndx = resTXT_OSC_CAM;	
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 1;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = CAM_NUM-1;

	i = parENUM_WeldProgramLinkMode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WeldProgramLinkMode");
	strcpy(cfg->EnumVar[i].name , "Load Next Program ");
	cfg->EnumVar[i].TxtIndx = resWelProgramLinkMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 1;	

	i= parENUM_PathMode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_PathMode");
	strcpy(cfg->EnumVar[i].name , "Change Head at the end ");
	cfg->EnumVar[i].TxtIndx = resPathMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 1;	

	
	i= parENUM_TVL_MODE;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_TVL_MODE");
	strcpy(cfg->EnumVar[i].name , "TVL Enc Mode");
	cfg->EnumVar[i].TxtIndx = resTVLMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 3; 



	i= parENUM_TVL_SYN_MODE;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_TVL_SYN_MODE");
	strcpy(cfg->EnumVar[i].name , "TVL Synch Mode");
	cfg->EnumVar[i].TxtIndx = resSYNCOption; // Chris add this parameter to enum
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 2; 

		
	i= parENUM_WF_SYN_MODE;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WF_SYN_MODE");
	strcpy(cfg->EnumVar[i].name , "WF Synch Mode");
	cfg->EnumVar[i].TxtIndx = resSYNCOption;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 2; 

		
	i= parENUM_WP_SYN_MODE;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WP_SYN_MODE");
	strcpy(cfg->EnumVar[i].name , "WP Synch Mode");
	cfg->EnumVar[i].TxtIndx = resSYNCOption;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 2; 
	

	/*
	i = parENUM_WeldProgramCase;
	strcpy(cfg->EnumVar[i].enumTXT , ""); // Use as a spare
	//strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WeldProgramCase");
	cfg->EnumVar[i].TxtIndx = resWeldProgramCase;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 5;	
	*/

	i = parENUM_AVC_Mode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_AVC_Mode");
	strcpy(cfg->EnumVar[i].name , "AVC Type ");
	cfg->EnumVar[i].TxtIndx = resAVCMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 3;	

	i = parENUM_TIG_Mode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_TIG_Mode");
	strcpy(cfg->EnumVar[i].name , "TIG PS Mode ");
	cfg->EnumVar[i].TxtIndx = resTIGMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 3;	

	i = parENUM_TIG_Cal_Mode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_TIG_Cal_Mode");
	strcpy(cfg->EnumVar[i].name , "TIG PS Cal. Mode ");
	cfg->EnumVar[i].TxtIndx = resTigCalMode;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 1;	


	i = parENUM_WF_WP_Relation;	
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WF_WP_Relation");
	strcpy(cfg->EnumVar[i].name , "Relation between WF and WP");
	cfg->EnumVar[i].TxtIndx = resTXT_ON_OFF;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 1; 


	i = parENUM_ARCLINK_MODE;	
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_ARCLINK_MODE");
	strcpy(cfg->EnumVar[i].name , "ARcLink ");
	cfg->EnumVar[i].TxtIndx = resTXT_ON_OFF;
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 1; 


	i = parENUM_OSC_Mode;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_OSC_Mode");
	strcpy(cfg->EnumVar[i].name , "OSC Mode ");
	cfg->EnumVar[i].TxtIndx = resTXT_OSC_Mode;	
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 3;

	i = parENUM_OSC_Synch;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_OSC_Synch");
	strcpy(cfg->EnumVar[i].name , "OSC Indx Synch ");
	cfg->EnumVar[i].TxtIndx = resTXT_OSC_Synch;	
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 4;

	i = parENUM_WF_AUX;
	strcpy(cfg->EnumVar[i].enumTXT , "parENUM_WF_AUX");
	strcpy(cfg->EnumVar[i].name , "parENUM_WF_AUX");
	cfg->EnumVar[i].TxtIndx = resTXT_WF_AUX;	
	cfg->EnumVar[i].segm = 0;
	cfg->Param_ENUM[i].defaultVal = 0;
	cfg->Param_ENUM[i].min = 0;
	cfg->Param_ENUM[i].max = 2;

	///////Int Variable    //////////	

	i = parINT_NumZone;
	strcpy(cfg->IntVar[i].enumTXT , "parINT_NumZone");
	strcpy(cfg->IntVar[i].name , 	"Segments per Travel");
	cfg->IntVar[i].pVal = &curCfg->Param_INT[parINT_NumZone].val[0];
	strcpy(cfg->IntVar[i].VarName , "CurCfg.Param_INT[0].val[0]"); //this is for Android
	cfg->IntVar[i].DefaultVal = 1;
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000;
	cfg->IntVar[i].segm = 0;
	
	i = varINT_PIN_PER_ROT;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_PIN_PER_ROT");
	strcpy(cfg->IntVar[i].name , 	"Pins per 360 degree");
	cfg->IntVar[i].pVal = &calib->GenSetup[SETUP_PIN_PER_ROT];
	strcpy(cfg->IntVar[i].VarName , "CurCalib.GenSetup[0]"); //this is for Android
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000;

	i = varINT_SEGM_NO;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_SEGM_NO");
	strcpy(cfg->IntVar[i].name , 	"Segments Number");
	cfg->IntVar[i].pVal = &calib->GenSetup[SETUP_SEGM_NO];
	strcpy(cfg->IntVar[i].VarName , "CurCalib.GenSetup[1]"); //this is for Android
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000;

	/*	
	i = varINT_ZONE_NO;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_ZONE_NO");
	cfg->IntVar[i].pVal = &calib->GenSetup[SETUP_ZONE_NO];
	strcpy(cfg->IntVar[i].VarName , "CurCalib.GenSetup[2]"); //this is for Android
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000;	

	i= varINT_curSegm;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_curSegm");
	strcpy(cfg->IntVar[i].name , 	"Current Pin");
	cfg->IntVar[i].pVal = &m->curSegm[HEAD_SEGM];
	strcpy(cfg->IntVar[i].VarName , "Master[0].curSegm[3]"); //this is for Android
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000; 
	*/

	i = varINT_curZone;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_curZone");
	strcpy(cfg->IntVar[i].name , 	"Current Segment");
	cfg->IntVar[i].pVal = &m->curSegm[ZONE];
	strcpy(cfg->IntVar[i].VarName , "Master[0].curSegm[1]"); //this is for Android
	cfg->IntVar[i].Min = 1;
	cfg->IntVar[i].Max = 1000; 

	i = varINT_StartPos;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_StartPos");
	strcpy(cfg->IntVar[i].name , 	"Start Position");
	cfg->IntVar[i].pVal = &StartPos;
	strcpy(cfg->IntVar[i].VarName , "StartPos"); //this is for Android
	cfg->IntVar[i].Min = -32767;
	cfg->IntVar[i].Max = 32767;

	i = varINT_FinalPos;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_FinalPos");
	strcpy(cfg->IntVar[i].name , 	"Target Position");
	cfg->IntVar[i].pVal = &FinalPos;
	strcpy(cfg->IntVar[i].VarName , "FinalPos"); //this is for Android
	cfg->IntVar[i].Min = -32767;
	cfg->IntVar[i].Max = 32767;

	i = varINT_CurTVLPos;
	strcpy(cfg->IntVar[i].enumTXT , "varINT_CurTVLPos");
	strcpy(cfg->IntVar[i].name , 	"Current TVL Position");
	cfg->IntVar[i].pVal = &m->curSegm[ACT_TVL_DIST];
	strcpy(cfg->IntVar[i].VarName , "Master[0].curSegm[4]"); //this is for Android
	cfg->IntVar[i].Min = -32767;
	cfg->IntVar[i].Max = 32767;
	
	
	

	/// Strings
	i = parSTRING_FreeTXT;
	strcpy(cfg->StringVar[i].enumTXT , "parSTRING_FreeTXT");
	strcpy(cfg->StringVar[i].name, "FreeText");
	cfg->Param_STR[i].len = 5;
	if(strlen(curCfg->Param_STR[i].str) == 0 )
		strcpy(curCfg->Param_STR[i].str , "12345");	

	i = varSTRING_NextProgramName;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_NextProgramName");
	strcpy(cfg->StringVar[i].name, "Next Program Name");
	cfg->StringVar[i].pStr = curCfg->NextProgramName;
	strcpy(cfg->StringVar[i].VarName , "CurCfg.NextProgramName");
	cfg->StringVar[i].Len = 20;


	i = varSTRING_curProgramName;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_curProgramName");
		strcpy(cfg->StringVar[i].name, "Current Program Name");
	cfg->StringVar[i].pStr   = curCfg->CurProgramName;
	strcpy(cfg->StringVar[i].VarName , "CurCfg.CurProgramName");
	cfg->StringVar[i].Len = 20;


	/*
	i = varSTRING_WelProgramName;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_WelProgramName");
	strcpy(cfg->StringVar[i].name, "Weld Program Name");
	cfg->StringVar[i].pStr = cfg->TXT_Resources[resWeldProgramName];
	strcpy(cfg->StringVar[i].VarName , "Cfg.TXT_Resources[7]");
	cfg->StringVar[i].Len = 15;
	strcpy(cfg->TXT_Resources[resWeldProgramName], "myFile000"); // temporary string for new save name


	i = varSTRING_Password;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_Password");
	strcpy(cfg->StringVar[i].name, "Password ");
	cfg->StringVar[i].pStr	= m->Password;
	strcpy(cfg->StringVar[i].VarName , "Master[0].Password");
	cfg->StringVar[i].Len = 3;
	strcpy(m->Password , "000");
	*/

	for(i = varSTRING_HMI_0 ; i <= varSTRING_HMI_19 ; i++){
		xsprintf(cfg->StringVar[i].enumTXT , "varSTRING_HMI_%d" ,i - varSTRING_HMI_0 );
		xsprintf(cfg->StringVar[i].name, "HMI String Line_%d" ,i - varSTRING_HMI_0 );
		cfg->StringVar[i].pStr	= HMI_lines[ i - varSTRING_HMI_0];	
		cfg->StringVar[i].Len   = 20;
		xsprintf(cfg->StringVar[i].VarName , "HMI_lines[%d]" , i - varSTRING_HMI_0);
	}

	i = varSTRING_UserName;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_UserName");
	strcpy(cfg->StringVar[i].name , "User Name");
	cfg->StringVar[i].pStr	= UserName;
	strcpy(cfg->StringVar[i].VarName , "UserName");
	cfg->StringVar[i].Len = 20;


	i = varSTRING_PlcVersion;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_PlcVersion");
	strcpy(cfg->StringVar[i].enumTXT , "PLC Program Version");
	cfg->StringVar[i].pStr = PLC_Version_TXT;
	strcpy(cfg->StringVar[i].VarName , "PLC_Version_TXT");
	cfg->StringVar[i].Len = 20;


	i = varSTRING_FileName;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_FileName");
	strcpy(cfg->StringVar[i].name , "File Name");
	cfg->StringVar[i].pStr = File_RW.FileInfo[0].Filename;
	strcpy(cfg->StringVar[i].VarName , "File_RW.FileInfo[0].Filename");
	cfg->StringVar[i].Len = 20;
	

	i = varSTRING_MasteStepTXT;
	strcpy(cfg->StringVar[i].enumTXT , "varSTRING_MasteStepTXT");
	strcpy(cfg->StringVar[i].name , "Master State Machine Step");
	cfg->StringVar[i].pStr = m->sm.Text;
	strcpy(cfg->StringVar[i].VarName , "Master[0].sm.Text");
	cfg->StringVar[i].Len = 20;

	
	/////////////  Auto Indexing //////////////
	
	for(i = 0 ; i < m->paramIntgLastNum; i++)
		cfg->IntVar[i].Indx = i;	

	for(i = 0 ; i < m->paramEnumLastNum; i++)
		cfg->EnumVar[i].Indx = i; 	

	for(i = 0 ; i < m->varLastNum; i++)
		var[i].Indx = i;	

	for(i = 0 ; i < m->paramStrLastNum; i++)
		cfg->StringVar[i].Indx = i;

	

	// Text resources
	memcpy(cfg->TXT_Resources[resTXT_OSC_CAM] , CAM_TYPE_TXT , sizeof(CAM_TYPE_TXT));
	memcpy(cfg->TXT_Resources[resTXT_ON_OFF] , ON_OFF_TXT, sizeof(ON_OFF_TXT));
	memcpy(cfg->TXT_Resources[resWelProgramLinkMode] , JOBLINK_MODE_TXT, sizeof(JOBLINK_MODE_TXT));
	memcpy(cfg->TXT_Resources[resPathMode] , PATH_MODE_TXT, sizeof(PATH_MODE_TXT));
	memcpy(cfg->TXT_Resources[resWeldProgramCase] , JOB_CASE_TXT, sizeof(JOB_CASE_TXT));
	memcpy(cfg->TXT_Resources[resAVCMode] , AVC_MODE_TXT, sizeof(AVC_MODE_TXT));
	memcpy(cfg->TXT_Resources[resTIGMode] , TIG_MODE_TXT, sizeof(TIG_MODE_TXT));
	memcpy(cfg->TXT_Resources[resTVLMode] , TVL_MODE_TXT, sizeof(TVL_MODE_TXT));
	memcpy(cfg->TXT_Resources[resSYNCOption] , SYNCH_OPTION_TXT, sizeof(SYNCH_OPTION_TXT));
	memcpy(cfg->TXT_Resources[resTigCalMode] , TIG_CAL_MODE_TXT, sizeof(TIG_CAL_MODE_TXT));
	memcpy(cfg->TXT_Resources[resTXT_OSC_Mode] , OSC_MODE_TXT, sizeof(OSC_MODE_TXT));
	memcpy(cfg->TXT_Resources[resTXT_OSC_Synch] , OSC_SYNCH_TXT, sizeof(OSC_SYNCH_TXT));
	memcpy(cfg->TXT_Resources[resTXT_WF_AUX] , WF_AUX_TXT, sizeof(WF_AUX_TXT));
	


	/****************************/  
	/*****  ERROR text ***********/


	strcpy(ErrorMgr.logs[ERROR_NONE].txt , " No Erros - Ready");

	/* 
	strcpy(ErrorMgr.logs[ERROR_TML_1].txt , "TML1 :");
	strcpy(ErrorMgr.logs[ERROR_TML_2].txt , "TML2 :");
	strcpy(ErrorMgr.logs[ERROR_TML_3].txt , "TML3 :");
	strcpy(ErrorMgr.logs[ERROR_TML_4].txt , "TML4 :");
	strcpy(ErrorMgr.logs[ERROR_TML_5].txt , "TML5 :");
	strcpy(ErrorMgr.logs[ERROR_TML_6].txt , "TML6 :");
	strcpy(ErrorMgr.logs[ERROR_TML_7].txt , "TML7 :");
	strcpy(ErrorMgr.logs[ERROR_TML_8].txt , "TML8 :");
	strcpy(ErrorMgr.logs[ERROR_TML_9].txt , "TML9 :");
	strcpy(ErrorMgr.logs[ERROR_TML_10].txt , "TML10 :");

	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_1].txt , "TML1 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_2].txt , "TML2 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_3].txt , "TML3 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_4].txt , "TML4 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_5].txt , "TML5 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_6].txt , "TML6 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_7].txt , "TML7 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_8].txt , "TML8 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_9].txt , "TML9 FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_TML_10].txt , "TML10 FATAL");				


	strcpy(ErrorMgr.logs[ERROR_ACP_1].txt , "BR Axis #1");
	strcpy(ErrorMgr.logs[ERROR_ACP_2].txt , "BR Axis #2");
	strcpy(ErrorMgr.logs[ERROR_ACP_3].txt , "BR Axis #3");
	strcpy(ErrorMgr.logs[ERROR_ACP_4].txt , "BR Axis #4");
	strcpy(ErrorMgr.logs[ERROR_ACP_5].txt , "BR Axis #5");
	strcpy(ErrorMgr.logs[ERROR_ACP_6].txt , "BR Axis #6");
	strcpy(ErrorMgr.logs[ERROR_ACP_7].txt , "BR Axis #7");
	strcpy(ErrorMgr.logs[ERROR_ACP_8].txt , "BR Axis #8");

	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_1].txt , "Axis #1 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_2].txt , "Axis #2 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_3].txt , "Axis #3 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_4].txt , "Axis #4 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_5].txt , "Axis #5 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_6].txt , "ACP6 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_7].txt , "ACP7 ERR FATAL");
	strcpy(ErrorMgr.logs[ERROR_FATAL_ACP_8].txt , "ACP8 ERR FATAL");
	*/

	//strcpy(ErrorMgr.logs[ERROR_MOTOR_INIT].txt , "MTRS INIT TMO");

	//strcpy(ErrorMgr.logs[ERROR_ANGLE_NOT_EQ].txt , "ANGLE NOT SYNCHRO");

	strcpy(ErrorMgr.logs[ERROR_TIG_TOUCH_TMO].txt , "AVC TOUCH TMO");
	strcpy(ErrorMgr.logs[ERROR_TIG_SP_TMO].txt , 	"AVC VOLT TMO");
	strcpy(ErrorMgr.logs[ERROR_TIG_VOLT_ERR].txt , 	"AVC VOLT ERR");
	strcpy(ErrorMgr.logs[ERROR_TIG_AVC_LIMIT_ERR].txt , "AVC LIMITS ERR");	


	strcpy(ErrorMgr.logs[ERROR_FILE_RW].txt , 	"File RW Err");

	strcpy(ErrorMgr.logs[ERROR_BR_HOME].txt , 	"BR Srv Home Err");

	strcpy(ErrorMgr.logs[ERROR_GAS_TMO].txt , 	"ARGON Tmo Err");

	strcpy(ErrorMgr.logs[ERROR_COOL_TMO].txt , 	"COOL Tmo Err");

	strcpy(ErrorMgr.logs[ERROR_TVL_STALL].txt ,  "TVL Stall Err");

	strcpy(ErrorMgr.logs[ERROR_TVL_STALL].txt ,  "TVL Stall Err");

	strcpy(ErrorMgr.logs[ERROR_ADROID_COM_LOST].txt ,  "Pendant Err");

	strcpy(ErrorMgr.logs[ERROR_ARCLINK_COM_LOST].txt ,  "ArcLink Com Lost");
	
	strcpy(ErrorMgr.logs[ERROR_ARCLINK_PS_ERR].txt ,  "ArcLink PS Err");

	strcpy(ErrorMgr.logs[ERROR_PROJECT_HRDW_ERR].txt ,  "WRONG CONTROLLER");

	strcpy(ErrorMgr.logs[ERROR_ESTOP_ERR].txt ,  "ESTOP !!!");

	strcpy(ErrorMgr.logs[ERROR_MODULE_ERR].txt ,  "MODULE MISSING");
	
	strcpy(ErrorMgr.logs[ERROR_TIG_MIG_SW_ERR].txt ,  "TIG/MIG Sw Err");
	
	

	//  Load Default Min Max for ENUM data

	for (i = 0 ; i < parENUM_LAST_PARAM; i++){

		curCfg->Param_ENUM[i].segmYes = 0;

		curCfg->Param_ENUM[i].incr = 1;
		curCfg->Param_ENUM[i].min = cfg->Param_ENUM[i].min;
		curCfg->Param_ENUM[i].max = cfg->Param_ENUM[i].max;

		
	}	

	for (i = 0 ; i < parINT_LAST_PARAM; i++){

		if(curCfg->Param_INT[i].incr == 0)
			curCfg->Param_INT[i].incr = 1.0;

	}	


	// Offset for File Manager

	File_RW._int.OffsetToWeldProgramRepeat = (UDINT)&File_RW.curCfgTemp.Param[parGEN_WeldProgramRepeat].val[0] - (UDINT)&File_RW.curCfgTemp;
	File_RW._int.OffsetToWeldProgramLinkMode = (UDINT)&File_RW.curCfgTemp.Param_ENUM[parENUM_WeldProgramLinkMode].val[0] - (UDINT)&File_RW.curCfgTemp;


	return;
}

void SetPageDict(PAGE_DICT_typ* dict , UINT indx , char* pTxt){

	if(dict){

		dict->indx = indx;
		strcpy(dict->enumTXT , pTxt);

	}

	return;

}

#endif
