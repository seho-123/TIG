/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: TIG_PS
 * File: TIG_PS.c
 * Author: ckaczkowski
 * Created: November 26, 2013
 * 02.02.2016 - Ramp delay first then ramp
 * prc->Param[1] - reduce Amp by Percentage and RampDwn then Ramp Up
 ********************************************************************
 * Implementation of program TIG_PS
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>
#include <..\..\Lib\LoggerLib.c>


_LOCAL Robot_typ *robot;

_LOCAL TON_typ Tmr[TIG_PS_NUM];
GenProcess_typ* prc;
_LOCAL AzzRamp_typ Ramp[TIG_PS_NUM];
_LOCAL  AzzLimScal_typ  LimScal_AmpCMD[TIG_PS_NUM];

float _AmpSP[TIG_PS_NUM];
_LOCAL float AmpSP , AmpBoost1 , AmpBoost2;



//_LOCAL float KK_float[TIG_PS_NUM];
_LOCAL ARCLINK_PS_CNTRL_typ *arcPS;

_LOCAL float KK_Perc, KK_RampDwn , KK_RampUp , KK_Amps , KK_Cont_Dwell;

void _INIT TIG_PSINIT(void)
{
int i;

	Process[0].Disable = Process[0].Disable;
	Hardware[0].Anal_Inp[0] = Hardware[0].Anal_Inp[0];

	

	arcPS = &ArcLink.robot.Cntrl;
		
	for(i=0 ; i < (TIG_PS_NUM); i++){
	
		Ramp[i].y_max = 1000.0;
		Ramp[i].y_min = 0;	
		Ramp[i].enable = 1;

		LimScal_AmpCMD[i].x1 = 0.0;
		LimScal_AmpCMD[i].y1 = 0.0;
						
	}


}

void _CYCLIC TIG_PSCYCLIC(void){


	


	if(!PS_Param_Set[0].pPrc)
			return;


	KK_Perc = *PS_Param_Set[0].pContin_Amp_Reduce;
	KK_RampDwn = *PS_Param_Set[0].pContin_RampDwn;
	KK_RampUp = *PS_Param_Set[0].pContin_RampUp;
	KK_Amps = *PS_Param_Set[0].pAmpSP;
	KK_Cont_Dwell = *PS_Param_Set[0].pContin_Dwell;
	
	
	if(PS_Param_Set[0].pPrc->ConfigReady <= 0)
			return;


	switch(PS_Param_Set[0].Mode){

		case 1:

			arcPS = &ArcLink.robot.Cntrl;
			TIG_PS_ARCLINK();

		break;


		default:
		
			TIG_PS_ANALOG();

		break;



	}



}




