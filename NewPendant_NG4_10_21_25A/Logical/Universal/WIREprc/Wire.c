/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: Wire
 * File: Wire.c
 * Author: ckaczkowski
 * Created: November 25, 2013

  * Warning when prcsStart Param[0] is set to 0 automaticly	
  	Param[0] == 1 - skip start, stop delay & retract delay 
  	Param[0] == 2 - skip start, stop delay & retract delay , Use Cont_Ramp 
  	Param[0] == 3 - Stop with zero ramp but with retract
  	Param[0] == 4 - stop with ramp but NO retract
 ********************************************************************
 * Implementation of program Wire
 ********************************************************************/


#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>
#include <..\..\Lib\LoggerLib.c>



_LOCAL TON_typ Tmr[WIRE_PULS_NUM + WIRE_FEED_NUM];
GenProcess_typ* prc;
StateVectorProxy_typ*  vect;
  
WIRE_Param_typ param[WIRE_PULS_NUM + WIRE_FEED_NUM];
_LOCAL AzzRamp_typ Ramp[WIRE_PULS_NUM + WIRE_FEED_NUM];

float _SpeedSp[WIRE_PULS_NUM + WIRE_FEED_NUM]; 
UINT calibType , calibTypeInv ;

//_LOCAL DINT PosLatch[WIRE_PULS_NUM + WIRE_FEED_NUM] , GlobPosLatch[WIRE_PULS_NUM + WIRE_FEED_NUM]; 
_LOCAL float fPosLatch[WIRE_PULS_NUM + WIRE_FEED_NUM] , fGlobPosLatch[WIRE_PULS_NUM + WIRE_FEED_NUM]; 


//_LOCAL DINT  Dest[WIRE_PULS_NUM + WIRE_FEED_NUM] , Dif[WIRE_PULS_NUM + WIRE_FEED_NUM];
_LOCAL float fDest[WIRE_PULS_NUM + WIRE_FEED_NUM] , fDif[WIRE_PULS_NUM + WIRE_FEED_NUM];
_LOCAL float VoltRampDown;
_LOCAL float SpeedSp , SpeedBoost1 , SpeedBoost2;
//_LOCAL float KK_Contin_WF_RampDwn,KK_Contin_WF_RampUp;
//_LOCAL float KK_Contin_WP_RampDwn,KK_Contin_WP_RampUp;


//_LOCAL plcbit _quickStop[WIRE_PULS_NUM + WIRE_FEED_NUM] , _quickStart[WIRE_PULS_NUM + WIRE_FEED_NUM];


_LOCAL float KK_Retract_Catch[WIRE_PULS_NUM + WIRE_FEED_NUM];

_LOCAL DINT _AuxMtrMode;

_LOCAL UINT _HeadID[4];

StateVectorProxy_typ *pVect;


void WIRE_MODE_BR_DC(void);
void WIRE_MODE_TML_ENC(void);
void WIRE_MODE_TML_DC(void);

void WIRE_MODE_TML_Fdbk_Sel(USINT select);




void _INIT WireINIT(void)
{
int i;

	 VoltRampDown = 0.5;
	 
	Process[0].Disable = Process[0].Disable;
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;

	for(i=0 ; i < (WIRE_PULS_NUM + WIRE_FEED_NUM); i++){
	
		Ramp[i].y_max = 1000.0;
		Ramp[i].y_min = 0;	
		Ramp[i].enable = 1;
			
	}
	

	
}


void _CYCLIC WireCYCLIC(void){


	if(!WIRE_Param_Set[0].pPrc)
		return;


	if(WIRE_Param_Set[0].pPrc->ConfigReady <= 0)
		return;


	//KK_Contin_WF_RampDwn = (*WIRE_Param_Set[0].pContinRampDwn);
	//KK_Contin_WF_RampUp = (*WIRE_Param_Set[0].pContinRampUp);
	
	//KK_Contin_WP_RampDwn = (*WIRE_Param_Set[1].pContinRampDwn);
	//KK_Contin_WP_RampUp = (*WIRE_Param_Set[1].pContinRampUp);

	if(WIRE_Param_Set[0].pAuxMtrMode)
		_AuxMtrMode = (*WIRE_Param_Set[0].pAuxMtrMode);

	switch(WIRE_Param_Set[0].Mode){


		case 1:

			WIRE_MODE_TML_ENC();

		break;

		case 2:

			WIRE_MODE_TML_DC();

		break;

		case 3:

			// WF motor encoder switch
			if(_HeadID[0] != Master[0].ToolHeadID){

				if(WIRE_Param_Set[0].pPrc){
					WIRE_Param_Set[0].pPrc->_step = 0;
					_HeadID[0] = Master[0].ToolHeadID;

				}
			}
			// WP motor encoder switch
			if(_HeadID[1] != Master[0].ToolHeadID){

				if(WIRE_Param_Set[1].pPrc){
					WIRE_Param_Set[1].pPrc->_step = 0;
					_HeadID[1] = Master[0].ToolHeadID;

				}
			}
				

			if(Master[0].ToolHeadID)				
				WIRE_MODE_TML_Fdbk_Sel(1);
			else
				WIRE_MODE_TML_Fdbk_Sel(0);

		break;


		default:

			WIRE_MODE_BR_DC(); 

		break;



	}	


}	

