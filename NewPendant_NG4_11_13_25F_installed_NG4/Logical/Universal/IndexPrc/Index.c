
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>

_LOCAL GenProcess_typ* prc;
_LOCAL  StateVectorProxy_typ *vect;
_LOCAL  StateVectorProxy_typ *slaveVect;
_LOCAL float actDistInTicks[INDEX_NUM] , targetDistInTicks[INDEX_NUM];
_LOCAL float actDistInTicks_slv[INDEX_NUM] , targetDistInTicks_slv[INDEX_NUM] ,  StepDistance[INDEX_NUM];
  
//_LOCAL MTProfilePositionGenerato r_typ Profile[GEN_MTR_NUM];
_LOCAL TON_typ TMR1[GEN_MTR_NUM];
_LOCAL float Acc , Dec , Vel;
_LOCAL UINT KK_UpdateParam;


/// For new BP v2 Head and New path generator
//_LOCAL HCRMovePara_typ MoveParam;
_LOCAL HCRMoveParamAzz_type MoveParam[INDEX_NUM];
_LOCAL HCRSPFazz_type SPF[INDEX_NUM];
///////////


void INDX_Stnd_2Tracks_DCMtr(void);
void INDX_Stnd_2Tracks_TML(void);
void INDX_TVL_Mtr_TML(void);
void INDX_MIG_TML(void);




//void UpdateProfileParam(float gain);

void _INIT Index_Init(void)
{int i;

	//////   INIT for old DC  motor module - option 0 /////////////

	Process[0].Disable = Process[0].Disable;
	Hardware[0].Anal_Inp[0] = Hardware[0].Anal_Inp[0];

	Acc = 10.0;
	Dec = 10.0;
	Vel = 7.0;

	//UpdateProfileParam(1.0);

	//////   INIT for new TML drives - option 1 /////////////
	// New Path Gen function for TML option



	for(i=0 ; i < INDEX_NUM; i++){
		
		SPF[i].Init = 0;
		SPF[i].pMove_Para = &MoveParam;

		SPF[i].Intr.TaskPeriod = 0.0024; // 2.4 ms

		MoveParam[i].vC_pos = Vel;
		
		MoveParam[i].vC_neg = Vel;

		MoveParam[i].aAcc_pos = Acc;	
		MoveParam[i].aDec_pos = Dec;

		MoveParam[i].aAcc_neg = Acc;	
		MoveParam[i].aDec_neg = Dec;
	
	}


	//////////////////////////////////////
	
	Process[0].Disable = Process[0].Disable;
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;
	CurCalib.Points[0].num = CurCalib.Points[0].num;	
	DefaultCalib.Points[0].num = DefaultCalib.Points[0].num;

	

}

void _CYCLIC Index_Cyclic(void)
{
	if(!INDEX_Param_Set[0].pPrc)
		return;
		
		
	if(INDEX_Param_Set[0].pPrc->ConfigReady <= 0)
		return;


	switch(INDEX_Param_Set[0].Mode){
	


		default:

			INDX_Stnd_2Tracks_DCMtr();	
			

		break;


		case 1:

			INDX_Stnd_2Tracks_TML();


		break;


		case 2:

			INDX_TVL_Mtr_TML();

		break;


		case 3:

			INDX_MIG_TML();

		break;


		case 4:


		break;
		
	}


}

