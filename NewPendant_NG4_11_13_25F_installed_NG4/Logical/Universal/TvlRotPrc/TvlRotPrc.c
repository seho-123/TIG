/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: TVL_ROT
 * File: TVL_ROT.c
 * Author: ckaczkowski
 * Created: October 29, 2013
 * ********************************************************
 *  prc->Param[0] == -> Absolute Target Position to Go

 *  prc->Param[1] - &0x1== 0 -> Direction Fwd
 *  prc->Param[1] - &0x1== 1 -> Direction Rev
	
 
 *  prc->Param[2] == 0 ->  not Auto Stop and Auto Rewrap
 *  prc->Param[2] == 1 ->  Auto Stop and Auto Rewrap
 *  prc->Param[2] == 2 ->  only Rewrap
 * 	prc->Param[2] == 3 ->  for Continuous Run - just information about end 
 *	prc->Param[2] == 4 ->  for Continuous Run - just information about end ,  Take Ramp from Cont_Ramp

 *  prc->Param[3] == -> Absolute return Position 

 * TVL_ROT_Param_Set[i].Extras&0x2) - // TVL Rev Rampdown	
 ********************************************************************
 * Implementation of program TVL_ROT
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\FourAxisTigCtrl\GlobDef.h>

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>
#include <..\..\Lib\LoggerLib.c>


#include <astime.h>

							

_LOCAL TON_typ Tmr[TVL_NUM +ROT_NUM];
_LOCAL GenProcess_typ* prc;
_LOCAL AzzRamp_typ Ramp[TVL_NUM +ROT_NUM];
_LOCAL StateVectorProxy_typ*  vect;
//_LOCAL HCRMovePara_typ  MovePara[TVL_NUM +ROT_NUM];
_LOCAL DINT actDistInTicks[TVL_NUM +ROT_NUM] , targetDistInTicks[TVL_NUM +ROT_NUM] ;
_LOCAL DINT	actDistInTicks_Catch_0[TVL_NUM +ROT_NUM] ,  actDistInTicks_Catch[TVL_NUM +ROT_NUM];
_LOCAL float KK_CalculatedDist[TVL_NUM +ROT_NUM] , KK_CalculatedDist_0[TVL_NUM +ROT_NUM];
_LOCAL  AzzScal_typ  LCRScal_0[TVL_NUM +ROT_NUM];

/*
_LOCAL float KK_StartDly, KK_RampUp , KK_StopDly, KK_RampDwn;
_LOCAL float KK_Contin_RampDwn , KK_Contin_RampUp; 
_LOCAL float KK_Dir;
_LOCAL float KK_Ramp_S;
_LOCAL float KK_Ramp_Acc;
*/
_LOCAL float KK_PrimeryVel;

_LOCAL plcbit KK_CatchTrig;
_LOCAL DINT KK_CatchActTicks , KK_CatchActTicks2;


UDINT _VirStatus[TVL_NUM +ROT_NUM];


_LOCAL float *calibPoint[TVL_NUM +ROT_NUM];

_LOCAL float _FinalVal[TVL_NUM +ROT_NUM] , _PrimVel[TVL_NUM +ROT_NUM];
_LOCAL float StepVolt , VoltRampDown , StepVel;
_LOCAL float PrimVel , VelBoost1 , VelBoost2;

_LOCAL float KK_Float[TVL_NUM +ROT_NUM];

//_LOCAL float KK_dir;
//_LOCAL DINT KK_Step;



/*
typedef struct CalibProc_typ{

	//INT actDistInTicks;
	//INT TargetDistInTicks;
	float measSpeed_out;
	
	plctime _time , TimeLoop;
	

}CalibProc_typ;


_LOCAL CalibProc_typ CalibProc[TVL_NUM +ROT_NUM];
*/

float Get_S_fromRamp(float v , float t);

void TVL_MODE_BEMF(void);
void TVL_MODE_BPV(void);
void TVL_MODE_BPV_PosMode(void);

//void TVL_MODE_BPV_experimental(void);


void _INIT TVL_ROTINIT(void)
{
	int i;

	CurCfg.Dirty = CurCfg.Dirty;
		
	StepVolt = 12.0;
	VoltRampDown = 0.5;
	StepVel = 2;

	
	Process[0].Disable = Process[0].Disable;
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;

		
	for(i=0 ; i < (ROT_NUM + TVL_NUM); i++){
	
		/*
		MovePara[i].vC_pos   = 0.035;
		MovePara[i].aAcc_pos = 0.35 ;
	
		MovePara[i].vC_neg = MovePara[i].vC_pos;
		MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
		MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
		MovePara[i].aDec_neg = MovePara[i].aDec_pos;
		*/
		
		Ramp[i].y_max = 1000.0;
		Ramp[i].y_min = -1000.0;;	
		Ramp[i].enable = 1;

	}

	
}

void _CYCLIC TVL_ROTCYCLIC(void)
{

	
				
	if(!TVL_ROT_Param_Set[0].pPrc)
		return;


	if(TVL_ROT_Param_Set[0].pPrc->ConfigReady <= 0)
		return;

	KK_PrimeryVel = (*TVL_ROT_Param_Set[0].pPrimVel);

	/*
	KK_StartDly 	= (*TVL_ROT_Param_Set[0].pStartDly);
	KK_RampUp 	= (*TVL_ROT_Param_Set[0].pRampUp);
	KK_StopDly 	= (*TVL_ROT_Param_Set[0].pStopDly);
	KK_RampDwn 	= (*TVL_ROT_Param_Set[0].pRampDwn);
	KK_Contin_RampDwn = (*TVL_ROT_Param_Set[0].pContinRampDwn);
	KK_Contin_RampUp = (*TVL_ROT_Param_Set[0].pContinRampUp);
	
	if((*TVL_ROT_Param_Set[0].pRampDwn) > 0.0)
		KK_Ramp_Acc = (*TVL_ROT_Param_Set[0].pPrimVel)/(*TVL_ROT_Param_Set[0].pRampDwn);
	else
		KK_Ramp_Acc = (*TVL_ROT_Param_Set[0].pPrimVel)/0.5;

	KK_Ramp_S = Get_S_fromRamp((*TVL_ROT_Param_Set[0].pPrimVel) , (*TVL_ROT_Param_Set[0].pRampDwn));
	
		*/

	switch(TVL_ROT_Param_Set[0].Mode){


		case 1:

			TVL_MODE_BPV();
		
		break;

		
		case 2: // SubStep by Pos
		case 3: // SubStep by Interrupt

			//if(CurCfg.Param_ENUM[parENUM_TVL_MODE].val[0] == 3) // Continues welding
			if(CurCalib.GenSetup[SETUP_CONTIN_WELD] == 0) // Standard Mode
				TVL_MODE_BPV();
			else
				TVL_MODE_BPV_PosMode();

		break;


		default:

			TVL_MODE_BEMF();

		break;



	}
	
	

}

void TVL_MODE_BEMF(void){

	TVL_MODE_BPV();
	return;


}

#if 0

