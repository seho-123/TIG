/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: AVC_SYNCH
 * File: AVC_SYNCH.c
 * Author: ckaczkowski
 * Created: November 18, 2014
 ********************************************************************
 * Implementation of program AVC_SYNCH
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <math.h>

#include <..\GlobDef.h>
#include <..\..\Lib\WSI_Lib.c>
#include <..\..\Lib\Statelib.c>


#include <..\GlobDecl.h>

_LOCAL MASTER_typ *m;
_LOCAL CurCfg_typ *curCfg;
_LOCAL Cfg_typ *cfg;


_LOCAL char Set_Full_Filter , FullFilter_Done , NormalFilter_Done ;
_LOCAL char Set_Gain , SetGain_Done , SetZeroGain_Done ;

_LOCAL float OSC_VelTreshold;

#define taskClass 0.0008

_LOCAL float TigClck;

_LOCAL plcbit KK_SetKIP_SATP;


_LOCAL TON_typ GainDly;
_LOCAL float GainDly_Tm;

_LOCAL HMI_typ * hmi;

_LOCAL float _PulseWidth , _PulseFreq;

UDINT _version;
_LOCAL TON_typ  SynchClckFromOsc_Tmr;
void Calculate_SynchClckFromOsc();

void _INIT AVC_SYNCHINIT(void)
{
	OSC_VelTreshold = 4.0;
	Set_Gain = 1;
	Set_AVC_BB_Gain = 1;

	KK_SetKIP_SATP = 1;

	GainDly_Tm = 0.05;
}

