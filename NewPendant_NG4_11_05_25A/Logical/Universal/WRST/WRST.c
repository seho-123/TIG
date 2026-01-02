
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>


#define ClassTime 0.0024

#define SETUP_POLE_ELEMENTS	17

typedef struct CAMSHELL_HMI_type
{	

	USINT JogFwd;
	USINT JogRev;
	USINT StepPos;
	USINT StepNeg;
	USINT SetZeroPos;
	USINT GoToPos;
	float TargetPos;
	float JogVel;
	float StepIncr;
	
	USINT step;

}CAMSHELL_HMI_type;

_GLOBAL CAMSHELL_HMI_type CAMSHELL_HMI;
_LOCAL AzzRamp_typ RampCMSL;

///////////////////////////////

_LOCAL GenProcess_typ* prc;
_LOCAL  StateVectorProxy_typ *vect;
_LOCAL TON_typ Tmr[WRST_NUM];

_LOCAL TON_typ AVC_FF_Tmr;


//_LOCAL Cfg_typ * cfg;
_LOCAL CurCfg_typ * curCfg;
//_LOCAL CurCalib_typ * curCalib, *defCalib;
//_LOCAL Scaled_Var_obj * var;
_LOCAL CurCalib_typ * curCalib;


_LOCAL MASTER_typ * m;

_LOCAL UINT Index;
_LOCAL float Value;
_LOCAL float AngleOffset[WRST_NUM] , MapAngleOffset[WRST_NUM];

USINT _StopEvaluateAngle[WRST_NUM];
DINT _MappingActive[WRST_NUM];

_LOCAL float TVLSpeedBoost[WRST_NUM] , TVLSpeedBoost_On_Radius[WRST_NUM] , AVC_Volt_FF;
_LOCAL float AVC_FF_Gain[WRST_NUM] , AVC_FF_T[WRST_NUM];
_LOCAL float TubeRadius;
_LOCAL float AVC_FF_FromMapping;
_LOCAL AzzMovAvgFlt_typ  filterAVC_Volt_FF;

_LOCAL  AzzMovAvgFlt_typ filterActRelAngle;

_LOCAL float ActTVLPos;
_LOCAL float TVL_POS_Offset;
_LOCAL float TVL_POS_Prediction;

_LOCAL plcbit StoreLastWrstPos;
_LOCAL plcbit FirstWrstPosDone;
_LOCAL plcbit TML_Wrst_GO;
_LOCAL UINT HMI_Angle_Btn_vis;
_LOCAL UINT HMI_Angle_Jog_Btn_vis;



_LOCAL plcbit _digInp[4];
_LOCAL UINT   HMI_digInp[4];
_LOCAL plcbit SwapHeadCmd;


float _AVC_FF_FromMapping;
_LOCAL UINT _toolHeadID;
_LOCAL plcbit _ESTOP;

float ValueFromMapping(UINT indx);
float GetCornerFromMapping(UINT indx);
float Get_AVC_FF_FromMapping(UINT indx);

void WorkAngleFromMapping();


UINT GetIndx();

void ClamShellOperations(CAMSHELL_HMI_type *hm , UINT select);
void ClamShell_Head(int i , UINT select);

void WristStandard_TML(int i);
void WristStandard_HMI(CAMSHELL_HMI_type *h , UINT select);





_LOCAL float slope;

_LOCAL float KK_extVoltSP_Fact;

_LOCAL float KK_Radius_Calib;


_LOCAL BOOL KK_Filter_ON;

_LOCAL BOOL KK_Test_Manual , KK_Test_Radius;


void _INIT WldAnglInit(void)
{

	
	KK_Test_Radius = 1;
	KK_Radius_Calib = 5.0;
			
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;
	Process[0].Disable = Process[0].Disable;
	
	TVL_ROT_Param_Set[0].TotalRescaleFactor = TVL_ROT_Param_Set[0].TotalRescaleFactor;
	
	WRST_TML_Param_Set[0].TVLSpeedOverride = 1;

	
	filterAVC_Volt_FF.base = 200;
	filterAVC_Volt_FF.enable = 1;
	AzzMovAvgFlt(&filterAVC_Volt_FF);

	filterActRelAngle.base = 100;
	filterActRelAngle.enable = 1;
	//AzzMovAvgFlt(&filterActRelAngle);
	
	RampCMSL.y_max = 360.0;
	RampCMSL.y_min = -360.0;
	RampCMSL.dy_up = 1.0;
	RampCMSL.dy_down = 1.0;
	RampCMSL.enable = 1;
	


	
	CAMSHELL_HMI.JogVel = 12.0; // Volts
	CAMSHELL_HMI.StepIncr = 1.0;
	StoreLastWrstPos = 0;
	FirstWrstPosDone = 0;
	_toolHeadID = 123;

	TimerStart(&AVC_FF_Tmr, 0.1);

}

