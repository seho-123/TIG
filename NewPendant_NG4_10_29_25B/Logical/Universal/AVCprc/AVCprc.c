/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: AVCprc
 * File: AVCprc.c
 * Author: ckaczkowski
 * Created: November 14, 2013
 ********************************************************************
 * Implementation of program AVCprc
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>

#define TouchVel_par 	0
#define RetVel_par   	1
#define ArcTrshld_par 	2
#define RetTM_par 		0
#define TouchTM_par 	1

#define ClassTime 0.0096


_LOCAL Cfg_typ *cfg;
_LOCAL CurCfg_typ *curCfg;
_LOCAL CurCalib_typ *calib;
_LOCAL UINT segm; 

_LOCAL  AzzMovAvgFlt_typ  filterAzzMovAvgFlt[AVC_NUM];
_LOCAL   float HMI_Window , HMI_RegVolt , HMI_RegGain , HMI_Error; 
_LOCAL DINT HMI_Mode;
_LOCAL plcbit KK_NewIdea;
_LOCAL float KK_Cmd_IN , KK_Cmd_Out, KK_CoLeci;

float _InVal[AVC_NUM];
_LOCAL float BoostVoltage , _BoostVoltage[AVC_NUM], BoostTime , _IntegrError[AVC_NUM], IntegrGain;
_LOCAL  plcbit BoostApply, IntegrApply;

_LOCAL AzzRamp_typ Ramp[AVC_NUM];
_LOCAL TON_typ Tmr[AVC_NUM], LimitsFilterTmr[AVC_NUM], JumpTmr[AVC_NUM];
GenProcess_typ* prc;
StateVectorProxy_typ* vect;
UINT repeatCnt[AVC_NUM];
_LOCAL plcbit FirstWindowReached[AVC_NUM];


_LOCAL AzzNode_typ FBK_CalibrPoints[AVC_NUM][10] , CMD_CalibrPoints[AVC_NUM][10] ;
_LOCAL AzzLinSpline_typ FBK_LinSpline[AVC_NUM] , CMD_LinSpline[AVC_NUM];
_LOCAL float VoltRampDown;
_LOCAL float VoltSP , VoltBoost1 , VoltBoost2  ;

_GLOBAL  UINT AVC_Mode;

_LOCAL float KK_StartDly;

_LOCAL float KK_Float;
_LOCAL plcbit KK_JogSpeedTest;

_LOCAL float KK_InVal;
_LOCAL float KK_Analog_Volt ;

StateVectorProxy_typ *_pVect[8];



void  AVC_VOLT_Ctrl(void);
void AVC_Speed_Ctrl(void);
void AVC_Speed_Ctrl_MIG(void);

void  AVC_ENC_Ctrl_2(void);
void  AVC_ENC_Ctrl_Sel(void);

void Start_Arc_Cmd(int i);
void Stop_Arc_Cmd(int i);
float GetArcValue( int k );


void _INIT AVCprcINIT(void)
{
int i , k;


	KK_JogSpeedTest = 0;

	HMI_RegVolt = 4.0;
	HMI_Window = 0.3;
	HMI_RegGain = 2.0;
	KK_NewIdea = 0;

	AVC_Mode = 3;

	BoostVoltage = 10.0;
	BoostTime = 0.050;
	BoostApply = 0;

	//IntegrGain = 1.0;
	IntegrApply = 1;

	 VoltRampDown = 0.5; 

	Process[0].Disable = Process[0].Disable;
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;
	CurCalib.Points[0].num = CurCalib.Points[0].num;	
	DefaultCalib.Points[0].num = DefaultCalib.Points[0].num;
	
	for(i=0 ; i < AVC_NUM ; i++){

		Ramp[i].enable = 1;
		Ramp[i].y_max = 100.0;
		Ramp[i].y_min = -100.0;

		Ramp[i].dy_up = 3.4e+38;
		Ramp[i].dy_down = Ramp[i].dy_up;
	
		
				
			
		FBK_LinSpline[i].NbNodes= 1; /* number of nodes */
		FBK_LinSpline[i].pNodeVec= &FBK_CalibrPoints[i][0]; /* node vector */
		FBK_LinSpline[i].Invert= 0; /* no inverting */
		FBK_LinSpline[i].Smooth= 1; /* smoothing */
		FBK_LinSpline[i].Rness= 100.0; /* smoothing factor if smoothing gets switched on */
		FBK_LinSpline[i].Update= 1; /* update all above values */
		FBK_LinSpline[i].DisMode= 0; /* there's no discontinuity, but set it to a defined value */

		FBK_LinSpline[i].enable= 1; /* enable instance */
		
		FBK_LinSpline[i].x = 0.0;
		AzzLinSpline(&FBK_LinSpline[i]);


		
		memcpy(CMD_CalibrPoints[i] , FBK_CalibrPoints[i] , 10*sizeof(AzzNode_typ)); 
		
		CMD_LinSpline[i].NbNodes= 1; /* number of nodes */
		CMD_LinSpline[i].pNodeVec= &CMD_CalibrPoints[i][0]; /* node vector */
		CMD_LinSpline[i].Invert= 1; /* inverting */
		CMD_LinSpline[i].Smooth= 1; /* smoothing */
		CMD_LinSpline[i].Rness= 100.0; /* smoothing factor if smoothing gets switched on */
		CMD_LinSpline[i].Update= 1; /* update all above values */
		CMD_LinSpline[i].DisMode= 0; /* there's no discontinuity, but set it to a defined value */

		CMD_LinSpline[i].enable= 1; /* enable instance */
		
		CMD_LinSpline[i].x = 0.0;
		AzzLinSpline(&CMD_LinSpline[i]);


		filterAzzMovAvgFlt[i].enable = 1;
  		filterAzzMovAvgFlt[i].base   = 50;

		
	}

	segm = 0;
}