void WIRE_MODE_TML_ENC(void)
{
int i;
float y;
UINT synchClck;

		
	
		for(i=0 ; i < WIRE_PULS_NUM + WIRE_FEED_NUM ; i++){

			prc = WIRE_Param_Set[i].pPrc;
			
			if(!prc)
				break;
			
			vect = WIRE_Param_Set[i].pVect;
			
			if(!vect)
				break;

			if(prc->ConfigReady <= 0)
				break;
					
			
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
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


					case prcsHalt:
					case prcsRESTART:

						prc->_step = 0;
						
					break;	
		
						
					case prcsSTART:


						if((prc->_step == 0) || (prc->_step > 5)){

#ifdef EXTENDED_LOGS

							if(i == 0)
					       		LogTxt(&LogMgr, "Cmd:Wire Start: %.2f" , ABS_TVL_Pos);
#endif			
							prc->Param[0] = 0; // Full delay and ramp - just in case Param[0] is set somewhere by mistake
							prc->_step = 1;
						}	

					break;

					case prcsQuickStart:

						if((prc->_step == 0) || (prc->_step > 5)){

#ifdef EXTENDED_LOGS

							if(i == 0)
					       		LogTxt(&LogMgr, "Cmd:Wire QStart: %.2f" , ABS_TVL_Pos);
#endif								
							
							prc->_step = 1;

						}

					break;
		
					case prcsJump2Run:
						
						_SpeedSp[i] = (SpeedSp);
						Ramp[i].x = 1.0;
						Ramp[i].y_set = 1.0;
						Ramp[i].set = 1;
						//vect->Mode = vectSt_Mode_Vel;					
						
						fGlobPosLatch[i] = vect->FbkVect.x;

#ifdef EXTENDED_LOGS

					if(i == 0)
			       		LogTxt(&LogMgr, "Cmd:Wire JStart: %.2f" , ABS_TVL_Pos);
#endif	

						vect->xCMD = VECT_Refresh_Mode;
						vect->Mode = vectSt_Mode_Vel;

						prc->Param[0] = 0; 
					
						prc->_step = 5;

					break;

					case prcsCalib:

						fGlobPosLatch[i] = vect->FbkVect.x;
						
						if(prc->_step != 70){

							if( prc->fParam[2] > 0)
								TimerStart(&Tmr[i], prc->fParam[2]);
							else
								Tmr[i].IN = 0;
							
							prc->_step = 70;
						}

						

					break;	

					case prcsRetract:

						if(WIRE_Param_Set[i].pRetracDist)  
							prc->_step = 25;


					break;			
					

					case prcsSTOP:
					case prcsQuickSTOP:	

						//_quickStop[i] = 0;	

						if((prc->_step != 0) && (prc->_step <= 5) ){

							switch(prc->xCMD){

								case prcsSTOP:

									prc->_step = 20;

								break;
								
								case prcsQuickSTOP:

									//_quickStop[i] = 1;		

									if(WIRE_Param_Set[i].pRetracDist 
										&& WIRE_Param_Set[i].pRetracSpeed){
										
										prc->_step = 22;
										//prc->_step = 25; // Wazne tak bylo przed 17 Dec 19
									}
									else{									

										TimerStart(&Tmr[i], 0.01);
										prc->_step = 22;

									}
									
								break;	

							}


						}	

					break;	
					
					case prcsJOGpos:

						if(prc->_step != 50)
							prc->_step = 50;
		
					break;

					case prcsDirectCntrl:
						
						if(prc->_step != 60)
							prc->_step = 60;
		
					break;

					

				break;

					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
			if(WIRE_Param_Set[i].pSynchMode){
			
				if((*WIRE_Param_Set[i].pSynchMode) == 0) // Straight
					SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;
				else
					SpeedSp =  *WIRE_Param_Set[i].pBkgSpeedSp;	

			}
			else
				SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;


			SpeedBoost1 = (*WIRE_Param_Set[i].pBoost1SP);
			SpeedBoost2 = (*WIRE_Param_Set[i].pBoost2SP);

			/*
			switch(WIRE_Param_Set[i].BoostCmd){

				case 1:

					SpeedSp += SpeedBoost1;

				break;


				case 2:

					SpeedSp += SpeedBoost2;

				break;


				default:

				break;

				
			}

			if(SpeedSp < 0.0)
				SpeedSp = 0.0;
			*/
			/////////////////////////
			
			
			if(WIRE_Param_Set[i].pCalibPoints){

				if(prc->Param[1] > 0){// travel direction means different head			
					calibType 		= CALIB_2POINTS;
					calibTypeInv 	= CALIB_2POINTS_INV;
				}				
				else {
					calibType 		= CALIB_2POINTS_4OFF;
					calibTypeInv 	= CALIB_2POINTS_4OFF_INV;
				}
			
				fDif[i]= fabsf(vect->FbkVect.x - fGlobPosLatch[i]);				
				prc->Fbk[4] = getFromCalib(fDif[i], WIRE_Param_Set[i].pCalibPoints , calibTypeInv); // curDistance	

				if(vect->FbkVect.vel > 0)
					prc->Fbk[0] = getFromCalib(vect->FbkVect.vel, WIRE_Param_Set[i].pCalibPoints , calibTypeInv);// cur Vel
				else
					prc->Fbk[0] = 0;

				//prc->Fbk[0] = vect->FbkVect.vel;
				

				

			}	


			
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					if(prc->Status != prcsSTATUS_Fault)
						prc->Status = prcsSTATUS_Stopped;

					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

					//vect->Mode = vectSt_Mode_Idle;
					vect->InVal = 0;

												
				break;
	
	
				case 1: // start	
					

					vect->xCMD = VECT_Refresh_Mode;
					vect->Mode = vectSt_Mode_Vel;

																					
					fGlobPosLatch[i] = vect->FbkVect.x;
									
					if(((*WIRE_Param_Set[i].pStartDly) > 0)
						&& (prc->Param[0] == 0)){
						TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	
					//_quickStart[i] = 0;

					prc->Param[0] = 0;
					
				break;
	
	
				case 2:

					prc->Status = prcsSTATUS_StartDly;
					
					if(TimerIsTimeOut(&Tmr[i])){

						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						prc->_step = 3;
							
					}	
						
				break;	
				
	
				case 3:

					vect->InVal = 0;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						break;
					}
					
					prc->Status = prcsSTATUS_RampUp;


					switch(prc->Param[0]){

						case 2:

							if ((*WIRE_Param_Set[i].pContinRampUp) > 0){ 
								Ramp[i].dy_up = fabsf(1.0)/(*WIRE_Param_Set[i].pContinRampUp);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pContinRampUp + 1.0);
							}	
							else {
								Ramp[i].dy_up = 3.4e+38;
								TimerStart(&Tmr[i], 1.0);

							}
						

						break;

						default:

							if ((*WIRE_Param_Set[i].pRampUp) > 0){ 
								Ramp[i].dy_up = fabsf(1.0)/(*WIRE_Param_Set[i].pRampUp);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp + 1.0);
							}								
							else{
								Ramp[i].dy_up = 3.4e+38;	
								TimerStart(&Tmr[i], 1.0);

							}


						break;

					}	

					prc->Param[0] = 0;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1;
					
					//TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp);

#ifdef EXTENDED_LOGS

					if(i == 0)
			       		LogTxt(&LogMgr, "P: Wire Start: %.2f" , ABS_TVL_Pos);
#endif					

					prc->_step = 4;		
					
					
				break;	
	
	
	
				case 4:			

					prc->Param[0] = 0; 
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_SpeedSp[i] = (SpeedSp);						
						prc->_step = 5;

					}	
										
	
				break;
	
	
	
				case 5:

					prc->Status = prcsSTATUS_Run;

									
				break;
	
	
	
				case 20:
	
					prc->Status = prcsSTATUS_StopDly;

					//_quickStop[i] = 0;
					
					if((*WIRE_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], *WIRE_Param_Set[i].pStopDly);
						prc->_step++;
					}
					else{					
						TimerStart(&Tmr[i], 0.01);
						prc->_step++;
					}
	
					
	
	
	
				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
						
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;


					switch(prc->Param[0]){

						case 2:

							if ((*WIRE_Param_Set[i].pContinRampDwn) > 0){ 
								Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pContinRampDwn);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pContinRampDwn + 1.0);								
							}	
							else {
								Ramp[i].dy_down = 3.4e+38;
								TimerStart(&Tmr[i], 1.0);

							}
						

						break;


						case 3: 
							

							Ramp[i].dy_down = 3.4e+38;
							TimerStart(&Tmr[i], 0.5);						

						break;

						default:

							if ((*WIRE_Param_Set[i].pRampDwn) > 0){ 
								Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pRampDwn);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn + 1.0);								
							}								
							else{
								Ramp[i].dy_down = 3.4e+38;	
								TimerStart(&Tmr[i], 1.0);

							}


						break;

					}	
										
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;			

					//TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn);
				
					prc->_step ++; 
	
					
	
				break;
	
	
	
				case 23:

										
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

#ifdef EXTENDED_LOGS

						if(i == 0)
			       			LogTxt(&LogMgr, "P: Wire Stop: %.2f" , ABS_TVL_Pos);
