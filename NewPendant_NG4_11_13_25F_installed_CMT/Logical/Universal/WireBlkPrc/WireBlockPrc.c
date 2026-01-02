/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: WireBlockPrc
 * File: WireBlockPrc.c
 * Author: ckaczkowski
 * Created: November 14, 2013
 ********************************************************************
 * Implementation of program WireBlockPrc
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>

GenProcess_typ* prc;
StateVectorProxy_typ*  vect;


void _INIT WireBlockPrcINIT(void)
{

	int i;
		
	Process[0].Disable = Process[0].Disable;
	TML_MTR_INST[0].Status = TML_MTR_INST[0].Status;
	VectorStateMach[0].InVal = VectorStateMach[0].InVal;

}

void _CYCLIC WireBlockPrcCYCLIC(void)
{
int i;


	for(i=0 ; i < WIRE_BLK_NUM ; i++){


		prc = WR_BLK_Param_Set[i].pPrc;	

		if(!prc)
			return;
		
		vect = WR_BLK_Param_Set[i].pVect;

		if(!vect)
			return;

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

				case prcsRESTART:

					prc->_step = 0;
				
						
				break;		

				case prcsSTART:
		
					prc->Status = prcsSTATUS_Run;

				break;

				
				case prcsHalt:	
				case prcsSTOP:

					prc->Status = prcsSTATUS_Stopped;

				break;

				case prcsDirectCntrl:
				case prcsJOGpos:
					
					prc->_step = 50;

				break;

				//test
				case 100:

						

				break;

				
			}
		}
		prc->xCMD = 0;

		/////////////////////////////////////
	
	
		switch(prc->_step){


			case 0:  // IDLE - stopped

				if(!((prc->Status == prcsSTATUS_Stopped) || (prc->Status == prcsSTATUS_Run)))
					prc->Status = prcsSTATUS_Stopped;
				
				vect->Mode = vectSt_Mode_Volt;
				vect->InVal = 0.0;
							
			break;


			
			
			case 50: /// Jogging
			
				//prc->Status = prcsSTATUS_Jog;

				if(*WR_BLK_Param_Set[i].pJogSpeed > 1.0)
					*WR_BLK_Param_Set[i].pJogSpeed = 1.0;
				else if(*WR_BLK_Param_Set[i].pJogSpeed < 0.0)
					*WR_BLK_Param_Set[i].pJogSpeed = 0.0;
				
				if(vect->FbkVect.Mode != vectSt_Mode_Volt)
					vect->Mode = vectSt_Mode_Volt;
				else{
					
					if(prc->fParam[0] > 0)
						vect->InVal = (*WR_BLK_Param_Set[i].pJogSpeed * WR_BLK_Param_Set[i].MaxSpeedJog);
					else if(prc->fParam[0] < 0)
						vect->InVal = (*WR_BLK_Param_Set[i].pJogSpeed * WR_BLK_Param_Set[i].MaxSpeedJog) * (-1.0);
					else						
						prc->_step = prcsIDLE;
					
				
				}
			

			break;

			


		}

	}

}
