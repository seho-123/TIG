
/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: LCD
 * File: LCDLib.c
 * Author: wsi
 * Created: March 01, 2013
 *******************************************************************/

#ifndef _LCDLIB_C
#define _LCDLIB_C


#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#define mm2inch 0.03936996
#define in_min_2_mm_s 25.4/60.0
#define in_2_mm 25.4


// zamienic na param aby bylo dynamicznie: parLAST_PARAM  , m->varLastNum , NUM_LINE 

void SerialPortInit(SerialPortDriver_typ *p, char *deviceStr, char *baudRateStr ) ;
void SerialPortClose(SerialPortDriver_typ *p) ;
void SerialPortCyclic(SerialPortDriver_typ *p) ;

DINT LCD_MGR_Srv(LCD_MGR_obj* lcd );

DINT LCD_MGR_Init(LCD_MGR_obj* lcd , UINT lineNum , UINT nodesNum, MASTER_typ *ms ,SerialPortDriver_typ *pDevice,  UDINT pNodes , UINT initNode , float tasktime );

void DisplayPage(LCD_MGR_obj* lcd );
void Build_N_Edit_SCALED(LCD_MGR_obj* lcd);
void Build_Segment_Select_Page(LCD_MGR_obj* lcd , Scaled_Var_obj* var);
void Build_VarSetup_Item_Select_Page(LCD_MGR_obj* lcd , Scaled_Var_obj* var);
void Build_VarSetup_Edit_Page(LCD_MGR_obj* lcd , Scaled_Var_obj* var);
void Build_Val_Edit_Page(LCD_MGR_obj* lcd  , Scaled_Var_obj* var);
void LinePad(char* l , UINT len); 
void Send_LCD_Cmd(LCD_MGR_obj* mgr , UINT cmd );
void IncrParamNew(LCD_MGR_obj* lcd , Scaled_Var_obj* var, INT incr , BOOL _enum , BOOL overlap);
char* GetScaledVal(LCD_MGR_obj* lcd , Scaled_Var_obj* var , USINT blink);
char* GetKeyPad(LCD_MGR_obj* mgr , char *p  , UINT type , USINT blink);
void IncrKeyPad(LCD_MGR_obj* lcd , char *p , INT incr );
char* GetScaledMinMax(LCD_MGR_obj* lcd , Scaled_Var_obj* var );
void LoadDefaultParam(MASTER_typ *m);
BOOL CheckCurCfg(MASTER_typ *m);
BOOL LoadDefaultCalib(MASTER_typ *m);
DINT getNodeTxt(LCD_MGR_obj *lcd , char* out ,LCD_NODE_obj* node);


DINT LCD_MGR_Init(LCD_MGR_obj* lcd , UINT lineNum , UINT nodesNum, MASTER_typ *ms ,SerialPortDriver_typ *pDevice,  UDINT pNodes , UINT initNode , float tasktime ){

int i;
Units_obj *unit;
		


	if(!lcd)
		return -1;

	if(!ms)
		return -2;

	if(!pDevice)
		return -3;

	if(!pNodes)
		return -4;
	

	lcd->LineID = 0;

	lcd->pDevice = pDevice;
		
	lcd->pMaster = ms;

	lcd->Intr.tasktime = tasktime;

	//lcd->paramLastNum = ms->paramLastNum;

	//lcd->varLastNum = ms->varLastNum;

	lcd->lineNum = lineNum;

	lcd->nodesNum = nodesNum;

	
	lcd->Intr.displStep = 0;

	lcd->pMyNodes = (LCD_NODE_obj*)pNodes;

	for (i = 0 ; i < lcd->lineNum ; i++)
		lcd->Intr.pNodes[i]= &lcd->pMyNodes[initNode+i];


	TimerStart(&lcd->MsgTmr ,0.1);

	for (i = 0 ; i < lcd->lineNum+1 ; i++)
		strcpy(lcd->Message[i] , "");


	
	unit = (CurCalib_typ*) ms->pUnits;

	if(!unit) 
		return -1;
	
	/************************************************/
	/**********   Units *******************************/

	unit[unitIU].unitType = unitIU;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;

	/************/
	unit[unitLength].unitType=unitLength;
	unit[unitLength].unitScale[UNIT_IU].gain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].offset = 0.0;
	unit[unitLength].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_IU].txt , "");

	unit[unitLength].unitScale[UNIT_mm].gain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_mm].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_mm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_mm].txt , "mm");

	
	unit[unitLength].unitScale[UNIT_inch].gain = mm2inch;
	unit[unitLength].unitScale[UNIT_inch].offset = 0.0;
	unit[unitLength].unitScale[UNIT_inch].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_inch].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_inch].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_inch].txt , "in");

	
	unit[unitLength].unitScale[UNIT_cm].gain = 0.1;
	unit[unitLength].unitScale[UNIT_cm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_cm].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_cm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_cm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_cm].txt , "cm");


	/************/
	
	unit[unitSpeed].unitType=unitSpeed;
	unit[unitSpeed].unitScale[UNIT_IU].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_IU].txt , "");
		
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_mm_per_sec].txt , "mm/s");

	unit[unitSpeed].unitScale[UNIT_inch_per_min].gain = mm2inch * 60;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].extraDigits = 1;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_min].txt , "ipm");

	
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].gain = 1.0/25.4;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].extraDigits = 2;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_sec].txt , "ips");


	/************/
	
	unit[unitAcc].unitType=unitAcc;
	unit[unitAcc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_IU].txt , "");
		
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_mm_per_sec2].txt , "mm/s2");

	unit[unitAcc].unitScale[UNIT_inch_per_sec2].gain = 0.03937;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_inch_per_sec2].txt , "in/s2");

	
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].gain = 0.1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_cm_per_sec2].txt , "cm/s2");

	
	/************/
		
	unit[unitRot].unitType=unitRot;
	unit[unitRot].unitScale[UNIT_IU].gain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].offset = 0.0;
	unit[unitRot].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_IU].txt , "");
		
	unit[unitRot].unitScale[UNIT_RPS].gain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPS].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_RPS].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPS].txt , "rps");

	unit[unitRot].unitScale[UNIT_RPM].gain = 0.01666666;
	unit[unitRot].unitScale[UNIT_RPM].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPM].extraDigits = 1;
	unit[unitRot].unitScale[UNIT_RPM].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPM].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPM].txt , "rpm");

	unit[unitRot].unitScale[UNIT_RPh].gain = 2.777e-8;
	unit[unitRot].unitScale[UNIT_RPh].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPh].extraDigits = 2;
	unit[unitRot].unitScale[UNIT_RPh].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPh].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPh].txt , "rph");

	/************/
		
	unit[unitTime].unitType=unitTime;
	unit[unitTime].unitScale[UNIT_IU].gain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].offset = 0.0;
	unit[unitTime].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_IU].txt , "");
		
	unit[unitTime].unitScale[UNIT_sec].gain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].offset = 0.0;
	unit[unitTime].unitScale[UNIT_sec].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_sec].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_sec].txt , "sec");

	unit[unitTime].unitScale[UNIT_min].gain = 0.0166666;
	unit[unitTime].unitScale[UNIT_min].offset = 0.0;
	unit[unitTime].unitScale[UNIT_min].extraDigits = 2;
	unit[unitTime].unitScale[UNIT_min].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_min].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_min].txt , "min");

	unit[unitTime].unitScale[UNIT_h].gain = 2.777777e-4;
	unit[unitTime].unitScale[UNIT_h].offset = 0.0;
	unit[unitTime].unitScale[UNIT_h].extraDigits = 4;
	unit[unitTime].unitScale[UNIT_h].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_h].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_h].txt , "h");


	/************/
		
	unit[unitAmp].unitType=unitAmp;
	unit[unitAmp].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_IU].txt , "");
		
	unit[unitAmp].unitScale[UNIT_Amp].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_Amp].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_Amp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_Amp].txt , "A");

	unit[unitAmp].unitScale[UNIT_kAmp].gain = 0.001;
	unit[unitAmp].unitScale[UNIT_kAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_kAmp].extraDigits = 3;
	unit[unitAmp].unitScale[UNIT_kAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_kAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_kAmp].txt , "kA");

	unit[unitAmp].unitScale[UNIT_mAmp].gain = 1000.0;
	unit[unitAmp].unitScale[UNIT_mAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_mAmp].extraDigits = -3;
	unit[unitAmp].unitScale[UNIT_mAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_mAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_mAmp].txt , "mA");

	/************/
		
	unit[unitVolt].unitType=unitVolt;
	unit[unitVolt].unitScale[UNIT_IU].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_IU].txt , "");
		
	unit[unitVolt].unitScale[UNIT_Volt].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_Volt].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_Volt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_Volt].txt , "V");

	unit[unitVolt].unitScale[UNIT_kVolt].gain = 0.001;
	unit[unitVolt].unitScale[UNIT_kVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_kVolt].extraDigits = 3;
	unit[unitVolt].unitScale[UNIT_kVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_kVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_kVolt].txt , "kV");


	unit[unitVolt].unitScale[UNIT_mVolt].gain = 1000.0;
	unit[unitVolt].unitScale[UNIT_mVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_mVolt].extraDigits = -3;
	unit[unitVolt].unitScale[UNIT_mVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_mVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_mVolt].txt , "mV");


	
	/************/
		
	unit[unitFreq].unitType=unitFreq;
	unit[unitFreq].unitScale[UNIT_IU].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_IU].txt , "");
		
	unit[unitFreq].unitScale[UNIT_Hz].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_Hz].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_Hz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_Hz].txt , "Hz");

	unit[unitFreq].unitScale[UNIT_kHz].gain = 0.001;
	unit[unitFreq].unitScale[UNIT_kHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_kHz].extraDigits = 3;
	unit[unitFreq].unitScale[UNIT_kHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_kHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_kHz].txt , "kHz");


	unit[unitFreq].unitScale[UNIT_mHz].gain = 0.000001;
	unit[unitFreq].unitScale[UNIT_mHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_mHz].extraDigits = 6;
	unit[unitFreq].unitScale[UNIT_mHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_mHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_mHz].txt , "mHz");


	
	/************/
			
	unit[unitPerc].unitType=unitPerc;
	unit[unitPerc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_IU].txt , "");
		
	unit[unitPerc].unitScale[UNIT_PERCENT].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT].txt , "%");

	unit[unitPerc].unitScale[UNIT_PERCENT+1].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+1].txt , "%");


	unit[unitPerc].unitScale[UNIT_PERCENT+2].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+2].txt , "%");


	/************/
			
	unit[unitAngle].unitType=unitAngle;
	unit[unitAngle].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_IU].txt , "");
		
	unit[unitAngle].unitScale[UNIT_DEG].gain = 360.0;
	unit[unitAngle].unitScale[UNIT_DEG].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_DEG].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_DEG].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_DEG].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_DEG].txt , " deg");

	unit[unitAngle].unitScale[UNIT_RAD].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD].txt , " rad");

	unit[unitAngle].unitScale[UNIT_RAD+1].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD+1].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD+1].txt , " rad");



	return 1;
	
	
}

