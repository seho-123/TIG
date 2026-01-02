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


/////////

_LOCAL AzzRamp_typ Ramp[OSC_NUM];
_LOCAL AzzRamp_typ IndxRamp[OSC_NUM];
_LOCAL AzzRamp_typ CompRamp[OSC_NUM];


_LOCAL TON_typ Tmr[OSC_NUM];
 _LOCAL GenProcess_typ* prc;
 _LOCAL StateVectorProxy_typ* vect;

_LOCAL AzzNode_typ l_NodeVector[CAM_NUM][11];
_LOCAL AzzLinSpline_typ fub_LinSpline[OSC_NUM][CAM_NUM];
_LOCAL float time[OSC_NUM], amplit[OSC_NUM] , oldAmplit[OSC_NUM];

/*
_LOCAL float  realTime[OSC_NUM], oscTime[OSC_NUM] , dwellTime[OSC_NUM] , TimeOffset;
_LOCAL UINT KK_SynchClckFromOsc;
*/
_LOCAL float Dist[OSC_NUM] , Pos[OSC_NUM] , StartPos[OSC_NUM] , TimeStrength[OSC_NUM];
_LOCAL BOOL Invert[OSC_NUM];


//_LOCAL HCRSPF_typ path[OSC_NUM];
//_LOCAL HCRMovePara_typ  MovePara[OSC_NUM];


_LOCAL HCRSPFazz_type path[OSC_NUM];
_LOCAL HCRMoveParamAzz_type  MovePara[OSC_NUM];


_LOCAL float JogSpeedFact[OSC_NUM] , JogStepFact[OSC_NUM] , JogStepFact2[OSC_NUM];
_LOCAL float VirtCenter[OSC_NUM] , orgStarPos[OSC_NUM] , orgStarPosEnc[OSC_NUM];
_LOCAL float DirectCntrlVolt;
_LOCAL float VoltRampDown;
_LOCAL DINT  OSC_Mode[OSC_NUM];
_LOCAL DINT   OSC_Period[OSC_NUM];
_LOCAL float OSC_TVL_Dist[OSC_NUM] , OSC_IndxDist[OSC_NUM];
_LOCAL float KK_Ampl[OSC_NUM] , KK_Excur[OSC_NUM] , KK_DwellIn[OSC_NUM] , KK_DwellOut[OSC_NUM] , KK_StartPos[OSC_NUM];
_LOCAL float OSC_Max[OSC_NUM] , OSC_Min[OSC_NUM] ;


_LOCAL float _StartPos[OSC_NUM] , StartPosbyPOT[OSC_NUM];
plcbit  _JumpToRun[OSC_NUM];

_LOCAL UINT updateLimits[OSC_NUM];

///////
_LOCAL float CompOffset[OSC_NUM] , OffsetPlus_Sum[OSC_NUM] , OffsetPlus_Avr[OSC_NUM], OffsetMinus_Sum[OSC_NUM] , OffsetMinus_Avr[OSC_NUM];

_LOCAL UINT CompOffset_Step[OSC_NUM] , OffsetPlus_Indx[OSC_NUM] , OffsetMinus_Indx[OSC_NUM] ;
_LOCAL UINT UP_Limit[OSC_NUM] , DOWN_Limit[OSC_NUM] ;
_LOCAL float CloseLoopTime , CloseLoopTMR[OSC_NUM] , CloseLoopDelay;
_LOCAL float ErrorMargen;
///////////	
#define DwellIn_par  		0
#define DwellOut_par 		1 
#define PosMin_par 		2
#define PosMax_par 		3
#define OSC_speed_par 	0

_LOCAL plcbit KK_Test;
_LOCAL float KK_margin;
_LOCAL float KK_delta;

_LOCAL float KK_OSC_JOG_Speed;

void StartOSC( StateVectorProxy_typ*  vect , int i , float amplit);
void StopOSC( StateVectorProxy_typ*  vect , int i);
void UpdateOSC( StateVectorProxy_typ*  vect , int i , float amplit);
void ObserveOsc(StateVectorProxy_typ*  vect , int i);
void StartAlligment( StateVectorProxy_typ*  vect , int i , float amplit);

float GenCam(UINT k);
void OscProcess_BEMF(void);
void OscProcess_TML_ENC(void);
void OscProcess_TML_ENC_POT(void);
void OscProcess_TML_NewConcept(void);


void OscProcess_STND();
void OscProcess_BEMF_new(void);


void CorrectStartPos(int i);
void CorrectAmplitude(int i);

void CorrectStartPosbyPOT(int i);
void CorrectAmplitudebyPOT(int i);

void MoveStartPosbyPot(int i ,float delta);
void MoveStartPosbyPot_Abs(int i ,float delta);

void CalculateCompOffset(int i);





_LOCAL plcbit KK_test , KK_Compensate;