void _CYCLIC AVCprcCYCLIC(void){
int i;


	

	if(!AVC_Param_Set[0].pPrc)
		return;


	if(AVC_Param_Set[0].pPrc->ConfigReady <= 0)
		return;


	KK_InVal = (*AVC_Param_Set[0].pTouchVel)* (-AVC_Param_Set[0].maxBEMFspeed); //* (-24.0);
	

	switch(AVC_Param_Set[0].Mode){


		case 1:

			AVC_ENC_Ctrl_2();

		break;

		case 2:

			AVC_Speed_Ctrl();

		break;

		case 3:								

			AVC_ENC_Ctrl_Sel();

		break;

		case 4:	

			AVC_Speed_Ctrl_MIG();
			//AVC_ENC_Ctrl_2();

		break;

		case 5:


		break;


		default:

			AVC_VOLT_Ctrl();			

		break;

	}
}
void AVC_Speed_Ctrl_MIG(void)
{
int i , k;
float y;
	
		
	for(i=0 ; i < AVC_NUM ; i++){

		//segm = Master.Segm[0];
		
		if(segm >= SEGM_NUM)			
			segm = 0;

		prc = AVC_Param_Set[i].pPrc;

		if(!prc)
			break;

		
		
		vect = AVC_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
				break;

		
		///////////////////////////////////////
		
		//FBK_LinSpline[i].x = vect->FbkVect.x;		

		/// Warning analog input is bipolar and polarity is inverted by (-10.0)
		FBK_LinSpline[i].x = ((float)Hardware[1].Anal_Inp[0] / 32767) * (-10.0); 
		
	    //FBK_LinSpline[i].x = ((float)TML_MTR_INST[2].AnInp[1] / 4095.0) * 5.0; 
		
		//FBK_LinSpline[i].x = KK_Analog_Volt;
		
		AzzLinSpline(&FBK_LinSpline[i]);
		prc->Fbk[0] = FBK_LinSpline[i].y; // cur Arc Volt
		prc->Fbk[1] = 0.0; // Arc Cmd

		filterAzzMovAvgFlt[i].x = FBK_LinSpline[i].y;
		AzzMovAvgFlt(&filterAzzMovAvgFlt[i]);		
		//prc->Fbk[0] =  filterAzzMovAvgFlt[i].y;
		
 				
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsRESTART:

					prc->xCMD = prcsRefrCalib;
						
				break;	


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

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


				case prcsIDLE:


				break;	

				case prcsRESTART:

					
					prc->_step = prcsIDLE;
					prc->xCMD = prcsRefrCalib;
						
				break;	

				
				case prcsSTART:

					prc->xCMD = prcsRefrCalib;
							
					if((prc->_step == 0) || (prc->_step > 4)){

						if(!ErrorMgr.Error){
							TimerStart(&Tmr[i], 0.5);
							repeatCnt[i] = 0;
							prc->_step = 1;
						}	

					}	
				

				break;

				case prcsHalt:


					prc->_step = prcsIDLE;


				break;

				case prcsQuickSTOP:
				case prcsSTOP:

					if(prc->TestMode){

						vect->InVal = 0;
						//vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pStopDly));
				
						prc->_step = 22;


					}	

					else if(prc->_step != 0) {

						switch(prc->xCMD){

							case prcsQuickSTOP:

								TimerStart(&Tmr[i], 0.01);

							break;
							
							case prcsSTOP:

								TimerStart(&Tmr[i],(*AVC_Param_Set[i].pStopDly));


							break;	


						}

					
						prc->_step = 20;

					}	

									
				break;

				case prcsDirectCntrl:


					if(KK_JogSpeedTest){

						if(prc->_step != 80)
							prc->_step = 80;

					}else{
						
						if(prc->_step != 60)
							prc->_step = 60;
					}
						
					/*
					if(prc->_step != 80)
						prc->_step = 80;
					*/	
					

				break;


				default:

					prc->_step = prcsIDLE;
					

				break;	
				
			}
		}
		
		

		if(prc->xCMD == prcsRefrCalib){

			if(AVC_Param_Set[i].pCalibPoints){

				AVC_Param_Set[i].pCalibPoints->num = 4;

				for(k = 0 ; k < AVC_Param_Set[i].pCalibPoints->num;  k++){

					FBK_CalibrPoints[i][k].Y = 6.0 * k;
					//FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalib->Points[CALIB_TIG_VOLT_FBK+i].val[k];
					FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalibPoints->val[k];

					CMD_CalibrPoints[i][k].X = FBK_CalibrPoints[i][k].X;
					CMD_CalibrPoints[i][k].Y = FBK_CalibrPoints[i][k].Y;

			    }	

				FBK_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */				
				FBK_LinSpline[i].Update = 1;
				AzzLinSpline(&FBK_LinSpline[i]);
				FBK_LinSpline[i].enable = 1;

				CMD_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */			
				CMD_LinSpline[i].Update = 1;
				AzzLinSpline(&CMD_LinSpline[i]);
				CMD_LinSpline[i].enable = 1;	
			}
	

		}		
		
		prc->xCMD = prcsIDLE;
		///////////////////////////////////

		if(TigSynchMode == 0) // Straight
			VoltSP = (*AVC_Param_Set[i].pVoltSP);
		else
			VoltSP = (*AVC_Param_Set[i].pBkgVoltSP);

		if(VoltSP <= 0.0)
			VoltSP = 10.0;

		VoltBoost1 = (*AVC_Param_Set[i].pBoost1);
		VoltBoost2 = (*AVC_Param_Set[i].pBoost2);

		/*
		switch(AVC_Param_Set[i].BoostCmd){

			case 1:
				
				VoltSP += VoltBoost1;

			break;

			case 2:
				
				VoltSP += VoltBoost2;

			break;

			default: 
			break;

		}

		if(VoltSP < 0.0)
			VoltSP = 0.0;
		*/	

		//////////////////////
		switch(prc->_step){


			case prcsIDLE:  // IDLE - stopped

				
				prc->Status = prcsSTATUS_Stopped;				

				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;
									
				vect->InVal = 0.0;


				KK_StartDly = (*AVC_Param_Set[i].pStartDly);
				
												
			break;



			case 1: // Start

				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else if(TimerIsTimeOut(&Tmr[i])) {

					//vect->xCMD = VECT_ClearSignFilter;

					// (*AVC_Param_Set[i].pTouchVolt)
					if((prc->Fbk[0] > 2.0 )|| prc->TestMode) {

						vect->InVal = (*AVC_Param_Set[i].pTouchVel)* (-AVC_Param_Set[i].maxBEMFspeed); //* (-24.0);


						if(prc->TestMode)
							TimerStart(&Tmr[i], 0.5);
							
						else					
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pTouchTm));

						prc->Status = prcsSTATUS_ForceON;
						prc->_step++;

					}
					
					else {
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_VOLT_ERR,0);

						prc->_step = 0;

					}
						
						
				}
														
			break;


			case 2: // Wait for below Touch Voltage or timeout

				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(TimerIsTimeOut(&Tmr[i])){

					if(prc->TestMode){

						vect->InVal = 0; 
						prc->Status = prcsSTATUS_Run;

						
						//if((vect->FbkVect.Mode != vectSt_Mode_Volt))
							//vect->Mode = vectSt_Mode_Volt;

						prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP); // Arc Volt Cmd		
						
						//prc->Fbk[0] = prc->Fbk[1]; // cur ARC Volt equal commanded
						prc->Fbk[0] = 0.0;
						break;

					}	

					repeatCnt[i]++;

					if(repeatCnt[i] >= (*AVC_Param_Set[i].pTouchRepeat)){

												
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_TOUCH_TMO,1);

						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)* AVC_Param_Set[i].maxBEMFspeed;// * (24.0);
						vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
										
						//prc->_step++;
						prc->_step = 23;

										
					}
					else { 

						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRetractTm));						
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].maxBEMFspeed;// * (24.0);
						
						prc->_step = 1;

					}						
					
							
				}

				
				else if(!prc->TestMode){
					
						if((prc->Fbk[0] - (*AVC_Param_Set[i].pTouchVolt)) < 0 ){  //  Success Voltage below treshold  Arc Start

							vect->InVal = (*AVC_Param_Set[i].pToSPVel) * AVC_Param_Set[i].maxBEMFspeed; //* (24.0);
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pVoltSPTimeOut));						
												
						
							prc->_step++;


						}
				}
				
				
														
			break;

			case 3: // Wait for  above VoltageInit or timeout

				
				if(TimerIsTimeOut(&Tmr[i])){

					Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_SP_TMO,0);
					prc->_step = 0;
					
							
				}

				else if((prc->Fbk[0] - (*AVC_Param_Set[i].pVoltInit)) > (-0.5)){ 

					prc->Status = prcsSTATUS_StartDly;		

					prc->Status = prcsSTATUS_Run;
					vect->Mode = vectSt_Mode_Vel;					

					vect->InVal = 0;

					vect->xCMD = VECT_UpdateSignFilter;
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pStartDly));
					KK_StartDly = (*AVC_Param_Set[i].pStartDly);
					
					prc->_step++;
					
				}
			
				else{
			
					vect->InVal = (*AVC_Param_Set[i].pToSPVel)* AVC_Param_Set[i].maxBEMFspeed; // * (24.0);


				}
					
				
														
			break;


			case 4:

					if((vect->FbkVect.Mode != vectSt_Mode_Vel)){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Vel;

					}	


					if(TimerIsTimeOut(&Tmr[i])){

						/*
						if((AVC_Mode) != 3){ // Not BB regulation
							vect->Mode = vectSt_Mode_Pos;	
						}
						*/

						vect->Mode = vectSt_Mode_Vel;

						

						/*
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));
							//Ramp[i].dy_up = fabsf((*AVC_Param_Set[i].pSpeedSp - Ramp[i].y)/(*AVC_Param_Set[i].pRampUp));
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = (*AVC_Param_Set[i].pVoltSP);
						AzzRamp(&Ramp[i]);

						*/

						
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf((VoltSP)/(*AVC_Param_Set[i].pRampUp));	
							//Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));						
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						//Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].y_set = VoltSP;
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);						
						vect->InVal =  CMD_LinSpline[i].y;

						if((*AVC_Param_Set[i].pRampUp) <= 0.0)
							TimerStart(&Tmr[i], 0.1);
						else							
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampUp));

						prc->_step ++;


					}


			break;				

			case 5:
					prc->Param[0] = 0;

					
					//if((vect->FbkVect.Mode == vectSt_Mode_Pos)){	

					if(1){

						AzzRamp(&Ramp[i]);

						CMD_LinSpline[i].x = GetArcValue(i);						
						//CMD_LinSpline[i].x = Ramp[i].y;
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y ;

						prc->Fbk[1] = Ramp[i].y; //ARC Cmd

					
						if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

							Ramp[i].dy_up = 50.0; // 3 volt per second
							Ramp[i].dy_down = Ramp[i].dy_up;

							/*
							if((AVC_Mode) != 3) // Not BB regulation					
								vect->fParam[0] = (*AVC_Param_Set[i].pBB_Window);							
							else
								vect->fParam[0] = 0.0;

							vect->xCMD = VECT_Set_MaxError;
							*/

							prc->Status = prcsSTATUS_AVC_OK;								

							prc->_step++;
						}


					}
			
			break;	


			case 6:		
		

				if(prc->Param[0]){
					
					vect->Mode = vectSt_Mode_Volt;					

					vect->InVal = 0;
					prc->_step++;

				}	

				else {	

					Ramp[i].x = VoltSP;
					AzzRamp(&Ramp[i]);										

					CMD_LinSpline[i].x = GetArcValue(i);
					prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y ;	


				}
				


			break;	


			case 7:

				
				if(vect->FbkVect.Mode == vectSt_Mode_Vel){

					//vect->InVal = 0;

					vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;

					prc->Status = prcsSTATUS_AVC_Hold;

					if(!prc->Param[0]){

						/*
						if((AVC_Mode) != 3){ // Not BB Regulation
							vect->Mode = vectSt_Mode_Pos;
						}
						*/
						
						vect->Mode = vectSt_Mode_Vel;			
						
						
						Ramp[i].dy_up = 50.0; // 50 volt per second
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y;

						prc->_step = 5;

					}
				}
				else{

					vect->Mode = vectSt_Mode_Volt;
					vect->InVal = 0;

				}
					
				


			break;	





			case 20:

				
				prc->Status = prcsSTATUS_StopDly;

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(!TimerIsTimeOut(&Tmr[i]))
				 	break;

				
				prc->Status = prcsSTATUS_RampDwn;
				
				if((*AVC_Param_Set[i].pRampDwn) >0){

					
					//Ramp[i].dy_up = fabsf((prc->Fbk[0]- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
					Ramp[i].dy_up = fabsf((VoltSP- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
									
					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].set = 1;
					Ramp[i].x = (*AVC_Param_Set[i].pFinalVolt);
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;
					
					//CMD_LinSpline[i].x = Ramp[i].y;
					CMD_LinSpline[i].x = GetArcValue(i);
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y;	
					
				}
				
				TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn));
				vect->xCMD = VECT_ClearSignFilter;					
				prc->_step++;
				

			break;	


			case 21:

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				AzzRamp(&Ramp[i]);
			
				//CMD_LinSpline[i].x = Ramp[i].y;
				CMD_LinSpline[i].x = GetArcValue(i);
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if((vect->FbkWarning!= 0))
					prc->_step = 0;

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					vect->InVal = 0.0;
					//vect->Mode = vectSt_Mode_Vel;

					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pHoldTm));

					prc->Status = prcsSTATUS_RampDwn;

					prc->_step++;
				}


			break;	


			


			case 22:

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if((vect->FbkWarning != 0))
					prc->_step = 0;

				else if(TimerIsTimeOut(&Tmr[i])){

					vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)* AVC_Param_Set[i].maxBEMFspeed; // * 24.0;
				
					vect->Mode = vectSt_Mode_Vel;					
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
									
					prc->_step++;

				}


			break;	

			case 23:				

				if(TimerIsTimeOut(&Tmr[i]) ||(vect->FbkWarning!= 0))
					prc->_step = 0;
				
				

			break;	

			
			case 60: ///  DirectCntrl

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

						if(prc->fParam[0] != 0.0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]) * AVC_Param_Set[i].MaxSpeedJog;
							
						else 
							prc->_step = 0;											
						
					}
												
				}			
			

			break;

			
			case 80: ///  DirectCntrl by speed BEMF

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

											
						if(prc->fParam[0] != 0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] * AVC_Param_Set[i].maxBEMFspeed/24;
						else 
							prc->_step = prcsIDLE;
							//prc->_step ++;									
					
						
					}
												
				}
			

			break;

			


			case 0xFF:  // Disable

				if(vect->FbkVect.Mode != vectSt_Mode_Idle)
					vect->Mode = vectSt_Mode_Idle;

				if(!prc->Disable)
					prc->_step = 0;
			

			break;	

			
		}

		// Throw error when AVC is running and Limits are triggered 
		if((vect->FbkWarning!= 0) ){

			if(!LimitsFilterTmr[i].IN){
				LimitsFilterTmr[i].IN = 1;
				LimitsFilterTmr[i].PT = (int) (((*AVC_Param_Set[i].pLimitsFilterTm) * 1000.0)+0.5);
			}
				
			
		}
		else 
			LimitsFilterTmr[i].IN = 0;
		

		if((prc->_step > prcsIDLE) && (prc->_step < 50) ) {

			//if(LimitsFilterTmr[i].Q && ((*AVC_Param_Set[i].pLimitsFilterTm)!= 0.0 ))			
				//Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_AVC_LIMIT_ERR,i);

		}	

		// Na wyrazne rzadanie zmienilem
		/*
		if(prc->Status == prcsSTATUS_Stopped){
				//prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		}	
		else
			prc->Fbk[1] = prc->Fbk[0];
		*/	

		if(TigSynchMode == 3 && (AVCSynchMode == 1)) // TIG Sync IN Out and AVC Back Locked
			prc->Fbk[1] = ((*AVC_Param_Set[i].pVoltInSP) +(*AVC_Param_Set[i].pVoltOutSP))/2.0; 
		else
			///prc->Fbk[1] = VoltSP;
			// 12 July2024
			prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);


		//////////////////////////////////	
		// Pseudo Servo
		HMI_Mode = (AVC_Mode);
		//if((AVC_Mode)== 3)){ // BB regulation
		if(1){ // BB regulation

			HMI_Window = (*AVC_Param_Set[i].pBB_Window);
			HMI_RegGain = (*AVC_Param_Set[i].pBB_Gain);
			HMI_RegVolt = (*AVC_Param_Set[i].pBB_Vel) * 24.0;
			IntegrGain = (*AVC_Param_Set[i].pBB_Integr) * 2.0;

			HMI_Error = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;
			
		
			if((prc->_step == 5) || (prc->_step == 6) || (prc->_step == 20)|| (prc->_step == 21)){

				//y = filterAzzMovAvgFlt[i].y - Ramp[i].y; 

				y = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;

				if(fabsf(y) > HMI_Window )
					_IntegrError[i] += IntegrGain * y;
				else
					_IntegrError[i] = 0.0;

				

				if(y > HMI_Window)					
					vect->InVal = ((y - HMI_Window) * HMI_RegGain) * (-1.0);
					//vect->InVal = ((y - HMI_Window) * HMI_RegGain + HMI_RegVolt) * (-1.0);

				else if(y  <  (HMI_Window *(-1.0)))
					vect->InVal = (-y - HMI_Window)* HMI_RegGain;
					//vect->InVal = (-y - HMI_Window)* HMI_RegGain  + HMI_RegVolt;
										
				else
					vect->InVal = 0;


				if(IntegrApply)
					vect->InVal -= _IntegrError[i];

				// Was added to scale voltage control to BEMF speed
				//vect->InVal = vect->InVal * AVC_Param_Set[i].maxBEMFspeed/24.0;
				vect->InVal = vect->InVal * AVC_Param_Set[i].maxBEMFspeed/240.0;
				
						
				if(Set_AVC_BB_Gain == 0)
					vect->InVal = 0;

			
			}
		}


		
		TimerExecute(&LimitsFilterTmr[i]);
		TimerExecute(&Tmr[i]);


		
		
	}



}


