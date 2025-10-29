/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: SingleProcess
 * File: SingleProcess.c
 * Author: ckaczkowski
 * Created: October 14, 2013
 ********************************************************************
 * Implementation of program SingleProcess
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>


#define ClassTime 0.0024

_LOCAL LCRRamp_typ Ramp[OSC_NUM];
_LOCAL TON_typ Tmr[OSC_NUM];
 GenProcess_typ* prc;
 StateVectorProxy_typ* vect;

_LOCAL HCRNode_typ l_NodeVector[CAM_NUM][7];
_LOCAL HCRLinSpline_typ fub_LinSpline[OSC_NUM][CAM_NUM];
_LOCAL float time[OSC_NUM], amplit[OSC_NUM] , oldAmplit[OSC_NUM];
_LOCAL float Dist[OSC_NUM] , Pos[OSC_NUM] , StartPos[OSC_NUM];
_LOCAL BOOL Invert[OSC_NUM];
_LOCAL HCRSPF_typ path[OSC_NUM];
_LOCAL HCRMovePara_typ  MovePara[OSC_NUM];
_LOCAL float JogSpeedFact[OSC_NUM];
_LOCAL float VirtCenter[OSC_NUM] , orgStarPos[OSC_NUM];
_LOCAL float KK_Ampl[OSC_NUM] , KK_Excur[OSC_NUM] , KK_DwellIn[OSC_NUM] , KK_DwellOut[OSC_NUM];


_LOCAL float DirectCntrlVolt;
_LOCAL float VoltRampDown;



float GenCam(UINT k);


void _INIT OscProcess_Init(void)
{int i , k;

	 VoltRampDown = 0.5; 
			
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;
	Process[0].Disable = Process[0].Disable;

	DirectCntrlVolt = 12.0;


	for(i = 0 ; i < CAM_NUM ; i++){

		switch(i){


			case 1:

				l_NodeVector[i][0].X= 0.0;  	l_NodeVector[i][0].Y= 0.0; 
				l_NodeVector[i][1].X= 0.1;  	l_NodeVector[i][1].Y= 0.1; 
				
				l_NodeVector[i][2].X= 0.4;  	l_NodeVector[i][2].Y= 0.4 ; 
				l_NodeVector[i][3].X= 0.5;  	l_NodeVector[i][3].Y= 0.5 ;
				l_NodeVector[i][4].X= 0.6;  	l_NodeVector[i][4].Y= 0.6 ;
				
				l_NodeVector[i][5].X= 0.9;  	l_NodeVector[i][5].Y= 0.9; 
				l_NodeVector[i][6].X= 1.0;  	l_NodeVector[i][6].Y= 1.0;


			break;	


			case 2:

				l_NodeVector[i][0].X= 0.0;  	l_NodeVector[i][0].Y= 0.0; 
				l_NodeVector[i][1].X= 0.05;  	l_NodeVector[i][1].Y= 0.1; 
				
				l_NodeVector[i][2].X= 0.1;  	l_NodeVector[i][2].Y= 0.2 ; 
				l_NodeVector[i][3].X= 0.5;  	l_NodeVector[i][3].Y= 0.5 ;
				l_NodeVector[i][4].X= 0.9;  	l_NodeVector[i][4].Y= 0.8 ;
				
				l_NodeVector[i][5].X= 0.95;  	l_NodeVector[i][5].Y= 0.9; 
				l_NodeVector[i][6].X= 1.0;  	l_NodeVector[i][6].Y= 1.0;


			break;	


			default:


				
				l_NodeVector[i][0].X= 0.0;		l_NodeVector[i][0].Y= 0.0; 
				l_NodeVector[i][1].X= 0.05; 	l_NodeVector[i][1].Y= 0.1; 
				
				l_NodeVector[i][2].X= 0.1;		l_NodeVector[i][2].Y= 0.2 ; 
				l_NodeVector[i][3].X= 0.5;		l_NodeVector[i][3].Y= 0.5 ;
				l_NodeVector[i][4].X= 0.9;		l_NodeVector[i][4].Y= 0.8 ;
				
				l_NodeVector[i][5].X= 0.95; 	l_NodeVector[i][5].Y= 0.9; 
				l_NodeVector[i][6].X= 1.0;		l_NodeVector[i][6].Y= 1.0;


			break;	



		}
	

	}

	for(i=0 ; i < OSC_NUM ; i++){
		
		Ramp[i].enable = 1;
		Ramp[i].y_max = 100.0;
		Ramp[i].y_min = -100.0;
	
		switch(i){

			case 0: 					
				
				MovePara[i].vC_pos	 = 2.0;
				MovePara[i].aAcc_pos = 20.0;
				MovePara[i].aDec_pos = MovePara[i].aAcc_pos;

				MovePara[i].vC_neg = MovePara[i].vC_pos;
				MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
				MovePara[i].aDec_neg = MovePara[i].aDec_pos;
				
				path[i].enable = 1;
				path[i].Init = 0;
				path[i].Brake = 0;
							
				path[i].pMove_Para = &MovePara[i];
				
				HCRSPF(&path[i]);
			
				/* linear spline characteristic */
				
				for(k = 0 ; k < CAM_NUM ; k++){
				
					fub_LinSpline[i][k].NbNodes= 7; /* number of nodes */
					fub_LinSpline[i][k].pNodeVec= &l_NodeVector[k][0]; /* node vector */
					fub_LinSpline[i][k].Invert= 0; /* no inverting */
					fub_LinSpline[i][k].Smooth= 1; /* smoothing */
					fub_LinSpline[i][k].Rness= 100.0; /* smoothing factor if smoothing gets switched on */
					fub_LinSpline[i][k].Update= 1; /* update all above values */
					fub_LinSpline[i][k].DisMode= 0; /* there's no discontinuity, but set it to a defined value */

					fub_LinSpline[i][k].enable= 1; /* enable instance */
					
					fub_LinSpline[i][k].x = 0.0;
					HCRLinSpline(&fub_LinSpline[i][k]);
				}

				//JogSpeedFact[i] = 0.005; bylo
				JogSpeedFact[i] = 0.025;
				
			break;	

		}
			
	}


}