void INDX_Stnd_2Tracks_TML(void)
{	
int i;
		
		

		for(i=0 ; i < INDEX_NUM ; i++){		

					
			
			if(! INDEX_Param_Set[i].pPrc)
				break;

			prc = INDEX_Param_Set[i].pPrc;

			if(prc->ConfigReady <= 0)
				break;
					
			if(prc->Disable){

				prc->_step = 0xFF;
				
				switch(prc->xCMD){

				
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					
					case prcsHalt:
					case prcsRESTART:
					case prcsSTOP:

						prc->Status = prcsSTATUS_Stopped;
						
					break;

				
					default:

						if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
							prc->Status = prcsSTATUS_Stopped;
						
					break;	
					


				}


				continue;

			}else{


				if(!INDEX_Param_Set[i].pVect)
					break;

				vect = INDEX_Param_Set[i].pVect ;

				if(INDEX_Param_Set[i].VectStateNum == 2)
					slaveVect = INDEX_Param_Set[i].pVect + 1;
				else
					slaveVect = vect;

				//if(!slaveVect)
					//break;
				
				actDistInTicks[i] = vect->FbkVect.x;

				actDistInTicks_slv[i] = slaveVect->FbkVect.x;
	
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsHalt:	
						
						case prcsRESTART:

							prc->_step = 0;
							
						break;	
			
			
						case prcsSTART:

						if((prc->_step) == 0 || (prc->_step > 5))
							prc->_step = 1;

						break;


						case prcsDirectCntrl:						
			
							
							if(prc->_step != 60){					

								prc->_step = 60;
							}

						break;							
				
						case prcsJOGpos:
							
							prc->Status = prcsSTATUS_Jog;

							if(prc->_step == 3){

								
								if(prc->Param[3])
									prc->_step = 60; // Direct Control

								else
									prc->_step = 50;

								
							}

						
						break;	

						case prcsSTEPpos:
							
							prc->Status = prcsSTATUS_Step;

							if(prc->_step == 3){


								if(INDEX_Param_Set[i].pStepDist){
									
									//StepDistance[i] = (*INDEX_Param_Set[i].pStepDist);
									StepDistance[i] = prc->fParam[1];
									
									prc->_step = 80;
									
								}
							}

						
						break;	

						

						case prcsSTOP:

							if((prc->_step != 0) && (prc->_step <= 5))
								prc->_step = 20;

						break;			
						
			
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
		
		
			switch(prc->_step){


				case 0:

					prc->Status = prcsSTATUS_Init;

					vect->InVal = 0;
				    slaveVect->InVal  = 0; 

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_Volt){
						slaveVect->InVal = 0.0;
						slaveVect->Mode = vectSt_Mode_Volt;
					}
					else
						prc->_step++;

				break;

				case 1:

					prc->_step++;

				break;


				case 2:

					slaveVect->InVal  = 0; 
					vect->InVal = 0; 	

					prc->_step++;

				break;


				case 3:

					if((vect->FbkVect.Mode != vectSt_Mode_Volt)
						|| (slaveVect->FbkVect.Mode != vectSt_Mode_Volt))
						prc->_step = 0;
					
					else
						prc->Status = prcsSTATUS_Stopped;
				
				break;

				/////////////////////////////////////////////////////
				//////////////// JOGING 


				case 50:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}						

					prc->Status = prcsSTATUS_Jog;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_PT){
						slaveVect->InVal = slaveVect->FbkVect.x;
						slaveVect->Mode = vectSt_Mode_PT;
					}
					else{ 	

									
						TimerStart(&TMR1[i], 0.5);
						prc->_step++;


					}



				break;

				case 51:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}
					
					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
									

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pJogSpeedDirCntrl);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;


					if(prc->fParam[0] > 0)
						SPF[i].s_end  = 10.0e+28;
					else if(prc->fParam[0] < 0)
						SPF[i].s_end =  -10.0e+28;
					else {
						prc->_step = 0;
						break;
					}

					SPF[i].s_home = vect->InVal;					
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;					
					
					prc->_step++;


				break;
				
								
				case 52:
					
					vect->InVal = SPF[i].s_set;
					slaveVect->InVal = SPF[i].s_set;
					

					
					if(prc->fParam[0] == 0.0){	

						SPF[i].Brake = 1;			
								
						prc->_step++;

					}						
				break;

			
				case 53:
					
					vect->InVal = SPF[i].s_set;
					slaveVect->InVal = SPF[i].s_set;
					

					
					if(SPF[i].Reached){	

						TimerStart(&TMR1[i], 0.5);
						
						prc->_step++;

					}	

										
				break;
				

				case 54:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
					
					prc->_step = 0;

				break;

				
				

				
				////////////////////////////////////////////////
				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					if(INDEX_Param_Set[i].pJogSpeedDirCntrl){
							
						if(prc->fParam[0] != 0.0){


							switch(prc->Param[3]){

								case 1:

									vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									slaveVect->InVal = 0.0;

								break;

								case 2:

									slaveVect->InVal  = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									vect->InVal = 0.0;

								break;

								default:

									vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									slaveVect->InVal = vect->InVal;
								break;


							}

						 							
							
						}		
						else 
							prc->_step = 2;

							
					}						
									

				break;								

			
				case 80:

					prc->Status = prcsSTATUS_Step;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}
				
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_PT){
						slaveVect->InVal = slaveVect->FbkVect.x;
						slaveVect->Mode = vectSt_Mode_PT;
					}
					
					else {

						TimerStart(&TMR1[i], 1.0);
						prc->_step = 90;
						//prc->_step++;

					}

				break;	


		
				case 90:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pStepVel);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;

					SPF[i].s_home = vect->InVal;
					SPF[i].s_end = SPF[i].s_home + StepDistance[i];
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;		
					prc->_step++;

				break;

				
				case 91:

					vect->InVal = SPF[i].s_set;
					slaveVect->InVal = SPF[i].s_set;

					if(SPF[i].Reached){	
			
						TimerStart(&TMR1[i], 1.0);
						prc->_step++;

					}


				break;

				case 92:

					if(TimerIsTimeOut(&TMR1[i]) )
						//|| (vect->FbkVect.x > (vect->InVal -1.0) ))
						prc->_step = 0;
				
					
				break;

				
		

				case 200: // Test


				break;


				default:

					prc->_step = 0;
					
				break;	



			}

			TimerExecute(&TMR1[i]);
			
			HCRSPFazz(&SPF[i]);
			SPF[i].Init = 0;
			SPF[i].Brake = 0;

			/*
			MTProfilePositionGenerator(&Profile[i]);
			Profile[i].SetHomePosition = 0;
			Profile[i].Start = 0;
			Profile[i].Stop = 0;
			Profile[i].Abort = 0;
			*/

			prc->Fbk[0] = vect->FbkVect.x;
			prc->Fbk[1] = slaveVect->FbkVect.x;
			prc->Fbk[2] = fabsf(vect->FbkVect.x -slaveVect->FbkVect.x) ;

					
			 
		}