void AVC_Speed_Ctrl(void)
{
int i , k;
float y;
	
		
	for(i=0 ; i < AVC_NUM ; i++){

		//segm = Master.Segm[0];
		
		if(segm >= SEGM_NUM)			
			segm = 0;

		prc = AVC_Param_Set[i].pPrc;

		if(!prc)
			break;

		
		
		vect = AVC_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
				break;

		
		///////////////////////////////////////
		
		//FBK_LinSpline[i].x = vect->FbkVect.x;		

		if(ProjectConfig == project_US_Orbital)
			FBK_LinSpline[i].x = ((float)Hardware[1].Anal_Inp[1] / 32767) * 10.0; 
		else
			FBK_LinSpline[i].x = ((float)TML_MTR_INST[2].AnInp[1] / 4095.0) * 5.0; 
		
		
		
		AzzLinSpline(&FBK_LinSpline[i]);
		prc->Fbk[0] = FBK_LinSpline[i].y; // cur Arc Volt
		prc->Fbk[1] = 0.0; // Arc Cmd

		filterAzzMovAvgFlt[i].x = FBK_LinSpline[i].y;
		AzzMovAvgFlt(&filterAzzMovAvgFlt[i]);		
		//prc->Fbk[0] =  filterAzzMovAvgFlt[i].y;
		
 				
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsRESTART:

					prc->xCMD = prcsRefrCalib;
						
				break;	


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

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


				case prcsIDLE:


				break;	

				case prcsRESTART:

					
					prc->_step = prcsIDLE;
					prc->xCMD = prcsRefrCalib;
						
				break;	

				
				case prcsSTART:

					prc->xCMD = prcsRefrCalib;
							
					if((prc->_step == 0) || (prc->_step > 4)){

						if(!ErrorMgr.Error){
							TimerStart(&Tmr[i], 0.5);
							repeatCnt[i] = 0;
							prc->_step = 1;
						}	

					}	
				

				break;

				case prcsHalt:


					prc->_step = prcsIDLE;


				break;

				case prcsQuickSTOP:
				case prcsSTOP:

					if(prc->TestMode){

						vect->InVal = 0;
						//vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pStopDly));
				
						prc->_step = 22;


					}	

					else if(prc->_step != 0) {

						switch(prc->xCMD){

							case prcsQuickSTOP:

								TimerStart(&Tmr[i], 0.01);

							break;
							
							case prcsSTOP:

								TimerStart(&Tmr[i],(*AVC_Param_Set[i].pStopDly));


							break;	


						}

					
						prc->_step = 20;

					}	

									
				break;

				case prcsDirectCntrl:


					if(KK_JogSpeedTest){

						if(prc->_step != 80)
							prc->_step = 80;

					}else{
						
						if(prc->_step != 60)
							prc->_step = 60;
					}
						
					/*
					if(prc->_step != 80)
						prc->_step = 80;
					*/	
					

				break;


				default:

					prc->_step = prcsIDLE;
					

				break;	
				
			}
		}
		
		

		if(prc->xCMD == prcsRefrCalib){

			if(AVC_Param_Set[i].pCalibPoints){

				AVC_Param_Set[i].pCalibPoints->num = 4;

				for(k = 0 ; k < AVC_Param_Set[i].pCalibPoints->num;  k++){

					FBK_CalibrPoints[i][k].Y = 6.0 * k;
					//FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalib->Points[CALIB_TIG_VOLT_FBK+i].val[k];
					FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalibPoints->val[k];

					CMD_CalibrPoints[i][k].X = FBK_CalibrPoints[i][k].X;
					CMD_CalibrPoints[i][k].Y = FBK_CalibrPoints[i][k].Y;

			    }	

				FBK_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */				
				FBK_LinSpline[i].Update = 1;
				AzzLinSpline(&FBK_LinSpline[i]);
				FBK_LinSpline[i].enable = 1;

				CMD_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */			
				CMD_LinSpline[i].Update = 1;
				AzzLinSpline(&CMD_LinSpline[i]);
				CMD_LinSpline[i].enable = 1;	
			}
	

		}		
		
		prc->xCMD = prcsIDLE;
		///////////////////////////////////

		if(TigSynchMode == 0) // Straight
			VoltSP = (*AVC_Param_Set[i].pVoltSP);
		else
			VoltSP = (*AVC_Param_Set[i].pBkgVoltSP);

		if(VoltSP <= 0.0)
			VoltSP = 10.0;

		VoltBoost1 = (*AVC_Param_Set[i].pBoost1);
		VoltBoost2 = (*AVC_Param_Set[i].pBoost2);

		/*
		switch(AVC_Param_Set[i].BoostCmd){

			case 1:
				
				VoltSP += VoltBoost1;

			break;

			case 2:
				
				VoltSP += VoltBoost2;

			break;

			default: 
			break;

		}

		if(VoltSP < 0.0)
			VoltSP = 0.0;
		*/	

		//////////////////////
		switch(prc->_step){


			case prcsIDLE:  // IDLE - stopped

				
				prc->Status = prcsSTATUS_Stopped;				

				if((vect->FbkVect.Mode != vectSt_Mode_Volt))
					vect->Mode = vectSt_Mode_Volt;
									
				vect->InVal = 0.0;


				KK_StartDly = (*AVC_Param_Set[i].pStartDly);
				
												
			break;



			case 1: // Start

				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else if(TimerIsTimeOut(&Tmr[i])) {

					//vect->xCMD = VECT_ClearSignFilter;

					// (*AVC_Param_Set[i].pTouchVolt)
					if((prc->Fbk[0] > 2.0 )|| prc->TestMode) {

						vect->InVal = (*AVC_Param_Set[i].pTouchVel)* (-AVC_Param_Set[i].maxBEMFspeed); //* (-24.0);


						if(prc->TestMode)
							TimerStart(&Tmr[i], 0.5);
							
						else					
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pTouchTm));

						prc->Status = prcsSTATUS_ForceON;
						prc->_step++;

					}
					
					else {
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_VOLT_ERR,0);

						prc->_step = 0;

					}
						
						
				}
														
			break;


			case 2: // Wait for below Touch Voltage or timeout

				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(TimerIsTimeOut(&Tmr[i])){

					if(prc->TestMode){

						vect->InVal = 0; 
						prc->Status = prcsSTATUS_Run;

						
						if((vect->FbkVect.Mode != vectSt_Mode_Volt))
							vect->Mode = vectSt_Mode_Volt;

						prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP); // Arc Volt Cmd		
						
						//prc->Fbk[0] = prc->Fbk[1]; // cur ARC Volt equal commanded
						prc->Fbk[0] = 0.0;
						break;

					}	

					repeatCnt[i]++;

					if(repeatCnt[i] >= (*AVC_Param_Set[i].pTouchRepeat)){

												
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_TOUCH_TMO,1);

						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)* AVC_Param_Set[i].maxBEMFspeed;// * (24.0);
						vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
										
						//prc->_step++;
						prc->_step = 23;

										
					}
					else { 

						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRetractTm));						
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].maxBEMFspeed;// * (24.0);
						
						prc->_step = 1;

					}						
					
							
				}

				
				else if(!prc->TestMode){
					
						if((prc->Fbk[0] - (*AVC_Param_Set[i].pTouchVolt)) < 0 ){  //  Success Voltage below treshold  Arc Start

							vect->InVal = (*AVC_Param_Set[i].pToSPVel) * AVC_Param_Set[i].maxBEMFspeed; //* (24.0);
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pVoltSPTimeOut));						
												
						
							prc->_step++;


						}
				}
				
				
														
			break;

			case 3: // Wait for  above VoltageInit or timeout

				
				if(TimerIsTimeOut(&Tmr[i])){

					Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_SP_TMO,0);
					prc->_step = 0;
					
							
				}

				else if((prc->Fbk[0] - (*AVC_Param_Set[i].pVoltInit)) > (-0.5)){ 

					prc->Status = prcsSTATUS_StartDly;		

					prc->Status = prcsSTATUS_Run;
					vect->Mode = vectSt_Mode_Vel;					

					vect->InVal = 0;

					vect->xCMD = VECT_UpdateSignFilter;
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pStartDly));
					KK_StartDly = (*AVC_Param_Set[i].pStartDly);
					
					prc->_step++;
					
				}
			
				else{
			
					vect->InVal = (*AVC_Param_Set[i].pToSPVel)* AVC_Param_Set[i].maxBEMFspeed; // * (24.0);


				}
					
				
														
			break;


			case 4:

					if((vect->FbkVect.Mode != vectSt_Mode_Vel)){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Vel;

					}	


					if(TimerIsTimeOut(&Tmr[i])){

						/*
						if((AVC_Mode) != 3){ // Not BB regulation
							vect->Mode = vectSt_Mode_Pos;	
						}
						*/

						vect->Mode = vectSt_Mode_Vel;

						

						/*
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));
							//Ramp[i].dy_up = fabsf((*AVC_Param_Set[i].pSpeedSp - Ramp[i].y)/(*AVC_Param_Set[i].pRampUp));
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = (*AVC_Param_Set[i].pVoltSP);
						AzzRamp(&Ramp[i]);

						*/

						
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf((VoltSP)/(*AVC_Param_Set[i].pRampUp));	
							//Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));						
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						//Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].y_set = VoltSP;
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);						
						vect->InVal =  CMD_LinSpline[i].y;

						if((*AVC_Param_Set[i].pRampUp) <= 0.0)
							TimerStart(&Tmr[i], 0.1);
						else							
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampUp));

						prc->_step ++;


					}


			break;				

			case 5:
					prc->Param[0] = 0;

					
					//if((vect->FbkVect.Mode == vectSt_Mode_Pos)){	

					if(1){

						AzzRamp(&Ramp[i]);

						CMD_LinSpline[i].x = GetArcValue(i);						
						//CMD_LinSpline[i].x = Ramp[i].y;
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y ;

						prc->Fbk[1] = Ramp[i].y; //ARC Cmd

					
						if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

							Ramp[i].dy_up = 50.0; // 3 volt per second
							Ramp[i].dy_down = Ramp[i].dy_up;

							/*
							if((AVC_Mode) != 3) // Not BB regulation					
								vect->fParam[0] = (*AVC_Param_Set[i].pBB_Window);							
							else
								vect->fParam[0] = 0.0;

							vect->xCMD = VECT_Set_MaxError;
							*/

							prc->Status = prcsSTATUS_AVC_OK;								

							prc->_step++;
						}


					}
			
			break;	


			case 6:		
		

				if(prc->Param[0]){
					
					vect->Mode = vectSt_Mode_Volt;					

					vect->InVal = 0;
					prc->_step++;

				}	

				else {	

					Ramp[i].x = VoltSP;
					AzzRamp(&Ramp[i]);										

					CMD_LinSpline[i].x = GetArcValue(i);
					prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y ;	


				}
				


			break;	


			case 7:

				
				if(vect->FbkVect.Mode == vectSt_Mode_Vel){

					//vect->InVal = 0;

					vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;

					prc->Status = prcsSTATUS_AVC_Hold;

					if(!prc->Param[0]){

						/*
						if((AVC_Mode) != 3){ // Not BB Regulation
							vect->Mode = vectSt_Mode_Pos;
						}
						*/
						
						vect->Mode = vectSt_Mode_Vel;			
						
						
						Ramp[i].dy_up = 50.0; // 50 volt per second
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y;

						prc->_step = 5;

					}
				}
				else{

					vect->Mode = vectSt_Mode_Volt;
					vect->InVal = 0;

				}
					
				


			break;	





			case 20:

				
				prc->Status = prcsSTATUS_StopDly;

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(!TimerIsTimeOut(&Tmr[i]))
				 	break;

				
				prc->Status = prcsSTATUS_RampDwn;
				
				if((*AVC_Param_Set[i].pRampDwn) >0){

					
					//Ramp[i].dy_up = fabsf((prc->Fbk[0]- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
					Ramp[i].dy_up = fabsf((VoltSP- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
									
					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].set = 1;
					Ramp[i].x = (*AVC_Param_Set[i].pFinalVolt);
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;
					
					//CMD_LinSpline[i].x = Ramp[i].y;
					CMD_LinSpline[i].x = GetArcValue(i);
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y;	
					
				}
				
				TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn));
				vect->xCMD = VECT_ClearSignFilter;					
				prc->_step++;
				

			break;	


			case 21:

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				AzzRamp(&Ramp[i]);
			
				//CMD_LinSpline[i].x = Ramp[i].y;
				CMD_LinSpline[i].x = GetArcValue(i);
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if((vect->FbkWarning!= 0))
					prc->_step = 0;

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					vect->InVal = 0.0;
					//vect->Mode = vectSt_Mode_Vel;

					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pHoldTm));

					prc->Status = prcsSTATUS_RampDwn;

					prc->_step++;
				}


			break;	


			


			case 22:

				
				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if((vect->FbkWarning != 0))
					prc->_step = 0;

				else if(TimerIsTimeOut(&Tmr[i])){

					vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)* AVC_Param_Set[i].maxBEMFspeed; // * 24.0;
				
					vect->Mode = vectSt_Mode_Vel;					
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
									
					prc->_step++;

				}


			break;	

			case 23:				

				if(TimerIsTimeOut(&Tmr[i]) ||(vect->FbkWarning!= 0))
					prc->_step = 0;
				
				

			break;	

			
			case 60: ///  DirectCntrl

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

											
						if(prc->fParam[0] != 0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
							//vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] * 24.0 ;
						else 
							prc->_step = prcsIDLE;
							//prc->_step ++;									
					
						
					}
												
				}
			

			break;

			case 61: /// Direct Cntrl - Ramp Down

					if(vect->InVal > VoltRampDown)
						vect->InVal -= VoltRampDown;
					else if(vect->InVal < (VoltRampDown * (-1)))
						vect->InVal += VoltRampDown;
					else 
						prc->_step = prcsIDLE;
			
			break;

			case 80: ///  DirectCntrl by speed BEMF

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

											
						if(prc->fParam[0] != 0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] * AVC_Param_Set[i].maxBEMFspeed/24;
						else 
							prc->_step = prcsIDLE;
							//prc->_step ++;									
					
						
					}
												
				}
			

			break;

			


			case 0xFF:  // Disable

				if(vect->FbkVect.Mode != vectSt_Mode_Idle)
					vect->Mode = vectSt_Mode_Idle;

				if(!prc->Disable)
					prc->_step = 0;
			

			break;	

			
		}

		// Throw error when AVC is running and Limits are triggered 
		if((vect->FbkWarning!= 0) ){

			if(!LimitsFilterTmr[i].IN){
				LimitsFilterTmr[i].IN = 1;
				LimitsFilterTmr[i].PT = (int) (((*AVC_Param_Set[i].pLimitsFilterTm) * 1000.0)+0.5);
			}
				
			
		}
		else 
			LimitsFilterTmr[i].IN = 0;
		

		if((prc->_step > prcsIDLE) && (prc->_step < 50) ) {

			//if(LimitsFilterTmr[i].Q && ((*AVC_Param_Set[i].pLimitsFilterTm)!= 0.0 ))			
				//Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_AVC_LIMIT_ERR,i);

		}	

		// Na wyrazne rzadanie zmienilem
		/*
		if(prc->Status == prcsSTATUS_Stopped){
				//prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		}	
		else
			prc->Fbk[1] = prc->Fbk[0];
		*/	

		if(TigSynchMode == 3 && (AVCSynchMode == 1)) // TIG Sync IN Out and AVC Back Locked
			prc->Fbk[1] = ((*AVC_Param_Set[i].pVoltInSP) +(*AVC_Param_Set[i].pVoltOutSP))/2.0; 
		else
			///prc->Fbk[1] = VoltSP;
			// 12 July2024
			prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);


		//////////////////////////////////	
		// Pseudo Servo
		HMI_Mode = (AVC_Mode);
		//if((AVC_Mode)== 3)){ // BB regulation
		if(1){ // BB regulation

			HMI_Window = (*AVC_Param_Set[i].pBB_Window);
			HMI_RegGain = (*AVC_Param_Set[i].pBB_Gain);
			HMI_RegVolt = (*AVC_Param_Set[i].pBB_Vel) * 24.0;
			IntegrGain = (*AVC_Param_Set[i].pBB_Integr) * 2.0;

			HMI_Error = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;
			
		
			if((prc->_step == 5) || (prc->_step == 6) || (prc->_step == 20)|| (prc->_step == 21)){

				//y = filterAzzMovAvgFlt[i].y - Ramp[i].y; 

				y = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;

				if(fabsf(y) > HMI_Window )
					_IntegrError[i] += IntegrGain * y;
				else
					_IntegrError[i] = 0.0;

				

				if(y > HMI_Window)					
					vect->InVal = ((y - HMI_Window) * HMI_RegGain) * (-1.0);
					//vect->InVal = ((y - HMI_Window) * HMI_RegGain + HMI_RegVolt) * (-1.0);

				else if(y  <  (HMI_Window *(-1.0)))
					vect->InVal = (-y - HMI_Window)* HMI_RegGain;
					//vect->InVal = (-y - HMI_Window)* HMI_RegGain  + HMI_RegVolt;
										
				else
					vect->InVal = 0;


				if(IntegrApply)
					vect->InVal -= _IntegrError[i];

				// Was added to scale voltage control to BEMF speed
				//vect->InVal = vect->InVal * AVC_Param_Set[i].maxBEMFspeed/24.0;
				vect->InVal = vect->InVal * AVC_Param_Set[i].maxBEMFspeed/240.0;
				
						
				if(Set_AVC_BB_Gain == 0)
					vect->InVal = 0;

			
			}
		}


		
		TimerExecute(&LimitsFilterTmr[i]);
		TimerExecute(&Tmr[i]);


		
		
	}



}


