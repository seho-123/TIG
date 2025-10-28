
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include<HMI_Reports.h>

_LOCAL UINT Page_Step;

_LOCAL TON_typ BtnDelay;

_LOCAL DATE_AND_TIME LocalTime;

_LOCAL UINT KK_Catch;
_LOCAL UINT HMI_ClearError_vis;
_LOCAL plcbit HMI_ClearError_btn;

//_LOCAL plcstring KK_str[32] , KK_format[32];
//_LOCAL plcbit KK_CheckString;
//_LOCAL float KK_Float;
_LOCAL SINT KK_Result;
//_LOCAL USINT KK_dzien;

_LOCAL UINT HMI_Lock_Line[16] , HMI_Line_vis[16] , HMI_Ack_Btn_vis[16];
_LOCAL UINT NewFilePage_vis , _NewFilePage_vis , SetTimePage_vis , _SetTimePage_vis;
_LOCAL UINT HMI_Message_Vis;
_LOCAL UINT HMI_Head_Lock;
_LOCAL UINT Ack_Btn[16] , Glb_Ack_Btn , Glb_Ack_Btn_vis;
_LOCAL DTStructure HMI_DTStr;
_LOCAL DTStructure HMI_SetDTStr;
_LOCAL UINT HMI_Encoder_vis;
_LOCAL UINT HMI_InetrpassTemp_upd;
_LOCAL UINT HMI_HeatInput_vis[16] , HMI_PwrRatio_vis[16] , HMI_HeatInputLimit_vis , HMI_PwrRatioLimit_vis;
_LOCAL UINT HMI_UpdateLine;

//_LOCAL UDINT DwellIn_dec, DwellOut_dec , ExcurTime_dec;
//_LOCAL float _DwellIn, _DwellOut , _ExcurTime;

//  New File Name Data
_LOCAL plcstring HMI_File_BeadLog[16];
_LOCAL plcstring HMI_File_Name[64];
_LOCAL UINT HMI_File_LayerNo , HMI_File_BeadNo;
_LOCAL USINT HMI_File_TempEvery;
_LOCAL float HMI_File_WireDim;
_LOCAL float HMI_PowerRatio;
_LOCAL float HMI_HeatInput;
_LOCAL USINT HMI_PowerRatio_status;
_LOCAL UINT HMI_PowerRatio_Color;


/////////////////////////////
// new in July 2024

_LOCAL UINT HMI_Options_Vis;

///////
_LOCAL UINT GotoScreen , ReadPage , SetPage;

_LOCAL float _PowerRatio , KK_PowerRatio;
_LOCAL UINT LastBad_BEADS_Cnt;

_LOCAL DTSetTime_typ DTSetTime_1;

_LOCAL int _Len[8];

_LOCAL char LongBuf[512];
_LOCAL UDINT LongBufSize;

_LOCAL USINT TempEveryBead_Flag;

_LOCAL plcbit ItWasTest;
_LOCAL plcbit NewBeat;


UINT _seqStep;

UINT _SeqStep;
UINT _weld_Report_Status;


void  MenuBar();
char* CreateHeaderTxt(char *p);
char* CreateLineTxt(char *p);

char* CreateHeaderTxt_01(char *p);
char* CreateLineTxt_01(char *p);


char* CreateHeaderTxt_2(char *p);
char* CreateLineTxt_2(char *p);


SINT IsItNumber(char*p);

float ActTemp();


void _INIT ProgramInit(void)
{
float f;

	//strcpy(KK_str , "80.2");

	strcpy(File_RW2.fileExten , ".txt");
	strcpy(File_RW2.deviceName, "REPORTS");

	TimerStart(&BtnDelay, 4.0);

	//strcpy(KK_format ,"%2d");

	NewFilePage_vis = INVISIBLE;
	SetTimePage_vis = INVISIBLE;

	HMI_Message_Vis = INVISIBLE;

	HMI_Encoder_vis = INVISIBLE;

	HMI_Options_Vis = INVISIBLE;


	_SetTimePage_vis = SetTimePage_vis;

	HMI_PowerRatio_Color = 1;

	TempEveryBead_Flag = 1;

	NewBeat = 1;


	if(ActTemp() > 340)
		Weld_Report_Status = 4; // Over temprature
	else
		Weld_Report_Status = 3; // Must enter temperature
		
	LastBad_BEADS_Cnt = Weld_Rep.BEADS_Cnt;

	if(CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_LO] < 10.0)
		CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_LO] = 10.0;

	if(CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_HI] > 200.0)
		CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_HI] = 200.0;
	
}