#endif		


						if(WIRE_Param_Set[i].pRetracDly && WIRE_Param_Set[i].pRetracSpeed && WIRE_Param_Set[i].pRetracDist
							&& (prc->Param[0] < 0x4) ){ // (prc->Param[0] == 0x4) - no ramp even with WF

													
							if(prc->Param[0] == 0){
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRetracDly);
								prc->_step ++; 
							}
							else{
								
								prc->_step = 25; 

							}

						}
						else
							prc->_step = 0; 

						prc->Param[0] = 0;
					}	

					
	
				break;


				case 24:

					prc->Status = prcsSTATUS_ReturnDly;

					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;


				break;	
				



				case 25:
					
					if((*WIRE_Param_Set[i].pRetracDist) > 0){	

						TimerStart(&Tmr[i],  0.25);
						prc->_step ++;
					}
					else
						prc->_step = 0;

				break;	
				
				case 26:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;
						

					vect->fParam[0] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog; // ACC
					vect->fParam[1] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog;
					vect->fParam[2] = (*WIRE_Param_Set[i].pRetracDist) * (-1.0);

					//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

					
					vect->xCMD = VECT_Move_Rel_Cmd;	

#ifdef EXTENDED_LOGS
			       	LogTxt(&LogMgr, "P: Wire Ret: %.2f" , ABS_TVL_Pos);