void AVC_VOLT_Ctrl(void)
{
int i , k;
float y;
	
		
	for(i=0 ; i < AVC_NUM ; i++){

		//segm = Master.Segm[0];
		
		if(segm >= SEGM_NUM)			
			segm = 0;

		prc = AVC_Param_Set[i].pPrc;

		if(!prc)
			break;

		
		
		vect = AVC_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
				break;

		
		///////////////////////////////////////
		
		//FBK_LinSpline[i].x = vect->FbkVect.x;		

		if(ProjectConfig == project_US_Orbital)
			FBK_LinSpline[i].x = ((float)Hardware[1].Anal_Inp[1] / 32767) * 10.0; 
		else
			FBK_LinSpline[i].x = ((float)TML_MTR_INST[2].AnInp[1] / 4095.0) * 5.0; 
		
		
		
		AzzLinSpline(&FBK_LinSpline[i]);
		prc->Fbk[0] = FBK_LinSpline[i].y; // cur Arc Volt
		prc->Fbk[1] = 0.0; // Arc Cmd

		filterAzzMovAvgFlt[i].x = FBK_LinSpline[i].y;
		AzzMovAvgFlt(&filterAzzMovAvgFlt[i]);		
		//prc->Fbk[0] =  filterAzzMovAvgFlt[i].y;
		
 				
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsRESTART:

					prc->xCMD = prcsRefrCalib;
						
				break;	


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

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


				case prcsIDLE:


				break;	

				case prcsRESTART:

					
					prc->_step = prcsIDLE;
					prc->xCMD = prcsRefrCalib;
						
				break;	

				
				case prcsSTART:

					prc->xCMD = prcsRefrCalib;
							
					if((prc->_step == 0) || (prc->_step > 4)){

						if(!ErrorMgr.Error){
							TimerStart(&Tmr[i], 0.5);
							repeatCnt[i] = 0;
							prc->_step = 1;
						}	

					}	
				

				break;

				case prcsHalt:


					prc->_step = prcsIDLE;


				break;

				case prcsQuickSTOP:
				case prcsSTOP:

					if(prc->TestMode){

						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pStopDly));
				
						prc->_step = 22;


					}	

					else if(prc->_step != 0) {

						switch(prc->xCMD){

							case prcsQuickSTOP:

								TimerStart(&Tmr[i], 0.01);

							break;
							
							case prcsSTOP:

								TimerStart(&Tmr[i],(*AVC_Param_Set[i].pStopDly));


							break;	


						}

					
						prc->_step = 20;

					}	

									
				break;

				case prcsDirectCntrl:
				//case prcsJOGpos:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;


				default:

					prc->_step = prcsIDLE;
					

				break;	
				
			}
		}
		
		

		if(prc->xCMD == prcsRefrCalib){

			if(AVC_Param_Set[i].pCalibPoints){

				AVC_Param_Set[i].pCalibPoints->num = 4;

				for(k = 0 ; k < AVC_Param_Set[i].pCalibPoints->num;  k++){

					FBK_CalibrPoints[i][k].Y = 6.0 * k;
					//FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalib->Points[CALIB_TIG_VOLT_FBK+i].val[k];
					FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalibPoints->val[k];

					CMD_CalibrPoints[i][k].X = FBK_CalibrPoints[i][k].X;
					CMD_CalibrPoints[i][k].Y = FBK_CalibrPoints[i][k].Y;

			    }	

				FBK_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */				
				FBK_LinSpline[i].Update = 1;
				AzzLinSpline(&FBK_LinSpline[i]);
				FBK_LinSpline[i].enable = 1;

				CMD_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */			
				CMD_LinSpline[i].Update = 1;
				AzzLinSpline(&CMD_LinSpline[i]);
				CMD_LinSpline[i].enable = 1;	
			}
	

		}		
		
		prc->xCMD = prcsIDLE;
		///////////////////////////////////

		if(TigSynchMode == 0) // Straight
			VoltSP = (*AVC_Param_Set[i].pVoltSP);
		else
			VoltSP = (*AVC_Param_Set[i].pBkgVoltSP);

		if(VoltSP <= 0.0)
			VoltSP = 10.0;

		VoltBoost1 = (*AVC_Param_Set[i].pBoost1);
		VoltBoost2 = (*AVC_Param_Set[i].pBoost2);


		/*

		switch(AVC_Param_Set[i].BoostCmd){

			case 1:
				
				VoltSP += VoltBoost1;

			break;

			case 2:
				
				VoltSP += VoltBoost2;

			break;

			default: 
			break;

		}
		
		if(VoltSP < 0.0)
			VoltSP = 0.0;

		*/	

		//////////////////////
		switch(prc->_step){


			case prcsIDLE:  // IDLE - stopped

				
				prc->Status = prcsSTATUS_Stopped;				

				if((vect->FbkVect.Mode != vectSt_Mode_Volt))
					vect->Mode = vectSt_Mode_Volt;
									
				vect->InVal = 0.0;


				KK_StartDly = (*AVC_Param_Set[i].pStartDly);
				
												
			break;



			case 1: // Start

				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}	
				else if(TimerIsTimeOut(&Tmr[i])) {

					vect->xCMD = VECT_ClearSignFilter;

					// (*AVC_Param_Set[i].pTouchVolt)
					if((prc->Fbk[0] > 2.0 )|| prc->TestMode) {

						vect->InVal = (*AVC_Param_Set[i].pTouchVel) * (-24.0);


						if(prc->TestMode)
							TimerStart(&Tmr[i], 0.5);
							
						else					
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pTouchTm));

						prc->Status = prcsSTATUS_ForceON;
						prc->_step++;

					}
					
					else {
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_VOLT_ERR,0);

						prc->_step = 0;

					}
						
						
				}
														
			break;


			case 2: // Wait for below Touch Voltage or timeout

				if((vect->FbkVect.Mode != vectSt_Mode_Volt))
					vect->Mode = vectSt_Mode_Volt;

				if(TimerIsTimeOut(&Tmr[i])){

					if(prc->TestMode){

						vect->InVal = 0; 
						prc->Status = prcsSTATUS_Run;

						prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP); // Arc Volt Cmd		
						
						//prc->Fbk[0] = prc->Fbk[1]; // cur ARC Volt equal commanded
						prc->Fbk[0] = 0.0;
						break;

					}	

					repeatCnt[i]++;

					if(repeatCnt[i] >= (*AVC_Param_Set[i].pTouchRepeat)){

												
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_TOUCH_TMO,1);

						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * (24.0);
						vect->Mode = vectSt_Mode_Volt;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
										
						//prc->_step++;
						prc->_step = 23;

										
					}
					else { 

						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRetractTm));						
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * (24.0);
						
						prc->_step = 1;

					}						
					
							
				}

				
				else if(!prc->TestMode){
					
						if((prc->Fbk[0] - (*AVC_Param_Set[i].pTouchVolt)) < 0 ){  //  Success Voltage below treshold  Arc Start

							vect->InVal = (*AVC_Param_Set[i].pToSPVel) * (24.0);
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pVoltSPTimeOut));						
												
						
							prc->_step++;


						}
				}
				
				
														
			break;

			case 3: // Wait for  above VoltageInit or timeout

				
				if(TimerIsTimeOut(&Tmr[i])){

					Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_SP_TMO,0);
					prc->_step = 0;
					
							
				}

				else if((prc->Fbk[0] - (*AVC_Param_Set[i].pVoltInit)) > (-0.5)){ 

					prc->Status = prcsSTATUS_StartDly;		

					prc->Status = prcsSTATUS_Run;
					vect->Mode = vectSt_Mode_Volt;					

					vect->InVal = 0;

					vect->xCMD = VECT_UpdateSignFilter;
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pStartDly));
					KK_StartDly = (*AVC_Param_Set[i].pStartDly);
					
					prc->_step++;
					
				}
			
				else{
			
					vect->InVal = (*AVC_Param_Set[i].pToSPVel) * (24.0);


				}
					
				
														
			break;


			case 4:

					if((vect->FbkVect.Mode != vectSt_Mode_Volt)){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Volt;

					}	
					

					if(TimerIsTimeOut(&Tmr[i])){

						
						if((AVC_Mode) != 3){ // Not BB regulation
							vect->Mode = vectSt_Mode_Pos;	
						}

						/*
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));
							//Ramp[i].dy_up = fabsf((*AVC_Param_Set[i].pSpeedSp - Ramp[i].y)/(*AVC_Param_Set[i].pRampUp));
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = (*AVC_Param_Set[i].pVoltSP);
						AzzRamp(&Ramp[i]);

						*/

						
						if(*AVC_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf((VoltSP)/(*AVC_Param_Set[i].pRampUp));	
							//Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));						
						else 
							Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						//Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].y_set = VoltSP;
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);						
						vect->InVal =  CMD_LinSpline[i].y;

						if((*AVC_Param_Set[i].pRampUp) <= 0.0)
							TimerStart(&Tmr[i], 0.1);
						else							
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampUp));

						prc->_step ++;


					}


			break;				

			case 5:
					prc->Param[0] = 0;

					// KK-dupa
					//if((vect->FbkVect.Mode == vectSt_Mode_Pos)){	

					if(1){

						AzzRamp(&Ramp[i]);

						CMD_LinSpline[i].x = GetArcValue(i);						
						//CMD_LinSpline[i].x = Ramp[i].y;
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y ;

						prc->Fbk[1] = Ramp[i].y; //ARC Cmd

					
						if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

							Ramp[i].dy_up = 50.0; // 3 volt per second
							Ramp[i].dy_down = Ramp[i].dy_up;
							
							if((AVC_Mode) != 3) // Not BB regulation					
								vect->fParam[0] = (*AVC_Param_Set[i].pBB_Window);							
							else
								vect->fParam[0] = 0.0;

							vect->xCMD = VECT_Set_MaxError;


							prc->Status = prcsSTATUS_AVC_OK;								

							prc->_step++;
						}


					}
			
			break;	


			case 6:		
		

				if(prc->Param[0]){
					
					vect->Mode = vectSt_Mode_Volt;					

					vect->InVal = 0;
					prc->_step++;

				}	

				else {	

					Ramp[i].x = VoltSP;
					AzzRamp(&Ramp[i]);										

					CMD_LinSpline[i].x = GetArcValue(i);
					prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y ;	


				}
				


			break;	


			case 7:

				
				if(vect->FbkVect.Mode == vectSt_Mode_Volt){

					//vect->InVal = 0;

					vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;

					prc->Status = prcsSTATUS_AVC_Hold;

					if(!prc->Param[0]){

						if((AVC_Mode) != 3) // Not BB Regulation
							vect->Mode = vectSt_Mode_Pos;
						
						
						Ramp[i].dy_up = 50.0; // 50 volt per second
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						Ramp[i].set = 0;
						
						//CMD_LinSpline[i].x = Ramp[i].y;
						CMD_LinSpline[i].x = GetArcValue(i);
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y;

						prc->_step = 5;

					}
				}
				else{

					vect->Mode = vectSt_Mode_Volt;
					vect->InVal = 0;

				}
					
				


			break;	





			case 20:

				
				prc->Status = prcsSTATUS_StopDly;

				if(!TimerIsTimeOut(&Tmr[i]))
				 	break;

				
				prc->Status = prcsSTATUS_RampDwn;
				
				if((*AVC_Param_Set[i].pRampDwn) >0){

					
					//Ramp[i].dy_up = fabsf((prc->Fbk[0]- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
					Ramp[i].dy_up = fabsf((VoltSP- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
									
					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].set = 1;
					Ramp[i].x = (*AVC_Param_Set[i].pFinalVolt);
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;
					
					//CMD_LinSpline[i].x = Ramp[i].y;
					CMD_LinSpline[i].x = GetArcValue(i);
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y;	
					
				}
				
				TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn));
				vect->xCMD = VECT_ClearSignFilter;					
				prc->_step++;
				

			break;	


			case 21:

				AzzRamp(&Ramp[i]);
			
				//CMD_LinSpline[i].x = Ramp[i].y;
				CMD_LinSpline[i].x = GetArcValue(i);
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if((vect->FbkWarning!= 0))
					prc->_step = 0;

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;

					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pHoldTm));

					prc->Status = prcsSTATUS_RampDwn;

					prc->_step++;
				}


			break;	


			


			case 22:

				if((vect->FbkWarning != 0))
					prc->_step = 0;

				else if(TimerIsTimeOut(&Tmr[i])){

					vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * 24.0;
				
					vect->Mode = vectSt_Mode_Volt;					
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
									
					prc->_step++;

				}


			break;	

			case 23:				

				if(TimerIsTimeOut(&Tmr[i]) ||(vect->FbkWarning!= 0))
					prc->_step = 0;
				
				

			break;	

			
			case 60: ///  DirectCntrl

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

											
						if(prc->fParam[0] != 0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
						else 									
							prc->_step ++;									
					
						
					}
												
				}
			

			break;

			case 61: /// Direct Cntrl - Ramp Down

					if(vect->InVal > VoltRampDown)
						vect->InVal -= VoltRampDown;
					else if(vect->InVal < (VoltRampDown * (-1)))
						vect->InVal += VoltRampDown;
					else 
						prc->_step = prcsIDLE;
			
			break;


			case 0xFF:  // Disable

				if(vect->FbkVect.Mode != vectSt_Mode_Idle)
					vect->Mode = vectSt_Mode_Idle;

				if(!prc->Disable)
					prc->_step = 0;
			

			break;	

			
		}

		// Throw error when AVC is running and Limits are triggered 
		if((vect->FbkWarning!= 0) ){

			if(!LimitsFilterTmr[i].IN){
				LimitsFilterTmr[i].IN = 1;
				LimitsFilterTmr[i].PT = (int) (((*AVC_Param_Set[i].pLimitsFilterTm) * 1000.0)+0.5);
			}
				
			
		}
		else 
			LimitsFilterTmr[i].IN = 0;
		

		if((prc->_step > prcsIDLE) && (prc->_step < 50) ) {

			if(LimitsFilterTmr[i].Q && ((*AVC_Param_Set[i].pLimitsFilterTm)!= 0.0 ))			
				Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_AVC_LIMIT_ERR,i);

		}	

		// Na wyrazne rzadanie zmienilem
		/*
		if(prc->Status == prcsSTATUS_Stopped){
				//prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		}	
		else
			prc->Fbk[1] = prc->Fbk[0];
		*/	

		if(TigSynchMode == 3 && (AVCSynchMode == 1)) // TIG Sync IN Out and AVC Back Locked
			prc->Fbk[1] = ((*AVC_Param_Set[i].pVoltInSP) +(*AVC_Param_Set[i].pVoltOutSP))/2.0; 
		else
			//prc->Fbk[1] = VoltSP;
			// 12 July2024
			prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);


		//////////////////////////////////	
		// Pseudo Servo
		HMI_Mode = (AVC_Mode);
		if((AVC_Mode)== 3){ // BB regulation

			HMI_Window = (*AVC_Param_Set[i].pBB_Window);
			HMI_RegGain = (*AVC_Param_Set[i].pBB_Gain);
			HMI_RegVolt = (*AVC_Param_Set[i].pBB_Vel) * 24.0;
			IntegrGain = (*AVC_Param_Set[i].pBB_Integr) * 2.0;

			HMI_Error = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;
			
		
			if((prc->_step == 5) || (prc->_step == 6) || (prc->_step == 20)|| (prc->_step == 21)){

				//y = filterAzzMovAvgFlt[i].y - Ramp[i].y; 

				y = filterAzzMovAvgFlt[i].x - CMD_LinSpline[i].x;

				if(fabsf(y) > HMI_Window )
					_IntegrError[i] += IntegrGain * y;
				else
					_IntegrError[i] = 0.0;

				

				if(y > HMI_Window)
					vect->InVal = ((y - HMI_Window) * HMI_RegGain + HMI_RegVolt) * (-1.0);

				else if(y  <  (HMI_Window *(-1.0)))
					vect->InVal = (-y - HMI_Window)* HMI_RegGain  + HMI_RegVolt;
										
				else
					vect->InVal = 0;

				

						
				if(Set_AVC_BB_Gain == 0)
					vect->InVal = 0;

			
			}
			else // This was changed in April 23 , 2019
				_IntegrError[i] = 0.0;
		}
		else // This was changed in April 23 , 2019
			_IntegrError[i] = 0.0;
			


		////////////////

		//Jump start
		if((_InVal[i] == 0.0) &&  (vect->InVal != 0.0)
			||((_InVal[i] * vect->InVal) < 0.0))
			TimerStart(&JumpTmr[i], BoostTime);
		else
			JumpTmr[i].IN = 0;

		_InVal[i] = vect->InVal;
		
		TimerExecute(&JumpTmr[i]);
		

		if(!JumpTmr[i].Q && JumpTmr[i].IN){ // Boost


			if(vect->InVal > 0.0)

				_BoostVoltage[i] = BoostVoltage;

			else if(vect->InVal < 0.0)

				_BoostVoltage[i] = BoostVoltage * (-1.0);

			else

				_BoostVoltage[i] = 0;
				
		}
		else
			_BoostVoltage[i] = 0;
		

		if(BoostApply)
			vect->InVal += _BoostVoltage[i];	

		// Integrator
		if(IntegrApply)
			vect->InVal -= _IntegrError[i];
		
		TimerExecute(&LimitsFilterTmr[i]);
		TimerExecute(&Tmr[i]);


		
		
	}



}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void  AVC_ENC_Ctrl_2(void)
{
int i , k;
float y;

	/*
	if(KK_StartArc || KK_HomeBtn){
		KK_StartArc = 0;
		Start_Arc_Cmd(0);
	}

	if(KK_StopBtn)
		Stop_Arc_Cmd(0);

	*/	
		
	for(i=0 ; i < AVC_NUM ; i++){

		//segm = Master.Segm[0];
		
		if(segm >= SEGM_NUM)			
			segm = 0;

		prc = AVC_Param_Set[i].pPrc;

		if(!prc)
			break;

		
		
		vect = AVC_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
				break;

		
		///////////////////////////////////////		
		//FBK_LinSpline[i].x = ((float)TML_MTR_INST[2].AnInp[1])/4096.0 * 5.0;
		FBK_LinSpline[i].x = vect->FbkVect.x;
		AzzLinSpline(&FBK_LinSpline[i]);
		prc->Fbk[0] = FBK_LinSpline[i].y; // cur Arc Volt  

		filterAzzMovAvgFlt[i].x = FBK_LinSpline[i].y;
		AzzMovAvgFlt(&filterAzzMovAvgFlt[i]);		
		//prc->Fbk[0] =  filterAzzMovAvgFlt[i].y;
		
 				
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsRESTART:

					prc->xCMD = prcsRefrCalib;
						
				break;	


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

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


				case prcsIDLE:


				break;	

				case prcsRESTART:

					
					prc->_step = prcsIDLE;
					prc->xCMD = prcsRefrCalib;
						
				break;	

				
				case prcsSTART:

					prc->xCMD = prcsRefrCalib;
							
					if((prc->_step == 0) || (prc->_step > 4)){

						if(!ErrorMgr.Error){
							TimerStart(&Tmr[i], 5.0);
							repeatCnt[i] = 0;
							prc->_step = 1;
						}	

					}	
				

				break;

				case prcsHalt:


					prc->_step = prcsIDLE;


				break;

				case prcsQuickSTOP:
				case prcsSTOP:


					Stop_Arc_Cmd(i);

					if(prc->TestMode){

						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Vel;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pStopDly));
				
						prc->_step = 22;


					}	

					else if(prc->_step != 0) {

						switch(prc->xCMD){

							case prcsQuickSTOP:

								TimerStart(&Tmr[i], 0.01);

							break;
							
							case prcsSTOP:

								TimerStart(&Tmr[i],(*AVC_Param_Set[i].pStopDly));


							break;	


						}

					
						prc->_step = 20;

					}	

									
				break;

				case prcsDirectCntrl:
				//case prcsJOGpos:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;


				default:

					prc->_step = prcsIDLE;
					

				break;	
				
			}
		}
		
		

		if(prc->xCMD == prcsRefrCalib){

			if(AVC_Param_Set[i].pCalibPoints){

				AVC_Param_Set[i].pCalibPoints->num = 4;

				for(k = 0 ; k < AVC_Param_Set[i].pCalibPoints->num;  k++){

					FBK_CalibrPoints[i][k].Y = 6.0 * k;
					//FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalib->Points[CALIB_TIG_VOLT_FBK+i].val[k];
					FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalibPoints->val[k];

					CMD_CalibrPoints[i][k].X = FBK_CalibrPoints[i][k].X;
					CMD_CalibrPoints[i][k].Y = FBK_CalibrPoints[i][k].Y;

			    }	

				FBK_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */				
				FBK_LinSpline[i].Update = 1;
				AzzLinSpline(&FBK_LinSpline[i]);
				FBK_LinSpline[i].enable = 1;

				CMD_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */			
				CMD_LinSpline[i].Update = 1;
				AzzLinSpline(&CMD_LinSpline[i]);
				CMD_LinSpline[i].enable = 1;	
			}
	

		}		
		
		prc->xCMD = prcsIDLE;

		
		///////////////////////////////////

		if(TigSynchMode == 0) // Straight
			VoltSP = (*AVC_Param_Set[i].pVoltSP);
		else
			VoltSP = (*AVC_Param_Set[i].pBkgVoltSP);

		if(VoltSP <= 0.0)
			VoltSP = 10.0;

		VoltBoost1 = (*AVC_Param_Set[i].pBoost1);
		VoltBoost2 = (*AVC_Param_Set[i].pBoost2);

		/*
		switch(AVC_Param_Set[i].BoostCmd){

			case 1:
				
				VoltSP += VoltBoost1;

			break;

			case 2:
				
				VoltSP += VoltBoost2;

			break;

			default: 
			break;

		}

		if(VoltSP < 0.0)
			VoltSP = 0.0;
		*/	

		//////////////////////////
		switch(prc->_step){


			case prcsIDLE:  // IDLE - stopped

				
				prc->Status = prcsSTATUS_Stopped;				

				vect->InVal = 0.0;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)					
					vect->Mode = vectSt_Mode_Vel;
				
				KK_StartDly = (*AVC_Param_Set[i].pStartDly);
				
												
			break;



			case 1: // Start

				vect->InVal = 0.0;

				if(vect->FbkVect.Mode != vectSt_Mode_Vel){	
					vect->Mode = vectSt_Mode_Vel;	
					break;
				}
				
				 if(!TimerIsTimeOut(&Tmr[i])) {

					//vect->xCMD = VECT_ClearSignFilter;

					// (*AVC_Param_Set[i].pTouchVolt)
					if((prc->Fbk[0] > 2.0 )|| prc->TestMode ) { 		
						
						

						if(prc->TestMode)
							TimerStart(&Tmr[i], 0.5);
							
						else{	
							Start_Arc_Cmd(i);
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pTouchTm));
						}

						prc->Status = prcsSTATUS_ForceON;
						prc->_step++;

					}				
						
						
				}else{						

					Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_VOLT_ERR,0);
					prc->_step = 0;			
				

				}
				 
														
			break;


			case 2: // Wait for below Touch Voltage or timeout

				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(TimerIsTimeOut(&Tmr[i])){

					if(prc->TestMode){

						vect->InVal = AVC_Param_Set[0].extVoltSP;
						//vect->InVal = 0.0;
					
						prc->Status = prcsSTATUS_Run;

						prc->Fbk[1] = VoltSP; // Arc Volt Cmd		
						
						//prc->Fbk[0] = prc->Fbk[1]; // cur ARC Volt equal commanded
						prc->Fbk[0] = 0.0;
						break;

					}	

					repeatCnt[i]++;

					if(repeatCnt[i] >= (*AVC_Param_Set[i].pTouchRepeat)){

												
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_TOUCH_TMO,1);

						Stop_Arc_Cmd(i);
					
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].MaxSpeedJog;
						vect->Mode = vectSt_Mode_Vel;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
										
						//prc->_step++;
						prc->_step = 23;

										
					}
					else { 

						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRetractTm));						
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)  * AVC_Param_Set[i].MaxSpeedJog;
						
						prc->_step = 1;

					}						
					
							
				}

				
				else if(!prc->TestMode){

						if(vect->curStep > 0)						
							prc->_step ++;
						
				}
				
				
														
			break;

			case 3:				

					vect->InVal = 0;

					if(vect->curStep == 0){						
						
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pVoltSPTimeOut));												

						prc->_step++;

					}				

			break;

			case 4: 			
				
				
					prc->Status = prcsSTATUS_Run;
					
					//vect->xCMD = VECT_UpdateSignFilter;
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pStartDly));
					KK_StartDly = (*AVC_Param_Set[i].pStartDly);
					
					prc->_step++;

																		
			break;


			case 5:	

				
				if(TimerIsTimeOut(&Tmr[i])){						
				

					vect->Mode = vectSt_Mode_Pos;
					vect->maxVel = 0; // Max speed  
					//vect->InVal = prc->Fbk[0]; // InVal like voltage Init

					if(vect->FbkVect.Mode != vectSt_Mode_Pos)
						break;										

					
					if(*AVC_Param_Set[i].pRampUp > 0)
						Ramp[i].dy_up = fabsf((VoltSP)/(*AVC_Param_Set[i].pRampUp));	
						//Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));						
					else 
						Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
					
					Ramp[i].dy_down = Ramp[i].dy_up;

					//Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].y_set = VoltSP;
					Ramp[i].set = 1;
					Ramp[i].x = VoltSP;
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;
					
					//CMD_LinSpline[i].x = Ramp[i].y;
					CMD_LinSpline[i].x = GetArcValue(i);
					AzzLinSpline(&CMD_LinSpline[i]);						
					vect->InVal =  CMD_LinSpline[i].y;

					if((*AVC_Param_Set[i].pRampUp) <= 0.0)
						TimerStart(&Tmr[i], 0.1);
					else							
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampUp));

					prc->_step ++;						

				}


			break;				

			case 6:

					prc->Param[0] = 0;
					
					if(1){

						
						AzzRamp(&Ramp[i]);

						CMD_LinSpline[i].x = GetArcValue(i);					
						
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y ;

						prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	

					
						if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

							Ramp[i].dy_up = 50.0; // 3 volt per second
							Ramp[i].dy_down = Ramp[i].dy_up;
							
							prc->Status = prcsSTATUS_AVC_OK;						

							prc->_step++;
						}


					}
			
			break;	


			case 7:				
				

					if(prc->Param[0]){
						
						vect->Mode = vectSt_Mode_Vel;					

						vect->InVal = 0;
						prc->_step++;

					}	

					else {	

						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);										

						CMD_LinSpline[i].x = GetArcValue(i);
						prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y;	

					}

			break;	


			case 8:

				
				if(vect->FbkVect.Mode == vectSt_Mode_Vel){

					//vect->InVal = 0;
					vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]) * AVC_Param_Set[i].MaxSpeedJog;
				

					prc->Status = prcsSTATUS_AVC_Hold;

					if(!prc->Param[0]){
						
						vect->Mode = vectSt_Mode_Pos;						
						
						Ramp[i].dy_up = 50.0; // 10 volt per second
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						Ramp[i].set = 0;						

						prc->_step = 6;

					}
				}
				else{

					vect->Mode = vectSt_Mode_Vel;
					vect->InVal = 0;

				}
					
				


			break;

			case 20:

				
				prc->Status = prcsSTATUS_StopDly;

				CMD_LinSpline[i].x = GetArcValue(i);
				prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
	
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if(!TimerIsTimeOut(&Tmr[i]))
				 	break;

				
				prc->Status = prcsSTATUS_RampDwn;
				
				if((*AVC_Param_Set[i].pRampDwn) >0){

					
					Ramp[i].dy_up = fabsf((VoltSP- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
									
					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].set = 1;
					Ramp[i].x = (*AVC_Param_Set[i].pFinalVolt);
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;

					CMD_LinSpline[i].x = GetArcValue(i);
					prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd				
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y;	
					
				}
				
				TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn));
				//vect->xCMD = VECT_ClearSignFilter;					
				prc->_step++;
				

			break;	


			case 21:

				AzzRamp(&Ramp[i]);

				CMD_LinSpline[i].x = GetArcValue(i);
				prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if((vect->FbkWarning!= 0))
					prc->_step = 0;

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;

					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pHoldTm));

					prc->Status = prcsSTATUS_RampDwn;

					prc->_step++;
				}


			break;	


			


			case 22:

				if((vect->FbkWarning != 0))
					prc->_step = 0;

				else if(TimerIsTimeOut(&Tmr[i])){

					vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].MaxSpeedJog;
				
					vect->Mode = vectSt_Mode_Vel;					
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
									
					prc->_step++;

				}


			break;	

			case 23:				

				if(TimerIsTimeOut(&Tmr[i]) ||(vect->FbkWarning!= 0))
					prc->_step = 0;
				
				

			break;	

			
			case 60: ///  DirectCntrl

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

						if(prc->fParam[0] != 0.0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]) * AVC_Param_Set[i].MaxSpeedJog;
							
						else 
							prc->_step = 0;											
						
					}
												
				}	
			

			break;		


			case 0xFF:  // Disable

				if(vect->FbkVect.Mode != vectSt_Mode_Idle)
					vect->Mode = vectSt_Mode_Idle;

				if(!prc->Disable)
					prc->_step = 0;
			

			break;	

			
		}

		// Throw error when AVC is running and Limits are triggered 
		if((vect->FbkWarning!= 0) ){

			if(!LimitsFilterTmr[i].IN){
				LimitsFilterTmr[i].IN = 1;
				LimitsFilterTmr[i].PT = (int) (((*AVC_Param_Set[i].pLimitsFilterTm) * 1000.0)+0.5);
			}
				
			
		}
		else 
			LimitsFilterTmr[i].IN = 0;
		

		if((prc->_step > prcsIDLE) && (prc->_step < 50) ) {

			//if(LimitsFilterTmr[i].Q && ((*AVC_Param_Set[i].pLimitsFilterTm)!= 0.0 ))			
				//Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_AVC_LIMIT_ERR,i);

		}	

		// Na wyrazne rzadanie zmienilem
		/*
		//if(prc->Status == prcsSTATUS_Stopped){
		if(prc->_step == 0){
				//prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		}	
		else
			prc->Fbk[1] = prc->Fbk[0];
		*/

		//prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		if(TigSynchMode == 3 && (AVCSynchMode == 1)) // TIG Sync IN Out and AVC Back Locked
			prc->Fbk[1] = ((*AVC_Param_Set[i].pVoltInSP) +(*AVC_Param_Set[i].pVoltOutSP))/2.0;
		else
			//prc->Fbk[1] = VoltSP;
			// 12 July2024
			prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);
		
		//////////////////////////////////	
		// Pseudo Servo
		HMI_Mode = (*AVC_Param_Set[i].pMode);		
		
		TimerExecute(&Tmr[i]);


		///////////////////////////////////////////////////
		TimerExecute(&LimitsFilterTmr[i]);
		

		
		
	}



}