void _CYCLIC ProgramCyclic(void)
{	
UINT i;
UINT status;
DTGetTime_typ dtGet;
float w1, w2 , f;

dtGet.enable = 1;
DTGetTime(&dtGet);
LocalTime = dtGet.DT1;

//Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp = dtGet.DT1;


/*

if(KK_CheckString){
	KK_CheckString = 0;

	KK_Result = IsItNumber(KK_str);
	xsprintf(KK_str , KK_format, KK_dzien);
	
}



if(OSC_TML_Param_Set[0].pDwell_In 
	&& OSC_TML_Param_Set[0].pDwell_Out
	&& OSC_TML_Param_Set[0].pFreq){

	_DwellIn = (*OSC_TML_Param_Set[0].pDwell_In);
	_DwellOut = (*OSC_TML_Param_Set[0].pDwell_Out);
	_ExcurTime = (*OSC_TML_Param_Set[0].pFreq);

	DwellIn_dec = (UDINT)(_DwellIn * 10.0 +0.5);
	DwellOut_dec = (UDINT)(_DwellOut * 10.0 +0.5);
	ExcurTime_dec = (UDINT)(_ExcurTime * 10.0 +0.5);
	
}

*/

if((Weld_Rep.Report_Type == 0)){ // HEAT INPUT

		HMI_HeatInputLimit_vis = VISIBLE;
		HMI_PwrRatioLimit_vis = INVISIBLE;	

	for(i = 0 ; i <16 ; i++){
		HMI_HeatInput_vis[i] =  HMI_Line_vis[i];
		HMI_PwrRatio_vis[i] = INVISIBLE;
	}

}	
else if((Weld_Rep.Report_Type == 1)){ // Power Ratio
	 
		HMI_PwrRatioLimit_vis = VISIBLE;
		HMI_HeatInputLimit_vis = INVISIBLE;

	for(i = 0 ; i <16 ; i++){
			HMI_PwrRatio_vis[i] =	HMI_Line_vis[i];
			HMI_HeatInput_vis[i] = INVISIBLE;
		}
}
else{

	HMI_HeatInputLimit_vis = INVISIBLE;
	HMI_PwrRatioLimit_vis = INVISIBLE;	

	for(i = 0 ; i <16 ; i++){
		HMI_HeatInput_vis[i] = INVISIBLE;
		HMI_PwrRatio_vis[i] = INVISIBLE;
	}
}

////////////////////////////////////////////////////
//if(_weld_Report_Status != Weld_Report_Status){
if(1){
	/* Tak bylo do 28 Jan 2025
	if(Weld_Report_Status == 0) // Start welding
		Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart = dtGet.DT1;

	else if(Weld_Report_Status == 1) // Stop welding
		Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp = dtGet.DT1;
	*/	
	if(CurCalib.GenSetup[SETUP_LOG_OPTION] == 3){ // Log any Start and Stop
	
		
		//if(!_seqStep && Master[0].SeqStep && NewBeat){
		//if(!Master[0].SeqStep && NewBeat){
		if((Weld_Report_Status == 0) && NewBeat){
			//NewBeat = 0;
			Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart = dtGet.DT1;

		}
		//else if(_seqStep && !Master[0].SeqStep)
		//if(Master[0].SeqStep){	
		if((Weld_Report_Status == 1)){		
			NewBeat = 0;
			Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp = dtGet.DT1;
		}

	}
	else{
		
		if(Weld_Report_Status == 0) // Start welding
			Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart = dtGet.DT1;

		else if(Weld_Report_Status == 1) // Stop welding
			Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp = dtGet.DT1;			

	}

}
_seqStep = Master[0].SeqStep;

_weld_Report_Status = Weld_Report_Status;

///////////////////////////////////////

if((Weld_Report_Status < 2))
	ItWasTest = (Master[0].TestMode)? 1:0;

///////////////////////////////////////////////

if((Weld_Report_Status < 2) // calculate only when begining and running
	|| HMI_UpdateLine
	|| HMI_InetrpassTemp_upd){ // or when line values are changed

	HMI_UpdateLine = 0;

	//Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp = dtGet.DT1;

	if(Master[0].ToolHeadID)
		strcpy(Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID , "B-");
	else
		strcpy(Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID , "A-");
	

	if(PS_Param_Set[0].pAmpSP)
	  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps = (*PS_Param_Set[0].pAmpSP);


	if(WIRE_Param_Set[0].pSpeedSp)
	  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed = (*WIRE_Param_Set[0].pSpeedSp) * 2.3622047244094488188976377952756;


	if(AVC_Param_Set[0].pVoltSP )
	  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts = (*AVC_Param_Set[0].pVoltSP) ;


	if(OSC_TML_Param_Set[0].pFreq
		&& OSC_TML_Param_Set[0].pDwell_In
		&& OSC_TML_Param_Set[0].pDwell_Out){

		xsprintf(Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , "%d - %d - %d" , \
			(UDINT)((*OSC_TML_Param_Set[0].pDwell_In)*10.0 +0.5) , (UDINT)((*OSC_TML_Param_Set[0].pFreq)*10.0 +0.5) , (UDINT)((*OSC_TML_Param_Set[0].pDwell_Out)*10.0 +0.5)); 
	}

	

	if(Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten > 0.0){
		w1 =  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps * Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts * 60.0;
			//Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput = (w1/((*TVL_ROT_Param_Set[0].pPrimVel)* 2.3622047244094488188976377952756))*0.001;
		Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput = (w1/Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten)*0.001;
	}
	else
		Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput = 0.0;

	HMI_HeatInput = Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput;


	if(Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten > 0.0){

		//w1 =  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed / (Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten * 2.3622047244094488188976377952756);			
		w1 =  Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed / (Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten);		
		w2 = Weld_Rep.Head.WireDim/2.0;
		w2 = w2*w2 * M_PI;
		
		if((w1*w2) > 0){
			
			Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio = (Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts *Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps)/(w1 * w2)/1000.0;
		}
		else
			Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio = 0.0;
 
	}
	else
		Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio = 0.0;
	

	HMI_PowerRatio = Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio;

	//HMI_PowerRatio_status = ((HMI_PowerRatio >= 66.2) &&  (HMI_PowerRatio <= 87.6))? INVISIBLE:VISIBLE;

	Weld_Report_Start_Enable = 0;
	
	/*
	if(HMI_PowerRatio < 71.0)
		HMI_PowerRatio_Color = 84;
	
	else if(HMI_PowerRatio > 97.6)
		HMI_PowerRatio_Color = 43;

	else {

	*/

	if(Weld_Rep.Report_Type == 0){ // HEAT INPUT

		if(HMI_HeatInput < CurCalib.GenFloatValue[SETUP_FLOAT_HEAT_INPUT_LO])
			HMI_PowerRatio_Color = 84;
		
		else if(HMI_HeatInput > CurCalib.GenFloatValue[SETUP_FLOAT_HEAT_INPUT_HI])
			HMI_PowerRatio_Color = 43;

		else {
			
			HMI_PowerRatio_Color = 15;
			Weld_Report_Start_Enable = 1;

		}

	}else if(Weld_Rep.Report_Type == 1){ // Power Ratio

		if(HMI_PowerRatio < CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_LO])
			HMI_PowerRatio_Color = 84;
		
		else if(HMI_PowerRatio > CurCalib.GenFloatValue[SETUP_FLOAT_POWER_RATIO_HI])
			HMI_PowerRatio_Color = 43;

		else {
			
			HMI_PowerRatio_Color = 15;
			Weld_Report_Start_Enable = 1;

		}
	}else{

		HMI_PowerRatio_Color = 15;
		Weld_Report_Start_Enable = 1;

	}

		
}

else{

	
	if(!Master[0].TestMode)  /// Good
	//if(Master[0].TestMode)  // Test Radom
		Weld_Report_Start_Enable = 1;
	else
		Weld_Report_Start_Enable = 0;
	
	HMI_PowerRatio_Color = 15;

}
/////////////////////////////
// Nie wiem po co to bylo - 3Jan25
/*
	if(!Master[0].ToolHeadID)
		CurCalib.GenSetup[SETUP_LOG_OPTION] = 0;
	else 
		CurCalib.GenSetup[SETUP_LOG_OPTION] = 1;
*/		
////////////////////////////	


switch(CurCalib.GenSetup[SETUP_LOG_OPTION]){

	case 1: // Log After A side

		if(Weld_Report_Status == 2){ // Beat Finished

			if(Master[0].ToolHeadID){

				Weld_Report_Status = 0;
			}


		}

	break;

	case 2: // Log After B side

		if(Weld_Report_Status == 2){ // Beat Finished

			if(!Master[0].ToolHeadID){

				Weld_Report_Status = 0;
			}


		}

	break;

	case 3: // Log After Any Stop - // Allow beat log after any Stop

		if(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart < Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp){

			if(!Master[0].SeqStep && _SeqStep)
				Weld_Report_Status = 2;

			if(Weld_Report_Status == 2)
				Weld_Report_Start_Enable = 1;

		}

		
	break;

	default: break;

}

_SeqStep = Master[0].SeqStep;

HMI_PowerRatio_status = (TempEveryBead_Flag > 0 )? VISIBLE:INVISIBLE;