#endif							
						
					fPosLatch[i] = vect->FbkVect.x;
			
					//TimerStart(&Tmr[i], 5.0);
					TimerStart(&Tmr[i], 0.5);
					prc->_step ++;
							
				break;

				case 27:

					prc->Status = prcsSTATUS_Retract;
					
					if(TimerIsTimeOut(&Tmr[i])){	

						//prc->Status = prcsSTATUS_Retract;
						
						KK_Retract_Catch[i] = fPosLatch[i] - vect->FbkVect.x; 						
						prc->_step = 0;

						
					}
	
				break;

				case 50: // Jogging

					prc->Status = prcsSTATUS_Jog;

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *WIRE_Param_Set[i].pJogSpeed * prc->fParam[0];
					else{
						prc->Status = prcsSTATUS_Stopped;
						Ramp[i].x = 0.0;
					}

				break;	
				

				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else{

						if(WIRE_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0){
								
								vect->InVal = (*WIRE_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
								
							}	
							else {
								
								prc->_step ++;
								//prc->_step = 0;

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
						prc->_step = 0;
			
				break;

				


				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					prc->Status = prcsSTATUS_ForceCalib;

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
											
				break;
					
//////////////////////////////////////
// This is for testing purpose only 
				case 100: // nic

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					else{

						TimerStart(&Tmr[i], 0.1);
						prc->_step = 101;
					}	
					
				break;


				case 101:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

						
					if(vect->FbkVect.Mode != vectSt_Mode_Hold){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Hold;	

					}else{

						TimerStart(&Tmr[i], 1.0);
						prc->_step = 102;

					}
			

				break;

				case 102: 


					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					else{

						TimerStart(&Tmr[i], 1.0);
						prc->_step = 101;
					}
						

				break;

////////////////////////////////////////////////////////////
				
				case 0xFF:  // Disable

					if(vect->FbkVect.Mode != vectSt_Mode_Idle)
						vect->Mode = vectSt_Mode_Idle;

					if(!prc->Disable)
						prc->_step = 0;
				

				break;	
	
				
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);
			Ramp[i].set = 0;

					
			if(vect->Mode == vectSt_Mode_Vel){


				if(prc->_step == 0)
					vect->InVal = 0.0;
				
				else if(prc->_step == 70){ // Force directly speed

					vect->InVal =  prc->fParam[1];
					
					//prc->Fbk[1] = vect->InVal ; // vel Commanded

					
				}else if(prc->_step == 2){ // Start delay

					vect->InVal =  0.0;
					
					//prc->Fbk[1] = 0.0 ; 
				}
				
				else{

					if(WIRE_Param_Set[i].pSynchClock)
						synchClck = (*WIRE_Param_Set[i].pSynchClock);
					else
						synchClck = 0;


					switch(synchClck){

						default:

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


						case 1:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedSp )/ ( SpeedSp);

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedFinal));


						break;

						case 2:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedInSp )/ ( SpeedSp);
							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;

						
						case 3:
							
							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedOutSp )/ ( SpeedSp);

							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


					}

										
					//prc->Fbk[1] = y ; // vel Commanded
					//prc->Fbk[1] = SpeedSp;


					switch(WIRE_Param_Set[i].BoostCmd){

						case 1:

							y += SpeedBoost1;

						break;


						case 2:

							y += SpeedBoost2;

						break;


						default:

						break;

						
					}

					if(y < 0.0)
						y = 0.0;
					
					vect->InVal = getFromCalib(y, WIRE_Param_Set[i].pCalibPoints, calibType);

					  if(Ramp[i].y <= 0)
						vect->InVal = 0.0;

				}

				
			}
			else{				
					
				//prc->Fbk[1] = 0 ;  // vel Commanded

			}	

			

			// Na wyrazne rzadanie zmienilem
			/*
			if(prc->Status == prcsSTATUS_Stopped){
				prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (SpeedSp) ; 
			}
			else
				prc->Fbk[1] = prc->Fbk[0];
			*/
			prc->Fbk[1] = (SpeedSp) ; 	
			
			
		}
	
	
	
	
}	

	
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void  WIRE_MODE_BR_DC(void)
{
int i;
float y;
UINT synchClck;


	
		for(i=0 ; i < WIRE_PULS_NUM + WIRE_FEED_NUM ; i++){

			prc = WIRE_Param_Set[i].pPrc;
			
			if(!prc)
				break;
			
			vect = WIRE_Param_Set[i].pVect;
			
			if(!vect)
				break;

			if(prc->ConfigReady <= 0)
				break;
					
			
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){


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


					case prcsHalt:
					case prcsRESTART:

						prc->_step = 0;
						
					break;	
		
						
					case prcsSTART:

					if((prc->_step == 0) || (prc->_step > 5))
						prc->_step = 1;

					break;
		
					case prcsJump2Run:
						
						_SpeedSp[i] = (SpeedSp);
						Ramp[i].x = 1.0;
						Ramp[i].y_set = 1.0;
						Ramp[i].set = 1;
						vect->Mode = vectSt_Mode_Vel;

						fGlobPosLatch[i] = vect->FbkVect.x;

						prc->_step = 5;

					break;

					case prcsCalib:

						fGlobPosLatch[i] = vect->FbkVect.x;
						
						if(prc->_step != 70){

							if( prc->fParam[2] > 0)
								TimerStart(&Tmr[i], prc->fParam[2]);
							else
								Tmr[i].IN = 0;
							
							prc->_step = 70;
						}

						

					break;	

					case prcsRetract:

						if(WIRE_Param_Set[i].pRetracDist)  
							prc->_step = 25;


					break;			
					

					case prcsSTOP:
					case prcsQuickSTOP:	

						if((prc->_step != 0) && (prc->_step <= 5) ){

							switch(prc->xCMD){

								case prcsSTOP:

									prc->_step = 20;

								break;
								
								case prcsQuickSTOP:

									if(WIRE_Param_Set[i].pRetracDist 
										&& WIRE_Param_Set[i].pRetracSpeed)
										prc->_step = 25;
									
									else{									

										TimerStart(&Tmr[i], 0.01);
										prc->_step = 21;

									}
									
								break;	

							}


						}	

					break;	
					
					case prcsJOGpos:

						if(prc->_step != 50)
							prc->_step = 50;
		
					break;

					case prcsDirectCntrl:
						
						if(prc->_step != 60)
							prc->_step = 60;
		
					break;

					

				break;

					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			if(WIRE_Param_Set[i].pSynchMode){
			
				if((*WIRE_Param_Set[i].pSynchMode) == 0) // Straight
					SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;
				else
					SpeedSp =  *WIRE_Param_Set[i].pBkgSpeedSp;	

			}
			else
				SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;

			SpeedBoost1 = (*WIRE_Param_Set[i].pBoost1SP);
			SpeedBoost2 = (*WIRE_Param_Set[i].pBoost2SP);

			/*
			switch(WIRE_Param_Set[i].BoostCmd){

				case 1:

					SpeedSp += SpeedBoost1;

				break;


				case 2:

					SpeedSp += SpeedBoost2;

				break;


				default:

				break;

				
			}	

			if(SpeedSp < 0.0)
				SpeedSp = 0.0;

			*/	

			/////////////////////////
			

			if(WIRE_Param_Set[i].pCalibPoints){

				if(prc->Param[1] > 0){// travel direction means different head			
					calibType 		= CALIB_2POINTS;
					calibTypeInv 	= CALIB_2POINTS_INV;
				}				
				else {
					calibType 		= CALIB_2POINTS_4OFF;
					calibTypeInv 	= CALIB_2POINTS_4OFF_INV;
				}
			
				fDif[i]= fabsf(vect->FbkVect.x - fGlobPosLatch[i]);				
				prc->Fbk[4] = getFromCalib(fDif[i], WIRE_Param_Set[i].pCalibPoints , calibTypeInv); // curDistance	

				if(vect->FbkVect.vel > 0)
					prc->Fbk[0] = getFromCalib(vect->FbkVect.vel, WIRE_Param_Set[i].pCalibPoints , calibTypeInv);// cur Vel
				else
					prc->Fbk[0] = 0;

				//prc->Fbk[0] = vect->FbkVect.vel;
				

				

			}	
				
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					if(prc->Status != prcsSTATUS_Fault)
						prc->Status = prcsSTATUS_Stopped;

					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

					vect->Mode = vectSt_Mode_Volt;
					vect->InVal = 0;
	
								
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;

					
					
					vect->InVal = 0;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						break;
					}

					
					fGlobPosLatch[i] = vect->FbkVect.x;
									
					if((*WIRE_Param_Set[i].pStartDly) > 0){
						TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	
					
				break;
	
	
				case 2:
											
					if(TimerIsTimeOut(&Tmr[i])){

						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						prc->_step = 3;
						
					}	
						
				break;	
				
	
				case 3:
					
					prc->Status = prcsSTATUS_RampUp;
	
									
					if ((*WIRE_Param_Set[i].pRampUp) > 0) 
						Ramp[i].dy_up = fabsf(1.0/(*WIRE_Param_Set[i].pRampUp));
					else 
						Ramp[i].dy_up = 3.4e+38;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1;

					TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp);

					prc->_step = 4;		
					
					
				break;	
	
	
	
				case 4:			
					
					

					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_SpeedSp[i] = (SpeedSp);
						prc->_step = 5;

					}						
						
										
	
				break;
	
	
	
				case 5:
	
					prc->Status = prcsSTATUS_Run;

										
				break;
	
	
	
				case 20:
	
					prc->Status = prcsSTATUS_StopDly;
					
					if((*WIRE_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], *WIRE_Param_Set[i].pStopDly);
						prc->_step++;
					}
					else{					
						TimerStart(&Tmr[i], 0.01);
						prc->_step++;
					}
	
					
	
	
	
				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
						
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;
					
					if ((*WIRE_Param_Set[i].pRampDwn) > 0) 
						Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pRampDwn);
					else 
						Ramp[i].dy_down = 3.4e+38;
						
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn);
				
					prc->_step ++; 
	
					
	
				break;
	
	
	
				case 23:

										
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){


						if(WIRE_Param_Set[i].pRetracDly && WIRE_Param_Set[i].pRetracSpeed && WIRE_Param_Set[i].pRetracDist){

							TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRetracDly);

							prc->_step ++; 

						}
						else
							prc->_step = 0; 
					}	
	
	
				break;


				case 24:

					prc->Status = prcsSTATUS_ReturnDly;

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Volt;
					}	

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;


				break;	
				



				case 25:
					

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
							vect->InVal = 0;
							vect->Mode = vectSt_Mode_Volt;
					}	
					else {

						if((*WIRE_Param_Set[i].pRetracDist) > 0){
							vect->InVal = -24.0 * (*WIRE_Param_Set[i].pRetracSpeed);

							
							fDest[i] = (*WIRE_Param_Set[i].pRetracDist) ;
							fPosLatch[i] = vect->FbkVect.x;

							prc->Status = prcsSTATUS_Retract;

							prc->_step ++;
						}
						else
							prc->_step = 0;

					}
						


				break;	
				
				case 26:
					
				
					if(fabsf(fPosLatch[i] - vect->FbkVect.x) > fDest[i]){
						vect->InVal = 0;
						prc->_step = 0;

					}		
	
	
				break;

				case 50: // Jogging

					prc->Status = prcsSTATUS_Jog;

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *WIRE_Param_Set[i].pJogSpeed * prc->fParam[0];
					else{
						prc->Status = prcsSTATUS_Stopped;
						Ramp[i].x = 0.0;
					}

				break;	
				

				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else{

						if(WIRE_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0){
								
								vect->InVal = (*WIRE_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;

							}	
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
						prc->_step = 0;
			
				break;

				


				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					prc->Status = prcsSTATUS_ForceCalib;

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
											
				break;


				
				case 0xFF:  // Disable

					if(vect->FbkVect.Mode != vectSt_Mode_Idle)
						vect->Mode = vectSt_Mode_Idle;

					if(!prc->Disable)
						prc->_step = 0;
				

				break;	
	
				
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);
			Ramp[i].set = 0;

					
			if(vect->Mode == vectSt_Mode_Vel){


				if(prc->_step == 0)
					vect->InVal = 0.0;
				
				else if(prc->_step == 70){ // Force directly speed

					vect->InVal =  prc->fParam[1];
					
					//prc->Fbk[1] = vect->InVal ; // vel Commanded

					
				}else if(prc->_step == 2){ // Start delay

					vect->InVal =  0.0;
					
					//prc->Fbk[1] = 0.0 ; 
				}
				
				else{

					if(WIRE_Param_Set[i].pSynchClock)
						synchClck = (*WIRE_Param_Set[i].pSynchClock);
					else
						synchClck = 0;


					switch(synchClck){

						default:

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


						case 1:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedSp )/ ( SpeedSp);

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedFinal));


						break;

						case 2:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedInSp )/ ( SpeedSp);
							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;

						
						case 3:
							
							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedOutSp )/ ( SpeedSp);

							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


					}

					switch(WIRE_Param_Set[i].BoostCmd){

						case 1:

							y += SpeedBoost1;

						break;


						case 2:

							y += SpeedBoost2;

						break;


						default:

						break;


					}

					if(y < 0.0)
						y = 0.0;

										
					//prc->Fbk[1] = y ; // vel Commanded
					//prc->Fbk[1] = SpeedSp;
					
					vect->InVal = getFromCalib(y, WIRE_Param_Set[i].pCalibPoints, calibType);

					  if(Ramp[i].y <= 0)
						vect->InVal = 0.0;

				}

				
			}
			else{				
					
				prc->Fbk[1] = 0 ;  // vel Commanded

			}	

			

			// Na wyrazne rzadanie zmienilem
			/*
			if(prc->Status == prcsSTATUS_Stopped){
				prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*WIRE_Param_Set[i].pSpeedSp) ; 
			}
			else
				prc->Fbk[1] = prc->Fbk[0];
			*/
			prc->Fbk[1] = SpeedSp ; 	
			
			
		}
	
	
	
	
}

