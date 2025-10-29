/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: Master
 * File: Master.c
 * Author: wsi
 * Created: September 25, 2012
 *Upgrades: 
 * 15 Jan 2015 (CK) -Outputs[Remote_Fwd_Rev_Sol_And_Rel] and  Inputs[RemoteRevDir] evaluation in GTVI mode
 ********************************************************************
 * Implementation of program Master
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobDef.h>
#include <..\GlobDecl.h>


//#include <..\Lib\Statelib.c>
//#include <..\lib\xstring.c>




//_LOCAL BOOL testBit;




HARDWARE_obj _hardware;  //temp copy
_LOCAL UINT debounceCount[16] , timerCountOutputs[16] ;
_LOCAL UINT debounceThreshold;

_LOCAL UINT fastBlinkCnt, fastBlinkCntMax;
_LOCAL BOOL FastBlink;
_LOCAL UINT DeadTime;



void _INIT Master_INIT(void)
{
	sm_init(&master_sm); //set up state machines

	master_sm.Step = PostCheck;
	debounceThreshold = 10; // was 5

	fastBlinkCntMax = 3;
	//testBit = false;

}


void _CYCLIC Master_CYCLIC(void)
{int i;

	//testBit = false;

	machine.EditMode = 0;

	for(i = 0 ; i < NUM_LCD ; i++){

		if(menu[i].Edit)
			machine.EditMode = 1;

	}

	// Fast Blink

	fastBlinkCnt++;
	if(fastBlinkCnt > fastBlinkCntMax){
		fastBlinkCnt = 0;
		FastBlink = !FastBlink;
	}
	
	// ESTOP fault
#ifdef SMALL_CPU
	machine.EstopFault  = 0;

#else
	machine.EstopFault  = !hardware.Inputs[ControlModule_Estop_Active];

#endif	

	if(machine.EstopFault )
		master_sm.Step = Ready; DeadTime = 0xFFFF; 

	if(machine.ErrorCode)		
		master_sm.Step = Error;
	
	// Simulation of Reset Estop and PL  on remote
	hardware.Outputs[Estop_Reset]= hardware.Inputs[WF_Suitcase_EstopReset];
	hardware.Outputs[Estop_Good_PL] = !machine.EstopFault;

	/* Arc On */
	hardware.Outputs[Arc_On_PL] = (hardware.Outputs[PrimaryPS_Enable] &&
		((powerSupplies[PrimaryPS].Command.command_sm.Step == Running_Power_Supply_State) || FastBlink));
		

	switch(master_sm.Step){


		case PostCheck: sm_text("RunState: PostCheck" , &master_sm);


				//Do setup...

				//if setup complete then go to Ready (HS_obj)
				//master_sm.Step = Ready;

				//else go to error
				//master_sm.Step = Error;

				
				master_sm.Step = Ready;
				
			break;	

		case Ready: sm_text("RunState: Ready" , &master_sm );

				for(i = 0 ; i < NUM_MOTORS ; i++)
					motors[i].Command.command_sm.Step = Stopped_Motor_State;				

				for(i = 0 ; i < NUM_POWER_SUPPLIES; i++)
					powerSupplies[i].Command.command_sm.Step = Stopped_Power_Supply_State;

				
				if(machine.PrimaryPSType == GTII_Type){

						
					if(timerCountOutputs[GTII_SeqStop_Latch]>10) // 10 x 50ms
						hardware.Outputs[GTII_SeqStop_Latch] = false;
					else
						timerCountOutputs[GTII_SeqStop_Latch]++;

					hardware.Outputs[GTII_SeqStart_Latch] = false;
					
				}else if(machine.PrimaryPSType == GTVI_Type){
				
					if(timerCountOutputs[GTII_SeqStart_Latch]>10) // 10 x 50ms
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					else
						timerCountOutputs[GTII_SeqStart_Latch]++;

					hardware.Outputs[GTII_SeqStop_Latch] = false;
					
				}
				else{ //  Miller or Lincoln
					hardware.Outputs[GTII_SeqStop_Latch] = false;	
					hardware.Outputs[GTII_SeqStart_Latch] = false;
				}

					
				

				debounceCount[Jog_Button]++;
				
				debounceCount[Arc_Button]++;

				debounceCount[Boost_1_Button]++;

				debounceCount[Boost_2_Button]++;

				debounceCount[WF_Button]++;

				debounceCount[RemoteSeqStart]++;

				debounceCount[GTII_SeqStart_Input]++;


				// ESTOP fault - no action from buttons)
				if(machine.EstopFault || (DeadTime < 100) ){ //  Dead Time  10 sek
					DeadTime++;
					break;

				}					
				
				if (machine.ControlMode == Manual && hardware.Inputs[Jog_Button] && debounceCount[Jog_Button] > debounceThreshold && hardware.Outputs[PrimaryPS] == false && hardware.Outputs[HotwirePS] == false && motors[Wf_0_Motor].Misc.PID.enable == false && motors[Wp_0_Motor].Misc.PID.enable == false && motors[Wf_1_Motor].Misc.PID.enable == false && motors[Wp_1_Motor].Misc.PID.enable == false) { //level trigger
				
					debounceCount[Jog_Button] = 0;

					master_sm.Step = Jogging;
									
				} else if (!machine.EditMode  && machine.ControlMode == Manual && !_hardware.Inputs[Arc_Button] && hardware.Inputs[Arc_Button] && debounceCount[Arc_Button] > debounceThreshold) { //leading edge trigger

						
					hardware.Outputs[GTII_SeqStart_Latch] = true;
					timerCountOutputs[GTII_SeqStart_Latch] = 0;
				
					master_sm.Step = Arc_Enabled;

					debounceCount[Arc_Button] = 0;
		
				} else if (machine.ControlMode == Manual && hardware.Inputs[Boost_2_Button] && debounceCount[Boost_2_Button] > debounceThreshold && hardware.Outputs[PrimaryPS] == false && hardware.Outputs[HotwirePS] == false && motors[Wf_0_Motor].Misc.PID.enable == false && motors[Wp_0_Motor].Misc.PID.enable == false && motors[Wf_1_Motor].Misc.PID.enable == false && motors[Wp_1_Motor].Misc.PID.enable == false) { //level trigger
				
					debounceCount[Boost_2_Button] = 0;

					master_sm.Step = JoggingRev;

				} else if (machine.ControlMode == Manual && hardware.Inputs[Boost_1_Button] && debounceCount[Boost_1_Button] > debounceThreshold && hardware.Outputs[PrimaryPS] == false && hardware.Outputs[HotwirePS] == false && motors[Wf_0_Motor].Misc.PID.enable == false && motors[Wp_0_Motor].Misc.PID.enable == false && motors[Wf_1_Motor].Misc.PID.enable == false && motors[Wp_1_Motor].Misc.PID.enable == false) { //level trigger
				
					debounceCount[Boost_1_Button] = 0;

					master_sm.Step = Jogging;
									
				} else if (!machine.EditMode && machine.ControlMode == Automatic && hardware.Inputs[RemoteSeqStart] && debounceCount[RemoteSeqStart] > debounceThreshold) { //level trigger
				
					debounceCount[RemoteSeqStart] = 0;
					
					hardware.Outputs[GTII_SeqStart_Latch] = true;
					timerCountOutputs[GTII_SeqStart_Latch] = 0;				

					master_sm.Step = RemoteRunning;

				} else if (!machine.EditMode && machine.ControlMode == FourAxis && hardware.Inputs[RemoteSeqStart] && debounceCount[RemoteSeqStart] > debounceThreshold) { //level trigger
								
					debounceCount[RemoteSeqStart] = 0;
					
					hardware.Outputs[GTII_SeqStart_Latch] = true;
					timerCountOutputs[GTII_SeqStart_Latch] = 0; 			

					master_sm.Step = RemoteRunning;
					

				//GTII Interface code
				} else if (!machine.EditMode && machine.ControlMode == Automatic && hardware.Inputs[GTII_SeqStart_Input] && debounceCount[GTII_SeqStart_Input] > debounceThreshold) { //level trigger
				
					debounceCount[GTII_SeqStart_Input] = 0;

					hardware.Outputs[GTII_SeqStart_Latch] = true;
					timerCountOutputs[GTII_SeqStart_Latch] = 0;				

					master_sm.Step = RemoteRunning;

				}  //end GTII interface code


				if (machine.ControlMode == Automatic) {

					/*

					if (hardware.Inputs[RemoteRevDir]) {

						hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = true;
					
					} else {

						hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;
					} 
					*/

					if (hardware.Inputs[GTII_WFAuto_Input]) {

						hardware.Outputs[GTII_WFAuto_Latch] = true;

					} else {

						hardware.Outputs[GTII_WFAuto_Latch] = false;
						
					}


				}/*
				else {
				
				

					hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;

				}
				*/

			break;	

		case Arc_Enabled: sm_text("RunState: Arc_Enabled" , &master_sm);

				if(machine.PrimaryPSType == GTII_Type 
					|| machine.PrimaryPSType == GTVI_Type){
					
					if(timerCountOutputs[GTII_SeqStart_Latch]>10) // 10 x 50ms
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					else
						timerCountOutputs[GTII_SeqStart_Latch]++;
					
				}
				
				if (machine.ControlMode == Manual) {

					for(i = 0 ; i < NUM_MOTORS ; i++)
				 		motors[i].Command.command_sm.Step = Stopped_Motor_State;
					
					powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
				 	
				} else {

					for(i = 0 ; i < NUM_MOTORS ; i++)	
				 		motors[i].Command.command_sm.Step = Stopped_Motor_State;
			
					powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

				}
				
				debounceCount[Arc_Button]++;
				
				debounceCount[WF_Button]++;

				if (!_hardware.Inputs[Arc_Button] && hardware.Inputs[Arc_Button] && debounceCount[Arc_Button] > debounceThreshold) { //leading edge trigger

					debounceCount[Arc_Button] = 0;

					/*
					//GTVI Code
					if(machine.PrimaryPSType == GTVI_Type){
						hardware.Outputs[GTII_SeqStart_Latch] = true;
					} else {
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					}
					//GTVI Manual Code end
					*/
					if(machine.PrimaryPSType == GTVI_Type){
						hardware.Outputs[GTII_SeqStart_Latch] = true;
						hardware.Outputs[GTII_SeqStop_Latch] = false;
						timerCountOutputs[GTII_SeqStart_Latch] = 0; 
						timerCountOutputs[GTII_SeqStop_Latch] = 0; 
					}
					else{
						hardware.Outputs[GTII_SeqStart_Latch] = false;					
						hardware.Outputs[GTII_SeqStop_Latch] = true;
						timerCountOutputs[GTII_SeqStart_Latch] = 0; 
						timerCountOutputs[GTII_SeqStop_Latch] = 0;
					}
					
					master_sm.Step = Ready; DeadTime = 0;
		
				} else if (!_hardware.Inputs[WF_Button] && hardware.Inputs[WF_Button] && debounceCount[WF_Button] > debounceThreshold) { //leading edge trigger

					debounceCount[WF_Button] = 0;

					master_sm.Step = Running;
		
				} 

			break;	

		case Jogging: sm_text("RunState: Jogging" , &master_sm);

				if (machine.ControlMode == Manual) {

				 	motors[Wf_0_Motor].Command.command_sm.Step = Jogging_Motor_State;
					motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
					motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
					motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
				
					powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
				 	
				} else {

					for(i = 0 ; i < NUM_MOTORS ; i++)
				 		motors[i].Command.command_sm.Step = Stopped_Motor_State;
									
					powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

				}

				debounceCount[Jog_Button]++;

				debounceCount[Boost_1_Button]++;

				if ((!hardware.Inputs[Jog_Button] && debounceCount[Jog_Button] > debounceThreshold) && (!hardware.Inputs[Boost_1_Button] && debounceCount[Boost_1_Button] > debounceThreshold)) { //level trigger
				
					debounceCount[Jog_Button] = 0;

					debounceCount[Boost_1_Button] = 0;

					master_sm.Step = Ready; DeadTime = 0xFFFF;
									
				}
				
			break;

		case JoggingRev: sm_text("RunState: JoggingRev" , &master_sm);

				if (machine.ControlMode == Manual) {

				 	motors[Wf_0_Motor].Command.command_sm.Step = JoggingRev_Motor_State;
					motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
					motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
					motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
				
					powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
				 	
				} else {

					for(i = 0 ; i < NUM_MOTORS ; i++)
				 		motors[i].Command.command_sm.Step = Stopped_Motor_State;
									
					powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
					powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

				}
				
				debounceCount[Boost_2_Button]++;

				if (!hardware.Inputs[Boost_2_Button] && debounceCount[Boost_2_Button] > debounceThreshold) { //level trigger
				
					debounceCount[Boost_2_Button] = 0;

					master_sm.Step = Ready; DeadTime = 0xFFFF;
									
				}
				
			break;	


		case Running: sm_text("RunState: Running" , &master_sm);

				switch (machine.RunMode) {

        			case  WF_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WP_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            		case  HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        					
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_WP_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;


        						break;

        					default:
        						break;

        				}

            			break;

            		case  WP_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_WP_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        						motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;

								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

								powerSupplies[PrimaryPS].Command.command_sm.Step = Running_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						

								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					default:
        						break;

        				}

            			break;

            
        			default:
            			break;
    			}


				debounceCount[Arc_Button]++;
				
				debounceCount[WF_Button]++;

				if (!_hardware.Inputs[Arc_Button] && hardware.Inputs[Arc_Button] && debounceCount[Arc_Button] > debounceThreshold) { //leading edge trigger

					debounceCount[Arc_Button] = 0;

					if(machine.PrimaryPSType == GTVI_Type)
						hardware.Outputs[GTII_SeqStart_Latch] = true;
					else
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					
					hardware.Outputs[GTII_SeqStop_Latch] = true;
					timerCountOutputs[GTII_SeqStop_Latch] = 0; 			
					

					master_sm.Step = Ready; DeadTime = 0;
		
				} else if (!_hardware.Inputs[WF_Button] && hardware.Inputs[WF_Button] && debounceCount[WF_Button] > debounceThreshold) { //leading edge trigger

					debounceCount[WF_Button] = 0;

					master_sm.Step = Arc_Enabled;
		
				} 

			break;	

		case RemoteRunning: sm_text("RunState: RemoteRunning" , &master_sm);


				if(machine.PrimaryPSType == GTII_Type 
					|| machine.PrimaryPSType == GTVI_Type){
					
					if(timerCountOutputs[GTII_SeqStart_Latch]>10) // 10 x 50ms
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					else
						timerCountOutputs[GTII_SeqStart_Latch]++;
					
				}

				
				if ((machine.ControlMode == Automatic) || (machine.ControlMode == FourAxis)){

					/*
					if (hardware.Inputs[RemoteRevDir]) {

						hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = true;
					
					} else {

						hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;
					} 
					*/

					if (hardware.Outputs[GTII_SeqStart_Latch]) {

						hardware.Outputs[GTII_SeqStart_Latch] = false;

					}

					if (hardware.Inputs[GTII_WFAuto_Input]) {

						hardware.Outputs[GTII_WFAuto_Latch] = true;

					} else {

						hardware.Outputs[GTII_WFAuto_Latch] = false;
						
					}
						

				} 
				/*else {

					hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;

				}*/
				

				switch (machine.RunMode) {

        			case  WF_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)	
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:	

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing

									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;
        							
									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {

									motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								if (hardware.Inputs[RemoteRevDir]) {
        									motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									motors[Wf_1_Motor].Command.command_sm.Step = Running_Motor_State;
        								} else {
        									motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        									motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								}

        							} else {
        								motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							}
        							

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						}

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WP_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here

								
								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
        							
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;        							

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {

        							motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;

        							if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								if (hardware.Inputs[RemoteRevDir]) {
        									motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									motors[Wp_1_Motor].Command.command_sm.Step = Running_Motor_State;
        								} else {
        									motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;
        									motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								}

        							} else {
        								motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							}

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						}
        						
        						break;

        					default:
        						break;

        				}

            			break;

            		case  HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here
								
								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
        							
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;
        							
									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {

									
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;        							

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;

									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        							} else {
        								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
        							}
        						}

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_WP_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here
								
								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
        							
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;
        							
									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {			

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
        						}

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here

								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
									
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;        							

									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {

									motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
									motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;										

									
									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								if (hardware.Inputs[RemoteRevDir]) {
											
											motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;        									
											motors[Wf_1_Motor].Command.command_sm.Step = Running_Motor_State;
        									
        								} else {
        								
        									motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;        									
											motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									
											
        								}
										
        							} else {        							
        								
										motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							}
        							

        							powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;

									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        							} else {
        								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
        							}

        						}

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WP_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here
								
								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;        						

								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
        							
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;
        							
									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {

        							motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;        																	

									
									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								if (hardware.Inputs[RemoteRevDir]) {
											
											motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;        									
											motors[Wp_1_Motor].Command.command_sm.Step = Running_Motor_State;
        									
        								} else {
        								
        									motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;        									
											motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									
											
        								}
										
        							} else {        							
        								
										motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							}

        							powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;

									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        							} else {
        								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
        							}

        						}

        						break;

        					default:
        						break;

        				}

            			break;

            		case  WF_And_WP_And_HW_Mode:

        				switch (machine.ControlMode) {

        					case  Manual:

        						//should not be here
								
								for(i = 0 ; i < NUM_MOTORS ; i++)
        							motors[i].Command.command_sm.Step = Stopped_Motor_State;
        						
								powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						break;

        					case Automatic:
							case FourAxis:

        						if (hardware.Inputs[RemoteSeqTest]) {

        							//test mode ... do nothing
        							
									for(i = 0 ; i < NUM_MOTORS ; i++)
        								motors[i].Command.command_sm.Step = Stopped_Motor_State;
        							
									powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
									powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

        						} else {



        							if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								if (hardware.Inputs[RemoteRevDir]) {
											
											motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
											motors[Wf_1_Motor].Command.command_sm.Step = Running_Motor_State;
        									motors[Wp_1_Motor].Command.command_sm.Step = Running_Motor_State;
											
        								} else {
        								
        									motors[Wf_0_Motor].Command.command_sm.Step = Running_Motor_State;
        									motors[Wp_0_Motor].Command.command_sm.Step = Running_Motor_State;
											motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        									motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
											
        								}
										
        							} else {
        							
        								motors[Wp_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wp_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
										motors[Wf_0_Motor].Command.command_sm.Step = Stopped_Motor_State;
        								motors[Wf_1_Motor].Command.command_sm.Step = Stopped_Motor_State;
        							}

        							powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;

									if (hardware.Inputs[RemoteWfEnable] && !hardware.Inputs[RemoteSeqTest] || hardware.Inputs[GTII_WFAuto_Input]) {

        								powerSupplies[HotwirePS].Command.command_sm.Step = Running_Power_Supply_State;

        							} else {
        								powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;
        							}
        						}
        						
        						break;

        					default:
        						break;

        				}

            			break;

            
        			default:
            			break;
    			}


				debounceCount[RemoteSeqStart]++;
				debounceCount[GTII_SeqStop_Input]++;
				
				if (_hardware.Inputs[RemoteSeqStart] && !hardware.Inputs[RemoteSeqStart] && debounceCount[RemoteSeqStart] > debounceThreshold) { //falling edge trigger

					debounceCount[RemoteSeqStart] = 0;

					if(machine.PrimaryPSType == GTVI_Type)
						hardware.Outputs[GTII_SeqStart_Latch] = true;
					else
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					
					hardware.Outputs[GTII_SeqStop_Latch] = true;
					timerCountOutputs[GTII_SeqStop_Latch] = 0; 	

					master_sm.Step = Ready; DeadTime = 0;
		
				}

				if (_hardware.Inputs[GTII_SeqStop_Input] && !hardware.Inputs[GTII_SeqStop_Input] && debounceCount[GTII_SeqStop_Input] > debounceThreshold) { //leading edge trigger

					debounceCount[RemoteSeqStart] = 0;

					if(machine.PrimaryPSType == GTVI_Type)
						hardware.Outputs[GTII_SeqStart_Latch] = true;
					else
						hardware.Outputs[GTII_SeqStart_Latch] = false;
					
					hardware.Outputs[GTII_SeqStop_Latch] = true;
					timerCountOutputs[GTII_SeqStop_Latch] = 0; 	

					master_sm.Step = Ready; DeadTime = 0;
		
				}


			break;	

		case Error: sm_text("RunState: Error" , &master_sm);

				//Stop everything and log error message and wait for clear bit from operator interface

				for(i = 0 ; i < NUM_MOTORS ; i++)
					motors[i].Command.command_sm.Step = Stopped_Motor_State;
				
				powerSupplies[PrimaryPS].Command.command_sm.Step = Stopped_Power_Supply_State;
				powerSupplies[HotwirePS].Command.command_sm.Step = Stopped_Power_Supply_State;

				//hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;

				if(machine.ErrorCode == No_Error)
					master_sm.Step = Ready; DeadTime = 0xFFFF;

			break;	

			

		default: sm_text("Dead End" , &master_sm);

				//Should never get here

		break;	


	}
	sm_serv(&master_sm);


	///////////////in Automatic or Four Axis mode always copy dir input signal to Sol output ////////////////////

	
	if ((machine.ControlMode == Automatic) || (machine.ControlMode == FourAxis)){

		hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = hardware.Inputs[RemoteRevDir];	
			

	} else {

		hardware.Outputs[Remote_Fwd_Rev_Sol_And_Rel] = false;

	}


	//////////////////////////////////////////////////////////////

	
	for (i=0; i<16; i++) {

		_hardware.Inputs[i] = hardware.Inputs[i];

	}

}