/*

if(HMI_InetrpassTemp_upd){

	if(Weld_Report_Status == 3){ // Must enter temprature

		if(IsItNumber(Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp)>= 0){
			Weld_Report_Status = 0;
			TempEveryBead_Flag = 1;

		}

	}
}

*/

if(HMI_InetrpassTemp_upd){

	HMI_InetrpassTemp_upd = 0;
	
	if(IsItNumber(Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp)>= 0){

		f = ActTemp();

		if(f > 340.0 ){
			
			//if(Weld_Report_Status == 0)	
				Weld_Report_Status = 4;

		}
		else if( (f < 320.0)  && ( f > 0.0)){

			if(Weld_Report_Status >= 3)			
				Weld_Report_Status = 0; // over temperature - no Welding

		}
		

		if(Weld_Report_Status == 3){ // Must enter temprature

			Weld_Report_Status = 0;
			TempEveryBead_Flag = 1;

		}

		////////////////////		 

		
		
	}
	
}

if(Weld_Report_Status == 4)
	HMI_PowerRatio_Color = 87;


/////////////////////////
//////////////////


/////////////////////////////////////////////////////////
Weld_Report_Mode = (CurCalib.GenSetup[SETUP_EXTRAS]&0x1)? 1:0;

//if((Weld_Report_Status == 2)){ //* Test Radom  ItWasTest
//if((Weld_Report_Status == 2) && !Master[0].TestMode){ // Stare przed 28Jan25

if((Weld_Report_Status == 2)
	&& !ItWasTest 
	&& BtnDelay.Q){	
	
 	HMI_Ack_Btn_vis[Weld_Rep.ActLINE] = VISIBLE;
	Glb_Ack_Btn_vis = VISIBLE;
}
else{
	
	HMI_Ack_Btn_vis[Weld_Rep.ActLINE] = INVISIBLE;	
	Glb_Ack_Btn_vis = INVISIBLE;
}


if(NewFilePage_vis == VISIBLE){

	if(_NewFilePage_vis != NewFilePage_vis){

		
		strcpy(HMI_File_BeadLog , Weld_Rep.Head.WELD_LOG);
		HMI_File_LayerNo =   Weld_Rep.Head.LAYER_No;
		HMI_File_BeadNo = Weld_Rep.Line[0].BeadNo;
		HMI_File_TempEvery = Weld_Rep.Head.TempEvery;
		HMI_File_WireDim = Weld_Rep.Head.WireDim;
		
		
	}

	DT_TO_DTStructure(dtGet.DT1, (UDINT) &HMI_DTStr);
	xsprintf(HMI_File_Name, "%s-%d-%d %d-%d-%d %d-%d" , HMI_File_BeadLog , HMI_File_LayerNo , HMI_File_BeadNo , \
							HMI_DTStr.year , HMI_DTStr.month , HMI_DTStr.day , HMI_DTStr.hour , HMI_DTStr.minute);

}
_NewFilePage_vis = NewFilePage_vis;





if(SetTimePage_vis == VISIBLE){

	if(_SetTimePage_vis != SetTimePage_vis)
		DT_TO_DTStructure(LocalTime, (UDINT) &HMI_SetDTStr);		
	
	
	if(DTSetTime_1.enable){		
				
		DTSetTime_1.DT1 = DTStructure_TO_DT((UDINT) &HMI_SetDTStr);
	
		DTSetTime(&DTSetTime_1);
		if(!DTSetTime_1.status)
			DTSetTime_1.enable = 0;
			
		//SetTimePage_vis = INVISIBLE;

	}
	
}
_SetTimePage_vis = SetTimePage_vis;

////////////////
HMI_ClearError_vis = (File_RW2.Done && File_RW2.Error )? VISIBLE:INVISIBLE;

if(HMI_ClearError_btn){
	HMI_ClearError_btn = 0;
	
	File_RW2.Error = 0;
	File_RW2.Done = 0;
	File_RW2.CMD = 0;
	Page_Step = 0;

}
////////////////////