void TVL_MODE_BPV_experimental(void)
{	
int i , k;
char status;
float dir;
UINT calibType;
float x , y;
TIME t;
UINT synchClck;


			
		for(i=0 ; i < (ROT_NUM + TVL_NUM) ; i++){

			
			//MovePara[i].vC_pos   = 0.035 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);
			//MovePara[i].aAcc_pos = 0.35 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);

			/*
			MovePara[i].vC_neg = MovePara[i].vC_pos;
			MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
			MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
			MovePara[i].aDec_neg = MovePara[i].aDec_pos;
			*/
		
				
			if(!TVL_ROT_Param_Set[i].pPrc)
				break;

			prc = TVL_ROT_Param_Set[i].pPrc;
		
			if(prc->ConfigReady <= 0)
				break;

				
			vect = TVL_ROT_Param_Set[i].pVect;

	
			if(!vect)
				break;

						
			if(TVL_ROT_Param_Set[i].TotalRescaleFactor <= 0.0)
				TVL_ROT_Param_Set[i].TotalRescaleFactor = 1.0;
			

			if(TVL_ROT_Param_Set[i].pHomePos && TVL_ROT_Param_Set[i].pEncoder
				&& (TVL_ROT_Param_Set[i].EncPerTick != 0)){
				actDistInTicks[i] = (DINT)(((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick); 
				TVL_ROT_Param_Set[i].EncPos_FBK_Float = (float)((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/ (TVL_ROT_Param_Set[i].EncPerTick); 
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = (*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos); 
			}


			actDistInTicks[i] = actDistInTicks[i] * TVL_ROT_Param_Set[i].EncDir;
				
			TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * TVL_ROT_Param_Set[i].EncDir;
		//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * TVL_ROT_Param_Set[i].EncDir;
	
			if(prc->Param[1] & 0x1 ){ // travel direction 				
				dir = -1.0;	
				actDistInTicks[i] = actDistInTicks[i] * (-1);

				TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * (-1.0);
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * (-1);

			}
			else
				dir = 1.0;		

			TVL_ROT_Param_Set[i].EncPos_FBK = actDistInTicks[i];
			

			if(prc->Param[1] & 0x2 ) {// Calibration				
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS;

			}
			else{
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS_4OFF;
		
			}		

			/////////////////////////////////////
							
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
					case prcsForceON:
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					case prcsRESTART:
					case prcsHalt:	
					case prcsSTOP:
					case prcsQuickSTOP:
  
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


				case prcsRESTART:
					
					//actDistInTicks[i] = 0;
					prc->_step = 0;
					
				break;	


				case prcsHalt:

			
					//prc->_step = 0;

					prc->_step = 61;
	

				break;
				
				case prcsIDLE:

					
					
				break;	
	

				case prcsQuickStart:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 13;

					}


				break;
				
				case prcsSTART:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 11;

					}
					
					

				break;			


				case prcsQuickSTOP:
				case prcsSTOP:					

					if((prc->_step !=0) && (prc->_step <= 100)){

						prc->Status = prcsSTATUS_StopDly;

						switch(prc->xCMD){

							case prcsQuickSTOP:
								
								prc->_step = 22;	

							break;
							
							case prcsSTOP:	

								prc->_step = 20;	
								
							break;

						}						

					}
					else					
						prc->_step = 0;
			
				break;				

				
				case prcsDirectCntrl:					
						
					//actDistInTicks[i] = 0;
					
					if(prc->_step != 60){					

						prc->_step = 60;
					}

				break;	

				case prcsCalib:
									
								
					if(prc->_step != 70){

						
						if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
							vect->Mode = vectSt_Mode_Vel;

						vect->InVal = 0.002; // just start

						prc->_step = 70;
					}

				break;	
				
				case prcsJOGpos:
					
					prc->Status = prcsSTATUS_Jog;

					if(prc->_step != 50){

						Ramp[i].dy_up = fabsf((*TVL_ROT_Param_Set[i].pJog2Stop)/(*TVL_ROT_Param_Set[i].pRampUpdate));
						Ramp[i].dy_down = Ramp[i].dy_up;
						Ramp[i].enable = 0;
						AzzRamp(&Ramp[i]);
						Ramp[i].enable = 1;
						AzzRamp(&Ramp[i]);

						prc->_step = 50;
					}

				
				break;	

				case prcsSTEPneg:					

					
					targetDistInTicks[0]= actDistInTicks[i]-1;
					
					prc->_step = 80;				


				break;	

				case prcsSTEPpos:	
					
					targetDistInTicks[0]= actDistInTicks[i]+1;
					
					prc->_step = 80;			


				break;	
					

				case prcsForceON:
					
					vect->Mode = vectSt_Mode_Vel;
				
					prc->_step = 13;
			
				break;	


						
				
				//test
				case 100:
	
						
	
				break;
	
				
			}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			//////////////   AUTO STOP
			
			//if(ABS_INT(  actDistInTicks[i] - prc->Param[0]) < 5){ // Stop after travel distance

				
			if(actDistInTicks[i] >= prc->Param[0]){		


				switch(prc->Param[2]){


					case 1:

						prc->subStatus = 1;	
					
						if(prc->_step > 0 ){

							if(prc->_step <= 15)
								prc->_step = 24;					

						}

					break;

					
					case 3:
					case 4:

						prc->subStatus = 1;	


					break;


				}
				/* was before switch
				if(prc->Param[2] == 1 ){ // Auto Stop 

					prc->subStatus = 1;	
					
					if(prc->_step > 0 ){

						if(prc->_step <= 15)
							prc->_step = 24;					

					}
				}	
				*/
				
			}	

			KK_PrimVel[i] =  (*TVL_ROT_Param_Set[i].pPrimBackVel);
			
			////////////////////////////////////
			if(TVL_ROT_Param_Set[i].pSynchMode){
			
				if((*TVL_ROT_Param_Set[i].pSynchMode) == 0) // Straight Mode
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);
				else
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimBackVel);

			}
			else
				PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);



			
			VelBoost1 = (*TVL_ROT_Param_Set[i].pBoost1);
			VelBoost2 = (*TVL_ROT_Param_Set[i].pBoost2);

			/*
			switch(TVL_ROT_Param_Set[i].BoostCmd){

				case 1:

					PrimVel += VelBoost1;

				break;


				case 2:

					PrimVel += VelBoost2;

				break;


				default:

				break;

				
			}


			if(PrimVel < 0.0)
				PrimVel = 0.0;

			*/	

			//////////////////////////
		
			switch(prc->_step){


				case 0:  //Init

					prc->Status = prcsSTATUS_Stopped;				

		
					vect->InVal = 0.0;

					
					if(vect->FbkVect.Mode != vectSt_Mode_Idle){
						//vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Idle;						
					}	
					
	
					/*
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						//vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;						
					}
					*/
					
					
				break;	

				

				/////////////////////////////////////////////

				case 11: // start

					prc->subStatus = 0;							

					Ramp[i].x = 0.0 ;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;					
	
					prc->Status = prcsSTATUS_StartDly;
					prc->subStatus = 0;	


					if(vect->FbkVect.Mode != vectSt_Mode_Vel){

						if(dir < 0)
							vect->InVal = 0.0;
						else	
							vect->InVal = 0.0;
						
						vect->Mode = vectSt_Mode_Vel;
						break;
					}
		
					TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pStartDly);		

												
					prc->_step = 12;
					
				break;


				case 12:					
						
					if(TimerIsTimeOut(&Tmr[i])){

						if(vect->FbkVect.Mode != vectSt_Mode_Vel){
							vect->InVal = 0.0 ;
							vect->Mode = vectSt_Mode_Vel;
						
						}else{
							
							//LogTxt(&LogMgr, "P:TVL Start: %.2f" , ABS_TVL_Pos);
							
							prc->_step++;
							
							
						}
						
					}	
						
				break;	
				
	
				case 13:

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0 ;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}		
					

					Ramp[i].y = 0.0;
					Ramp[i].y_set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].y_set = 0;


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampUp > 0) {
															
									Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampUp));
									TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pContinRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampUp > 0) {
															
								Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampUp));
								TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;

					}

					

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1.0;
					Ramp[i].set = 0;

					prc->Status = prcsSTATUS_RampUp;
					prc->subStatus = 0;	


					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp);	
	
					prc->_step++;
					
				break;	
	
	
	
				case 14:					
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						
						prc->_step++;
						_FinalVal[i] = (PrimVel);
					}	

													
	
				break;
	
	
	
				case 15:
	
					prc->Status = prcsSTATUS_Run;		
									
				break;
	
				
	
				case 20:

					prc->Status = prcsSTATUS_StopDly;				
										
					if((*TVL_ROT_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pStopDly));
						prc->_step ++;
					}
					else					
						prc->_step = 22;

				break;	
		
				case 21:

					/*

					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}	
					*/
					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;

					/*
					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}
					*/

					
					if((*TVL_ROT_Param_Set[i].pFinalVel) > (PrimVel))
						(*TVL_ROT_Param_Set[i].pFinalVel) = (PrimVel);


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampDwn > 0) {
															
									Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));
									TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampDwn > 0) {
															
								Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
								TimerStart(&Tmr[i],  (*TVL_ROT_Param_Set[i].pRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;

					}

					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					/*					
					if (*TVL_ROT_Param_Set[i].pRampDwn > 0){ 

						if(prc->Param[2] == 4){
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));	
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) + 2.0);

						}
						else{
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);			
						}	

						Ramp[i].dy_up = Ramp[i].dy_down;

						Ramp[i].x = 0.0;
										

					}
					else {

						Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;
						
					}

					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);
					*/
					prc->_step++;
	
				break;


				case 23:

					 if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){		

							Ramp[i].x = 0;
							Ramp[i].y_set = 0.0;
							Ramp[i].set = 1;
							AzzRamp(&Ramp[i]);
							Ramp[i].set = 0;

							if(prc->Param[2] >= 3) // Continuous Run 
								prc->_step = 0;
							else
								prc->_step++;	

							
					}
					
	
				break;	
	
	
				case 24:	

					Ramp[i].y = 0.0;

					TimerStart(&Tmr[i], 0.5);
					prc->Status = prcsSTATUS_ReturnDly;

					LogTxt(&LogMgr, "P:TVL Stop: %.2f" , ABS_TVL_Pos);

		
					
					prc->_step++;	
					
	
				break;

				

				case 25:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					prc->Status = prcsSTATUS_ReturnDly;
							
					if(TVL_ROT_Param_Set[i].pRewrapDly  && TVL_ROT_Param_Set[i].pRewrapVel
						&& (prc->Param[2] != 0)){
							
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRewrapDly));	

						prc->_step++;						

					}
					else	
						prc->_step = 0;	
					
				
	
				break;	


				case 26:

					prc->Status = prcsSTATUS_Return;	

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						vect->InVal = 0.0;
					}	


					if(TimerIsTimeOut(&Tmr[i])){

						//targetDistInTicks[i] = prc->Param[0] - (INT)prc->Param[3]; 
						targetDistInTicks[i] = (INT)prc->Param[3]; 
						prc->_step++;
					}
					
			
				break;	


				case 27:

					
					
					if(actDistInTicks[i] > targetDistInTicks[i]){

						Ramp[i].y =  (*TVL_ROT_Param_Set[i].pRewrapVel)* (-1.0);			
						
													
					}
					else if(actDistInTicks[i] < targetDistInTicks[i]){
						
						Ramp[i].y = (*TVL_ROT_Param_Set[i].pRewrapVel);						
							
					}
							
					if(ABS_INT(actDistInTicks[i] - targetDistInTicks[i]) <  TVL_ROT_Param_Set[i].TicksAtHighVel)	{		
					
							Ramp[i].y = 0.0;
							prc->_step++;

					}
						
				break;


				
				case 28:									
				
					if(actDistInTicks[i] >= (targetDistInTicks[i])){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] < (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	


				/*

				case 28:									

					if(actDistInTicks[i] >= (targetDistInTicks[i] + 1)){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] <= (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	

				*/


				case 29:

					 if(actDistInTicks[i] <= (targetDistInTicks[i]))	{	
					 	
					 	Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;
	
	
				///////////////////////////////////////////////////////////////////////


				case 50:

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *TVL_ROT_Param_Set[i].pJog2Stop * prc->fParam[0];
					else
						prc->_step = 0;	
						

				break;	

				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					//vectSt_Mode_Torq
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;
					}
					else{

					
						if(TVL_ROT_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0.0){

								vect->InVal = (*TVL_ROT_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]/24.0) * TVL_ROT_Param_Set[i].MaxSpeedJog;
								KK_Float[i] = vect->InVal;
							}		
							else {
							
								//prc->_step = 0;

								vect->Mode = vectSt_Mode_Idle;
								prc->_step ++;
							}
								
						}
						
					}
				

				break;	


				case 61:

					if(vect->FbkVect.Mode != vectSt_Mode_Idle){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Idle;
					}
					else						
						prc->_step = 0;
					


				break;

			
				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode == vectSt_Mode_Vel)						
						vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;
					else
						vect->InVal = 0.0002;

					
					prc->Status = prcsSTATUS_ForceCalib;
											
				break;



				case 80:	

					prc->Status = prcsSTATUS_Step;

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){						
						vect->Mode = vectSt_Mode_Vel;
						break;
					}	
					

					if(actDistInTicks[i] >= (targetDistInTicks[i] + 1)){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] <= (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	


				case 81:

					 if(actDistInTicks[i] <= (targetDistInTicks[i]))	{	
					 	
					 	Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;			
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			
			
			
			//if(vect->FbkVect.Mode == vectSt_Mode_Vel){

				switch(prc->_step){

					case 0: // Stop

						//vect->InVal = 0;

					break;	

					 // Halt & Stop
					case 8:
					case 9:
					case 10:
					break;
					///////
					
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:	
					case 80:
					case 81:					
					case 90:
					case 91:
					// Direct value without calibration - homing process

						vect->InVal = getFromCalib(Ramp[i].y , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
					
					break;

					case 60: // Jogging - Direct Controll


					break;	

					case 70: // Force directly speed in Calibration

						// was moved to state
						//vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;

					break;	

					default:
						
						AzzRamp(&Ramp[i]);
						//prc->Fbk[1] = Ramp[i].y;
						//prc->Fbk[1] = PrimVel;

						if((*TVL_ROT_Param_Set[i].pInitVel) < 0.0)
							(*TVL_ROT_Param_Set[i].pInitVel) = 0.0;

						if(TVL_ROT_Param_Set[i].pSynchClock)
							synchClck = (*TVL_ROT_Param_Set[i].pSynchClock);
						else
							synchClck = 0;

						switch(synchClck){

							default: 

								//y = Ramp[i].y;
								
								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


							case 1: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pFinalVel));
								
							break;

							
							case 2:	

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimInVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;

							case 3: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimOutVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


						}

						switch(TVL_ROT_Param_Set[i].BoostCmd){

							case 1:

								y += VelBoost1;

							break;


							case 2:

								y += VelBoost2;

							break;


							default:

							break;

							
						}


						if(y < 0.0)
							y = 0.0;
						
					 	vect->InVal = getFromCalib(y / TVL_ROT_Param_Set[i].TotalRescaleFactor , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;


					break;	

				}	
						
			/*	
			}			
			else{
				prc->Fbk[1] = 0;
				vect->InVal = 0.0;
			}
			*/

		vect->InVal = vect->InVal * dir;

		prc->Fbk[1] = PrimVel;
			
		if(prc->Status == prcsSTATUS_Stopped){
			prc->Fbk[0] = 0.0;
			//prc->Fbk[1] = (PrimVel);
		}
		else
			prc->Fbk[0] = prc->Fbk[1]; // CurTvlVel th same as commanded   

			
	}
	
	
	
	
}
#endif

