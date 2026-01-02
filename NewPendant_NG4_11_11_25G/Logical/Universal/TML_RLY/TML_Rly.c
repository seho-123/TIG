

/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: TML_RLY
 * File: TML_Rly.c
 * Author: ckaczkowski
 * Created: Juky 28, 2018
 ********************************************************************
 * 
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>


float _InVal[TML_RLY_NUM];


void _INIT DC2DIRMTR_INIT(void)
{
	VectorStateMach[0].Status = VectorStateMach[0].Status;
	Process[0].Status = Process[0].Status;
	TML_MTR_INST[0].xCMD = TML_MTR_INST[0].xCMD;

	
}

void _CYCLIC DC2DIRMTR_CYCLIC(void)
{
	int i;
	StateVectorProxy_typ *vect , *tmlVect;
	TML_typ *tml;
	//ERROR_Mgr_typ* log;
	TML_OUTPUTS_obj *rly;
	
	
	for (i = 0 ; i < TML_RLY_NUM ; i++){
	
	
		////////////////////////////////////
		// Vectore State Commands
	
		vect = (StateVectorProxy_typ*) TmlRelay[i].pState;		
		rly = &TmlRelay[i];

		if(!vect)
			continue;

		if(rly->Cfg.TML_Vect_Indx >= VECTORS_NUM)			
			continue;
		
		tmlVect =  &VectorStateMach[rly->Cfg.TML_Vect_Indx];

		if(rly->Cfg.TML_Indx >= TML_SERVO_NUM)			
			continue;

		tml = &TML_MTR_INST[rly->Cfg.TML_Indx];

		

		if(rly->Disable){

			if(vect)
				vect->simul = 1;

			break;

		}	
		
	
		if(vect){			
	
	
			if(!vect->simul){	
	
					
				switch(vect->OutVect.Mode){


					default:

						rly->xCmd = 0;

						break;
	
					
	
					case vectSt_Mode_Volt:

						rly->xCmd = 10;							
																			
						break;						
	
				}	
			}
			//else
			//rly->xCmd = 0;

				
			vect->FbkVect.Mode = vect->Mode;
	
		}
		//////////////////////////////////////

			
 
		switch(rly->xCmd){



			case 0:

				rly->msc.sw_step = 0;					

				break;	


			case 10:	

				
				//if(_InVal[i] != vect->InVal){

					if((vect->InVal * rly->Cfg.MtrDir) > 0.0){


						if(!(!tml->DigOut_stat[1] && tml->DigOut_stat[2])){
							
							if(rly->msc.sw_step == 0)
								rly->msc.sw_step = 10;	

						}	

						


					}
					else if((vect->InVal * rly->Cfg.MtrDir) < 0.0){

						
						if(!(tml->DigOut_stat[1] && !tml->DigOut_stat[2])){
							
							if(rly->msc.sw_step == 0)
								rly->msc.sw_step = 20;	

						}	

					}
					else{

						if(!(tml->DigOut_stat[1] && tml->DigOut_stat[2])){
							
							if(rly->msc.sw_step == 0)
								rly->msc.sw_step = 100;	

						}					
						
					}

				//}

				if(rly->msc.sw_step == 0)
					_InVal[i] = vect->InVal;
					

			break;			


			


		}	

		switch(rly->msc.sw_step){


				
				/////////  FWD ////////////

				case 10: 

					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Rst_OUT4;
						rly->msc.sw_step++;

					}

					
				break;

				case 11: 

					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Set_OUT1;
						rly->msc.sw_step++;

					}

					
				break;

				case 12:
					
					if(!tmlVect->xCMD ){

						_InVal[i] = vect->InVal;

						rly->msc.sw_step = 0;
						

					}

				
				break;

				/////////  REV ////////////
				
				case 20: 

					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Rst_OUT1;
						rly->msc.sw_step++;

					}

					
				break;

				case 21: 

					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Set_OUT4;
						rly->msc.sw_step++;

					}

					
				break;

				case 22:
					
					if(!tmlVect->xCMD ){

						_InVal[i] = vect->InVal;
						rly->msc.sw_step = 0;

					}

				
				break;

				///////// STOP ////////////

				case 100: // STOP

					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Rst_OUT1;
						rly->msc.sw_step++;

					}
					
				break;

				case 101:
					
					if(!tmlVect->xCMD ){
							
						tmlVect->xCMD = VECT_Rst_OUT4;
						rly->msc.sw_step++;

					}

				break;


				case 102:
					
					if(!tmlVect->xCMD ){

						_InVal[i] = vect->InVal;

						rly->msc.sw_step = 0;

					}

				
				break;

			}		
			
	}
		
				
}