return;		

}







void INDX_Stnd_2Tracks_DCMtr(void)
{	
int i;

	


		for(i=0 ; i < INDEX_NUM ; i++){		

					
			
			if(! INDEX_Param_Set[i].pPrc)
				break;

			prc = INDEX_Param_Set[i].pPrc;

			if(prc->ConfigReady <= 0)
				break;
					
			if(prc->Disable){

				prc->_step = 0xFF;
				
				switch(prc->xCMD){

				
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					
					case prcsHalt:
					case prcsRESTART:
					case prcsSTOP:

						prc->Status = prcsSTATUS_Stopped;
						
					break;

				
					default:

						if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
							prc->Status = prcsSTATUS_Stopped;
						
					break;	
					


				}


				continue;

			}else{


				if(!INDEX_Param_Set[i].pVect)
					break;

				vect = INDEX_Param_Set[i].pVect ;

				if(INDEX_Param_Set[i].VectStateNum == 2)
					slaveVect = INDEX_Param_Set[i].pVect + 1;
				else
					slaveVect = 0;

				if(!slaveVect)
					break;
				
				actDistInTicks[i] = vect->FbkVect.x;

				actDistInTicks_slv[i] = slaveVect->FbkVect.x;
	
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsHalt:	
						
						case prcsRESTART:

							prc->_step = 0;
							
						break;	
			
			
						case prcsSTART:

						if((prc->_step) == 0 || (prc->_step > 5))
							prc->_step = 1;

						break;


						case prcsDirectCntrl:						
			
							
							if(prc->_step != 60){					

								prc->_step = 60;
							}

						break;							
				
						case prcsJOGpos:
							
							prc->Status = prcsSTATUS_Jog;

							if(prc->_step == 3){

								
								if(prc->Param[3])
									prc->_step = 60; // Direct Control

								else
									prc->_step = 50;

								
							}

						
						break;	

						case prcsSTEPpos:
							
							prc->Status = prcsSTATUS_Step;

							if(prc->_step == 3){


								if(INDEX_Param_Set[i].pStepDist){
									
									//StepDistance[i] = (*INDEX_Param_Set[i].pStepDist);
									StepDistance[i] = prc->fParam[1];
									
									prc->_step = 80;
									
								}
							}

						
						break;	

						

						case prcsSTOP:

							if((prc->_step != 0) && (prc->_step <= 5))
								prc->_step = 20;

						break;			
						
			
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
		
		
			switch(prc->_step){


				case 0:

					prc->Status = prcsSTATUS_Init;

					if(slaveVect)	
						slaveVect->InVal  = 0; 

					vect->InVal = 0; 	

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_Volt){
						slaveVect->InVal = 0.0;
						slaveVect->Mode = vectSt_Mode_Volt;
					}
					else
						prc->_step++;

				break;

				case 1:

					prc->_step++;

				break;


				case 2:

					slaveVect->InVal  = 0; 
					vect->InVal = 0; 	

					prc->_step++;

				break;


				case 3:

					if((vect->FbkVect.Mode != vectSt_Mode_Volt)
						|| (slaveVect->FbkVect.Mode != vectSt_Mode_Volt))
						prc->_step = 0;
					
					else
						prc->Status = prcsSTATUS_Stopped;
				
				break;

				/////////////////////////////////////////////////////
				//////////////// JOGING 


				case 50:

					prc->Status = prcsSTATUS_Jog;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Pos){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_Pos;
					}
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_Pos){
						slaveVect->InVal = slaveVect->FbkVect.x;
						slaveVect->Mode = vectSt_Mode_Pos;
					}
					else 						
						prc->_step++;



				break;
				
				case 51:

					/*
					UpdateProfileParam( (*INDEX_Param_Set[i].pJogSpeedDirCntrl));
					Profile[i].HomePosition = vect->InVal;
					Profile[i].SetHomePosition = 1;

					*/

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pJogSpeedDirCntrl);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;
				

					if(prc->fParam[0] > 0)
						SPF[i].s_end  = 10.0e+28;
					else if(prc->fParam[0] < 0)
						SPF[i].s_end =  -10.0e+28;
					else {
						prc->_step = 0;
						break;
					}

					SPF[i].s_home = vect->InVal;					
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;					
					
					
					prc->_step++;

				break;

				case 52:

					//vect->InVal = Profile[i].Position;
					//slaveVect->InVal = Profile[i].Position;

					vect->InVal = SPF[i].s_set;
					slaveVect->InVal = SPF[i].s_set;
					

					if(SPF[i].Reached)						
						prc->_step++;
					else if(prc->fParam[0] == 0.0)
						prc->_step++;
						
				break;

				case 53:

					SPF[i].Brake = 1;
					prc->_step ++;

				break;

				case 54:

					if(SPF[i].Reached){
					
						prc->_step = 0;
					}

				break;
				


				////////////////////////////////////////////////
				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					if(INDEX_Param_Set[i].pJogSpeedDirCntrl){
							
						if(prc->fParam[0] != 0.0){


							switch(prc->Param[3]){

								case 1:

									vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									slaveVect->InVal = 0.0;

								break;

								case 2:

									slaveVect->InVal  = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									vect->InVal = 0.0;

								break;

							default:

									vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
									slaveVect->InVal = vect->InVal;
								break;


							}

						 							
							
						}		
						else 
							prc->_step = 2;

							
					}						
									

				break;								

			
				case 80:

					prc->Status = prcsSTATUS_Step;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Pos){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_Pos;
					}
					else if(slaveVect->FbkVect.Mode != vectSt_Mode_Pos){
						slaveVect->InVal = slaveVect->FbkVect.x;
						slaveVect->Mode = vectSt_Mode_Pos;
					}
					else 
						prc->_step = 90;
						//prc->_step++;

				break;	

				
				case 90:
				
									

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pStepVel);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;

					SPF[i].s_home = vect->InVal;
					SPF[i].s_end = SPF[i].s_home + StepDistance[i];
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;		
					prc->_step++;

				break;

				
				case 91:

					vect->InVal = SPF[i].s_set;
					slaveVect->InVal = SPF[i].s_set;

					if(SPF[i].Reached){ 

						TimerStart(&TMR1[i], 2.0);
						prc->_step++;

					}


				break;

				case 93:

					if(TimerIsTimeOut(&TMR1[i]))
						prc->_step = 0;

				break;


			/*	

				case 81:

					vect->InVal += StepDistance[i];
					slaveVect->InVal += StepDistance[i];
					
					prc->_step++;

				break;			


				case 82:

					
					if((fabsf(vect->InVal - vect->FbkVect.x ) < 1.0)
						&&  (fabsf(slaveVect->InVal - slaveVect->FbkVect.x ) < 1.0))
						prc->_step = 0;
					
				break;

				case 90:

					UpdateProfileParam((*INDEX_Param_Set[i].pStepVel));
					Profile[i].HomePosition = vect->InVal;
					Profile[i].SetHomePosition = 1;
					prc->_step++;

				break;

				case 91:

					Profile[i].EndPosition = Profile[i].HomePosition + StepDistance[i];
					Profile[i].Start = 1;
					prc->_step++;

				break;


				case 92:

					vect->InVal = Profile[i].Position;
					slaveVect->InVal = Profile[i].Position;

					if(Profile[i].MotionState == mtPROFILE_STANDSTILL){	

						TimerStart(&TMR1[i], 2.0);
						prc->_step++;

					}


				break;

				case 93:

					if(TimerIsTimeOut(&TMR1[i]))
						prc->_step = 0;

				break;

				*/

				
		

				case 200: // Test


				break;


				default:

					prc->_step = 0;
					
				break;	



			}

			TimerExecute(&TMR1[i]);

			
			HCRSPFazz(&SPF[i]);
			SPF[i].Init = 0;
			SPF[i].Brake = 0;

			/*
			MTProfilePositionGenerator(&Profile[i]);
			Profile[i].SetHomePosition = 0;
			Profile[i].Start = 0;
			Profile[i].Stop = 0;
			Profile[i].Abort = 0;
			*/

			prc->Fbk[0] = vect->FbkVect.x;
			prc->Fbk[1] = slaveVect->FbkVect.x;
			prc->Fbk[2] = fabsf(vect->FbkVect.x -slaveVect->FbkVect.x) ;

					
			 
		}