void TVL_MODE_BPV_PosMode(void)
{	
int i , k;
char status;
float dir;
UINT calibType;
float x , y , f;
TIME t;
UINT synchClck;
DINT d;



			
		for(i=0 ; i < (ROT_NUM + TVL_NUM) ; i++){

			
			//MovePara[i].vC_pos   = 0.035 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);
			//MovePara[i].aAcc_pos = 0.35 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);

			/*
			MovePara[i].vC_neg = MovePara[i].vC_pos;
			MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
			MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
			MovePara[i].aDec_neg = MovePara[i].aDec_pos;
			*/
		
				
			if(!TVL_ROT_Param_Set[i].pPrc)
				break;

			prc = TVL_ROT_Param_Set[i].pPrc;
		
			if(prc->ConfigReady <= 0)
				break;

				
			vect = TVL_ROT_Param_Set[i].pVect;

	
			if(!vect)
				break;

								
			if(TVL_ROT_Param_Set[i].TotalRescaleFactor <= 0.0)
				TVL_ROT_Param_Set[i].TotalRescaleFactor = 1.0;
			

			if(TVL_ROT_Param_Set[i].pHomePos && TVL_ROT_Param_Set[i].pEncoder
				&& (TVL_ROT_Param_Set[i].EncPerTick != 0)){
				//actDistInTicks[i] = (DINT)(((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick); 
				actDistInTicks[i] = (DINT)((((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick)+0.5);
				TVL_ROT_Param_Set[i].EncPos_FBK_Float = (float)((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/ (TVL_ROT_Param_Set[i].EncPerTick); 
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = (*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos); 
			}


			actDistInTicks[i] = actDistInTicks[i] * TVL_ROT_Param_Set[i].EncDir;
				
			TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * TVL_ROT_Param_Set[i].EncDir;
		//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * TVL_ROT_Param_Set[i].EncDir;
	
			if(prc->Param[1] & 0x1 ){ // travel direction 				
				dir = -1.0;	
				actDistInTicks[i] = actDistInTicks[i] * (-1);

				TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * (-1.0);
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * (-1);

			}
			else
				dir = 1.0;	

			//KK_dir = dir;

			TVL_ROT_Param_Set[i].EncPos_FBK = actDistInTicks[i];
			

			if(prc->Param[1] & 0x2 ) {// Calibration				
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS;

			}
			else{
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS_4OFF;
		
			}		

			/////////////////////////////////////
							
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
					case prcsForceON:
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					case prcsRESTART:
					case prcsHalt:	
					case prcsSTOP:
					case prcsQuickSTOP:
  
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


				case prcsRESTART:
					
					//actDistInTicks[i] = 0;
					prc->_step = 0;
					
				break;	


				case prcsHalt:

			
					prc->_step = 0;
	

				break;
				
				case prcsIDLE:

					
					
				break;	
	

				case prcsQuickStart:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 12;

					}


				break;
				
				case prcsSTART:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 11;

					}
					
					

				break;						
				
				case prcsSTOP:					

					
					if((prc->_step !=0) && (prc->_step < 24)){

						prc->Status = prcsSTATUS_StopDly;

						prc->_step = 24;							

					}
					else					
						prc->_step = 0;

					
			
				break;	

				case prcsQuickSTOP:


					if((prc->_step !=0) && (prc->_step < 25)){

						prc->Status = prcsSTATUS_StopDly;

						prc->_step = 25;							

					}
					else					
						prc->_step = 0;
					
				break;

				
				case prcsDirectCntrl:					
						
					//actDistInTicks[i] = 0;
					
					if(prc->_step != 60){					

						prc->_step = 60;
					}

				break;	

				case prcsCalib:
									
								
					if(prc->_step != 70){

						
						if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
							vect->Mode = vectSt_Mode_Vel;

						vect->InVal = 0.002; // just start

						prc->_step = 70;
					}

				break;	
				
				case prcsJOGpos:
					
					prc->Status = prcsSTATUS_Jog;

					if(prc->_step != 50){

						Ramp[i].dy_up = fabsf((*TVL_ROT_Param_Set[i].pJog2Stop)/(*TVL_ROT_Param_Set[i].pRampUpdate));
						Ramp[i].dy_down = Ramp[i].dy_up;
						Ramp[i].enable = 0;
						AzzRamp(&Ramp[i]);
						Ramp[i].enable = 1;
						AzzRamp(&Ramp[i]);

						prc->_step = 50;
					}

				
				break;	

				case prcsSTEPneg:					

					if(prc->_step == 0){

						if(dir > 0){
						
							if(actDistInTicks[i] <=0)
								targetDistInTicks[i] = actDistInTicks[i] - 2; 
							else
								targetDistInTicks[i] = actDistInTicks[i] - 1; 
						}
						else {	

						
							if(actDistInTicks[i] <=0)
								targetDistInTicks[i] = actDistInTicks[i] - 1; 
							else
								targetDistInTicks[i] = actDistInTicks[i] - 0; 


						}
			
						prc->_step = 80;	

					}


				break;	

				case prcsSTEPpos:	
					
					if(prc->_step == 0){
						
						if(dir > 0){
						
							if(actDistInTicks[i] < 0)
								
								targetDistInTicks[i] = actDistInTicks[i] + 0; 	
							else
								targetDistInTicks[i] = actDistInTicks[i] + 1;
						}
						else {	

						
							if(actDistInTicks[i] < 0)
								targetDistInTicks[i] = actDistInTicks[i] + 1;
							else
								targetDistInTicks[i] = actDistInTicks[i] + 2 ;


						}

						
						prc->_step = 80;	

					}			


				break;	
					

				case prcsForceON:
					
					vect->Mode = vectSt_Mode_Vel;
				
					prc->_step = 13;
			
				break;	


						
				
				//test
				case 100:
	
						
	
				break;
	
				
			}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			//////////////   AUTO STOP

			////////////////////////////////////////////////////////////
			// Only diagnostic
			if(prc->_step == 18){
			
					if(TVL_ROT_Param_Set[i].EncDir*((DINT)dir) == 1){

						if(vect->StatusReg & 0x20000){

							if(KK_CatchTrig){
								KK_CatchTrig = 0;
								KK_CatchActTicks2 = actDistInTicks[i];
								
								

							}
							
							//prc->subStatus = 1;	

						}

					}
					else{
						
						if(!(vect->StatusReg & 0x20000)){

							if(KK_CatchTrig){
								KK_CatchTrig = 0;
								KK_CatchActTicks2 = actDistInTicks[i];
								
							}
						
							//prc->subStatus = 1; 

						}
					}
					
						
			}	
			///////////////////////////////////////////////////////////////
			///
			
			if(TVL_ROT_Param_Set[i].Mode == 3){	// By Interrupt

					if(_VirStatus[i] != vect->VirStatus){						

						if(prc->_step == 18){
							
							KK_CatchActTicks = actDistInTicks[i];

							if(!(TVL_ROT_Param_Set[i].Extras&0x2)) // TVL Rev Rampdown	
								prc->subStatus = 1;	
						
						}

					}
					_VirStatus[i] = vect->VirStatus;			
					

			}
			else{ // By actDist
			

				if(actDistInTicks[i] >= prc->Param[0]){						


					switch(prc->Param[2]){


						case 1:


							if(!(TVL_ROT_Param_Set[i].Extras&0x2)){ // TVL Rev Rampdown	
								prc->subStatus = 1;							

								/*
								if(prc->_step > 0 ){

									if(prc->_step <= 15)
										prc->_step = 24;					

								}*/

							}

						break;

						
						case 3:
						case 4:

							if(!(TVL_ROT_Param_Set[i].Extras&0x2)) // TVL Rev Rampdown
								prc->subStatus = 1;		


						break;


					}
				}
				
			}	

			
			////////////////////////////////////
			if(TVL_ROT_Param_Set[i].pSynchMode){
			
				if((*TVL_ROT_Param_Set[i].pSynchMode) == 0) // Straight Mode
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);
				else
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimBackVel);

			}
			else
				PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);



			
			VelBoost1 = (*TVL_ROT_Param_Set[i].pBoost1);
			VelBoost2 = (*TVL_ROT_Param_Set[i].pBoost2);

			/*
			switch(TVL_ROT_Param_Set[i].BoostCmd){

				case 1:

					PrimVel += VelBoost1;

				break;


				case 2:

					PrimVel += VelBoost2;

				break;


				default:

				break;

				
			}


			if(PrimVel < 0.0)
				PrimVel = 0.0;

			*/	

			//////////////////////////
		
			switch(prc->_step){


				case 0:  //Init

					prc->Status = prcsSTATUS_Stopped;				

		
					vect->InVal = 0.0;

					/*
					if(vect->FbkVect.Mode != vectSt_Mode_Idle){
						//vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Idle;						
					}	
					*/
	
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;						
					}
					
					
				break;	

				
			/////////////////////////////////////////////

				case 11: // start

					prc->subStatus = 0;							

					Ramp[i].x = 0.0 ;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;					
	
					prc->Status = prcsSTATUS_StartDly;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Drive_Independant){
						vect->InVal = 0.0;
						vect->Mode = vect->Mode = vectSt_Mode_Drive_Independant;
						break;
					}
		
					TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pStartDly);		

												
					prc->_step ++;
					
				break;


				case 12:					
						
					if(TimerIsTimeOut(&Tmr[i])){


						f = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;

						if((*TVL_ROT_Param_Set[i].pRampUp) > 0.0)
							vect->fParam[0] =  f/(*TVL_ROT_Param_Set[i].pRampUp); // ACC
						else						
							vect->fParam[0] =  10e+23; // Max ACC from Config

										
						vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pPrimVel);// CSPD					
						
						if(TVL_ROT_Param_Set[i].Extras&0x2) // TVL Rev Rampdown											
							vect->Param[0] =  (DINT)((prc->Param[0]) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
						

						else{ // Full Pass - not reverse

							f = Get_S_fromRamp((*TVL_ROT_Param_Set[0].pPrimVel) , (*TVL_ROT_Param_Set[0].pRampDwn)); 

							f= f + ((*TVL_ROT_Param_Set[0].pPrimVel) * (*TVL_ROT_Param_Set[0].pStopDly));

							d = (DINT)(f * TML_MTR_INST[0]._int.kPos);

							vect->Param[0] =  ((DINT)((prc->Param[0]) *  TVL_ROT_Param_Set[i].EncPerTick) + d)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);


						}

						vect->Param[1] =  ((DINT)((prc->Param[0]) *  TVL_ROT_Param_Set[i].EncPerTick) + 0)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);	
						
						KK_CatchTrig = 1;
						KK_CatchActTicks = 0;
						KK_CatchActTicks2 = 0;
	

						prc->subStatus = 0;
						
						vect->xCMD = VECT_Move_Abs_Cmd;	

						prc->_step++;						

					}

				break;


				case 13:

					if(!(vect->StatusReg & 0x00000400)){ // movement started

						prc->Status = prcsSTATUS_RampUp;	
						TimerStart(&Tmr[i],  (*TVL_ROT_Param_Set[i].pRampUp) +0.5);					
						prc->_step++;

					}
			

				break;


				case 14: // New Dec - agresive

					if(TimerIsTimeOut(&Tmr[i])){ //- agresive


						if(TVL_ROT_Param_Set[i].Extras&0x2){ // TVL Rev Rampdown
						
							prc->Status = prcsSTATUS_Run;
							vect->fParam[0] =  10e+23; // Max ACC from Config
							
							vect->xCMD = VECT_Upd_CACC; 
							
							TimerStart(&Tmr[i], 0.5);	

							_PrimVel[i] = (*TVL_ROT_Param_Set[i].pPrimVel);	
											
							prc->_step++;


						}else{ //- pRampDwn

							f = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
							
							if((*TVL_ROT_Param_Set[0].pRampDwn) > 0.0)
								vect->fParam[0] = f/(*TVL_ROT_Param_Set[0].pRampDwn);
							else
								vect->fParam[0] =  10e+23; // Max ACC from Config


							TimerStart(&Tmr[i], 0.5);

							vect->xCMD = VECT_Upd_CACC;

							prc->_step = 18;

						}

					}

				break;

				case 15: 

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					if(_PrimVel[i] != (*TVL_ROT_Param_Set[i].pPrimVel)){

						vect->fParam[1] = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType);
						TimerStart(&Tmr[i], 0.5);
						vect->xCMD = VECT_Upd_CSPD;
						
					}
					_PrimVel[i] = (*TVL_ROT_Param_Set[i].pPrimVel);	
					
	
					if(vect->StatusReg & 0x00000400){ // movement stopped

						prc->Status = prcsSTATUS_StopDly;						

						prc->subStatus = 1;	

						f = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;

						vect->fParam[0] =  10e+23; // Max ACC from Config
						vect->fParam[1] = f;					

						
						f = Get_S_fromRamp((*TVL_ROT_Param_Set[0].pPrimVel) , (*TVL_ROT_Param_Set[0].pRampDwn)); 

						f+= (getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType) * (*TVL_ROT_Param_Set[0].pStopDly));

						d = (DINT)(f * TML_MTR_INST[0]._int.kPos);
																
						vect->Param[0] =  ((DINT)((prc->Param[0]) *  TVL_ROT_Param_Set[i].EncPerTick) - d)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
						

						vect->xCMD = VECT_Move_Abs_Cmd;	
					
						prc->_step++;

					}
									
				break;


				case 16:

					

					if(!(vect->StatusReg & 0x00000400)){ // movement started

						TimerStart(&Tmr[i],  0.5);		

										
						prc->_step++;

					}
			

				break;


				case 17:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;	

					f = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType);

					if((*TVL_ROT_Param_Set[0].pRampDwn) > 0.0)
						vect->fParam[0] = f /(*TVL_ROT_Param_Set[0].pRampDwn);
					else
						vect->fParam[0] =  10e+23; // Max ACC from Config

					TimerStart(&Tmr[i],  0.5);

					vect->xCMD = VECT_Upd_CACC;

					_PrimVel[i] = (*TVL_ROT_Param_Set[i].pPrimVel);						
					
					prc->_step++;



				break;							
	
				case 18:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					if(_PrimVel[i] != (*TVL_ROT_Param_Set[i].pPrimVel)){

						vect->fParam[1] = getFromCalib((*TVL_ROT_Param_Set[i].pPrimVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType);
						TimerStart(&Tmr[i], 0.5);
						vect->xCMD = VECT_Upd_CSPD;
						
					}
					_PrimVel[i] = (*TVL_ROT_Param_Set[i].pPrimVel);						
					

					if(vect->StatusReg & 0x00000400){ // movement stopped
						

						prc->Status = prcsSTATUS_RampDwn;

						prc->_step = 24;
						
						//prc->_step = 0;


					}		
					
	
				break;			
	
				/////////////////  End of TRAVEL
				////////////////////////////////////////////////
				
				case 20:

					prc->Status = prcsSTATUS_StopDly;				
										
					if((*TVL_ROT_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pStopDly));
						prc->_step ++;
					}
					else					
						prc->_step = 22;

				break;	
		
				case 21:

					/*

					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}	
					*/
					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
					
	
	
				break;	
	
					
				/////////////   Old Stop COmmand
