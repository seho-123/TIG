/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: BR_DC
 * File: BR_DC.c
 * Author: ckaczkowski
 * Created: December 10, 2013
 ********************************************************************
 * Implementation of program BR_DC
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>



#define CLASS_TASK 0.0016


/***** Variable declaration *****/
_LOCAL DINT stamp1;        /* timestamp at start of task */
_LOCAL DINT old_stamp1;    /* timestamp at start of previous cycle */
_LOCAL DINT stamp2;        /* timestamp at end of task */
_LOCAL float duration;      /* duration of task in previous cycle */


_LOCAL INT KK_ENCD[GEN_MTR_NUM] , _KK_ENCD[GEN_MTR_NUM] , KK_ENCD_delta[GEN_MTR_NUM];


_LOCAL AzzSlimPID_typ AZZ_PID;
_LOCAL AzzRamp_typ AZZ_Ramp;
_LOCAL AzzLimScal_typ AZZ_LimScal;
_LOCAL AzzScal_typ AZZ_Scal;
_LOCAL AzzTON_typ AZZ_TON;
_LOCAL AzzMovAvgFlt_typ AZZ_MoveAvg;

_LOCAL azzslimpid_par_typ AZZ_pid_Param;

_LOCAL plcbit AZZ_Fun_test;


_LOCAL AzzNode_typ Azz_CalibrPoints[10];
_LOCAL AzzLinSpline_typ Azz_LinSpline;




/*void AzzSlimPID(AzzSlimPID_typ *lcr);
void AzzRamp(AzzRamp_typ *lcr);
void AzzMovAvgFlt(AzzMovAvgFlt_typ *lcr) ;
void AzzLimScal(AzzLimScal_typ *lcr);
void AzzScal(AzzScal_typ *lcr);
void AzzTON(AzzTON_typ *t); 
*/


void _INIT BR_DCINIT(void)
{
	


	AZZ_Fun_test = 0;

	AZZ_pid_Param.Kp = 1.0;
	AZZ_pid_Param.Tn = 0;
	AZZ_pid_Param.Tv = 0;
	AZZ_pid_Param.Y_max = 100;
	AZZ_pid_Param.Y_min = -100;

	AZZ_PID.pPar = &AZZ_pid_Param;


	AZZ_Scal.x1 = 1.0;
	AZZ_Scal.y1 = 1.0;

	AZZ_Scal.x2 = -10.0;
	AZZ_Scal.y2 = -10.0;

	AZZ_LimScal.x1 = 1.0;
	AZZ_LimScal.y1 = 1.0;

	AZZ_LimScal.x2 = 10.0;
	AZZ_LimScal.y2 = 10.0;


	AZZ_Ramp.dy_down = 1.0;
	AZZ_Ramp.dy_up = 1.0;
	AZZ_Ramp.y_max = 1000.0;
	AZZ_Ramp.y_min = - AZZ_Ramp.y_max; 
	
	AZZ_Ramp.y_set = 12.0;
	AZZ_Ramp.set = 1;



	AZZ_MoveAvg.base = 20;
	AZZ_MoveAvg.x = 1;


	Azz_CalibrPoints[0].X = 0.0;
	Azz_CalibrPoints[0].Y = 0.0;

	Azz_CalibrPoints[1].X = 1.0;
	Azz_CalibrPoints[1].Y = 1.0;

	Azz_CalibrPoints[2].X = 5.0;
	Azz_CalibrPoints[2].Y = 5.0;

	Azz_CalibrPoints[3].X = 6.0;
	Azz_CalibrPoints[3].Y = 6.0;

	Azz_CalibrPoints[4].X = 7.0;
	Azz_CalibrPoints[4].Y = 7.0;
	

	Azz_LinSpline.NbNodes= 5; /* number of nodes */
	Azz_LinSpline.pNodeVec= &Azz_CalibrPoints[0]; /* node vector */
	Azz_LinSpline.Invert= 0; /* no inverting */
	Azz_LinSpline.Update= 1; /* update all above values */	
	Azz_LinSpline.enable= 1; /* enable instance */


	////////////
	VectorStateMach[0].Status = VectorStateMach[0].Status;

	
}