return;		

}


void INDX_MIG_TML(void)
{	
int i;
		
		

		for(i=0 ; i < INDEX_NUM ; i++){		

					
			
			if(! INDEX_Param_Set[i].pPrc)
				break;

			prc = INDEX_Param_Set[i].pPrc;

			if(prc->ConfigReady <= 0)
				break;
					
			if(prc->Disable){

				prc->_step = 0xFF;
				
				switch(prc->xCMD){

				
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					
					case prcsHalt:
					case prcsRESTART:
					case prcsSTOP:

						prc->Status = prcsSTATUS_Stopped;
						
					break;

				
					default:

						if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
							prc->Status = prcsSTATUS_Stopped;
						
					break;	
					


				}


				continue;

			}else{


				if(!INDEX_Param_Set[i].pVect)
					break;

				vect = INDEX_Param_Set[i].pVect ;

					
				actDistInTicks[i] = vect->FbkVect.x;

				
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsHalt:	
						
						case prcsRESTART:

							prc->_step = 0;
							
						break;	
			
			
						case prcsSTART:

						if((prc->_step) == 0 || (prc->_step > 5))
							prc->_step = 1;

						break;


						case prcsDirectCntrl:						
			
							
							if(prc->_step != 60){					

								prc->_step = 60;
							}

						break;							
				
						case prcsJOGpos:
							
							prc->Status = prcsSTATUS_Jog;

							if(prc->_step == 3){

								/*
								if(prc->Param[3])
									prc->_step = 60; // Direct Control

								else
									prc->_step = 50;
								*/

								prc->_step = 60; // Direct Control by Vel
								
							}

						
						break;	

						case prcsSTEPpos:
							
							prc->Status = prcsSTATUS_Step;

							if(prc->_step == 3){


								if(INDEX_Param_Set[i].pStepDist){
									
									//StepDistance[i] = (*INDEX_Param_Set[i].pStepDist);
									StepDistance[i] = prc->fParam[1];
									
									prc->_step = 80;
									
								}
							}

						
						break;	

						

						case prcsSTOP:

							if((prc->_step != 0) && (prc->_step <= 5))
								prc->_step = 20;

						break;			
						
			
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
		
		
			switch(prc->_step){


				case 0:

					prc->Status = prcsSTATUS_Init;

					vect->InVal = 0;
				    
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Vel;
					}					
					else
						prc->_step++;

				break;

				case 1:

					prc->_step++;

				break;


				case 2:

					vect->InVal = 0; 	

					prc->_step++;

				break;


				case 3:

				prc->Status = prcsSTATUS_Stopped;
				
				break;

				/////////////////////////////////////////////////////
				//////////////// JOGING 


				case 50:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}						

					prc->Status = prcsSTATUS_Jog;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}				
					else{ 
									
						TimerStart(&TMR1[i], 0.5);
						prc->_step++;


					}

				break;

				case 51:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}
					
					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
									

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pJogSpeedDirCntrl);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;


					if(prc->fParam[0] > 0)
						SPF[i].s_end  = 10.0e+28;
					else if(prc->fParam[0] < 0)
						SPF[i].s_end =  -10.0e+28;
					else {
						prc->_step = 0;
						break;
					}

					SPF[i].s_home = vect->InVal;					
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;					
					
					prc->_step++;


				break;
				
								
				case 52:
					
					vect->InVal = SPF[i].s_set;
										
					if(prc->fParam[0] == 0.0){	

						SPF[i].Brake = 1;			
								
						prc->_step++;

					}						
				break;

			
				case 53:
					
					vect->InVal = SPF[i].s_set;
										
					if(SPF[i].Reached){	

						TimerStart(&TMR1[i], 0.5);
						
						prc->_step++;

					}	

										
				break;
				

				case 54:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
					
					prc->_step = 0;

				break;

				
				

				
				////////////////////////////////////////////////
				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Vel;
						break;
					}					

					if(INDEX_Param_Set[i].pJogSpeedDirCntrl){
							
						if(prc->fParam[0] != 0.0){


							vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
				
						}		
						else 
							prc->_step = 2;

							
					}						
									

				break;								

			
				case 80:

					prc->Status = prcsSTATUS_Step;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}
									
					else {

						TimerStart(&TMR1[i], 1.0);
						prc->_step = 90;
						//prc->_step++;

					}

				break;	


		
				case 90:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pStepVel);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;

					SPF[i].s_home = vect->InVal;
					SPF[i].s_end = SPF[i].s_home + StepDistance[i];
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;		
					prc->_step++;

				break;

				
				case 91:

					vect->InVal = SPF[i].s_set;

					if(SPF[i].Reached){	
			
						TimerStart(&TMR1[i], 1.0);
						prc->_step++;

					}


				break;

				case 92:

					if(TimerIsTimeOut(&TMR1[i]) )
						//|| (vect->FbkVect.x > (vect->InVal -1.0) ))
						prc->_step = 0;
				
					
				break;

				
		

				case 200: // Test


				break;


				default:

					prc->_step = 0;
					
				break;	



			}

			TimerExecute(&TMR1[i]);
			
			HCRSPFazz(&SPF[i]);
			SPF[i].Init = 0;
			SPF[i].Brake = 0;

			/*
			MTProfilePositionGenerator(&Profile[i]);
			Profile[i].SetHomePosition = 0;
			Profile[i].Start = 0;
			Profile[i].Stop = 0;
			Profile[i].Abort = 0;
			*/

			prc->Fbk[0] = vect->FbkVect.x;
			/*
			prc->Fbk[1] = slaveVect->FbkVect.x;
			prc->Fbk[2] = fabsf(vect->FbkVect.x -slaveVect->FbkVect.x) ;
			*/

			prc->Fbk[1] = vect->FbkVect.x;
			prc->Fbk[2] = 0;

					
			 
		}