void _CYCLIC WldAnglCyclic(void)
{int i;
	float x;

		
	curCfg				= &CurCfg;
	curCalib			=  &CurCalib;
	//defCalib			= (CurCalib_typ *)	m->pDefaultCalib;


	switch(WRST_TML_Param_Set[0].Mode){


		case 9: // One Lag Angle BPV2 Video

			////////////////////////////////////////////////
			if(SwapHeadCmd){
				SwapHeadCmd = 0;
				
				if (Master[0].SeqStep == 0){ // No Head Changes on run				 

					if (Master[0].ToolHeadID != 0)
						Master[0].ToolHeadID = 0;
					else 
						Master[0].ToolHeadID = 1;

					if(CurCalib.GenSetup[13]){ //SETUP_TVL_ENC_SET - 13
						if (TVL_ROT_Param_Set[0].pEncoder)
							OLD_MTR_POS[2] = *TVL_ROT_Param_Set[0].pEncoder; // TVL_ENC_HOME - 2				

						Master[0].SubSeqStep = 1;
					}

				}	
				
			}

			////////////////////////////////////
			if(_toolHeadID != Master[0].ToolHeadID){
				StoreLastWrstPos = 0;
				FirstWrstPosDone = 0;
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;	

			}
			_toolHeadID = Master[0].ToolHeadID;

			///////////////

			//if(Master[0].sm.Step == 50){

				if(_ESTOP != Master[0].ESTOP){
					StoreLastWrstPos = 0;
					FirstWrstPosDone = 0;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;	

				}
				
				_ESTOP = Master[0].ESTOP;

			//}
			/////////////////////////////////////////////////

			WristStandard_HMI(&CAMSHELL_HMI, Master[0].ToolHeadID);
			WristStandard_TML(0);	


		break;
///////////////////////////////////////////////////////////////////////////////////////////////

		case 10: //  ClamShell Head

			CAMSHELL_HMI.JogFwd = TML_MTR_INST[6].DigInp[0]; // TML_AX_WRST
			CAMSHELL_HMI.JogRev = TML_MTR_INST[6].DigInp[1]; // TML_AX_WRST

			////////////////////////////////////////////////////////////////////

			if(TML_MTR_INST[2]._int.Init
				&& (Master[0].sm.Step == 50)){
				
					TML_Wrst_GO = 1;

					/*
					if(Process[10]._step == 1){ //PROC_WRST - 10
					
						if(Master[0].ToolHeadID == 1){
							if(TML_MTR_INST[2].DigInp[0] && !_digInp[0])
								SwapHeadCmd = 1;						

						}else{
						
							if(TML_MTR_INST[2].DigInp[1] && !_digInp[1])
								SwapHeadCmd = 1;
							
						}
					}
					*/

			}
			else
				SwapHeadCmd = 0;

			_digInp[0] = TML_MTR_INST[6].DigInp[0]; 
			_digInp[1] = TML_MTR_INST[6].DigInp[1];

			HMI_digInp[0] = (TML_MTR_INST[6].DigInp[0])? 1:0;
			HMI_digInp[1] = (TML_MTR_INST[6].DigInp[1])? 1:0;
			//HMI_digInp[0] = HMI_digInp[0];
			//HMI_digInp[1] = HMI_digInp[1];
				
			////////////////////////////////////////////////
			if(SwapHeadCmd){
				SwapHeadCmd = 0;
				
				if (Master[0].SeqStep == 0){ // No Head Changes on run				 

					if (Master[0].ToolHeadID != 0)
						Master[0].ToolHeadID = 0;
					else 
						Master[0].ToolHeadID = 1;

					if(CurCalib.GenSetup[13]){ //SETUP_TVL_ENC_SET - 13
						if (TVL_ROT_Param_Set[0].pEncoder)
							OLD_MTR_POS[2] = *TVL_ROT_Param_Set[0].pEncoder; // TVL_ENC_HOME - 2				

						Master[0].SubSeqStep = 1;
					}

				}	

				/////////////////////////////
				
				//StoreLastWrstPos = 0;
				//FirstWrstPosDone = 1;		
				
				//FirstWrstPosDone = 0;
				//Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;
					/////////////////////////////

				
			}

			////////////////////////////////////
			if(_toolHeadID != Master[0].ToolHeadID){
				StoreLastWrstPos = 0;
				FirstWrstPosDone = 0;
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;	

			}
			_toolHeadID = Master[0].ToolHeadID;

			///////////////

			//if(Master[0].sm.Step == 50){

				if(_ESTOP != Master[0].ESTOP){
					StoreLastWrstPos = 0;
					FirstWrstPosDone = 0;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;	

				}
				
				_ESTOP = Master[0].ESTOP;

			//}
			/////////////////////////////////////////////////
			
			ClamShellOperations(&CAMSHELL_HMI, Master[0].ToolHeadID);
			ClamShell_Head(0 ,  Master[0].ToolHeadID);

		break;
		//////////////////////////////////////////////////////////////		


		default:

			WorkAngleFromMapping();


		break;


	}

	////////////////////////////////////////////////


	
	

}