switch(Page_Step){


	default:

		GotoScreen = 10;		

		if(Weld_Rep.ActLINE < 16 ){

			for (i = Weld_Rep.ActLINE ; i < 16 ; i++){
			
				HMI_Line_vis[i] = INVISIBLE;
				HMI_Ack_Btn_vis[i] = INVISIBLE;
			}


			for (i = 0 ; i < Weld_Rep.ActLINE ; i++){

					HMI_Line_vis[i] = VISIBLE;
					HMI_Ack_Btn_vis[i] = INVISIBLE;
					HMI_Lock_Line[i] = 1;
			}		

		}
		else{

			Weld_Rep.ActLINE = 15;
			
			for (i = 0 ; i < 15 ; i++){
			
				HMI_Line_vis[i] = VISIBLE;
				HMI_Ack_Btn_vis[i] = INVISIBLE;
				HMI_Lock_Line[i] = 1;
			}

			
						
		}
		
		HMI_Lock_Line[Weld_Rep.ActLINE] = 0;
		HMI_Ack_Btn_vis[Weld_Rep.ActLINE] = VISIBLE;
		HMI_Line_vis[i] = VISIBLE;

		HMI_Head_Lock = (Weld_Rep.ActLINE > 0)? 1:0;

		HMI_Message_Vis = INVISIBLE;
	
		Page_Step = 1;

	break;


	case 1:	// Wait for Acknowledge Line		


		if(!BtnDelay.Q){

			Glb_Ack_Btn = 0;			
			
			for(i = 0 ; i < 16 ; i++)
				Ack_Btn[i] = 0;

			break;

		}

		/////////////////////////////////
		status = 0;
		for(i = 0 ; i < 16 ; i++){

			if(Ack_Btn[i])
				status = i+1;
			
		}

		KK_Catch = status;

		if((status > 0) || Glb_Ack_Btn){

			Glb_Ack_Btn = 0;			
			
			for(i = 0 ; i < 16 ; i++)
				Ack_Btn[i] = 0;

			if(ActTemp() > 340)
				Weld_Report_Status = 4;	
			else				
				Weld_Report_Status = 0;	
				
			TimerStart(&BtnDelay, 4.0);
			
			Page_Step = 3;

		}
		

	break;

	
	case 3: // Add records TXT		

		//NewFilePage_vis = INVISIBLE;

		KK_Result = IsItNumber(Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp);

		
		_PowerRatio = KK_PowerRatio; 
		
		//status = ((_PowerRatio > 350) || (_PowerRatio < 340))? 1:0;
		//if(status)

		if(KK_Result >= 0){
				
			f = atof((UDINT) Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp); 
			
			if(f > 340)
				TempEveryBead_Flag = 1;
			else if (f < 300)
				TempEveryBead_Flag = 0;			

		}
	
		//if(HMI_PowerRatio_status == VISIBLE)
			//LastBad_BEADS_Cnt = Weld_Rep.BEADS_Cnt;
			
		if(TempEveryBead_Flag)
			LastBad_BEADS_Cnt = Weld_Rep.BEADS_Cnt;

		
		if(((KK_Result< 0))
			&& ((((Weld_Rep.BEADS_Cnt - LastBad_BEADS_Cnt)%(UINT)Weld_Rep.Head.TempEvery) == 0)
				|| (LastBad_BEADS_Cnt == Weld_Rep.BEADS_Cnt))){

			HMI_Message_Vis = VISIBLE;

			Page_Step = 1;
			
			break;
		}
	
		strcpy(File_RW2.fileName , Weld_Rep.FileName);
		strcat(File_RW2.fileName , File_RW2.fileExten);
		File_RW2.CMD = F_OPEN_ONLY;

		Page_Step ++;

	break;	

	case 4:

		if(File_RW2.Done && !File_RW2.Error){

			if((File_RW2._int.FOpen.filelen == 0)){	

				memset(&LongBuf[0] , 0 , 512);
				LongBufSize = 0;

				
				Weld_Rep.BEADS_Cnt = 1;

				//CreateHeaderTxt(&LongBuf[0]);

				/*
				if(Weld_Rep.Report_Type == 0) // HEAT INPUT
					CreateLineTxt_01(CreateHeaderTxt_01(LongBuf));
				else if((Weld_Rep.Report_Type == 1)) // Power Ratio	
					CreateLineTxt_01(CreateHeaderTxt_01(LongBuf));
				else // 
				*/
					CreateLineTxt_01(CreateHeaderTxt_01(LongBuf));
				
				//File_RW2.pMem = LongBuf;
				//File_RW2.Len = LongBufSize;
				//File_RW2.Offset = 0;

			}
			else{

				for(i = 0 ; i < 512 ; i++)
					LongBuf[i] = 0;
				
				//memset(&LongBuf[0] , 0 , 512);
				LongBufSize = 0;

				Weld_Rep.BEADS_Cnt++;

				/*
				if(Weld_Rep.Report_Type == 0) // HEAT INPUT
					CreateLineTxt_01(LongBuf);
				else if((Weld_Rep.Report_Type == 1)) // Power Ratio	
					CreateLineTxt_01(LongBuf);
				else // */
					CreateLineTxt_01(LongBuf);			
				
				

			}

			File_RW2.pMem = LongBuf;
			File_RW2.Len = LongBufSize;
			File_RW2.Offset = File_RW2._int.FOpen.filelen;

			HMI_Head_Lock = 1;	
			File_RW2.CMD = F_WRITE_ONLY;

			Page_Step ++;

		}
		else if(File_RW2.Error){

			//Page_Step = 0;
		}

	break;	

	case 5:

		if(File_RW2.Done && !File_RW2.Error ){			

			File_RW2.CMD = F_CLOSE_ONLY;

			Page_Step ++;

		}
		else if(File_RW2.Error){

			//Page_Step = 0;
		}

	break;



	case 6:

		if(File_RW2.Done){


			if(File_RW2.Error){

				//Page_Step = 0;

			}else{

				if(Weld_Rep.ActLINE < 15 ){

					HMI_Lock_Line[Weld_Rep.ActLINE] = 1;
					HMI_Ack_Btn_vis[Weld_Rep.ActLINE] = INVISIBLE;
					Weld_Rep.ActLINE++;
					HMI_Line_vis[Weld_Rep.ActLINE] = VISIBLE;
					HMI_Lock_Line[Weld_Rep.ActLINE] = 0;
					HMI_Ack_Btn_vis[Weld_Rep.ActLINE] = VISIBLE;				
					
					i = Weld_Rep.ActLINE;
					
					memcpy(&Weld_Rep.Line[i] , &Weld_Rep.Line[i-1] , sizeof(Weld_Report_Line_typ));
					

				}else{


					for(i = 1 ; i < 16 ; i++)
						memcpy(&Weld_Rep.Line[i-1] , &Weld_Rep.Line[i] , sizeof(Weld_Report_Line_typ));

					i = 15;				
				}

				Weld_Rep.Line[i].BeadNo = Weld_Rep.Line[i-1].BeadNo + 1; 
				
				if(ActTemp() < 340.0)
				 	strcpy(Weld_Rep.Line[i].InetrpassTemp , ""); 
				
				strcpy(Weld_Rep.Line[i].Comments , ""); 

				if(ActTemp() > 340)
					Weld_Report_Status = 4;	
				else				
					Weld_Report_Status = 0;	


				NewBeat = 1;
				
				Page_Step = 1;

			}

		}
		

	break;	
	
	//////////////////    New Report TXT ////////////////////////////
	 case 80:		
		
		strcpy(Weld_Rep.Head.WELD_LOG , HMI_File_BeadLog);
		Weld_Rep.Head.LAYER_No = HMI_File_LayerNo;
		Weld_Rep.Line[0].BeadNo = HMI_File_BeadNo;
		Weld_Rep.Head.TempEvery = HMI_File_TempEvery;
		Weld_Rep.Head.WireDim = HMI_File_WireDim;
		//'%0.2d'
		DT_TO_DTStructure(dtGet.DT1, (UDINT) &HMI_DTStr);
		xsprintf(Weld_Rep.FileName, "%s-%d-%d %d-%0.2d-%0.2d %0.2d-%0.2d" , Weld_Rep.Head.WELD_LOG , Weld_Rep.Head.LAYER_No , Weld_Rep.Line[0].BeadNo , \
											HMI_DTStr.year , HMI_DTStr.month , HMI_DTStr.day , HMI_DTStr.hour , HMI_DTStr.minute);

		if(Weld_Rep.Head.WireDim <= 0.0)
			Weld_Rep.Head.WireDim = 0.045;

		if(Weld_Rep.Head.TempEvery == 0)
			Weld_Rep.Head.TempEvery = 4;
		
		i = 0;
		
		xsprintf(Weld_Rep.Line[i].WelderStamp,"RB 22%d", (DINT)i+1);	

		Weld_Rep.Line[i].PrimaryAmps = 100.0;
		Weld_Rep.Line[i].PrimaryWireSpeed = 200.0;
		Weld_Rep.Line[i].PrimaryVolts = 10.0;
		Weld_Rep.Line[i].Travel_Tungsten = 3.0;
		Weld_Rep.Line[i].PowerRatio = 75.0;
		strcpy(Weld_Rep.Line[i].InetrpassTemp , "");
		xsprintf(Weld_Rep.Line[i].Dwell_Excursion,"%d - %d - %d", (i%10) , ((i+1)%10) , ((i+2)%10));

		xsprintf(Weld_Rep.Line[i].Comments,"");
		

		
		Weld_Rep.BEADS_Cnt = 0;
		Weld_Rep.LINES_Offset = 0;
		Weld_Rep.ActLINE = 0;

		HMI_Line_vis[0] = VISIBLE;
		HMI_Ack_Btn_vis[0] = VISIBLE;
		HMI_Lock_Line[0] = 0;

		HMI_Head_Lock = 0;
		

		for (i = 1 ; i < 16 ; i++){
			
			HMI_Line_vis[i] = INVISIBLE;
			HMI_Ack_Btn_vis[i] = INVISIBLE;
		}

		Page_Step ++;
		
	 break;



	 case 81:

	 	strcpy(File_RW2.fileNewName , Weld_Rep.FileName);
		File_RW2.CMD = F_WRITE;
	 	File_RW2.pMem =&Weld_Rep;
		File_RW2.Len = 0;
		File_RW2.Offset = 0;;

		Page_Step ++;


	 break;

	 case 82:

	 	if(File_RW2.Done){

	 	
			if(File_RW2.Error){

				//Page_Step = 0;
			}
			else{
		
				NewFilePage_vis = INVISIBLE;	
				Page_Step = 1;

				Weld_Report_Status = 3;
				LastBad_BEADS_Cnt = Weld_Rep.BEADS_Cnt;
			}

	 	}

	 break;

}
	