void  WIRE_MODE_TML_DC(void)
{
int i;
float y;
UINT synchClck;


	
		for(i=0 ; i < WIRE_PULS_NUM + WIRE_FEED_NUM ; i++){

			prc = WIRE_Param_Set[i].pPrc;
			
			if(!prc)
				break;
			
			vect = WIRE_Param_Set[i].pVect;
			
			if(!vect)
				break;

			if(prc->ConfigReady <= 0)
				break;
					
			
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){


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


					case prcsHalt:
					case prcsRESTART:

						prc->_step = 0;
						
					break;	
		
						
					case prcsSTART:

					if((prc->_step == 0) || (prc->_step > 5))
						prc->_step = 1;

					break;
		
					case prcsJump2Run:
						
						_SpeedSp[i] = (SpeedSp);
						Ramp[i].x = 1.0;
						Ramp[i].y_set = 1.0;
						Ramp[i].set = 1;
						vect->Mode = vectSt_Mode_Vel;

						fGlobPosLatch[i] = vect->FbkVect.x;

						prc->_step = 5;

					break;

					case prcsCalib:

						fGlobPosLatch[i] = vect->FbkVect.x;
						
						if(prc->_step != 70){

							if( prc->fParam[2] > 0)
								TimerStart(&Tmr[i], prc->fParam[2]);
							else
								Tmr[i].IN = 0;
							
							prc->_step = 70;
						}

						

					break;	

					case prcsRetract:

						if(WIRE_Param_Set[i].pRetracDist)  
							prc->_step = 25;


					break;			
					

					case prcsSTOP:
					case prcsQuickSTOP:	

						if((prc->_step != 0) && (prc->_step <= 5) ){

							switch(prc->xCMD){

								case prcsSTOP:

									prc->_step = 20;

								break;
								
								case prcsQuickSTOP:

									if(WIRE_Param_Set[i].pRetracDist 
										&& WIRE_Param_Set[i].pRetracSpeed)
										prc->_step = 25;
									
									else{									

										TimerStart(&Tmr[i], 0.01);
										prc->_step = 21;

									}
									
								break;	

							}


						}	

					break;	
					
					case prcsJOGpos:

						if(prc->_step != 50)
							prc->_step = 50;
		
					break;

					case prcsDirectCntrl:
						
						if(prc->_step != 60)
							prc->_step = 60;
		
					break;

					

				break;

					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////

			if(WIRE_Param_Set[i].pSynchMode){
			
				if((*WIRE_Param_Set[i].pSynchMode) == 0) // Straight
					SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;
				else
					SpeedSp =  *WIRE_Param_Set[i].pBkgSpeedSp;	

			}
			else
				SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;

			SpeedBoost1 = (*WIRE_Param_Set[i].pBoost1SP);
			SpeedBoost2 = (*WIRE_Param_Set[i].pBoost2SP);

			/*
			switch(WIRE_Param_Set[i].BoostCmd){

				case 1:

					SpeedSp += SpeedBoost1;

				break;


				case 2:

					SpeedSp += SpeedBoost2;

				break;


				default:

				break;

				
			}	

			if(SpeedSp < 0.0)
				SpeedSp = 0.0;

			*/	

			/////////////////////////
			

			if(WIRE_Param_Set[i].pCalibPoints){

				if(prc->Param[1] > 0){// travel direction means different head			
					calibType 		= CALIB_2POINTS;
					calibTypeInv 	= CALIB_2POINTS_INV;
				}				
				else {
					calibType 		= CALIB_2POINTS_4OFF;
					calibTypeInv 	= CALIB_2POINTS_4OFF_INV;
				}
			
				fDif[i]= fabsf(vect->FbkVect.x - fGlobPosLatch[i]);				
				prc->Fbk[4] = getFromCalib(fDif[i], WIRE_Param_Set[i].pCalibPoints , calibTypeInv); // curDistance	

				if(vect->FbkVect.vel > 0)
					prc->Fbk[0] = getFromCalib(vect->FbkVect.vel, WIRE_Param_Set[i].pCalibPoints , calibTypeInv);// cur Vel
				else
					prc->Fbk[0] = 0;

				//prc->Fbk[0] = vect->FbkVect.vel;
				

				

			}	
				
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					if(prc->Status != prcsSTATUS_Fault)
						prc->Status = prcsSTATUS_Stopped;

					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

					vect->Mode = vectSt_Mode_Volt;
					vect->InVal = 0;
	
								
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;

					
					
					vect->InVal = 0;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						break;
					}

					
					fGlobPosLatch[i] = vect->FbkVect.x;
									
					if((*WIRE_Param_Set[i].pStartDly) > 0){
						TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	
					
				break;
	
	
				case 2:
											
					if(TimerIsTimeOut(&Tmr[i])){

						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						prc->_step = 3;
						
					}	
						
				break;	
				
	
				case 3:
					
					prc->Status = prcsSTATUS_RampUp;
	
									
					if ((*WIRE_Param_Set[i].pRampUp) > 0) 
						Ramp[i].dy_up = fabsf(1.0/(*WIRE_Param_Set[i].pRampUp));
					else 
						Ramp[i].dy_up = 3.4e+38;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1;

					TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp);

					prc->_step = 4;		
					
					
				break;	
	
	
	
				case 4:			
					
					

					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_SpeedSp[i] = (SpeedSp);
						prc->_step = 5;

					}						
						
										
	
				break;
	
	
	
				case 5:
	
					prc->Status = prcsSTATUS_Run;

										
				break;
	
	
	
				case 20:
	
					prc->Status = prcsSTATUS_StopDly;
					
					if((*WIRE_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], *WIRE_Param_Set[i].pStopDly);
						prc->_step++;
					}
					else{					
						TimerStart(&Tmr[i], 0.01);
						prc->_step++;
					}
	
					
	
	
	
				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
						
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;
					
					if ((*WIRE_Param_Set[i].pRampDwn) > 0) 
						Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pRampDwn);
					else 
						Ramp[i].dy_down = 3.4e+38;
						
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;

					TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn);
				
					prc->_step ++; 
	
					
	
				break;
	
	
	
				case 23:

										
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){


						if(WIRE_Param_Set[i].pRetracDly && WIRE_Param_Set[i].pRetracSpeed && WIRE_Param_Set[i].pRetracDist){

							TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRetracDly);

							prc->_step ++; 

						}
						else
							prc->_step = 0; 
					}	
	
	
				break;


				case 24:

					prc->Status = prcsSTATUS_ReturnDly;

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Volt;
					}	

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;


				break;	
				



				case 25:
					

					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
							vect->InVal = 0;
							vect->Mode = vectSt_Mode_Volt;
					}	
					else {

						if((*WIRE_Param_Set[i].pRetracDist) > 0){
							vect->InVal = -24.0 * (*WIRE_Param_Set[i].pRetracSpeed);

							prc->Status = prcsSTATUS_Retract;

							TimerStart(&Tmr[i],  (*WIRE_Param_Set[i].pRetracDist)/5.0);

							prc->_step ++;
						}
						else
							prc->_step = 0;

					}
						


				break;	
				
				case 26:
					
				
					if(TimerIsTimeOut(&Tmr[i])){
						vect->InVal = 0;
						prc->_step = 0;

					}		
	
	
				break;

				case 50: // Jogging

					prc->Status = prcsSTATUS_Jog;

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *WIRE_Param_Set[i].pJogSpeed * prc->fParam[0];
					else{
						prc->Status = prcsSTATUS_Stopped;
						Ramp[i].x = 0.0;
					}

				break;	
				

				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else{

						if(WIRE_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0){
								
								vect->InVal = (*WIRE_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;

							}	
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
						prc->_step = 0;
			
				break;

				


				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					prc->Status = prcsSTATUS_ForceCalib;

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
											
				break;


				
				case 0xFF:  // Disable

					if(vect->FbkVect.Mode != vectSt_Mode_Idle)
						vect->Mode = vectSt_Mode_Idle;

					if(!prc->Disable)
						prc->_step = 0;
				

				break;	
	
				
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);
			Ramp[i].set = 0;

					
			if(vect->Mode == vectSt_Mode_Vel){


				if(prc->_step == 0)
					vect->InVal = 0.0;
				
				else if(prc->_step == 70){ // Force directly speed

					vect->InVal =  prc->fParam[1];
					
					//prc->Fbk[1] = vect->InVal ; // vel Commanded

					
				}else if(prc->_step == 2){ // Start delay

					vect->InVal =  0.0;
					
					//prc->Fbk[1] = 0.0 ; 
				}
				
				else{

					if(WIRE_Param_Set[i].pSynchClock)
						synchClck = (*WIRE_Param_Set[i].pSynchClock);
					else
						synchClck = 0;


					switch(synchClck){

						default:

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


						case 1:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedSp )/ ( SpeedSp);

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedFinal));


						break;

						case 2:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedInSp )/ ( SpeedSp);
							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;

						
						case 3:
							
							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedOutSp )/ ( SpeedSp);

							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


					}

					switch(WIRE_Param_Set[i].BoostCmd){

						case 1:

							y += SpeedBoost1;

						break;


						case 2:

							y += SpeedBoost2;

						break;


						default:

						break;


					}

					if(y < 0.0)
						y = 0.0;

										
					//prc->Fbk[1] = y ; // vel Commanded
					//prc->Fbk[1] = SpeedSp;
					
					vect->InVal = getFromCalib(y, WIRE_Param_Set[i].pCalibPoints, calibType);

					  if(Ramp[i].y <= 0)
						vect->InVal = 0.0;

				}

				
			}
			else{				
					
				prc->Fbk[1] = 0 ;  // vel Commanded

			}	

			

			// Na wyrazne rzadanie zmienilem
			/*
			if(prc->Status == prcsSTATUS_Stopped){
				prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*WIRE_Param_Set[i].pSpeedSp) ; 
			}
			else
				prc->Fbk[1] = prc->Fbk[0];
			*/
			prc->Fbk[1] = SpeedSp ; 	
			
			
		}
	
	
	
	
}