void _CYCLIC BR_DCCYCLIC(void)
{
int i;
StateVectorProxy_typ* vect;
ERROR_Mgr_typ* log;
GEN_MTR_obj *mtr;
DINT deltaPos;


	///  Paul Function Test /////////

	if(AZZ_Fun_test){

	/*
		AzzSlimPID(&AZZ_PID);
		AzzRamp(&AZZ_Ramp);
		AzzMovAvgFlt(&AZZ_MoveAvg) ;
		AzzLimScal(&AZZ_LimScal);
		AzzScal(&AZZ_Scal);
		AzzTON(&AZZ_TON); 

		
		AZZ_MoveAvg.x = (AZZ_MoveAvg.x > 0.5)?  0.5:1.0;
	*/

		/////////	

		AzzLinSpline(&Azz_LinSpline);

	}

	/////////////////////////////////////

	/* calculate the duration of the previous task cycle */
	stamp1 = AsIOTimeStamp(); /* Stamp  task */
	duration = (stamp1 - old_stamp1) * 0.000001; 
	old_stamp1 = stamp1;	  /* remember for calculating duration in next cycle */
	
	if(duration <= 0)
		duration = CLASS_TASK;	

	
	for (i = 0 ; i < GEN_MTR_NUM ; i++){
	
	
			////////////////////////////////////
			// Vectore State Commands
	
			vect = (StateVectorProxy_typ*)DC_Mtr[i].pState;
			mtr = &DC_Mtr[i];


			if(!vect || !mtr){

				return;

			}	

			if(!mtr->Hrdw.pEncFbk || !mtr->Hrdw.pPulseWidthPWM){

				return;

			}	

			if( mtr->msc.kPos <= 0){

				*mtr->Hrdw.pPulseWidthPWM = 0;
				return;

			}	

			

			if(mtr->Disable){

				if(vect)
					vect->simul = 1;

				break;

			}	
		
	
			if(vect){


				switch(vect->xCMD){

					case VECT_Set_Act_Pos:

						mtr->msc.Pos = vect->Param[0];
						vect->xCMD = 0;
					
					break;


					default:

						vect->xCMD = 0;
						
					break;
					

				}
	
	
				if(!vect->simul){	
	
					
					switch(vect->OutVect.Mode){
	
						case vectSt_Mode_Hold:
						case vectSt_Mode_Idle:
	
							mtr->xCmd = 0;
							mtr->msc.PID.W = mtr->actVel;							
	
						break;	
		
						case vectSt_Mode_Home:
	
	
						break;	
	
						case vectSt_Mode_Vel:

							mtr->fParam[0] = vect->OutVect.vel * mtr->Cfg.mtr_dir;

							if(mtr->xCmd != 10){
								mtr->msc.PID.request = 3;
								mtr->xCmd = 10;
							}

							
							
						break;	
	
						case vectSt_Mode_Torq:
	
						break;	
	
						case vectSt_Mode_Volt:

							mtr->fParam[0] = vect->OutVect.volt * mtr->Cfg.mtr_dir;
							mtr->xCmd = 30;

							mtr->msc.PID.W = mtr->actVel;
													
						break;	
	
						case vectSt_Mode_Pos:
						case vectSt_Mode_PT:

							mtr->fParam[0] = vect->OutVect.x * mtr->Cfg.mtr_dir;
							
							if(mtr->xCmd != 21)
								mtr->xCmd = 20;
							
						break;	
	
						
	
					}	
				}
				else
					mtr->xCmd = 0;

				
				
	
			}
			//////////////////////////////////////

			/**************************************************************************/
			/*Feedback Evaluation */
			
			deltaPos = (*mtr->Hrdw.pEncFbk) - mtr->msc._pos;
			mtr->msc._pos = (*mtr->Hrdw.pEncFbk) ;		 

			
			if(deltaPos > 32768)
				deltaPos -= 65536;
			else if(deltaPos <= -32768)
				deltaPos+= 65536;
			
			mtr->msc.deltaPos = deltaPos * mtr->Cfg.enc_dir ;
			//mtr->actVel = ((float)(mtr->msc.deltaPos) / mtr->msc.kVel) ; 
			mtr->actVel = ((float)(mtr->msc.deltaPos) / (mtr->msc.kPos * duration)) ; 
			mtr->msc.Pos += mtr->msc.deltaPos;
			mtr->actPos = ((float)(mtr->msc.Pos) / mtr->msc.kPos) ; 

			vect->actPosDINT = mtr->msc.Pos;
 
			switch(mtr->xCmd){



				case 0:

					if(vect)
						vect->FbkVect.Mode = vectSt_Mode_Idle;

					mtr->msc.PID.enable = 0;

				break;	


				case 10:
					
					vect->FbkVect.Mode = vectSt_Mode_Vel;
					
					mtr->msc.PID.enable = 1;
					mtr->msc.PID.W = mtr->fParam[0];	
					mtr->msc.PID.X = mtr->actVel;

				break;	

				case 20:

					vect->FbkVect.Mode = vect->Mode;

					mtr->msc.PID.enable = 1;
					mtr->msc.PID.request = 3;
					
					//mtr->msc.Pos = (DINT) (mtr->fParam[0] * mtr->msc.kPos * mtr->Cfg.mtr_dir );		
					
					mtr->msc.PID.W = mtr->fParam[0];
					mtr->msc.PID.X = mtr->msc.PID.W;
					//mtr->msc._pos = (*mtr->Hrdw.pEncFbk);
					mtr->xCmd++;
					
								

				break;	

				case 21:

					vect->FbkVect.Mode = vectSt_Mode_Pos;

					//mtr->msc.PID.W = mtr->fParam[0] * mtr->Cfg.mtr_dir;
				
					mtr->msc.PID.W = mtr->fParam[0];
					mtr->msc.PID.X = mtr->actPos;

				break;	


				case 30:

					if(vect)
						vect->FbkVect.Mode = vectSt_Mode_Volt;
				
					mtr->msc.PID.enable = 0;
					
				break;	
				


			}	


			//////////////////////////////		

			if(vect){
				
				vect->FbkVect.x = mtr->actPos;
				vect->FbkVect.vel = mtr->actVel;
			}	
			

			/* Ramp Contriol */
	  		AzzRamp(&mtr->msc.Ramp);	
	  		AzzSlimPID(&mtr->msc.PID);
			
		
					
			
			
			if(mtr->xCmd == 30){ //Volt Mode

				if(fabsf(mtr->fParam[0])<=24.0 )

					*mtr->Hrdw.pPulseWidthPWM = (INT)(1365.291 *  mtr->fParam[0]); // 32767/24 = 
				
				else {

					if(mtr->fParam[0] > 0)
						*mtr->Hrdw.pPulseWidthPWM = 32767;
					else
						*mtr->Hrdw.pPulseWidthPWM = -32767;

				}	

			}	
			else if(mtr->msc.PID.enable){
				
				if(mtr->msc.PID.request == 3 || mtr->msc.PID.request == 4)	// Read and Write Parameters request
					mtr->msc.PID.request = 0;	
					
				//*mtr->Hrdw.pPulseWidthPWM = (INT)(mtr->msc.PID.Y * 327.670 * mtr->Cfg.mtr_dir  );  //32767.0 max PWM permissible value

				*mtr->Hrdw.pPulseWidthPWM = (INT)(mtr->msc.PID.Y * 327.670);
				
			}
			
			else // Hold or Idle
				*mtr->Hrdw.pPulseWidthPWM = 0;
			/**/
		}

				
}