DINT  LCD_MGR_Srv(LCD_MGR_obj* lcd ){
DINT i;
UINT len;
int _LineID;
USINT error;
USINT incr;
UINT line;
INT delta;
LCD_NODE_obj *node,  *nextNode , *prevNode , *head;
Enum_Var_obj *enumVar;
Integer_Var_obj *intVar;
DINT *pDINT;
Cfg_typ *cfg;
CurCfg_typ *curCfg;
BOOL _enum ,done;
String_Var_obj *strVar;
char* pStr;



	/*****/
	if(!lcd)
		return -1;

	/*****/
	if(!lcd->Intr.pNodes)
		return -2;

	/*****/
	if(!lcd->pMaster)
		return -3;

	/*****/
	
	if(lcd->pMaster->pCfg)
		cfg = lcd->pMaster->pCfg;
	else
		return -4;
	
	/*****/

	if(lcd->pMaster->pCurCfg)
		curCfg = lcd->pMaster->pCurCfg;
	else
		return - 5;

	

	/// Commands
	if(!lcd->IntrfCmdDisable && lcd->MsgTmr.Q){

	
		if(lcd->Intrf.TickPlus)
	 		delta = 1;
		else if(lcd->Intrf.TickMinus)
			delta = (-1);
		else
			delta = 0;
		

		lcd->Intrf.WheelDelta = delta;
		

		
		if(lcd->Intrf.LCDEntr_NegEdge && lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT){		

						
			if(lcd->Intr.pNodes[lcd->LineID]->var.Type & (T_STRING)){
			
				
				pStr = 0;
				
				strVar = &cfg->StringVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID]; 

				if(strVar->pStr) // by pointer
					pStr = strVar->pStr;
				
				else if(strVar->Indx < lcd->pMaster->paramStrLastNum) // in CurCfg area
					pStr = curCfg->Param_STR[strVar->Indx].str;
				
				if(pStr){
					
					lcd->Intr.keyPos++;					
					len = strlen(pStr);
					
					if(len > 0){

						if(lcd->Intr.keyPos >= len)
							lcd->Intr.keyPos = 0;


					}
					else
						lcd->Intr.keyPos = 0;

				}
					
				

			}

		}	
		

		if(lcd->Intrf.LCDEntr_PosEdge && lcd->Intr.pNodes[lcd->LineID]){	


			if(lcd->Intr.pNodes[lcd->LineID]->Type & N_EVENT)
				lcd->FiredEvent = lcd->Intr.pNodes[lcd->LineID]->Event;

			if((lcd->Intr.pNodes[lcd->LineID]->Type & N_JUMP)){
				
				lcd->pNode2Jump = (LCD_NODE_obj*)lcd->Intr.pNodes[lcd->LineID]->pJumpNode;	
				lcd->xCMD = JUMP_CMD;	
			}

		}
		
		else if(lcd->Intrf.TickPlus || lcd->Intrf.TickMinus
			|| (delta != 0)){
			
			lcd->xCMD = UPDATE_CMD;
		}	
	
	}
	else
		delta = 0;
	

	if(lcd->IntrfCmdDisable)
		lcd->xCMD = 0;

	// Reset editStep when node is not editable
	if(!(lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT))
		lcd->Intr.editStep = 0;
			
	// Command execution
	switch(lcd->xCMD){


		case IDLE_CMD:


		break;

		case JUMP_CMD:

			if(lcd->pNode2Jump && (lcd->UserLevel >= lcd->Intr.pNodes[lcd->LineID]->MinUserLevel)){

				lcd->LineID = lcd->JumpLineID; 	
				
				if((lcd->LineID >= lcd->lineNum) || (lcd->LineID < 0))
					lcd->LineID = 0;
						

				
				lcd->Intr.pNodes[0] = lcd->pNode2Jump;
				
				
				for(i = 0 ; i < lcd->lineNum ; i++){

					//lcd->Intr.pNodes[i] = lcd->Intr.pNodes[i]->pNextNode;									
					lcd->Intr.pNodes[i]= lcd->Intr.pNodes[0]+i;
					//lcd->Intr.sm[i].Step = 0;
				}		

		
			}
		

		break;	

		case JUMP2ID_CMD:

			if((lcd->Node2JumpID < lcd->nodesNum) && (lcd->UserLevel >= lcd->Intr.pNodes[lcd->LineID]->MinUserLevel)){

				lcd->LineID = lcd->JumpLineID; 	
				
				if((lcd->LineID >= lcd->lineNum) || (lcd->LineID < 0))
					lcd->LineID = 0;
				
				lcd->Intr.pNodes[0] = &lcd->pMyNodes[lcd->Node2JumpID];
				
				
				for(i = 0 ; i < lcd->lineNum ; i++){

					//lcd->Intr.pNodes[i] = lcd->Intr.pNodes[i]->pNextNode;									
					lcd->Intr.pNodes[i]= lcd->Intr.pNodes[0]+i;
					//lcd->Intr.sm[i].Step = 0;
				}		

		
			}
		

		break;	
		

		case UPDATE_CMD:
			
			line = 0;
			done = 0;

	
		
			if(!lcd->Intr.pNodes[lcd->LineID])
				break;					
			
		
			switch(lcd->Intr.pNodes[lcd->LineID]->Type & 0x0F){


				case N_STAT:
				case N_JUMP:
				case N_EDIT:	
			
					if((!(lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT_DIS))
						&& (lcd->UserLevel >= lcd->Intr.pNodes[lcd->LineID]->MinUserLevel)) {

						
						if(!lcd->Intr.pNodes[lcd->LineID])
							break;

						if(delta!=0){

							
							
							switch(lcd->Intr.pNodes[lcd->LineID]->var.Type & 0xFF){


								
								case T_NATIVE:
								
								
								break;								

								case T_INTEGER:

									
									//if(lcd->EntrBtnPressReq){
									if(lcd->Intrf.LCDEntr){ 						

									
										intVar = &cfg->IntVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID];
									
																											
										if(intVar->Indx < lcd->pMaster->paramEnumLastNum) // in CurCfg area
											pDINT = &curCfg->Param_INT[intVar->Indx].val[0]; // now only one segment

										else if(intVar->pVal) // by pointer
											pDINT = (DINT*) intVar->pVal;	
										
										else
											break;

										if(delta > 0)
											(*pDINT) += 1;
										
										else if(delta < 0)
											(*pDINT) -= 1;

										
										if(intVar->Indx < lcd->pMaster->paramIntgLastNum) {// in CurCfg area

											
											if((*pDINT)< lcd->pMaster->pCurCfg->Param_INT[enumVar->Indx].min)
												(*pDINT) = lcd->pMaster->pCurCfg->Param_INT[enumVar->Indx].min;
											
											else if((*pDINT)> lcd->pMaster->pCurCfg->Param_INT[enumVar->Indx].max)
												(*pDINT) = lcd->pMaster->pCurCfg->Param_INT[enumVar->Indx].max;

										}
										else{
											if((*pDINT)< intVar->Min)
												(*pDINT) = intVar->Min;
											
											else if((*pDINT)> intVar->Max)
												(*pDINT) = intVar->Max;

										}
										

											
									}

									
									


								break;

								case T_ENUM:


									//if(lcd->EntrBtnPressReq){
									if(lcd->Intrf.LCDEntr){							

									
										enumVar = &cfg->EnumVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID];
									
																											
										if(enumVar->Indx < lcd->pMaster->paramEnumLastNum) // in CurCfg area
											pDINT = &curCfg->Param_ENUM[enumVar->Indx].val[0];

										else if(enumVar->pVal) // by pointer
											pDINT = (DINT*) enumVar->pVal;	
										
										else
											break;

										if(delta > 0)
											(*pDINT) += 1;
										
										else if(delta < 0)
											(*pDINT) -= 1;

										
										if(enumVar->Indx < lcd->pMaster->paramEnumLastNum) {// in CurCfg area

											
											if((*pDINT)< lcd->pMaster->pCurCfg->Param_ENUM[enumVar->Indx].min)
												(*pDINT) = lcd->pMaster->pCurCfg->Param_ENUM[enumVar->Indx].min;
											
											else if((*pDINT)> lcd->pMaster->pCurCfg->Param_ENUM[enumVar->Indx].max)
												(*pDINT) = lcd->pMaster->pCurCfg->Param_ENUM[enumVar->Indx].max;

										}
										else{
											if((*pDINT)< enumVar->Min)
												(*pDINT) = enumVar->Min;
											
											else if((*pDINT)> enumVar->Max)
												(*pDINT) = enumVar->Max;

										}
										

											
									}

									
									

								break;


								case T_STRING:

									pStr = 0;
				
									strVar = &cfg->StringVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID]; 

									if(strVar->pStr) // by pointer
										pStr = strVar->pStr;
									
									else if(strVar->Indx < lcd->pMaster->paramStrLastNum) // in CurCfg area
										pStr = curCfg->Param_STR[strVar->Indx].str;

									if(pStr)
										IncrKeyPad(lcd , pStr , delta);	

									


								break;

								case T_SCALED:

									if(lcd->Intr.editStep == 3){

										if(lcd->Intr.pNodes[lcd->LineID]->var.Var1ID < lcd->pMaster->varLastNum){
											
											IncrParamNew(lcd ,&lcd->pMaster->pCfg->ScaledVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID], delta , 0 , 0);
										
											
										}
									}


								break;



								

							}

							curCfg->PleaseUpdate = 1;

						}
			
							
					}
					

				case N_SCROLL_PG:
				case N_SCROLL:	

					// Do Not Scroll when enter pressed
					if(lcd->Intrf.LCDEntr) 
						break;

					// Do Not Scroll when Param item is selecting or editing 
					if((lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT)
						&& (lcd->Intr.editStep != 0))
						break;

					node = lcd->Intr.pNodes[lcd->LineID];
					
					error = 0;
					_LineID = lcd->LineID;

					if(lcd->Intr.pNodes[lcd->LineID]->Type & N_SCROLL_PG)
						incr = lcd->lineNum;
					else
						incr = 1;

					
					
					if(lcd->Intrf.TickPlus && !lcd->Intrf.LCDEntr ){

						node += incr;
						_LineID += incr;
						
						do{							

							if(!node)
								error = 1;
							
							else if(node->pHeadNode != lcd->Intr.pNodes[lcd->LineID]->pHeadNode)
								error  = 1;
							
							else if(node->Type != N_STAT)
								done = 1;


							if(error)
								done = 1;

							else if(!done){

								node += incr;
								_LineID += incr;
							}
							else{


								//lcd->LineID += (_LineID + incr) %lcd->lineNum;

								if(_LineID >= lcd->lineNum && (incr == 1))
									_LineID = lcd->lineNum - 1;
								else
									_LineID = _LineID % lcd->lineNum;

																
								lcd->LineID = _LineID;
															

							}

						}while(!done);	
									

						if(!error){		
							
							lcd->Intr.pNodes[0] = node - lcd->LineID  ;
								
							for(i = 1  ; i < lcd->lineNum ; i++){	
								//lcd->Intr.pNodes[i] = lcd->Intr.pNodes[i]->pNextNode;									
								lcd->Intr.pNodes[i] = lcd->Intr.pNodes[0] + i;
							}

							

						}	
			
					}
					else if(lcd->Intrf.TickMinus && !lcd->Intrf.LCDEntr ){


						node -= incr;
						_LineID -= incr;
						
						do{							

							if(!node)
								error = 1;
							
							else if(node->pHeadNode != lcd->Intr.pNodes[lcd->LineID]->pHeadNode)
								error  = 1;
							
							else if(node->Type != N_STAT)
								done = 1;


							if(error)
								done = 1;

							else if(!done){

								node -= incr;
								_LineID -= incr;
							}
							else{


								_LineID = _LineID % lcd->lineNum;
								
								if(_LineID < 0)
									_LineID = 0;
								
								lcd->LineID = _LineID;
													

							}

						}while(!done);	
									

						if(!error){		
							
							lcd->Intr.pNodes[0] = node - lcd->LineID ;
								
							for(i = 1  ; i < lcd->lineNum ; i++){	
								//lcd->Intr.pNodes[i] = lcd->Intr.pNodes[i]->pNextNode;									
								lcd->Intr.pNodes[i] = lcd->Intr.pNodes[0] + i;
							}

						

						}	

						
					}	

					
				break;	


			}



			if(!lcd->Intr.pNodes[lcd->LineID])
				done =1;

			if(!done){
				
				done = ((lcd->Intr.pNodes[lcd->LineID]->Type != N_STAT) || (line > lcd->lineNum)); 
			}	

			line ++;

			if(done && lcd->Intr.pNodes[lcd->LineID]->Type == N_STAT){
				lcd->LineID = _LineID;

			}	
				
			
								

		break;
			


		default:

		break;	

	}

	if(lcd->LineID < 0) 
		lcd->LineID = 0;
	else if(lcd->LineID >= lcd->lineNum)
		lcd->LineID = (lcd->lineNum - 1);

	/*
	lcd->Intrf.TickPlus = 0;	
	lcd->Intrf.TickMinus = 0;
	lcd->Intrf.LCDEntr_PosEdge = 0;
	lcd->Intrf.LCDEntr_NegEdge = 0;
	*/
	lcd->xCMD = IDLE_CMD;	


	/** Blinking **/
	lcd->Intr._slowBlink += lcd->Intr.tasktime;
	lcd->Intr._fastBlink += lcd->Intr.tasktime;

	if(lcd->Intr._slowBlink > 1.0){
		lcd->Intr.SlowBlink = 0;
		lcd->Intr._slowBlink = 0;
	}
	else if(lcd->Intr._slowBlink > 0.5)
		lcd->Intr.SlowBlink = 1;


	if(lcd->Intr._fastBlink > 0.6){
		lcd->Intr.FastBlink = 0;
		lcd->Intr._fastBlink = 0;
	}
	else if(lcd->Intr._fastBlink > 0.3)
		lcd->Intr.FastBlink = 1;	


	

	////////////////////////////////////////////////////

	if(lcd->Intr.pNodes)
		lcd->curNodeID = lcd->Intr.pNodes[lcd->LineID]->ID;

		
	head = (LCD_NODE_obj*)lcd->Intr.pNodes[0]->pHeadNode;

	if(head)		
		lcd->curHeadID = head->HeadID;

	////////////////////////////////////////////////////////
		
	

	DisplayPage (lcd);	
	SerialPortCyclic(lcd->pDevice);
	//TimerExecute(&lcd->MsgTmr);
	//TimerExecute(&lcd->WarnTmr);

	lcd->IntrfCmdDisable = 0;	
	