//////////////////
void WIRE_MODE_TML_Fdbk_Sel(USINT select)
{
int i;
float y;
UINT synchClck;

		
	
		for(i=0 ; i < WIRE_PULS_NUM + WIRE_FEED_NUM ; i++){

			prc = WIRE_Param_Set[i].pPrc;
			
			if(!prc)
				break;
			
			vect = WIRE_Param_Set[i].pVect;
			
			if(!vect)
				break;

			if(prc->ConfigReady <= 0)
				break;
					
			
			if(prc->Disable){

				prc->_step = 0xFF;

				switch(prc->xCMD){

					case prcsQuickStart:
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


					case prcsHalt:
					case prcsRESTART:

						prc->_step = 0;
						
					break;	
		
						
					case prcsSTART:


						if((prc->_step == 0) || (prc->_step > 5)){

#ifdef EXTENDED_LOGS

							if(i == 0)
					       		LogTxt(&LogMgr, "Cmd:Wire Start: %.2f" , ABS_TVL_Pos);
#endif			
							prc->Param[0] = 0; // Full delay and ramp - just in case Param[0] is set somewhere by mistake
							prc->_step = 1;
						}	

					break;

					case prcsQuickStart:

						if((prc->_step == 0) || (prc->_step > 5)){

#ifdef EXTENDED_LOGS

							if(i == 0)
					       		LogTxt(&LogMgr, "Cmd:Wire QStart: %.2f" , ABS_TVL_Pos);
#endif								
							
							prc->_step = 1;

						}

					break;
		
					case prcsJump2Run:
						
						_SpeedSp[i] = (SpeedSp);
						Ramp[i].x = 1.0;
						Ramp[i].y_set = 1.0;
						Ramp[i].set = 1;
						//vect->Mode = vectSt_Mode_Vel;					
						
						fGlobPosLatch[i] = vect->FbkVect.x;

#ifdef EXTENDED_LOGS

					if(i == 0)
			       		LogTxt(&LogMgr, "Cmd:Wire JStart: %.2f" , ABS_TVL_Pos);
#endif	

						vect->xCMD = VECT_Refresh_Mode;
						vect->Mode = vectSt_Mode_Vel;

						prc->Param[0] = 0; 
					
						prc->_step = 5;

					break;

					case prcsCalib:

						fGlobPosLatch[i] = vect->FbkVect.x;
						
						if(prc->_step != 70){

							if( prc->fParam[2] > 0)
								TimerStart(&Tmr[i], prc->fParam[2]);
							else
								Tmr[i].IN = 0;
							
							prc->_step = 70;
						}

						

					break;	

					case prcsRetract:

						if(WIRE_Param_Set[i].pRetracDist)  
							prc->_step = 25;


					break;			
					

					case prcsSTOP:
					case prcsQuickSTOP:	

						//_quickStop[i] = 0;	

						if((prc->_step != 0) && (prc->_step <= 5) ){

							switch(prc->xCMD){

								case prcsSTOP:

									prc->_step = 20;

								break;
								
								case prcsQuickSTOP:

									//_quickStop[i] = 1;		

									if(WIRE_Param_Set[i].pRetracDist 
										&& WIRE_Param_Set[i].pRetracSpeed){
										
										prc->_step = 22;
										//prc->_step = 25; // Wazne tak bylo przed 17 Dec 19
									}
									else{									

										TimerStart(&Tmr[i], 0.01);
										prc->_step = 22;

									}
									
								break;	

							}


						}	

					break;	
					
					case prcsJOGpos:

						if(prc->_step != 50)
							prc->_step = 50;
		
					break;

					case prcsDirectCntrl:
						
						if(prc->_step != 60)
							prc->_step = 60;
		
					break;

					

				break;

					//test
					case 100:
		
							
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
			if(WIRE_Param_Set[i].pSynchMode){
			
				if((*WIRE_Param_Set[i].pSynchMode) == 0) // Straight
					SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;
				else
					SpeedSp =  *WIRE_Param_Set[i].pBkgSpeedSp;	

			}
			else
				SpeedSp =  *WIRE_Param_Set[i].pSpeedSp;


			SpeedBoost1 = (*WIRE_Param_Set[i].pBoost1SP);
			SpeedBoost2 = (*WIRE_Param_Set[i].pBoost2SP);

			/*
			switch(WIRE_Param_Set[i].BoostCmd){

				case 1:

					SpeedSp += SpeedBoost1;

				break;


				case 2:

					SpeedSp += SpeedBoost2;

				break;


				default:

				break;

				
			}

			if(SpeedSp < 0.0)
				SpeedSp = 0.0;
			*/
			/////////////////////////
			
			
			if(WIRE_Param_Set[i].pCalibPoints){

				if(prc->Param[1] > 0){// travel direction means different head			
					calibType 		= CALIB_2POINTS;
					calibTypeInv 	= CALIB_2POINTS_INV;
				}				
				else {
					calibType 		= CALIB_2POINTS_4OFF;
					calibTypeInv 	= CALIB_2POINTS_4OFF_INV;
				}
			
				fDif[i]= fabsf(vect->FbkVect.x - fGlobPosLatch[i]);				
				prc->Fbk[4] = getFromCalib(fDif[i], WIRE_Param_Set[i].pCalibPoints , calibTypeInv); // curDistance	

				if(vect->FbkVect.vel > 0)
					prc->Fbk[0] = getFromCalib(vect->FbkVect.vel, WIRE_Param_Set[i].pCalibPoints , calibTypeInv);// cur Vel
				else
					prc->Fbk[0] = 0;

				//prc->Fbk[0] = vect->FbkVect.vel;
				

				

			}	


			
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped
	
					if(prc->Status != prcsSTATUS_Fault)
						prc->Status = prcsSTATUS_Stopped;

					Ramp[i].y_set = 0;
					Ramp[i].set = 1;

					vect->Mode = vectSt_Mode_Idle;
					vect->InVal = 0;

					prc->_step = 8;

												
				break;

				case 8:

					if(!(vect->xCmd_busy || vect->xCMD)){

						if(select)
							vect->xCMD = VECT_Set_Fbk_2;
						else
							vect->xCMD = VECT_Set_Fbk_1;
						
						prc->_step++;

					}
					

				break;

				case 9:

					if(!(vect->xCmd_busy || vect->xCMD)){	

						if(select)
							vect->xCMD = VECT_Set_OUT0;	
						else
							vect->xCMD = VECT_Rst_OUT0;
					
						prc->_step++;



					}


				break;

				case 10:


				break;				
	
				///////////////////////////////////
				case 1: // start	
					

					vect->xCMD = VECT_Refresh_Mode;
					vect->Mode = vectSt_Mode_Vel;

																					
					fGlobPosLatch[i] = vect->FbkVect.x;
									
					if(((*WIRE_Param_Set[i].pStartDly) > 0)
						&& (prc->Param[0] == 0)){
						TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	
					//_quickStart[i] = 0;

					prc->Param[0] = 0;
					
				break;
	
	
				case 2:

					prc->Status = prcsSTATUS_StartDly;
					
					if(TimerIsTimeOut(&Tmr[i])){

						Ramp[i].y_set = 0.0;
						Ramp[i].set = 1;
						prc->_step = 3;
							
					}	
						
				break;	
				
	
				case 3:

					vect->InVal = 0;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel){
						vect->Mode = vectSt_Mode_Vel;
						break;
					}
					
					prc->Status = prcsSTATUS_RampUp;


					switch(prc->Param[0]){

						case 2:

							if ((*WIRE_Param_Set[i].pContinRampUp) > 0){ 
								Ramp[i].dy_up = fabsf(1.0)/(*WIRE_Param_Set[i].pContinRampUp);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pContinRampUp + 1.0);
							}	
							else {
								Ramp[i].dy_up = 3.4e+38;
								TimerStart(&Tmr[i], 1.0);

							}
						

						break;

						default:

							if ((*WIRE_Param_Set[i].pRampUp) > 0){ 
								Ramp[i].dy_up = fabsf(1.0)/(*WIRE_Param_Set[i].pRampUp);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp + 1.0);
							}								
							else{
								Ramp[i].dy_up = 3.4e+38;	
								TimerStart(&Tmr[i], 1.0);

							}


						break;

					}	

					prc->Param[0] = 0;

					Ramp[i].dy_down = Ramp[i].dy_up;

					Ramp[i].x = 1;
					
					//TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampUp);

#ifdef EXTENDED_LOGS

					if(i == 0)
			       		LogTxt(&LogMgr, "P: Wire Start: %.2f" , ABS_TVL_Pos);
#endif					

					prc->_step = 4;		
					
					
				break;	
	
	
	
				case 4:			

					prc->Param[0] = 0; 
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_SpeedSp[i] = (SpeedSp);						
						prc->_step = 5;

					}	
										
	
				break;
	
	
	
				case 5:

					prc->Status = prcsSTATUS_Run;

									
				break;
	
	
	
				case 20:
	
					prc->Status = prcsSTATUS_StopDly;

					//_quickStop[i] = 0;
					
					if((*WIRE_Param_Set[i].pStopDly) > 0){
						TimerStart(&Tmr[i], *WIRE_Param_Set[i].pStopDly);
						prc->_step++;
					}
					else{					
						TimerStart(&Tmr[i], 0.01);
						prc->_step++;
					}
	
					
	
	
	
				case 21:
	
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;
						
					
	
	
				break;	
	
					
	
				case 22:
	
					prc->Status = prcsSTATUS_RampDwn;


					switch(prc->Param[0]){

						case 2:

							if ((*WIRE_Param_Set[i].pContinRampDwn) > 0){ 
								Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pContinRampDwn);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pContinRampDwn + 1.0);								
							}	
							else {
								Ramp[i].dy_down = 3.4e+38;
								TimerStart(&Tmr[i], 1.0);

							}
						

						break;


						case 3: 
							

							Ramp[i].dy_down = 3.4e+38;
							TimerStart(&Tmr[i], 0.5);						

						break;

						default:

							if ((*WIRE_Param_Set[i].pRampDwn) > 0){ 
								Ramp[i].dy_down = fabsf(1.0)/(*WIRE_Param_Set[i].pRampDwn);
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn + 1.0);								
							}								
							else{
								Ramp[i].dy_down = 3.4e+38;	
								TimerStart(&Tmr[i], 1.0);

							}


						break;

					}	
										
										
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = 0.0;			

					//TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRampDwn);
				
					prc->_step ++; 
	
					
	
				break;
	
	
	
				case 23:

										
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){

#ifdef EXTENDED_LOGS

						if(i == 0)
			       			LogTxt(&LogMgr, "P: Wire Stop: %.2f" , ABS_TVL_Pos);
#endif		


						if(WIRE_Param_Set[i].pRetracDly && WIRE_Param_Set[i].pRetracSpeed && WIRE_Param_Set[i].pRetracDist
							&& (prc->Param[0] < 0x4) ){ // (prc->Param[0] == 0x4) - no ramp even with WF

													
							if(prc->Param[0] == 0){
								TimerStart(&Tmr[i],  *WIRE_Param_Set[i].pRetracDly);
								prc->_step ++; 
							}
							else{
								
								prc->_step = 25; 

							}

						}
						else
							prc->_step = 0; 

						prc->Param[0] = 0;
					}	

					
	
				break;


				case 24:

					prc->Status = prcsSTATUS_ReturnDly;

					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step++;


				break;	
				



				case 25:
					
					if((*WIRE_Param_Set[i].pRetracDist) > 0){	

						TimerStart(&Tmr[i],  0.25);
						prc->_step ++;
					}
					else
						prc->_step = 0;

				break;	
				
				case 26:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;
						

					vect->fParam[0] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog; // ACC
					vect->fParam[1] =  (*WIRE_Param_Set[i].pRetracSpeed) * WIRE_Param_Set[i].MaxSpeedJog;
					vect->fParam[2] = (*WIRE_Param_Set[i].pRetracDist) * (-1.0);

					//vect->StatusReg = 0; Wazne bo jest assign w TML_Server

					
					vect->xCMD = VECT_Move_Rel_Cmd;	

