
/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: DC2DirMtr
 * File: DC2DirMtr.c
 * Author: ckaczkowski
 * Created: November 1, 2017
 ********************************************************************
 * Implementation of program DC2DirMtr
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>


_LOCAL plcbit Q1[DC2DIRMTR_NUM], Q2[DC2DIRMTR_NUM] ,Q3[DC2DIRMTR_NUM] ,Q4[DC2DIRMTR_NUM]; 

void _INIT DC2DIRMTR_INIT(void)
{
	VectorStateMach[0].Status = VectorStateMach[0].Status;
	Process[0].Status = Process[0].Status;

	
}

void _CYCLIC DC2DIRMTR_CYCLIC(void)
{
int i;
StateVectorProxy_typ* vect;
//ERROR_Mgr_typ* log;
DC2DIRMTR_obj *mtr;
	
	
	for (i = 0 ; i < DC2DIRMTR_NUM ; i++){
	
	
			////////////////////////////////////
			// Vectore State Commands
	
			vect = (StateVectorProxy_typ*) DC2DirMtr[i].pState;
			mtr = &DC2DirMtr[i];


			if(!vect || !mtr){

				return;

			}			

			if(mtr->Disable){

				if(vect)
					vect->simul = 1;

				break;

			}	
		
	
			if(vect){			
	
	
				if(!vect->simul){	
	
					
					switch(vect->OutVect.Mode){


						default:

							mtr->xCmd = 0;

						break;
	
					
	
						case vectSt_Mode_Volt:

							mtr->xCmd = 10;							
																			
						break;						
	
					}	
				}
				//else
					//mtr->xCmd = 0;

				
				vect->FbkVect.Mode = vect->Mode;
	
			}
			//////////////////////////////////////

			
 
			switch(mtr->xCmd){



				case 0:

					mtr->msc.sw_step = 0;					

				break;	


				case 10:	


					if((vect->InVal * mtr->Cfg.MtrDir) > 0.0){


						if(mtr->msc.sw_step == 2)
							mtr->msc.sw_step = 0;
						else
							mtr->msc.sw_step = 1;


					}
					else if((vect->InVal * mtr->Cfg.MtrDir) < 0.0){

						if(mtr->msc.sw_step == 1)
							mtr->msc.sw_step = 0;
						else
							mtr->msc.sw_step = 2;


					}
					else{

						mtr->msc.sw_step = 0;
					}
					

				break;			


			}	


			switch(mtr->msc.sw_step){



				case 0: // STOP

					Q1[i] = 0;
					Q2[i] = 0;
					Q3[i] = 0;
					Q4[i] = 0;
					
				break;


				case 1: // FWD

					Q1[i] = 1;
					Q2[i] = 0;
					Q3[i] = 1;
					Q4[i] = 0;
					
				break;


				case 2: // REV

					Q1[i] = 0;
					Q2[i] = 1;
					Q3[i] = 0;
					Q4[i] = 1;
					
				break;

			}

			if(mtr->Cfg.pQ1_output 
			&& mtr->Cfg.pQ2_output
			&& mtr->Cfg.pQ3_output
			&& mtr->Cfg.pQ4_output){

				(*mtr->Cfg.pQ1_output) = Q1[i];
				(*mtr->Cfg.pQ2_output) = Q2[i];
				(*mtr->Cfg.pQ3_output) = Q3[i];
				(*mtr->Cfg.pQ4_output) = Q4[i];

			}
			
		}


		
				
}




