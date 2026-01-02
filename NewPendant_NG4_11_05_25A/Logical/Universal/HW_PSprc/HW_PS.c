/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: HW_PS
 * File: HW_PS.c
 * Author: ckaczkowski
 * Created: November 26, 2013
 ********************************************************************
 * Implementation of program HW_PS
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>


_LOCAL TON_typ Tmr[HW_PS_NUM];
GenProcess_typ* prc;
_LOCAL AzzRamp_typ Ramp[HW_PS_NUM];
_LOCAL  AzzLimScal_typ  LimScal_AmpCMD[HW_PS_NUM];



float _AmpSP[HW_PS_NUM];


void _INIT HW_PSINIT(void)
{
int i;

	Process[0].Disable = Process[0].Disable;
	Hardware[0].Anal_Inp[0] = Hardware[0].Anal_Inp[0];

	for(i=0 ; i < (HW_PS_NUM); i++){
		
		Ramp[i].y_max = 1000.0;
		Ramp[i].y_min = 0;	
		Ramp[i].enable = 1;

		LimScal_AmpCMD[i].x1 = 0.0;
		LimScal_AmpCMD[i].y1 = 0.0;
			
	}

}

void _CYCLIC HW_PSCYCLIC(void)
{	int i;
	float y;

	
	
		for(i=0 ; i < HW_PS_NUM ; i++){


			prc = HW_PS_Param_Set[i].pPrc;

			if(!prc)
				break;

			if(!HW_PS_Param_Set[i].pCalibPoints)
				break;

			if(prc->ConfigReady <= 0)
				break;

			LimScal_AmpCMD[i].x2 = HW_PS_Param_Set[i].AmpCMD_Max;
			LimScal_AmpCMD[i].y2 = HW_PS_Param_Set[i].rawCmdMax;
			
						
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

			}else{
	
				///////////////////////////////////
				switch(prc->xCMD){

					case prcsRESTART:
					case prcsHalt:	

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

					if((prc->_step) == 0 || (prc->_step > 5))
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
		
		
			switch(prc->_step){
	
				case 255: // Disable
				case 0:  // IDLE - stopped
	
					prc->Status = prcsSTATUS_Stopped;
					Ramp[i].y_set = 0.0;
					Ramp[i].set = 1;
			
			
								
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;


					Ramp[i].y_set = *HW_PS_Param_Set[i].pStartAmp;
					Ramp[i].set = 1;

									
					if(*HW_PS_Param_Set[i].pStartDly > 0){
						TimerStart(&Tmr[i],  *HW_PS_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 3;
	

										
				break;
	
	
				case 2:
	
					Ramp[i].set = 1;					
					
					if(TimerIsTimeOut(&Tmr[i])){
						prc->_step = 3;
						
					}	
						
				break;	
				
	
				case 3:
	
					prc->Status = prcsSTATUS_RampUp;				
	
				
					if (*HW_PS_Param_Set[i].pRampUp > 0) 
						Ramp[i].dy_up = fabsf((*HW_PS_Param_Set[i].pAmpSP - Ramp[i].y)/(*HW_PS_Param_Set[i].pRampUp));
					else 
						Ramp[i].dy_up = 3.4e+38;

					
					Ramp[i].dy_down = Ramp[i].dy_up;
					Ramp[i].x = *HW_PS_Param_Set[i].pAmpSP;

					TimerStart(&Tmr[i],  *HW_PS_Param_Set[i].pRampUp);

					Ramp[i].set = 0;

					prc->_step = 4;	
					
				break;	
	
	
	
				case 4:
	
					
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i])){
						_AmpSP[i] = (*HW_PS_Param_Set[i].pAmpSP);
						prc->_step = 5;

					}	
					
										
	
				break;
	
	
	
				case 5:
	
					prc->Status = prcsSTATUS_Run;


					if(_AmpSP[i] != (*HW_PS_Param_Set[i].pAmpSP)){	
						_AmpSP[i] = (*HW_PS_Param_Set[i].pAmpSP);
						
						Ramp[i].x = *HW_PS_Param_Set[i].pAmpSP;

						
						if (*HW_PS_Param_Set[i].pRampUp > 0)
							Ramp[i].dy_up = fabsf((*HW_PS_Param_Set[i].pAmpSP)/(*HW_PS_Param_Set[i].pRampUp));
							//Ramp[i].dy_up = fabsf((*HW_PS_Param_Set[i].pSpeedSp - Ramp[i].y)/(*HW_PS_Param_Set[i].pRampUp));
						else 
							Ramp[i].dy_up = 3.4e+38;
						
						Ramp[i].dy_down = Ramp[i].dy_up;
					}

					
				break;


				case 20:
	
					prc->Status = prcsSTATUS_StopDly;
					
					if(*HW_PS_Param_Set[i].pStopDly > 0){
						TimerStart(&Tmr[i], *HW_PS_Param_Set[i].pStopDly);
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
					
					if (*HW_PS_Param_Set[i].pRampDwn > 0) 
						Ramp[i].dy_down = fabsf((Ramp[i].y - (*HW_PS_Param_Set[i].pEndAmp))/(*HW_PS_Param_Set[i].pRampDwn));
					else 
						Ramp[i].dy_down = 3.4e+38;
						
								
					Ramp[i].dy_up = Ramp[i].dy_down;

					Ramp[i].x = *HW_PS_Param_Set[i].pEndAmp;

					TimerStart(&Tmr[i],  *HW_PS_Param_Set[i].pRampDwn);
				
					prc->_step++; 

				break;	


				case 23:
	
					if(Ramp[i].x_reached || TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;	
	
	
				break;
					
		
				

				case 50:	
					
					Ramp[i].y_set = *HW_PS_Param_Set[i].pStartAmp;
					Ramp[i].set = 1;

					prc->Status = prcsSTATUS_ForceON;
	
				break;	


				case 60:	

					_AmpSP[i] = (*HW_PS_Param_Set[i].pAmpSP);
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



			if(HW_PS_Param_Set[i].pAnal_Output && HW_PS_Param_Set[i].pCalibPoints){


				prc->Fbk[1] = Ramp[i].y; // Cmd Amp
								
				if(prc->_step == 70){ // Calib
				
				   y = prc->fParam[1];

				   prc->Fbk[1] = 0; // Cmd Amp
				

				}				
				else if(prc->Param[0])
					y = getFromCalib(Ramp[i].y, HW_PS_Param_Set[i].pCalibPoints , CALIB_2POINTS) ;
				
				else
					y = getFromCalib(Ramp[i].y, HW_PS_Param_Set[i].pCalibPoints , CALIB_2POINTS)  * ((*HW_PS_Param_Set[i].pAmpBoost)+1.0);


				(*HW_PS_Param_Set[i].pAnal_Output) = (INT)(LimScal( y , &LimScal_AmpCMD[i]));
							
			}

			if(	HW_PS_Param_Set[i].pOutput){

				if(prc->Status == prcsSTATUS_Stopped)
					
					(*HW_PS_Param_Set[i].pOutput) = 0;
				
				else
					(*HW_PS_Param_Set[i].pOutput) = ((*HW_PS_Param_Set[i].pAnal_Output)> 0);

			}


			if(!(HW_PS_Param_Set[i].pOutput))
				prc->Fbk[1] = 0; // Cmd Amp

			if((prc->Status == prcsSTATUS_Stopped) || prc->Disable){
				
				prc->Fbk[0] = 0.0;
				prc->Fbk[1] = (*HW_PS_Param_Set[i].pAmpSP);	
				
			}else
				prc->Fbk[0] = prc->Fbk[1]; // cur Amp as  Cmd Amp

			// for implementation in future
			prc->Fbk[2] = 0; //varCurHW_Volt
			prc->Fbk[3] = 0; //varHW_Volt_Cmd
				
				
			
	
}
	
	
	
	
}

