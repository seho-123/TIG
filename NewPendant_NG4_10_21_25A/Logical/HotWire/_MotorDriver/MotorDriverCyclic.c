/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: MotorDriver
 * File: MotorDriverCyclic.c
 * Author: Administrator
 * Created: February 26, 2013
 ********************************************************************
 * Implementation of program MotorDriver
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif



#include <..\GlobDef.h>
#include <..\GlobDecl.h>

#include <..\..\Lib\Statelib.c>
#include <..\..\lib\xstring.c>

#include <..\..\lib\WSI_types.h>
#include <..\..\lib\WSI_lib.c>


#include <float.h>
#include <limits.h>

_LOCAL float Mtr_Absolute_Max_Vel[NUM_MOTORS];

BOOL ErrorState(MTR_obj *m);




/** Variable declarations for first order simulation of motor**/
//_LOCAL  LCRPT1_typ  lowPassLCRPT1;


_LOCAL int loopsToDisableThreshold;


_LOCAL float hSet;
_LOCAL float lSet;
_LOCAL float limitWindow;
_LOCAL float speedError;

_LOCAL UINT intTime , _intTime;
_LOCAL INT  difTime , difTick;
_LOCAL UINT intTick , _intTick ;

_LOCAL float taskClock;
_LOCAL UINT myNUM_MOTORS;


/********************************/

void _INIT MotorDriverInit(void)


{int i; //loop var	
	
	myNUM_MOTORS = NUM_MOTORS;
	
	hSet = 10;
	lSet = 0.0;
	limitWindow = 0.01;
	

	loopsToDisableThreshold = 2;



	/* Parameters for low pass for first order simulation of motor
	  lowPassLCRPT1.enable = 1;
	  lowPassLCRPT1.t	   = 0.3;	//seconds 
	  lowPassLCRPT1.y_set  = 0.0;
	  lowPassLCRPT1.set    = 0;
	
	*/

	


	//set to 1 for testing
	for (i=0; i<NUM_MOTORS; i++) {  //motors 0 through 4

		//motors[i].Parameters.SpeedSetpoint = 10.0;
		//motors[i].Parameters.OnRampTime = 1.0;
		//motors[i].Parameters.OffRampTime = 2.0;

		sm_init(&motors[i].Command.command_sm);  //set up state machines

		motors[i].Command.command_sm.Step = Stopped_Motor_State;

		//motors[i].HardwareConfig.TaskTimeInSec= 0.02; // sec

		motors[i].Hardware.PeriodDurationPWM = 50; // motors[i].HardwareConfig.TaskTimeInSec *4;
		
		motors[i].Hardware.StartCurrentMeasurement = 1;

		//motors[i].HardwareConfig.EngUnit = 10;

		//motors[i].HardwareConfig.RatioNum = 1;
		//motors[i].HardwareConfig.RatioNum = 1;

		
		/* Parameters for Ramp controller */
		//additional settings made in cyclic
		motors[i].Misc.Ramp.enable  = 1; 		

		motors[i].Misc.Ramp.dy_up = 500.0;  //1// time in seconds from 0 to max velocity
		motors[i].Misc.Ramp.dy_down = motors[i].Misc.Ramp.dy_up  ;  // 10.0 = inch per min^2
		

  		switch (i) {
				
				case Wf_0_Motor:

					motors[i].HardwareConfig.EngUnit = (1.0 / WF_INCHES_PER_REV)* (WF_GBOX_NUM / WF_GBOX_DEN) * WF_ENCODER_PER_REV; // counts per inch
					motors[i].HardwareConfig.MtrDir = 1;
					Mtr_Absolute_Max_Vel[i] = 20 * WF_INCHES_PER_REV; //max 20 rps of output shaft
					motors[i].Misc.Ramp.y_max   = WFSPEED_MAX ; // WFSPEED_MAX
					motors[i].Misc.PIDParameters.Kp= 1.0; //proportional gain / was 0.5
					motors[i].Misc.PIDParameters.Tn= 0.1; //intergral action time					
					motors[i].Misc.PIDParameters.Kfbk = 0; //windup dampenin
					
					break;
					
				case Wf_1_Motor:
		
					motors[i].HardwareConfig.EngUnit = (1.0 / WF_INCHES_PER_REV)* (WF_GBOX_NUM / WF_GBOX_DEN) * WF_ENCODER_PER_REV; // counts per inch
					motors[i].HardwareConfig.MtrDir = -1;
					Mtr_Absolute_Max_Vel[i] = 20 * WF_INCHES_PER_REV; //max 20 rps of output shaft
					motors[i].Misc.Ramp.y_max   = WFSPEED_MAX ; // WFSPEED_MAX
					motors[i].Misc.PIDParameters.Kp= 1.0; //proportional gain / was 0.5
					motors[i].Misc.PIDParameters.Tn= 0.1; //intergral action time					
					motors[i].Misc.PIDParameters.Kfbk = 0; //windup dampenin
					
					break;
					
				case Wp_0_Motor:

					motors[i].HardwareConfig.EngUnit = (1.0)* (WP_GBOX_NUM / WP_GBOX_DEN) * WP_ENCODER_PER_REV;
					motors[i].HardwareConfig.MtrDir = 1;
					Mtr_Absolute_Max_Vel[i] = 20; // max 20 rps of output shaft
					motors[i].Misc.Ramp.y_max   =  WPFREQ_MAX;
					motors[i].Misc.PIDParameters.Kp= 1.0;//0.4; //proportional gain / was 0.6
					motors[i].Misc.PIDParameters.Tn= 1.0;//0.3.1; //intergral action time
					motors[i].Misc.PIDParameters.Kfbk = 0.01; //windup dampenin
					
					break;
					
				case Wp_1_Motor:

					motors[i].HardwareConfig.EngUnit = (1.0)* (WP_GBOX_NUM / WP_GBOX_DEN) * WP_ENCODER_PER_REV;
					motors[i].HardwareConfig.MtrDir = -1;
					Mtr_Absolute_Max_Vel[i] = 20; // max 20 rps of output shaft
					motors[i].Misc.Ramp.y_max   =  WPFREQ_MAX;
					motors[i].Misc.PIDParameters.Kp= 1.0;//0.4; //proportional gain / was 0.6
					motors[i].Misc.PIDParameters.Tn= 1.0;//0.3.1; //intergral action time
					motors[i].Misc.PIDParameters.Kfbk = 0.01; //windup dampenin

					break;

				
		
		}

		motors[i].Misc.Ramp.y_min   = motors[i].Misc.Ramp.y_max *(-1); //
	
		/*********************************************/
				
		/* Parameters for PID controller */
		motors[i].Misc.PID.enable = 1;
		motors[i].Misc.PID.pPar = &motors[i].Misc.PIDParameters;	
		motors[i].Misc.PID.request = 3; // Read parameters from PIDParameters	
		motors[i].Misc.PIDParameters.Tv= 0; //derivative action time
		motors[i].Misc.PIDParameters.Y_max= 1.0;  //max manipulated variable
		motors[i].Misc.PIDParameters.Y_min= -1.0; //max manipulated variable

	}
}