void _CYCLIC AVC_SYNCHCYCLIC(void)
{

	#ifdef MIG_PENDANT

		m = &Master[1];

	#else
		m = &Master[0];

	#endif	
		
	//m = &Master[0];	

	
	
	if(!m->pCurCfg)
		return;

	if(!m->pCalib)
		return;

	if(!m->Active)
		return;
	
	curCfg =  m->pCurCfg;	


	if(!m->pCfg)
		return;
	
	cfg =  m->pCfg;	

	hmi = &m->Hmi[0];

	TigSynchMode = curCfg->Param_ENUM[parENUM_TIG_Mode].val[0];
	AVCSynchMode = curCfg->Param_ENUM[parENUM_AVC_Mode].val[0];
	TVLSynchMode = curCfg->Param_ENUM[parENUM_TVL_SYN_MODE].val[0];	
	WFSynchMode = curCfg->Param_ENUM[parENUM_WF_SYN_MODE].val[0];
	WPSynchMode = curCfg->Param_ENUM[parENUM_WP_SYN_MODE].val[0];	
	
	PS_Param_Set[0].Mode = curCfg->Param_ENUM[parENUM_ARCLINK_MODE].val[0];

	_PulseFreq = (*PS_Param_Set[0].pPulseFreq);
	_PulseWidth = (*PS_Param_Set[0].pPulseWidth);


	if(m->pCalib->GenSetup[SETUP_HEAD_SETUP] == RUBIX)
		AVC_Mode = 3 ; // Bang Bang
		
	else if(m->pCalib->GenSetup[SETUP_AVC_LEGACY])
		AVC_Mode = 3 ; // Bang Bang
	else
		AVC_Mode = 0;; // Normal
	/**********************************************************/

	if(OSC_TML_Param_Set[0].Mode == 11)
		Calculate_SynchClckFromOsc();
	
	switch(TigSynchMode){

		default: // Straight

			SynchClckToTIG = 0;

		break;	

		case 1: // Pulse Arc	- it is own generator		

			if(!PS_Param_Set[0].pPulseFreq || !PS_Param_Set[0].pPulseWidth){
				
				SynchClckToTIG = 0;
				break;
			}

			if((*PS_Param_Set[0].pPulseFreq) >0){

				TigClck += taskClass * (*PS_Param_Set[0].pPulseFreq);
				
				if(TigClck >= 1)
					TigClck = 0.0;


				SynchClckToTIG = (TigClck >= (*PS_Param_Set[0].pPulseWidth)) ? 0 : 1;			
			
			}
			else
				SynchClckToTIG = 0;


		break;	

		case 2: // Synch Pulse


			if(SynchClckFromOsc) // ForeGround
				SynchClckToTIG = 1; // Primary Current
			else
				SynchClckToTIG = 0; // BackGround

		
		break;


		case 3: // Synch Pulse In Amp & Out Amp


			if(SynchClckFromOsc != 0) // ForeGround
				SynchClckToTIG = SynchClckFromOsc + 1; // Primary In or Primary Out
			else
				SynchClckToTIG = 0; // BackGround
		
		break;


	}
	/********************************************************************/

	switch(TVLSynchMode){

		default: // Straight

			SynchClckToTVL = 0;

		break;	

		
		case 1: // Synch Pulse


			if(SynchClckFromOsc) // 
				SynchClckToTVL = 1; // Foreground
			else
				SynchClckToTVL = 0; // BackGround

		
		break;


		case 2: // 


			if(SynchClckFromOsc != 0) // ForeGround
				SynchClckToTVL = SynchClckFromOsc + 1; // Primary In or Primary Out
			else
				SynchClckToTVL = 0; // BackGround
		
		break;


	}

	

	/********************************************************************/
	switch(WFSynchMode){

		default: // Straight

			SynchClckToWF = 0;

		break;	

		
		case 1: // Synch Pulse


			if(SynchClckFromOsc) // 
				SynchClckToWF = 1; // Foreground
			else
				SynchClckToWF = 0; // BackGround

		
		break;


		case 2: // 


			if(SynchClckFromOsc != 0) // ForeGround
				SynchClckToWF = SynchClckFromOsc + 1; // Primary In or Primary Out
			else
				SynchClckToWF = 0; // BackGround
		
		break;


	}

	/*********************************************************************/

	switch(WPSynchMode){

		default: // Straight

			SynchClckToWP = 0;

		break;	

		
		case 1: // Synch Pulse


			if(SynchClckFromOsc) // 
				SynchClckToWP = 1; // Foreground
			else
				SynchClckToWP = 0; // BackGround

		
		break;


		case 2: // 


			if(SynchClckFromOsc != 0) // ForeGround
				SynchClckToWP = SynchClckFromOsc + 1; // Primary In or Primary Out
			else
				SynchClckToWP = 0; // BackGround
		
		break;


	}

	/********************************************************************/

	
	
	switch(curCfg->Param_ENUM[parENUM_AVC_Mode].val[0]){
	
		default:  // Cont. - Foreground / Background 

			if(curCfg->Param_ENUM[parENUM_TIG_Mode].val[0] == 0) // Straight
				SynchClckToAVC = 1; // Only Foreground
			else
				SynchClckToAVC = SynchClckToTIG;
			
			Set_Gain = 1;
			Set_AVC_BB_Gain = 1;

		break;


		case 1:  // Background locked


			if(TigSynchMode == 0){ // Tig straight

				Set_Gain = 1;				
				Set_AVC_BB_Gain = 1;


			}else{
			

				if(SynchClckToTIG == 0){
					Set_Gain = 0;				
					Set_AVC_BB_Gain = 0;
					GainDly.IN = 0;
					
				}
				else {
					
					Set_Gain = GainDly.Q;
					Set_AVC_BB_Gain = 1;
					GainDly.IN = 1;				

				}

			}

				
			if(TigSynchMode == 3 ){ // Synch In/ Out

				if(SynchClckToTIG == 0)
					SynchClckToAVC = 4; // Average In and Out
				else
					SynchClckToAVC = SynchClckToTIG;
			}
			else
				SynchClckToAVC = SynchClckToTIG;  // Only ForeGround and BackGround					

		break;	


		case 2:  // Foreground locked


			if(SynchClckToTIG != 0){
				Set_Gain = 0;				
				Set_AVC_BB_Gain = 0;
			}
			else{ 
				Set_Gain = 1;				
				Set_AVC_BB_Gain = 1;
			}

			SynchClckToAVC = 0; // Only BackGround

		break;	


		case 3:  // Locked

			Process[PROC_AVC].Param[0] = 1;

			Set_Gain = 0;		
			Set_AVC_BB_Gain = 0;

			SynchClckToAVC = 0; // Only Foreground
			
		break;


	}

	//////////////////////////////////////////

	//if(cfg->HeadCfg.TIG.Arclink){
	//if(1){
	if(CurCalib.GenSetup[SETUP_HEAD_SETUP] != MIG_TIG_HEAD){	
		
		if(TML_MTR_INST[cfg->HeadCfg.AVC.TML_A_Indx]._int.Init){
			
			if(VectorStateMach[cfg->HeadCfg.AVC.VECT_A_Indx].xCMD == 0){

				if(!Set_Gain  && !SetZeroGain_Done){

					if(KK_SetKIP_SATP)
						VectorStateMach[cfg->HeadCfg.AVC.VECT_A_Indx].xCMD = VECT_Set_KPP_Zero;
						
					else	
						VectorStateMach[cfg->HeadCfg.AVC.VECT_A_Indx].xCMD = VECT_Set_SATP_Zero;
					
					SetZeroGain_Done = 1;
					SetGain_Done = 0;

				}		
				else if(Set_Gain && !SetGain_Done){
					
					if(KK_SetKIP_SATP)
						VectorStateMach[cfg->HeadCfg.AVC.VECT_A_Indx].xCMD = VECT_Set_KPP;
						
					else	
						VectorStateMach[cfg->HeadCfg.AVC.VECT_A_Indx].xCMD = VECT_Set_SATP;
					
					SetGain_Done = 1;
					SetZeroGain_Done = 0;
				}

			}
		}
		else{

			SetGain_Done = 0;
			SetZeroGain_Done = 0;


		}

	}
	else{

		SetZeroGain_Done = 1;
		SetGain_Done = 1;

	}

	////////////////////////////////////////////
	GainDly.PT  = (int) ((GainDly_Tm * 1000.0)+0.5);
	TON(&GainDly);
}


void Calculate_SynchClckFromOsc(){
	

	if(_version != AxisCh[TML_AX_OSC].Version){

		if(AxisCh[TML_AX_OSC].Version == 1){

			SynchClckFromOsc = 1;
			TimerStart(&SynchClckFromOsc_Tmr, (*OSC_TML_Param_Set[0].pDwell_In));

		}
		else{

			SynchClckFromOsc = 2;
			TimerStart(&SynchClckFromOsc_Tmr, (*OSC_TML_Param_Set[0].pDwell_Out));
		}

	}

	if(SynchClckFromOsc_Tmr.Q)
		SynchClckFromOsc = 0;

	
	TimerExecute(&SynchClckFromOsc_Tmr);
	_version = AxisCh[TML_AX_OSC].Version;

}