return 1;	
}



void IncrKeyPad(LCD_MGR_obj* lcd , char *p, INT incr ){

char s;
UINT len;


	if(lcd->EntrBtnPressReq){
		if(!lcd->Intrf.LCDEntr)
			return;
	}
	
	
	len = strlen(p);

	if (len == 0)
		return;
	
	if(lcd->Intr.keyPos >= len)
		lcd->Intr.keyPos = len - 1;
		

	s = *(p + lcd->Intr.keyPos);

	if(incr> 0){
		s++;

		if((s > '9') && (s < 'A'))
			s = 'A';

			
		else if((s > 'Z') && (s < '_'))
				s = '_';

		
		else if((s > '_') && (s < 'a'))
					s = 'a';

		else if(s > 'z')
			s = '0';

		
	}
	else if(incr < 0){
		s--;

		if(s < '0')
			s = 'z';
		
		else if((s > '9') && (s < 'A'))
			s = '9';

		else if((s > '_') && (s < 'a'))
					s = '_';
		
		else if((s > 'Z') && (s < '_'))
				s = 'Z';

		
	}	
	*(p + lcd->Intr.keyPos) = s;
	

}

void IncrParamNew(LCD_MGR_obj* lcd , Scaled_Var_obj* var, INT incr , BOOL _enum , BOOL overlap){
int i;
Cfg_typ *cfg;
CurCfg_typ *curCfg;
	
	
	if(!lcd || !var)
		return;

	
	if(lcd->EntrBtnPressReq){
		if(!lcd->Intrf.LCDEntr)
			return;
	}

	if(var->segm > SEGM_NUM_MAX)
		return;
	
	cfg = (Cfg_typ*) lcd->pMaster->pCfg;
	curCfg = (CurCfg_typ *)lcd->pMaster->pCurCfg;


	if(!cfg || ! curCfg )
		return;

	if((var->Indx >= lcd->pMaster->varLastNum)) //parLAST_PARAM
		return;
	
	else if(var->Indx > lcd->pMaster->paramLastNum){

		lcd->pMaster->pScaledUnitIndx[var->Indx]+= incr;
		
		if(lcd->pMaster->pScaledUnitIndx[var->Indx] < 0)
			lcd->pMaster->pScaledUnitIndx[var->Indx] = 0;

		if(lcd->pMaster->pScaledUnitIndx[var->Indx] >= SUB_UNIT_NUM)
			lcd->pMaster->pScaledUnitIndx[var->Indx] = SUB_UNIT_NUM - 1;

		return;

	}



	switch(lcd->Intr.WhatToEdit){


		case EDIT_VAL: // Value

			if(_enum)
				curCfg->Param[var->Indx].val[var->segm] +=  incr;
			else {
				
				curCfg->Param[var->Indx].val[var->segm] += curCfg->Param[var->Indx].incr * incr;

				// This is for discussion
				for(i = var->segm ; i <= SEGM_NUM_MAX ; i++)
					curCfg->Param[var->Indx].val[i] = curCfg->Param[var->Indx].val[var->segm];

			}	


			if(curCfg->Param[var->Indx].min < cfg->Param[var->Indx].min)
				curCfg->Param[var->Indx].min < cfg->Param[var->Indx].min;

			if(curCfg->Param[var->Indx].max > cfg->Param[var->Indx].max)
				curCfg->Param[var->Indx].max < cfg->Param[var->Indx].max;

			if(overlap){

				if(curCfg->Param[var->Indx].val[var->segm] < curCfg->Param[var->Indx].min)
					curCfg->Param[var->Indx].val[var->segm] = curCfg->Param[var->Indx].max;
				
				if(curCfg->Param[var->Indx].val[var->segm] > curCfg->Param[var->Indx].max)
					curCfg->Param[var->Indx].val[var->segm] = curCfg->Param[var->Indx].min;


			}else {

				if(curCfg->Param[var->Indx].val[var->segm] < curCfg->Param[var->Indx].min)
					curCfg->Param[var->Indx].val[var->segm] = curCfg->Param[var->Indx].min;

				if(curCfg->Param[var->Indx].val[var->segm] > curCfg->Param[var->Indx].max)
					curCfg->Param[var->Indx].val[var->segm] = curCfg->Param[var->Indx].max;	
			}


		break;
		
		case EDIT_UNIT: //Units	
		
			lcd->pMaster->pScaledUnitIndx[var->Indx]+= incr;
			
			if(lcd->pMaster->pScaledUnitIndx[var->Indx] < 0)
				lcd->pMaster->pScaledUnitIndx[var->Indx] = 0;

			if(lcd->pMaster->pScaledUnitIndx[var->Indx] >= SUB_UNIT_NUM)
				lcd->pMaster->pScaledUnitIndx[var->Indx] = SUB_UNIT_NUM - 1;

			
				
		break;

		case EDIT_MIN:// Min

			curCfg->Param[var->Indx].min += curCfg->Param[var->Indx].incr * incr;

			
			if(curCfg->Param[var->Indx].min < cfg->Param[var->Indx].min)
				curCfg->Param[var->Indx].min =	cfg->Param[var->Indx].min;								

			else if(curCfg->Param[var->Indx].min > curCfg->Param[var->Indx].max)
				curCfg->Param[var->Indx].min = curCfg->Param[var->Indx].max;

			for(i = 0 ; i < 8 ; i++){

				if(curCfg->Param[var->Indx].val[i] < curCfg->Param[var->Indx].min)
					curCfg->Param[var->Indx].val[i] = curCfg->Param[var->Indx].min;

			}

		break;


		case EDIT_MAX: // Max
		
			curCfg->Param[var->Indx].max += curCfg->Param[var->Indx].incr * incr;

			if(curCfg->Param[var->Indx].max > cfg->Param[var->Indx].max)
				curCfg->Param[var->Indx].max = cfg->Param[var->Indx].max;

			else if(curCfg->Param[var->Indx].max < curCfg->Param[var->Indx].min)
				curCfg->Param[var->Indx].max = curCfg->Param[var->Indx].min;

			for(i = 0 ; i < 8 ; i++){

				if(curCfg->Param[var->Indx].val[i] > curCfg->Param[var->Indx].max)
					curCfg->Param[var->Indx].val[i] = curCfg->Param[var->Indx].max;

			}

		break;	

		case EDIT_INCR: // Incr

			if(incr > 0)	
				//curCfg->Param[var->Indx].incr += 1/ powf(10 ,var->precision);
				curCfg->Param[var->Indx].incr += cfg->Param[var->Indx].incrMin;
			else if(incr < 0)
				curCfg->Param[var->Indx].incr -= cfg->Param[var->Indx].incrMin;
				//curCfg->Param[var->Indx].incr -= 1/ powf(10 ,var->precision);

			if(curCfg->Param[var->Indx].incr < cfg->Param[var->Indx].incrMin)
				curCfg->Param[var->Indx].incr = cfg->Param[var->Indx].incrMin;
				//curCfg->Param[var->Indx].incr = 1/ powf(10 ,var->precision);	

		break;	
	}
			
	
}