#if 0
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;

					/*
					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}
					*/

					
					if((*TVL_ROT_Param_Set[i].pFinalVel) > (PrimVel))
						(*TVL_ROT_Param_Set[i].pFinalVel) = (PrimVel);


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampDwn > 0) {
															
									Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));
									TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampDwn > 0) {
															
								Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
								TimerStart(&Tmr[i],  (*TVL_ROT_Param_Set[i].pRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;

					}

					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					/*					
					if (*TVL_ROT_Param_Set[i].pRampDwn > 0){ 

						if(prc->Param[2] == 4){
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));	
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) + 2.0);

						}
						else{
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);			
						}	

						Ramp[i].dy_up = Ramp[i].dy_down;

						Ramp[i].x = 0.0;
										

					}
					else {

						Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;
						
					}

					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);
					*/
					prc->_step++;
	
				break;


				case 23:

					 if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){		

							Ramp[i].x = 0;
							Ramp[i].y_set = 0.0;
							Ramp[i].set = 1;
							AzzRamp(&Ramp[i]);
							Ramp[i].set = 0;

							if(prc->Param[2] >= 3) // Continuous Run 
								prc->_step = 0;
							else
								prc->_step++;	

							
					}
					
	
				break;	
#endif

				//////////////////////////////////////////////////////////////
	
	
				case 24:	

					Ramp[i].y = 0.0;

					TimerStart(&Tmr[i], 0.5);
					prc->Status = prcsSTATUS_ReturnDly;

					LogTxt(&LogMgr, "P:TVL Stop: %.2f" , ABS_TVL_Pos);

		
					
					prc->_step++;	
					
	
				break;

				

				case 25:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					prc->Status = prcsSTATUS_ReturnDly;
							
					if(TVL_ROT_Param_Set[i].pRewrapDly  && TVL_ROT_Param_Set[i].pRewrapVel
						&& (prc->Param[2] != 0)){
							
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRewrapDly));	

						prc->_step++;						

					}
					else	
						prc->_step = 0;	
					
				
	
				break;	


				case 26:

					prc->Status = prcsSTATUS_Return;	

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						vect->InVal = 0.0;
					}	


					if(TimerIsTimeOut(&Tmr[i])){

						
						targetDistInTicks[i] = (INT)prc->Param[3]; 
						prc->_step++;

					}
					
			
				break;	


				case 27:

					vect->Mode = vectSt_Mode_Drive_Independant;

					vect->InVal = 0.0;
					
					prc->_step++;
					

				break;


				case 28:

					vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
					vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
					
					
					d = TVL_ROT_Param_Set[i].TicksAtHighVel;

									
					if(TVL_ROT_Param_Set[i].HeadSwitch) //HeadSwitch
						d = -d;
					
					vect->Param[0] =  (DINT)((prc->Param[3]- d) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);										
					//vect->Param[0] =  (DINT)((prc->Param[3] + d) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
				
					vect->xCMD = VECT_Move_Abs_Cmd;	

					//vect->xCMD = VECT_Move_Rel_Cmd;	

					prc->_step++;

				break;


				case 29:

						if(!(vect->StatusReg & 0x00000400)) // movement started
							prc->_step++;
			

				break;


				case 30:


					if(vect->StatusReg & 0x00000400){ // movement stopped

						vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
						vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
												
						vect->Param[0] =  (DINT)(((float)prc->Param[3] + 0.25) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					
						vect->xCMD = VECT_Move_Abs_Cmd;	

						//vect->xCMD = VECT_Move_Rel_Cmd;	

						prc->_step++;

					}

				break;


				case 31:

						if(!(vect->StatusReg & 0x00000400)) // movement started
							prc->_step++;
			

				break;
						

				case 32:
					
					
					if(vect->StatusReg & 0x00000400){ // movement stopped
							
							prc->_step = 0;


					}
					

				break;	


				///////////////////////////////////////////////////////////////////////


				case 50:

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *TVL_ROT_Param_Set[i].pJog2Stop * prc->fParam[0];
					else
						prc->_step = 0;	
						

				break;	

				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					//vectSt_Mode_Torq
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;
					}
					else{

					
						if(TVL_ROT_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0.0){

								vect->InVal = (*TVL_ROT_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]/24.0) * TVL_ROT_Param_Set[i].MaxSpeedJog;
								KK_Float[i] = vect->InVal;
							}		
							else {
							
								prc->_step = 0;
							}
								
						}
						
					}
				

				break;	

			
				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode == vectSt_Mode_Vel)						
						vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;
					else
						vect->InVal = 0.0002;

					
					prc->Status = prcsSTATUS_ForceCalib;
											
				break;

				///////////////////////////   Step
				
				case 80:				

					if(vect->FbkVect.Mode != vectSt_Mode_Drive_Independant){
						vect->InVal = 0.0;
						vect->Mode = vect->Mode = vectSt_Mode_Drive_Independant;
						break;
					}
	
					f = getFromCalib((*TVL_ROT_Param_Set[i].pRewrapVel) , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
						
					vect->fParam[0] =  10e+23; // Max ACC from Config

									
					vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pPrimVel);// CSPD 

					vect->Param[0] =  (DINT)((float)targetDistInTicks[i]  *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					//vect->Param[0] =  (DINT)(targetStepDistInTicks[i]  *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					
					vect->xCMD = VECT_Move_Abs_Cmd; 

					prc->_step++;	

					//prc->_step = 0;

			

				break;


				case 81:

					if(!(vect->StatusReg & 0x00000400)){ // movement started

						TimerStart(&Tmr[i], 0.5);
						prc->_step++;

					}


				break;
				
				case 82:
					
					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					if(vect->StatusReg & 0x00000400){ // movement stopped


						if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
							vect->InVal = 0.0;
							vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;						
						}

						
						//prc->_step = 0;
						prc->_step++;	

					}
									
				break;

				case 83:

					if(vect->FbkVect.Mode == TVL_ROT_Param_Set[i].VectModeForJog){

						vect->xCMD = VECT_Refresh_Mode; 
						prc->_step = 0;

					}

				break;
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			
			
			
			//if(vect->FbkVect.Mode == vectSt_Mode_Vel){

				switch(prc->_step){

					case 0: // Stop
					case 27: // Retract to absolute value
					case 28: // Retract to absolute value
					case 29: // Retract to absolute value
					case 30: // Retract to absolute value
					case 31: // Retract to absolute value
					case 32: // Retract to absolute value
					case 40: // Retract to absolute value

						//vect->InVal = 0;

					break;	

					 // Halt & Stop
					case 8:
					case 9:
					case 10:
					break;
					///////
					
					case 24:
					case 25:
					case 26:
					//case 27:
					//case 28:
					//case 29:	
					case 80:
					case 81:					
					case 90:
					case 91:
					// Direct value without calibration - homing process

						vect->InVal = getFromCalib(Ramp[i].y , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
					
					break;

					case 60: // Jogging - Direct Controll


					break;	

					case 70: // Force directly speed in Calibration

						// was moved to state
						//vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;

					break;	

					default:
						
						AzzRamp(&Ramp[i]);
						//prc->Fbk[1] = Ramp[i].y;
						//prc->Fbk[1] = PrimVel;

						if((*TVL_ROT_Param_Set[i].pInitVel) < 0.0)
							(*TVL_ROT_Param_Set[i].pInitVel) = 0.0;

						if(TVL_ROT_Param_Set[i].pSynchClock)
							synchClck = (*TVL_ROT_Param_Set[i].pSynchClock);
						else
							synchClck = 0;

						switch(synchClck){

							default: 

								//y = Ramp[i].y;
								
								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


							case 1: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pFinalVel));
								
							break;

							
							case 2:	

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimInVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;

							case 3: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimOutVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


						}

						switch(TVL_ROT_Param_Set[i].BoostCmd){

							case 1:

								y += VelBoost1;

							break;


							case 2:

								y += VelBoost2;

							break;


							default:

							break;

							
						}


						if(y < 0.0)
							y = 0.0;
						
					 	vect->InVal = getFromCalib(y / TVL_ROT_Param_Set[i].TotalRescaleFactor , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;


					break;	

				}	
						
			/*	
			}			
			else{
				prc->Fbk[1] = 0;
				vect->InVal = 0.0;
			}
			*/

	
		if(TVL_ROT_Param_Set[i].Extras&0x2){ // TVL Rev Rampdown	
		
			if((prc->_step >= 20) && (prc->_step <= 25))
				vect->InVal = vect->InVal * dir * (-1.0);
			else
				vect->InVal = vect->InVal * dir;
		}
		else
			vect->InVal = vect->InVal * dir;
		
		////////////////////////

		prc->Fbk[1] = PrimVel;
			
		if(prc->Status == prcsSTATUS_Stopped){
			prc->Fbk[0] = 0.0;
			//prc->Fbk[1] = (PrimVel);
		}
		else
			prc->Fbk[0] = prc->Fbk[1]; // CurTvlVel th same as commanded   

			
	}
	
	
	
	
}