void TIG_PS_ARCLINK(void)
{	
int i;
float y;
Robot_typ *robot;		
	
		for(i=0 ; i < TIG_PS_NUM ; i++){

			
			prc = PS_Param_Set[i].pPrc;
			
			if(!prc)
				break;

			if(prc->ConfigReady <= 0)
				break;


			if(!PS_Param_Set[i].pArcLinkRobot)
				break;

			robot = (Robot_typ*) PS_Param_Set[i].pArcLinkRobot;

								
			//LimScal_AmpCMD[i].x2 = PS_Param_Set[i].AmpCMD_Max;
			//LimScal_AmpCMD[i].y2 = PS_Param_Set[i].AmpCMD_Max;

			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

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

			}
			else{
	
	
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsRESTART:
				
						prc->_step = 0;						
						robot->Cntrl.Cmd.RUN = 0;
						//robot->Cntrl.Cmd.RST = 1;
						
					break;	

					case prcsHalt:

						robot->Cntrl.Cmd.RUN = 0;
						prc->_step = 0;

					break;	


					case prcsForceON:
					
						if(prc->_step != 50)
							prc->_step = 50;


					break;	


					case prcsJump2Run:
					
						if(prc->_step != 60)
							prc->_step = 60;


					break;	

					case prcsCalib:
					
						if(prc->_step != 70)
							prc->_step = 70;


					break;	

	
					case prcsSTART:

					if((prc->_step == 0) || (prc->_step > 5))
						prc->_step = 1;

					break;


					case prcsQuickSTOP:
					case prcsSTOP:

						if((prc->_step != 0) && (prc->_step <= 5)){

							switch(prc->xCMD){

								case prcsQuickSTOP:

									prc->_step = 22;

								break;	

								case prcsSTOP:

									prc->_step = 20;

								break;	

							}


						}	
							

					break;
						
					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
			//if(*PS_Param_Set[i].pMode == 0 ) //Straight
			if(TigSynchMode == 0 ) //Straight			
				AmpSP = *PS_Param_Set[i].pAmpSP;	
			else
				AmpSP = *PS_Param_Set[i].pBackAmp;


			AmpBoost1 = (*PS_Param_Set[i].pBoost1);
			AmpBoost2 = (*PS_Param_Set[i].pBoost2);

			/*
			switch(PS_Param_Set[i].BoostCmd){

				case 1:

					AmpSP += AmpBoost1;

				break;


				case 2:

					AmpSP += AmpBoost2;

				break;


				default:

				break;

				
			}


			if(AmpSP < 0.0)
				AmpSP = 0.0;
			*/	

			//////////////////////////
			
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					prc->Status = prcsSTATUS_Stopped;
					Ramp[i].y_set = 0;
					Ramp[i].set = 1;
					
								
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;

					Ramp[i].x = 0.0;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;

									
					if(*PS_Param_Set[i].pStartDly > 0){
						TimerStart(&Tmr[i],  *PS_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	

										
				break;
	
	
				case 2:
	
										
					
					if(TimerIsTimeOut(&Tmr[i])){
						prc->_step = 3;
						
					}	
						
				break;	
				
	
				case 3:
	
					prc->Status = prcsSTATUS_RampUp;
	
								
					if (*PS_Param_Set[i].pRampUp > 0) 
						Ramp[i].dy_up = fabsf(1.0/(*PS_Param_Set[i].pRampUp));
					else 
						Ramp[i].dy_up = 3.4e+38;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1.0;
					Ramp[i].set = 0;

					TimerStart(&Tmr[i],  *PS_Param_Set[i].pRampUp);

					prc->_step = 4;	
					
				break;	
	
	
	
				case 4:
	
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_AmpSP[i] = (AmpSP);
						prc->_step = 5;

					}					
										
	
				break;
	
	
	
				case 5:
	
					prc->Status = prcsSTATUS_Run;

														
				break;


				case 20:
	
					prc->Status = prcsSTATUS_StopDly;
					
					if(*PS_Param_Set[i].pStopDly > 0){
						TimerStart(&Tmr[i], *PS_Param_Set[i].pStopDly);
						prc->_step ++;
					}
					else					
						prc->_step = 22;
					
	
				break;

				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;	
	
	
				break;
	
	
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;
					
					if (*PS_Param_Set[i].pRampDwn > 0) 
						Ramp[i].dy_down = fabsf(1.0/(*PS_Param_Set[i].pRampDwn));
					else 
						Ramp[i].dy_down = 3.4e+38;
						
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					
					TimerStart(&Tmr[i],  *PS_Param_Set[i].pRampDwn);
				
					prc->_step++; 

				break;	


				case 23:
	
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
	
	
				break;
					
		
					
				case 50:	
					
					Ramp[i].y_set = 0.0;
					Ramp[i].x = 0.0;
					Ramp[i].set = 1;

					prc->Status = prcsSTATUS_ForceON;
	
				break;	


				case 60:	

					_AmpSP[i] = (AmpSP);
					Ramp[i].y_set = 1.0;
					Ramp[i].x = 1.0;
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;

					prc->_step = 5;
	
				break;	


				case 70:	
					
					prc->Status = prcsSTATUS_ForceCalib;
	
				break;	
				

	
			}

			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);



			switch(prc->_step){


				case 0: // Stop

					robot->Cntrl.Cmd.AmpSP =  0.0;	

				break;


				case 50: //Force ON

					robot->Cntrl.Cmd.AmpSP =  getFromCalib((*PS_Param_Set[i].pStartAmp), PS_Param_Set[i].pCalibPoints , CALIB_2POINTS);	
					
				break;


				case 70: //Calibration

					robot->Cntrl.Cmd.AmpSP =  prc->fParam[1];	

				break;


				default:


					switch((*PS_Param_Set[i].pMode)){

						case 0: // Straight

							//y =  Ramp[i].y;	

							if(prc->_step <= 5) // Ramp Up
								y = (*PS_Param_Set[i].pStartAmp) + Ramp[i].y * (AmpSP - (*PS_Param_Set[i].pStartAmp));
							else
								y = (*PS_Param_Set[i].pEndAmp) + Ramp[i].y * (AmpSP - (*PS_Param_Set[i].pEndAmp));

						break;	

						case 1: // Pulse Arc	- it is own generator	
						case 2: // Synch Pulse				
						case 3: // Synch In/Out
						
							switch(SynchClckToTIG){

								default:

									//y = Ramp[i].y ; // BackAmp

									if(prc->_step <= 5) // Ramp Up
										y = (*PS_Param_Set[i].pStartAmp) + Ramp[i].y * (AmpSP - (*PS_Param_Set[i].pStartAmp));
									else
										y = (*PS_Param_Set[i].pEndAmp) + Ramp[i].y * (AmpSP - (*PS_Param_Set[i].pEndAmp));

								break;

								case 1:

									//y = Ramp[i].y * ( *PS_Param_Set[i].pAmpSP)/ ( AmpSP); // ForeGround Amp

									if(prc->_step <= 5) // Ramp Up
										y = (*PS_Param_Set[i].pStartAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpSP) - (*PS_Param_Set[i].pStartAmp));
									else
										y = (*PS_Param_Set[i].pEndAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpSP) - (*PS_Param_Set[i].pEndAmp));

								break;


								case 2: // IN 

									//y = Ramp[i].y * ( *PS_Param_Set[i].pAmpInSP)/ ( AmpSP); // ForeGround Amp

									if(prc->_step <= 5) // Ramp Up
										y = (*PS_Param_Set[i].pStartAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpInSP) - (*PS_Param_Set[i].pStartAmp));
									else
										y = (*PS_Param_Set[i].pEndAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpInSP) - (*PS_Param_Set[i].pEndAmp));

								break;

								case 3: // IN 

									//y = Ramp[i].y * ( *PS_Param_Set[i].pAmpOutSP)/ ( AmpSP); // ForeGround Amp
									
									if(prc->_step <= 5) // Ramp Up
										y = (*PS_Param_Set[i].pStartAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpOutSP) - (*PS_Param_Set[i].pStartAmp));
									else
										y = (*PS_Param_Set[i].pEndAmp) + Ramp[i].y * (( *PS_Param_Set[i].pAmpOutSP) - (*PS_Param_Set[i].pEndAmp));

								break;

							}
							
																
						break;

					}

					switch(PS_Param_Set[i].BoostCmd){

						case 1:

							y += AmpBoost1;

						break;


						case 2:

							y += AmpBoost2;

						break;


						default:

						break;

						
					}

					if(y < 0.0)
						y = 0.0;

					if(prc->Param[0])
						robot->Cntrl.Cmd.AmpSP = getFromCalib(y, PS_Param_Set[i].pCalibPoints , CALIB_2POINTS);
					
					else
						robot->Cntrl.Cmd.AmpSP = getFromCalib(y, PS_Param_Set[i].pCalibPoints , CALIB_2POINTS) * ((* PS_Param_Set[i].pAmpBoost)+1.0);

				break;


			}

			

			//prc->Fbk[1] = Ramp[i].y; // Cmd Amp
			//prc->Fbk[1] = AmpSP;
			prc->Fbk[1] = ( *PS_Param_Set[i].pAmpSP);

			if((prc->Status == prcsSTATUS_Stopped) || (prc->Disable))				
				robot->Cntrl.Cmd.RUN  = 0;			
			else				
				robot->Cntrl.Cmd.RUN  = (robot->Cntrl.Cmd.AmpSP > 0.0);

			robot->Cntrl.Cmd.Mode = 3;

			prc->Fbk[2] = robot->Cntrl.Fbk.curArcAmp;
			prc->Fbk[3] = robot->Cntrl.Fbk.curArcVolt;			
			
			
			if((prc->Status == prcsSTATUS_Stopped) || prc->Disable) {
				prc->Fbk[0] = 0.0;
				//prc->Fbk[1] = (AmpSP);

			}else		
				prc->Fbk[0] = prc->Fbk[1]; // cur Amp the same as commanded

		
	}	
	
}