float ValueFromMapping(UINT indx){
UINT i , k;
float x ,y;
//float slope;

	if(!curCalib)
		return 0.0;
	
	if(indx < 64)
		i = indx;
	else
		i = 63;
	k = curCalib->GenSetup[SETUP_POLE_ELEMENTS]* 2 - 1;

	if(ActTVLPos >= curCalib->Map[k].x){
		y = curCalib->Map[k].angle;
	}
	else if(ActTVLPos <= curCalib->Map[0].x){
		y = curCalib->Map[0].angle;
	}
	else{

		x = curCalib->Map[i+1].x - curCalib->Map[i].x;
		if(x != 0.0)
			slope = (curCalib->Map[i+1].angle - curCalib->Map[i].angle)/x;
		else
			slope = 0.0;
		
		y = (ActTVLPos - curCalib->Map[i].x) * slope + curCalib->Map[i].angle ;

		//y = (ActTVLPos - curCalib->Map[i].x) * slope;
		//y -= (x/2.0) * slope;
	}
	
	return y;
}
float GetCornerFromMapping(UINT indx){

UINT i;// , k;
float x ,y;

	
	if(!curCalib)
		return 1.0;
		
	if(indx < 64)
		i = indx;
	else
		i = 63;
	
	//k = curCalib->GenSetup[SETUP_POLE_ELEMENTS]* 2 - 1;

	if(indx%2){

		x =  fabsf(curCalib->Map[i+1].angle - curCalib->Map[i].angle) / 360.0 * TubeRadius * M_PI * 2.0 ;
		y = fabsf(curCalib->Map[i+1].x - curCalib->Map[i].x);

		if( y > 0 && x > 0)
			return x/y;
		else
			return 1.0; 
	}
	else
		return 1.0;

}

UINT GetIndx(){
UINT i , k;

	if(!curCalib)
		return 0;

	i = 0;
	//k = curCalib->GenSetup[SETUP_POLE_ELEMENTS]* 2 - 1;
	k = curCalib->GenSetup[SETUP_POLE_ELEMENTS]* 2;
	
	while((ActTVLPos > curCalib->Map[i].x )
		&& (i < k)){
		i++;
	}
	if( i > 0)
		i--;
	
	return i;
}

float Get_AVC_FF_FromMapping(UINT indx){

UINT i , k;
float x ,y;
//float slope;

	if(!curCalib)
		return 0.0;
	
	if(indx < 64)
		i = indx;
	else
		i = 63;
	k = curCalib->GenSetup[SETUP_POLE_ELEMENTS]* 2 - 1;

	
	if(ActTVLPos >= curCalib->Map[k].x){
		//y = curCalib->Map[k].angle;
		y = 0;
	}
	else if(ActTVLPos <= curCalib->Map[0].x){
		//y = curCalib->Map[0].angle;
		y = 0;
	}
	else{

		x = curCalib->Map[i+1].x - curCalib->Map[i].x;
		if(x != 0.0)
			slope = (curCalib->Map[i+1].angle - curCalib->Map[i].angle)/x;
		else
			slope = 0.0;
		
		//y = (ActTVLPos - curCalib->Map[i].x) * slope + curCalib->Map[i].angle ;

		//y -= (curCalib->Map[i+1].angle - curCalib->Map[i].angle)/2.0;


		y = (ActTVLPos - curCalib->Map[i].x) * slope;
		y -= (x/2.0) * slope;

		
	}
	

	return y;

}

void ClamShellOperations(CAMSHELL_HMI_type *h , UINT select){
float dir;

	if(!Master[0].pCfg)
		return;

	if(!prc)
		return;

	dir = (select)? 1.0:-1.0;

	HMI_Angle_Jog_Btn_vis = 1; // INVISIBLE
	if(StoreLastWrstPos && FirstWrstPosDone){

		HMI_Angle_Btn_vis = 0;// VISIBLE;
		
		if(Master[0].ToolHeadID == 0){ //
			OLD_MTR_POS[7] = AxisCh[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].curPos; // WRST1_LAST_POS
		}
		else{
			OLD_MTR_POS[8] = AxisCh[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].curPos; // WRST2_LAST_POS
			
		}

	}
	else
		HMI_Angle_Btn_vis = 1; //INVISIBLE;
		
	////////////////////////////////////////////////

	if(!TML_Wrst_GO){

		h->SetZeroPos = 0;
		h->GoToPos = 0;

		h->JogFwd = 0;
		h->JogRev = 0;

		h->StepPos = 0;
		h->StepNeg = 0;

		Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = 0;
		
	}

	if(!Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD
		&& FirstWrstPosDone){	
		
		if(h->SetZeroPos){
			h->SetZeroPos =0;
			
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absLastPos = 0; //OLD_MTR_POS[INDX_B_LAST_POS];
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absFbk = 2;
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = 1;
			
			Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;

			StoreLastWrstPos = 0;
			FirstWrstPosDone = 1;
			
		}else if(h->GoToPos){
			h->GoToPos = 0;

			if(TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._int.kPos != 0){
				
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].Param[0] = (DINT)(h->TargetPos *dir * TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._int.kPos);
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsGoToPos;
			}
			/*
			RampCMSL.y_set = AngleOffset[0];
			RampCMSL.set = 1;
			RampCMSL.x = h->TargetPos;
			AzzRamp(&RampCMSL);		
			RampCMSL.set = 0;	
			*/
				

		}
		else if(1){

				if(h->JogFwd){
					//h->JogFwd = 0;

					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = h->JogVel;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
				}
				else if(h->JogRev){ 
					//h->JogRev = 0;

					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = -h->JogVel;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
				}
				else if(h->StepPos){ 
					h->StepPos = 0;
				
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] = h->StepIncr;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsSTEPpos;

				
				}
				else if(h->StepNeg){ 
					h->StepNeg = 0;
				
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] = -h->StepIncr;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsSTEPpos;		
				}
		}
		
	}


	h->TargetPos = WRST_TML_Param_Set[0].actAngle;

	/*
	if(!RampCMSL.x_reached){
		h->TargetPos = RampCMSL.x;	
		AngleOffset[0] = RampCMSL.y;
	}
	else
		h->TargetPos = AngleOffset[0];

	AzzRamp(&RampCMSL);
	*/
	
}

