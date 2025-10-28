/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: GasCool
 * File: GasCool.c
 * Author: ckaczkowski
 * Created: March 05, 2014
 ********************************************************************
 * Implementation of program GasCool
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>



_LOCAL TON_typ Tmr[GAS_NUM];
GenProcess_typ *prc;
GenProcess_typ *prcCool;

_LOCAL TON_typ CoolTmr[COOL_NUM];

_LOCAL  plcbit GasSolOutput[GAS_NUM];
_LOCAL  plcbit GasSwInput[GAS_NUM];
_LOCAL  plcbit CoolSolOutput[COOL_NUM];
_LOCAL  plcbit CoolSwInput[COOL_NUM];

_LOCAL TON_typ CoolTmo[COOL_NUM];
_LOCAL TON_typ GasTmo[GAS_NUM];

_LOCAL plcbit _GasTmoTrig[GAS_NUM] , _CoolTmoTrig[COOL_NUM];

_LOCAL plcbit Gas_Arclink_ON;
_LOCAL USINT ArcLink_Gas_Valve_step;

plcbit _gas_Arclink_ON;

void ArcLinkGasSol();



void _INIT GasCoolINIT(void)
{
	Process[0].Disable = Process[0].Disable;
}


void _CYCLIC GasCoolCYCLIC(void){
int i;

	//if(GAS_Param_Set[0].Mode == 1)
		ArcLinkGasSol();
		
		
	for(i=0 ; i < GAS_NUM ; i++){


			prc = GAS_Param_Set[i].pPrc;

			if(!prc)
				break;

			if(prc->ConfigReady <= 0)
				break;

						
			if(GAS_Param_Set[i].pSensInput)
				GasSwInput[i] = (*GAS_Param_Set[i].pSensInput);
			else
				GasSwInput[i] = GasSolOutput[i];
			
			if(prc->Disable){

				prc->_step = 0xFF;
				GasSolOutput[i]= 0;	

				switch(prc->xCMD){


				case prcsForceON:
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

			}else{
	
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


					case prcsSTOP:

						if((prc->_step != 0) && (prc->_step <= 5))
							prc->_step = 20;

					break;

			
					case prcsForceON:
		
						prc->_step = 5;	
		
					break;
		
					
				}
			}
			prc->xCMD = 0;
	
			/////////////////////////////////////
		
		
			switch(prc->_step){
	
	
				case 0:  // IDLE - stopped

					GasSolOutput[i]= 0;					
					prc->Status = prcsSTATUS_Stopped;
				
				break;
	
	
				case 1: // start
	
					prc->Status = prcsSTATUS_StartDly;

					GasSolOutput[i]= 1;	
									
					if(*GAS_Param_Set[i].pStartDly > 0.0){
						TimerStart(&Tmr[i],  *GAS_Param_Set[i].pStartDly);
						prc->_step = 2;
					}
					else					
						prc->_step = 5;
	

										
				break;
	
	
				case 2:										

					GasSolOutput[i]= 1;
			
					if(TimerIsTimeOut(&Tmr[i])){
						prc->_step = 5;
						
					}	
						
				break;				
	
				
	
				case 5:
					
					GasSolOutput[i]= 1;
			
					prc->Status = prcsSTATUS_Run;
					
				break;
	
	
	
						
				case 20:  // stop


					GasSolOutput[i]= 1;
					
					prc->Status = prcsSTATUS_StopDly;
					
					if(*GAS_Param_Set[i].pStopDly > 0){
						TimerStart(&Tmr[i], *GAS_Param_Set[i].pStopDly);
						prc->_step = 21;
					}
					else					
						prc->_step = 0;
					
	
				break;

				case 21:
					
					GasSolOutput[i]= 1;
					
					if(TimerIsTimeOut(&Tmr[i]))
						prc->_step = 0;
					
	
	
				break;					
		

	
			}

			TimerExecute(&Tmr[i]);

			
			if(TimerIsTimeOut(&GasTmo[i])){
			
				GasTmo[i].IN = 0;
				GasTmo[i].Q = 0;

				if(GasTmo[i].PT > 0){ 
					Errors(&ErrorMgr,ERROR_COOL_GAS ,ERROR_GAS_TMO,i);
					prc->_step = 0;
				}


			}
			else if(((!GasSwInput[i])&& GasSolOutput[i])
				&& !_GasTmoTrig[i]){
			
				TimerStart(&GasTmo[i],*GAS_Param_Set[i].pTmo);
				_GasTmoTrig[i] = 1;
			}
			
			else if(GasSwInput[i] || (!GasSolOutput[i])){

				GasTmo[i].IN = 0;
				GasTmo[i].Q = 0;

				_GasTmoTrig[i] = 0;

			}
			
		TimerExecute(&GasTmo[i]);
		if(GAS_Param_Set[i].pOutput)
			(*GAS_Param_Set[i].pOutput) = GasSolOutput[i];
						
	
	}


	
	///////////////////////////////////
	//  COOLANT 
	//////////////////////////////////

	
	for(i=0 ; i < COOL_NUM ; i++){

	

			if(COOL_Param_Set[i].pSensInput)
				CoolSwInput[i] = (*COOL_Param_Set[i].pSensInput);
			else
				CoolSwInput[i] = CoolSolOutput[i];

			prcCool = COOL_Param_Set[i].pPrc;

			if(!prcCool)
				break;
		
			if(prcCool->ConfigReady <= 0)
				break;
			
			if(prcCool->Disable){

				prcCool->_step = 0xFF;
				CoolSolOutput[i]= 0;	

				switch(prcCool->xCMD){


					case prcsSTART:

						prcCool->Status = prcsSTATUS_Run;
						
					break;

					
					case prcsHalt:
					case prcsSTOP:

						prcCool->Status = prcsSTATUS_Stopped;
						
					break;


					default:

						if(!((prcCool->Status == prcsSTATUS_Stopped) || (prcCool->Status == prcsSTATUS_Run)))
							prcCool->Status = prcsSTATUS_Stopped;
						
					break;	
					


				}

			}
			else{
	
				///////////////////////////////////
				switch(prcCool->xCMD){

					
					case prcsHalt:
					case prcsSTOP:
					case prcsRESTART:
					
						CoolSolOutput[i]= 0;	
						prcCool->Status = prcsSTATUS_Stopped;
						
					break;	
		
					case prcsForceON:
					case prcsSTART:

					
						CoolSolOutput[i]= 1;
						prcCool->Status = prcsSTATUS_Run;
					

					break;
						
				}
			}

		prcCool->xCMD = 0;

		
		if(TimerIsTimeOut(&CoolTmo[i])){
		
			CoolTmo[i].IN = 0;
			CoolTmo[i].Q = 0;

			if(CoolTmo[i].PT > 0){
				Errors(&ErrorMgr,ERROR_COOL_GAS,ERROR_COOL_TMO,0);
				prcCool->xCMD = prcsHalt;
			}

		}
		
		else if(((!CoolSwInput[i])&& CoolSolOutput[i])
			&& !_CoolTmoTrig[i]){
			
			TimerStart(&CoolTmo[i],*COOL_Param_Set[i].pTmo);
			_CoolTmoTrig[i] = 1;
		}
		
		else if(CoolSwInput[i] || (!CoolSolOutput[i])){

			CoolTmo[i].IN = 0;
			CoolTmo[i].Q = 0;

			_CoolTmoTrig[i] = 0;

		}
		
		TimerExecute(&CoolTmr[i]);
		TimerExecute(&CoolTmo[i]);

		if(COOL_Param_Set[i].pOutput)
			(*COOL_Param_Set[i].pOutput) = CoolSolOutput[i];
		
	}
}
void ArcLinkGasSol(){
	/////////  GAS Solenoid ////

	Gas_Arclink_ON = GasSolOutput[0];

	if(!ArcLink.robot.isMapped){	
		
		ArcLink_Gas_Valve_step = 2;
	
	}else{	

		switch(ArcLink_Gas_Valve_step){

			default:

			break;

			case 1:

				if(ArcLink.robot.isMapped && (ArcLink.robot.miscSetup.state == 9)
					&& ArcLink.robot.isReady){

					if(!ArcLink.robot.Intrf.xCMD){

						ArcLink.robot.miscSetup.GasSolAddr = 0;
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_SEARCH_GAS_SOL_ADR;
						ArcLink_Gas_Valve_step ++;
					}

				}
				
			break;

			case 2:

				if(!ArcLink.robot.Intrf.xCMD 
					&& (ArcLink.robot.miscSetup.GasSolAddr > 0)){

					_gas_Arclink_ON = !Gas_Arclink_ON;
					ArcLink_Gas_Valve_step ++;

				}
				
			break;


			case 3:

				if(!ArcLink.robot.Intrf.xCMD){

					if(Gas_Arclink_ON !=_gas_Arclink_ON){

						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_ENABLE;
						ArcLink_Gas_Valve_step ++;
						
					}
				}

			break;

			case 4:

				if(!ArcLink.robot.Intrf.xCMD){

					if(Gas_Arclink_ON)
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_ON;					

					else 
						ArcLink.robot.Intrf.xCMD = ROBOT_CMD_GAS_SOL_OFF;
						
					
					ArcLink_Gas_Valve_step ++;

				}

			break;

			case 5:

				if(!ArcLink.robot.Intrf.xCMD){
					
					_gas_Arclink_ON = Gas_Arclink_ON;
					ArcLink_Gas_Valve_step = 3;

				}	

			break;

		}

	}
	/////////////////////
		
}