char* GetKeyPad(LCD_MGR_obj* mgr , char *p , UINT type , USINT blink){
UINT len , i;
static char ret[64];
UINT pos;

	 //blink=0	-> full text
	 //blink=1	-> space in keypad position
	 //blink=2	-> '*' everyware beside keypad position 

	 len = strlen(p);

	if(len == 0){

		//strcpy(ret , "N/A");
		
		return p;

	}
	 
	else if(len < 21)
		strcpy(ret , p);
	
	else{

		strcpy(ret , "Too Long");

		return ret;


	}

	pos = mgr->Intr.keyPos;

	if(	pos >= len)
		pos = len -1;


	//if(type & T_STRING_EDIT_HIDDEN){
	if(type & T_ATR_TXT_HIDDEN){
		for(i= 0 ; i < len ; i++){

			if(blink & 0x02){ // Show hidden letter
				if(i != pos)
					*(ret+i)='*';
			}
			else				
				*(ret+i)='*';
			
		}

	}
			
	if(blink & 0x01)
		*(ret + pos) = ' ';


	return ret;
		
	

}


char* GetScaledVal(LCD_MGR_obj* lcd , Scaled_Var_obj* var , USINT blink){

//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;


	if(!var || !lcd)
			return "N/A1";	
		
	//var = (Scaled_Var_obj*)pVal;

	//if(var->Indx >= lcd->pMaster->varLastNum)
		//return "N/A2";

	//if((var->Indx >= lcd->pMaster->varLastNum))
		//return "N/A3";


	if(!lcd->pMaster->pUnits)
		return "N/A4";

	if(var->segm > lcd->pMaster->numZone)
		return "Wrong Segm";

	
	
	if(var->pfVal)		
		//val = (*(var->pfVal+ var->segm));
		val = (*(var->pfVal));
	
	else if(var->Indx < lcd->pMaster->paramLastNum)
		
		val = lcd->pMaster->pCurCfg->Param[var->Indx].val[var->segm];
	
	else
		return "Wrong Var";
	
	units = (Units_obj*)lcd->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[lcd->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;
 

	if(blink & 0x01){

		strcpy(blankField , "                     ");
		*(blankField + strlen(dat))= 0;

		
		if(var->segm)			
			xsprintf(ret,"%d: %s %s", var->segm , blankField , scale->txt );
		
		else		
			xsprintf(ret,"%s %s", blankField , scale->txt );


	}
	else{
	
		if(var->segm)
			xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
		else
			xsprintf(ret,"%s %s", dat , scale->txt );
	}
		
	return ret;

}

char* GetScaledMinMax(LCD_MGR_obj* lcd , Scaled_Var_obj* var  ){
char *p1 , *p2;
float val , gain , offset , min , max;
Unit_Scale_obj *scale;
Units_obj *units;
static char ret[64];


	if(!var || !lcd)
		return "N/A5";
	

	if((var->Indx >= lcd->pMaster->paramLastNum))
		return "N/A6";

		
	if(!lcd->pMaster->pUnits)
		return "N/A7";

	units = (Units_obj*)lcd->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[lcd->pMaster->pScaledUnitIndx[var->Indx]];

	min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
	max = lcd->pMaster->pCurCfg->Param[var->Indx].max;

	min = min * scale->gain + scale->offset;
	max = max * scale->gain + scale->offset;

	p1 = xfdtoa((double)min , var->precision + scale->extraDigits);
	strcpy(ret , p1);

	/*
	if(p1[0]== '+')
		p1++; */
	
	p2 = xfdtoa((double)max , var->precision + scale->extraDigits);

	/*
	if(p2[0]== '+')
		p2++; */
	
	xsprintf(ret + strlen(ret)," / %s",  p2);
		
	return ret;

}

void DisplayPage(LCD_MGR_obj* lcd){
int i , k ;
DINT *pDINT;
SerialPortDriver_typ *rs;
Cfg_typ *cfg;
CurCfg_typ *curCfg;

float *pFloat;
Scaled_Var_obj *var;
Enum_Var_obj *enumVar;
String_Var_obj *strVar;
char* pStr;
Integer_Var_obj *intVar;

char *p;

	if(!lcd)
		return;

	if(!lcd->pDevice)
		return;

	rs = (SerialPortDriver_typ*)lcd->pDevice; 

	cfg = lcd->pMaster->pCfg;

	curCfg = lcd->pMaster->pCurCfg;

	if(!cfg || !curCfg)
		return;

//#if 0

	// Param Edit Page

	
	if(lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT){ 
		

		switch(lcd->Intr.pNodes[lcd->LineID]->var.Type & 0xFF){

			case T_SCALED:

				Build_N_Edit_SCALED(lcd);

			break;	


		}


	}

	// display as regular page if Node is not N_EDIT or N_EDIT but not during editing
	if( !(lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT)
		|| ((lcd->Intr.pNodes[lcd->LineID]->Type & N_EDIT)&& !lcd->Intr.editStep)) {

		for(i =0 ; i < lcd->lineNum ; i++){

			if(lcd->Intr.pNodes[i]){
				
				if(lcd->Intr.pNodes[i]->pHeadNode == lcd->Intr.pNodes[0]->pHeadNode){


					// Select Type of Node //
					
					if(lcd->Intr.pNodes[i]->Type & N_NO_PREFIX)
						strcpy(lcd->Intr.LineTXT[i] , "");

					else{
						
						if(lcd->LineID == i){

							switch(lcd->Intr.pNodes[i]->Type & 0x0F  ){

								case N_STAT:

									strcpy(lcd->Intr.LineTXT[i] , "");

								break;	


								case N_EDIT:

									if(lcd->Intrf.LCDEntr && lcd->Intr.SlowBlink 
										&& !(lcd->Intr.pNodes[i]->Type & N_EDIT_DIS) && (lcd->UserLevel >= lcd->Intr.pNodes[i]->MinUserLevel) )
										strcpy(lcd->Intr.LineTXT[i] , "  ");
									else{
			
										if(lcd->UserLevel >= lcd->Intr.pNodes[i]->MinUserLevel)
											strcpy(lcd->Intr.LineTXT[i] , "= ");
										else
											strcpy(lcd->Intr.LineTXT[i] , "x ");

									}	


								break;	


								case N_JUMP:

									if(lcd->UserLevel >= lcd->Intr.pNodes[i]->MinUserLevel)
										strcpy(lcd->Intr.LineTXT[i] , "> ");
									else
										strcpy(lcd->Intr.LineTXT[i] , "x ");

								break;	

								 
								case N_SCROLL:

									strcpy(lcd->Intr.LineTXT[i] , "* ");

								break;	

								default:
								
									strcpy(lcd->Intr.LineTXT[i] , "  ");
									
								break;	


							}

							// Extra atrubutes //////////////////
							if(lcd->Intr.pNodes[i]->Type & N_USER_EDIT){

								if(lcd->UserLevel >= lcd->Intr.pNodes[i]->MinUserLevel)
									strcpy(lcd->Intr.LineTXT[i] , "= ");
								else
									strcpy(lcd->Intr.LineTXT[i] , "x ");

							}
							//////////////////////////	
							
						}			
						else if(lcd->Intr.pNodes[i]->Type == N_STAT)
							strcpy(lcd->Intr.LineTXT[i] , "");
						else
							strcpy(lcd->Intr.LineTXT[i] , "  ");
					}
					
					/////////////////////////

					//  Select Type of Variable


					lcd->KK_test[i] = lcd->Intr.pNodes[lcd->LineID]->var.Type;


					switch(lcd->Intr.pNodes[i]->var.Type & 0xFF){


							case T_SCALED:

								if(lcd->Intr.pNodes[i]->var.Var1ID < lcd->pMaster->varLastNum)								
									xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , GetScaledVal(lcd ,&cfg->ScaledVar[lcd->Intr.pNodes[i]->var.Var1ID] , 0 ));
								

								
							break;

							case T_ENUM:

								if(lcd->Intr.pNodes[i]->var.Var1ID >= lcd->pMaster->varEnumLastNum)	
									break;
										
								enumVar = &cfg->EnumVar[lcd->Intr.pNodes[i]->var.Var1ID];
							

								 if(enumVar->pVal) // by pointer
									pDINT = (DINT*) enumVar->pVal;	

								 else if(enumVar->Indx < lcd->pMaster->paramEnumLastNum) // in CurCfg area
									pDINT = &curCfg->Param_ENUM[enumVar->Indx].val[0];
								
								else
									break;

								if(enumVar->Indx < lcd->pMaster->varEnumLastNum) {// in CurCfg area

											
									if((*pDINT)< lcd->pMaster->pCfg->Param_ENUM[enumVar->Indx].min)
										(*pDINT) = lcd->pMaster->pCfg->Param_ENUM[enumVar->Indx].min;
									
									else if((*pDINT)> lcd->pMaster->pCfg->Param_ENUM[enumVar->Indx].max)
										(*pDINT) = lcd->pMaster->pCfg->Param_ENUM[enumVar->Indx].max;

								}
								else{
									if((*pDINT)< enumVar->Min)
										(*pDINT) = enumVar->Min;
									
									else if((*pDINT)> enumVar->Max)
										(*pDINT) = enumVar->Max;

								}
								
								xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , cfg->TXT_Resources[enumVar->TxtIndx +(*pDINT)] );
								
								

							break;	

							case T_INTEGER:

								if(lcd->Intr.pNodes[i]->var.Var1ID >= lcd->pMaster->varIntLastNum)	
									break;

								intVar = &cfg->IntVar[lcd->Intr.pNodes[i]->var.Var1ID];
							

								 if(intVar->pVal) // by pointer
									pDINT = (DINT*) intVar->pVal;	

								 else if(intVar->Indx < lcd->pMaster->paramIntgLastNum) // in CurCfg area
									pDINT = &curCfg->Param_INT[intVar->Indx].val[0];
								
								else
									break;

								xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , *pDINT );


							break;	


							case T_STRING:

								if(lcd->Intr.pNodes[i]->var.Var1ID >= lcd->pMaster->varStrLastNum)	
									break;

								

								strVar = &cfg->StringVar[lcd->Intr.pNodes[i]->var.Var1ID]; 
								
							
								if(strVar->pStr) // by pointer
									pStr = strVar->pStr;
								
								else if(strVar->Indx < lcd->pMaster->paramStrLastNum) // in CurCfg area
									pStr = curCfg->Param_STR[strVar->Indx].str;
								
								else
									break;


								if(lcd->Intr.pNodes[i]->Type & N_EDIT){

						
									if((i != lcd->LineID) || !lcd->Intrf.LCDEntr)//show full text
										xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , GetKeyPad(lcd ,pStr , lcd->Intr.pNodes[i]->var.Type, 0x00));
																	
									
									else{

										
										if(lcd->Intr.FastBlink)
											xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , GetKeyPad(lcd ,pStr , lcd->Intr.pNodes[i]->var.Type , 0x01));
										else
											xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , GetKeyPad(lcd ,pStr , lcd->Intr.pNodes[i]->var.Type , 0x02));
								
																		
									}
								}
								else
									xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , lcd->Intr.pNodes[i]->var.Format , GetKeyPad(lcd ,pStr , lcd->Intr.pNodes[i]->var.Type, 0x00));
																	
							
							
								

							break;							

							case T_NATIVE: // NATIVE

								//psprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) ,  lcd->Intr.pNodes[i]->var.Format , lcd->Intr.pNodes[i]->var.pVal1 , lcd->Intr.pNodes[i]->var.pVal2 );
								psprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) ,  lcd->Intr.pNodes[i]->var.Format , 0 , 0);
									
							break;	

							default:

									xsprintf(lcd->Intr.LineTXT[i]+strlen(lcd->Intr.LineTXT[i]) , "dupa");
							break;	


					}	


					

					// Blink Attribute

					if(i == lcd->LineID){

						if(lcd->Intr.pNodes[i]->Type & N_EVENT){

							if(lcd->Intr.SlowBlink)
								memset(lcd->Intr.LineTXT[i] ,' ', 20);
						
						}

						else if((lcd->Intr.pNodes[i]->Type & N_JUMP)){

							if(lcd->Intr.SlowBlink)
								memset(lcd->Intr.LineTXT[i] ,' ', 20);
						}

					}

					
					
					if(lcd->Intr.pNodes[i]->var.Type & T_ATR_FAST_BLINK){

						if(lcd->Intr.SlowBlink)
							memset(lcd->Intr.LineTXT[i] ,' ', 20);

					
					}	
					else if(lcd->Intr.pNodes[i]->var.Type & T_ATR_BLINK){ 

						if(lcd->Intr.SlowBlink)
							memset(lcd->Intr.LineTXT[i] ,' ', 20);

					
					}	
					

					
				}
			else
				strcpy(lcd->Intr.LineTXT[i] , "");

		}
		}

	}

	//#endif
	switch(lcd->Intr.displStep){
		

		case 0:

			Send_LCD_Cmd(lcd,LCD_BLINK_CURSOR_OFF);
					
			lcd->Intr.displStep ++;			
			
		break;


		case 1:

			
			Send_LCD_Cmd(lcd,LCD_SET_CURSOR);
			
			lcd->Intr.displStep++;
		
			

		break;

	

		case 2:		


			//memcpy(rs->txData , LCD_CMD[LCD_SET_CURSOR].cmd , LCD_CMD[LCD_SET_CURSOR].len);

			p = rs->txData + LCD_CMD[LCD_SET_CURSOR].len;

			p = rs->txData;

			if(lcd->StaticMessageVisible){

				strcpy(p , lcd->Message[0]);
					LinePad(p  , 20);
					
				strcpy(p + 20 , lcd->Message[2]);
				LinePad(p + 20 , 20);
				
				strcpy(p + 40, lcd->Message[1]);
				LinePad(p + 40, 20);
					
				strcpy(p + 60 ,lcd->Message[3]);
				LinePad(p + 60 , 20);	

			}

			else if(lcd->MsgTmr.Q){
			
				strcpy(p ,lcd->Banner);
				LinePad(p  , 20);
				
				strcpy(p + 20 ,lcd->Intr.LineTXT[1]);
				LinePad(p + 20 , 20);
				
				strcpy(p + 40,lcd->Intr.LineTXT[0]);
				LinePad(p + 40, 20);
					
				strcpy(p + 60 ,lcd->Intr.LineTXT[2]);
				LinePad(p + 60 , 20);	

			}
			else{

				if(lcd->Intr.SlowBlink){

					strcpy(p , lcd->Message[0]);
					LinePad(p  , 20);
					
					strcpy(p + 20 , lcd->Message[2]);
					LinePad(p + 20 , 20);
					
					strcpy(p + 40, lcd->Message[1]);
					LinePad(p + 40, 20);
						
					strcpy(p + 60 ,lcd->Message[3]);
					LinePad(p + 60 , 20);	

				}
				else {
					
					strcpy(p , "");
					LinePad(p  , 20);
					
					strcpy(p + 20 ,"");
					LinePad(p + 20 , 20);
					
					strcpy(p + 40, "");
					LinePad(p + 40, 20);
						
					strcpy(p + 60 , "");
					LinePad(p + 60 , 20);	


				}	


			}

			rs->txDataLen = 80;				
			//rs->txBufRequest = 1;

		break;


		
		default:

			lcd->Intr.displStep = 0;

		break;	


	}

	
	
	
}

	
void Build_N_Edit_SCALED(LCD_MGR_obj* lcd)
{
int i , k;
Scaled_Var_obj *var;
char curPos = 0;
char tempBuf[281];
char *p;
float *pFloat;
		

		if(lcd->Intr.pNodes[lcd->LineID]->var.Var1ID >= lcd->pMaster->varLastNum)
			return;
			
		var = &lcd->pMaster->pCfg->ScaledVar[lcd->Intr.pNodes[lcd->LineID]->var.Var1ID];
		
		/******************************************************/
		// This Code Allow to switch beetween item selection and item edit 
		
		if(lcd->Intrf.LCDEntr_PosEdge ){

			// edit unit in read only scaled var
			if(var->type & VAR_READONLY){

				if(lcd->Intr.editStep == 0)
					lcd->Intr.editStep = 3;	// edit				
				else
					lcd->Intr.editStep = 4; // reset

				lcd->Intr.WhatToEdit = EDIT_UNIT;	

			}

			// go straight to edit value 
			else if((lcd->EditMode != EDIT_MODE_FULL) && (var->segm == 0)){  

				if(lcd->Intr.editStep == 0)
					lcd->Intr.editStep = 3;	// edit				
				else
					lcd->Intr.editStep = 4; // reset

				switch(lcd->EditMode){

					case EDIT_MODE_VALUE: 	lcd->Intr.WhatToEdit = EDIT_VAL; 	break;
					
					case EDIT_MODE_UNIT: 	lcd->Intr.WhatToEdit = EDIT_UNIT; 	break;
					
					case EDIT_MODE_MIN: 	lcd->Intr.WhatToEdit = EDIT_MIN; 	break;
					
					case EDIT_MODE_MAX: 	lcd->Intr.WhatToEdit = EDIT_MAX; 	break;
					
					case EDIT_MODE_INCR: 	lcd->Intr.WhatToEdit = EDIT_INCR; 	break;
				}

						
			}	
			// go straight to select array
			else if((lcd->EditMode == EDIT_MODE_VALUE) && (var->segm >0)
				&& (lcd->Intr.editStep == 0)){ // go straight to select array

				lcd->Intr.editStep = 2;
				lcd->Intr.WhatToEdit = EDIT_VAL;


			}
			// full edit
			else {
								
				if(lcd->Intr.editStep == 1){// skip value array if not value editing or not segmented
					if((lcd->Intr.WhatToEdit != EDIT_VAL) // skip value array if not value editing or not segmented
						|| (var->segm == 0))
						lcd->Intr.editStep++;	
				}
				
				lcd->Intr.editStep++;
				lcd->Intr.scrollDir = 0;

			}

		}

				
		/*************************************************/	

		
		switch(lcd->Intr.editStep){


			case 1: // select item: what to edit

							
				if(lcd->Intrf.TickPlus){
					lcd->Intrf.TickPlus = 0;
					
					lcd->Intr.WhatToEdit++;
					lcd->Intr.scrollDir = 1;

					
					if(lcd->Intr.WhatToEdit > EDIT_INCR)
						lcd->Intr.WhatToEdit = EDIT_INCR;
				}	
				else if(lcd->Intrf.TickMinus){
					lcd->Intrf.TickMinus = 0;

					lcd->Intr.WhatToEdit--;
					lcd->Intr.scrollDir = (-1);

					if(lcd->Intr.WhatToEdit < EDIT_VAL)
						lcd->Intr.WhatToEdit = EDIT_VAL;
				}
				
				Build_VarSetup_Item_Select_Page(lcd , var);

					
	
			break;	


			
			case 2:  // Select value from array

								
				if(lcd->Intrf.TickPlus){
					lcd->Intrf.TickPlus = 0;
					
					var->segm++;

					lcd->Intr.scrollDir = 1;

					if(var->segm > lcd->pMaster->numZone)
						var->segm = lcd->pMaster->numZone;
				}	
				else if(lcd->Intrf.TickMinus){
					lcd->Intrf.TickMinus = 0;

					var->segm--;
					lcd->Intr.scrollDir = (-1);

					if((var->segm < 1 )|| (var->segm > lcd->pMaster->numZone))
						var->segm = 1;
				}

									
				Build_Segment_Select_Page(lcd , var);

			break;


			case 3:  // edit item

				if(lcd->Intr.WhatToEdit == EDIT_VAL)
					Build_Val_Edit_Page(lcd , var);
				else
					Build_VarSetup_Edit_Page(lcd , var);

				if(lcd->EntrBtnPressReq){
					if(lcd->Intrf.LCDEntr_NegEdge) // return to begining
						lcd->Intr.editStep++;
				}
				
				
			break;	

			default:

				lcd->Intr.editStep = 0;
				lcd->Intr.WhatToEdit = 0;

			break;	

		}
				

		

		
}