void ClamShell_Head(int i , UINT select){		
float dir;

		if(!Master[0].pCfg)
				return;


		if(!WRST_TML_Param_Set[i].pVect)
			return;	

		if(!WRST_TML_Param_Set[i].pPrc)
			return;

		vect = WRST_TML_Param_Set[i].pVect;
		prc =  WRST_TML_Param_Set[i].pPrc;		
		
		if(prc->ConfigReady <= 0){
			prc->xCMD = 0;
			return;	
		}	

		dir = (select)? 1.0:-1.0;	
			
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
					break;

				case prcsRESTART:
				case prcsHalt:
				case prcsQuickSTOP:
				case prcsSTOP:

					prc->Status = prcsSTATUS_Stopped;
					
					break;


				default:

					if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
						prc->Status = prcsSTATUS_Stopped;
					
					break;	
				


			}

		}else{
	
		
	
		///////////////////////////////////
		switch(prc->xCMD){


			case prcsHalt:
			case prcsRESTART:

				prc->_step = 0;
				
			break;	

			
			case prcsSTART:


				
			break;


			case prcsJump2Run:

				
			

			break;	
			
			case prcsSTOP:

				

			break;

			case prcsQuickSTOP:
			
				

			break;	

			case prcsGoToPos:

				if(prc->_step < 80){
					prc->_step = 80;
					vect->Param[0] =  prc->Param[0];

				}
				

			break;

			
			case prcsSTEPpos:
			case prcsSTEPneg:

				if(prc->_step < 70){
					prc->_step = 70;

					vect->fParam[2] = Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] * dir;

	

				}



				
				/*
				if (prc->_step == 2){

					AngleOffset[i] += prc->fParam[0];
					
				}
				//prc->fParam[0] = 0;
				*/
				
				
			break;

			case prcsJOGpos:
			case prcsJOGneg:

			
				//if(prc->_step != 61)
				if(prc->_step == 1)
					prc->_step = 61;

			break;
			
			
			case prcsDirectCntrl:
				
				/*
				if(prc->_step != 60)
					prc->_step = 60;
					*/

				//if(prc->_step != 61)
				if(prc->_step == 1)
					prc->_step = 61;

			break;

			

			
			}

		
		}
			
		prc->xCMD = 0;
	
			
		switch(prc->_step){


			case 0:
				
				prc->Status = prcsSTATUS_Init;	


				vect->InVal = 0.0;

				if((vect->xCmd_busy || vect->xCMD))
					break;

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){

					vect->Mode = vectSt_Mode_Volt;
				}
				else if(!TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD){

					if(!TML_Wrst_GO){;}


					else if(!FirstWrstPosDone){		
						

						if(select)
							TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = TML_SET_SCR_2;
						else
							TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = TML_SET_SCR_1;

						
						 vect->xCMD = 0;
						
						TimerStart(&Tmr[i], 0.1);

						prc->_step = 7;


					}

					else if(StoreLastWrstPos){

						if(select)
							TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = TML_SET_SCR_2;
						else
							TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = TML_SET_SCR_1;

						 vect->xCMD = 0;
						
						TimerStart(&Tmr[i], 0.1);
						
						prc->_step = 1;	

					}else{

						if(!(vect->xCmd_busy || vect->xCMD)){	

							/*
							if(select)
								vect->xCMD = VECT_Set_OUT0;	
							else
								vect->xCMD = VECT_Rst_OUT0;
							*/	

							TimerStart(&Tmr[i], 0.1);
							prc->_step = 8;

						}

					}

				}
				
														
			break;


			case 7:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;

				if(!(vect->xCmd_busy || vect->xCMD)){

					if(Master[0].ToolHeadID == 0)
						TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absLastPos = OLD_MTR_POS[7]; // WRST1_LAST_POS
					
					else
						TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absLastPos = OLD_MTR_POS[8]; // WRST2_LAST_POS
			
					TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absFbk = 2;
					TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = 1;

					 vect->xCMD = 0;

					TimerStart(&Tmr[i], 0.1);
					prc->_step ++;
				}

				


			break;

			
			case 8:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
			
				if(!(vect->xCmd_busy || vect->xCMD)){

					if(select)
						vect->xCMD = VECT_Set_OUT0; 
					else
						vect->xCMD = VECT_Rst_OUT0;
					
					
					prc->_step++;

				}
				

			break;

			case 9:

				if(!(vect->xCmd_busy || vect->xCMD)){	

					if(select)
						vect->xCMD = VECT_Set_Fbk_2;
					else
						vect->xCMD = VECT_Set_Fbk_1;
					

					TimerStart(&Tmr[i], 1.0);
					prc->_step = 10;



				}

			break;


			case 10:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				StoreLastWrstPos = 1;
				FirstWrstPosDone = 1;
				
				prc->_step = 0;

			break;

			
			case 1:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				vect->InVal = 0.0;
				
				
			break;
				
			/*
			case 2:

				prc->Status = prcsSTATUS_Run;	

				if(vect->FbkVect.Mode == vectSt_Mode_PT)	
					vect->InVal = AngleOffset[i] + WRST_TML_Param_Set[i].actAngleFromMapping - MapAngleOffset[i];
				else
					prc->_step = 0;
				

			break;
			*/

			case 60: /// Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;

				

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}
				else{

					if(WRST_TML_Param_Set[i].pJogSpeedDirCntrl){ 

												
						if(prc->fParam[0] != 0){
							vect->InVal = (*WRST_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] *	WRST_TML_Param_Set[i].GlbDirection * dir;
						}else {	

							vect->InVal = 0.0;
							prc->_step = 0;

						}
						prc->fParam[1] = 0.0;
							
					}						

				}	

			break;		

			case 61: /// Speed Mode Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;

				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{

					if(WRST_TML_Param_Set[i].pJogSpeedDirCntrl){ 

												
						if(prc->fParam[0] != 0.0)
							vect->InVal = (*WRST_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] *	WRST_TML_Param_Set[i].GlbDirection * dir ;
						else {	

							vect->InVal = 0.0;
							prc->_step = 0;
							
						}
						prc->fParam[1] = 0.0;
						
							
					}						

				}										

			break;	


			case 70:


				vect->fParam[0] =  10.0; // ACC
				vect->fParam[1] =  100.0;
				//vect->fParam[2] = 1;

				//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

				TimerStart(&Tmr[i], 1.0); 
				vect->xCMD = VECT_Move_Rel_Cmd;	

				prc->_step = 100;


			break;
			
			case 80:


				vect->fParam[0] =  10.0; // ACC
				vect->fParam[1] =  100.0;
				//vect->Param[0] =  (DINT)(1024);

				TimerStart(&Tmr[i], 1.5);
				vect->xCMD = VECT_Move_Abs_Cmd;

				prc->_step = 100;


			break;

			
			case 100:

				//if(!TimerIsTimeOut(&Tmr[i]))
					//break;

				if(!(vect->StatusReg & 0x00000400)){ // movement started
										
					prc->_step++;

				}

			break;	

			case 101:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;

				if((vect->StatusReg & 0x00000400)){ // movement stopped

					if(!(vect->xCmd_busy || vect->xCMD)){	
						vect->xCMD = VECT_Refresh_Mode;
						prc->_step = 0;
					}

				}

			break;	



			case 102:

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){

					vect->Mode = vectSt_Mode_Volt;
				}
				else
					prc->_step = 0;
				
			break;
			
			
		}
	
			WRST_TML_Param_Set[i].actAngle =  vect->FbkVect.x * dir;
	
			_MappingActive[i] = WRST_TML_Param_Set[i].MappingActive;
			_StopEvaluateAngle[i] = WRST_TML_Param_Set[i].StopEvaluateAngle;
			
			TimerExecute(&Tmr[i]);	

			/*

			vect->fParam[0] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog; // ACC
					vect->fParam[1] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog;
					vect->fParam[2] = (*WIRE_Param_Set[i].pRetracDist) * (-1.0);

					//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

					
					vect->xCMD = VECT_Move_Rel_Cmd;	


					vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
						vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
												
						vect->Param[0] =  (DINT)(((float)prc->Param[3] + 0.25) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					
						vect->xCMD = VECT_Move_Abs_Cmd;

			*/			
	
			
}


