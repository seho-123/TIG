/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: PowerSupplyDriver
 * File: PowerSupplyDriverCyclic.c
 * Author: Administrator
 * Created: February 27, 2013
 ********************************************************************
 * Implementation of program PowerSupplyDriver
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\GlobDef.h>
#include <..\GlobDecl.h>

#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_types.h>
#include <..\..\Lib\WSI_lib.c>



#include <math.h>
#include <float.h>

_LOCAL 	AzzScal_typ AcopianVoltsCmdScale;

_LOCAL plcbit KK_test;


/** Variable declarations for testing only**/


void _INIT PowerSupplyDriverInit(void)
{
	/* TODO: Add code here */

	int i; //loop var
	

	//set to 1 for testing
	for (i=0; i<NUM_POWER_SUPPLIES; i++) {  //PSs 0 through 2


		//powerSupplies[i].Parameters.AmpsSetpoint = 10.0;
		//powerSupplies[i].Parameters.OnRampTime = 1.0;
		//powerSupplies[i].Parameters.OffRampTime = 2.0;

		sm_init(&powerSupplies[i].Command.command_sm); //set up state machines

		powerSupplies[i].Command.command_sm.Step = Stopped_Power_Supply_State;

		//powerSupplies[i].HardwareConfig.TaskTimeInSec= 0.0052;

		
		/* Parameters for Ramp controller */
		//additional settings made in cyclic
		powerSupplies[i].Misc.Ramp.enable  = 1;
  		powerSupplies[i].Misc.Ramp.y_max   = 1000.0; //not used
  		powerSupplies[i].Misc.Ramp.y_min   = -1000.0; //not used
		powerSupplies[i].Misc.Ramp.max_limit = 0; //not used
  		powerSupplies[i].Misc.Ramp.max_limit = 0; //not used	

		//powerSupplies[i].HardwareConfig.TaskTimeInSec= 0.01;

		
	}

	
	AcopianVoltsCmdScale.x1 = 0;
	AcopianVoltsCmdScale.y1 = ACCOPIAN_PS_VOLTS_AT_MIN_CURRENT;
	
	AcopianVoltsCmdScale.x2 = ACCOPIAN_PS_AMPS_AT_MAX_COMMAND_V;
	AcopianVoltsCmdScale.y1 = ACCOPIAN_PS_VOLTS_AT_MAX_CURRENT;

	
}