void Build_Segment_Select_Page(LCD_MGR_obj* lcd, Scaled_Var_obj* var){
	
	char *p ;	
	float val , gain , offset;
	Unit_Scale_obj *scale;
	Units_obj *units;
	float *pVal;

	char dat[2][32];

	if(!var)
		return;	

	xsprintf(lcd->Intr.LineTXT[0], "  %s" , var->name);

	if(var->Indx >= lcd->pMaster->varLastNum){
		strcpy(lcd->Intr.LineTXT[1] , "N/B3");
		return ;
		
	}	

	if(!lcd->pMaster->pUnits)
	{
		strcpy(lcd->Intr.LineTXT[1] , "N/B4");
		return ;
	}	
	

	if(var->segm > SEGM_NUM_MAX){
		strcpy(lcd->Intr.LineTXT[1] , "Wrong Segm");
		return ;
	}	

	if(var->pfVal)		
		pVal = var->pfVal;
	
	else if(var->Indx < lcd->pMaster->paramLastNum)
		
		pVal = lcd->pMaster->pCurCfg->Param[var->Indx].val;
	
	else
		return "Wrong Var";

	
	units = (Units_obj*)lcd->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[lcd->pMaster->pScaledUnitIndx[var->Indx]];

	if(lcd->Intr.scrollDir > 0){

		if(var->segm > 0){

			
			val = (*(pVal+ var->segm -1));
			val = val * scale->gain + scale->offset;

			strcpy(dat[0] , xfdtoa((double)val , var->precision + scale->extraDigits));
			
			if(dat[0][0]== '+')
				dat[0][0] = ' ';

			val = (*(pVal+ var->segm));
			val = val * scale->gain + scale->offset;	
			
			strcpy(dat[1] , xfdtoa((double)val , var->precision + scale->extraDigits));

			if(dat[1][0]== '+')
				dat[1][0] = ' ';
			
			
			xsprintf(lcd->Intr.LineTXT[1], "  Sgm: %d:%s %s" , var->segm -1 , dat[0] , scale->txt);				
			
			if(lcd->Intr.FastBlink)
				xsprintf(lcd->Intr.LineTXT[2], "* Sgm: %d:%s %s" ,var->segm , dat[1] , scale->txt);
			else
				strcpy(lcd->Intr.LineTXT[2] , " ");

		}


	}
	else if(lcd->Intr.scrollDir <= 0){


		val = (*(pVal+ var->segm));
		val = val * scale->gain + scale->offset;
		
		strcpy(dat[0] , xfdtoa((double)val , var->precision + scale->extraDigits));

		if(dat[0][0]== '+')
			dat[0][0] = ' ';

		val = (*(pVal+ var->segm + 1));
		val = val * scale->gain + scale->offset;	
		
		strcpy(dat[1] , xfdtoa((double)val , var->precision + scale->extraDigits));

		if(dat[1][0]== '+')
			dat[1][0] = ' ';

		
		if(var->segm >= 0){
			
			if(lcd->Intr.FastBlink)
				xsprintf(lcd->Intr.LineTXT[1], "* Sgm: %d:%s %s" ,var->segm, dat[0] , scale->txt);
			else
				strcpy(lcd->Intr.LineTXT[1] , " ");	
			
			
			xsprintf(lcd->Intr.LineTXT[2], "  Sgm: %d:%s %s" ,var->segm+1, dat[1] , scale->txt);
			
		}		
		
	}
		
}
void Build_VarSetup_Item_Select_Page(LCD_MGR_obj* lcd, Scaled_Var_obj* var){
int i , k;
char curPos = 0;
char tempBuf[64];
char tempBuf1[64];
float *pFloat;
Unit_Scale_obj *scale;
Units_obj *units;
float  val ,gain , offset , min , max , incr;

	
	
	if(!var)
			return;

	if(var->Indx >= lcd->pMaster->varLastNum){
		strcpy(lcd->Intr.LineTXT[0] , "N/C2");
		return ;	

	}
	

	xsprintf(lcd->Intr.LineTXT[0], "  %s" , var->name);

	if((var->Indx >= lcd->pMaster->varLastNum)){
		strcpy(lcd->Intr.LineTXT[1] , "N/C3");
		return ;
		
	}	

	if(!lcd->pMaster->pUnits)
	{
		strcpy(lcd->Intr.LineTXT[1] , "N/C4");
		return ;
	}	
	

	if(var->segm > SEGM_NUM_MAX){
		strcpy(lcd->Intr.LineTXT[1] , "Wrong Segm");
		return ;
	}	
	

	units = (Units_obj*)lcd->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[lcd->pMaster->pScaledUnitIndx[var->Indx]];

	
		
	if(lcd->Intr.scrollDir > 0){


			
			switch(lcd->Intr.WhatToEdit){


				case EDIT_UNIT:

						
	
					if(var->Indx < lcd->pMaster->paramLastNum)
		
						val = lcd->pMaster->pCurCfg->Param[var->Indx].val[var->segm];
					
					else if(var->pfVal)
						val = (*(var->pfVal));
					else
						val = 0;
								
					
					val = val * scale->gain + scale->offset;	
					
					if(var->segm)
						xsprintf(lcd->Intr.LineTXT[1], "  Val: %d:%s %s" ,var->segm, xfdtoa((double)val , var->precision + scale->extraDigits) , scale->txt);
					else
						xsprintf(lcd->Intr.LineTXT[1], "  Val:  %s %s" , xfdtoa((double)val , var->precision + scale->extraDigits) , scale->txt);
				

					if(lcd->Intr.FastBlink)
						xsprintf(lcd->Intr.LineTXT[2], "* Unit: %s" , scale->txt);
					else
						strcpy(lcd->Intr.LineTXT[2] , " ");
					
				break;


				
				case EDIT_MIN:

					xsprintf(lcd->Intr.LineTXT[1], "  Unit: %s" , scale->txt);	

					if(lcd->Intr.FastBlink){
						min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
						min = min * scale->gain + scale->offset;
						xsprintf(lcd->Intr.LineTXT[2], "* Min:  %s %s" , xfdtoa((double)min , var->precision + scale->extraDigits) ,  scale->txt);
					}	
					else
						strcpy(lcd->Intr.LineTXT[2] , " ");
					
				break;

				case EDIT_MAX:

					
					min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
					min = min * scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[1], "  Min:  %s %s" , xfdtoa((double)min , var->precision + scale->extraDigits) , scale->txt);
					
					if(lcd->Intr.FastBlink){
						max = lcd->pMaster->pCurCfg->Param[var->Indx].max;
						max = max * scale->gain + scale->offset;
						xsprintf(lcd->Intr.LineTXT[2], "* Max:  %s %s" , xfdtoa((double)max , var->precision + scale->extraDigits), scale->txt);
					}
					else
						strcpy(lcd->Intr.LineTXT[2] , " ");
					
				break;

				case EDIT_INCR:

					max = lcd->pMaster->pCurCfg->Param[var->Indx].max;
					max = max * scale->gain + scale->offset;					
					xsprintf(lcd->Intr.LineTXT[1], "  Max:  %s %s" , xfdtoa((double)max , var->precision + scale->extraDigits), scale->txt);
					
					if(lcd->Intr.FastBlink){
						incr = lcd->pMaster->pCurCfg->Param[var->Indx].incr;
						incr = incr * scale->gain + scale->offset;
						xsprintf(lcd->Intr.LineTXT[2], "* Incr: %s %s" , xfdtoa((double)incr , var->precision + scale->extraDigits), scale->txt);
					}
					else
						strcpy(lcd->Intr.LineTXT[2] , " ");
					
					
				break;


				default:
				
					xsprintf(lcd->Intr.LineTXT[1], "Wrong selection");	
					xsprintf(lcd->Intr.LineTXT[2], " " );
					
				break;	




			}	
			


	}
	else if(lcd->Intr.scrollDir <= 0){


		switch(lcd->Intr.WhatToEdit){


				case EDIT_VAL:

					if(lcd->Intr.FastBlink){

						if(var->Indx < lcd->pMaster->paramLastNum)
		
							val = lcd->pMaster->pCurCfg->Param[var->Indx].val[var->segm];
					
						else if(var->pfVal)
							val = (*(var->pfVal));
						else
							val = 0;

						
						val = val * scale->gain + scale->offset;	
						
						if(var->segm)
							xsprintf(lcd->Intr.LineTXT[1], "* Val: %d:%s %s" ,var->segm, xfdtoa((double)val , var->precision + scale->extraDigits) , scale->txt);
						else
							xsprintf(lcd->Intr.LineTXT[1], "* Val:  %s %s" , xfdtoa((double)val , var->precision + scale->extraDigits) , scale->txt);
					}
					else
						strcpy(lcd->Intr.LineTXT[1] , " ");
					
					xsprintf(lcd->Intr.LineTXT[2], "  Unit: %s" , scale->txt);
					
				break;


				case EDIT_UNIT:

					if(lcd->Intr.FastBlink)
						xsprintf(lcd->Intr.LineTXT[1], "* Unit: %s" , scale->txt);
					else
						strcpy(lcd->Intr.LineTXT[1] , " ");
					
					min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
					min = min * scale->gain + scale->offset;	
					xsprintf(lcd->Intr.LineTXT[2], "  Min:  %s %s" , xfdtoa((double)min , var->precision + scale->extraDigits) , scale->txt);
					
				break;

				case EDIT_MIN:

					min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
					min = min * scale->gain + scale->offset;
	
					if(lcd->Intr.FastBlink)	
						xsprintf(lcd->Intr.LineTXT[1], "* Min:  %s %s" , xfdtoa((double)min , var->precision + scale->extraDigits) , scale->txt);
					else
						strcpy(lcd->Intr.LineTXT[1] , " ");

					max = lcd->pMaster->pCurCfg->Param[var->Indx].max;
					max = max * scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[2], "  Max:  %s %s" , xfdtoa((double)max , var->precision + scale->extraDigits) , scale->txt);
					
				break;

				case EDIT_MAX:

					max = lcd->pMaster->pCurCfg->Param[var->Indx].max;
					max = max * scale->gain + scale->offset;
					
					if(lcd->Intr.FastBlink)
						xsprintf(lcd->Intr.LineTXT[1], "* Max:  %s %s" , xfdtoa((double)max , var->precision + scale->extraDigits) , scale->txt);	
					else
						strcpy(lcd->Intr.LineTXT[1] , " ");

					incr = lcd->pMaster->pCurCfg->Param[var->Indx].incr;
					incr = incr * scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[2], "  Incr: %s %s" , xfdtoa((double)incr , var->precision + scale->extraDigits) , scale->txt);
					
					
				break;

				
				default:
				
					xsprintf(lcd->Intr.LineTXT[1], "Wrong selection");	
					xsprintf(lcd->Intr.LineTXT[2], " " );
					
				break;					




			}	
			


		
		
			
	}

	
	
}