void _INIT OscProcess_Init(void)
{int i , k;

	//TimeOffset = 0.0;


	 VoltRampDown = 0.5; 
	 KK_margin = 1.0;
			
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;
	Process[0].Disable = Process[0].Disable;

	DirectCntrlVolt = 12.0;

	ErrorMargen = 0.25;
	CloseLoopTime = 1.0;
	CloseLoopDelay = 1.0;
	KK_Compensate = 1;
	


	for(i = 0 ; i < CAM_NUM ; i++){

		switch(i){


			case 1: // Harmonic

				l_NodeVector[i][0].X= 0.0;  	l_NodeVector[i][0].Y= 0.0; 
		
				l_NodeVector[i][1].X= 0.1;  	l_NodeVector[i][1].Y= 0.1; 				
				l_NodeVector[i][2].X= 0.2;  	l_NodeVector[i][2].Y= 0.2; 
				
				l_NodeVector[i][3].X= 0.3;  	l_NodeVector[i][3].Y= 0.3;				
				l_NodeVector[i][4].X= 0.4;  	l_NodeVector[i][4].Y= 0.4;
				
				l_NodeVector[i][5].X= 0.5;  	l_NodeVector[i][5].Y= 0.5;
				
				l_NodeVector[i][6].X= 0.6;  	l_NodeVector[i][6].Y= 0.6;
				l_NodeVector[i][7].X= 0.7;  	l_NodeVector[i][7].Y= 0.7; 

				l_NodeVector[i][8].X= 0.8;  	l_NodeVector[i][8].Y= 0.8;
				l_NodeVector[i][9].X= 0.9;  	l_NodeVector[i][9].Y= 0.9; 

				l_NodeVector[i][10].X= 1.0;  	l_NodeVector[i][10].Y= 1.0;  				
				

			

			break;	

			case 2: // Trapezoid

				l_NodeVector[i][0].X= 0.0;  	l_NodeVector[i][0].Y= 0.0; 
		
				l_NodeVector[i][1].X= 0.1;  	l_NodeVector[i][1].Y= 0.1; 				
				l_NodeVector[i][2].X= 0.2;  	l_NodeVector[i][2].Y= 0.2; 
				
				l_NodeVector[i][3].X= 0.3;  	l_NodeVector[i][3].Y= 0.3;				
				l_NodeVector[i][4].X= 0.4;  	l_NodeVector[i][4].Y= 0.4;
				
				l_NodeVector[i][5].X= 0.5;  	l_NodeVector[i][5].Y= 0.5;
				
				l_NodeVector[i][6].X= 0.6;  	l_NodeVector[i][6].Y= 0.6;
				l_NodeVector[i][7].X= 0.7;  	l_NodeVector[i][7].Y= 0.7; 

				l_NodeVector[i][8].X= 0.8;  	l_NodeVector[i][8].Y= 0.8;
				l_NodeVector[i][9].X= 0.9;  	l_NodeVector[i][9].Y= 0.9; 

				l_NodeVector[i][10].X= 1.0;  	l_NodeVector[i][10].Y= 1.0;  

			break;	


			default: // Agresive
			

				l_NodeVector[i][0].X= 0.0;  	l_NodeVector[i][0].Y= 0.0; 
		
				l_NodeVector[i][1].X= 0.1;  	l_NodeVector[i][1].Y= 0.025; 				
				l_NodeVector[i][2].X= 0.2;  	l_NodeVector[i][2].Y= 0.1125; 
				
				l_NodeVector[i][3].X= 0.3;  	l_NodeVector[i][3].Y= 0.25;				
				l_NodeVector[i][4].X= 0.4;  	l_NodeVector[i][4].Y= 0.4;
				
				l_NodeVector[i][5].X= 0.5;  	l_NodeVector[i][5].Y= 0.55;
				
				l_NodeVector[i][6].X= 0.6;  	l_NodeVector[i][6].Y= 0.7;
				l_NodeVector[i][7].X= 0.7;  	l_NodeVector[i][7].Y= 0.85; 

				l_NodeVector[i][8].X= 0.8;  	l_NodeVector[i][8].Y= 0.925;
				l_NodeVector[i][9].X= 0.9;  	l_NodeVector[i][9].Y= 0.975; 

				l_NodeVector[i][10].X= 1.0;  	l_NodeVector[i][10].Y= 1.0; 	
				
				

			break;	


		}
	

	}

	for(i=0 ; i < OSC_NUM ; i++){

		_JumpToRun[i] =0;
		
		Ramp[i].enable = 1;
		Ramp[i].y_max = 100.0;
		Ramp[i].y_min = -100.0;


		IndxRamp[i].enable = 1;
		IndxRamp[i].y_max = 3.4e+38;
		IndxRamp[i].y_min  = -IndxRamp[i].y_max;

		IndxRamp[i].dy_up = 8.0;
		IndxRamp[i].dy_down = IndxRamp[i].dy_up;

		CompRamp[i].enable = 1;
		CompRamp[i].y_max = 3.4e+38;
		CompRamp[i].y_min  = -CompRamp[i].y_max;

		CompRamp[i].dy_up = 20.0;
		CompRamp[i].dy_down = CompRamp[i].dy_up;
	
		switch(i){

			case 0: 					
				
				MovePara[i].vC_pos	 = 7.0;
				MovePara[i].aAcc_pos = 50.0;
				MovePara[i].aDec_pos = MovePara[i].aAcc_pos;

				MovePara[i].vC_neg = MovePara[i].vC_pos;
				MovePara[i].aAcc_neg = MovePara[i].aAcc_pos;
				MovePara[i].aDec_neg = MovePara[i].aDec_pos;
				
				path[i].enable = 1;
				path[i].Init = 0;
				path[i].Brake = 0;
							
				path[i].pMove_Para = &MovePara[i];

				//path[i].Intr.TaskPeriod = 0.0024;
				
				HCRSPFazz(&path[i]);

							
				/* linear spline characteristic */
				
				for(k = 0 ; k < CAM_NUM ; k++){
				
					fub_LinSpline[i][k].NbNodes= 11; /* number of nodes */
					fub_LinSpline[i][k].pNodeVec= &l_NodeVector[k][0]; /* node vector */
					fub_LinSpline[i][k].Invert= 0; /* no inverting */
					fub_LinSpline[i][k].Smooth= 1; /* smoothing */
					fub_LinSpline[i][k].Rness= 100.0; /* smoothing factor if smoothing gets switched on */
					fub_LinSpline[i][k].Update= 1; /* update all above values */
					fub_LinSpline[i][k].DisMode= 0; /* there's no discontinuity, but set it to a defined value */

					fub_LinSpline[i][k].enable= 1; /* enable instance */
					
					fub_LinSpline[i][k].x = 0.0;
					AzzLinSpline(&fub_LinSpline[i][k]);
				}

				//JogSpeedFact[i] = 0.005; bylo
				JogSpeedFact[i] = 0.025;
				JogStepFact[i] = 0.01;
				JogStepFact2[i] = 0.012;
				
			break;	

		}
			
	}


}
void _CYCLIC OscProcess(void)
{

	if(!OSC_TML_Param_Set[0].pPrc)
		return;


	if(OSC_TML_Param_Set[0].pPrc->ConfigReady <= 0)
		return;

	if(OSC_TML_Param_Set[0].pJogSpeed)
		KK_OSC_JOG_Speed = (*OSC_TML_Param_Set[0].pJogSpeed);

	switch(OSC_TML_Param_Set[0].Mode){


		case 1:

			OscProcess_TML_ENC();

		break;

		case 2:

			OscProcess_BEMF_new();

		break;

		case 3:

			OscProcess_TML_ENC_POT();

		break;	

		case 10:

			OscProcess_TML_NewConcept();

		break;	

		
		case 11:

			// Nothing new but synch pulse based on ax->Version calcultaed in AVC_SYNCH task
			OscProcess_BEMF();

		break;


		case 12: // Nowy MIG Osc

			//KK_Compensate = 0;
			//OscProcess_TML_ENC_POT();

			OscProcess_TML_ENC();

		break;


		case 13:


		break;


		default:

			//OscProcess_STND();
			OscProcess_BEMF();
			
		break;



	}


}
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void OscProcess_TML_NewConcept(void)
{
int i;
float x, delta , t;
UINT status;
	
	
		
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
		/*
		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_SteerSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeed); 
		KK_JogSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl); 
		*/
		//////////////////////////////////////

				/////////////////////////////////////////
		if(OSC_TML_Param_Set[i].pTML){
			
			//VirtCenter[i] = OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg +((OSC_TML_Param_Set[i].pTML->_cfg.sftLimitPos - OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg)/2);
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			orgStarPos[i] =  OSC_TML_Param_Set[i].pTML->_int.startPos;
		
		}

#if 0
		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if(prc->_step >= 12){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;

							}

							
						
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	IndxRamp[i].x;
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}

						}					

						if(OSC_TML_Param_Set[i].pIndxVel){
							IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							_StartPos[i] += delta;

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	IndxRamp[i].x;
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}
							KK_delta = delta;	

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							 if(t > 0){
								
								IndxRamp[i].dy_up = fabsf(delta/t * (1000.0/ 2.4));

							 	if(IndxRamp[i].dy_up > (*OSC_TML_Param_Set[i].pIndxVel))
									IndxRamp[i].dy_up  = (*OSC_TML_Param_Set[i].pIndxVel);
								
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
								
							 }
							 else {
				

								//IndxRamp[i].dy_up = 3.4e+38;
								IndxRamp[i].dy_up = 8.0;
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;

							}
							
							/*
							else if(OSC_TML_Param_Set[i].pIndxVel){
								
								IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							}
							*/

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;


	#endif	
			
	
		
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

				case prcsAlligment:
				

				break;

				case prcsJump2Run:


					if(prc->_step > 2)
						prc->_step = 10;	

				break;
				
				case prcsSTART:

					if(prc->_step > 2)
						prc->_step = 5;	
									
				break;

				
				
				case prcsSTOP:

					if(prc->_step > 2)
						prc->_step = 20;									

					

				break;

				case prcsQuickSTOP:
				
					if(prc->_step > 2)
						prc->_step = 24;										

					

				break;				


				case prcsSTEPpos:		

					
					if((prc->fParam[0] != 0.0) && (prc->_step == 15)){		


						vect->fParam[V_fParam_Steer] = prc->fParam[0];
						
						//prc->fParam[0] = 0;
												
						prc->Status = prcsSTATUS_Step;
						prc->_step = 50;
					}
					
					
				break;						
					
				case prcsJOGpos:


					if(prc->fParam[0] != 0.0){

						if(prc->_step == 15){	

							vect->fParam[V_fParam_Steer] = prc->fParam[0] * 5.0;
							
							//prc->fParam[0] = 0;
													
							prc->Status = prcsSTATUS_Step;
							prc->_step = 50;
						}
						else if(prc->_step == 3){	

							vect->fParam[V_fParam_Steer] = prc->fParam[0] * 5.0;
							vect->cmdWord = 2; // Steps
						
							//prc->fParam[0] = 0;

							
													
							prc->Status = prcsSTATUS_Step;
							prc->_step = 55;
						}

					}
					
					
				break;


				case prcsSetHomePos:

					if(prc->_step == 3){

						vect->fParam[V_fParam_HomePos] = prc->fParam[3];	
						prc->_step = 100;

					}
						

				break;


				case prcsDirectCntrl:					
							
					if(prc->_step != 60 && (prc->fParam[0] != 0))
						prc->_step = 60;
					

				break;

				

				
			}

		}
		
	
		prc->xCMD = 0;
		
		switch(prc->_step){

			

			case 0:  // IDLE - stopped
			
				prc->Status = prcsSTATUS_Init;	

				if(vect->FbkVect.Mode != vectSt_Mode_Drive_Independant){

					vect->Mode = vectSt_Mode_Drive_Independant;

					prc->Status = prcsSTATUS_Init;	
				}
				else{			

					
					prc->_step++;

				}
							
														
			break;


			case 1:

				if(vect->xCmd_busy || vect->xCMD)
					break;


				vect->fParam[V_fParam_Amplit] =  (*OSC_TML_Param_Set[i].pPrimAmpl);
				
				//vect->fParam[V_fParam_Max] = vect->OutVect.softLimits[1];
				//vect->fParam[V_fParam_Min] = vect->OutVect.softLimits[0];

				vect->fParam[V_fParam_Max] = vect->FbkVect.x + 20.0;
				vect->fParam[V_fParam_Min] = vect->FbkVect.x - 20.0;				

				vect->fParam[V_fParam_Jog] = 0.0;
				vect->fParam[V_fParam_Steer] = 0.0;

				vect->Param[V_Param_Time] = (DINT)( (*OSC_TML_Param_Set[i].pFreq) * 1000.0);
				vect->Param[V_Param_DwellIn] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_In) * 1000.0);
				vect->Param[V_Param_DwellOut] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_Out) * 1000.0);

				KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
				KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
				KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
				KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
				OSC_Max[i] = vect->fParam[V_fParam_Max];
				OSC_Min[i] = vect->fParam[V_fParam_Min];

				vect->cmdWord = 0;
				
				vect->xCMD = VECT_Set_OSC_AllParam;
				
					
				prc->_step++;

				
				

			break;


			case 2:

				
				if(vect->xCmd_busy || vect->xCMD)
					break;

				vect->cmdWord = 0.0;
				vect->xCMD = VECT_Set_CmdWord;
				
				prc->_step++;

			break;


			case 3: //Ready

				prc->Status = prcsSTATUS_Stopped;


				
				if((KK_Ampl[i] != (*OSC_TML_Param_Set[i].pPrimAmpl))
					//||(OSC_Max[i] != vect->fParam[V_fParam_Max])
					//||(OSC_Min[i] != vect->fParam[V_fParam_Min])
					){

					KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
					KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
					KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
					KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
					//OSC_Max[i] = vect->fParam[V_fParam_Max];
					//OSC_Min[i] = vect->fParam[V_fParam_Min];

					prc->_step = 1;

				}	
			


			break;

			

			case 5:
								
				prc->Status = prcsSTATUS_StartDly;	

				if(vect->xCmd_busy || vect->xCMD)
					break;	

							
				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;

				vect->fParam[V_fParam_Amplit] = (*OSC_TML_Param_Set[i].pInitAmpl)/2.0;

				
				//vect->fParam[V_fParam_Max] = vect->OutVect.softLimits[1];
				//vect->fParam[V_fParam_Min] = vect->OutVect.softLimits[0];

				vect->fParam[V_fParam_Max] = vect->FbkVect.x + 20.0;
				vect->fParam[V_fParam_Min] = vect->FbkVect.x - 20.0;

				vect->fParam[V_fParam_Jog] = 0.0;
				vect->fParam[V_fParam_Steer] = 0.0;

				vect->Param[V_Param_Time] = (DINT)( (*OSC_TML_Param_Set[i].pFreq) * 1000.0);
				vect->Param[V_Param_DwellIn] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_In) * 1000.0);
				vect->Param[V_Param_DwellOut] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_Out) * 1000.0);

				vect->cmdWord = 0;
				
				vect->xCMD = VECT_Set_OSC_AllParam;
				prc->_step++;
					
				
				
			break;	
			

			case 6:
				

				//////////////////////////////////////////////

				if(_JumpToRun[i] || ((*OSC_TML_Param_Set[i].pStartDly) < 0.005))				
					TimerStart(&Tmr[i], 0.005);
				else
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
				
				prc->_step++;
				
			break;
					

			case 7:		


				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				

				prc->Status = prcsSTATUS_RampUp;

				
				if(vect->xCmd_busy || vect->xCMD)
					break;				

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) {// not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));
				}	
				else {					
					
					TimerStart(&Tmr[i], 0.01);

				}

				prc->Param[0] = 0;

				// It is important to make first 
				if(Ramp[i].dy_up > 100.0)
					Ramp[i].dy_up = 100.0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				Ramp[i].set = 0;

				AzzRamp(&Ramp[i]);
				

				vect->cmdWord = 1;
					
				vect->xCMD = VECT_Set_CmdWord;
				
				prc->_step++;
				
				
				
			break;	



			case 8:

							
				if(vect->xCmd_busy || vect->xCMD)
					break;					

				vect->fParam[V_fParam_Amplit] = Ramp[i].y/2.0;
				vect->xCMD = VECT_Set_OSC_Amplit;

				//if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i]))
				if(Ramp[i].x_reached)	
					prc->_step = 14;
					
							

			break;



			case 10:
				

				if(vect->xCmd_busy || vect->xCMD)
					break;

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;

				vect->fParam[V_fParam_Amplit] = (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0;

				vect->fParam[V_fParam_Max] = vect->FbkVect.x + 100.0;
				vect->fParam[V_fParam_Min] = vect->FbkVect.x - 100.0;

				vect->fParam[V_fParam_Jog] = 0.0;
				vect->fParam[V_fParam_Steer] = 0.0;

				vect->Param[V_Param_Time] = (DINT)( (*OSC_TML_Param_Set[i].pFreq) * 1000.0);
				vect->Param[V_Param_DwellIn] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_In) * 1000.0);
				vect->Param[V_Param_DwellOut] = (DINT)( (*OSC_TML_Param_Set[i].pDwell_Out) * 1000.0);

				vect->cmdWord = 1;


				vect->xCMD = VECT_Set_OSC_AllParam;
				prc->_step++;
	
			break;


			case 11:

				

				if(vect->xCmd_busy || vect->xCMD)
					break;

				prc->Status = prcsSTATUS_Run;
				
				KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
				KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
				KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
				KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 

		
				vect->cmdWord = 1;
				vect->xCMD = VECT_Set_CmdWord;
			
				prc->_step = 15;
				

			break;


			
			case 14:

				prc->Status = prcsSTATUS_Run;

				KK_Ampl[i] = 0.0; 
				KK_Excur[i] = 0.0;
				KK_DwellIn[i] = 0.0; 
				KK_DwellOut[i] = 0.0; 

				prc->_step++;
	
			break;


			case 15:

				prc->Status = prcsSTATUS_Run;

				if(vect->xCmd_busy || vect->xCMD)
					break;	
					
				if((KK_Ampl[i] != (*OSC_TML_Param_Set[i].pPrimAmpl)) 
					||(KK_Excur[i] != (*OSC_TML_Param_Set[i].pFreq))
					||(KK_DwellIn[i] != (*OSC_TML_Param_Set[i].pDwell_In))
					||(KK_DwellOut[i] != (*OSC_TML_Param_Set[i].pDwell_Out))){


						KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
						KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
						KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
						KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 


						vect->fParam[V_fParam_Amplit] = KK_Ampl[i] /2.0;

						vect->Param[V_Param_Time] = (DINT)(KK_Excur[i] * 1000.0);
						vect->Param[V_Param_DwellIn] = (DINT)(KK_DwellIn[i] * 1000.0);
						vect->Param[V_Param_DwellOut] = (DINT)(KK_DwellOut[i] * 1000.0);

						vect->xCMD = VECT_Set_All_Run_Param;			
						
	
				}

			break;

			case 20:

				prc->Status = prcsSTATUS_StopDly;

				if((*OSC_TML_Param_Set[i].pStopDly) > 0){
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStopDly));
					prc->_step = 21;
				}
				else					
					prc->_step = 22;				
				

			break;


			case 21:				

				if(TimerIsTimeOut(&Tmr[i]) )
					prc->_step++;
				


			break;					

			case 22:


				prc->Status = prcsSTATUS_RampDwn;				

				if((*OSC_TML_Param_Set[i].pFinalAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pFinalAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) {// not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));

				}	
				else {
					Ramp[i].dy_down = 3.4e+38;
					TimerStart(&Tmr[i], 0.01);

				}
				
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				AzzRamp(&Ramp[i]);
								
				prc->_step++;	

				

			break;

			case 23:

				if(vect->xCmd_busy || vect->xCMD)
					break;					

				vect->fParam[V_fParam_Amplit] = Ramp[i].y/2.0;
				vect->xCMD = VECT_Set_OSC_Amplit;

				//if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i]))
				if(Ramp[i].x_reached)	
					prc->_step++;
					
								

			break;

			case 24:

				if(vect->xCmd_busy || vect->xCMD)
					break;	

				vect->fParam[V_fParam_Amplit] = 0.0;
				vect->xCMD = VECT_Set_OSC_Amplit;
				
				prc->_step++;

			break;	

			
			case 25:	


				if(vect->xCmd_busy || vect->xCMD)
					break;


				vect->cmdWord = 0;
				vect->xCMD = VECT_Set_CmdWord;
				prc->_step = 3;
				

			break;		



			case 50: // Steer

				if(vect->xCmd_busy || vect->xCMD)
					break;

				prc->Status = prcsSTATUS_Run;
				
				vect->xCMD = VECT_Set_OSC_Steer;
				prc->_step = 15;
				
			break;


			case 55: // Steer by Stepping

				if(vect->xCmd_busy || vect->xCMD)
					break;

							
				vect->xCMD = VECT_Set_OSC_Steer;
				prc->_step ++;
				
			break;

			
			case 56: // Steer by Stepping
			
				if(vect->xCmd_busy || vect->xCMD)
					break;

					
				
				vect->xCMD = VECT_Set_CmdWord;
				prc->_step = 3;
				
			break;

			
			case 60: /// Direct Cntrl		

				prc->Status = prcsSTATUS_DirectCntrl;
				
				
				if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){	
					
					if(prc->fParam[0] != 0){
						
						IndxRamp[i].y_set =_StartPos[i];
						IndxRamp[i].set = 1;
						AzzRamp(&IndxRamp[i]);
						IndxRamp[i].set = 0;

						if(vect->FbkVect.Status != 10){
							
							vect->cmdWord = 0;
							vect->xCMD = VECT_Set_CmdWord; 
						
						}else{

							
							vect->fParam[V_fParam_Jog] = prc->fParam[0];						
							vect->xCMD =  VECT_Set_OSC_Jog_Volt;
						}
						 
					}
					else { 		

						vect->fParam[V_fParam_Jog] = 0.0;						
						vect->xCMD =  VECT_Set_OSC_Jog_Volt;
						prc->_step  = 3;

					}
											
						
				}						

			break;		



			case 100:

				if(vect->xCmd_busy || vect->xCMD)
					break;	

									
				vect->xCMD =  VECT_Set_OSC_Home;

				prc->_step  = 3;


			break;

			
		}

		////////////////////////////

		if(IndxRamp[i].dy_up <= 0.0){

			IndxRamp[i].dy_up = 3.4e+38;
			IndxRamp[i].dy_down = IndxRamp[i].dy_up;

		}
		IndxRamp[i].x = _StartPos[i];
		AzzRamp(&IndxRamp[i]);

		if(IndxRamp[i].status == 0)
			StartPos[i] = IndxRamp[i].y;
		else
			StartPos[i] = _StartPos[i];
				

		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}

		//////////   ////////////////////////

		prc->Fbk[0] = vect->FbkVect.x  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = vect->FbkVect.Center;// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->fParam[V_fParam_Amplit] * 2.0; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = vect->fParam[V_fParam_Amplit] * 2.0; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = (float)vect->Param[V_Param_Time]/1000.0; //varOSC_curExcurTm

	
		/*
		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = (StartPos[i] + orgStarPos[i]) - VirtCenter[i];// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		*/

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);

			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void OscProcess_TML_ENC(void)
{
int i;
float x, delta , t;
UINT status;
	
	
		
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
		/*
		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_SteerSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeed); 
		KK_JogSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl); 
		*/
		//////////////////////////////////////

				/////////////////////////////////////////
		if(OSC_TML_Param_Set[i].pTML){
			
			//VirtCenter[i] = OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg +((OSC_TML_Param_Set[i].pTML->_cfg.sftLimitPos - OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg)/2);
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			orgStarPos[i] =  OSC_TML_Param_Set[i].pTML->_int.startPos;
		
		}


		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if(prc->_step >= 12){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;

							}

							
						
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	IndxRamp[i].x;
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}

						}					

						if(OSC_TML_Param_Set[i].pIndxVel){
							IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							_StartPos[i] += delta;

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	IndxRamp[i].x;
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}
							KK_delta = delta;	

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							 if(t > 0){
								
								IndxRamp[i].dy_up = fabsf(delta/t * (1000.0/ 2.4));

							 	if(IndxRamp[i].dy_up > (*OSC_TML_Param_Set[i].pIndxVel))
									IndxRamp[i].dy_up  = (*OSC_TML_Param_Set[i].pIndxVel);
								
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
								
							 }
							 else {
				

								//IndxRamp[i].dy_up = 3.4e+38;
								IndxRamp[i].dy_up = 8.0;
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;

							}
							
							/*
							else if(OSC_TML_Param_Set[i].pIndxVel){
								
								IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							}
							*/

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;
			
	
		
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

				case prcsAlligment:

					
					if(prc->_step == 3){ 

						CorrectStartPos(i);
						prc->_step = 5;
					}	

				break;

				case prcsJump2Run:
				case prcsSTART:

					_JumpToRun[i] = ((prc->xCMD == prcsJump2Run))? 1:0;		
					//_JumpToRun[i] = 0;	

					prc->Param[0] = 0; // Not bypass Ramp ?? This can be discussed

					if(prc->Param[3] > 0 )						
						CorrectAmplitude(i);
					else  
						CorrectStartPos(i);

					if((prc->_step == 3)){
						
						path[i].Stop = 1;  // stop everything 
						path[i].s_home = vect->InVal ; // homing position 
						path[i].Set_home = 1;	

						HCRSPFazz(&path[i]);

						path[i].Init = 1;
						path[i].s_end = StartPos[i];

						HCRSPFazz(&path[i]);

						prc->_step = 10;
					}

									
				break;

				
				
				case prcsSTOP:

					if((prc->_step > 3) && (prc->_step <= 15)){				

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 23)){
						
						prc->_step = 22;										

					}

				break;				


				case prcsSTEPpos:		

					
					if((prc->fParam[1] != 0.0) && (prc->_step == 3)){

						prc->Status = prcsSTATUS_Step;
						prc->_step = 4;
					}
					
					
				break;		

				case prcsSTEPneg:	// Make step imediately	

					
					if((prc->fParam[1] != 0.0)){

						if((*OSC_TML_Param_Set[i].pStopDly) > 0)
							TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStopDly));
						else
							TimerStart(&Tmr[i],0.1);

						prc->Status = prcsSTATUS_StopDly;
						prc->_step = 4;
					}
					
					
				break;	
					
				case prcsJOGpos:

					
					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;
					

					if(vect->FbkVect.Mode == vectSt_Mode_PT){
						
						//StartPos[i] += prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)* JogStepFact[i];
						//_StartPos[i] =  StartPos[i] + (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed) ) ;
						_StartPos[i] =  IndxRamp[i].x + (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed) ) ;
						

						if((prc->fParam[0] ) > 0){
						
							if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								_StartPos[i] =  StartPos[i];

							}	
							
							
						}
					
						else if((prc->fParam[0] ) < 0){

							if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								_StartPos[i] =  StartPos[i];
							}
							
						}

						//CorrectStartPos(i);

						if(IndxRamp[i].x_reached){
							IndxRamp[i].dy_up = 3.4e+38;
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}
												
					}
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}

		}
		
		

		/////////////////////////////////////////////////////////
		
		prc->xCMD = 0;
		
		switch(prc->_step){

			

			case 0:  // IDLE - stopped
			
				prc->Status = prcsSTATUS_Init;	


				vect->Mode = vectSt_Mode_PT;
				vect->xCMD = VECT_Refresh_Mode;

				TimerStart(&Tmr[i], 0.1);

				prc->_step++;


				/*

				if(vect->FbkVect.Mode != vectSt_Mode_Idle){

					vect->Mode = vectSt_Mode_Idle;
				}
				else{
					TimerStart(&Tmr[i], 4.0);
					//vect->InVal = vect->FbkVect.x;
					//vect->Mode = vectSt_Mode_PT;
							
					prc->_step++;		

				}
				*/
				
														
			break;

			case 1:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				
				vect->InVal = vect->FbkVect.x;		
				
				if(vect->FbkVect.Mode == vectSt_Mode_PT){	
						
					_StartPos[i] = vect->InVal;
									
					IndxRamp[i].y_set =_StartPos[i];
					IndxRamp[i].set = 1;
					AzzRamp(&IndxRamp[i]);
					IndxRamp[i].set = 0;
					
					vect->InVal = IndxRamp[i].y;
					prc->_step = 3;
				}
				else
					vect->Mode = vectSt_Mode_PT;
				
			break;	


			case 2:


			break;
			
			case 3: 

				prc->Status = prcsSTATUS_Stopped;	
				//CorrectAmplitude(i);
				vect->InVal = StartPos[i];
			
			break;	


			case 4: 

				vect->InVal =  GenCam(i);

				if(!TimerIsTimeOut(&Tmr[i]))
					break;

				if(vect->FbkVect.Mode == vectSt_Mode_PT){

					prc->Status = prcsSTATUS_Step;
					
					MovePara[i].vC_pos	 = (*OSC_TML_Param_Set[i].pIndxVel) ;//7.0
					MovePara[i].vC_neg	= MovePara[i].vC_pos;
					MovePara[i].aAcc_pos = 50.0;
					MovePara[i].aDec_pos = MovePara[i].aAcc_pos;
					
					path[i].s_home = StartPos[i];
					path[i].s_end = StartPos[i] + prc->fParam[1];
					

					if((prc->fParam[0] ) > 0){
					
						if((path[i].s_end + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
							path[i].s_end =  StartPos[i];

						}	
						
						
					}
				
					else if((prc->fParam[0] ) < 0){

						if((path[i].s_end - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
							path[i].s_end =  StartPos[i];
						}
						
					}						

					path[i].Stop = 1;  // stop everything 
					path[i].Set_home = 1;	

					HCRSPFazz(&path[i]);

					path[i].Init = 1;
					HCRSPFazz(&path[i]);
					
					_StartPos[i] =  path[i].s_end; 
				
					prc->_step = 7;
											
				}
				else
					vect->Mode = vectSt_Mode_PT;


			break;



			case 5:
					
				prc->Status = prcsSTATUS_Alligment;
				path[i].Stop = 1;  // stop everything 
				path[i].s_home = vect->InVal ; // homing position 
				path[i].Set_home = 1;	
				prc->_step++; 						
				

			break;

			case 6:

				
				path[i].Init = 1;
				path[i].s_end = StartPos[i];
				prc->_step++;			
				


			break;	

			case 7:				

				vect->InVal = path[i].s_set;
				
				if(path[i].Reached){

					StartPos[i] = vect->InVal;
					prc->_step = 3;	

				}	

					
			break;

			case 10: // start


				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}

			
				prc->Status = prcsSTATUS_StartDly;	
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}

				//////////////////////////////////////////////

				if(_JumpToRun[i] || ((*OSC_TML_Param_Set[i].pStartDly) < 0.005))				
					TimerStart(&Tmr[i], 0.005);
				else
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
				
				prc->_step = 11;
				
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
				
				
				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}
				
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}				

				prc->Status = prcsSTATUS_RampUp;

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) {// not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));
				}	
				else {					
					
					TimerStart(&Tmr[i], 0.01);

				}

				prc->Param[0] = 0;

				// It is important to make first 
				if(Ramp[i].dy_up > 100.0)
					Ramp[i].dy_up = 100.0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				Ramp[i].set = 0;

				AzzRamp(&Ramp[i]);
				

				time[i] = 0;
				Invert[i] = 0;
				Dist[i] = 0;
				amplit[i] = Ramp[i].y;
				oldAmplit[i]= amplit[0];
				vect->InVal =  GenCam(i);	

				//OSC_TVL_Dist[i] = floorf(TVL_ROT_Param_Set[i].EncPos_FBK_Float / OSC_Period[i]) * OSC_Period[i];
				

				prc->_step++;
				
				//prc->_step = 255;

				
				
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

			break;


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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) {// not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));

				}	
				else {
					Ramp[i].dy_down = 3.4e+38;
					TimerStart(&Tmr[i], 0.01);

				}
				
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				AzzRamp(&Ramp[i]);
								
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
					
					prc->_step = 3;			

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
				
				
				if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){	
					
					if(prc->fParam[0] != 0){
						
						if(vect->FbkVect.Mode == vectSt_Mode_PT){

							///////////////////

							//_StartPos[i] =  vect->InVal + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
							_StartPos[i] =  IndxRamp[i].x + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
							 

							if(prc->fParam[0] > 0.0){
							
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =  vect->InVal;

								}
								//else
									//vect->InVal = _StartPos[i];
									
								
							}
						
							else {

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =  vect->InVal;

								}
								//else
									//vect->InVal = _StartPos[i];
							}

							 //StartPos[i] = vect->InVal;

							IndxRamp[i].y_set =_StartPos[i];
							IndxRamp[i].set = 1;
							AzzRamp(&IndxRamp[i]);
							IndxRamp[i].set = 0;
					
							vect->InVal = _StartPos[i];
							

							//////////////////
						}						
						 
					}
					else 						
						prc->_step  = 3;
											
						
				}						

			break;		

			
		}

		if(IndxRamp[i].dy_up <= 0.0){

			IndxRamp[i].dy_up = 3.4e+38;
			IndxRamp[i].dy_down = IndxRamp[i].dy_up;

		}
		IndxRamp[i].x = _StartPos[i];
		AzzRamp(&IndxRamp[i]);

		if(IndxRamp[i].status == 0)
			StartPos[i] = IndxRamp[i].y;
		else
			StartPos[i] = _StartPos[i];
				

		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = (StartPos[i] + orgStarPos[i]) - VirtCenter[i];// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);

			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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