#ifdef EXTENDED_LOGS
			       	LogTxt(&LogMgr, "P: Wire Ret: %.2f" , ABS_TVL_Pos);
#endif							
						
					fPosLatch[i] = vect->FbkVect.x;
			
					//TimerStart(&Tmr[i], 5.0);
					TimerStart(&Tmr[i], 0.5);
					prc->_step ++;
							
				break;

				case 27:

					prc->Status = prcsSTATUS_Retract;
					
					if(TimerIsTimeOut(&Tmr[i])){	

						//prc->Status = prcsSTATUS_Retract;
						
						KK_Retract_Catch[i] = fPosLatch[i] - vect->FbkVect.x; 						
						prc->_step = 0;

						
					}
	
				break;

				case 50: // Jogging

					prc->Status = prcsSTATUS_Jog;

					if(prc->fParam[0] != 0.0)
						Ramp[i].x = *WIRE_Param_Set[i].pJogSpeed * prc->fParam[0];
					else{
						prc->Status = prcsSTATUS_Stopped;
						Ramp[i].x = 0.0;
					}

				break;	
				

				case 60: /// Direct Cntrl
				
					prc->Status = prcsSTATUS_DirectCntrl;
					
					if(vect->FbkVect.Mode != vectSt_Mode_Volt){
						vect->InVal = 0.0;
						vect->Mode = vectSt_Mode_Volt;
					}
					else{

						if(WIRE_Param_Set[i].pJogSpeedDirCntrl){
							
							if(prc->fParam[0] != 0){
								
								vect->InVal = (*WIRE_Param_Set[i].pJogSpeedDirCntrl)* prc->fParam[0] ;
								
							}	
							else {
								
								prc->_step ++;
								//prc->_step = 0;

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
						prc->_step = 0;
			
				break;

				


				case 70: // Force directly speed in Calib

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					prc->Status = prcsSTATUS_ForceCalib;

					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
											
				break;
					
//////////////////////////////////////
// This is for testing purpose only 
				case 100: // nic

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					else{

						TimerStart(&Tmr[i], 0.1);
						prc->_step = 101;
					}	
					
				break;


				case 101:

					if(!TimerIsTimeOut(&Tmr[i]))
						break;

						
					if(vect->FbkVect.Mode != vectSt_Mode_Hold){
						vect->InVal = 0;
						vect->Mode = vectSt_Mode_Hold;	

					}else{

						TimerStart(&Tmr[i], 1.0);
						prc->_step = 102;

					}
			

				break;

				case 102: 


					if(!TimerIsTimeOut(&Tmr[i]))
						break;

					
					if(vect->FbkVect.Mode != vectSt_Mode_Vel)						
						vect->Mode = vectSt_Mode_Vel;
					
					else{

						TimerStart(&Tmr[i], 1.0);
						prc->_step = 101;
					}
						

				break;

////////////////////////////////////////////////////////////
				
				case 0xFF:  // Disable

					if(vect->FbkVect.Mode != vectSt_Mode_Idle)
						vect->Mode = vectSt_Mode_Idle;

					if(!prc->Disable)
						prc->_step = 0;
				

				break;	
	
				
	
	
			}
				
			
			TimerExecute(&Tmr[i]);
			if(Ramp[i].dy_up <= 0.0){
				Ramp[i].dy_up = 3.4e+38;
				Ramp[i].dy_down = Ramp[i].dy_up;
				
			}
			AzzRamp(&Ramp[i]);
			Ramp[i].set = 0;

					
			if(vect->Mode == vectSt_Mode_Vel){


				if(prc->_step == 0)
					vect->InVal = 0.0;
				
				else if(prc->_step == 70){ // Force directly speed

					vect->InVal =  prc->fParam[1];
					
					//prc->Fbk[1] = vect->InVal ; // vel Commanded

					
				}else if((prc->_step == 2)  // Start delay
						|| (prc->_step == 10)){ // Select Mtr

					vect->InVal =  0.0;
					
					//prc->Fbk[1] = 0.0 ; 
				}				
								
				else{

					if(WIRE_Param_Set[i].pSynchClock)
						synchClck = (*WIRE_Param_Set[i].pSynchClock);
					else
						synchClck = 0;


					switch(synchClck){

						default:

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * (SpeedSp - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


						case 1:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedSp )/ ( SpeedSp);

							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedSp ) - (*WIRE_Param_Set[i].pSpeedFinal));


						break;

						case 2:

							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedInSp )/ ( SpeedSp);
							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedInSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;

						
						case 3:
							
							//y = Ramp[i].y * (*WIRE_Param_Set[i].pSpeedOutSp )/ ( SpeedSp);

							
							if(prc->_step <= 5) // Ramp Up
								y = (*WIRE_Param_Set[i].pSpeedInit) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedInit));
							else
								y = (*WIRE_Param_Set[i].pSpeedFinal) + Ramp[i].y * ((*WIRE_Param_Set[i].pSpeedOutSp ) - (*WIRE_Param_Set[i].pSpeedFinal));

						break;


					}

										
					//prc->Fbk[1] = y ; // vel Commanded
					//prc->Fbk[1] = SpeedSp;


					switch(WIRE_Param_Set[i].BoostCmd){

						case 1:

							y += SpeedBoost1;

						break;


						case 2:

							y += SpeedBoost2;

						break;


						default:

						break;

						
					}

					if(y < 0.0)
						y = 0.0;
					
					vect->InVal = getFromCalib(y, WIRE_Param_Set[i].pCalibPoints, calibType);

					  if(Ramp[i].y <= 0)
						vect->InVal = 0.0;

				}

				
			}
			else{				
					
				//prc->Fbk[1] = 0 ;  // vel Commanded

			}	

			

			// Na wyrazne rzadanie zmienilem
			/*
			if(prc->Status == prcsSTATUS_Stopped){
				prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (SpeedSp) ; 
			}
			else
				prc->Fbk[1] = prc->Fbk[0];
			*/
			prc->Fbk[1] = (SpeedSp) ; 	
			
			
		}
	
	
	
	
}