void _CYCLIC PowerSupplyDriverCyclic(void)
{int n , i;
float y , x;
float maxAmp;

/* TODO: Add code here */


for (i=0; i<NUM_POWER_SUPPLIES; i++) {
	


	switch(powerSupplies[i].Command.command_sm.Step){

		case Stopped_Power_Supply_State: sm_text("RunState: Stopped_Power_Supply_State" , &powerSupplies[i].Command.command_sm);

			if(powerSupplies[i].Command.command_sm.Step != powerSupplies[i].Command.command_sm.oldStep){

				//calculate ramp rate
				if (powerSupplies[i].Parameters.OffRampTime > 0) {
					powerSupplies[i].Misc.Ramp.dy_down = powerSupplies[i].Parameters.AmpsSetpoint / powerSupplies[i].Parameters.OffRampTime;
				}
				else {
					powerSupplies[i].Misc.Ramp.dy_down = DBL_MAX;
				}
				SmTimerStart(&powerSupplies[i].Command.command_sm , powerSupplies[i].Parameters.OffDelayTime);

			}
			else if(SmTimerDone(&powerSupplies[i].Command.command_sm)){
				

				powerSupplies[i].Misc.Ramp.x = 0;

				
				if (powerSupplies[i].Misc.Ramp.y == 0.0) {

					switch (i) {
			
						case PrimaryPS:

							hardware.Outputs[PrimaryPS_Enable] = false;
						
							break;
				
						case HotwirePS:

							hardware.Outputs[HotwirePS_Enable] = false;
						
							break;

						default:

							break;	

					}


				}
			}

			break;	

		case Running_Power_Supply_State: sm_text("RunState: Running_Power_Supply_State" , &powerSupplies[i].Command.command_sm);

			if(powerSupplies[i].Command.command_sm.Step != powerSupplies[i].Command.command_sm.oldStep){

				//calculate ramp rate
				if (powerSupplies[i].Parameters.OnRampTime > 0) {
					powerSupplies[i].Misc.Ramp.dy_up = powerSupplies[i].Parameters.AmpsSetpoint / powerSupplies[i].Parameters.OnRampTime;
				} else {
					powerSupplies[i].Misc.Ramp.dy_up = DBL_MAX;
				}

				SmTimerStart(&powerSupplies[i].Command.command_sm , powerSupplies[i].Parameters.OnDelayTime);

			}

			else if(SmTimerDone(&powerSupplies[i].Command.command_sm)){

				
				//add boost
				if (hardware.Inputs[Boost_1_Button] == true) {

					powerSupplies[i].CommandAmps  = powerSupplies[i].Parameters.AmpsSetpoint + powerSupplies[i].Parameters.BoostLevel1SetPoint;
					powerSupplies[i].Misc.Ramp.x = powerSupplies[i].CommandAmps  * powerSupplies[i].HardwareConfig.CalSlope + powerSupplies[i].HardwareConfig.CalOffset;

					//check limits

					/*
					switch (i) {
			
						case PrimaryPS:

							if (powerSupplies[i].Misc.Ramp.x > ARCAMPS_MAX) {
								powerSupplies[i].Misc.Ramp.x = ARCAMPS_MAX;
							} else if (powerSupplies[i].Misc.Ramp.x < ARCAMPS_MIN) {
								powerSupplies[i].Misc.Ramp.x = ARCAMPS_MIN;
							}

							break;
				
						case HotwirePS:

							if (powerSupplies[i].Misc.Ramp.x > HWAMPS_MAX) {
								powerSupplies[i].Misc.Ramp.x = HWAMPS_MAX;
							} else if (powerSupplies[i].Misc.Ramp.x < HWAMPS_MIN) {
								powerSupplies[i].Misc.Ramp.x = HWAMPS_MIN;
							}				

							break;

						default:

							break;	

					}
					*/


				} else if (hardware.Inputs[Boost_2_Button] == true) {

					powerSupplies[i].CommandAmps = powerSupplies[i].Parameters.AmpsSetpoint + powerSupplies[i].Parameters.BoostLevel2SetPoint;
					powerSupplies[i].Misc.Ramp.x = powerSupplies[i].CommandAmps  * powerSupplies[i].HardwareConfig.CalSlope + powerSupplies[i].HardwareConfig.CalOffset;

					//check limits

					//check limits
					/*
					switch (i) {
			
						case PrimaryPS:

							if (powerSupplies[i].Misc.Ramp.x > ARCAMPS_MAX) {
								powerSupplies[i].Misc.Ramp.x = ARCAMPS_MAX;
							} else if (powerSupplies[i].Misc.Ramp.x < ARCAMPS_MIN) {
								powerSupplies[i].Misc.Ramp.x = ARCAMPS_MIN;
							}

							break;
				
						case HotwirePS:

							if (powerSupplies[i].Misc.Ramp.x > HWAMPS_MAX) {
								powerSupplies[i].Misc.Ramp.x = HWAMPS_MAX;
							} else if (powerSupplies[i].Misc.Ramp.x < HWAMPS_MIN) {
								powerSupplies[i].Misc.Ramp.x = HWAMPS_MIN;
							}				

							break;

						default:

							break;	

					}
					*/


				} 
				else {
				
					powerSupplies[i].CommandAmps = powerSupplies[i].Parameters.AmpsSetpoint;
					powerSupplies[i].Misc.Ramp.x = powerSupplies[i].CommandAmps * powerSupplies[i].HardwareConfig.CalSlope + powerSupplies[i].HardwareConfig.CalOffset;

				}

				switch (i) {
			
						case PrimaryPS:

							hardware.Outputs[PrimaryPS_Enable] = true;
							
							break;
				
						case HotwirePS:

							hardware.Outputs[HotwirePS_Enable] = true;
							
							break;

						default:

							break;	

				}
			}

			break;	

		case Error_Power_Supply_State: sm_text("RunState: Error_Power_Supply_State" , &powerSupplies[i].Command.command_sm);

			powerSupplies[i].Misc.Ramp.dy_down = DBL_MAX;  //fast stop
			powerSupplies[i].Misc.Ramp.x = 0;

			switch (i) {
		
					case PrimaryPS:

						hardware.Outputs[PrimaryPS_Enable] = false;
						
						break;
			
					case HotwirePS:

						hardware.Outputs[HotwirePS_Enable] = false;
						
						break;

					default:

						break;	

			}

			break;	


		default: sm_text("Dead End" , &powerSupplies[i].Command.command_sm);

			//Should never get here
			powerSupplies[i].Misc.Ramp.dy_down = 0;  //fast stop
			powerSupplies[i].Misc.Ramp.x = 0;

			switch (i) {
		
					case PrimaryPS:

						hardware.Outputs[PrimaryPS_Enable] = false;
						
						break;
			
					case HotwirePS:

						hardware.Outputs[HotwirePS_Enable] = false;
						
						break;

					default:

						break;	

			}

			break;	


		}
		sm_serv(&powerSupplies[i].Command.command_sm);

/*
		switch (i) {
		
			case PrimaryPS:

				switch (machine.PrimaryPSType) {

					case Miller_Type: 	maxAmp = MILLER_PS_AMPS_AT_MAX_COMMAND_V; 	break;
					case GTII_Type: 	maxAmp = GTII_PS_AMPS_AT_MAX_COMMAND_V; 	break;
					case GTVI_Type:		maxAmp = GTVI_PS_AMPS_AT_MAX_COMMAND_V; 	break;
					case LINC_Type: 	maxAmp = LINC_PS_AMPS_AT_MAX_COMMAND_V; 	break;

				}
				
				break;
	
			case HotwirePS:

				maxAmp = HWAMPS_MAX;
				
				break;

			default:

				maxAmp = 0,0;

				break;	

		}

				
		if(powerSupplies[i].Misc.Ramp.x > maxAmp)
			powerSupplies[i].Misc.Ramp.x = maxAmp;

*/			
		
		/* Ramp Contriol */
	  	AzzRamp(&powerSupplies[i].Misc.Ramp);


		//calculate 0-10 command voltage
		
		powerSupplies[i].Hardware.AnologCommandVoltage = powerSupplies[i].Misc.Ramp.y / powerSupplies[i].HardwareConfig.AmpsPerVoltCommand;

		//convert to int and update AO module
		
		switch (i) {
			
			case PrimaryPS:

				
					x = (powerSupplies[i].Hardware.AnologCommandVoltage / MAX_ANALOG_OUT_VALUE) * MAX_ANALOG_OUT_INT_VALUE;
					
					if( x > 32767.0)
						x = 32767.0;
					else if( x < 0)
						x = 0;
					
					hardware.AnalogOutputs[PrimaryPS_AO] = (INT)(x);

					
				
				break;
				
			case HotwirePS:

					x = (powerSupplies[i].Hardware.AnologCommandVoltage / MAX_ANALOG_OUT_VALUE) * MAX_ANALOG_OUT_INT_VALUE;
					
					if( x > 32767.0)
						x = 32767.0;
					else if( x < 0)
						x = 0;

					hardware.AnalogOutputs[HotwirePS_AO] = (INT)(x); 

					//powerSupplies[i].Misc.Ramp.x = powerSupplies[i].CommandAmps  * powerSupplies[i].HardwareConfig.CalSlope + powerSupplies[i].HardwareConfig.CalOffset;
					 
					AcopianVoltsCmdScale.x = (powerSupplies[i].Misc.Ramp.x - powerSupplies[i].HardwareConfig.CalOffset)/ powerSupplies[i].HardwareConfig.CalSlope;
					AzzScal(&AcopianVoltsCmdScale);

					
					y =(AcopianVoltsCmdScale.y  * ACCOPIAN_PS_VOLTS_CMD_SLOPE / MAX_ANALOG_OUT_VALUE);
					
					if(y > 1.0)
						y = 1.0;
						
					hardware.AnalogOutputs[AcopianVoltCmd_AO] = (INT)(y * MAX_ANALOG_OUT_INT_VALUE); 
					

				break;	

		}


	
	}



}



void _EXIT PowerSupplyDriverExit(void)
{
	/* TODO: Add code here */
}