void OscProcess_TML_ENC_dziala(void)
{
int i;
float x, delta , t;
UINT status;
	
	
		
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
		/*
		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_SteerSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeed); 
		KK_JogSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl); 
		*/
		//////////////////////////////////////

				/////////////////////////////////////////
		if(OSC_TML_Param_Set[i].pTML){
			
			//VirtCenter[i] = OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg +((OSC_TML_Param_Set[i].pTML->_cfg.sftLimitPos - OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg)/2);
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			orgStarPos[i] =  OSC_TML_Param_Set[i].pTML->_int.startPos;
		
		}


		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if(prc->_step >= 12){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;

							}

							
						
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	IndxRamp[i].x;
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}

						}					

						if(OSC_TML_Param_Set[i].pIndxVel){
							IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							_StartPos[i] += delta;

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	IndxRamp[i].x;
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}
							KK_delta = delta;	

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							 if(t > 0){
								
								IndxRamp[i].dy_up = fabsf(delta/t * (1000.0/ 2.4));

							 	if(IndxRamp[i].dy_up > (*OSC_TML_Param_Set[i].pIndxVel))
									IndxRamp[i].dy_up  = (*OSC_TML_Param_Set[i].pIndxVel);
								
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
								
							 }
							 else {
				

								//IndxRamp[i].dy_up = 3.4e+38;
								IndxRamp[i].dy_up = 8.0;
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;

							}
							
							/*
							else if(OSC_TML_Param_Set[i].pIndxVel){
								
								IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							}
							*/

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;
			
	
		
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

				case prcsAlligment:

					
					if(prc->_step == 3){ 

						CorrectStartPos(i);
						prc->_step = 5;
					}	

				break;

				case prcsJump2Run:
				case prcsSTART:

					_JumpToRun[i] = ((prc->xCMD == prcsJump2Run))? 1:0;		
					//_JumpToRun[i] = 0;	

					prc->Param[0] = 0; // Not bypass Ramp ?? This can be discussed

					if(prc->Param[3] > 0 )						
						CorrectAmplitude(i);
					else  
						CorrectStartPos(i);

					if((prc->_step == 3)){
						
						path[i].Stop = 1;  // stop everything 
						path[i].s_home = vect->InVal ; // homing position 
						path[i].Set_home = 1;	

						HCRSPFazz(&path[i]);

						path[i].Init = 1;
						path[i].s_end = StartPos[i];

						HCRSPFazz(&path[i]);

						prc->_step = 10;
					}

									
				break;

				
				
				case prcsSTOP:

					if((prc->_step > 3) && (prc->_step <= 15)){				

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 23)){
						
						prc->_step = 22;										

					}

				break;				


				case prcsSTEPpos:		

					
					if((prc->fParam[1] != 0.0) && (prc->_step == 3)){

						prc->Status = prcsSTATUS_Step;
						prc->_step = 4;
					}
					
					
				break;						
					
				case prcsJOGpos:

					
					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;
					

					if(vect->FbkVect.Mode == vectSt_Mode_PT){
						
						//StartPos[i] += prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)* JogStepFact[i];
						//_StartPos[i] =  StartPos[i] + (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed) ) ;
						_StartPos[i] =  IndxRamp[i].x + (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed) ) ;
						

						if((prc->fParam[0] ) > 0){
						
							if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								_StartPos[i] =  StartPos[i];

							}	
							
							
						}
					
						else if((prc->fParam[0] ) < 0){

							if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								_StartPos[i] =  StartPos[i];
							}
							
						}

						//CorrectStartPos(i);

						if(IndxRamp[i].x_reached){
							IndxRamp[i].dy_up = 3.4e+38;
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}
												
					}
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}

		}
		
		

		/////////////////////////////////////////////////////////
		
		prc->xCMD = 0;
		
		switch(prc->_step){

			

			case 0:  // IDLE - stopped
			
				prc->Status = prcsSTATUS_Init;	

				if(vect->FbkVect.Mode != vectSt_Mode_Idle){

					vect->Mode = vectSt_Mode_Idle;
				}
				else{
					TimerStart(&Tmr[i], 4.0);
					//vect->InVal = vect->FbkVect.x;
					//vect->Mode = vectSt_Mode_PT;
							
					prc->_step++;		

				}
				
														
			break;

			case 1:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				
				vect->InVal = vect->FbkVect.x;		
				
				if(vect->FbkVect.Mode == vectSt_Mode_PT){	
						
					_StartPos[i] = vect->InVal;
									
					IndxRamp[i].y_set =_StartPos[i];
					IndxRamp[i].set = 1;
					AzzRamp(&IndxRamp[i]);
					IndxRamp[i].set = 0;
					
					vect->InVal = IndxRamp[i].y;
					prc->_step = 3;
				}
				else
					vect->Mode = vectSt_Mode_PT;
				
			break;	


			case 2:


			break;
			
			case 3: 

				prc->Status = prcsSTATUS_Stopped;	
				//CorrectAmplitude(i);
				vect->InVal = StartPos[i];
			
			break;	


			case 4: 

				if(vect->FbkVect.Mode == vectSt_Mode_PT){
					
				
					
					path[i].s_home = StartPos[i];
					path[i].s_end = StartPos[i] + prc->fParam[1];
					

					if((prc->fParam[0] ) > 0){
					
						if((path[i].s_end + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
							path[i].s_end =  StartPos[i];

						}	
						
						
					}
				
					else if((prc->fParam[0] ) < 0){

						if((path[i].s_end - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
							path[i].s_end =  StartPos[i];
						}
						
					}						

					path[i].Stop = 1;  // stop everything 
					path[i].Set_home = 1;	

					HCRSPFazz(&path[i]);

					path[i].Init = 1;
					HCRSPFazz(&path[i]);
					
					_StartPos[i] =  path[i].s_end; 
				
					prc->_step = 7;
											
				}
				else
					vect->Mode = vectSt_Mode_PT;


			break;



			case 5:
					
				prc->Status = prcsSTATUS_Alligment;
				path[i].Stop = 1;  // stop everything 
				path[i].s_home = vect->InVal ; // homing position 
				path[i].Set_home = 1;	
				prc->_step++; 						
				

			break;

			case 6:

				
				path[i].Init = 1;
				path[i].s_end = StartPos[i];
				prc->_step++;			
				


			break;	

			case 7:				

				vect->InVal = path[i].s_set;
				
				if(path[i].Reached){

					StartPos[i] = vect->InVal;
					prc->_step = 3;	

				}	

					
			break;

			case 10: // start


				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}

			
				prc->Status = prcsSTATUS_StartDly;	
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}

				//////////////////////////////////////////////

				if(_JumpToRun[i] || ((*OSC_TML_Param_Set[i].pStartDly) < 0.005))				
					TimerStart(&Tmr[i], 0.005);
				else
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
				
				prc->_step = 11;
				
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
				
				
				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}
				
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}				

				prc->Status = prcsSTATUS_RampUp;

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) {// not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));
				}	
				else {					
					
					TimerStart(&Tmr[i], 0.01);

				}

				prc->Param[0] = 0;

				// It is important to make first 
				if(Ramp[i].dy_up > 100.0)
					Ramp[i].dy_up = 100.0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				Ramp[i].set = 0;

				AzzRamp(&Ramp[i]);
				

				time[i] = 0;
				Invert[i] = 0;
				Dist[i] = 0;
				amplit[i] = Ramp[i].y;
				oldAmplit[i]= amplit[0];
				vect->InVal =  GenCam(i);	

				//OSC_TVL_Dist[i] = floorf(TVL_ROT_Param_Set[i].EncPos_FBK_Float / OSC_Period[i]) * OSC_Period[i];
				

				prc->_step++;
				
				//prc->_step = 255;

				
				
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

			break;


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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) {// not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));

				}	
				else {
					Ramp[i].dy_down = 3.4e+38;
					TimerStart(&Tmr[i], 0.01);

				}
				
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				AzzRamp(&Ramp[i]);
								
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
					
					prc->_step = 3;			

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
				
				
				if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){	
					
					if(prc->fParam[0] != 0){
						
						if(vect->FbkVect.Mode == vectSt_Mode_PT){

							///////////////////

							//_StartPos[i] =  vect->InVal + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
							_StartPos[i] =  IndxRamp[i].x + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
							 

							if(prc->fParam[0] > 0.0){
							
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =  vect->InVal;

								}
								//else
									//vect->InVal = _StartPos[i];
									
								
							}
						
							else {

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =  vect->InVal;

								}
								//else
									//vect->InVal = _StartPos[i];
							}

							 //StartPos[i] = vect->InVal;

							IndxRamp[i].y_set =_StartPos[i];
							IndxRamp[i].set = 1;
							AzzRamp(&IndxRamp[i]);
							IndxRamp[i].set = 0;
					
							vect->InVal = _StartPos[i];
							

							//////////////////
						}						
						 
					}
					else 						
						prc->_step  = 3;
											
						
				}						

			break;		

			
		}

		if(IndxRamp[i].dy_up <= 0.0){

			IndxRamp[i].dy_up = 3.4e+38;
			IndxRamp[i].dy_down = IndxRamp[i].dy_up;

		}
		IndxRamp[i].x = _StartPos[i];
		AzzRamp(&IndxRamp[i]);

		if(IndxRamp[i].status == 0)
			StartPos[i] = IndxRamp[i].y;
		else
			StartPos[i] = _StartPos[i];
				

		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = (StartPos[i] + orgStarPos[i]) - VirtCenter[i];// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);

			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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