void _CYCLIC OscProcess(void)
{int i;
float x;
	
	
		
	for(i=0 ; i < OSC_NUM ; i++){

		prc = OSC_TML_Param_Set[i].pPrc ;

		if(!prc)
			return;

		vect = OSC_TML_Param_Set[i].pVect;

		if(!vect)
			return;

		if(prc->ConfigReady <= 0)
			break;

		

		//////////  Test ////////////////////////

		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		
		//////////////////////////////////////

		
		if(OSC_TML_Param_Set[i].pTML){
			
			VirtCenter[i] = OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg +((OSC_TML_Param_Set[i].pTML->_cfg.sftLimitPos - OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg)/2);
			orgStarPos[i] =  OSC_TML_Param_Set[i].pTML->_int.startPos;
		
		}

	
		
		if(prc->Disable){

			prc->_step = 0xFF;

			switch(prc->xCMD){


				case prcsSTART:

					prc->Status = prcsSTATUS_Run;
					
				break;

				case prcsRESTART:
				case prcsHalt:
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

				/*
				case prcsHalt:

					
					if(prc->_step >= 2) 
						prc->_step = 30;

				break;
				*/

				case prcsSTART:

					if((prc->_step == 3)){

						StartPos[i] = 0;
						prc->_step = 10;

					}	

					
				break;


				case prcsJump2Run:

					
					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

					LCRRamp(&Ramp[i]);
					Ramp[i].set = 0;

					StartPos[i] = 0;
					
					vect->InVal = StartPos[i];  
					
					prc->_step = 12;
				

				break;	


				case prcsSTOP:

					if((prc->_step > 3) && (prc->_step <= 15)){// stop only in PT mode

						if(prc->Param[1] > 0) // skip stop delay
							prc->_step = 22;
						else
							prc->_step = 20;

						prc->Param[1] = 0;

					}

				break;

				case prcsJOGpos:

					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;

					/*
					if((prc->fParam[0] > 0) && (vect->FbkWarning) > 0))
						prc->fParam[0] = 0;

					else if((prc->fParam[0] < 0 && (vect->FbkWarning) < 0))
						prc->fParam[0] = 0;
					*/

					if(vect->FbkVect.Mode == vectSt_Mode_PT)
						StartPos[i] += prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)* JogSpeedFact[i];
					
					//StartPos[i] += prc->fParam[0] * JogSpeedFact[i];

					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}
		}
		
		prc->xCMD = 0;

		// Software Limits consideration

		if(vect->FbkVect.softLimits[0] > vect->FbkVect.softLimits[1]){	
			
			if((*OSC_TML_Param_Set[i].pPrimAmpl) > vect->FbkVect.softLimits[0] - vect->FbkVect.softLimits[1])
				(*OSC_TML_Param_Set[i].pPrimAmpl) = vect->FbkVect.softLimits[0] - vect->FbkVect.softLimits[1];
				
			if((StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->FbkVect.softLimits[0]){
				//StartPos[i] = vect->FbkVect.softLimits[0] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2;
				SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
			}

			else if((StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->FbkVect.softLimits[1]){
				//StartPos[i] = vect->FbkVect.softLimits[1] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2;
				SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
			}
		}
			
		///////////////////////////////////
		switch(prc->_step){


			case 0:  // IDLE - stopped

				prc->Status = prcsSTATUS_Stopped;				
				
				vect->InVal = 0.0;
				StartPos[i] = vect->FbkVect.x;

				if(vect->FbkVect.Mode != vectSt_Mode_Volt)									
					vect->Mode = vectSt_Mode_Volt;
				else
					prc->_step ++;	
										
														
			break;

			case 1:

				/*
				if(vect->FbkVect.Mode != vectSt_Mode_Idle)									
					vect->Mode = vectSt_Mode_Idle;
				
				else{										

					StartPos[i] = vect->FbkVect.x;
					vect->InVal = StartPos[i];	
					prc->_step++;
										
				}
				
				*/
				
				TimerStart(&Tmr[i], 1.0);
				prc->_step++;

				
				
			break;	


			case 2:
				
				prc->Status = prcsSTATUS_Stopped;

				if(vect->FbkVect.Mode != vectSt_Mode_Volt)									
					vect->Mode = vectSt_Mode_Volt;
								
				vect->InVal = 0.0;
				

				
				if(TimerIsTimeOut(&Tmr[i]))
				{
					StartPos[i] = vect->FbkVect.x;
					StartPos[i] = 0;
					prc->_step++;
				}
				else
					break;
				

			break;	
				

			case 3: 

				prc->Status = prcsSTATUS_Stopped;

				if(vect->FbkVect.Mode != vectSt_Mode_Volt)									
					vect->Mode = vectSt_Mode_Volt;
								
				vect->InVal = 0.0;
				

			break;	


			case 10: // start

				
				prc->Status = prcsSTATUS_StartDly;	
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}

				/*
				if(vect->FbkVect.Mode == vectSt_Mode_Volt){	
					
					vect->InVal = 0.0;
					break; 

				}

				*/
				
		
				if((*OSC_TML_Param_Set[i].pStartDly) > 0){
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
					prc->_step = 11;
				}
				else 					
					prc->_step = 12;

				
				
			break;


			case 11:

				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}
				
					
				if(TimerIsTimeOut(&Tmr[i]))
					prc->_step++;
				
					
				
					
			break;	
			

			case 12:

				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}				

				prc->Status = prcsSTATUS_RampUp;

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);
					

				Ramp[i].y_set 	= 1;
				Ramp[i].y 		= (*OSC_TML_Param_Set[i].pInitAmpl);
				LCRRamp(&Ramp[i]);
				Ramp[i].y_set 	= 0;

				if ((*OSC_TML_Param_Set[i].pRampUp) > 0) 
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
				else 
					Ramp[i].dy_up = 3.4e+38;
					
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				Ramp[i].set = 0;


				time[i] = 0;
				Invert[i] = 0;
				Dist[i] = 0;
				amplit[i] = Ramp[i].y;
				oldAmplit[i]= amplit[0];
				vect->InVal =  GenCam(i);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));

				prc->_step++;	

				
				
			break;	



			case 13:

				vect->InVal =  GenCam(i);

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i]))
					prc->_step = 15;

		

			break;



			case 15:

				prc->Status = prcsSTATUS_Run;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				
				if((*OSC_TML_Param_Set[i].pRampUpdate) > 0)
					Ramp[i].dy_up = (*OSC_TML_Param_Set[i].pPrimAmpl) / (*OSC_TML_Param_Set[i].pRampUpdate);
				else
					Ramp[i].dy_up = 3.4e+38;
				
				Ramp[i].dy_down = 	Ramp[i].dy_up;
				
				vect->InVal =  GenCam(i);

			break;



			case 20:

				prc->Status = prcsSTATUS_StopDly;
				
				if((*OSC_TML_Param_Set[i].pStopDly) > 0){
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStopDly));
					prc->_step = 21;
				}
				else					
					prc->_step = 22;

				
				vect->InVal =  GenCam(i);


			case 21:



				vect->InVal =  GenCam(i);

				if(TimerIsTimeOut(&Tmr[i]))
					prc->_step++;
				


			break;	

				

			case 22:


				prc->Status = prcsSTATUS_RampDwn;

				vect->InVal =  GenCam(i);

				if((*OSC_TML_Param_Set[i].pFinalAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pFinalAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0) 
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
				else 
					Ramp[i].dy_down = 3.4e+38;
					
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));
				
				prc->_step++;	

				

			break;

			case 23:

				
				vect->InVal =  GenCam(i);

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					path[i].Stop = 1;  // stop everything 
					path[i].s_home = vect->InVal ; // homing position 
					path[i].Set_home = 1;	
					prc->_step++; 
						
				}

			break;

			case 24:

				
				path[i].Init = 1;
				path[i].s_end = StartPos[i];
				prc->_step++;
				
				


			break;	


			case 25:

				vect->InVal = path[i].s_set;
				
				if(path[i].Reached){

					//if(fabsf(vect->InVal - vect->FbkVect.x)< 2.0){
						
						prc->_step = 3;
					//}

				}	

					
			break;

			case 30: // Quick Stop

				path[i].Stop = 1;  // stop everything 
				path[i].s_home = vect->InVal ; // homing position 
				path[i].Set_home = 1;	
				prc->_step = 24;


			break;	
		

			case 60: /// Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
					vect->InVal = 0.0;
					vect->Mode = vectSt_Mode_Volt;
				}	
				else{

					if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){	

						StartPos[i] = vect->FbkVect.x;
						
						if(prc->fParam[0] != 0)
							vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
						else {								
							prc->_step ++;

						}	
							
					}
					
						
				
				}				

			break;

			case 61: /// Direct Cntrl - Ramp Down

				if(vect->InVal > VoltRampDown)
					vect->InVal -= VoltRampDown;
				else if(vect->InVal < (VoltRampDown * (-1)))
					vect->InVal += VoltRampDown;
				else 
					prc->_step = 3;
		
			break;

			
		}

		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}
		LCRRamp(&Ramp[i]);		
	 	HCRSPF(&path[i]);


		/*
		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos
		prc->Fbk[1] =  VirtCenter[i] - (StartPos[i] + orgStarPos[i]);// center Position  - varOSC_Centr
		*/

		
		prc->Fbk[0] = vect->InVal;// act Position  vs center - varOSC_CurPos

		if((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_DirectCntrl)){
			
			prc->Fbk[1] =  VirtCenter[i] - vect->FbkVect.x;// center Position  - varOSC_Centr

		}
		else
			prc->Fbk[1] =  VirtCenter[i] - (StartPos[i] + orgStarPos[i]);// center Position  - varOSC_Centr
			

		prc->Fbk[2] = vect->FbkVect.x; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);

			prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
			prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);

		}

		if(OSC_TML_Param_Set[i].pFreq){

			x =(*OSC_TML_Param_Set[i].pFreq);
			
			//x +=(*OSC_TML_Param_Set[i].pFreq);
			
			if(x > 0)
				prc->Fbk[5] = x ; //  varOSC_ExcurTm_Cmd
			else
				prc->Fbk[5] = 0;

		}
		else
			prc->Fbk[5] = 0;


		
		if(prc->Status == prcsSTATUS_Stopped){
				prc->Fbk[0] = 0.0;
				prc->Fbk[3] = (*OSC_TML_Param_Set[i].pPrimAmpl);
				prc->Fbk[4]= (*OSC_TML_Param_Set[i].pFreq);
				prc->Fbk[5]= (*OSC_TML_Param_Set[i].pFreq);

		}

		
	}

	
}