void Build_VarSetup_Edit_Page(LCD_MGR_obj* lcd, Scaled_Var_obj* var){
	int i , k;
	char curPos = 0;
	char tempBuf[64];
	char tempBuf1[64];
	//char *p , *pMin , *pMax ,*pIncr;
	float *pFloat;
	Unit_Scale_obj *scale;
	Units_obj *units;
	float  gain , offset , min , max , incr;

	
	
	if(!var)
		return;	

	xsprintf(lcd->Intr.LineTXT[0], "  %s" , var->name);

	if((var->Indx >= lcd->pMaster->varLastNum)){
		strcpy(lcd->Intr.LineTXT[1] , "N/C3");
		return ;
		
	}	
	

	if(!lcd->pMaster->pUnits)
	{
		strcpy(lcd->Intr.LineTXT[1] , "N/C4");
		return ;
	}	
	

	if(var->segm > SEGM_NUM_MAX){
		strcpy(lcd->Intr.LineTXT[1] , "Wrong Segm");
		return ;
	}	
	

	units = (Units_obj*)lcd->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[lcd->pMaster->pScaledUnitIndx[var->Indx]];


	strcpy(lcd->Intr.LineTXT[1] , " ");

	if(lcd->Intr.FastBlink){
		
		strcpy(lcd->Intr.LineTXT[2] , " ");

	}
	else{

		if(var->Indx > lcd->pMaster->varLastNum){

			if(lcd->pMaster->pScaledUnitIndx[var->Indx]== 0)
				strcpy(lcd->Intr.LineTXT[2] , "  Unit: internal unit");
			else			
				xsprintf(lcd->Intr.LineTXT[2], "  Unit: %s" , scale->txt);	

		}
		else {	

			switch(lcd->Intr.WhatToEdit){


				case EDIT_UNIT:

					xsprintf(lcd->Intr.LineTXT[2], "  Unit: %s" , scale->txt);	
					//xsprintf(lcd->Intr.LineTXT[2], "  Unit: %d" , lcd->pMaster->pScaledUnitIndx[var->Indx]);	
					
				break;					

				case EDIT_MIN:

					min = lcd->pMaster->pCurCfg->Param[var->Indx].min;
					min = min * scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[2], "  Min:  %s %s" , xfdtoa((double)min , var->precision + scale->extraDigits), scale->txt);
					
				break;

				case EDIT_MAX:

					max = lcd->pMaster->pCurCfg->Param[var->Indx].max;
					max = max* scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[2], "  Max:  %s %s" , xfdtoa((double)max , var->precision + scale->extraDigits), scale->txt);
					
				break;

				case EDIT_INCR:

					incr = lcd->pMaster->pCurCfg->Param[var->Indx].incr;
					incr = incr * scale->gain + scale->offset;
					xsprintf(lcd->Intr.LineTXT[2], "  Incr: %s %s" , xfdtoa((double)incr , var->precision + scale->extraDigits), scale->txt);
					
					
				break;


				default:
				
					xsprintf(lcd->Intr.LineTXT[1], "Wrong selection");	
					xsprintf(lcd->Intr.LineTXT[2], " " );
					
				break;	




			}	
		}
	}
	
}