//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void OscProcess_TML_ENC_POT(void)
{int i;
float x, delta , t;
UINT status;
	
	
		
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
		/*
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_SteerSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeed); 
		KK_JogSpeed[i] = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl); 
		*/
		//////////////////////////////////////

				/////////////////////////////////////////
		if(OSC_TML_Param_Set[i].pTML){
			
			//VirtCenter[i] = OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg +((OSC_TML_Param_Set[i].pTML->_cfg.sftLimitPos - OSC_TML_Param_Set[i].pTML->_cfg.sftLimitNeg)/2);
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			//orgStarPos[i] =  OSC_TML_Param_Set[i].pTML->_int.startPos;
		
		}


		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if(prc->_step >= 12){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								//_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
								delta = OSC_IndxDist[i]; 
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								//_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;
								delta = -OSC_IndxDist[i]; 

							}

							MoveStartPosbyPot(i, delta);

							/*
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	IndxRamp[i].x;
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}
							*/

						}					

						if(OSC_TML_Param_Set[i].pIndxVel){
							IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							//_StartPos[i] += delta;

							
							MoveStartPosbyPot(i, delta);

							/*

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	IndxRamp[i].x;
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	IndxRamp[i].x;
								}
								
							}
							KK_delta = delta;	

							*/

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							 if(t > 0){
								
								IndxRamp[i].dy_up = fabsf(delta/t * (1000.0/ 2.4));

							 	if(IndxRamp[i].dy_up > (*OSC_TML_Param_Set[i].pIndxVel))
									IndxRamp[i].dy_up  = (*OSC_TML_Param_Set[i].pIndxVel);
								
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
								
							 }
							 else {
				

								//IndxRamp[i].dy_up = 3.4e+38;
								IndxRamp[i].dy_up = 8.0;
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;

							}
							
							/*
							else if(OSC_TML_Param_Set[i].pIndxVel){
								
								IndxRamp[i].dy_up = (*OSC_TML_Param_Set[i].pIndxVel);
								IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							}
							*/

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;
			
	
		
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

				case prcsAlligment:

					
					if(prc->_step == 3){ 

						CorrectStartPosbyPOT(i);
						prc->_step = 5;
					}	

				break;
						
				/*
				case prcsSTART:


					if(prc->Param[3] > 0 )						
						CorrectAmplitudebyPOT(i);
					else  
						CorrectStartPosbyPOT(i);

					if((prc->_step == 3)){
						
						path[i].Stop = 1;  // stop everything 
						path[i].s_home = vect->InVal ; // homing position 
						path[i].Set_home = 1;	

						HCRSPFazz(&path[i]);

						path[i].Init = 1;
						path[i].s_end = StartPos[i];

						HCRSPFazz(&path[i]);

						prc->_step = 10;
					}

									
				break;
				*/
				
				case prcsSTART:
				case prcsJump2Run:

					if(prc->Param[3] > 0 )						
						CorrectAmplitudebyPOT(i);
					else 
						CorrectStartPosbyPOT(i);
					
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;

					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;
					
					path[i].Stop = 1;  // stop everything 
					path[i].s_home = vect->InVal ; // homing position 
					path[i].Set_home = 1;	

					HCRSPFazz(&path[i]);

					path[i].Init = 1;
					path[i].s_end = StartPos[i];

					HCRSPFazz(&path[i]);


					switch(prc->xCMD){

						case prcsSTART: prc->_step = 10; break;
							
						case prcsJump2Run: prc->_step = 12; break;
					}									

				break;	
				
				case prcsSTOP:

					if((prc->_step > 3) && (prc->_step <= 15)){				

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 23)){
						
						prc->_step = 22;										

					}

				break;				

				case prcsJOGpos:

					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;
					

					if(vect->FbkVect.Mode == vectSt_Mode_PT){
						
						//StartPos[i] += prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)* JogStepFact[i];
						//_StartPos[i] =  StartPos[i] + (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed) ) ;

						delta = (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed));

						MoveStartPosbyPot(i , delta);					
						

						//CorrectStartPos(i);

						if(IndxRamp[i].x_reached){
							IndxRamp[i].dy_up = 3.4e+38;
							IndxRamp[i].dy_down = IndxRamp[i].dy_up;
						}
												
					}
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}

		}
		
		

		/////////////////////////////////////////////////////////
		
		prc->xCMD = 0;
		
		switch(prc->_step){

			

			case 0:  // IDLE - stopped
			
				prc->Status = prcsSTATUS_Init;	

				if(vect->FbkVect.Mode != vectSt_Mode_Idle){

					vect->Mode = vectSt_Mode_Idle;
				}
				else{
					TimerStart(&Tmr[i], 4.0);
					//vect->InVal = vect->FbkVect.x;
					//vect->Mode = vectSt_Mode_PT;

					CompOffset_Step[i] = 0;	
				
					prc->_step++;		

				}
				
														
			break;

			case 1:

				if(!TimerIsTimeOut(&Tmr[i]))
					break;
				
				orgStarPos[i] = vect->FbkVect.x2;
				orgStarPosEnc[i] = vect->FbkVect.x;
				StartPosbyPOT[i] = vect->FbkVect.x2;
					
				vect->InVal = vect->FbkVect.x;		
				
				if(vect->FbkVect.Mode == vectSt_Mode_PT){	
						
					_StartPos[i] = vect->InVal;
					
					IndxRamp[i].y_set =_StartPos[i];
					IndxRamp[i].set = 1;
					AzzRamp(&IndxRamp[i]);
					IndxRamp[i].set = 0;
					
					vect->InVal = IndxRamp[i].y;
					prc->_step = 3;
				}
				else
					vect->Mode = vectSt_Mode_PT;
				
			break;	


			case 2:


			break;
			
			case 3: 

				prc->Status = prcsSTATUS_Stopped;	
				//CorrectAmplitude(i);
				vect->InVal = StartPos[i];
			
				//orgStarPos[i] = vect->FbkVect.x2;
				//orgStarPosEnc[i] = vect->FbkVect.x;
				//StartPosbyPOT[i] = vect->FbkVect.x2;

			break;	



			case 5:
					
				prc->Status = prcsSTATUS_Alligment;
				path[i].Stop = 1;  // stop everything 
				path[i].s_home = vect->InVal ; // homing position 
				path[i].Set_home = 1;	
				prc->_step++; 						
				

			break;

			case 6:

				
				path[i].Init = 1;
				path[i].s_end = StartPos[i];
				prc->_step++;			
				


			break;	

			case 7:

				vect->InVal = path[i].s_set;
				
				if(path[i].Reached){

					prc->_step = 3;	

				}	

					
			break;

			case 10: // start


				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}

			
				prc->Status = prcsSTATUS_StartDly;	
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}

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
				
				
				if(!path[i].Reached){
					vect->InVal = path[i].s_set;
					break;

				}
				
				
				if(vect->FbkVect.Mode != vectSt_Mode_PT){									
					vect->Mode = vectSt_Mode_PT;
					vect->InVal = StartPos[i];
					break;
				}				

				prc->Status = prcsSTATUS_RampUp;

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) {// not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));
				}	
				else {
					
					Ramp[i].dy_up = 3.4e+38;
					TimerStart(&Tmr[i], 0.01);

				}
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				Ramp[i].set = 0;

				AzzRamp(&Ramp[i]);
				

				time[i] = 0;
				Invert[i] = 0;
				Dist[i] = 0;
				amplit[i] = Ramp[i].y/2.0;
				oldAmplit[i]= Ramp[i].y/2.0;
				//oldAmplit[i]= 0.0; // Zle przesuwa
				vect->InVal =  GenCam(i);	

				//OSC_TVL_Dist[i] = floorf(TVL_ROT_Param_Set[i].EncPos_FBK_Float / OSC_Period[i]) * OSC_Period[i];
				

				prc->_step++;
				
				//prc->_step = 255;

				
				
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

			break;


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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) {// not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));

				}	
				else {
					Ramp[i].dy_down = 3.4e+38;
					TimerStart(&Tmr[i], 0.01);

				}
				
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				AzzRamp(&Ramp[i]);
								
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
				
				
				if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){	
					
					if(prc->fParam[0] != 0){
						
						if(vect->FbkVect.Mode == vectSt_Mode_PT){



							delta = (prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)) * JogStepFact2[i];

							
							MoveStartPosbyPot_Abs(i , delta);
							StartPosbyPOT[i] = vect->FbkVect.x2;
							
							//MoveStartPosbyPot(i , delta);
									
							IndxRamp[i].y_set =_StartPos[i];
							IndxRamp[i].set = 1;
							AzzRamp(&IndxRamp[i]);
							IndxRamp[i].set = 0;
							
							vect->InVal = _StartPos[i];

							if(KK_Compensate)
								vect->InVal += CompRamp[i].y;							
							

							//////////////////
						}						
						 
					}
					else 						
						prc->_step  = 3;
											
						
				}						

			break;		

			
		}

		if(IndxRamp[i].dy_up <= 0.0){

			IndxRamp[i].dy_up = 3.4e+38;
			IndxRamp[i].dy_down = IndxRamp[i].dy_up;

		}
		IndxRamp[i].x = _StartPos[i];
		AzzRamp(&IndxRamp[i]);

		if(IndxRamp[i].status == 0)
			StartPos[i] = IndxRamp[i].y; 
		else
			StartPos[i] = _StartPos[i];

		CalculateCompOffset(i);

		if(KK_Compensate)
			StartPos[i] += CompRamp[i].y;	
		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos

		//prc->Fbk[1] = (StartPos[i] + orgStarPos[i]) - VirtCenter[i];// center Position  - varOSC_Centr
		prc->Fbk[1] = StartPosbyPOT[i] - VirtCenter[i];// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x2; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);

			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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