void WorkAngleFromMapping(){
int i;
float x;



	if(!(WRST_TML_Param_Set[0].pTVL_Pos_Predict && WRST_TML_Param_Set[0].pTVL_Pos_Offset 
		&& WRST_TML_Param_Set[0].pRadSpeed_Boost && WRST_TML_Param_Set[0].pRadius))
		return;

	TVL_POS_Prediction = (*WRST_TML_Param_Set[0].pTVL_Pos_Predict);
	TVL_POS_Offset = (*WRST_TML_Param_Set[0].pTVL_Pos_Offset);
	TubeRadius = (*WRST_TML_Param_Set[0].pRadius);
	
	TVLSpeedBoost[0] = (*WRST_TML_Param_Set[0].pRadSpeed_Boost);
		
	if(TVLSpeedBoost[0] > 0)
		TVLSpeedBoost[0] = 1.0/TVLSpeedBoost[0];
	else
		TVLSpeedBoost[0] = 1.0;
	
	if(!KK_Test_Manual)
		ActTVLPos = ABS_TVL_Pos + TVL_POS_Offset + TVL_POS_Prediction  * Process[0].Fbk[0]/25.4 * 32.0;
		//ActTVLPos = ABS_TVL_Pos + TVL_POS_Prediction  * Process[Cfg.HeadCfg.TIG.PROC_Indx].Fbk[0]/25.4 * 32.0;

	
	
	for(i=0 ; i < WRST_NUM ; i++){ 

		

		if(!WRST_TML_Param_Set[i].pVect)
			break;	

		if(!WRST_TML_Param_Set[i].pPrc)
			break;

		vect = WRST_TML_Param_Set[i].pVect;
		prc =  WRST_TML_Param_Set[i].pPrc;		
		
		if(prc->ConfigReady <= 0)
			continue;	

		
		
		if(!WRST_TML_Param_Set[i].StopEvaluateAngle && WRST_TML_Param_Set[i].MappingActive){
		
			Index = GetIndx();

			if(!KK_Test_Manual)
				WRST_TML_Param_Set[i].actIndx = Index;
			
			WRST_TML_Param_Set[i].actAngleFromMapping = ValueFromMapping(WRST_TML_Param_Set[i].actIndx);
			
			TVLSpeedBoost_On_Radius[i] = GetCornerFromMapping(WRST_TML_Param_Set[i].actIndx) ;

			if(KK_Test_Radius)
				WRST_TML_Param_Set[i].TVLSpeedOverride = (Index%2 > 0)? TVLSpeedBoost[i] * TVLSpeedBoost_On_Radius[i] * KK_Radius_Calib : cosf(WRST_TML_Param_Set[i].actAngleFromMapping/180.0 * M_PI);
			else
				WRST_TML_Param_Set[i].TVLSpeedOverride = cosf(WRST_TML_Param_Set[i].actAngleFromMapping/180.0 * M_PI);
		}
		else		
			WRST_TML_Param_Set[i].TVLSpeedOverride = 1; 	
		

		if(WRST_TML_Param_Set[i].MappingActive && !_MappingActive[i])
			prc->_step = 0;

		if(WRST_TML_Param_Set[i].StopEvaluateAngle && !_StopEvaluateAngle[i])
	 		prc->_step = 0;

		
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
					break;

				case prcsRESTART:
				case prcsHalt:
				case prcsQuickSTOP:
				case prcsSTOP:

					prc->Status = prcsSTATUS_Stopped;
					
					break;


				default:

					if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
						prc->Status = prcsSTATUS_Stopped;
					
					break;	
				


			}

		}else{

	

			///////////////////////////////////
			switch(prc->xCMD){


				case prcsHalt:
				case prcsRESTART:

					prc->_step = 0;
					
				break;	

				
				case prcsSTART:


					
				break;


				case prcsJump2Run:

					
				

				break;	
				
				case prcsSTOP:

					

				break;

				case prcsQuickSTOP:
				
					

				break;				

				case prcsJOGpos:

					if (prc->_step == 2){

						AngleOffset[i] += prc->fParam[0];
						
					}
					//prc->fParam[0] = 0;
					
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}
		}
		
		prc->xCMD = 0;

		
		switch(prc->_step){


			case 0:
				
				
				prc->Status = prcsSTATUS_Init;	
			
				if(vect->FbkVect.Mode != vectSt_Mode_Idle){

					vect->Mode = vectSt_Mode_Idle;
				}
				else{
					TimerStart(&Tmr[i], 2.0);
					
					prc->_step++;		

				}
				
														
			break;

			case 1:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				
				vect->InVal = vect->FbkVect.x;		
				
				if(vect->FbkVect.Mode == vectSt_Mode_PT){	
						
					AngleOffset[i] = vect->FbkVect.x;
					MapAngleOffset[i] = WRST_TML_Param_Set[i].actAngleFromMapping;	

					CAMSHELL_HMI.TargetPos = AngleOffset[i];
					
					prc->_step ++;	
					
					
				}
				else
					vect->Mode = vectSt_Mode_PT;
				
			break;	

			case 2:

				prc->Status = prcsSTATUS_Run;	

				if(vect->FbkVect.Mode == vectSt_Mode_PT)	
					vect->InVal = AngleOffset[i] + WRST_TML_Param_Set[i].actAngleFromMapping - MapAngleOffset[i];
				else
					prc->_step = 0;
				

			break;


			case 60: /// Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;

				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}	
				else{

					if(WRST_TML_Param_Set[i].pJogSpeedDirCntrl){ 

												
						if(prc->fParam[0] != 0)
							vect->InVal = (*WRST_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] *  WRST_TML_Param_Set[i].GlbDirection ;
						else {	

							vect->InVal = 0.0;
							prc->_step = 0;

						}
							
					}						

				}										

			break;				
			
		}

		WRST_TML_Param_Set[i].actAngle =  vect->FbkVect.x;

		_MappingActive[i] = WRST_TML_Param_Set[i].MappingActive;
		_StopEvaluateAngle[i] = WRST_TML_Param_Set[i].StopEvaluateAngle;
		
		TimerExecute(&Tmr[i]);	
		

		
	}

		
	/////////////////////////////////////////////////


}