return;		

}

void INDX_TVL_Mtr_TML(void)
{	
int i;
		
		

		for(i=0 ; i < INDEX_NUM ; i++){		

					
			
			if(! INDEX_Param_Set[i].pPrc)
				break;

			prc = INDEX_Param_Set[i].pPrc;

			if(prc->ConfigReady <= 0)
				break;
					
			if(prc->Disable){

				prc->_step = 0xFF;
				
				switch(prc->xCMD){

				
					case prcsSTART:

						prc->Status = prcsSTATUS_Run;
						
					break;

					
					case prcsHalt:
					case prcsRESTART:
					case prcsSTOP:

						prc->Status = prcsSTATUS_Stopped;
						
					break;

				
					default:

						if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
							prc->Status = prcsSTATUS_Stopped;
						
					break;	
					


				}


				continue;

			}else{


				if(!INDEX_Param_Set[i].pVect)
					break;

				vect = INDEX_Param_Set[i].pVect ;

					
				actDistInTicks[i] = vect->FbkVect.x;

				
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsHalt:	
						
						case prcsRESTART:

							prc->_step = 0;
							
						break;	
			
			
						case prcsSTART:

						if((prc->_step) == 0 || (prc->_step > 5))
							prc->_step = 1;

						break;


						case prcsDirectCntrl:						
			
							
							if(prc->_step != 60){					

								prc->_step = 60;
							}

						break;							
				
						case prcsJOGpos:
							
							prc->Status = prcsSTATUS_Jog;

							if(prc->_step == 3){

								/*
								if(prc->Param[3])
									prc->_step = 60; // Direct Control

								else
									prc->_step = 50;
								*/

								prc->_step = 60; // Direct Control by Vel
								
							}

						
						break;	

						case prcsSTEPpos:
							
							prc->Status = prcsSTATUS_Step;

							if(prc->_step == 3){


								if(INDEX_Param_Set[i].pStepDist){
									
									//StepDistance[i] = (*INDEX_Param_Set[i].pStepDist);
									StepDistance[i] = prc->fParam[1];
									
									prc->_step = 80;
									
								}
							}

						
						break;	

						

						case prcsSTOP:

							if((prc->_step != 0) && (prc->_step <= 5))
								prc->_step = 20;

						break;			
						
			
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
		
		
			switch(prc->_step){


				case 0:

					prc->Status = prcsSTATUS_Init;

					vect->InVal = 0;
				    
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Vel;
					}					
					else
						prc->_step++;

				break;

				case 1:

					prc->_step++;

				break;


				case 2:

					vect->InVal = 0; 	

					prc->_step++;

				break;


				case 3:

					if(vect->FbkVect.Mode != vectSt_Mode_Vel)
						prc->_step = 0;
					
					else
						prc->Status = prcsSTATUS_Stopped;
				
				break;

				/////////////////////////////////////////////////////
				//////////////// JOGING 


				case 50:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}						

					prc->Status = prcsSTATUS_Jog;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}				
					else{ 
									
						TimerStart(&TMR1[i], 0.5);
						prc->_step++;


					}

				break;

				case 51:

					if(prc->fParam[0] == 0){

						prc->_step = 0;
							break;
					}
					
					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
									

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pJogSpeedDirCntrl);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;


					if(prc->fParam[0] > 0)
						SPF[i].s_end  = 10.0e+28;
					else if(prc->fParam[0] < 0)
						SPF[i].s_end =  -10.0e+28;
					else {
						prc->_step = 0;
						break;
					}

					SPF[i].s_home = vect->InVal;					
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;					
					
					prc->_step++;


				break;
				
								
				case 52:
					
					vect->InVal = SPF[i].s_set;
										
					if(prc->fParam[0] == 0.0){	

						SPF[i].Brake = 1;			
								
						prc->_step++;

					}						
				break;

			
				case 53:
					
					vect->InVal = SPF[i].s_set;
										
					if(SPF[i].Reached){	

						TimerStart(&TMR1[i], 0.5);
						
						prc->_step++;

					}	

										
				break;
				

				case 54:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;
					
					
					prc->_step = 0;

				break;

				
				

				
				////////////////////////////////////////////////
				
				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;

					if(INDEX_Param_Set[i].pJogSpeedDirCntrl){
							
						if(prc->fParam[0] != 0.0){


							vect->InVal = (*INDEX_Param_Set[i].pJogSpeedDirCntrl) * prc->fParam[0] * INDEX_Param_Set[i].MaxSpeed;	
				
						}		
						else 
							prc->_step = 2;

							
					}						
									

				break;								

			
				case 80:

					prc->Status = prcsSTATUS_Step;
					
					if(vect->FbkVect.Mode != vectSt_Mode_PT){
						vect->InVal = vect->FbkVect.x;
						vect->Mode = vectSt_Mode_PT;
					}
									
					else {

						TimerStart(&TMR1[i], 1.0);
						prc->_step = 90;
						//prc->_step++;

					}

				break;	


		
				case 90:	

					if(!TimerIsTimeOut(&TMR1[i])) 
						break;

					MoveParam[i].vC_pos = Vel * (*INDEX_Param_Set[i].pStepVel);
					MoveParam[i].vC_neg = MoveParam[i].vC_pos;

					SPF[i].s_home = vect->InVal;
					SPF[i].s_end = SPF[i].s_home + StepDistance[i];
					SPF[i].Set_home = 1;
					SPF[i].Init = 1;		
					prc->_step++;

				break;

				
				case 91:

					vect->InVal = SPF[i].s_set;

					if(SPF[i].Reached){	
			
						TimerStart(&TMR1[i], 1.0);
						prc->_step++;

					}


				break;

				case 92:

					if(TimerIsTimeOut(&TMR1[i]) )
						//|| (vect->FbkVect.x > (vect->InVal -1.0) ))
						prc->_step = 0;
				
					
				break;

				
		

				case 200: // Test


				break;


				default:

					prc->_step = 0;
					
				break;	



			}

			TimerExecute(&TMR1[i]);
			
			HCRSPFazz(&SPF[i]);
			SPF[i].Init = 0;
			SPF[i].Brake = 0;

			/*
			MTProfilePositionGenerator(&Profile[i]);
			Profile[i].SetHomePosition = 0;
			Profile[i].Start = 0;
			Profile[i].Stop = 0;
			Profile[i].Abort = 0;
			*/

			prc->Fbk[0] = vect->FbkVect.x;
			/*
			prc->Fbk[1] = slaveVect->FbkVect.x;
			prc->Fbk[2] = fabsf(vect->FbkVect.x -slaveVect->FbkVect.x) ;
			*/

			prc->Fbk[1] = vect->FbkVect.x;
			prc->Fbk[2] = 0;

					
			 
		}

return;		

}

/*
void UpdateProfileParam(float gain){
UINT i;

	for (i = 0 ; i < GEN_MTR_NUM ; i++){

		Profile[i].Enable = 1;
		
		Profile[i].Parameter.AccelerationNegDirection = Acc;
		Profile[i].Parameter.AccelerationPosDirection = Profile[i].Parameter.AccelerationNegDirection;

		Profile[i].Parameter.DecelerationNegDirection = Dec;
		Profile[i].Parameter.DecelerationPosDirection = Profile[i].Parameter.DecelerationNegDirection;

		Profile[i].Parameter.VelocityPosDirection = Vel * gain;
		Profile[i].Parameter.VelocityNegDirection = Vel * gain;

		Profile[i].Parameter.JoltTime = 0.0;
		
		Profile[i].Update = 1;
		
		MTProfilePositionGenerator(&Profile[i]);
		Profile[i].Update = 0;
	}	

	return;

}
*/