//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

void OscProcess_STND()
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
		/*
		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		*/
		//////////////////////////////////////

		
				
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

				/*
				case prcsHalt:

					
					if(prc->_step >= 2) 
						prc->_step = 30;

				break;
				*/

				case prcsSTART:

					if((prc->_step == 3))
						prc->_step = 10;

					
				break;


				case prcsJump2Run:
					
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;

					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;
					
					vect->InVal = StartPos[i];  
					
					prc->_step = 12;
				

				break;	


				case prcsSTOP:

					if((prc->_step > 3) && (prc->_step <= 15)){				

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 21)){
						
						prc->_step = 22;										

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
						//StartPos[i] += prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeed)* JogSpeedFact[i];
					
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

		if(prc->_step >= 3){ //Evaluate only when StartPos is determined

			if(vect->OutVect.softLimits[0]  < vect->OutVect.softLimits[1] ){	

				if(OSC_TML_Param_Set[i].MaxOscAmpl == 0.0){ // Old verification system
				
					if((*OSC_TML_Param_Set[i].pPrimAmpl) > (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]))
						(*OSC_TML_Param_Set[i].pPrimAmpl) = (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]);
						
					if((StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
						StartPos[i] = vect->OutVect.softLimits[1] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2;
						SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
					}

					else if((StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
						StartPos[i] = vect->OutVect.softLimits[0] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2;
						SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
					}

				}
				else if(OSC_TML_Param_Set[i].MaxOscAmpl > 0.0){
					
					if(StartPos[i] > vect->OutVect.softLimits[1]){
						StartPos[i] = vect->OutVect.softLimits[1];
						SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
					}				
					else if(StartPos[i] < vect->OutVect.softLimits[0]){
						StartPos[i] = vect->OutVect.softLimits[0];
						SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
					}

				}
				
			}

		}
			
		///////////////////////////////////
			
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
			

				/*
				if(vect->FbkVect.Mode != vectSt_Mode_PT)									
					vect->Mode = vectSt_Mode_PT;
								
				vect->InVal = StartPos[i];

				*/
				
				

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

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) // not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
				else 
					Ramp[i].dy_up = 3.4e+38;
					
				prc->Param[0] = 0;
				
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
				
				//prc->_step = 255;

				
				
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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) // not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
				else 
					Ramp[i].dy_down = 3.4e+38;
					
				prc->Param[0] = 0;
				
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

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = StartPos[i] -  vect->InVal  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] =  (StartPos[i] + orgStarPos[i]) - VirtCenter[i];// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);


		
			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			
			

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

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
void OscProcess_BEMF(void)
{
int i;
float x, delta , t;
UINT status;	
	
		
	for(i=0 ; i < OSC_NUM ; i++){

		prc = OSC_TML_Param_Set[i].pPrc ;

		if(!prc)
			break;

		vect = OSC_TML_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
			break;


		if(OSC_TML_Param_Set[i].pTML){
			
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			orgStarPos[i] =  0;
		
		}

		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if((prc->_step >= 12) && (prc->_step < 30)  ){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;

							}

							
						
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	StartPos[i];
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	StartPos[i];
								}
								
							}

						}					

						StartPos[i] = _StartPos[i];

						updateLimits[i] = 1;

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							_StartPos[i] += delta;

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	StartPos[i];
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	StartPos[i];
								}
								
							}
							KK_delta = delta;	

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							StartPos[i] = _StartPos[i];
							updateLimits[i] = 1;

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing or No Oscillation

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

		
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		
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

				case prcsAlligment:

					if(OSC_Mode[i] != 3){ // NOT NO OSICLLATION 
					
						if(prc->_step == 3){ 

							CorrectStartPos(i);
							prc->_step = 5;
						}

					}

				break;

				case prcsSTART:

					if(OSC_Mode[i] == 3){ // NO OSICLLATION jus Steering

						prc->_step = 80;
						

					}else{
					
						if((prc->_step == 3)){
							

							if(prc->Param[3] > 0 )						
								CorrectAmplitude(i);
							else  
								CorrectStartPos(i);
						
							prc->_step = 10;


						}
					}

					
				break;


				case prcsJump2Run:

					if(OSC_Mode[i] == 3){ // NO OSICLLATION jus Steering

						prc->_step = 80;
						

					}else{
					
						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;

						AzzRamp(&Ramp[i]);
						Ramp[i].set = 0;
						
						//vect->InVal = StartPos[i];  
						
						prc->_step = 12;

					}
				

				break;	


				
				case prcsSTOP:

				
					if(OSC_Mode[i] == 3){ // NO OSICLLATION jus Steering

						prc->_step = 0;
						

					}
				
					else if((prc->_step > 3) && (prc->_step <= 15)){ 			

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:

					if(OSC_Mode[i] == 3){ // NO OSICLLATION jus Steering

						prc->_step = 0;

					}				
					else if((prc->_step > 3) && (prc->_step <= 23))
					{
						
						prc->_step = 24;										

					}

				break;				

				case prcsJOGpos:

					
					if(OSC_Mode[i] == 3) // NO OSICLLATION jus Steering
						break;
					
					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;
					
				
					_StartPos[i] =	StartPos[i] + (prc->fParam[0] * (*OSC_TML_Param_Set[i].pJogSpeed) ) ;
					

					if((prc->fParam[0]  ) > 0){
					
						if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1])
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
						else
							StartPos[i] = _StartPos[i];
						
					}
				
					else if((prc->fParam[0] ) < 0){

						if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0])
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
						else
							StartPos[i] = _StartPos[i];
					}
					
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}
		}
		
		prc->xCMD = 0;

		
		///////////////////////////////////
		switch(prc->_step){


			case 0:  // IDLE - stopped

				prc->Status = prcsSTATUS_Stopped;	
				
				vect->InVal = 0.0;
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)									
					vect->Mode = vectSt_Mode_Vel;
				else
					prc->_step ++;					
														
			break;

			case 1:

				StartPos[i] = vect->FbkVect.x;
				StopOSC(vect, i);				
				TimerStart(&Tmr[i], 1.0);
				prc->_step++;				
				
			break;	


			case 2:
				
				prc->Status = prcsSTATUS_Stopped;	

				
				if(TimerIsTimeOut(&Tmr[i]))
				{
					StartPos[i] = vect->FbkVect.x;
					StopOSC(vect, i);
					prc->_step++;
				}
				else
					break;
				

			break;	
				

			case 3: 

				prc->Status = prcsSTATUS_Stopped;	
				vect->InVal = 0.0;
				StartPos[i] = vect->FbkVect.x;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)									
					vect->Mode = vectSt_Mode_Vel;

			break;	


			case 5:

				prc->Status = prcsSTATUS_Alligment;	
				StartAlligment(vect, i , 1.0);
				TimerStart(&Tmr[i], 1.5);
				prc->_step++;
			

			break;



			case 6:

				if(TimerIsTimeOut(&Tmr[i])){
					StopOSC(vect, i);
					prc->_step = 3;

			}

			break;

			case 10: // start

				
				prc->Status = prcsSTATUS_StartDly;	
				
						
				if((*OSC_TML_Param_Set[i].pStartDly) > 0){

					CorrectAmplitude(i);
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
					prc->_step = 11;
				}
				else 					
					prc->_step = 12;

				
				
			break;


			case 11:

						
				CorrectAmplitude(i);	
				if(TimerIsTimeOut(&Tmr[i]))
					prc->_step++;
				
							
					
			break;	
			

			case 12:
							

				prc->Status = prcsSTATUS_RampUp;
				CorrectAmplitude(i);

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) // not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
				else 
					Ramp[i].dy_up = 3.4e+38;
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				
				Ramp[i].set = 0;		
				AzzRamp(&Ramp[i]);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));

				StartOSC(vect, i , Ramp[i].x );

				prc->_step++;
				
				//prc->_step = 255;

				
				
			break;	



			case 13:	

				CorrectAmplitude(i);
				
				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
					TimerStart(&Tmr[i], 2.0);
					prc->_step ++;
				}
				else if(!vect->xCmd_busy)
					UpdateOSC( vect, i , Ramp[i].y );
		

			break;


			case 14:

				if(TimerIsTimeOut(&Tmr[i])){

					if(!vect->xCmd_busy){
						CorrectAmplitude(i);
						StartOSC( vect, i , Ramp[i].y );			
						prc->_step ++;

					}

				}	

			break;


			case 15:

				prc->Status = prcsSTATUS_Run;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);

				CorrectAmplitude(i);
				if((KK_Ampl[i] != (*OSC_TML_Param_Set[i].pPrimAmpl)) 
					||(KK_Excur[i] != (*OSC_TML_Param_Set[i].pFreq))
					||(KK_DwellIn[i] != (*OSC_TML_Param_Set[i].pDwell_In))
					||(KK_DwellOut[i] != (*OSC_TML_Param_Set[i].pDwell_Out)) 
					||(KK_StartPos[i] != StartPos[i]))
					StartOSC(vect, i, Ramp[i].y);
									
			break;


			case 20:

				prc->Status = prcsSTATUS_StopDly;
				
				if((*OSC_TML_Param_Set[i].pStopDly) > 0){
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStopDly));
					prc->_step = 21;
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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pFinalAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) // not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
				else 
					Ramp[i].dy_down = 3.4e+38;
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));				
				
				prc->_step++;	

				

			break;

			case 23:

				if(!vect->xCmd_busy)
					UpdateOSC( vect, i , Ramp[i].y );

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					//StopOSC(vect, i);
					prc->_step++; 
						
				}

			break;

			case 24:	
				
				StopOSC(vect, i);
				prc->_step++;	

			break;	


			case 25:

				StopOSC(vect, i);
				prc->_step = 0;			

					
			break;

			case 30: // Quick Stop

				StopOSC(vect , i);
				
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

						///////////////////

						_StartPos[i] =  vect->FbkVect.x + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
						
											
						if((prc->fParam[0]  )  > 0.0){
						
							if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
								vect->InVal = 0.0;
												
												
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
												
							
						}
					
						else if((prc->fParam[0]  ) < 0.0) {

							if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) < vect->OutVect.softLimits[0]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								vect->InVal = 0.0;
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0]  ;
												
							
						}else
							vect->InVal = 0;
						
						
						//////////////////
						
						if(prc->fParam[0] == 0.0)
							prc->_step = 3;
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
					prc->_step = 3;
		
			break;



			case 80: // No Oscillation just steering


				prc->Status = prcsSTATUS_Run;	
				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
				}	
				else{

					if(OSC_TML_Param_Set[i].pJogSpeedDirCntrl){ 

						StartPos[i] = vect->FbkVect.x;

						///////////////////

						_StartPos[i] =	vect->FbkVect.x + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
						
											
						if((prc->fParam[0]	)  > 0.0){
						
							if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
								vect->InVal = 0.0;
												
												
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] * 10.0 ;
												
							
						}
					
						else if((prc->fParam[0]  ) < 0.0) {

							if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) < vect->OutVect.softLimits[0]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								vect->InVal = 0.0;
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0]* 10.0   ;
												
							
						}else
							vect->InVal = 0;
						
						
						//////////////////
											
							
					}				
						
				}				


			break;

			
		}

		
		
		//prc->Fbk[0] = vect->FbkVect.x;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = (StartPos[i] -  vect->InVal)  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = ((StartPos[i] + orgStarPos[i]) - VirtCenter[i]) ;// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x  ; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);
			
			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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


		//////////   ////////////////////////

		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		//KK_Ampl[i] = Ramp[i].y;
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_StartPos[i] = StartPos[i];
		
		//////////////////////////////////////
		if(OSC_TML_Param_Set[0].Mode != 11)
			ObserveOsc(vect , i);

		if(KK_test && (prc->Status == prcsSTATUS_Run)){

			if(!updateLimits[i]){
				updateLimits[i] = 1;

				StartPos[i] += 0.01;
				
			}

		}

		if(updateLimits[i]){

			if((!vect->xCMD) && (!vect->xCmd_busy))
				UpdateOSC(vect, i , Ramp[i].y );
		}

		
	}

	
}