void  AVC_ENC_Ctrl_Sel(void)
{
int i , k;
float y;


	

	
		
	for(i=0 ; i < AVC_NUM ; i++){

		//segm = Master.Segm[0];
		
		if(segm >= SEGM_NUM)			
			segm = 0;

		prc = AVC_Param_Set[i].pPrc;

		if(!prc)
			break;

		
		
		vect = AVC_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0){

			_pVect[i] = 0;
			break;

		}


		if(_pVect[i] != AVC_Param_Set[i].pVect){

			prc->_step = prcsIDLE;
			vect->Mode = vectSt_Mode_Idle;
			_pVect[i] = AVC_Param_Set[i].pVect;
		}
		
		///////////////////////////////////////		
		//FBK_LinSpline[i].x = ((float)TML_MTR_INST[2].AnInp[1])/4096.0 * 5.0;
		FBK_LinSpline[i].x = vect->FbkVect.x;
		AzzLinSpline(&FBK_LinSpline[i]);
		prc->Fbk[0] = FBK_LinSpline[i].y; // cur Arc Volt  

		filterAzzMovAvgFlt[i].x = FBK_LinSpline[i].y;
		AzzMovAvgFlt(&filterAzzMovAvgFlt[i]);		
		//prc->Fbk[0] =  filterAzzMovAvgFlt[i].y;
		
 				
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsRESTART:

					prc->xCMD = prcsRefrCalib;
						
				break;	


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

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


				case prcsIDLE:


				break;	

				case prcsRESTART:

					
					prc->_step = prcsIDLE;
					prc->xCMD = prcsRefrCalib;
						
				break;	

				
				case prcsSTART:

					prc->xCMD = prcsRefrCalib;
							
					if((prc->_step == 0) || (prc->_step > 4)){

						if(!ErrorMgr.Error){
							TimerStart(&Tmr[i], 5.0);
							repeatCnt[i] = 0;
							prc->_step = 1;
						}	

					}	
				

				break;

				case prcsHalt:


					prc->_step = prcsIDLE;


				break;

				case prcsQuickSTOP:
				case prcsSTOP:


					Stop_Arc_Cmd(i);

					if(prc->TestMode){

						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Vel;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pRampDwn) + (*AVC_Param_Set[i].pStopDly));
				
						prc->_step = 22;


					}	

					else if(prc->_step != 0) {

						switch(prc->xCMD){

							case prcsQuickSTOP:

								TimerStart(&Tmr[i], 0.01);

							break;
							
							case prcsSTOP:

								TimerStart(&Tmr[i],(*AVC_Param_Set[i].pStopDly));


							break;	


						}

					
						prc->_step = 20;

					}	

									
				break;

				case prcsDirectCntrl:
				//case prcsJOGpos:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;


				default:

					prc->_step = prcsIDLE;
										

				break;	
				
			}
		}
		
		

		if(prc->xCMD == prcsRefrCalib){

			if(AVC_Param_Set[i].pCalibPoints){

				AVC_Param_Set[i].pCalibPoints->num = 4;

				for(k = 0 ; k < AVC_Param_Set[i].pCalibPoints->num;  k++){

					FBK_CalibrPoints[i][k].Y = 6.0 * k;
					//FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalib->Points[CALIB_TIG_VOLT_FBK+i].val[k];
					FBK_CalibrPoints[i][k].X = AVC_Param_Set[i].pCalibPoints->val[k];

					CMD_CalibrPoints[i][k].X = FBK_CalibrPoints[i][k].X;
					CMD_CalibrPoints[i][k].Y = FBK_CalibrPoints[i][k].Y;

			    }	

				FBK_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */				
				FBK_LinSpline[i].Update = 1;
				AzzLinSpline(&FBK_LinSpline[i]);
				FBK_LinSpline[i].enable = 1;

				CMD_LinSpline[i].NbNodes = AVC_Param_Set[i].pCalibPoints->num; /* number of nodes */			
				CMD_LinSpline[i].Update = 1;
				AzzLinSpline(&CMD_LinSpline[i]);
				CMD_LinSpline[i].enable = 1;	
			}
	

		}		
		
		prc->xCMD = prcsIDLE;

		
		///////////////////////////////////

		if(TigSynchMode == 0) // Straight
			VoltSP = (*AVC_Param_Set[i].pVoltSP);
		else
			VoltSP = (*AVC_Param_Set[i].pBkgVoltSP);

		if(VoltSP <= 0.0)
			VoltSP = 10.0;

		VoltBoost1 = (*AVC_Param_Set[i].pBoost1);
		VoltBoost2 = (*AVC_Param_Set[i].pBoost2);

		/*
		switch(AVC_Param_Set[i].BoostCmd){

			case 1:
				
				VoltSP += VoltBoost1;

			break;

			case 2:
				
				VoltSP += VoltBoost2;

			break;

			default: 
			break;

		}

		if(VoltSP < 0.0)
			VoltSP = 0.0;
		*/	

		//////////////////////////
		switch(prc->_step){


			case prcsIDLE:  // IDLE - stopped

				
				prc->Status = prcsSTATUS_Stopped;				

				vect->InVal = 0.0;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)					
					vect->Mode = vectSt_Mode_Vel;
				
				KK_StartDly = (*AVC_Param_Set[i].pStartDly);
				
												
			break;



			case 1: // Start

				vect->InVal = 0.0;

				if(vect->FbkVect.Mode != vectSt_Mode_Vel){	
					vect->Mode = vectSt_Mode_Vel;	
					break;
				}
				
				 if(!TimerIsTimeOut(&Tmr[i])) {

					//vect->xCMD = VECT_ClearSignFilter;

					// (*AVC_Param_Set[i].pTouchVolt)
					if((prc->Fbk[0] > 2.0 )|| prc->TestMode ) { 		
						
						

						if(prc->TestMode)
							TimerStart(&Tmr[i], 0.5);
							
						else{	
							Start_Arc_Cmd(i);
							TimerStart(&Tmr[i], (*AVC_Param_Set[i].pTouchTm));
						}

						prc->Status = prcsSTATUS_ForceON;
						prc->_step++;

					}				
						
						
				}else{						

					Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_VOLT_ERR,0);
					prc->_step = 0;			
				

				}
				 
														
			break;


			case 2: // Wait for below Touch Voltage or timeout

				if((vect->FbkVect.Mode != vectSt_Mode_Vel))
					vect->Mode = vectSt_Mode_Vel;

				if(TimerIsTimeOut(&Tmr[i])){

					if(prc->TestMode){

						vect->InVal = AVC_Param_Set[0].extVoltSP;
						//vect->InVal = 0.0;
					
						prc->Status = prcsSTATUS_Run;

						prc->Fbk[1] = VoltSP; // Arc Volt Cmd		
						
						//prc->Fbk[0] = prc->Fbk[1]; // cur ARC Volt equal commanded
						prc->Fbk[0] = 0.0;
						break;

					}	

					repeatCnt[i]++;

					if(repeatCnt[i] >= (*AVC_Param_Set[i].pTouchRepeat)){

												
						Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_TOUCH_TMO,1);

						Stop_Arc_Cmd(i);
					
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].MaxSpeedJog;
						vect->Mode = vectSt_Mode_Vel;				
					
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
										
						//prc->_step++;
						prc->_step = 23;

										
					}
					else { 

						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRetractTm));						
						vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl)  * AVC_Param_Set[i].MaxSpeedJog;
						
						prc->_step = 1;

					}						
					
							
				}

				
				else if(!prc->TestMode){

						if(vect->curStep > 0)						
							prc->_step ++;
						
				}
				
				
														
			break;

			case 3:				

					vect->InVal = 0;

					if(vect->curStep == 0){						
						
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pVoltSPTimeOut));												

						prc->_step++;

					}				

			break;

			case 4: 			
				
				
					prc->Status = prcsSTATUS_Run;
					
					//vect->xCMD = VECT_UpdateSignFilter;
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pStartDly));
					KK_StartDly = (*AVC_Param_Set[i].pStartDly);
					
					prc->_step++;

																		
			break;


			case 5:	

				
				if(TimerIsTimeOut(&Tmr[i])){						
				

					vect->Mode = vectSt_Mode_Pos;
					vect->maxVel = 0; // Max speed  
					//vect->InVal = prc->Fbk[0]; // InVal like voltage Init

					if(vect->FbkVect.Mode != vectSt_Mode_Pos)
						break;										

					
					if(*AVC_Param_Set[i].pRampUp > 0)
						Ramp[i].dy_up = fabsf((VoltSP)/(*AVC_Param_Set[i].pRampUp));	
						//Ramp[i].dy_up = fabsf(((*AVC_Param_Set[i].pVoltSP)- prc->Fbk[0])/(*AVC_Param_Set[i].pRampUp));						
					else 
						Ramp[i].dy_up = 5.0; // minimum ramp 5 V/sec
					
					Ramp[i].dy_down = Ramp[i].dy_up;

					//Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].y_set = VoltSP;
					Ramp[i].set = 1;
					Ramp[i].x = VoltSP;
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;
					
					//CMD_LinSpline[i].x = Ramp[i].y;
					CMD_LinSpline[i].x = GetArcValue(i);
					AzzLinSpline(&CMD_LinSpline[i]);						
					vect->InVal =  CMD_LinSpline[i].y;

					if((*AVC_Param_Set[i].pRampUp) <= 0.0)
						TimerStart(&Tmr[i], 0.1);
					else							
						TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampUp));

					prc->_step ++;						

				}


			break;				

			case 6:

					prc->Param[0] = 0;
					
					if(1){

						
						AzzRamp(&Ramp[i]);

						CMD_LinSpline[i].x = GetArcValue(i);					
						
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y ;

						prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	

					
						if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

							Ramp[i].dy_up = 50.0; // 3 volt per second
							Ramp[i].dy_down = Ramp[i].dy_up;
							
							prc->Status = prcsSTATUS_AVC_OK;						

							prc->_step++;
						}


					}
			
			break;	


			case 7:				
				

					if(prc->Param[0]){
						
						vect->Mode = vectSt_Mode_Vel;					

						vect->InVal = 0;
						prc->_step++;

					}	

					else {	

						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);										

						CMD_LinSpline[i].x = GetArcValue(i);
						prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
						AzzLinSpline(&CMD_LinSpline[i]);
						vect->InVal =  CMD_LinSpline[i].y;	

					}

			break;	


			case 8:

				
				if(vect->FbkVect.Mode == vectSt_Mode_Vel){

					//vect->InVal = 0;
					vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]) * AVC_Param_Set[i].MaxSpeedJog;
				

					prc->Status = prcsSTATUS_AVC_Hold;

					if(!prc->Param[0]){
						
						vect->Mode = vectSt_Mode_Pos;						
						
						Ramp[i].dy_up = 50.0; // 10 volt per second
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].y_set = prc->Fbk[0];
						Ramp[i].set = 1;
						Ramp[i].x = VoltSP;
						AzzRamp(&Ramp[i]);
						
						Ramp[i].set = 0;						

						prc->_step = 6;

					}
				}
				else{

					vect->Mode = vectSt_Mode_Vel;
					vect->InVal = 0;

				}
					
				


			break;

			case 20:

				
				prc->Status = prcsSTATUS_StopDly;

				CMD_LinSpline[i].x = GetArcValue(i);
				prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
	
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if(!TimerIsTimeOut(&Tmr[i]))
				 	break;

				
				prc->Status = prcsSTATUS_RampDwn;
				
				if((*AVC_Param_Set[i].pRampDwn) >0){

					
					Ramp[i].dy_up = fabsf((VoltSP- (*AVC_Param_Set[i].pFinalVolt))/(*AVC_Param_Set[i].pRampDwn));
									
					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].y_set = prc->Fbk[0];
					Ramp[i].set = 1;
					Ramp[i].x = (*AVC_Param_Set[i].pFinalVolt);
					AzzRamp(&Ramp[i]);
					
					Ramp[i].set = 0;

					CMD_LinSpline[i].x = GetArcValue(i);
					prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd				
					AzzLinSpline(&CMD_LinSpline[i]);
					vect->InVal =  CMD_LinSpline[i].y;	
					
				}
				
				TimerStart(&Tmr[i], (*AVC_Param_Set[i].pRampDwn));
				//vect->xCMD = VECT_ClearSignFilter;					
				prc->_step++;
				

			break;	


			case 21:

				AzzRamp(&Ramp[i]);

				CMD_LinSpline[i].x = GetArcValue(i);
				prc->Fbk[1] = CMD_LinSpline[i].x; //ARC Cmd	
				AzzLinSpline(&CMD_LinSpline[i]);
				vect->InVal =  CMD_LinSpline[i].y;

				if((vect->FbkWarning!= 0))
					prc->_step = 0;

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;

					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pHoldTm));

					prc->Status = prcsSTATUS_RampDwn;

					prc->_step++;
				}


			break;	


			


			case 22:

				if((vect->FbkWarning != 0))
					prc->_step = 0;

				else if(TimerIsTimeOut(&Tmr[i])){

					vect->InVal = (*AVC_Param_Set[i].pLiftSpeedDirCntrl) * AVC_Param_Set[i].MaxSpeedJog;
				
					vect->Mode = vectSt_Mode_Vel;					
					
					TimerStart(&Tmr[i], (*AVC_Param_Set[i].pGoBackTm));
									
					prc->_step++;

				}


			break;	

			case 23:				

				if(TimerIsTimeOut(&Tmr[i]) ||(vect->FbkWarning!= 0))
					prc->_step = 0;
				
				

			break;	

			
			case 60: ///  DirectCntrl

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Vel;
				}	
				else{				

					if(AVC_Param_Set[i].pJogSpeedDirCntrl){		

						if(prc->fParam[0] != 0.0)
							vect->InVal = (*AVC_Param_Set[i].pJogSpeedDirCntrl) * (prc->fParam[0]) * AVC_Param_Set[i].MaxSpeedJog;
							
						else 
							prc->_step = 0;											
						
					}
												
				}	
			

			break;		


			case 0xFF:  // Disable

				if(vect->FbkVect.Mode != vectSt_Mode_Idle)
					vect->Mode = vectSt_Mode_Idle;

				if(!prc->Disable)
					prc->_step = 0;
			

			break;	

			
		}

		// Throw error when AVC is running and Limits are triggered 
		if((vect->FbkWarning!= 0) ){

			if(!LimitsFilterTmr[i].IN){
				LimitsFilterTmr[i].IN = 1;
				LimitsFilterTmr[i].PT = (int) (((*AVC_Param_Set[i].pLimitsFilterTm) * 1000.0)+0.5);
			}
				
			
		}
		else 
			LimitsFilterTmr[i].IN = 0;
		

		if((prc->_step > prcsIDLE) && (prc->_step < 50) ) {

			//if(LimitsFilterTmr[i].Q && ((*AVC_Param_Set[i].pLimitsFilterTm)!= 0.0 ))			
				//Errors(&ErrorMgr,ERROR_TIG_PROC,ERROR_TIG_AVC_LIMIT_ERR,i);

		}	

		// Na wyrazne rzadanie zmienilem
		/*
		//if(prc->Status == prcsSTATUS_Stopped){
		if(prc->_step == 0){
				//prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		}	
		else
			prc->Fbk[1] = prc->Fbk[0];
		*/

		//prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);

		if(TigSynchMode == 3 && (AVCSynchMode == 1)) // TIG Sync IN Out and AVC Back Locked
			prc->Fbk[1] = ((*AVC_Param_Set[i].pVoltInSP) +(*AVC_Param_Set[i].pVoltOutSP))/2.0;
		else
			//prc->Fbk[1] = VoltSP;
			// 12 July2024
			prc->Fbk[1] = (*AVC_Param_Set[i].pVoltSP);
		
		//////////////////////////////////	
		// Pseudo Servo
		HMI_Mode = (*AVC_Param_Set[i].pMode);		
		
		TimerExecute(&Tmr[i]);


		///////////////////////////////////////////////////
		TimerExecute(&LimitsFilterTmr[i]);
		

		
		
	}



}