void TVL_MODE_BPV_PosMode_dobry(void)
{	
int i , k;
char status;
float dir;
UINT calibType;
float x , y;
TIME t;
UINT synchClck;
DINT d;



			
		for(i=0 ; i < (ROT_NUM + TVL_NUM) ; i++){

			
			//MovePara[i].vC_pos   = 0.035 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);
			//MovePara[i].aAcc_pos = 0.35 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);

			/*
			MovePara[i].vC_neg = MovePara[i].vC_pos;
			MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
			MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
			MovePara[i].aDec_neg = MovePara[i].aDec_pos;
			*/
		
				
			if(!TVL_ROT_Param_Set[i].pPrc)
				break;

			prc = TVL_ROT_Param_Set[i].pPrc;
		
			if(prc->ConfigReady <= 0)
				break;

				
			vect = TVL_ROT_Param_Set[i].pVect;

	
			if(!vect)
				break;

						
			if(TVL_ROT_Param_Set[i].TotalRescaleFactor <= 0.0)
				TVL_ROT_Param_Set[i].TotalRescaleFactor = 1.0;
			

			if(TVL_ROT_Param_Set[i].pHomePos && TVL_ROT_Param_Set[i].pEncoder
				&& (TVL_ROT_Param_Set[i].EncPerTick != 0)){
				//actDistInTicks[i] = (DINT)(((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick); 
				actDistInTicks[i] = (DINT)((((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick)+0.0);
				TVL_ROT_Param_Set[i].EncPos_FBK_Float = (float)((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/ (TVL_ROT_Param_Set[i].EncPerTick); 
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = (*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos); 
			}


			actDistInTicks[i] = actDistInTicks[i] * TVL_ROT_Param_Set[i].EncDir;
				
			TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * TVL_ROT_Param_Set[i].EncDir;
		//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * TVL_ROT_Param_Set[i].EncDir;
	
			if(prc->Param[1] & 0x1 ){ // travel direction 				
				dir = -1.0;	
				actDistInTicks[i] = actDistInTicks[i] * (-1);

				TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * (-1.0);
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * (-1);

			}
			else
				dir = 1.0;		

			TVL_ROT_Param_Set[i].EncPos_FBK = actDistInTicks[i];
			

			if(prc->Param[1] & 0x2 ) {// Calibration				
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS;

			}
			else{
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS_4OFF;
		
			}		

			/////////////////////////////////////
							
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
					case prcsForceON:
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					case prcsRESTART:
					case prcsHalt:	
					case prcsSTOP:
					case prcsQuickSTOP:
  
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


				case prcsRESTART:
					
					//actDistInTicks[i] = 0;
					prc->_step = 0;
					
				break;	


				case prcsHalt:

			
					prc->_step = 0;
	

				break;
				
				case prcsIDLE:

					
					
				break;	
	

				case prcsQuickStart:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 13;

					}


				break;
				
				case prcsSTART:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 11;

					}
					
					

				break;			


				case prcsQuickSTOP:
				case prcsSTOP:					

					if((prc->_step !=0) && (prc->_step <= 100)){

						prc->Status = prcsSTATUS_StopDly;

						switch(prc->xCMD){

							case prcsQuickSTOP:
								
								prc->_step = 22;	

							break;
							
							case prcsSTOP:	

								prc->_step = 20;	
								
							break;

						}						

					}
					else					
						prc->_step = 0;
			
				break;				

				
				case prcsDirectCntrl:					
						
					//actDistInTicks[i] = 0;
					
					if(prc->_step != 60){					

						prc->_step = 60;
					}

				break;	

				case prcsCalib:
									
								
					if(prc->_step != 70){

						
						if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
							vect->Mode = vectSt_Mode_Vel;

						vect->InVal = 0.002; // just start

						prc->_step = 70;
					}

				break;	
				
				case prcsJOGpos:
					
					prc->Status = prcsSTATUS_Jog;

					if(prc->_step != 50){

						Ramp[i].dy_up = fabsf((*TVL_ROT_Param_Set[i].pJog2Stop)/(*TVL_ROT_Param_Set[i].pRampUpdate));
						Ramp[i].dy_down = Ramp[i].dy_up;
						Ramp[i].enable = 0;
						AzzRamp(&Ramp[i]);
						Ramp[i].enable = 1;
						AzzRamp(&Ramp[i]);

						prc->_step = 50;
					}

				
				break;	

				case prcsSTEPneg:					

					
					targetDistInTicks[0]= actDistInTicks[i]-1;
					
					prc->_step = 80;				


				break;	

				case prcsSTEPpos:	
					
					targetDistInTicks[0]= actDistInTicks[i]+1;
					
					prc->_step = 80;			


				break;	
					

				case prcsForceON:
					
					vect->Mode = vectSt_Mode_Vel;
				
					prc->_step = 13;
			
				break;	


						
				
				//test
				case 100:
	
						
	
				break;
	
				
			}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			//////////////   AUTO STOP
			
			//if(ABS_INT(  actDistInTicks[i] - prc->Param[0]) < 5){ // Stop after travel distance

				
			if(actDistInTicks[i] >= prc->Param[0]){		


				switch(prc->Param[2]){


					case 1:

						prc->subStatus = 1;	
					
						if(prc->_step > 0 ){

							if(prc->_step <= 15)
								prc->_step = 24;					

						}

					break;

					
					case 3:
					case 4:

						prc->subStatus = 1;	


					break;


				}
				/* was before switch
				if(prc->Param[2] == 1 ){ // Auto Stop 

					prc->subStatus = 1;	
					
					if(prc->_step > 0 ){

						if(prc->_step <= 15)
							prc->_step = 24;					

					}
				}	
				*/
				
			}	

				
			////////////////////////////////////
			if(TVL_ROT_Param_Set[i].pSynchMode){
			
				if((*TVL_ROT_Param_Set[i].pSynchMode) == 0) // Straight Mode
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);
				else
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimBackVel);

			}
			else
				PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);



			
			VelBoost1 = (*TVL_ROT_Param_Set[i].pBoost1);
			VelBoost2 = (*TVL_ROT_Param_Set[i].pBoost2);

			/*
			switch(TVL_ROT_Param_Set[i].BoostCmd){

				case 1:

					PrimVel += VelBoost1;

				break;


				case 2:

					PrimVel += VelBoost2;

				break;


				default:

				break;

				
			}


			if(PrimVel < 0.0)
				PrimVel = 0.0;

			*/	

			//////////////////////////
		
			switch(prc->_step){


				case 0:  //Init

					prc->Status = prcsSTATUS_Stopped;				

		
					vect->InVal = 0.0;

					/*
					if(vect->FbkVect.Mode != vectSt_Mode_Idle){
						//vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Idle;						
					}	
					*/
	
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;						
					}
					
					
				break;	

				

				/////////////////////////////////////////////

				case 11: // start

					prc->subStatus = 0;							

					Ramp[i].x = 0.0 ;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;					
	
					prc->Status = prcsSTATUS_StartDly;
					prc->subStatus = 0;	


					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}
		
					TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pStartDly);		

												
					prc->_step = 12;
					
				break;


				case 12:					
						
					if(TimerIsTimeOut(&Tmr[i])){

						if(vect->FbkVect.Mode != vectSt_Mode_Vel){
							vect->InVal = 0.0 ;
							vect->Mode = vectSt_Mode_Vel;
						
						}else{
							
							//LogTxt(&LogMgr, "P:TVL Start: %.2f" , ABS_TVL_Pos);
							
							prc->_step++;
							
							
						}
						
					}	
						
				break;	
				
	
				case 13:

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0 ;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}		
					

					Ramp[i].y = 0.0;
					Ramp[i].y_set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].y_set = 0;


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampUp > 0) {
															
									Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampUp));
									TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pContinRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampUp > 0) {
															
								Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampUp));
								TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;

					}

					

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1.0;
					Ramp[i].set = 0;

					prc->Status = prcsSTATUS_RampUp;
					prc->subStatus = 0;	


					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp);	
	
					prc->_step++;
					
				break;	
	
	
	
				case 14:					
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						
						prc->_step++;
						_FinalVal[i] = (PrimVel);
					}	

													
	
				break;
	
	
	
				case 15:
	
					prc->Status = prcsSTATUS_Run;		
									
				break;
	
				
	
				case 20:

					prc->Status = prcsSTATUS_StopDly;				
										
					if((*TVL_ROT_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pStopDly));
						prc->_step ++;
					}
					else					
						prc->_step = 22;

				break;	
		
				case 21:

					/*

					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}	
					*/
					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;

					/*
					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}
					*/

					
					if((*TVL_ROT_Param_Set[i].pFinalVel) > (PrimVel))
						(*TVL_ROT_Param_Set[i].pFinalVel) = (PrimVel);


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampDwn > 0) {
															
									Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));
									TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampDwn > 0) {
															
								Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
								TimerStart(&Tmr[i],  (*TVL_ROT_Param_Set[i].pRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;

					}

					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					/*					
					if (*TVL_ROT_Param_Set[i].pRampDwn > 0){ 

						if(prc->Param[2] == 4){
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));	
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) + 2.0);

						}
						else{
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);			
						}	

						Ramp[i].dy_up = Ramp[i].dy_down;

						Ramp[i].x = 0.0;
										

					}
					else {

						Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;
						
					}

					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);
					*/
					prc->_step++;
	
				break;


				case 23:

					 if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){		

							Ramp[i].x = 0;
							Ramp[i].y_set = 0.0;
							Ramp[i].set = 1;
							AzzRamp(&Ramp[i]);
							Ramp[i].set = 0;

							if(prc->Param[2] >= 3) // Continuous Run 
								prc->_step = 0;
							else
								prc->_step++;	

							
					}
					
	
				break;	
	
	
				case 24:	

					Ramp[i].y = 0.0;

					TimerStart(&Tmr[i], 0.5);
					prc->Status = prcsSTATUS_ReturnDly;

					LogTxt(&LogMgr, "P:TVL Stop: %.2f" , ABS_TVL_Pos);

		
					
					prc->_step++;	
					
	
				break;

				

				case 25:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					prc->Status = prcsSTATUS_ReturnDly;
							
					if(TVL_ROT_Param_Set[i].pRewrapDly  && TVL_ROT_Param_Set[i].pRewrapVel
						&& (prc->Param[2] != 0)){
							
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRewrapDly));	

						prc->_step++;						

					}
					else	
						prc->_step = 0;	
					
				
	
				break;	


				case 26:

					prc->Status = prcsSTATUS_Return;	

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						vect->InVal = 0.0;
					}	


					if(TimerIsTimeOut(&Tmr[i])){

						
						targetDistInTicks[i] = (INT)prc->Param[3]; 
						prc->_step++;

					}
					
			
				break;	


				case 27:

					vect->Mode = vectSt_Mode_Drive_Independant;

					vect->InVal = 0.0;
					
					prc->_step++;
					

				break;


				case 28:

					vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
					vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
					
					
					d = TVL_ROT_Param_Set[i].TicksAtHighVel;
					
					if(TVL_ROT_Param_Set[i].HeadSwitch) //HeadSwitch
						d = -d;
															
					vect->Param[0] =  (DINT)((prc->Param[3]- d) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					

					vect->xCMD = VECT_Move_Abs_Cmd;	

					//vect->xCMD = VECT_Move_Rel_Cmd;	

					prc->_step++;

				break;


				case 29:

						if(!(vect->StatusReg & 0x00000400)) // movement started
							prc->_step++;
			

				break;


				case 30:


					if(vect->StatusReg & 0x00000400){ // movement stopped

						vect->fParam[0] =  (*TVL_ROT_Param_Set[i].pRewrapVel)/3.0; // ACC
						vect->fParam[1] =  (*TVL_ROT_Param_Set[i].pRewrapVel);					
												
						vect->Param[0] =  (DINT)(((float)prc->Param[3] + 0.25) *  TVL_ROT_Param_Set[i].EncPerTick)*TVL_ROT_Param_Set[i].EncDir*((DINT)dir) +  (*TVL_ROT_Param_Set[i].pHomePos);
					
						vect->xCMD = VECT_Move_Abs_Cmd;	

						//vect->xCMD = VECT_Move_Rel_Cmd;	

						prc->_step++;

					}

				break;


				case 31:

						if(!(vect->StatusReg & 0x00000400)) // movement started
							prc->_step++;
			

				break;
						

				case 32:
					
					
					if(vect->StatusReg & 0x00000400){ // movement stopped
							
							prc->_step = 0;


					}
					

				break;	


				///////////////////////////////////////////////////////////////////////


				case 50:

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *TVL_ROT_Param_Set[i].pJog2Stop * prc->fParam[0];
					else
						prc->_step = 0;	
						

				break;	

				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					//vectSt_Mode_Torq
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;
					}
					else{

					
						if(TVL_ROT_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0.0){

								vect->InVal = (*TVL_ROT_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]/24.0) * TVL_ROT_Param_Set[i].MaxSpeedJog;
								KK_Float[i] = vect->InVal;
							}		
							else {
							
								prc->_step = 0;
							}
								
						}
						
					}
				

				break;	

			
				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode == vectSt_Mode_Vel)						
						vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;
					else
						vect->InVal = 0.0002;

					
					prc->Status = prcsSTATUS_ForceCalib;
											
				break;



				case 80:	

					prc->Status = prcsSTATUS_Step;

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){						
						vect->Mode = vectSt_Mode_Vel;
						break;
					}	
					

					if(actDistInTicks[i] >= (targetDistInTicks[i] + 1)){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] <= (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	


				case 81:

					 if(actDistInTicks[i] <= (targetDistInTicks[i]))	{	
					 	
					 	Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;			
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			
			
			
			//if(vect->FbkVect.Mode == vectSt_Mode_Vel){

				switch(prc->_step){

					case 0: // Stop
					case 27: // Retract to absolute value
					case 28: // Retract to absolute value
					case 29: // Retract to absolute value
					case 30: // Retract to absolute value
					case 31: // Retract to absolute value
					case 32: // Retract to absolute value
					case 40: // Retract to absolute value

						//vect->InVal = 0;

					break;	

					 // Halt & Stop
					case 8:
					case 9:
					case 10:
					break;
					///////
					
					case 24:
					case 25:
					case 26:
					//case 27:
					//case 28:
					//case 29:	
					case 80:
					case 81:					
					case 90:
					case 91:
					// Direct value without calibration - homing process

						vect->InVal = getFromCalib(Ramp[i].y , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
					
					break;

					case 60: // Jogging - Direct Controll


					break;	

					case 70: // Force directly speed in Calibration

						// was moved to state
						//vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;

					break;	

					default:
						
						AzzRamp(&Ramp[i]);
						//prc->Fbk[1] = Ramp[i].y;
						//prc->Fbk[1] = PrimVel;

						if((*TVL_ROT_Param_Set[i].pInitVel) < 0.0)
							(*TVL_ROT_Param_Set[i].pInitVel) = 0.0;

						if(TVL_ROT_Param_Set[i].pSynchClock)
							synchClck = (*TVL_ROT_Param_Set[i].pSynchClock);
						else
							synchClck = 0;

						switch(synchClck){

							default: 

								//y = Ramp[i].y;
								
								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


							case 1: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pFinalVel));
								
							break;

							
							case 2:	

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimInVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;

							case 3: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimOutVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


						}

						switch(TVL_ROT_Param_Set[i].BoostCmd){

							case 1:

								y += VelBoost1;

							break;


							case 2:

								y += VelBoost2;

							break;


							default:

							break;

							
						}


						if(y < 0.0)
							y = 0.0;
						
					 	vect->InVal = getFromCalib(y / TVL_ROT_Param_Set[i].TotalRescaleFactor , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;


					break;	

				}	
						
			/*	
			}			
			else{
				prc->Fbk[1] = 0;
				vect->InVal = 0.0;
			}
			*/

	
		if(TVL_ROT_Param_Set[i].Extras&0x2){ // TVL Rev Rampdown	
		
			if((prc->_step >= 20) && (prc->_step <= 25))
				vect->InVal = vect->InVal * dir * (-1.0);
			else
				vect->InVal = vect->InVal * dir;
		}
		else
			vect->InVal = vect->InVal * dir;
		
		////////////////////////

		prc->Fbk[1] = PrimVel;
			
		if(prc->Status == prcsSTATUS_Stopped){
			prc->Fbk[0] = 0.0;
			//prc->Fbk[1] = (PrimVel);
		}
		else
			prc->Fbk[0] = prc->Fbk[1]; // CurTvlVel th same as commanded   

			
	}
	
	
	
	
}