float GenCam(UINT k){
float d;
HCRLinSpline_typ *cam;

	if((*OSC_TML_Param_Set[k].pCamType)>= CAM_NUM)
		(*OSC_TML_Param_Set[k].pCamType)= CAM_NUM-1;
	
	else if((*OSC_TML_Param_Set[k].pCamType)<0)
		(*OSC_TML_Param_Set[k].pCamType)= 0;
	
		
	cam = &fub_LinSpline[k][(*OSC_TML_Param_Set[k].pCamType)];
	
	if((*OSC_TML_Param_Set[k].pFreq) < 0)
		(*OSC_TML_Param_Set[k].pFreq) = 1.0;

	if(time[k] <= 1)
		time[k] += (ClassTime / (*OSC_TML_Param_Set[k].pFreq)); // time[k] += (ClassTime * (*OSC_TML_Param_Set[k].pFreq) * 4);
	else
		time[k] += ClassTime;
	

	cam->x = time[k];	
	HCRLinSpline(cam);
	
	 if(Invert[k])
		 Pos[k] = Dist[k] - cam->y * amplit[k];
	else
		 Pos[k] = Dist[k] + cam->y * amplit[k]; 


	if(time[k] >= 1.0 ){


		prc->subStatus = 1;	
		

		if((Invert[k] && (time[k] >= (1.0 + (*OSC_TML_Param_Set[k].pDwell_In))))
			|| (!Invert[k] && (time[k] >= (1.0 + (*OSC_TML_Param_Set[k].pDwell_Out))))){
			
			Invert[k] = !Invert[k];
			
			time[k] = 0.0;

			Dist[k] = Pos[k];
				
			amplit[k] = Ramp[k].y / 2; //
			d = amplit[k];		
			amplit[k] = oldAmplit[k] + amplit[k];
			oldAmplit[k] = d; 

		}

		
	}	
	else
		prc->subStatus = 0;

	
		
return  Pos[k] + StartPos[k];

}