void WristStandard_HMI(CAMSHELL_HMI_type *h , UINT select){
float dir;

	if(!Master[0].pCfg)
		return;

	if(!prc)
		return;

	dir = 1.0;


	if((Master[0].sm.Step >= 50)
		&& (Master[0].sm.Step < 100)){

		
		if(TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD){
			
			HMI_Angle_Btn_vis = 1;// INVISIBLE;
			HMI_Angle_Jog_Btn_vis = 1; // INVISIBLE;
			
		}else{	
	
			HMI_Angle_Btn_vis = 0;// VISIBLE;
			HMI_Angle_Jog_Btn_vis = 0; // VISIBLE;
		}

	}else{


		if(TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD)
		HMI_Angle_Btn_vis = 1;// INVISIBLE;
		HMI_Angle_Jog_Btn_vis = 1; // INVISIBLE;

	}				
	////////////////////////////////////////////////

	if(!TML_Wrst_GO){
		TML_Wrst_GO = 1;

		h->SetZeroPos = 0;
		h->GoToPos = 0;

		h->JogFwd = 0;
		h->JogRev = 0;

		h->StepPos = 0;
		h->StepNeg = 0;

		Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = 0;
		
	}

	if(!Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD
		//&& FirstWrstPosDone){
		&& 1){
		
		if(h->SetZeroPos){
			h->SetZeroPos =0;
			
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absLastPos = 0; //OLD_MTR_POS[INDX_B_LAST_POS];
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absFbk = 2;
			TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = 1;
			
			Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx]._step = 0;

			//StoreLastWrstPos = 0;
			//FirstWrstPosDone = 1;
			
		}else if(h->GoToPos){
			h->GoToPos = 0;

			if(TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._int.kPos != 0){
				
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].Param[0] = (DINT)(h->TargetPos *dir * TML_MTR_INST[Master[0].pCfg->HeadCfg.WRST.TML_A_Indx]._int.kPos);
				Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsGoToPos;
			}		

		}
		else if(1){

				if(h->JogFwd){
					//h->JogFwd = 0;

					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = h->JogVel;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
				}
				else if(h->JogRev){ 
					//h->JogRev = 0;

					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[0] = -h->JogVel;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsJOGpos;
				}
				else if(h->StepPos){ 
					h->StepPos = 0;
				
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] = h->StepIncr;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsSTEPpos;

				
				}
				else if(h->StepNeg){ 
					h->StepNeg = 0;
				
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] = -h->StepIncr;
					Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].xCMD = prcsSTEPpos;		
				}
		}
		
	}


	h->TargetPos = WRST_TML_Param_Set[0].actAngle;

	WRST_TML_Param_Set[0].actAngle =  vect->FbkVect.x * dir ;
	
}