void Start_Arc_Cmd(int i){


	AVC_Param_Set[i].pVect->fParam[TouchVel_par] 	= (*AVC_Param_Set[i].pTouchVel)  * AVC_Param_Set[i].MaxSpeedJog;
	AVC_Param_Set[i].pVect->fParam[RetVel_par] 		= (*AVC_Param_Set[i].pToSPVel)  * AVC_Param_Set[i].MaxSpeedJog;
	
	CMD_LinSpline[i].x = (*AVC_Param_Set[i].pTouchVolt) ;
	AzzLinSpline(&CMD_LinSpline[i]);
	AVC_Param_Set[i].pVect->fParam[ArcTrshld_par]  =  CMD_LinSpline[i].y ;	
	
	AVC_Param_Set[i].pVect->Param[TouchTM_par]		= (DINT)((*AVC_Param_Set[i].pKiss_Tm) * 1000.0);
	
	AVC_Param_Set[i].pVect->Param[RetTM_par]		= (DINT)((*AVC_Param_Set[i].pLU_Tm) * 1000.0);

	AVC_Param_Set[i].pVect->xCMD = VECT_ARC_Start_Cmd;

	return;



}
void Stop_Arc_Cmd(int i){

	AVC_Param_Set[i].pVect->xCMD = VECT_ARC_Stop_Cmd;


return;
}

float GetArcValue(int k ){
float ret;

	

	switch(SynchClckToAVC){


		default:

			ret = Ramp[k].y;
	
		break;


		case 1:

			ret = Ramp[k].y  * (*AVC_Param_Set[k].pVoltSP) /VoltSP ;
				
		break;

		case 2:

			ret = Ramp[k].y  * (*AVC_Param_Set[k].pVoltInSP) /VoltSP ;

		break;

		case 3:

			ret = Ramp[k].y  * (*AVC_Param_Set[k].pVoltOutSP) /VoltSP ;

		break;

		case 4:

			ret = Ramp[k].y  * (((*AVC_Param_Set[k].pVoltOutSP) +  (*AVC_Param_Set[k].pVoltInSP))/2.0) /VoltSP ;

		break;


		}


		switch(AVC_Param_Set[k].BoostCmd){

			case 1:
				
				ret += VoltBoost1;

			break;

			case 2:
				
				ret += VoltBoost2;

			break;

			default: 
			break;

		}

		if(ret < 0.0)
			ret = 0.0;
		
	
	return ret;
}