///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void OscProcess_BEMF_new(void) 
{
int i;
float x, delta , t;
UINT status;	
	
		
	for(i=0 ; i < OSC_NUM ; i++){

		prc = OSC_TML_Param_Set[i].pPrc ;

		if(!prc)
			break;

		vect = OSC_TML_Param_Set[i].pVect;

		if(!vect)
			break;

		if(prc->ConfigReady <= 0)
			break;


		if(OSC_TML_Param_Set[i].pTML){
			
			VirtCenter[i] = vect->OutVect.softLimits[0] +((vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0])/2);
			orgStarPos[i] =  0;
		
		}

		/////////////////////////////////////////////////////////
		//////   OScillation Center moving 
		OSC_Mode[i] = (*OSC_TML_Param_Set[i].pOSC_Mode);
		OSC_IndxDist[i] = (*OSC_TML_Param_Set[i].pIndexDist);
		OSC_Period[i] = (*OSC_TML_Param_Set[i].pPeriod);


		if((prc->_step >= 12) && (prc->_step < 20)  ){

			status = 1;
			t = 0.0;

			switch(*OSC_TML_Param_Set[i].pOSC_Synch){

				case 1: // Dwell In

					status = (SynchClckFromOsc == 1)? 1:0;
			
					t = (*OSC_TML_Param_Set[i].pDwell_In);

				break;
				
				case 2: // Dwell Out

					status = (SynchClckFromOsc == 2)? 1:0;
					t = (*OSC_TML_Param_Set[i].pDwell_Out);

				break;

				case 3: // Dwell in or Out

					if(SynchClckFromOsc == 1)
						t = (*OSC_TML_Param_Set[i].pDwell_In);					
					
					else if(SynchClckFromOsc == 2)
						t = (*OSC_TML_Param_Set[i].pDwell_Out);	
					
					else
						status = 0;


				break;

				case 4: // Excrusion

					status = (SynchClckFromOsc == 0)? 1:0;
					t = time[i];

				break;
				

			}

			switch( OSC_Mode[i] ){

				// STEPOVER
				case 1: 

					//if(status){
						
						if(fabsf((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])) > fabsf((float)OSC_Period[i]) ){

							if(TVL_ROT_Param_Set[i].EncPos_FBK_Float > OSC_TVL_Dist[i]){
								OSC_TVL_Dist[i] += fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] + OSC_IndxDist[i] ;
							}
							else {
								OSC_TVL_Dist[i] -= fabsf(OSC_Period[i]);
								_StartPos[i] =  _StartPos[i] - OSC_IndxDist[i] ;

							}

							
						
							if(_StartPos[i]  >  IndxRamp[i].x){
								
									if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
										SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
										_StartPos[i] =	StartPos[i];
									}
									
									
							}
						
							else if(_StartPos[i]  <  IndxRamp[i].x){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	StartPos[i];
								}
								
							}

						}					

						StartPos[i] = _StartPos[i];

						updateLimits[i] = 1;

					//}					

				break;


				// SPIRAL
				case 2:						


					if(status){
				
						if((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i]) != 0.0 ){

							
							if(OSC_Period[i] > 0.0)
								delta = OSC_IndxDist[i]* ((TVL_ROT_Param_Set[i].EncPos_FBK_Float - OSC_TVL_Dist[i])/((float)OSC_Period[i])) ;
							else
								delta = 0.0;
							
							_StartPos[i] += delta;

							if(delta > 0){								
								if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
									_StartPos[i] =	StartPos[i];
								}
									
									
							}

							else if(delta < 0){

								if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
									SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
									_StartPos[i] =	StartPos[i];
								}
								
							}
							KK_delta = delta;	

							
							//if(IndxRamp[i].x_reached){
								//IndxRamp[i].dy_up = 3.4e+38;
								//IndxRamp[i].dy_down = IndxRamp[i].dy_up;
							//}


							StartPos[i] = _StartPos[i];
							updateLimits[i] = 1;

							OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

						}

					}				


				break;



				// Nothing

				default: break;

			}

		}
		else
			OSC_TVL_Dist[i] = TVL_ROT_Param_Set[i].EncPos_FBK_Float;

		
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		
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

				case prcsAlligment:

					
					if(prc->_step == 3){ 

						CorrectStartPos(i);
						prc->_step = 5;
					}	

				break;

				case prcsSTART:

					if((prc->_step == 3)){

						if(prc->Param[3] > 0 )						
							CorrectAmplitude(i);
						else  
							CorrectStartPos(i);
					
						prc->_step = 10;


					}

					
				break;


				case prcsJump2Run:
					
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;

					AzzRamp(&Ramp[i]);
					Ramp[i].set = 0;
					
					//vect->InVal = StartPos[i];  
					
					prc->_step = 12;
				

				break;	


				
				case prcsSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 15)){ 			

						prc->_step = 20;									

					}

				break;

				case prcsQuickSTOP:
				
					if((prc->_step > 3) && (prc->_step <= 23))
					{
						
						prc->_step = 24;										

					}

				break;				

				case prcsJOGpos:

					if((prc->fParam[0] * vect->FbkWarning)> 0)
						prc->fParam[0] = 0;
					
				
					_StartPos[i] =	StartPos[i] + (prc->fParam[0] * (*OSC_TML_Param_Set[i].pJogSpeed) ) ;
					

					if((prc->fParam[0]  ) > 0){
					
						if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1])
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
						else
							StartPos[i] = _StartPos[i];
						
					}
				
					else if((prc->fParam[0] ) < 0){

						if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0])
							SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
						else
							StartPos[i] = _StartPos[i];
					}
					
					
				break;


				case prcsDirectCntrl:
					
					
					if(prc->_step != 60)
						prc->_step = 60;

					

				break;

				

				
			}
		}
		
		prc->xCMD = 0;

		
		///////////////////////////////////
		switch(prc->_step){


			case 0:  // IDLE - stopped

				prc->Status = prcsSTATUS_Stopped;	
				
				vect->InVal = 0.0;
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)									
					vect->Mode = vectSt_Mode_Vel;
				else
					prc->_step ++;					
														
			break;

			case 1:

				StartPos[i] = vect->FbkVect.x2;
				StopOSC(vect, i);				
				TimerStart(&Tmr[i], 1.0);
				prc->_step++;				
				
			break;	


			case 2:
				
				prc->Status = prcsSTATUS_Stopped;	

				
				if(TimerIsTimeOut(&Tmr[i]))
				{
					StartPos[i] = vect->FbkVect.x2;
					StopOSC(vect, i);
					prc->_step++;
				}
				else
					break;
				

			break;	
				

			case 3: 

				prc->Status = prcsSTATUS_Stopped;	
				vect->InVal = 0.0;
				// Comment - so only jogging change start position
				//StartPos[i] = vect->FbkVect.x2;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Vel)									
					vect->Mode = vectSt_Mode_Vel;

			break;	


			case 5:

				prc->Status = prcsSTATUS_Alligment;	
				StartAlligment(vect, i , 1.0);
				TimerStart(&Tmr[i], 1.5);
				prc->_step++;
			

			break;



			case 6:

				if(TimerIsTimeOut(&Tmr[i])){
					StopOSC(vect, i);
					prc->_step = 3;

			}

			break;

			case 10: // start

				
				prc->Status = prcsSTATUS_StartDly;	
				
						
				if((*OSC_TML_Param_Set[i].pStartDly) > 0){

					CorrectAmplitude(i);
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStartDly));
					prc->_step = 11;
				}
				else 					
					prc->_step = 12;

				
				
			break;


			case 11:

						
				CorrectAmplitude(i);	
				if(TimerIsTimeOut(&Tmr[i]))
					prc->_step++;
				
							
					
			break;	
			

			case 12:
							

				prc->Status = prcsSTATUS_RampUp;
				CorrectAmplitude(i);

				if((*OSC_TML_Param_Set[i].pInitAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pInitAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pInitAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pInitAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				Ramp[i].set 	= 1;
				Ramp[i].y_set 	= (*OSC_TML_Param_Set[i].pInitAmpl);
				AzzRamp(&Ramp[i]);
				Ramp[i].set 	= 0;

				if (((*OSC_TML_Param_Set[i].pRampUp) > 0) && (prc->Param[0] == 0)) // not bypass ramp
					Ramp[i].dy_up = fabsf(((*OSC_TML_Param_Set[i].pPrimAmpl) - Ramp[i].y)/(*OSC_TML_Param_Set[i].pRampUp));
				else 
					Ramp[i].dy_up = 3.4e+38;
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_down = Ramp[i].dy_up;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);
				
				Ramp[i].set = 0;		
				AzzRamp(&Ramp[i]);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampUp));

				StartOSC(vect, i , Ramp[i].x );

				prc->_step++;
				
				//prc->_step = 255;

				
				
			break;	



			case 13:	

				CorrectAmplitude(i);
				
				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
					TimerStart(&Tmr[i], 2.0);
					prc->_step ++;
				}
				else if(!vect->xCmd_busy)
					UpdateOSC( vect, i , Ramp[i].y );
		

			break;


			case 14:

				if(TimerIsTimeOut(&Tmr[i])){

					if(!vect->xCmd_busy){
						CorrectAmplitude(i);
						StartOSC( vect, i , Ramp[i].y );			
						prc->_step ++;

					}

				}	

			break;


			case 15:

				prc->Status = prcsSTATUS_Run;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pPrimAmpl);

				CorrectAmplitude(i);
				if((KK_Ampl[i] != (*OSC_TML_Param_Set[i].pPrimAmpl)) 
					||(KK_Excur[i] != (*OSC_TML_Param_Set[i].pFreq))
					||(KK_DwellIn[i] != (*OSC_TML_Param_Set[i].pDwell_In))
					||(KK_DwellOut[i] != (*OSC_TML_Param_Set[i].pDwell_Out)) 
					||(KK_StartPos[i] != StartPos[i]))
					StartOSC(vect, i, Ramp[i].y);
									
			break;


			case 20:

				prc->Status = prcsSTATUS_StopDly;
				
				if((*OSC_TML_Param_Set[i].pStopDly) > 0){
					TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pStopDly));
					prc->_step = 21;
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

				if((*OSC_TML_Param_Set[i].pFinalAmpl) > (*OSC_TML_Param_Set[i].pPrimAmpl))
					(*OSC_TML_Param_Set[i].pFinalAmpl) = (*OSC_TML_Param_Set[i].pPrimAmpl);

				if((*OSC_TML_Param_Set[i].pFinalAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pFinalAmpl) = 0.0;

				if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
					(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;
					

				if ((*OSC_TML_Param_Set[i].pRampDwn) > 0 && (prc->Param[0] == 0)) // not stop with delay and ramp  
					Ramp[i].dy_down = fabsf((Ramp[i].y - (*OSC_TML_Param_Set[i].pFinalAmpl)) /(*OSC_TML_Param_Set[i].pRampDwn));
				else 
					Ramp[i].dy_down = 3.4e+38;
					
				prc->Param[0] = 0;
				
				Ramp[i].dy_up = Ramp[i].dy_down;

				Ramp[i].x = (*OSC_TML_Param_Set[i].pFinalAmpl);

				TimerStart(&Tmr[i], (*OSC_TML_Param_Set[i].pRampDwn));				
				
				prc->_step++;	

				

			break;

			case 23:

				if(!vect->xCmd_busy)
					UpdateOSC( vect, i , Ramp[i].y );

				if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

					//StopOSC(vect, i);
					prc->_step++; 
						
				}

			break;

			case 24:	
				if(!vect->xCmd_busy){
					StopOSC(vect, i);
					prc->_step++;	
				}

			break;	


			case 25:

				if(!vect->xCmd_busy){
					StopOSC(vect, i);
					prc->_step = 3;		
				}

					
			break;

			case 30: // Quick Stop

				StopOSC(vect , i);
				
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

						StartPos[i] = vect->FbkVect.x2;

						///////////////////

						_StartPos[i] =  vect->FbkVect.x2 + prc->fParam[0] *	(*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* JogStepFact2[i] ;
						
											
						if((prc->fParam[0]  )  > 0.0){
						
							if((_StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);	
								vect->InVal = 0.0;
												
												
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
												
							
						}
					
						else if((prc->fParam[0]  ) < 0.0) {

							if((_StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) < vect->OutVect.softLimits[0]){
								SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
								vect->InVal = 0.0;
							}
							else
								vect->InVal = (*OSC_TML_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0]  ;
												
							
						}else
							vect->InVal = 0;
						
						
						//////////////////
						
						if(prc->fParam[0] == 0.0)
							prc->_step = 3;
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
					prc->_step = 3;
		
			break;

			
		}

		
		
		//prc->Fbk[0] = vect->FbkVect.x2;// act Position 
		//prc->Fbk[1] = StartPos[i]; // center Position 
	
		TimerExecute(&Tmr[i]);	
		if(Ramp[i].dy_up <= 0.0){
			Ramp[i].dy_up = 3.4e+38;
			Ramp[i].dy_down = Ramp[i].dy_up;
			
		}

		if(prc->_step != 255){
			AzzRamp(&Ramp[i]);		
		 	HCRSPFazz(&path[i]);
		}


		prc->Fbk[0] = (StartPos[i] -  vect->InVal)  ;// act Position  vs center - varOSC_CurPos

		prc->Fbk[1] = ((StartPos[i] + orgStarPos[i]) - VirtCenter[i]) ;// center Position  - varOSC_Centr

		prc->Fbk[2] = vect->FbkVect.x2  ; // act Amlitude - varOSC_CurRawPos
		
		prc->Fbk[3] = Ramp[i].y; //Amplitude Cmd -varOSC_Amplit_Cmd

		prc->Fbk[4] = time[i]; //varOSC_curExcurTm
		

		if(OSC_TML_Param_Set[i].pDwell_In && OSC_TML_Param_Set[i].pDwell_Out){
			x = (*OSC_TML_Param_Set[i].pDwell_In) + (*OSC_TML_Param_Set[i].pDwell_Out);
			
			if(Master[0].pCfg){

				if(Master[0].pCfg->HeadCfg.OSC.DwellDir){

					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
				else{

					prc->Fbk[6]= (*OSC_TML_Param_Set[i].pDwell_In);
					prc->Fbk[7]= (*OSC_TML_Param_Set[i].pDwell_Out);
				}
			}
			

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


		//////////   ////////////////////////

		KK_Ampl[i] = (*OSC_TML_Param_Set[i].pPrimAmpl); 
		//KK_Ampl[i] = Ramp[i].y;
		KK_Excur[i] = (*OSC_TML_Param_Set[i].pFreq);
		KK_DwellIn[i] = (*OSC_TML_Param_Set[i].pDwell_In); 
		KK_DwellOut[i] = (*OSC_TML_Param_Set[i].pDwell_Out); 
		KK_StartPos[i] = StartPos[i];
		
		//////////////////////////////////////

		ObserveOsc(vect , i);

		if(KK_test && (prc->Status == prcsSTATUS_Run)){

			if(!updateLimits[i]){
				updateLimits[i] = 1;

				StartPos[i] += 0.01;
				
			}

		}

		if(updateLimits[i]){

			if((!vect->xCMD) && (!vect->xCmd_busy))
				UpdateOSC(vect, i , Ramp[i].y );
		}

		
	}

	
}




///////////////////////////////////////////////////////////////////

float GenCam(UINT k){
float d;
AzzLinSpline_typ *cam;

	if(OSC_TML_Param_Set[k].Mode == 3) // correct by Pot
		CorrectAmplitudebyPOT(k);
	else
		CorrectAmplitude(k);

	if((*OSC_TML_Param_Set[k].pCamType)>= CAM_NUM)
		(*OSC_TML_Param_Set[k].pCamType)= CAM_NUM-1;
	
	else if((*OSC_TML_Param_Set[k].pCamType)<0)
		(*OSC_TML_Param_Set[k].pCamType)= 0;
	
		
	cam = &fub_LinSpline[k][(*OSC_TML_Param_Set[k].pCamType)];
	
	if((*OSC_TML_Param_Set[k].pFreq) < 0)
		(*OSC_TML_Param_Set[k].pFreq) = 1.0;

	if(time[k] <= 1){
		time[k] += (ClassTime / ((*OSC_TML_Param_Set[k].pFreq)* (1.0 + TimeStrength[k]))); // time[k] += (ClassTime * (*OSC_TML_Param_Set[k].pFreq) * 4);

		//if(oldAmplit[k] > 0.0)
			//time[k] += (ClassTime / ((*OSC_TML_Param_Set[k].pFreq)* (amplit[k]/oldAmplit[k]))); 
		//else
			//time[k] += (ClassTime / (*OSC_TML_Param_Set[k].pFreq));
			
	}
	else
		time[k] += ClassTime;

	//  Calculate Synch in time advance
	/*
	realTime[k]+= ClassTime;

	if((*OSC_TML_Param_Set[k].pFreq) > 0.0)
		oscTime[k] = (*OSC_TML_Param_Set[k].pFreq);
	else
		oscTime[k] = 1.0;


	oscTime[k] -= TimeOffset;

	if(Invert[k])
		dwellTime[k] = (*OSC_TML_Param_Set[k].pDwell_In) + oscTime[k];
	else
		dwellTime[k] = (*OSC_TML_Param_Set[k].pDwell_Out)+ oscTime[k];

	if((realTime[k]>= oscTime[k]) && (realTime[k]<= dwellTime[k]))
		SynchClckFromOsc = 0;
	else
		SynchClckFromOsc = 1;
	*/
	//////////////////////////////////////////////

	cam->x = time[k];	
	AzzLinSpline(cam);
	
	 if(Invert[k])
		 Pos[k] = Dist[k] - cam->y * (amplit[k]) ;
	else
		 Pos[k] = Dist[k] + cam->y * (amplit[k]); 


	if(time[k] >= 1.0 ){


		prc->subStatus = 1;	

		if(Invert[k])
			SynchClckFromOsc = 1;
		else
			SynchClckFromOsc = 2;

		if((Invert[k] && (time[k] >= (1.0 + (*OSC_TML_Param_Set[k].pDwell_Out))))
			|| (!Invert[k] && (time[k] >= (1.0 + (*OSC_TML_Param_Set[k].pDwell_In))))){
			
			Invert[k] = !Invert[k];
			
			time[k] = 0.0;

			/* Tak Bylo bez Index
			Dist[k] = Pos[k];				
			amplit[k] = Ramp[k].y / 2; //
			d = amplit[k];		
			amplit[k] = oldAmplit[k] + amplit[k];
			oldAmplit[k] = d; 
			*/

			TimeStrength[k] = 0.0;

			amplit[k] = Ramp[k].y / 2; //						
			d = amplit[k];
						
			if(prc->fParam[2] > 0.0){
			
				if(!Invert[k]){

					amplit[k] += prc->fParam[2];

					TimeStrength[k] = prc->fParam[2] / amplit[k] * 2.0;

					Pos[k] -= prc->fParam[2];
					//StartPos[k] += prc->fParam[2];
					_StartPos[k] += prc->fParam[2];

					IndxRamp[k].y_set =_StartPos[k];
					IndxRamp[k].set = 1;
					AzzRamp(&IndxRamp[k]);
					IndxRamp[k].set = 0;
										
					prc->fParam[2] = 0.0;
				}
			}
			else if(prc->fParam[2] < 0.0){
			
				if(Invert[k]){

					amplit[k] -= prc->fParam[2];	

					TimeStrength[k] = -prc->fParam[2] / amplit[k] * 2.0;

					Pos[k] -= prc->fParam[2];
					//StartPos[k] += prc->fParam[2];
					_StartPos[k] += prc->fParam[2];

					IndxRamp[k].y_set =_StartPos[k];
					IndxRamp[k].set = 1;
					AzzRamp(&IndxRamp[k]);
					IndxRamp[k].set = 0;
						
					prc->fParam[2] = 0.0;
				}
			}
			
			
			amplit[k] += oldAmplit[k];	
			oldAmplit[k] = d; 
			Dist[k] = Pos[k];	

		}

		
	}	
	else {
		prc->subStatus = 0;
		SynchClckFromOsc = 0;

	}

	
		
return  Pos[k] + StartPos[k];

}

void ObserveOsc(StateVectorProxy_typ *  vect , int i ){
	float x1 , x2;

	float fbk;

	if(OSC_TML_Param_Set[i].PosFbkIndx == 2)
		fbk = vect->FbkVect.x2;
	else
		fbk = vect->FbkVect.x;

	x1 =(StartPos[i] +(*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) - KK_margin;
	
	x2 =(StartPos[i] -(*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) + KK_margin;

	if(fbk >= x1 )
		SynchClckFromOsc = 2;
	
	else if(fbk <= x2 )
		SynchClckFromOsc = 1;

	else
		SynchClckFromOsc = 0;
		

	return;		

}
void StartOSC( StateVectorProxy_typ*  vect , int i , float amplit){
float x;

	if((*OSC_TML_Param_Set[i].pPrimAmpl) < 1.0)
		vect->fParam[OSC_speed_par] =(1/0.5 ) *    OSC_TML_Param_Set[i].SpeedFactor;
	else
		vect->fParam[OSC_speed_par] =((*OSC_TML_Param_Set[i].pPrimAmpl)/(*OSC_TML_Param_Set[i].pFreq) ) *    OSC_TML_Param_Set[i].SpeedFactor;
	

	x =(StartPos[i] +(*OSC_TML_Param_Set[i].pPrimAmpl)/2.0);
	if(x > vect->OutVect.softLimits[1])
		x = vect->OutVect.softLimits[1];
	
	vect->Param[PosMax_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;

	x = (StartPos[i] -(*OSC_TML_Param_Set[i].pPrimAmpl)/2.0);
	if(x < vect->OutVect.softLimits[0])
		x = vect->OutVect.softLimits[0];
	
	vect->Param[PosMin_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;
	
	vect->Param[DwellIn_par] = (DINT)((*OSC_TML_Param_Set[i].pDwell_In) * 1000.0);
	vect->Param[DwellOut_par] = (DINT)((*OSC_TML_Param_Set[i].pDwell_Out) * 1000.0);

	vect->cmdWord = 1;

	vect->xCMD = VECT_OSC_Start_Cmd;



}

void StartAlligment( StateVectorProxy_typ*  vect , int i , float amplit){
float x;

	if((*OSC_TML_Param_Set[i].pPrimAmpl) < 1.0)
		vect->fParam[OSC_speed_par] =(1/0.5 ) *    OSC_TML_Param_Set[i].SpeedFactor;
	else
		vect->fParam[OSC_speed_par] =((*OSC_TML_Param_Set[i].pPrimAmpl)/(*OSC_TML_Param_Set[i].pFreq) ) *    OSC_TML_Param_Set[i].SpeedFactor;

	x =(StartPos[i] +(amplit)/2.0);
		
	vect->Param[PosMax_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;

	x = (StartPos[i] -(amplit)/2.0);	
	
	vect->Param[PosMin_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;
	
	vect->Param[DwellIn_par] = 0;
	vect->Param[DwellOut_par] = 0;

	vect->cmdWord = 1;

	vect->xCMD = VECT_OSC_Start_Cmd;



}


void StopOSC( StateVectorProxy_typ*  vect , int i){
float x;	
	
	vect->Param[PosMax_par] = (DINT)(StartPos[i]  * OSC_TML_Param_Set[i].mm2ticks)  ;
	vect->cmdWord = 0;

	if(OSC_TML_Param_Set[i].Mode == 2)		
		vect->xCMD = VECT_OSC_Stop2_Cmd;
	else
		vect->xCMD = VECT_OSC_Stop_Cmd;

}

void UpdateOSC( StateVectorProxy_typ*  vect , int i , float amplit){
float x;

	
	//vect->fParam[OSC_speed_par] =( (amplit)/(*OSC_TML_Param_Set[i].pFreq) ) *	 OSC_TML_Param_Set[i].SpeedFactor;
	//vect->fParam[OSC_speed_par] =((*OSC_TML_Param_Set[i].pPrimAmpl)/(*OSC_TML_Param_Set[i].pFreq) ) *    OSC_TML_Param_Set[i].SpeedFactor;

	if((*OSC_TML_Param_Set[i].pPrimAmpl) < 1.0)
		vect->fParam[OSC_speed_par] =(1/0.5 ) *    OSC_TML_Param_Set[i].SpeedFactor;
	else
		vect->fParam[OSC_speed_par] =((*OSC_TML_Param_Set[i].pPrimAmpl)/(*OSC_TML_Param_Set[i].pFreq) ) *    OSC_TML_Param_Set[i].SpeedFactor;
	
	x = (StartPos[i] + (amplit/2.0));
	if(x > vect->OutVect.softLimits[1])
		x = vect->OutVect.softLimits[1];
	
	vect->Param[PosMax_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;

	x = (StartPos[i] -(amplit/2.0));
	if(x < vect->OutVect.softLimits[0])
		x = vect->OutVect.softLimits[0];
	
	vect->Param[PosMin_par] = (DINT)(x * OSC_TML_Param_Set[i].mm2ticks)  ;
	
	
	vect->cmdWord = 1;

	vect->xCMD = VECT_OSC_Update_Cmd;

	updateLimits[i] = 0;



}

void CorrectStartPos(int i){

	if((StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
		StartPos[i] = vect->OutVect.softLimits[1] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0 - 0.5;
		SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
	}

	else if((StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
		StartPos[i] = vect->OutVect.softLimits[0] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0 + 0.5;
		SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
	}


	return;

}

void CorrectAmplitude(int i){

	// Software Limits consideration
	
	if(vect->OutVect.softLimits[0]	< vect->OutVect.softLimits[1] ){	


		if((*OSC_TML_Param_Set[i].pPrimAmpl) > (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]))
			(*OSC_TML_Param_Set[i].pPrimAmpl) = (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]);

		
		if((StartPos[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
			//StartPos[i] = vect->OutVect.softLimits[1] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0;
			(*OSC_TML_Param_Set[i].pPrimAmpl) = (vect->OutVect.softLimits[1] - StartPos[i])*2.0;
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
		}

		else if((StartPos[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
			//StartPos[i] = vect->OutVect.softLimits[0] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0;
			(*OSC_TML_Param_Set[i].pPrimAmpl) = ( StartPos[i] - vect->OutVect.softLimits[0])*2.0;
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
		}
						
	}

	if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
		(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;

		
	///////////////////////////////////

	return;

}

void CorrectStartPosbyPOT(int i){
float delta;

	if((StartPosbyPOT[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){

		delta = (StartPosbyPOT[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) - vect->OutVect.softLimits[1];

		_StartPos[i] -= (delta + 0.5);
		StartPosbyPOT[i] -= (delta + 0.5);

		//StartPos[i] = vect->OutVect.softLimits[1] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0 - 0.5;
		SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
	}

	else if((StartPosbyPOT[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){

		delta = vect->OutVect.softLimits[0] - ((StartPosbyPOT[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2));

		_StartPos[i] += (delta + 0.5);
		StartPosbyPOT[i] += (delta + 0.5);
		
		//StartPos[i] = vect->OutVect.softLimits[0] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0 + 0.5;
		SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
	}


	return;

}

void CorrectAmplitudebyPOT(int i){


	// Software Limits consideration
	
	if(vect->OutVect.softLimits[0]	< vect->OutVect.softLimits[1] ){	


		if((*OSC_TML_Param_Set[i].pPrimAmpl) > (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]))
			(*OSC_TML_Param_Set[i].pPrimAmpl) = (vect->OutVect.softLimits[1] - vect->OutVect.softLimits[0]);

		
		if((StartPosbyPOT[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0) > vect->OutVect.softLimits[1]){
			//StartPos[i] = vect->OutVect.softLimits[1] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0;
			(*OSC_TML_Param_Set[i].pPrimAmpl) = (vect->OutVect.softLimits[1] - StartPosbyPOT[i])*2.0;
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);			
		}

		else if((StartPosbyPOT[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
			//StartPos[i] = vect->OutVect.softLimits[0] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2.0;
			(*OSC_TML_Param_Set[i].pPrimAmpl) = ( StartPosbyPOT[i] - vect->OutVect.softLimits[0])*2.0;
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
		}
						
	}

	if((*OSC_TML_Param_Set[i].pPrimAmpl) < 0.0)
		(*OSC_TML_Param_Set[i].pPrimAmpl) = 0.0;

		
	///////////////////////////////////

	return;

}


void MoveStartPosbyPot(int i ,float delta){

	StartPosbyPOT[i] += delta;

				
	if(delta > 0){
	
		if((StartPosbyPOT[i] + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
			StartPosbyPOT[i] -= delta;

		}
		else
			_StartPos[i] =  IndxRamp[i].x + delta;
		
		
	}

	else if(delta < 0){

		if((StartPosbyPOT[i] - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
			StartPosbyPOT[i] -= delta;
		}
		else
			_StartPos[i] =  IndxRamp[i].x + delta;
		
	}


	return;
}

void MoveStartPosbyPot_Abs(int i ,float delta){

	if(!vect)
		return;
				
	if(delta > 0){
	
		if((vect->FbkVect.x2 + (*OSC_TML_Param_Set[i].pPrimAmpl)/2) > vect->OutVect.softLimits[1]){
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);

		}
		else
			_StartPos[i] =  IndxRamp[i].x + delta;
		
		
	}

	else if(delta < 0){

		if((vect->FbkVect.x2  - (*OSC_TML_Param_Set[i].pPrimAmpl)/2) < vect->OutVect.softLimits[0]){
			SetBit(&ErrorMgr.warnings , WARN_OSC_LIMITS);
		}
		else
			_StartPos[i] =  IndxRamp[i].x + delta;
		
	}



	return;
}


void CalculateCompOffset(int i){

	switch(CompOffset_Step[i]){

		case 0:

			
	
			OffsetPlus_Sum[i] = 0.0;
			OffsetPlus_Indx[i] = 0;
			OffsetPlus_Avr[i] = 0.0;

			OffsetMinus_Sum[i] = 0.0;
			OffsetMinus_Indx[i] = 0;
			OffsetMinus_Avr[i] = 0.0;

			CompOffset[i] = 0.0;
			CompRamp[i].x = CompOffset[i];
	
			CompRamp[i].y_set = CompRamp[i].x;
			CompRamp[i].set = 1;
			AzzRamp(&CompRamp[i]);
			CompRamp[i].set = 0;	
			
			CompOffset_Step[i]++;

		break;


		case 1:

			if((prc->Status == prcsSTATUS_Run)){

				
				CloseLoopTMR[i] = 0.0;
				CompOffset_Step[i] ++;

			}


		break;


		case 2:

			CloseLoopTMR[i] += 0.0024;

			if(CloseLoopTMR[i] > CloseLoopDelay)
				CompOffset_Step[i]++;

		break;

		case 3:

			if((prc->Status == prcsSTATUS_Run)){

				UP_Limit[i]=StartPosbyPOT[i] + ((*OSC_TML_Param_Set[i].pPrimAmpl)/2.0);
				DOWN_Limit[i]=StartPosbyPOT[i] - ((*OSC_TML_Param_Set[i].pPrimAmpl)/2.0);

				//if(vect->FbkVect.x2 > vect->OutVect.softLimits[1]){
				if(vect->FbkVect.x2 > UP_Limit[i]){

					OffsetPlus_Sum[i] += (vect->FbkVect.x2 - UP_Limit[i]);
					OffsetPlus_Indx[i]++;
					OffsetPlus_Avr[i] = OffsetPlus_Sum[i]/(float)OffsetPlus_Indx[i];

				}

				//if(vect->FbkVect.x2 < vect->OutVect.softLimits[0]){
				else if(vect->FbkVect.x2 < DOWN_Limit[i]){

					OffsetMinus_Sum[i] += (DOWN_Limit[i] - vect->FbkVect.x2);
					OffsetMinus_Indx[i]++;
					OffsetMinus_Avr[i] = OffsetMinus_Sum[i]/(float)OffsetMinus_Indx[i];

				}

				//if((OffsetPlus_Indx[i] > 100) || (OffsetMinus_Indx[i] > 100))
				if((OffsetMinus_Avr[i] > ErrorMargen) || (OffsetPlus_Avr[i] > ErrorMargen) ){
							
						
					if(OffsetMinus_Avr[i] > OffsetPlus_Avr[i])
						CompOffset[i] += OffsetMinus_Avr[i];
					else
						CompOffset[i] -= OffsetPlus_Avr[i];
					
				
					CompOffset_Step[i]++;
				}	

			}
			else {

				OffsetPlus_Sum[i] = 0.0;
				OffsetPlus_Indx[i] = 0;
				OffsetPlus_Avr[i] = 0.0;

				OffsetMinus_Sum[i] = 0.0;
				OffsetMinus_Indx[i] = 0;
				OffsetMinus_Avr[i] = 0.0;

				CompOffset_Step[i] = 1;
			}
		

		break;


		case 4:			

			OffsetPlus_Sum[i] = 0.0;
			OffsetPlus_Indx[i] = 0;
			OffsetPlus_Avr[i] = 0.0;

			OffsetMinus_Sum[i] = 0.0;
			OffsetMinus_Indx[i] = 0;
			OffsetMinus_Avr[i] = 0.0;

			CloseLoopTMR[i] = 0.0;
			CompOffset_Step[i] ++;

			


		break;


		case 5: //wait

			CloseLoopTMR[i] += 0.0024;

			if(CloseLoopTMR[i] > CloseLoopTime)
				CompOffset_Step[i] = 3;

		break;


	}
	CompRamp[i].x = CompOffset[i];
	AzzRamp(&CompRamp[i]);

}