void WristStandard_TML(int i){		
float dir;

		if(!Master[0].pCfg)
				return;


		if(!WRST_TML_Param_Set[i].pVect)
			return;	

		if(!WRST_TML_Param_Set[i].pPrc)
			return;

		vect = WRST_TML_Param_Set[i].pVect;
		prc =  WRST_TML_Param_Set[i].pPrc;		
		
		if(prc->ConfigReady <= 0){
			prc->xCMD = 0;
			return;	
		}	

		//dir = (select)? 1.0:-1.0;	
		dir = 1.0;
			
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
					break;

				case prcsRESTART:
				case prcsHalt:
				case prcsQuickSTOP:
				case prcsSTOP:

					prc->Status = prcsSTATUS_Stopped;
					
					break;


				default:

					if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
						prc->Status = prcsSTATUS_Stopped;
					
					break;	
				


			}

		}else{
	
		
	
		///////////////////////////////////
		switch(prc->xCMD){


			case prcsHalt:
			case prcsRESTART:

				prc->_step = 0;
				
			break;	

			
			case prcsSTART:


				
			break;


			case prcsJump2Run:

				
			

			break;	
			
			case prcsSTOP:

				

			break;

			case prcsQuickSTOP:
			
				

			break;	

			case prcsGoToPos:

				if(prc->_step < 80){
					prc->_step = 80;
					vect->Param[0] =  prc->Param[0];

				}
				

			break;

			
			case prcsSTEPpos:
			case prcsSTEPneg:

				if(prc->_step < 70){
					prc->_step = 70;

					vect->fParam[2] = Process[Master[0].pCfg->HeadCfg.WRST.PROC_Indx].fParam[2] * dir;

	

				}



				
				/*
				if (prc->_step == 2){

					AngleOffset[i] += prc->fParam[0];
					
				}
				//prc->fParam[0] = 0;
				*/
				
				
			break;

			case prcsJOGpos:
			case prcsJOGneg:

			
				//if(prc->_step != 61)
				if(prc->_step == 1)
					prc->_step = 61;

			break;
			
			
			case prcsDirectCntrl:
				
				/*
				if(prc->_step != 60)
					prc->_step = 60;
					*/

				//if(prc->_step != 61)
				if(prc->_step == 1)
					prc->_step = 61;

			break;

			

			
			}

		
		}
			
		prc->xCMD = 0;
	
			
		switch(prc->_step){


			case 0:
				
				prc->Status = prcsSTATUS_Init;	


				vect->InVal = 0.0;

				if((vect->xCmd_busy || vect->xCMD))
					break;

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){

					vect->Mode = vectSt_Mode_Volt;
				}
				else if(!(vect->xCmd_busy || vect->xCMD))
					prc->_step = 1;
														
			break;	

			case 1:


			break;
			

			case 60: /// Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;

				

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}
				else{

					if(WRST_TML_Param_Set[i].pJogSpeedDirCntrl){ 

												
						if(prc->fParam[0] != 0){
							vect->InVal = (*WRST_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] *	WRST_TML_Param_Set[i].GlbDirection * dir;
						}else {	

							vect->InVal = 0.0;
							prc->_step = 0;

						}
						prc->fParam[1] = 0.0;
							
					}						

				}	

			break;		

			case 61: /// Speed Mode Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;

				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{

					if(WRST_TML_Param_Set[i].pJogSpeedDirCntrl){ 

												
						if(prc->fParam[0] != 0.0)
							vect->InVal = (*WRST_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] *	WRST_TML_Param_Set[i].GlbDirection * dir ;
						else {	

							vect->InVal = 0.0;
							prc->_step = 0;
							
						}
						prc->fParam[1] = 0.0;
						
							
					}						

				}										

			break;	


			case 70:


				vect->fParam[0] =  10.0; // ACC
				vect->fParam[1] =  100.0;
				//vect->fParam[2] = 1;

				//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

				TimerStart(&Tmr[i], 2.0); 
				vect->xCMD = VECT_Move_Rel_Cmd;	

				prc->_step = 100;


			break;
			
			case 80:


				vect->fParam[0] =  10.0; // ACC
				vect->fParam[1] =  100.0;
				//vect->Param[0] =  (DINT)(1024);

				TimerStart(&Tmr[i], 5.0);
				vect->xCMD = VECT_Move_Abs_Cmd;

				prc->_step = 100;


			break;

			
			case 100:

				//if(!TimerIsTimeOut(&Tmr[i]))
					//break;

				if(!(vect->StatusReg & 0x00000400)){ // movement started

					//TimerStart(&Tmr[i], 1.0);
										
					prc->_step++;

				}

			break;	

			case 101:

				
				if((vect->StatusReg & 0x00000400)
					|| TimerIsTimeOut(&Tmr[i])){ // movement stopped

					if(!(vect->xCmd_busy || vect->xCMD)){	
						vect->xCMD = VECT_Refresh_Mode;
						prc->_step = 0;
					}

				}
				

			break;	



			case 102:

				if(vect->FbkVect.Mode != vectSt_Mode_Volt){

					vect->Mode = vectSt_Mode_Volt;
				}
				else
					prc->_step = 0;
				
			break;
			
			
		}
	
			WRST_TML_Param_Set[i].actAngle =  vect->FbkVect.x * dir;
	
			_MappingActive[i] = WRST_TML_Param_Set[i].MappingActive;
			_StopEvaluateAngle[i] = WRST_TML_Param_Set[i].StopEvaluateAngle;
			
			TimerExecute(&Tmr[i]);	

			/*

			vect->fParam[0] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog; // ACC
					vect->fParam[1] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog;
					vect->fParam[2] = (*WIRE_Param_Set[i].pRetracDist) * (-1.0);

					//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

					
					vect->xCMD = VECT_Move_Rel_Cmd;	


					vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
						vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
												
						vect->Param[0] =  (DINT)(((float)prc->Param[3] + 0.25) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					
						vect->xCMD = VECT_Move_Abs_Cmd;

			*/			
	
			
}