void TVL_MODE_BPV(void)
{	
int i , k;
char status;
float dir;
UINT calibType;
float x , y;
TIME t;
UINT synchClck;


			
		for(i=0 ; i < (ROT_NUM + TVL_NUM) ; i++){

			
			//MovePara[i].vC_pos   = 0.035 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);
			//MovePara[i].aAcc_pos = 0.35 * (*TVL_ROT_Param_Set[i].pMoveAngle_Vel);

			/*
			MovePara[i].vC_neg = MovePara[i].vC_pos;
			MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
			MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
			MovePara[i].aDec_neg = MovePara[i].aDec_pos;
			*/
		
				
			if(!TVL_ROT_Param_Set[i].pPrc)
				break;

			prc = TVL_ROT_Param_Set[i].pPrc;
		
			if(prc->ConfigReady <= 0)
				break;

				
			vect = TVL_ROT_Param_Set[i].pVect;

	
			if(!vect)
				break;

						
			if(TVL_ROT_Param_Set[i].TotalRescaleFactor <= 0.0)
				TVL_ROT_Param_Set[i].TotalRescaleFactor = 1.0;
			

			if(TVL_ROT_Param_Set[i].pHomePos && TVL_ROT_Param_Set[i].pEncoder
				&& (TVL_ROT_Param_Set[i].EncPerTick != 0)){
				//actDistInTicks[i] = (DINT)(((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick); 
				actDistInTicks[i] = (DINT)((((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/TVL_ROT_Param_Set[i].EncPerTick)+0.5);
				TVL_ROT_Param_Set[i].EncPos_FBK_Float = (float)((*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos))/ (TVL_ROT_Param_Set[i].EncPerTick); 
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = (*TVL_ROT_Param_Set[i].pEncoder)  - (*TVL_ROT_Param_Set[i].pHomePos); 
			}

			
			actDistInTicks[i] = actDistInTicks[i] * TVL_ROT_Param_Set[i].EncDir;
				
			TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * TVL_ROT_Param_Set[i].EncDir;
		//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * TVL_ROT_Param_Set[i].EncDir;
	
			if(prc->Param[1] & 0x1 ){ // travel direction 				
				dir = -1.0;	
				actDistInTicks[i] = actDistInTicks[i] * (-1);

				TVL_ROT_Param_Set[i].EncPos_FBK_Float = TVL_ROT_Param_Set[i].EncPos_FBK_Float * (-1.0);
				//TVL_ROT_Param_Set[i].EncPos_FBK_Raw = TVL_ROT_Param_Set[i].EncPos_FBK_Raw * (-1);

			}
			else
				dir = 1.0;		

			TVL_ROT_Param_Set[i].EncPos_FBK = actDistInTicks[i];
			

			if(prc->Param[1] & 0x2 ) {// Calibration				
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS;

			}
			else{
				
				if(TVL_ROT_Param_Set[i].pCalibPoints)
					calibType = CALIB_2POINTS_4OFF;
		
			}		

			/////////////////////////////////////
							
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
					case prcsForceON:
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					case prcsRESTART:
					case prcsHalt:	
					case prcsSTOP:
					case prcsQuickSTOP:
  
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


				case prcsRESTART:
					
					//actDistInTicks[i] = 0;
					prc->_step = 0;
					
				break;	


				case prcsHalt:

			
					prc->_step = 0;
	

				break;
				
				case prcsIDLE:

					
					
				break;	
	

				case prcsQuickStart:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 13;

					}


				break;
				
				case prcsSTART:

					if(prc->Param[2] == 2){// Rewrap ONLY 

						prc->Status = prcsSTATUS_Return;
						TimerStart(&Tmr[i], 0.5);
						prc->_step = 26;


					}
					else {

						prc->Status = prcsSTATUS_StartDly;
						prc->_step = 11;

					}
					
					

				break;			


				case prcsQuickSTOP:
				case prcsSTOP:					

					if((prc->_step !=0) && (prc->_step <= 100)){

						prc->Status = prcsSTATUS_StopDly;

						switch(prc->xCMD){

							case prcsQuickSTOP:
								
								prc->_step = 22;	

							break;
							
							case prcsSTOP:	

								prc->_step = 20;	
								
							break;

						}						

					}
					else					
						prc->_step = 0;
			
				break;				

				
				case prcsDirectCntrl:					
						
					//actDistInTicks[i] = 0;
					
					if(prc->_step != 60){					

						prc->_step = 60;
					}

				break;	

				case prcsCalib:
									
								
					if(prc->_step != 70){

						
						if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
							vect->Mode = vectSt_Mode_Vel;

						vect->InVal = 0.002; // just start

						prc->_step = 70;
					}

				break;	
				
				case prcsJOGpos:
					
					prc->Status = prcsSTATUS_Jog;

					if(prc->_step != 50){

						Ramp[i].dy_up = fabsf((*TVL_ROT_Param_Set[i].pJog2Stop)/(*TVL_ROT_Param_Set[i].pRampUpdate));
						Ramp[i].dy_down = Ramp[i].dy_up;
						Ramp[i].enable = 0;
						AzzRamp(&Ramp[i]);
						Ramp[i].enable = 1;
						AzzRamp(&Ramp[i]);

						prc->_step = 50;
					}

				
				break;	

				case prcsSTEPneg:					

					
					targetDistInTicks[0]= actDistInTicks[i]-1;
					
					prc->_step = 80;				


				break;	

				case prcsSTEPpos:	
					
					targetDistInTicks[0]= actDistInTicks[i]+1;
					
					prc->_step = 80;			


				break;	
					

				case prcsForceON:
					
					vect->Mode = vectSt_Mode_Vel;
				
					prc->_step = 13;
			
				break;	


						
				
				//test
				case 100:
	
						
	
				break;
	
				
			}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			//////////////   AUTO STOP
			
			//if(ABS_INT(  actDistInTicks[i] - prc->Param[0]) < 5){ // Stop after travel distance

				
			if(actDistInTicks[i] >= prc->Param[0]){		


				switch(prc->Param[2]){


					case 1:

						prc->subStatus = 1;	
					
						if(prc->_step > 0 ){

							if(prc->_step <= 15)
								prc->_step = 24;					

						}

					break;

					
					case 3:
					case 4:

						prc->subStatus = 1;	


					break;


				}
				/* was before switch
				if(prc->Param[2] == 1 ){ // Auto Stop 

					prc->subStatus = 1;	
					
					if(prc->_step > 0 ){

						if(prc->_step <= 15)
							prc->_step = 24;					

					}
				}	
				*/
				
			}	

			//KK_PrimVel[i] =  (*TVL_ROT_Param_Set[i].pPrimBackVel);
			
			////////////////////////////////////
			if(TVL_ROT_Param_Set[i].pSynchMode){
			
				if((*TVL_ROT_Param_Set[i].pSynchMode) == 0) // Straight Mode
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);
				else
					PrimVel =  (*TVL_ROT_Param_Set[i].pPrimBackVel);

			}
			else
				PrimVel =  (*TVL_ROT_Param_Set[i].pPrimVel);



			
			VelBoost1 = (*TVL_ROT_Param_Set[i].pBoost1);
			VelBoost2 = (*TVL_ROT_Param_Set[i].pBoost2);

			/*
			switch(TVL_ROT_Param_Set[i].BoostCmd){

				case 1:

					PrimVel += VelBoost1;

				break;


				case 2:

					PrimVel += VelBoost2;

				break;


				default:

				break;

				
			}


			if(PrimVel < 0.0)
				PrimVel = 0.0;

			*/	

			//////////////////////////
		
			switch(prc->_step){


				case 0:  //Init

					prc->Status = prcsSTATUS_Stopped;				

		
					vect->InVal = 0.0;

					/*
					if(vect->FbkVect.Mode != vectSt_Mode_Idle){
						//vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Idle;						
					}	
					*/
	
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;						
					}
					
					
				break;	

				

				/////////////////////////////////////////////

				case 11: // start

					prc->subStatus = 0;							

					Ramp[i].x = 0.0 ;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;					
	
					prc->Status = prcsSTATUS_StartDly;
					prc->subStatus = 0;	


					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}
		
					TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pStartDly);		

												
					prc->_step = 12;
					
				break;


				case 12:					
						
					if(TimerIsTimeOut(&Tmr[i])){

						if(vect->FbkVect.Mode != vectSt_Mode_Vel){
							vect->InVal = 0.0 ;
							vect->Mode = vectSt_Mode_Vel;
						
						}else{
							
							//LogTxt(&LogMgr, "P:TVL Start: %.2f" , ABS_TVL_Pos);
							
							prc->_step++;
							
							
						}
						
					}	
						
				break;	
				
	
				case 13:

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0 ;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}		
					

					Ramp[i].y = 0.0;
					Ramp[i].y_set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].y_set = 0;


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampUp > 0) {
															
									Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampUp));
									TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pContinRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampUp > 0) {
															
								Ramp[i].dy_up = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampUp));
								TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_up = 3.4e+38;

							}
							
						break;

					}

					

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1.0;
					Ramp[i].set = 0;

					prc->Status = prcsSTATUS_RampUp;
					prc->subStatus = 0;	


					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i],  *TVL_ROT_Param_Set[i].pRampUp);	
	
					prc->_step++;
					
				break;	
	
	
	
				case 14:					
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						
						prc->_step++;
						_FinalVal[i] = (PrimVel);
					}	

													
	
				break;
	
	
	
				case 15:
	
					prc->Status = prcsSTATUS_Run;		
									
				break;
	
				
	
				case 20:

					prc->Status = prcsSTATUS_StopDly;				
										
					if((*TVL_ROT_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pStopDly));
						prc->_step ++;
					}
					else					
						prc->_step = 22;

				break;	
		
				case 21:

					/*

					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}	
					*/
					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;

					/*
					if(actDistInTicks[i] > targetDistInTicks[i]){
						prc->_step = 24;
						break;
					}
					*/

					
					if((*TVL_ROT_Param_Set[i].pFinalVel) > (PrimVel))
						(*TVL_ROT_Param_Set[i].pFinalVel) = (PrimVel);


					switch(prc->Param[2]){


						case 4: // Contin_Ramp

							if (*TVL_ROT_Param_Set[i].pContinRampDwn > 0) {
															
									Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));
									TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;


						default:

							if (*TVL_ROT_Param_Set[i].pRampDwn > 0) {
															
								Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
								TimerStart(&Tmr[i],  (*TVL_ROT_Param_Set[i].pRampDwn) +1.0);	

							}
							else{
								TimerStart(&Tmr[i],  1.0);
								Ramp[i].dy_down = 3.4e+38;

							}
							
						break;

					}

					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					/*					
					if (*TVL_ROT_Param_Set[i].pRampDwn > 0){ 

						if(prc->Param[2] == 4){
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pContinRampDwn));	
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pContinRampDwn) + 2.0);

						}
						else{
							Ramp[i].dy_down = fabsf((1.0)/(*TVL_ROT_Param_Set[i].pRampDwn));
							TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);			
						}	

						Ramp[i].dy_up = Ramp[i].dy_down;

						Ramp[i].x = 0.0;
										

					}
					else {

						Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;
						
					}

					//CalibProc[i].TimeLoop = 0;
					//CalibProc[i]._time = clock_ms();

					//TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRampDwn) + 2.0);
					*/
					prc->_step++;
	
				break;


				case 23:

					 if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){		

							Ramp[i].x = 0;
							Ramp[i].y_set = 0.0;
							Ramp[i].set = 1;
							AzzRamp(&Ramp[i]);
							Ramp[i].set = 0;

							if(prc->Param[2] >= 3) // Continuous Run 
								prc->_step = 0;
							else
								prc->_step++;	

							
					}
					
	
				break;	
	
	
				case 24:	

					Ramp[i].y = 0.0;

					TimerStart(&Tmr[i], 0.5);
					prc->Status = prcsSTATUS_ReturnDly;

					LogTxt(&LogMgr, "P:TVL Stop: %.2f" , ABS_TVL_Pos);

		
					
					prc->_step++;	
					
	
				break;

				

				case 25:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					prc->Status = prcsSTATUS_ReturnDly;
							
					if(TVL_ROT_Param_Set[i].pRewrapDly  && TVL_ROT_Param_Set[i].pRewrapVel
						&& (prc->Param[2] != 0)){
							
						TimerStart(&Tmr[i], (*TVL_ROT_Param_Set[i].pRewrapDly));	

						prc->_step++;						

					}
					else	
						prc->_step = 0;	
					
				
	
				break;	


				case 26:

					prc->Status = prcsSTATUS_Return;	

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						vect->InVal = 0.0;
					}	


					if(TimerIsTimeOut(&Tmr[i])){

						//targetDistInTicks[i] = prc->Param[0] - (INT)prc->Param[3]; 
						targetDistInTicks[i] = (INT)prc->Param[3]; 
						prc->_step++;
					}
					
			
				break;	


				
				case 27:					
					
					if((actDistInTicks[i] + TVL_ROT_Param_Set[i].TicksAtHighVel) > targetDistInTicks[i]){

						Ramp[i].y =  (*TVL_ROT_Param_Set[i].pRewrapVel)* (-1.0);			
						
													
					}
					else{						

							Ramp[i].y = 0.0;
							prc->_step ++;


					}					
						
				break;			


				case 28:

					Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);

					if(actDistInTicks[i] >= (targetDistInTicks[i])) {	
						
						Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;


#if 0 // Old Code - was changed 3 June 2021 - for Corry machine 

				case 27:

					
					
					if(actDistInTicks[i] > targetDistInTicks[i]){

						Ramp[i].y =  (*TVL_ROT_Param_Set[i].pRewrapVel)* (-1.0);			
						
													
					}
					else if(actDistInTicks[i] < targetDistInTicks[i]){
						
						Ramp[i].y = (*TVL_ROT_Param_Set[i].pRewrapVel);						
							
					}
							
					if(ABS_INT(actDistInTicks[i] - targetDistInTicks[i]) <  TVL_ROT_Param_Set[i].TicksAtHighVel)	{		
					
							Ramp[i].y = 0.0;
							prc->_step++;

					}
						
				break;


				
				case 28:									
				
					if(actDistInTicks[i] >= (targetDistInTicks[i])){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] < (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	


				/*

				case 28:									

					if(actDistInTicks[i] >= (targetDistInTicks[i] + 1)){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] <= (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	

				*/


				case 29:

					 if(actDistInTicks[i] <= (targetDistInTicks[i]))	{	
					 	
					 	Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;
	
#endif	
				///////////////////////////////////////////////////////////////////////


				case 50:

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *TVL_ROT_Param_Set[i].pJog2Stop * prc->fParam[0];
					else
						prc->_step = 0;	
						

				break;	

				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					//vectSt_Mode_Torq
					
					if(vect->FbkVect.Mode != TVL_ROT_Param_Set[i].VectModeForJog){
						vect->InVal = 0.0;
						vect->Mode = TVL_ROT_Param_Set[i].VectModeForJog;
					}
					else{

					
						if(TVL_ROT_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0.0){

								vect->InVal = (*TVL_ROT_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]/24.0) * TVL_ROT_Param_Set[i].MaxSpeedJog;
								KK_Float[i] = vect->InVal;
							}		
							else {
							
								prc->_step = 0;
							}
								
						}
						
					}
				

				break;	

			
				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode == vectSt_Mode_Vel)						
						vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;
					else
						vect->InVal = 0.0002;

					
					prc->Status = prcsSTATUS_ForceCalib;
											
				break;



				case 80:	

					prc->Status = prcsSTATUS_Step;

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){						
						vect->Mode = vectSt_Mode_Vel;
						break;
					}	
					

					if(actDistInTicks[i] >= (targetDistInTicks[i] + 1)){						

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop)* (-1.0);
						prc->_step++;
						
													
					}
					else if(actDistInTicks[i] <= (targetDistInTicks[i])){

						Ramp[i].y = (*TVL_ROT_Param_Set[i].pJog2Stop);
					}					
	
				break;	


				case 81:

					 if(actDistInTicks[i] <= (targetDistInTicks[i]))	{	
					 	
					 	Ramp[i].x = 0;
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;

						vect->InVal = 0;
					
						prc->_step = 0;

					 }

				break;			
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			
			
			
			//if(vect->FbkVect.Mode == vectSt_Mode_Vel){

				switch(prc->_step){

					case 0: // Stop

						//vect->InVal = 0;

					break;	

					 // Halt & Stop
					case 8:
					case 9:
					case 10:
					break;
					///////
					
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:	
					case 80:
					case 81:					
					case 90:
					case 91:
					// Direct value without calibration - homing process

						vect->InVal = getFromCalib(Ramp[i].y , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;
					
					break;

					case 60: // Jogging - Direct Controll


					break;	

					case 70: // Force directly speed in Calibration

						// was moved to state
						//vect->InVal =  prc->fParam[1] / TVL_ROT_Param_Set[i].TotalRescaleFactor;

					break;	

					default:
						
						AzzRamp(&Ramp[i]);
						//prc->Fbk[1] = Ramp[i].y;
						//prc->Fbk[1] = PrimVel;

						if((*TVL_ROT_Param_Set[i].pInitVel) < 0.0)
							(*TVL_ROT_Param_Set[i].pInitVel) = 0.0;

						if(TVL_ROT_Param_Set[i].pSynchClock)
							synchClck = (*TVL_ROT_Param_Set[i].pSynchClock);
						else
							synchClck = 0;

						switch(synchClck){

							default: 

								//y = Ramp[i].y;
								
								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * (PrimVel - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


							case 1: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimVel) - (*TVL_ROT_Param_Set[i].pFinalVel));
								
							break;

							
							case 2:	

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimInVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimInVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;

							case 3: 

								//y = Ramp[i].y * (*TVL_ROT_Param_Set[i].pPrimOutVel)/PrimVel;

								if(prc->_step <= 15) // Ramp Up
									y = (*TVL_ROT_Param_Set[i].pInitVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pInitVel));
								else
									y = (*TVL_ROT_Param_Set[i].pFinalVel) + Ramp[i].y * ((*TVL_ROT_Param_Set[i].pPrimOutVel) - (*TVL_ROT_Param_Set[i].pFinalVel));

							break;


						}

						switch(TVL_ROT_Param_Set[i].BoostCmd){

							case 1:

								y += VelBoost1;

							break;


							case 2:

								y += VelBoost2;

							break;


							default:

							break;

							
						}


						if(y < 0.0)
							y = 0.0;
						
					 	vect->InVal = getFromCalib(y / TVL_ROT_Param_Set[i].TotalRescaleFactor , TVL_ROT_Param_Set[i].pCalibPoints, calibType) ;


					break;	

				}	
						
			/*	
			}			
			else{
				prc->Fbk[1] = 0;
				vect->InVal = 0.0;
			}
			*/

		//vect->InVal = vect->InVal * dir;

		if(TVL_ROT_Param_Set[i].Extras&0x2){ // TVL Rev Rampdown	
		
			if((prc->_step >= 20) && (prc->_step <= 25))
				vect->InVal = vect->InVal * dir * (-1.0);
			else
				vect->InVal = vect->InVal * dir;
		}
		else
			vect->InVal = vect->InVal * dir;

		prc->Fbk[1] = PrimVel;
			
		if(prc->Status == prcsSTATUS_Stopped){
			prc->Fbk[0] = 0.0;
			//prc->Fbk[1] = (PrimVel);
		}
		else
			prc->Fbk[0] = prc->Fbk[1]; // CurTvlVel th same as commanded   

			
	}
	
	
	
	
}

float Get_S_fromRamp(float v , float t){
float ret;


	if(t > 0.0)

		ret = (v*t)/2.0;
	
	else
		ret = (v*0.5)/2.0;

	return ret;

}