void Build_Val_Edit_Page(LCD_MGR_obj* lcd , Scaled_Var_obj* var){
int i , k;
char curPos = 0;
char tempBuf[64];
char *p;
float *pFloat;

	
	xsprintf(lcd->Intr.LineTXT[0], "  %s" , var->name);

	if(lcd->Intr.FastBlink) 
		p =  GetScaledVal(lcd ,var , 1);
	else
		p =  GetScaledVal(lcd ,var , 0);
	
	memset(tempBuf , ' ' , 20);
	strcpy(tempBuf + ((20 - strlen(p))/2)  , p);			
	xsprintf(lcd->Intr.LineTXT[1] , "%s" , tempBuf);
	
	p = GetScaledMinMax(lcd ,var);
	memset(tempBuf , ' ' , 20); 				
	strcpy(tempBuf + ((20 - strlen(p))/2)  , p);
	xsprintf(lcd->Intr.LineTXT[2] , "%s" , tempBuf);
			
	

}

void LinePad(char* l , UINT len){
int i , k;

	k = strlen(l);

	if( k < len)		
		memset(l+k , ' ' , len -k);

return ;	

}

/********************************************/

void Send_LCD_Cmd(LCD_MGR_obj* mgr , UINT cmd  ){
SerialPortDriver_typ *rs;

	if(!mgr)
		return;

	if(!mgr->pDevice)
		return;
		
		
	rs = (SerialPortDriver_typ*)mgr->pDevice;

	memcpy(rs->txData , LCD_CMD[cmd].cmd , LCD_CMD[cmd].len);
	rs->txDataLen = LCD_CMD[cmd].len;
	rs->txBufRequest = 1;
	
	if(cmd == LCD_SET_CURSOR ||
		cmd == LCD_SET_BACKLIGHT|| 
		cmd == LCD_SET_BAUDRATE||
		cmd == LCD_SET_CURSOR)
		rs->txData[2] = mgr->cursPos;

	return;
	


}



/*
========================================================================================
*/
void SerialPortInit(SerialPortDriver_typ *p, char *deviceStr, char *baudRateStr ) {
	/* Init Values */ 
	p->initError = 0; 
	p->rxReady = 0;
	p->rxError = 0;  
	p->txError = 0;  

	
	/* Port Parameters */
	strcpy(p->sDevice, deviceStr);
	strcpy(p->sMode, baudRateStr);
	
	//strcpy(p->sMode, "/BD=");
	//strcat(p->sMode, baudRateStr);
	//strcat(p->sMode, " /DB=8 /SB=1 /CCHAR1=0x0a");	/*Device sends: <string><CR<LF>, CR=0xd, LF=0xa*/

	/* Initialize Port Structure */ 
	p->Open.config 	= (UDINT) 0; 
	p->Open.device 	= (UDINT) p->sDevice; 
	p->Open.mode 	= (UDINT) p->sMode; 
	p->Open.enable 	= 1; 
  
  	/* Open Port */ 
	FRM_xopen(&p->Open); 							
	
	p->StatusOpen 	= p->Open.status; 							
	p->Ident 		= p->Open.ident; 							
	
	/* Check Port Open Status */  
	if ( p->Open.status ){
		p->initError = 1;
		p->Ident	= 0;
		return;
	}
		
	/* Initialize Get Buffer 	*/ 
	p->GetBuffer.enable 		= 1; 
	p->GetBuffer.ident 		= p->Ident; 
	/* Initialize Write Buffer 	*/ 
	p->Write.enable 			= 1; 
	p->Write.ident 			= p->Ident; 
	/* Initialize Release Output Buffer */ 
	p->ReleaseOutput.enable 	= 1; 
	p->ReleaseOutput.ident 	= p->Ident;
	/* Initialize Read */ 
	p->Read.enable 			= 1; 
	p->Read.ident 				= p->Ident; 
	/* Initialize Release Buffer */ 
	p->Release.enable 			= 1; 
	p->Release.ident 			= p->Ident; 
	/* Initialize Mode */ 
	p->Mode.enable 				= 1; 
	p->Mode.ident 				= p->Ident; 

}


/*
========================================================================================
*/
void SerialPortClose(SerialPortDriver_typ *p) {

	/* open error? -> stop! cant' close of never opened*/
	if ( p->initError || !p->Ident)
		return;

	/* Initialize Close */ 
	p->Close.enable 			= 1; 
	p->Close.ident 				= p->Ident; 

	FRM_close(&p->Close);
	

	/* Init Values */ 
	p->initError	= 0; 
	p->rxReady 		= 0;
	p->rxError 		= 0;  
	p->txError 		= 0;  
	p->Ident 		= 0;	

}

/*
========================================================================================
*/
void SerialPortCyclic(SerialPortDriver_typ *p) {
unsigned long len;


/*TODO: error handling!*/			 
	/* open error? -> stop! cant' do much if driver not ready*/
	if ( p->initError || !p->Ident)
		return;

	
	/* Write Message: */	
	if ( p->txRequest || p->txBufRequest) {
		
		p->txError = 0;  
		 			
		/* Get Send Buffer */ 
		FRM_gbuf(&p->GetBuffer); 
		p->pSendBuffer = (UDINT *) p->GetBuffer.buffer; 	/* Get Address 	*/ 
		p->SendLength = p->GetBuffer.buflng; 				/* Get Length 	*/ 
		p->StatusGetBuffer = p->GetBuffer.status; 			/* Get Status 	*/ 

		 
		/* Check Status */  
		if ( p->GetBuffer.status ){
			p->txError = 1;  
			return;
		}
					
	 	/* Write Data to Send Buffer */
		if(p->txBufRequest)
			len = p->txDataLen;			
		else
			len = strlen(p->txData);	

		memcpy( p->pSendBuffer, p->txData, len ); 
		 
		/* Write Port: Get Status */ 
		p->Write.buffer = (UDINT) p->pSendBuffer; 
		p->Write.buflng = len;
		FRM_write(&p->Write); 				
		p->StatusWrite = p->Write.status; 	

		/* Check Status */  
		if( p->Write.status ) {
			
			/* Release Output Buffer */ 
			p->ReleaseOutput.buffer 		= (UDINT) p->pSendBuffer; 
			p->ReleaseOutput.buflng 		= p->SendLength; 
			FRM_robuf(&p->ReleaseOutput); 	
			p->StatusReleaseOutputBuffer 	= p->ReleaseOutput.status;			
			
			p->txError = 1;
		} 
		else{
			/* write ok; Message Sent; write buffer automaticaly released */
			p->txMessageCounter++;			
			p->txError = 0;
		}	
		
		/* Reset handshake flags*/
		p->txRequest  = 0; 
		p->txBufRequest = 0;
	} 

	
	/* Read Continuosly*/	 
	FRM_read(&p->Read); 		
	p->pReadBuffer = (UDINT *) p->Read.buffer; 		/* Get Address 	*/
	p->ReadLength = p->Read.buflng; 				/* Get Length 	*/ 
	p->StatusRead = p->Read.status; 				/* Get Status 	*/ 

	/* Check Status */   
	if ( !p->StatusRead ) {
		/* set handshake flag*/
		p->rxReady = 1; 
		p->rxError = 0;  
	 	memset( p->rxData, 0, sizeof(p->rxData) );
		/* Copy Read to Array */
		memcpy( p->rxData, (UDINT *) p->pReadBuffer, p->ReadLength);
		p->rxMessageCounter++;	
		memcpy( p->sInput, (UDINT *) p->pReadBuffer, p->ReadLength); 
		p->l = p->ReadLength;
		/* Release Read Buffer */
		p->Release.buffer 		= (UDINT) p->pReadBuffer; 
		p->Release.buflng 	= p->ReadLength; 
		FRM_rbuf(&p->Release); 						
		
	} 

	/*keep cheking for read errros; status=60->no data received*/
	if (p->Read.status && (p->Read.status != 60) ) {		
		p->rxError = 1;
	}
}