TimerExecute(&BtnDelay);
MenuBar();
/////////////////////////////////////////////////////////



	/**************************************************/
	pFileRW = (FILE2_RW_typ*) &File_RW2;

	/*************************************************/
	if((pFileRW->CMD == 0)){

		pFileRW->Tmr.IN = 0;
		pFileRW->Tmr.Q = 0;


	}

	else if(TimerIsTimeOut(&pFileRW->Tmr)){

		pFileRW->Tmr.IN = 0;
		pFileRW->Tmr.Q = 0;

				
	}
	/**********************************************/

	if(pFileRW->_int.step == 0){	
		
		
		if(pFileRW->CMD && !_CMD){

	

			switch(pFileRW->CMD){			
				

				default:
					TimerStart(&pFileRW->Tmr, 10.0); // 15 sec to run command 	
				break;
			}
			


		}

		
		
		switch(pFileRW->CMD){			


			case F_WRITE:

				//strcpy(pFileRW->fileNewName , pFileRW->fileNewName);

				DeleteExtension(pFileRW->fileNewName);				
				strcat(pFileRW->fileNewName , pFileRW->fileExten);
				
				
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 50;

				

			break;	

			case F_OPEN_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 120;
				
			break;			
			
			
			case F_WRITE_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 140;
				
			break;

			
			case F_CLOSE_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 150;
				
			break;	

			
			default:

				if(pFileRW->CMD)
					pFileRW->_int.step = 254;  // error

			break;	

			
		 }


		/////////////////////////////////
		_CMD = pFileRW->CMD;

	}
	/********************************************/

	switch (pFileRW->_int.step )
  	{
  
		case 0: /**** First step ****/

			pFileRW->CMD = 0;
					
		break;
	
		
		////////    //////////////////////

		case 50: /**** open existing file ****/

			strcpy(pFileRW->fileName ,pFileRW->fileNewName );
			
			_Open(pFileRW);
			

			 if (pFileRW->_int.wStatus == ERR_OK)
			{									
					
					pFileRW->_int.step ++;
			
							
			}
			else if (pFileRW->_int.wStatus == fiERR_FILE_NOT_FOUND )
			{

				pFileRW->_int.step += 2;
			
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				pFileRW->_int.step  = 254; // Error	
				
			if (pFileRW->_int.wStatus == 20799)
			{
			  pFileRW->_int.wError = FileIoGetSysError();
			}
			}
		break;

		case 51: /**** Delete file ****/

			_Delete(pFileRW);
			
			/* Verify status */
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)		   {			
				
		   	 	pFileRW->_int.step ++;		   	 
		   }
			
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 7;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}		
		break;

		case 52: /**** Create file ****/
			
			_Create(pFileRW);

			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
					
			 	pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 2;
				pFileRW->_int.step  = 254; // Error	
				                    
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

		case 53: /**** Write data ****/
	
			
			_Write(pFileRW);
			
			if (pFileRW->_int.wStatus == 0)
			{  
												
			  	pFileRW->_int.step++;  
			  
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 6;
			  	pFileRW->_int.step  = 254; // Error	
			  	
				if (pFileRW->_int.wStatus == 20799)
				{
				 	pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

     	
		case 54: /**** Close file ****/

			_Close(pFileRW);

			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {
		   				
				
				DeleteExtension(pFileRW->fileName);											
				
				//pFileRW->_int.step = 62; // get new directory info into string	

				pFileRW->_int.step  = 255; // Success
								
				
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}				  
		break;

		//////////////////////////////////////////////////////

		case 120: /** OPEN ONLY **/
			
			_Open(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;			

		//////////////////////////////////////////////////////

		case 140: /** Write ONLY **/
			
			_Write(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	

		
		//////////////////////////////////////////////////////
		
		case 150: /** Close ONLY **/
			
			_Close(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	
		


		////////////////////////////////////////////////////////
		// Error

		case 254: /**** Error ****/

		
			switch(pFileRW->_int.wStatus){

				case 9999: //ERR_NOTIMPLEMENTED
					strcpy(pFileRW->MsgText , "Err: not Implemented"); break;				

				case 20700: //fiERR_INVALID_PATH 
					strcpy(pFileRW->MsgText , "Err: Invalid Path"); break;

				case 20705: //fiERR_EXIST
					strcpy(pFileRW->MsgText , "Err: File Already Exist"); break;				

				case 20799: //fiERR_SYSTEM
					strcpy(pFileRW->MsgText , "Err: System "); break;

				case 20798://fiERR_DEVICE_MANAGER
					strcpy(pFileRW->MsgText , "Err: Device Manager"); break;

				case 20797: //fiERR_DEVICE_DRIVER
					strcpy(pFileRW->MsgText , "Err: Device Driver"); break;

				case 20725: //fiERR_DIR_ALREADY_EXIST
					strcpy(pFileRW->MsgText , "Err: Directory Already Exist"); break;

				case 20724: //fiERR_DIR_NOT_EMPTY
					strcpy(pFileRW->MsgText , "Err: Directory Not Empty"); break;

				case 20723: //fiERR_DIR_NOT_EXIST
					strcpy(pFileRW->MsgText , "Err: Directory Not Exist"); break;

				case 20722: //fiERR_INVALID_DIRECTORY
					strcpy(pFileRW->MsgText , "Err: Invalid Directory"); break;

				case 20721:	//fiERR_FILE_NOT_OPENED
					strcpy(pFileRW->MsgText , "Err: File Not Opened"); break;

				case 20718:	//fiERR_COM_FILE_IOCTL
					strcpy(pFileRW->MsgText , "Err: File IO Control"); break;

				case 20717: //fiERR_COM_FILE_WRITE
					strcpy(pFileRW->MsgText , "Err: File Write"); break;

				case 20716:	//fiERR_COM_FILE_READ
					strcpy(pFileRW->MsgText , "Err: File Read"); break;

				case 20715:	//fiERR_COM_FILE_CLOSE
					strcpy(pFileRW->MsgText , "Err: File Close"); break;

				case 20714:	//fiERR_COM_FILE_OPEN
					strcpy(pFileRW->MsgText , "Err: File Open"); break;

				case 20708:	//fiERR_FILE_NOT_FOUND
					strcpy( pFileRW->MsgText , "Err: File Not Found"); break;

				default: strcpy( pFileRW->MsgText , "Err: Unknown"); break;
				//default: xsprintf(pFileRW->MsgText , "Err.Cmd: %d Code %d" , pFileRW->CMD , pFileRW->_int.wStatus); break;

				

			}

			PleaseShowFileMessage = 1;	

			
			DeleteExtension(pFileRW->fileName);			
			DeleteExtension(pFileRW->fileNewName);

			pFileRW->Error = 1;
			pFileRW->Done  = 1;	
			pFileRW->_int.step  = 0; 
			pFileRW->CMD = 0;
		

		break;	

		case 255: /**** Success****/

			DeleteExtension(pFileRW->fileName);			
			DeleteExtension(pFileRW->fileNewName);

			pFileRW->Error = 0;
			pFileRW->Done  = 1;				
			pFileRW->_int.step  = 0;

			
			switch(	pFileRW->CMD){

				case F_READ: 				strcpy(pFileRW->MsgText , "F_READ Done" ); PleaseShowFileMessage = 1; break;  
				case F_WRITE: 				strcpy(pFileRW->MsgText , "F_WRITE Done");  PleaseShowFileMessage = 1; break;  
				case F_DELETE: 				strcpy(pFileRW->MsgText , "F_DELETE Done");  PleaseShowFileMessage = 1; break;  
				case F_FREE_MEM: 			strcpy(pFileRW->MsgText , "F_FREE_MEM"); break;  
								
				
				case F_RENAME: 				strcpy(pFileRW->MsgText , "F_RENAME Done" );  PleaseShowFileMessage = 1; break;  
				case F_COPY: 				strcpy(pFileRW->MsgText , "F_COPY Done" );  PleaseShowFileMessage = 1; break;  
				
				//case F_READ_FILE_TO_TMP: 	strcpy(pFileRW->MsgText , "F_READ_FILE_TO_TMP"); break;  
				//case F_SAVE_AS_TMP: 		strcpy(pFileRW->MsgText , "F_SAVE_AS_TMP"); break;  
								
				case F_READ_DIR: 			strcpy(pFileRW->MsgText , "F_READ_DIR"); break;  
				case F_READ_FILE_INF: 		strcpy(pFileRW->MsgText , "F_READ_FILE_INF"); break;  
				case F_READ_DIR_FILELIST: 	strcpy(pFileRW->MsgText , "F_READ_DIR_FILELIST"); break;  

				
			


			}
			
			pFileRW->CMD = 0;
			

		break;

			
  }
	TimerExecute(&pFileRW->Tmr);
}


void _Open(FILE2_RW_typ *f)
{
	
	f->_int.FOpen.enable		= 1;
	f->_int.FOpen.pDevice   = (UDINT)f->deviceName;
	f->_int.FOpen.pFile = (UDINT)f->fileName;
	f->_int.FOpen.mode	  = FILE_RW;
	FileOpen(&f->_int.FOpen);

	f->_int.dwIdent = f->_int.FOpen.ident;
	f->_int.wStatus = f->_int.FOpen.status;

	return;
}
void _Close(FILE2_RW_typ *f){

	f->_int.FClose.enable	 = 1;
	f->_int.FClose.ident 	 = f->_int.dwIdent;
	FileClose(&f->_int.FClose);

	f->_int.wStatus = f->_int.FClose.status;

	
	return;

		   
}
void _Create(FILE2_RW_typ *f){

	
	f->_int.FCreate.enable	= 1;
	f->_int.FCreate.pDevice = (UDINT) f->deviceName;
	f->_int.FCreate.pFile   = (UDINT)f->fileNewName;
	FileCreate(&f->_int.FCreate);

	f->_int.dwIdent = f->_int.FCreate.ident;
	f->_int.wStatus = f->_int.FCreate.status;

	
	return;
	
}

void _Write(FILE2_RW_typ *f){

	f->_int.FWrite.enable   = 1;
	f->_int.FWrite.ident    = f->_int.dwIdent;
	f->_int.FWrite.offset   = f->Offset;
	f->_int.FWrite.pSrc	   = f->pMem;
	f->_int.FWrite.len	   = f->Len;

	FileWrite(&f->_int.FWrite);

	f->_int.wStatus = f->_int.FWrite.status;

	
	return;
	

}



void _Delete(FILE2_RW_typ *f){

	f->_int.FDelete.enable	 = 1;
	f->_int.FDelete.pDevice = (UDINT) f->deviceName;
	f->_int.FDelete.pName	=  (UDINT)f->fileName;
	/* Call FBK */
	FileDelete(&f->_int.FDelete);
	/* Get status */
	f->_int.wStatus = f->_int.FDelete.status;

	
	return;
}



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

void  MenuBar(){

	if ( GotoScreen < 0xFFFF ) {

		if(GotoScreen >= 10){
			if(GotoScreen != ReadPage) {

				//Screen.ReturnScreen	= Screen.ReadPage;		
				SetPage = GotoScreen;
		
			}
			GotoScreen 	= 0xFFFF;
		}
	}

	
return;
}

char* CreateHeaderTxt_01(char *p){
	
	xsprintf(p ,"\t WELD OPERATOR BEAD LOG -\t %s\n\nWELD No.:\t%s\t" ,\
		Weld_Rep.Head.WELD_LOG , Weld_Rep.Head.WELD_No); 
		
	_Len[0] = strlen(p); p+=_Len[0];
	
	xsprintf(p ,"\t WIRE HEAT No.:\t%s\t\tMACHINE I.D.:\t%s\n\n" , \
		Weld_Rep.Head.WIRE_HEAT_No , Weld_Rep.Head.MACHINE_Id);
	_Len[1] = strlen(p); p+=_Len[1];

	xsprintf(p ,"LAYER No.:\t%d\t\tWPS:\t %s\t\tM_TE:\t %s\t\tCal.Due:\t%s\n\n"\
			, Weld_Rep.Head.LAYER_No , Weld_Rep.Head.WPS , Weld_Rep.Head.M_TE , Weld_Rep.Head.Cal_Due );
	_Len[2] = strlen(p); p+=_Len[2];

	xsprintf(p ,"Welder\t\t\tBead\tPrimary\tPrimary\tPrimary\tDwell Out /\t");
	_Len[3] = strlen(p); p+=_Len[3];


	switch(Weld_Rep.Report_Type){

		case 0:	 // Heat Input	
			xsprintf(p ,"Travel @\tHeat Input)\t Interpass\t \n");
			_Len[4] = strlen(p); p+=_Len[4];
	
			xsprintf(p ,"Stamp No.\tStart\tFinish\tNo.\tAmps\tWire Speed\tVolts\tExcur/Dwell In\t");
			_Len[5] = strlen(p); p+=_Len[5];

			xsprintf(p ,"Tungsten\t (kW/in2)\t Temp (F)\t Comments \n");
			_Len[6] = strlen(p); p+=_Len[6];
			
		break;

		case 1:	// Power Ratio	
		
			xsprintf(p ,"Travel @\tPower Ratio)\t Interpass\t \n");
			_Len[4] = strlen(p); p+=_Len[4];
	
			xsprintf(p ,"Stamp No.\tStart\tFinish\tNo.\tAmps\tWire Speed\tVolts\tExcur/Dwell In\t");
			_Len[5] = strlen(p); p+=_Len[5];

			xsprintf(p ,"Tungsten\t (kW/in2)\t Temp (F)\t Comments \n");
			_Len[6] = strlen(p); p+=_Len[6];
			
		break;

		default:// Nothing	
		
			xsprintf(p ,"Travel @\tInterpass\t\n");	

			_Len[4] = strlen(p); p+=_Len[4];
	
			xsprintf(p ,"Stamp No.\tStart\tFinish\tNo.\tAmps\tWire Speed\tVolts\tExcur/Dwell In\t");
			_Len[5] = strlen(p); p+=_Len[5];

			//xsprintf(p ,"Tungsten\t (kW/in2)\t (kJ)\t Temp (F)\t Comments \n");

			xsprintf(p ,"Tungsten\tTemp (F)\tComments\n");
			_Len[6] = strlen(p); p+=_Len[6];		
			
						
		break;
		
	}					
	
	LongBufSize += _Len[0] + _Len[1] + _Len[2] + _Len[3] + _Len[4] + _Len[5] + _Len[6];
	
	return p;

}


char* CreateHeaderTxt_2(char *p){
	
	xsprintf(p ,"\t WELD OPERATOR BEAD LOG -\t %s\n\nWELD No.:\t%s\t" ,\
		Weld_Rep.Head.WELD_LOG , Weld_Rep.Head.WELD_No); 
		
	_Len[0] = strlen(p); p+=_Len[0];
	
	xsprintf(p ,"\t WIRE HEAT No.:\t%s\t\tMACHINE I.D.:\t%s\n\n" , \
		Weld_Rep.Head.WIRE_HEAT_No , Weld_Rep.Head.MACHINE_Id);
	_Len[1] = strlen(p); p+=_Len[1];

	xsprintf(p ,"LAYER No.:\t%d\t\tWPS:\t %s\t\tM_TE:\t %s\t\tCal.Due:\t%s\n\n"\
			, Weld_Rep.Head.LAYER_No , Weld_Rep.Head.WPS , Weld_Rep.Head.M_TE , Weld_Rep.Head.Cal_Due );
	_Len[2] = strlen(p); p+=_Len[2];

	xsprintf(p ,"Welder\t\t\tBead\tPrimary\tPrimary\tPrimary\tDwell Out /\t");
	_Len[3] = strlen(p); p+=_Len[3];

		
	xsprintf(p ,"Travel @\tInterpass\t\n");			
		
	_Len[4] = strlen(p); p+=_Len[4];
	
	xsprintf(p ,"Stamp No.\tStart\tFinish\tNo.\tAmps\tWire Speed\tVolts\tExcur/Dwell In\t");
	_Len[5] = strlen(p); p+=_Len[5];

	//xsprintf(p ,"Tungsten\t (kW/in2)\t (kJ)\t Temp (F)\t Comments \n");

	xsprintf(p ,"Tungsten\tTemp (F)\tComments\n");
	_Len[6] = strlen(p); p+=_Len[6];
	
	
	LongBufSize += _Len[0] + _Len[1] + _Len[2] + _Len[3] + _Len[4] + _Len[5] + _Len[6];

	
	
	return p;

}


char* CreateHeaderTxt(char *p){
	
	xsprintf(p ,"\t WELD OPERATOR BEAD LOG - \t %s \n\n WELD No.:\t %s \t" ,\
		Weld_Rep.Head.WELD_LOG , Weld_Rep.Head.WELD_No); 
		
	_Len[0] = strlen(p); p+=_Len[0];
	
	xsprintf(p ,"\t WIRE HEAT No.: \t %s \t \tMACHINE I.D.: \t %s \n\n" , \
		Weld_Rep.Head.WIRE_HEAT_No , Weld_Rep.Head.MACHINE_Id);
	_Len[1] = strlen(p); p+=_Len[1];

	xsprintf(p ,"LAYER No.:\t %d \t\t WPS:\t %s \t\tM_TE:\t %s \t\t Cal.Due:\t %s \n\n"\
			, Weld_Rep.Head.LAYER_No , Weld_Rep.Head.WPS , Weld_Rep.Head.M_TE , Weld_Rep.Head.Cal_Due );
	_Len[2] = strlen(p); p+=_Len[2];

	xsprintf(p ,"Welder\t \tBead\t Primary\t Primary\t Primary\t Dwell Out / \t");
	_Len[3] = strlen(p); p+=_Len[3];


	switch(Weld_Rep.Report_Type){

		case 0:	 // Heat Input	
			xsprintf(p ,"Travel @\tHeat Input)\t Interpass\t \n");
			
		break;

		case 1:	// Power Ratio	
		
			xsprintf(p ,"Travel @\tPower Ratio)\t Interpass\t \n");
			
		break;

		default:// Nothing	
		
			xsprintf(p ,"Travel @\t \t Interpass\t \n");
			
		break;

		
	}	
	_Len[4] = strlen(p); p+=_Len[4];
	
	xsprintf(p ,"Stamp No.\t Date\tNo. \t Amps\t Wire Speed \t Volts \tExcur/Dwell In\t");
	_Len[5] = strlen(p); p+=_Len[5];

	//xsprintf(p ,"Tungsten\t (kW/in2)\t (kJ)\t Temp (F)\t Comments \n");

	switch(Weld_Rep.Report_Type){

		case 0:	 // Heat Input

			xsprintf(p ,"Tungsten\t (kW/in2)\t Temp (F)\t Comments \n");

		break;

		case 1:	// Power Ratio

			xsprintf(p ,"Tungsten\t (kW/in2)\t Temp (F)\t Comments \n");

		break;

		default:

			xsprintf(p ,"Tungsten\t \t Temp (F)\t Comments \n");


		break;

	}
	
	_Len[6] = strlen(p); p+=_Len[6];
	
	
	LongBufSize += _Len[0] + _Len[1] + _Len[2] + _Len[3] + _Len[4] + _Len[5] + _Len[6];

	
	
	return p;

}
char* CreateLineTxt_2(char *p){
DTStructure DTStr , DTStr_start;


	if(Weld_Rep.ActLINE >= 16)
		Weld_Rep.ActLINE = 15;	

	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp, (UDINT) &DTStr);
	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart, (UDINT) &DTStr_start);
	
	
	//xsprintf(p ,"%s\t-%0.2d/%0.2d/%d-%0.2d:%0.2d:%0.2d\t=%0.2d/%0.2d/%d-%0.2d:%0.2d:%0.2d\t" ,
	xsprintf(p ,"%s\t%0.2d/%0.2d/%d-%0.2d:%0.2d:%0.2d\t%0.2d/%0.2d/%d-%0.2d:%0.2d:%0.2d\t" ,\	
		Weld_Rep.Line[Weld_Rep.ActLINE].WelderStamp , DTStr_start.month ,  DTStr_start.day ,   DTStr_start.year , \
		DTStr_start.hour , DTStr_start.minute , DTStr_start.second , DTStr.month ,  DTStr.day ,   DTStr.year ,DTStr.hour , DTStr.minute , DTStr.second);
		
	_Len[0] = strlen(p); p+=_Len[0];

	xsprintf(p ,"%s%d\t%f\t%f\t%4.1f\t%s\t%4.1f\t%s\t%s\t\n" ,
				Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
				Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
				Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 


	
	_Len[1] = strlen(p); p+=_Len[1];
	
	LongBufSize += _Len[0] + _Len[1];
	
	
	return p;
}

char* CreateLineTxt_01(char *p){
DTStructure DTStr , DTStr_start;


	if(Weld_Rep.ActLINE >= 16)
		Weld_Rep.ActLINE = 15;	

	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp, (UDINT) &DTStr);
	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart, (UDINT) &DTStr_start);
	
	
	//xsprintf(p ,"%s\t-%0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d\t=%0.2d/%0.2d/%d-%0.2d:%0.2d:%0.2d\t" ,
	xsprintf(p ,"%s\t%0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d\t%0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d\t" ,\	
		Weld_Rep.Line[Weld_Rep.ActLINE].WelderStamp , DTStr_start.month ,  DTStr_start.day ,   DTStr_start.year , \
		DTStr_start.hour , DTStr_start.minute , DTStr_start.second , DTStr.month ,  DTStr.day ,   DTStr.year ,DTStr.hour , DTStr.minute , DTStr.second);
		
	_Len[0] = strlen(p); p+=_Len[0];

	
	switch(Weld_Rep.Report_Type){
	
			case 0:  // Heat Input	
				
				xsprintf(p ," %s%d\t%f\t%f\t%4.1f\t %s\t%4.1f\t%4.1f\t %s\t%s\t\n" ,
					Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
					Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
					Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput	, Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 
	
			break;
	
			case 1: // Power Ratio
	
				
				xsprintf(p ," %s%d\t%f\t%f\t%4.1f\t %s\t%4.1f\t%4.1f\t%s\t%s\t\n" ,
					Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
					Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
					Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio	, Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 
	
	
			break;
	
			default:
	
				xsprintf(p ,"%s%d\t%f\t%f\t%4.1f\t %s\t%4.1f\t%s\t%s\t\n" ,
					Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
					Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
					Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 

	
			break;
	
		}	

	
	_Len[1] = strlen(p); p+=_Len[1];
	
	LongBufSize += _Len[0] + _Len[1];
	
	
	return p;
}


char* CreateLineTxt(char *p){
DTStructure DTStr , DTStr_start;


	if(Weld_Rep.ActLINE >= 16)
		Weld_Rep.ActLINE = 15;	

	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStamp, (UDINT) &DTStr);
	DT_TO_DTStructure(Weld_Rep.Line[Weld_Rep.ActLINE].TimeStampStart, (UDINT) &DTStr_start);
	
	/*
	xsprintf(p ,"%s\t %0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d \t" ,\
		Weld_Rep.Line[Weld_Rep.ActLINE].WelderStamp , DTStr.month ,  DTStr.day ,   DTStr.year , DTStr.hour , DTStr.minute , DTStr.second); 
	*/

	xsprintf(p ,"%s\t %0.2d/%0.2d/%d %0.2d:%0.2d:%0.2d - %0.2d:%0.2d:%0.2d \t" ,\
		Weld_Rep.Line[Weld_Rep.ActLINE].WelderStamp , DTStr_start.month ,  DTStr_start.day ,   DTStr_start.year , \
		DTStr_start.hour , DTStr_start.minute , DTStr_start.second , DTStr.hour , DTStr.minute , DTStr.second);
		
	_Len[0] = strlen(p); p+=_Len[0];

	switch(Weld_Rep.Report_Type){

		case 0:	 // Heat Input	
			
			xsprintf(p ," %s%d \t %f \t %f \t %4.1f \t %s \t %4.1f \t %4.1f \t %s \t %s \t \n" ,
				Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
				Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
				Weld_Rep.Line[Weld_Rep.ActLINE].HeatInput	, Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 

		break;

		case 1: // Power Ratio

			
			xsprintf(p ," %s%d \t %f \t %f \t %4.1f \t %s \t %4.1f \t %4.1f \t %s \t %s \t \n" ,
				Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
				Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
				Weld_Rep.Line[Weld_Rep.ActLINE].PowerRatio	, Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 


		break;

		default:

			xsprintf(p ," %s%d \t %f \t %f \t %4.1f \t %s \t %4.1f \t  \t %s \t %s \t \n" ,
				Weld_Rep.Line[Weld_Rep.ActLINE].ToolHeadID, Weld_Rep.Line[Weld_Rep.ActLINE].BeadNo , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryAmps , Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryWireSpeed , \
				Weld_Rep.Line[Weld_Rep.ActLINE].PrimaryVolts , Weld_Rep.Line[Weld_Rep.ActLINE].Dwell_Excursion , Weld_Rep.Line[Weld_Rep.ActLINE].Travel_Tungsten ,\
				Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp , Weld_Rep.Line[Weld_Rep.ActLINE].Comments ); 


		break;

	}	

		
	
	_Len[1] = strlen(p); p+=_Len[1];
	
	LongBufSize += _Len[0] + _Len[1];
	
	
	return p;
}

SINT IsItNumber(char*p){
UINT len;
SINT status;

	len = 0;
	status = 0;
	

	while((*p!= 0) && (len < 10) && (status >= 0)){

		if(*p == '.'){

			if(status == 0)
				status = 1;
			else
				status = -1;

		}
		else if((*p < '0') || (*p > '9'))
			status = -2;

		p++;
		len++;

	}

	if(len == 0)
		status = -3;

	return status;

}

float ActTemp(){

float ret;

	if(IsItNumber(Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp)>= 0){

		
		ret = atof((UDINT) Weld_Rep.Line[Weld_Rep.ActLINE].InetrpassTemp);  

	}
	else
		ret = 0.0;

	return ret;

}