void TIG_PS_ANALOG(void)
{	
int i;
float y;


	
		for(i=0 ; i < TIG_PS_NUM ; i++){

			
			prc = PS_Param_Set[i].pPrc;
			
			if(!prc)
				break;

			if(prc->ConfigReady <= 0)
				break;

						
			LimScal_AmpCMD[i].x2 = PS_Param_Set[i].AmpCMD_Max;
			LimScal_AmpCMD[i].y2 = PS_Param_Set[i].rawCmdMax;

			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

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

			}
			else{
	
	
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsHalt:
					case prcsRESTART:

						prc->_step = 0;
						
					break;	

					case prcsForceON:
					
						if(prc->_step != 50)
							prc->_step = 50;


					break;	


					case prcsJump2Run:
					
						if(prc->_step != 60)
							prc->_step = 60;


					break;	

					case prcsCalib:
					
						if(prc->_step != 70)
							prc->_step = 70;


					break;	

	
					case prcsSTART:

					if((prc->_step == 0) || (prc->_step > 8))
						prc->_step = 1;

					break;


					case prcsQuickSTOP:
					case prcsSTOP:

						if((prc->_step != 0) && (prc->_step <= 8)){

							switch(prc->xCMD){

								case prcsQuickSTOP:

									prc->_step = 22;

								break;	

								case prcsSTOP:

									prc->_step = 20;

								break;	

							}


						}	
							

					break;
						
					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			
			if(TigSynchMode == 0 ) //Straight			
				AmpSP = *PS_Param_Set[i].pAmpSP;	
			else
				AmpSP = *PS_Param_Set[i].pBackAmp;

			AmpBoost1 = (*PS_Param_Set[i].pBoost1);
			AmpBoost2 = (*PS_Param_Set[i].pBoost2);

			switch(PS_Param_Set[i].BoostCmd){

				case 1:

					AmpSP += AmpBoost1;

				break;


				case 2:

					AmpSP += AmpBoost2;

				break;


				default:

				break;

				
			}

			if(AmpSP < 0.0)
				AmpSP = 0.0;
			
			//////////////////////////
			
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					prc->Status = prcsSTATUS_Stopped;
					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

			
								
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;


					Ramp[i].y_set = *PS_Param_Set[i].pStartAmp;
					Ramp[i].set = 1;

									
					if(*PS_Param_Set[i].pStartDly > 0){
						TimerStart(&Tmr[i],  *PS_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	

										
				break;
	
	
				case 2:
	
										
					
					if(TimerIsTimeOut(&Tmr[i])){
						prc->_step = 3;
						
					}	
						
				break;	
				
	
				case 3:
	
					prc->Status = prcsSTATUS_RampUp;
	
								
					if (*PS_Param_Set[i].pRampUp > 0) 
						Ramp[i].dy_up = fabsf((AmpSP - Ramp[i].y)/(*PS_Param_Set[i].pRampUp));
					else 
						Ramp[i].dy_up = 3.4e+38;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = AmpSP;
					Ramp[i].set = 0;

					TimerStart(&Tmr[i],  *PS_Param_Set[i].pRampUp);


#ifdef EXTENDED_LOGS

			       LogTxt(&LogMgr, "P: AMP Start: %.2f" , ABS_TVL_Pos);
#endif	

					prc->_step = 4;	
					
				break;	
	
	
	
				case 4:	
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_AmpSP[i] = (AmpSP);
						prc->_step = 5;

					}					
										
	
				break;	
	
	
				case 5:
	
					prc->Status = prcsSTATUS_Run;



					if(prc->Param[1]){

						AmpSP = AmpSP * (1.0 -  (*PS_Param_Set[i].pContin_Amp_Reduce));
												
						if(*PS_Param_Set[i].pContin_RampDwn > 0)
							Ramp[i].dy_up = fabsf((AmpSP - _AmpSP[i])/(*PS_Param_Set[i].pContin_RampDwn));
							//Ramp[i].dy_up = fabsf((AmpSP )/(*PS_Param_Set[i].pContin_RampDwn));
						else 
							Ramp[i].dy_up = 3.4e+38;
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].x = AmpSP;

						prc->_step = 7;

						break;


					}
					if(_AmpSP[i] != (AmpSP)){	
						_AmpSP[i] = (AmpSP);
						
						Ramp[i].x = AmpSP;

						
						if (*PS_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf((AmpSP)/(*PS_Param_Set[i].pRampUp));
							//Ramp[i].dy_up = fabsf((*PS_Param_Set[i].pSpeedSp - Ramp[i].y)/(*PS_Param_Set[i].pRampUp));
						else 
							Ramp[i].dy_up = 3.4e+38;
						
						Ramp[i].dy_down = Ramp[i].dy_up;
					}

					
				break;


				case 6:
	
					prc->Status = prcsSTATUS_Run;


					if(Ramp[i].x_reached ){

						if(*PS_Param_Set[i].pContin_Dwell > 0)
							TimerStart(&Tmr[i],  *PS_Param_Set[i].pContin_Dwell);
						else
							TimerStart(&Tmr[i], 0.01);

						prc->_step = 7;
						
					}					

				break;	


				case 7:


					//if(TimerIsTimeOut(&Tmr[i])){
					if(!prc->Param[1]){
																		
						if (*PS_Param_Set[i].pContin_RampUp > 0)
							Ramp[i].dy_up = fabsf((AmpSP - Ramp[i].x)/(*PS_Param_Set[i].pContin_RampUp));
							//Ramp[i].dy_up = fabsf((AmpSP)/(*PS_Param_Set[i].pContin_RampUp));
						else 
							Ramp[i].dy_up = 3.4e+38;
						
						Ramp[i].dy_down = Ramp[i].dy_up;

						Ramp[i].x = AmpSP;

						prc->_step = 8;


					}


				break;


				case 8:


					if(Ramp[i].x_reached ){


						prc->Param[1] = 0; // Reset signal

						prc->_step = 5;


					}


				break;


				case 20:
	
					prc->Status = prcsSTATUS_StopDly;
					
					if(*PS_Param_Set[i].pStopDly > 0){
						TimerStart(&Tmr[i], *PS_Param_Set[i].pStopDly);
						prc->_step ++;
					}
					else					
						prc->_step = 22;
					
	
				break;

				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;	
	
	
				break;
	
	
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;
					
					if (*PS_Param_Set[i].pRampDwn > 0) 
						Ramp[i].dy_down = fabsf((Ramp[i].y - (*PS_Param_Set[i].pEndAmp))/(*PS_Param_Set[i].pRampDwn));
					else 
						Ramp[i].dy_down = 3.4e+38;
						
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = *PS_Param_Set[i].pEndAmp;

					
					TimerStart(&Tmr[i],  *PS_Param_Set[i].pRampDwn);
				
					prc->_step++; 

				break;	


				case 23:
	
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

#ifdef EXTENDED_LOGS

			       		LogTxt(&LogMgr, "P: AMP Stop: %.2f" , ABS_TVL_Pos);
#endif	
						prc->_step = 0;	

					}
	
	
				break;
					
		
					
				case 50:	
					
					Ramp[i].y_set = *PS_Param_Set[i].pStartAmp;
					Ramp[i].set = 1;

					prc->Status = prcsSTATUS_ForceON;
	
				break;	


				case 60:	

					_AmpSP[i] = (AmpSP);
					Ramp[i].y_set = _AmpSP[i];
					Ramp[i].x = _AmpSP[i];
					Ramp[i].set = 1;
					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;

					prc->_step = 5;
	
				break;	


				case 70:	
					
					prc->Status = prcsSTATUS_ForceCalib;
	
				break;	
				

	
			}

			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);

			


			if(PS_Param_Set[i].pAnal_Output && PS_Param_Set[i].pCalibPoints ){
				
								
				if(prc->_step == 70){ // Calib
				
				   y = prc->fParam[1];

				   //prc->Fbk[1] = y; // Cmd Amp
				
				   
				} 	
				else{	
								

					if(prc->Param[0])
						y = getFromCalib(Ramp[i].y, PS_Param_Set[i].pCalibPoints , CALIB_2POINTS);
					
					else
						y = getFromCalib(Ramp[i].y, PS_Param_Set[i].pCalibPoints , CALIB_2POINTS) * ((* PS_Param_Set[i].pAmpBoost)+1.0);

				}
				
				 prc->Fbk[1] =  Ramp[i].y; // Cmd Amp
									
				(*PS_Param_Set[i].pAnal_Output) = (INT)(LimScal( y , &LimScal_AmpCMD[i]));

				if(PS_Param_Set[i].pAnal_Output_neg)
					(*PS_Param_Set[i].pAnal_Output_neg) = (*PS_Param_Set[i].pAnal_Output) * (-1);
					
			}	

			
				
 
			if(PS_Param_Set[i].pOutput){

				if((prc->Status == prcsSTATUS_Stopped) || (prc->Disable))
					
					(*PS_Param_Set[i].pOutput) = 0;
				
				else
					
					(*PS_Param_Set[i].pOutput) = ((*PS_Param_Set[i].pAnal_Output) > 0);

			}

		if(!(*PS_Param_Set[i].pOutput))
				prc->Fbk[1] = 0; // Cmd Amp

		if((prc->Status == prcsSTATUS_Stopped) || prc->Disable) {
			prc->Fbk[0] = 0.0;
			prc->Fbk[1] = (AmpSP);
			
		}else		
			prc->Fbk[0] = prc->Fbk[1]; // cur Amp the same as commanded


		prc->Fbk[1] = ( *PS_Param_Set[i].pAmpSP);	

		
}
	
	
	
}