void _CYCLIC MotorDriverCyclic(void)
{
UINT status;

	int i , n; //loop var
#ifdef SMALL_CPU


	/** Real time elapsed **/
	intTick = TIM_ticks();
	intTime = TIM_musec();	

	difTime = intTime - _intTime ;
	difTick = intTick - _intTick;
	
	if(difTime > 0 && difTick > 0)
		taskClock = (float)difTick * 0.01 + (float)difTime * 0.000001; 

	_intTick = intTick;
	_intTime = intTime;

#else

	taskClock = 0.0024;

#endif
	
		
	/**********************/
	
/*****************************************/	
	
for (i=0; i<myNUM_MOTORS; i++) {

/***************************************************/

		if(ErrorState(&motors[i]))
			motors[i].Command.command_sm.Step = Error_Motor_State;

		motors[i].Hardware.ClearError = ((motors[i].Command.command_sm.Step == Error_Motor_State) && machine.AckErrors);
		

		switch(motors[i].Command.command_sm.Step){

			case Stopped_Motor_State: sm_text("RunState: Stopped_Motor_State" , &motors[i].Command.command_sm);


				if(motors[i].Command.command_sm.Step != motors[i].Command.command_sm.oldStep){
					
					if (motors[i].Parameters.OffRampTime > 0) {
						motors[i].Misc.Ramp.dy_down = motors[i].Parameters.SpeedSetpoint / motors[i].Parameters.OffRampTime;
					} 
					else {
						motors[i].Misc.Ramp.dy_down = DBL_MAX;
					}
					
					SmTimerStart(&motors[i].Command.command_sm ,motors[i].Parameters.OffDelayTime);
				}	

				else if(SmTimerDone(&motors[i].Command.command_sm)){
					
					//calculate ramp rate					
					
					motors[i].Misc.Ramp.x = 0;
					motors[i].CommandVelocity = (motors[i].Misc.Ramp.y)
												 * motors[i].HardwareConfig.CalSlope +	motors[i].HardwareConfig.CalOffset;


					if(motors[i].Misc.Ramp.y == 0)
						motors[i].Misc.PID.enable = 0;
								
			
						switch (i) {
			
							case Wf_0_Motor:
							case Wf_1_Motor:
								
								motors[i].CommandVelocity = motors[i].CommandVelocity / SEC_PER_MIN;

							break;	
								
							case Wp_0_Motor:
							case Wp_1_Motor:

							break;	
								
							default:

							break;								
								
						}


			
				}

				break;	

			case Running_Motor_State: sm_text("RunState: Running_Motor_State" , &motors[i].Command.command_sm);

				if(motors[i].Command.command_sm.Step != motors[i].Command.command_sm.oldStep){

					if(!motors[i].Misc.PID.enable){
						motors[i].Misc.PID.enable = 1;
						motors[i].Misc.PID.request = 3;

					}

					//calculate ramp rate
					if (motors[i].Parameters.OnRampTime > 0) {
						motors[i].Misc.Ramp.dy_up = motors[i].Parameters.SpeedSetpoint / motors[i].Parameters.OnRampTime;
					} 
					else {
						motors[i].Misc.Ramp.dy_up = DBL_MAX;
					}

					
										
					SmTimerStart(&motors[i].Command.command_sm ,motors[i].Parameters.OnDelayTime);

				}
				else if(SmTimerDone(&motors[i].Command.command_sm)){					

					motors[i].Misc.Ramp.x =  motors[i].Parameters.SpeedSetpoint;

				}
				else{

					motors[i].Misc.Ramp.x = 0;

				}	
					
								
				//add boost
				if (hardware.Inputs[Boost_1_Button] == true) {

					//motors[i].CommandVelocity = motors[i].Parameters.SpeedSetpoint + motors[i].Parameters.BoostLevel1SetPoint;
					//motors[i].Misc.Ramp.x = motors[i].CommandVelocity * motors[i].HardwareConfig.CalSlope +  motors[i].HardwareConfig.CalOffset;						

					motors[i].CommandVelocity = (motors[i].Misc.Ramp.y   +  motors[i].Parameters.BoostLevel1SetPoint)
						 * motors[i].HardwareConfig.CalSlope +  motors[i].HardwareConfig.CalOffset;

				}
				 else if (hardware.Inputs[Boost_2_Button] == true) {

					//motors[i].CommandVelocity = motors[i].Parameters.SpeedSetpoint + motors[i].Parameters.BoostLevel2SetPoint;
					//motors[i].Misc.Ramp.x = motors[i].CommandVelocity * motors[i].HardwareConfig.CalSlope +  motors[i].HardwareConfig.CalOffset;

					motors[i].CommandVelocity = (motors[i].Misc.Ramp.y   +  motors[i].Parameters.BoostLevel2SetPoint)
						 * motors[i].HardwareConfig.CalSlope +  motors[i].HardwareConfig.CalOffset;

					
				}
				 //no boost
				else {

					//motors[i].CommandVelocity = motors[i].Parameters.SpeedSetpoint;
					//motors[i].Misc.Ramp.x = motors[i].CommandVelocity * motors[i].HardwareConfig.CalSlope +  motors[i].HardwareConfig.CalOffset;

					
					motors[i].CommandVelocity = (motors[i].Misc.Ramp.y)
												 * motors[i].HardwareConfig.CalSlope +	motors[i].HardwareConfig.CalOffset;

				}	


				//check limits
				switch (i) {

					case Wf_0_Motor:
					case Wf_1_Motor:


						if (motors[i].CommandVelocity > WFSPEED_MAX) {
							motors[i].CommandVelocity = WFSPEED_MAX;
						} else if (motors[i].CommandVelocity < WFSPEED_MIN) {
							motors[i].CommandVelocity = WFSPEED_MIN;
						}

						
						// per sec instead per min
						motors[i].CommandVelocity = motors[i].CommandVelocity / SEC_PER_MIN;

						break;

					case Wp_0_Motor:	
					case Wp_1_Motor:
					
						if (motors[i].CommandVelocity > WPFREQ_MAX) {
							motors[i].CommandVelocity = WPFREQ_MAX;
						} else if (motors[i].CommandVelocity < WPFREQ_MIN) {
							motors[i].CommandVelocity = WPFREQ_MIN;
						}				

						break;								

					default:

						break;	

				}


				break;	


			case Jogging_Motor_State:
			case JoggingRev_Motor_State:
				
				sm_text("JogState: Jogging Forward or Reverse" , &motors[i].Command.command_sm);

				if(!motors[i].Misc.PID.enable){
						motors[i].Misc.PID.enable = 1;
						motors[i].Misc.PID.request = 3;

				}
				
				switch (i) {
							
					case Wf_0_Motor:
					case Wf_1_Motor:

						if(motors[i].Command.command_sm.Step == JoggingRev_Motor_State)
							motors[i].CommandVelocity = WF_REV_JOG_SPEED * (-1.0);
						else							
							motors[i].CommandVelocity = WF_JOG_SPEED;

						motors[i].CommandVelocity = motors[i].CommandVelocity / SEC_PER_MIN;
						break;
		

					default:

						motors[i].CommandVelocity = 0;

						break;	
				
				}
				
				

				break;

			case Error_Motor_State: sm_text("RunState: Error_Motor_State" , &motors[i].Command.command_sm);

				motors[i].Misc.Ramp.dy_down = DBL_MAX;  //fast stop
				motors[i].Misc.Ramp.x = 0;

				motors[i].Misc.PID.enable = 0;

				motors[i].CommandVelocity = 0;

								
				switch (i) {
		
					case Wf_0_Motor:

						machine.ErrorCode = WF_0_Motor_High_Amps_Error;

						break;
		
					case Wp_0_Motor:

						machine.ErrorCode = WP_0_Motor_High_Amps_Error;

						break;

					case Wf_1_Motor:

						machine.ErrorCode = WF_1_Motor_High_Amps_Error;
						
						break;

					case Wp_1_Motor:
						
						machine.ErrorCode = WP_1_Motor_High_Amps_Error;
				
						break;

					default:

						break;	

				}				

				break;	


			default: sm_text("Dead End" , &motors[i].Command.command_sm);

				//Should never get here
				motors[i].Misc.Ramp.dy_down = 0;  //fast stop
				motors[i].Misc.Ramp.x = 0;

				motors[i].Misc.PID.enable = 0;

				switch (i) {
		
						case Wf_0_Motor:

						
							break;
			
						case Wp_0_Motor:

					
							break;

						case Wf_1_Motor:


							break;

						case Wp_1_Motor:

				
							break;

						default:

							break;	

				}	

				break;	


		}
		sm_serv(&motors[i].Command.command_sm);

	
		
		/**************************************************************************/

		/*Feedback Evaluation */
		motors[i].Misc.DeltaPosition = motors[i].Hardware.Counter - motors[i].Misc.LastPosition ;
		motors[i].Misc.DeltaPosition = motors[i].Misc.DeltaPosition * motors[i].HardwareConfig.EncDir;
		motors[i].Misc.LastPosition = motors[i].Hardware.Counter;		 

		if(motors[i].Misc.DeltaPosition > 32768)
			motors[i].Misc.DeltaPosition -= 65536;
		else if(motors[i].Misc.DeltaPosition <= -32768)
			motors[i].Misc.DeltaPosition+= 65536;

		//motors[i].CurrentVelocity = ((float)(motors[i].Misc.DeltaPosition * motors[i].HardwareConfig.MtrDir) / motors[i].HardwareConfig.EngUnit) / taskClock;   //(ips or hz)
		motors[i].CurrentVelocity = ((float)motors[i].Misc.DeltaPosition / motors[i].HardwareConfig.EngUnit) / taskClock;
		/* Ramp Contriol */
  		//LCRRamp(&motors[i].Misc.Ramp);
  		AzzRamp(&motors[i].Misc.Ramp);

		
		/* PID Control */			
		//motors[i].Misc.PID.W = motors[i].CommandVelocity / Mtr_Absolute_Max_Vel[i] * 1.0;	

		motors[i].Misc.PID.W = (motors[i].CommandVelocity / Mtr_Absolute_Max_Vel[i]) * (float)motors[i].HardwareConfig.MtrDir;	
		/**/
		motors[i].Misc.PID.X = motors[i].CurrentVelocity / Mtr_Absolute_Max_Vel[i];		
		
		//LCRSlimPID(&motors[i].Misc.PID);
		AzzSlimPID(&motors[i].Misc.PID);

		
		if(motors[i].Misc.PID.enable){
			
			if(motors[i].Misc.PID.request == 3 || motors[i].Misc.PID.request == 4)	// Read and Write Parameters request
				motors[i].Misc.PID.request = 0;	

			//motors[i].Hardware.PulseWidthPWM = (INT)(motors[i].Misc.PID.Y * 32767.0) * motors[i].HardwareConfig.MtrDir;  //32767.0 max PWM permissible value
			motors[i].Hardware.PulseWidthPWM = (INT)(motors[i].Misc.PID.Y * 32767.0);  //32767.0 max PWM permissible value
		}/**/
		else
			motors[i].Hardware.PulseWidthPWM = 0;
		/**/
	
		
		


	}


	
	

	
}

void _EXIT MotorDriverExit(void)
{
	/* TODO: Add code here */

	int i; //loop var
	
	for (i=0; i<4; i++) {  //motors 0 through 4
	
		motors[i].Hardware.StartCurrentMeasurement = 1;

		/* Cleanup */
		motors[i].Misc.Ramp.enable = 0;
		//motors[i].Misc.Trajectory.enable= 0;
		motors[i].Misc.PID.enable = 0;
		//motors[i].Misc.FIFODelay.enable = 0;
		//lowPassLCRPT1.enable = 0;

	}
	
}


BOOL ErrorState(MTR_obj *m){

	return (m->Hardware.CurrentError || m->Hardware.OverCurrentError || m->Hardware.PWMError);
}