BOOL CheckCurCfg(MASTER_typ *m){
	
int i, k;
BOOL status;
Cfg_typ *cfg;
CurCfg_typ *curCfg;
Scaled_Var_obj *var , *var_i;
SegmentsTable_obj *segmTable;

	if(!m)
		return 0;

	if(!m->pCfg || !m->pCurCfg || !m->pScaledVar || !m->pSegmentTable )
		return 0;
	
	cfg = (Cfg_typ*) m->pCfg;
	curCfg = (CurCfg_typ*) m->pCurCfg;
	var = (Scaled_Var_obj*) m->pScaledVar;
	segmTable = (SegmentsTable_obj*) m->pSegmentTable;


	if(curCfg->Signature != 1957){
		LoadDefaultParam(m);
		//ApplyGenRulesToCurCfg(m);
			

	}	
	
	
	status = 1;

	// check units
	for (i = 0 ; i < m->varLastNum; i++){
		if(m->pScaledUnitIndx[i] >= SUB_UNIT_NUM)
			m->pScaledUnitIndx[i] = 1;

	}	
	
	for(i = 0 ; i < m->paramLastNum ; i++){

		for(k = 0 ; k < SEGM_NUM ; k++){
			
			if(curCfg->Param[i].val[k] < cfg->Param[i].min)
				curCfg->Param[i].val[k] = cfg->Param[i].min;
			
			if(curCfg->Param[i].val[k] > cfg->Param[i].max)
				curCfg->Param[i].val[k] = cfg->Param[i].max;

			if(curCfg->Param[i].min < cfg->Param[i].min)
				curCfg->Param[i].min = cfg->Param[i].min;
			
			if(curCfg->Param[i].max > cfg->Param[i].max)
				curCfg->Param[i].max = cfg->Param[i].max;

			
		}

	}	

	k = 0;
	for(i = 0 ; i < m->paramLastNum ; i++){


		// New parameters or fresh CF
		if(curCfg->Param[i].min == cfg->Param[i].max){
			curCfg->Param[i].min = cfg->Param[i].min;
			curCfg->Param[i].max = cfg->Param[i].max;
			curCfg->Param[i].val[0] = cfg->Param[i].defaultVal;
		}
		
		
		if(curCfg->Param[i].incr <= 0.0)
			curCfg->Param[i].incr = cfg->Param[i].incrMin;
		
		if(curCfg->Param[i].incr < cfg->Param[i].incrMin)
			curCfg->Param[i].incr = cfg->Param[i].incrMin;

		
		if(curCfg->Param[i].incr > cfg->Param[i].incrMax)
			curCfg->Param[i].incr = cfg->Param[i].incrMax;

		
		
		if(cfg->ScaledVar[i].segm == 0)
			curCfg->Param[i].segmYes = 0;
		else
			segmTable->ParamIndx[k++] = i;
		
			
		segmTable->segm_num = k;
		
		var_i = var+i;

		/* I do not know for what was that
		
		if(curCfg->Param[i].segmYes){
			
			var_i->segm = 1;

		}
		else{
			var_i = var+i;
			var_i->segm = 0;

		}
		*/
	

	}

	// Force limits from Cfg in curCfg for enumerated values

	for(i = 0 ; i < m->paramEnumLastNum; i++){

		curCfg->Param_ENUM[i].min = cfg->Param_ENUM[i].min;
		curCfg->Param_ENUM[i].max = cfg->Param_ENUM[i].max;

	}

	
		
	return status;
}

void LoadDefaultParam(MASTER_typ *m){
int i, k;
char status;
Cfg_typ *cfg;
CurCfg_typ *curCfg;

	if(!m)
		return;

	cfg = (Cfg_typ*) m->pCfg;
	curCfg = (Cfg_typ*) m->pCurCfg;

	
	if(!cfg || !curCfg)
		return;

	for (i = 0 ; i < m->paramLastNum; i++){

		//curCfg->Param[i].segmYes = cfg->Param[i].SegmType;
		if(cfg->ScaledVar[i].segm == 1)
			curCfg->Param[i].segmYes = 1;
		else
			curCfg->Param[i].segmYes = 0;
		
		curCfg->Param[i].val[0] = cfg->Param[i].defaultVal;
		
		for(k=1 ; k <= m->numZone; k++)
			curCfg->Param[i].val[k] = cfg->Param[i].defaultVal;
		
		curCfg->Param[i].min = cfg->Param[i].min;
		curCfg->Param[i].max = cfg->Param[i].max;

		curCfg->Param[i].incr = cfg->Param[i].incrMin;
	}	

	for (i = 0 ; i < m->paramEnumLastNum; i++){

		//curCfg->Param[i].segmYes = cfg->Param[i].SegmType;
		curCfg->Param_ENUM[i].segmYes = 0;
		
		curCfg->Param_ENUM[i].val[0] = cfg->Param_ENUM[i].defaultVal;
		curCfg->Param_ENUM[i].incr = 1;
		
		for(k=1 ; k <= m->numZone; k++)
			curCfg->Param_ENUM[i].val[k] = cfg->Param_ENUM[i].defaultVal;
		
		curCfg->Param_ENUM[i].min = cfg->Param_ENUM[i].min;
		curCfg->Param_ENUM[i].max = cfg->Param_ENUM[i].max;

		//curCfg->Param_ENUM[i].incr = cfg->Param_ENUM[i].incrMin;
	}	

	for (i = 0 ; i < m->paramIntgLastNum; i++){

		//curCfg->Param[i].segmYes = cfg->Param[i].SegmType;
		curCfg->Param_INT[i].segmYes = 0;
		
		curCfg->Param_INT[i].val[0] = cfg->Param_INT[i].defaultVal;

		curCfg->Param_INT[i].incr = 1.0;
		
		for(k=1 ; k <= m->numZone; k++)
			curCfg->Param_INT[i].val[k] = cfg->Param_INT[i].defaultVal;
		
		curCfg->Param_INT[i].min = cfg->Param_INT[i].min;
		curCfg->Param_INT[i].max = cfg->Param_INT[i].max;

		//curCfg->Param_ENUM[i].incr = cfg->Param_ENUM[i].incrMin;
	}	


	//for (i = 0 ; i < m->varLastNum ; i++)
		//m->pScaledUnitIndx[i] = 1;
	
	strcpy(curCfg->CurProgramName , "default");

	curCfg->Signature = 1957;
	
}

BOOL LoadDefaultCalib(MASTER_typ *m){
int i, k;	
CurCalib_typ *defCalib , *calib;

	
	if(!m)
		return 0;	
		
	defCalib =	(CurCalib_typ*) m->pDefaultCalib;
	calib = (CurCalib_typ*) m->pCalib;
	
	if((!defCalib) || (!calib))
		return 0;


	 for(i = 0 ; i < defCalib->num ; i++){

		for(k = 0 ; k < defCalib->Points[i].num ;k++)			
				calib->Points[i].val[k] =  defCalib->Points[i].val[k];

	  	calib->Points[i].num = defCalib->Points[i].num;
		
		
	}	
	
	calib->num = defCalib->num;

	
	return 1;

}


DINT getNodeTxt(LCD_MGR_obj *lcd , char* out ,LCD_NODE_obj* node){

int i , k ;
DINT *pDINT;
SerialPortDriver_typ *rs;
Cfg_typ *cfg;
CurCfg_typ *curCfg;

float *pFloat;
Scaled_Var_obj *var;
Enum_Var_obj *enumVar;
String_Var_obj *strVar;
char* pStr;
Integer_Var_obj *intVar;
char *p;



	if(!lcd || !out)
		return;

	cfg = lcd->pMaster->pCfg;

	curCfg = lcd->pMaster->pCurCfg;

	if(!cfg || !curCfg)
		return;	




	switch(node->var.Type & 0xFF){

	
		case T_SCALED:

			if(node->var.Var1ID < lcd->pMaster->varLastNum)								
				xsprintf(out , node->var.Format , GetScaledVal(lcd ,&cfg->ScaledVar[node->var.Var1ID] , 0 ));
			

			
		break;

		case T_ENUM:

			if(node->var.Var1ID >= lcd->pMaster->varEnumLastNum)	
				break;
					
			enumVar = &cfg->EnumVar[node->var.Var1ID];


			 if(enumVar->pVal) // by pointer
				pDINT = (DINT*) enumVar->pVal;	

			 else if(enumVar->Indx < lcd->pMaster->paramEnumLastNum) // in CurCfg area
				pDINT = &curCfg->Param_ENUM[enumVar->Indx].val[0];
			
			else
				break;

			if(enumVar->Indx < lcd->pMaster->varEnumLastNum) {// in CurCfg area

						
				if((*pDINT)< cfg->Param_ENUM[enumVar->Indx].min)
					(*pDINT) = cfg->Param_ENUM[enumVar->Indx].min;
				
				else if((*pDINT)> cfg->Param_ENUM[enumVar->Indx].max)
					(*pDINT) = cfg->Param_ENUM[enumVar->Indx].max;

			}
			else{
				if((*pDINT)< enumVar->Min)
					(*pDINT) = enumVar->Min;
				
				else if((*pDINT)> enumVar->Max)
					(*pDINT) = enumVar->Max;

			}
			
			xsprintf(out , node->var.Format , cfg->TXT_Resources[enumVar->TxtIndx +(*pDINT)] );
			
			

		break;	

		

		case T_INTEGER:

			if(node->var.Var1ID >= lcd->pMaster->varIntLastNum)	
				break;

			intVar = &cfg->IntVar[node->var.Var1ID];


			 if(intVar->pVal) // by pointer
				pDINT = (DINT*) intVar->pVal;	

			 else if(intVar->Indx < lcd->pMaster->paramIntgLastNum) // in CurCfg area
				pDINT = &curCfg->Param_INT[intVar->Indx].val[0];
			
			else
				break;

			xsprintf(out , node->var.Format , *pDINT );


		break;	


		case T_STRING:

			if(node->var.Var1ID >= lcd->pMaster->varStrLastNum)	
				break;

			

			strVar = &cfg->StringVar[node->var.Var1ID]; 
			

			if(strVar->pStr) // by pointer
				pStr = strVar->pStr;
			
			else if(strVar->Indx < lcd->pMaster->paramStrLastNum) // in CurCfg area
				pStr = curCfg->Param_STR[strVar->Indx].str;
			
			else
				break;


			xsprintf(out , node->var.Format , pStr);
												


			

		break;							

		case T_NATIVE: // NATIVE

			psprintf(out ,  node->var.Format , 0 , 0);
				
		break;	


		default:

				xsprintf(out , "dupa");
		break;	


		}

	
	return (DINT)strlen(out) ;
}

#endif

			
