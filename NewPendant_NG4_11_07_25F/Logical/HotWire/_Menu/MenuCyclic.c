/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Menu
 * File: MenuCyclic.c
 * Author: Administrator
 * Created: March 03, 2013
 ********************************************************************
 * Implementation of program Menu
 ********************************************************************/




#include <bur/plctypes.h>
#include <bur/plc.h>       /* macros for B&R PLC programming */
//#include <AsMath.h>        /* prototypes for AsMath-Libary */
#include <AsBrMath.h> 
 
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>

	 
#include <..\GlobDef.h>
#include <..\GlobDecl.h>
	 
#include <..\..\Lib\Statelib.c>
#include <..\..\lib\xstring.c>


_LOCAL SerialPortDriver_typ LCD_RS232[NUM_LCD];



//prototypes
void loadDefaults();
void loadUserValues();
void saveUserValues();
void Send_LCD_Cmd(SerialPortDriver_typ *rs, UINT cmd , char par);
void LinePad(char* l , UINT len);
void AssignHardwareParam();


_LOCAL plcstring varSting[NUM_LCD][21];  // simply large enough - don't forget the extra byte needed for the trailing '\0'
_LOCAL plcstring varStingBlink[NUM_LCD][21];  // simply large enough - don't forget the extra byte needed for the trailing '\0'
                
_LOCAL plcstring statusString[NUM_LCD][21]; // simply large enough - don't forget the extra byte needed for the trailing '\0'
                
char arcStatusString[4]; //"ARC" or "   " - don't forget the extra byte needed for the trailing '\0'
char remStatusString[4]; //"REM" or "   " - don't forget the extra byte needed for the trailing '\0'
char wfStatusString[3]; //"WF" or "  " - don't forget the extra byte needed for the trailing '\0'
char wpStatusString[3]; //"WP" or "  " - don't forget the extra byte needed for the trailing '\0'
char hwStatusString[3]; //"HW" or "  " - don't forget the extra byte needed for the trailing '\0'
                
char twirlString[2];
char twirl;
int twirlCount;



_LOCAL INT ticks;

_LOCAL INT TempCounter;
_LOCAL INT Position[NUM_LCD];


HARDWARE_obj _hardware;  //temp copy
_LOCAL UINT debounceCount[NUM_LCD];
_LOCAL UINT debounceThreshold;

_LOCAL UINT blinkThreshold;
_LOCAL UINT blinkCounter[NUM_LCD];

BOOL _MenuButton[NUM_LCD];
UINT QuickNavDlyCnt[NUM_LCD];


// Only for test
_LOCAL char LCD_cmd_param;
_LOCAL UINT LCD_cmd_indx;
_LOCAL BOOL LCD_send_Cmd;
_LOCAL UINT LCD_Init_Step;

_LOCAL BOOL KK_test;
  
UINT _ErrorCode;


void _INIT MenuInit(void)
{int i;
 
	/* TODO: Add code here */

	machine.LoadDefaultsMode = LOAD_DEFAULTS_MODE_INITIAL_SP;
	
	//initialize twirl
	twirl = '<';
	twirlCount = 0;
	
	ticks = 4;


	blinkThreshold = 5; //blinker, 5*cycle time = 5 * 0.0504 = approx .5 sec 

	debounceThreshold = 2;

	
	if (userValues.FirstTimeThrough == false) {

		loadDefaults();

		loadUserValues();

	} else {

		loadUserValues();

	}

	for(i = 0 ; i < NUM_LCD ; i++){
		menu[i].Edit = false;
		machine.LoadDefaultsMode = LOAD_DEFAULTS_MODE_INITIAL_SP;
		menu[i].DisplayCode = DISPCODE_INITIAL_SP;
		menu[i].MenuMode = User; //Configuration;

		Position[i] = 0;
		blinkCounter[i] = 0;

	}

	
	//SerialPortInit(&LCD_RS232[0] , "IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
	//SerialPortInit(&LCD_RS232[1] , "SS1.IF1.ST5.IF1.ST4.IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
	//SerialPortInit(&LCD_RS232[0] , "IF6.ST2.IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
	//LCD_Init_Step = 0;
	

}


void _CYCLIC MenuCyclic(void)
{	int i , k;
	char *p;

	float arcMax;

	/* TODO: Add code here */

	if(KK_test){
		KK_test = 0;
		loadUserValues();

	}

	/*CK added to restart RS when project selected */
	if(HotWireProjectCMD == 1){

		HotWireProjectCMD = 0;
		
		SerialPortInit(&LCD_RS232[0] , "IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
		SerialPortInit(&LCD_RS232[1] , "SS1.IF1.ST5.IF1.ST4.IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
					
		//SerialPortInit(&LCD_RS232[0] , "IF6.ST2.IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");
		LCD_Init_Step = 0;

	}

	/*******************************************/	



	// Auto navigate to error code 	
	if(machine.ErrorCode && !_ErrorCode ){

		for(i = 0; i < NUM_LCD ; i++)
			menu[i].DisplayCode = ErrorCode_Ack;


	}
	_ErrorCode = machine.ErrorCode;

	// Set machine.AckErrors 
	machine.AckErrors = 0;

	for(i = 0; i < NUM_LCD ; i++){

		// Port is not opened
		if(LCD_RS232[i].Open.status!=0)
			break;

		// Blinking
		blinkCounter[i] ++;
		if (blinkCounter[i] >= blinkThreshold) {

			menu[i].Blink = !menu[i].Blink;
			blinkCounter[i] = 0;
		
		}

		// Set machine.AckErrors 
		if(menu[i].MenuButton && menu[i].DisplayCode == ErrorCode_Ack)
			machine.AckErrors = 1;

	
		/* Edit Controls */

		debounceCount[i]++;


		if (!_MenuButton[i] && menu[i].MenuButton && debounceCount[i] > debounceThreshold ) { //leading edge trigger
					
			debounceCount[i] = 0;

			//if currentRunState is Ready or Error anything can be edited, else only wpFreqSetPoint, wfSpeedSetPoint, arcAmpsSetPoint and hwAmpsSetPoint, arcCalFactor, wfCalFactor, hwCalFactor, wfVLoopPGain, wfVLoopIGain, wfVLoopDGain can be edited
	    
			if (master_sm.Step == Ready || master_sm.Step == Error) {
			
				//if currentDisplayCode == ErrorCode_Ack then just clear code, else go into edit mode
			
				//clear error code and set currentRunState to Ready
				if (menu[i].DisplayCode == ErrorCode_Ack) {
				
					machine.ErrorCode = No_Error;

					master_sm.Step = Ready;
				
				} else {
				
					//Push to edit value, push again to set current value
					if (menu[i].Edit == false) {  //} & State) {  removed because of rising edge check above
					
						//set position and index to zero 
						Position[i] = 0; //CPhidgetEncoder_setPosition[i](ENC,0, 0);
					
						//start editing
						menu[i].Edit = true;

					} else if (menu[i].Edit == true) {  //} & State) {  removed because of rising edge check above

						menu[i].Edit = false;
					
						//save changes to file unless ErrorCode_Ack or LoadDefaults_Setting
						if (!(menu[i].DisplayCode == ErrorCode_Ack || menu[i].DisplayCode == LoadDefaults_Setting)) {
						
							//save to userValues
							saveUserValues();
					
						}
				
					}
				}
			
				//check for load defaults
				if (menu[i].DisplayCode == LoadDefaults_Setting &  machine.LoadDefaultsMode == Yes_Load) {
				
					//load defaultValues
					loadDefaults();

					loadUserValues();

				
					machine.LoadDefaultsMode = No_Load;

				}
			
			} else {
			
				if (menu[i].DisplayCode == WpFreq_Setting || menu[i].DisplayCode == WfSpeed_Setting || menu[i].DisplayCode == ArcAmps_Setting ||  menu[i].DisplayCode == HwAmps_Setting || menu[i].DisplayCode == ArcCalSlope_Setting || menu[i].DisplayCode == WfCalSlope_Setting || menu[i].DisplayCode == HwCalSlope_Setting || menu[i].DisplayCode == ArcCalOffset_Setting || menu[i].DisplayCode == WfCalOffset_Setting || menu[i].DisplayCode == HwCalOffset_Setting ) {

					//Push to edit value, push again to set current value
					if (menu[i].Edit == false) {  //} & State) {  removed because of rising edge check above
					
						//set position and index to zero 
						Position[i] = 0; //CPhidgetEncoder_setPosition[i](ENC,0, 0);
					
						//start editing
						menu[i].Edit = true;

					} else if (menu[i].Edit == true) {  //} & State) {  removed because of rising edge check above

						menu[i].Edit = false;
					
						//save changes to file unless ErrorCode_Ack or LoadDefaults_Setting
						if (!(menu[i].DisplayCode == ErrorCode_Ack || menu[i].DisplayCode == LoadDefaults_Setting)) {
						
							//save to userValues
							saveUserValues();
					
						}
				
					}
				
				
				}
				
			} 
										
		}

		/* Quick navigation to top  */
		if(menu[i].MenuButton){
			
			if(QuickNavDlyCnt[i] > 20){
				menu[i].Edit = 0;
				menu[i].DisplayCode = WpFreq_Setting;
			}

			QuickNavDlyCnt[i]++;
		}	
		else
			QuickNavDlyCnt[i] = 0;
		
		/***********************/

		/* Only One LCD can be in Edit Mode */
		if(menu[i].Edit){

			for(k = 0; k < NUM_LCD ; k++){

				if(i!=k && menu[k].Edit){
					menu[i].Edit = 0;
					break;	
				}
			}
		}

		

		/* Output Generation */
		//TempCounter = menu[i].Counter;
		//Position[i] = (Position[i] + (menu[i].Counter - menu[i].LastCounter));  // /4
		//menu[i].LastCounter = TempCounter;

		Position[i] = (Position[i] + (menu[i].Counter - menu[i].LastCounter));  // /4
		menu[i].LastCounter = menu[i].Counter ;

		
		
		if (menu[i].Edit == true) {

			switch (menu[i].DisplayCode) {

				case WpFreq_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wp_0_Motor].Parameters.SpeedSetpoint = (brmfloor((motors[Wp_0_Motor].Parameters.SpeedSetpoint + WPFREQ_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.SpeedSetpoint > WPFREQ_MAX) {
							motors[Wp_0_Motor].Parameters.SpeedSetpoint = WPFREQ_MAX;
						}else if (motors[Wp_0_Motor].Parameters.SpeedSetpoint < WPFREQ_MIN) {
							motors[Wp_0_Motor].Parameters.SpeedSetpoint = WPFREQ_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
												
					} else if (Position[i] >= ticks) {
						
						motors[Wp_0_Motor].Parameters.SpeedSetpoint = (brmfloor((motors[Wp_0_Motor].Parameters.SpeedSetpoint - WPFREQ_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.SpeedSetpoint > WPFREQ_MAX) {
							motors[Wp_0_Motor].Parameters.SpeedSetpoint = WPFREQ_MAX;
						}else if (motors[Wp_0_Motor].Parameters.SpeedSetpoint < WPFREQ_MIN) {
							motors[Wp_0_Motor].Parameters.SpeedSetpoint = WPFREQ_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
								
					} 
					break;
					
				case WfSpeed_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wf_0_Motor].Parameters.SpeedSetpoint = (brmfloor((motors[Wf_0_Motor].Parameters.SpeedSetpoint + WFSPEED_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.SpeedSetpoint > WFSPEED_MAX) {
							motors[Wf_0_Motor].Parameters.SpeedSetpoint = WFSPEED_MAX;
						}else if (motors[Wf_0_Motor].Parameters.SpeedSetpoint < WFSPEED_MIN) {
							motors[Wf_0_Motor].Parameters.SpeedSetpoint = WFSPEED_MIN;
						}
						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
						
					} else if (Position[i] >= ticks) {
						
						motors[Wf_0_Motor].Parameters.SpeedSetpoint = (brmfloor((motors[Wf_0_Motor].Parameters.SpeedSetpoint - WFSPEED_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.SpeedSetpoint > WFSPEED_MAX) {
							motors[Wf_0_Motor].Parameters.SpeedSetpoint = WFSPEED_MAX;
						}else if (motors[Wf_0_Motor].Parameters.SpeedSetpoint < WFSPEED_MIN) {
							motors[Wf_0_Motor].Parameters.SpeedSetpoint = WFSPEED_MIN;
						}
						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
						
					} 
					break;
					
				case ArcAmps_Setting:

					switch (machine.PrimaryPSType) {

    					case Miller_Type: 		arcMax = MILLER_PS_AMPS_AT_MAX_COMMAND_V; 		break;
						case GTII_Type: 		arcMax = GTII_PS_AMPS_AT_MAX_COMMAND_V; 		break;
						case GTVI_Type:			arcMax = GTVI_PS_AMPS_AT_MAX_COMMAND_V; 		break;						
						case LINCS350_Type: 	arcMax = LINCS350_PS_AMPS_AT_MAX_COMMAND_V; 	break;
						case LINCS500_Type: 	arcMax = LINCS500_PS_AMPS_AT_MAX_COMMAND_V; 	break;

					}
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.AmpsSetpoint + ARCAMPS_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;						
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.AmpsSetpoint > arcMax) {
							powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = arcMax;
						}else if (powerSupplies[PrimaryPS].Parameters.AmpsSetpoint < ARCAMPS_MIN) {
							powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = ARCAMPS_MIN;
						}
						
					} else if (Position[i] >= ticks) {
						
						powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.AmpsSetpoint - ARCAMPS_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.AmpsSetpoint > arcMax) {
							powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = arcMax;
						}else if (powerSupplies[PrimaryPS].Parameters.AmpsSetpoint < ARCAMPS_MIN) {
							powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = ARCAMPS_MIN;
						}
						
					} 
					break;
					
				case HwAmps_Setting:
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[HotwirePS].Parameters.AmpsSetpoint = (brmfloor((powerSupplies[HotwirePS].Parameters.AmpsSetpoint + HWAMPS_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.AmpsSetpoint > HWAMPS_MAX) {
							powerSupplies[HotwirePS].Parameters.AmpsSetpoint = HWAMPS_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.AmpsSetpoint < HWAMPS_MIN) {
							powerSupplies[HotwirePS].Parameters.AmpsSetpoint = HWAMPS_MIN;
						}
						
					} else if (Position[i] >= ticks) {
						
						powerSupplies[HotwirePS].Parameters.AmpsSetpoint = (brmfloor((powerSupplies[HotwirePS].Parameters.AmpsSetpoint - HWAMPS_INCR) * ROUND_OF_TO_2_DECIMAL) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.AmpsSetpoint > HWAMPS_MAX) {
							powerSupplies[HotwirePS].Parameters.AmpsSetpoint = HWAMPS_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.AmpsSetpoint < HWAMPS_MIN) {
							powerSupplies[HotwirePS].Parameters.AmpsSetpoint = HWAMPS_MIN;
						}
						
					} 
					break;
	                
				case BoostLevel_1_Variable_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.Boost1Type == ARC_WF_WP_HW_Boost_Type) {
							machine.Boost1Type = 0;
						} else {
							machine.Boost1Type = machine.Boost1Type + 1;
						}
	                    
					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.Boost1Type == ARC_Boost_Type) {
							machine.Boost1Type = ARC_WF_WP_HW_Boost_Type;
						} else {
							machine.Boost1Type = machine.Boost1Type - 1;
						}
	                    
					}
					break;
	                
				case BoostLevel_2_Variable_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.Boost2Type == ARC_WF_WP_HW_Boost_Type) {
							machine.Boost2Type = 0;
						} else {
							machine.Boost2Type = machine.Boost2Type + 1;
						}
	                                        
					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.Boost2Type == ARC_Boost_Type) {
							machine.Boost2Type = ARC_WF_WP_HW_Boost_Type;
						} else {
							machine.Boost2Type = machine.Boost2Type - 1;
						}
	                                        
					}
					break;
	            
				case ArcAmpsBoostLevel_1_Setting:
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint + BOOST_LEVEL_1_ARC_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint > ARCAMPS_BOOST_MAX) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = ARCAMPS_BOOST_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint < ARCAMPS_BOOST_MIN) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = ARCAMPS_BOOST_MIN;
						}
	                    
					} else if (Position[i] >= ticks) {
						
						powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint - BOOST_LEVEL_1_ARC_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint > ARCAMPS_BOOST_MAX) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = ARCAMPS_BOOST_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint < ARCAMPS_BOOST_MIN) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = ARCAMPS_BOOST_MIN;
						}
						
					} 
					break;
	                
				case ArcAmpsBoostLevel_2_Setting:
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint + BOOST_LEVEL_2_ARC_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint > ARCAMPS_BOOST_MAX) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = ARCAMPS_BOOST_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint < ARCAMPS_BOOST_MIN) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = ARCAMPS_BOOST_MIN;
						}
	                    
					} else if (Position[i] >= ticks) {
						
						powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = (brmfloor((powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint - BOOST_LEVEL_2_ARC_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint > ARCAMPS_BOOST_MAX) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = ARCAMPS_BOOST_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint < ARCAMPS_BOOST_MIN) {
							powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = ARCAMPS_BOOST_MIN;
						}
						
					} 
					break;    
	                
				case WfSpeedBoostLevel_1_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = (brmfloor((motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint + BOOST_LEVEL_1_WF_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint > WFSPEED_BOOST_MAX) {
							motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = WFSPEED_BOOST_MAX;
						}else if (motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint < WFSPEED_BOOST_MIN) {
							motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = WFSPEED_BOOST_MIN;
						}

						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
	                    
					} else if (Position[i] >= ticks) {
						
						motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = (brmfloor((motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint - BOOST_LEVEL_1_WF_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint > WFSPEED_BOOST_MAX) {
							motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = WFSPEED_BOOST_MAX;
						}else if (motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint < WFSPEED_BOOST_MIN) {
							motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = WFSPEED_BOOST_MIN;
						}

						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
						
					} 
					break;
	                
				case WfSpeedBoostLevel_2_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = (brmfloor((motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint + BOOST_LEVEL_2_WF_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint > WFSPEED_BOOST_MAX) {
							motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = WFSPEED_BOOST_MAX;
						}else if (motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint < WFSPEED_BOOST_MIN) {
							motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = WFSPEED_BOOST_MIN;
						}

						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
	                    
					} else if (Position[i] >= ticks) {
						
						motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = (brmfloor((motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint - BOOST_LEVEL_2_WF_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint > WFSPEED_BOOST_MAX) {
							motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = WFSPEED_BOOST_MAX;
						}else if (motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint < WFSPEED_BOOST_MIN) {
							motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = WFSPEED_BOOST_MIN;
						}

						motors[Wf_1_Motor].Parameters.SpeedSetpoint = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
						
					} 
					break;
	                
				case WpFreqBoostLevel_1_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = (brmfloor((motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint + BOOST_LEVEL_1_WP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint > WPFREQ_BOOST_MAX) {
							motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = WPFREQ_BOOST_MAX;
						}else if (motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint < WPFREQ_BOOST_MIN) {
							motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = WPFREQ_BOOST_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
						
	                    
					} else if (Position[i] >= ticks) {
						
						motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = (brmfloor((motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint - BOOST_LEVEL_1_WP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint > WPFREQ_BOOST_MAX) {
							motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = WPFREQ_BOOST_MAX;
						}else if (motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint < WPFREQ_BOOST_MIN) {
							motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = WPFREQ_BOOST_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
						
					} 
					break;
	                
				case WpFreqBoostLevel_2_Setting:
					
					if (Position[i] <= -ticks) {
						
						motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = (brmfloor((motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint + BOOST_LEVEL_2_WP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint > WPFREQ_BOOST_MAX) {
							motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = WPFREQ_BOOST_MAX;
						}else if (motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint < WPFREQ_BOOST_MIN) {
							motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = WPFREQ_BOOST_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
	                    
					} else if (Position[i] >= ticks) {
						
						motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = (brmfloor((motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint - BOOST_LEVEL_2_WP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint > WPFREQ_BOOST_MAX) {
							motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = WPFREQ_BOOST_MAX;
						}else if (motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint < WPFREQ_BOOST_MIN) {
							motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = WPFREQ_BOOST_MIN;
						}

						motors[Wp_1_Motor].Parameters.SpeedSetpoint = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
						
					} 
					break;
	                
				case HwAmpsBoostLevel_1_Setting:
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = (brmfloor((powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint + BOOST_LEVEL_1_HW_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint > HWAMPS_BOOST_MAX) {
							powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = HWAMPS_BOOST_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint < HWAMPS_BOOST_MIN) {
							powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = HWAMPS_BOOST_MIN;
						}
	                    
					} else if (Position[i] >= ticks) {
						
						powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = (brmfloor((powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint - BOOST_LEVEL_1_HW_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint > HWAMPS_BOOST_MAX) {
							powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = HWAMPS_BOOST_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint < HWAMPS_BOOST_MIN) {
							powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = HWAMPS_BOOST_MIN;
						}
						
					} 
					break;
	                
				case HwAmpsBoostLevel_2_Setting:
					
					if (Position[i] <= -ticks) {
						
						powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = (brmfloor((powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint + BOOST_LEVEL_2_HW_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint > HWAMPS_BOOST_MAX) {
							powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = HWAMPS_BOOST_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint < HWAMPS_BOOST_MIN) {
							powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = HWAMPS_BOOST_MIN;
						}
	                    
					} else if (Position[i] >= ticks) {
						
						powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = (brmfloor((powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint - BOOST_LEVEL_2_HW_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint > HWAMPS_BOOST_MAX) {
							powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = HWAMPS_BOOST_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint < HWAMPS_BOOST_MIN) {
							powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = HWAMPS_BOOST_MIN;
						}
						
					} 
					break;
	                
	            
				//---- delays
					
				case WfStartDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wf_0_Motor].Parameters.OnDelayTime = (brmfloor((motors[Wf_0_Motor].Parameters.OnDelayTime + WFSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OnDelayTime > WFSTARTDELAY_MAX) {
							motors[Wf_0_Motor].Parameters.OnDelayTime = WFSTARTDELAY_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OnDelayTime < WFSTARTDELAY_MIN) {
							motors[Wf_0_Motor].Parameters.OnDelayTime = WFSTARTDELAY_MIN;
						}

						motors[Wf_1_Motor].Parameters.OnDelayTime = motors[Wf_0_Motor].Parameters.OnDelayTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wf_0_Motor].Parameters.OnDelayTime = (brmfloor((motors[Wf_0_Motor].Parameters.OnDelayTime - WFSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OnDelayTime > WFSTARTDELAY_MAX) {
							motors[Wf_0_Motor].Parameters.OnDelayTime = WFSTARTDELAY_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OnDelayTime < WFSTARTDELAY_MIN) {
							motors[Wf_0_Motor].Parameters.OnDelayTime = WFSTARTDELAY_MIN;
						}
						motors[Wf_1_Motor].Parameters.OnDelayTime = motors[Wf_0_Motor].Parameters.OnDelayTime;
					
					} 
					break;
							
					
				case WpStartDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wp_0_Motor].Parameters.OnDelayTime = (brmfloor((motors[Wp_0_Motor].Parameters.OnDelayTime + WPSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OnDelayTime > WPSTARTDELAY_MAX) {
							motors[Wp_0_Motor].Parameters.OnDelayTime = WPSTARTDELAY_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OnDelayTime < WPSTARTDELAY_MIN) {
							motors[Wp_0_Motor].Parameters.OnDelayTime = WPSTARTDELAY_MIN;
						}

						motors[Wp_1_Motor].Parameters.OnDelayTime = motors[Wp_0_Motor].Parameters.OnDelayTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wp_0_Motor].Parameters.OnDelayTime = (brmfloor((motors[Wp_0_Motor].Parameters.OnDelayTime - WPSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OnDelayTime > WPSTARTDELAY_MAX) {
							motors[Wp_0_Motor].Parameters.OnDelayTime = WPSTARTDELAY_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OnDelayTime < WPSTARTDELAY_MIN) {
							motors[Wp_0_Motor].Parameters.OnDelayTime = WPSTARTDELAY_MIN;
						}

						motors[Wp_1_Motor].Parameters.OnDelayTime = motors[Wp_0_Motor].Parameters.OnDelayTime;
					
					} 
					break;
					
				case HwStartDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[HotwirePS].Parameters.OnDelayTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OnDelayTime + HWSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OnDelayTime > HWSTARTDELAY_MAX) {
							powerSupplies[HotwirePS].Parameters.OnDelayTime = HWSTARTDELAY_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OnDelayTime < HWSTARTDELAY_MIN) {
							powerSupplies[HotwirePS].Parameters.OnDelayTime = HWSTARTDELAY_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[HotwirePS].Parameters.OnDelayTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OnDelayTime - HWSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OnDelayTime > HWSTARTDELAY_MAX) {
							powerSupplies[HotwirePS].Parameters.OnDelayTime = HWSTARTDELAY_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OnDelayTime < HWSTARTDELAY_MIN) {
							powerSupplies[HotwirePS].Parameters.OnDelayTime = HWSTARTDELAY_MIN;
						}
						
					} 
					break;
					
				case ArcStartDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
											
						powerSupplies[PrimaryPS].Parameters.OnDelayTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OnDelayTime + ARCSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OnDelayTime > ARCSTARTDELAY_MAX) {
							powerSupplies[PrimaryPS].Parameters.OnDelayTime = ARCSTARTDELAY_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OnDelayTime < ARCSTARTDELAY_MIN) {
							powerSupplies[PrimaryPS].Parameters.OnDelayTime = ARCSTARTDELAY_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
											
						powerSupplies[PrimaryPS].Parameters.OnDelayTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OnDelayTime - ARCSTARTDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);

						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OnDelayTime > ARCSTARTDELAY_MAX) {
							powerSupplies[PrimaryPS].Parameters.OnDelayTime = ARCSTARTDELAY_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OnDelayTime < ARCSTARTDELAY_MIN) {
							powerSupplies[PrimaryPS].Parameters.OnDelayTime = ARCSTARTDELAY_MIN;
						}
						
					} 
					break;
					
				case WfStopDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wf_0_Motor].Parameters.OffDelayTime = (brmfloor((motors[Wf_0_Motor].Parameters.OffDelayTime + WFSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OffDelayTime > WFSTOPDELAY_MAX) {
							motors[Wf_0_Motor].Parameters.OffDelayTime = WFSTOPDELAY_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OffDelayTime < WFSTOPDELAY_MIN) {
							motors[Wf_0_Motor].Parameters.OffDelayTime = WFSTOPDELAY_MIN;
						}

						motors[Wf_1_Motor].Parameters.OffDelayTime = motors[Wf_0_Motor].Parameters.OffDelayTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wf_0_Motor].Parameters.OffDelayTime = (brmfloor((motors[Wf_0_Motor].Parameters.OffDelayTime - WFSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OffDelayTime > WFSTOPDELAY_MAX) {
							motors[Wf_0_Motor].Parameters.OffDelayTime = WFSTOPDELAY_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OffDelayTime < WFSTOPDELAY_MIN) {
							motors[Wf_0_Motor].Parameters.OffDelayTime = WFSTOPDELAY_MIN;
						}

						motors[Wf_1_Motor].Parameters.OffDelayTime = motors[Wf_0_Motor].Parameters.OffDelayTime;
						
					} 
					break;	
					
				case HwStopDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[HotwirePS].Parameters.OffDelayTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OffDelayTime + HWSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OffDelayTime > HWSTOPDELAY_MAX) {
							powerSupplies[HotwirePS].Parameters.OffDelayTime = HWSTOPDELAY_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OffDelayTime < HWSTOPDELAY_MIN) {
							powerSupplies[HotwirePS].Parameters.OffDelayTime = HWSTOPDELAY_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[HotwirePS].Parameters.OffDelayTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OffDelayTime - HWSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OffDelayTime > HWSTOPDELAY_MAX) {
							powerSupplies[HotwirePS].Parameters.OffDelayTime = HWSTOPDELAY_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OffDelayTime < HWSTOPDELAY_MIN) {
							powerSupplies[HotwirePS].Parameters.OffDelayTime = HWSTOPDELAY_MIN;
						}
						
					} 
					break;
					
				case WpStopDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wp_0_Motor].Parameters.OffDelayTime = (brmfloor((motors[Wp_0_Motor].Parameters.OffDelayTime + WPSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OffDelayTime > WPSTOPDELAY_MAX) {
							motors[Wp_0_Motor].Parameters.OffDelayTime = WPSTOPDELAY_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OffDelayTime < WPSTOPDELAY_MIN) {
							motors[Wp_0_Motor].Parameters.OffDelayTime = WPSTOPDELAY_MIN;
						}

						motors[Wp_1_Motor].Parameters.OffDelayTime = motors[Wp_0_Motor].Parameters.OffDelayTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wp_0_Motor].Parameters.OffDelayTime = (brmfloor((motors[Wp_0_Motor].Parameters.OffDelayTime - WPSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OffDelayTime > WPSTOPDELAY_MAX) {
							motors[Wp_0_Motor].Parameters.OffDelayTime = WPSTOPDELAY_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OffDelayTime < WPSTOPDELAY_MIN) {
							motors[Wp_0_Motor].Parameters.OffDelayTime = WPSTOPDELAY_MIN;
						}

						motors[Wp_1_Motor].Parameters.OffDelayTime = motors[Wp_0_Motor].Parameters.OffDelayTime;
						
					} 
					break;
				
				case ArcStopDelay_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[PrimaryPS].Parameters.OffDelayTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OffDelayTime + ARCSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OffDelayTime > ARCSTOPDELAY_MAX) {
							powerSupplies[PrimaryPS].Parameters.OffDelayTime = ARCSTOPDELAY_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OffDelayTime < ARCSTOPDELAY_MIN) {
							powerSupplies[PrimaryPS].Parameters.OffDelayTime = ARCSTOPDELAY_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[PrimaryPS].Parameters.OffDelayTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OffDelayTime - ARCSTOPDELAY_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OffDelayTime > ARCSTOPDELAY_MAX) {
							powerSupplies[PrimaryPS].Parameters.OffDelayTime = ARCSTOPDELAY_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OffDelayTime < ARCSTOPDELAY_MIN) {
							powerSupplies[PrimaryPS].Parameters.OffDelayTime = ARCSTOPDELAY_MIN;
						}
						
					} 
					break;
					
				//---- ramps	
				
				case WfStartRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wf_0_Motor].Parameters.OnRampTime = (brmfloor((motors[Wf_0_Motor].Parameters.OnRampTime + WFSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OnRampTime > WFSTARTRAMP_MAX) {
							motors[Wf_0_Motor].Parameters.OnRampTime = WFSTARTRAMP_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OnRampTime < WFSTARTRAMP_MIN) {
							motors[Wf_0_Motor].Parameters.OnRampTime = WFSTARTRAMP_MIN;
						}

						motors[Wf_1_Motor].Parameters.OnRampTime = motors[Wf_0_Motor].Parameters.OnRampTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wf_0_Motor].Parameters.OnRampTime = (brmfloor((motors[Wf_0_Motor].Parameters.OnRampTime - WFSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OnRampTime > WFSTARTRAMP_MAX) {
							motors[Wf_0_Motor].Parameters.OnRampTime = WFSTARTRAMP_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OnRampTime < WFSTARTRAMP_MIN) {
							motors[Wf_0_Motor].Parameters.OnRampTime = WFSTARTRAMP_MIN;
						}
						
						motors[Wf_1_Motor].Parameters.OnRampTime = motors[Wf_0_Motor].Parameters.OnRampTime;
					} 
					break;
					
					
					
				case WpStartRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wp_0_Motor].Parameters.OnRampTime = (brmfloor((motors[Wp_0_Motor].Parameters.OnRampTime + WPSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OnRampTime > WPSTARTRAMP_MAX) {
							motors[Wp_0_Motor].Parameters.OnRampTime = WPSTARTRAMP_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OnRampTime < WPSTARTRAMP_MIN) {
							motors[Wp_0_Motor].Parameters.OnRampTime = WPSTARTRAMP_MIN;
						}
						motors[Wp_1_Motor].Parameters.OnRampTime = motors[Wp_0_Motor].Parameters.OnRampTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wp_0_Motor].Parameters.OnRampTime = (brmfloor((motors[Wp_0_Motor].Parameters.OnRampTime - WPSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OnRampTime > WPSTARTRAMP_MAX) {
							motors[Wp_0_Motor].Parameters.OnRampTime = WPSTARTRAMP_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OnRampTime < WPSTARTRAMP_MIN) {
							motors[Wp_0_Motor].Parameters.OnRampTime = WPSTARTRAMP_MIN;
						}
						motors[Wp_1_Motor].Parameters.OnRampTime = motors[Wp_0_Motor].Parameters.OnRampTime;
					
					} 
					break;
					
				case HwStartRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[HotwirePS].Parameters.OnRampTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OnRampTime + HWSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OnRampTime > HWSTARTRAMP_MAX) {
							powerSupplies[HotwirePS].Parameters.OnRampTime = HWSTARTRAMP_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OnRampTime < HWSTARTRAMP_MIN) {
							powerSupplies[HotwirePS].Parameters.OnRampTime = HWSTARTRAMP_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[HotwirePS].Parameters.OnRampTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OnRampTime - HWSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OnRampTime > HWSTARTRAMP_MAX) {
							powerSupplies[HotwirePS].Parameters.OnRampTime = HWSTARTRAMP_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OnRampTime < HWSTARTRAMP_MIN) {
							powerSupplies[HotwirePS].Parameters.OnRampTime = HWSTARTRAMP_MIN;
						}
						
					} 
					break;
					
				case ArcStartRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[PrimaryPS].Parameters.OnRampTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OnRampTime + ARCSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OnRampTime > ARCSTARTRAMP_MAX) {
							powerSupplies[PrimaryPS].Parameters.OnRampTime = ARCSTARTRAMP_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OnRampTime < ARCSTARTRAMP_MIN) {
							powerSupplies[PrimaryPS].Parameters.OnRampTime = ARCSTARTRAMP_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[PrimaryPS].Parameters.OnRampTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OnRampTime - ARCSTARTRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OnRampTime > ARCSTARTRAMP_MAX) {
							powerSupplies[PrimaryPS].Parameters.OnRampTime = ARCSTARTRAMP_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OnRampTime < ARCSTARTRAMP_MIN) {
							powerSupplies[PrimaryPS].Parameters.OnRampTime = ARCSTARTRAMP_MIN;
						}
						
					} 
					break;
					
				case WfStopRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wf_0_Motor].Parameters.OffRampTime = (brmfloor((motors[Wf_0_Motor].Parameters.OffRampTime + WFSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OffRampTime > WFSTOPRAMP_MAX) {
							motors[Wf_0_Motor].Parameters.OffRampTime = WFSTOPRAMP_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OffRampTime < WFSTOPRAMP_MIN) {
							motors[Wf_0_Motor].Parameters.OffRampTime = WFSTOPRAMP_MIN;
						}	

						motors[Wf_1_Motor].Parameters.OffRampTime = motors[Wf_0_Motor].Parameters.OffRampTime ;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wf_0_Motor].Parameters.OffRampTime = (brmfloor((motors[Wf_0_Motor].Parameters.OffRampTime - WFSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].Parameters.OffRampTime > WFSTOPRAMP_MAX) {
							motors[Wf_0_Motor].Parameters.OffRampTime = WFSTOPRAMP_MAX;
						}else if (motors[Wf_0_Motor].Parameters.OffRampTime < WFSTOPRAMP_MIN) {
							motors[Wf_0_Motor].Parameters.OffRampTime = WFSTOPRAMP_MIN;
						}
						
						motors[Wf_1_Motor].Parameters.OffRampTime = motors[Wf_0_Motor].Parameters.OffRampTime ;
						
					} 
					break;	
					
				case HwStopRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[HotwirePS].Parameters.OffRampTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OffRampTime + HWSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OffRampTime > HWSTOPRAMP_MAX) {
							powerSupplies[HotwirePS].Parameters.OffRampTime = HWSTOPRAMP_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OffRampTime < HWSTOPRAMP_MIN) {
							powerSupplies[HotwirePS].Parameters.OffRampTime = HWSTOPRAMP_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[HotwirePS].Parameters.OffRampTime = (brmfloor((powerSupplies[HotwirePS].Parameters.OffRampTime - HWSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].Parameters.OffRampTime > HWSTOPRAMP_MAX) {
							powerSupplies[HotwirePS].Parameters.OffRampTime = HWSTOPRAMP_MAX;
						}else if (powerSupplies[HotwirePS].Parameters.OffRampTime < HWSTOPRAMP_MIN) {
							powerSupplies[HotwirePS].Parameters.OffRampTime = HWSTOPRAMP_MIN;
						}
						
					} 
					break;
					
				case WpStopRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						motors[Wp_0_Motor].Parameters.OffRampTime = (brmfloor((motors[Wp_0_Motor].Parameters.OffRampTime + WPSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OffRampTime > WPSTOPRAMP_MAX) {
							motors[Wp_0_Motor].Parameters.OffRampTime = WPSTOPRAMP_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OffRampTime < WPSTOPRAMP_MIN) {
							motors[Wp_0_Motor].Parameters.OffRampTime = WPSTOPRAMP_MIN;
						}

						motors[Wp_1_Motor].Parameters.OffRampTime = motors[Wp_0_Motor].Parameters.OffRampTime;
						
					} else if (Position[i] >= ticks) { //5
						
						motors[Wp_0_Motor].Parameters.OffRampTime = (brmfloor((motors[Wp_0_Motor].Parameters.OffRampTime - WPSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wp_0_Motor].Parameters.OffRampTime > WPSTOPRAMP_MAX) {
							motors[Wp_0_Motor].Parameters.OffRampTime = WPSTOPRAMP_MAX;
						}else if (motors[Wp_0_Motor].Parameters.OffRampTime < WPSTOPRAMP_MIN) {
							motors[Wp_0_Motor].Parameters.OffRampTime = WPSTOPRAMP_MIN;
						}

						motors[Wp_1_Motor].Parameters.OffRampTime = motors[Wp_0_Motor].Parameters.OffRampTime;
						
					} 
					break;
					
				case ArcStopRamp_Setting:
					
					if (Position[i] <= -ticks) { //-5
						
						powerSupplies[PrimaryPS].Parameters.OffRampTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OffRampTime + ARCSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OffRampTime > ARCSTOPRAMP_MAX) {
							powerSupplies[PrimaryPS].Parameters.OffRampTime = ARCSTOPRAMP_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OffRampTime < ARCSTOPRAMP_MIN) {
							powerSupplies[PrimaryPS].Parameters.OffRampTime = ARCSTOPRAMP_MIN;
						}					
						
					} else if (Position[i] >= ticks) { //5
						
						powerSupplies[PrimaryPS].Parameters.OffRampTime = (brmfloor((powerSupplies[PrimaryPS].Parameters.OffRampTime - ARCSTOPRAMP_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].Parameters.OffRampTime > ARCSTOPRAMP_MAX) {
							powerSupplies[PrimaryPS].Parameters.OffRampTime = ARCSTOPRAMP_MAX;
						}else if (powerSupplies[PrimaryPS].Parameters.OffRampTime < ARCSTOPRAMP_MIN) {
							powerSupplies[PrimaryPS].Parameters.OffRampTime = ARCSTOPRAMP_MIN;
						}
						
					} 
					break;	


				//cal slopes

				case ArcCalSlope_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						powerSupplies[PrimaryPS].HardwareConfig.CalSlope = (brmfloor((powerSupplies[PrimaryPS].HardwareConfig.CalSlope + ARC_AMPS_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].HardwareConfig.CalSlope > ARC_AMPS_CAL_SLOPE_MAX) {
							powerSupplies[PrimaryPS].HardwareConfig.CalSlope = ARC_AMPS_CAL_SLOPE_MAX;
						}else if (powerSupplies[PrimaryPS].HardwareConfig.CalSlope < ARC_AMPS_CAL_SLOPE_MIN) {
							powerSupplies[PrimaryPS].HardwareConfig.CalSlope = ARC_AMPS_CAL_SLOPE_MIN;
						}
						
					} else if (Position[i] >= ticks) { //10
						
						powerSupplies[PrimaryPS].HardwareConfig.CalSlope = (brmfloor((powerSupplies[PrimaryPS].HardwareConfig.CalSlope - ARC_AMPS_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].HardwareConfig.CalSlope > ARC_AMPS_CAL_SLOPE_MAX) {
							powerSupplies[PrimaryPS].HardwareConfig.CalSlope = ARC_AMPS_CAL_SLOPE_MAX;
						}else if (powerSupplies[PrimaryPS].HardwareConfig.CalSlope < ARC_AMPS_CAL_SLOPE_MIN) {
							powerSupplies[PrimaryPS].HardwareConfig.CalSlope = ARC_AMPS_CAL_SLOPE_MIN;
						}
						
					} 
					break;
	                
				case WfCalSlope_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						motors[Wf_0_Motor].HardwareConfig.CalSlope = (brmfloor((motors[Wf_0_Motor].HardwareConfig.CalSlope + WF_SPEED_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].HardwareConfig.CalSlope > WF_SPEED_CAL_SLOPE_MAX) {
							motors[Wf_0_Motor].HardwareConfig.CalSlope = WF_SPEED_CAL_SLOPE_MAX;
						}else if (motors[Wf_0_Motor].HardwareConfig.CalSlope < WF_SPEED_CAL_SLOPE_MIN) {
							motors[Wf_0_Motor].HardwareConfig.CalSlope = WF_SPEED_CAL_SLOPE_MIN;
						}
						
						motors[Wf_0_Motor].HardwareConfig.CalSlope = motors[Wf_0_Motor].HardwareConfig.CalSlope;
						
					} else if (Position[i] >= ticks) { //10
						
						motors[Wf_0_Motor].HardwareConfig.CalSlope = (brmfloor((motors[Wf_0_Motor].HardwareConfig.CalSlope - WF_SPEED_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].HardwareConfig.CalSlope > WF_SPEED_CAL_SLOPE_MAX) {
							motors[Wf_0_Motor].HardwareConfig.CalSlope = WF_SPEED_CAL_SLOPE_MAX;
						}else if (motors[Wf_0_Motor].HardwareConfig.CalSlope < WF_SPEED_CAL_SLOPE_MIN) {
							motors[Wf_0_Motor].HardwareConfig.CalSlope = WF_SPEED_CAL_SLOPE_MIN;
						}

						motors[Wf_0_Motor].HardwareConfig.CalSlope = motors[Wf_0_Motor].HardwareConfig.CalSlope;
						
					} 
					break;
	                
				case HwCalSlope_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						powerSupplies[HotwirePS].HardwareConfig.CalSlope = (brmfloor((powerSupplies[HotwirePS].HardwareConfig.CalSlope + HW_AMPS_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].HardwareConfig.CalSlope > HW_AMPS_CAL_SLOPE_MAX) {
							powerSupplies[HotwirePS].HardwareConfig.CalSlope = HW_AMPS_CAL_SLOPE_MAX;
						}else if (powerSupplies[HotwirePS].HardwareConfig.CalSlope < HW_AMPS_CAL_SLOPE_MIN) {
							powerSupplies[HotwirePS].HardwareConfig.CalSlope = HW_AMPS_CAL_SLOPE_MIN;
						}
						
					} else if (Position[i] >= ticks) { //10
						
						powerSupplies[HotwirePS].HardwareConfig.CalSlope = (brmfloor((powerSupplies[HotwirePS].HardwareConfig.CalSlope - HW_AMPS_CAL_SLOPE_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].HardwareConfig.CalSlope > HW_AMPS_CAL_SLOPE_MAX) {
							powerSupplies[HotwirePS].HardwareConfig.CalSlope = HW_AMPS_CAL_SLOPE_MAX;
						}else if (powerSupplies[HotwirePS].HardwareConfig.CalSlope < HW_AMPS_CAL_SLOPE_MIN) {
							powerSupplies[HotwirePS].HardwareConfig.CalSlope = HW_AMPS_CAL_SLOPE_MIN;
						}
						
					} 
					break;
	            

				//cal offsets

				case ArcCalOffset_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						powerSupplies[PrimaryPS].HardwareConfig.CalOffset = (brmfloor((powerSupplies[PrimaryPS].HardwareConfig.CalOffset + ARC_AMPS_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].HardwareConfig.CalOffset > ARC_AMPS_CAL_OFFSET_MAX) {
							powerSupplies[PrimaryPS].HardwareConfig.CalOffset = ARC_AMPS_CAL_OFFSET_MAX;
						}else if (powerSupplies[PrimaryPS].HardwareConfig.CalOffset < ARC_AMPS_CAL_OFFSET_MIN) {
							powerSupplies[PrimaryPS].HardwareConfig.CalOffset = ARC_AMPS_CAL_OFFSET_MIN;
						}
						
					} else if (Position[i] >= ticks) { //10
						
						powerSupplies[PrimaryPS].HardwareConfig.CalOffset = (brmfloor((powerSupplies[PrimaryPS].HardwareConfig.CalOffset - ARC_AMPS_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[PrimaryPS].HardwareConfig.CalOffset > ARC_AMPS_CAL_OFFSET_MAX) {
							powerSupplies[PrimaryPS].HardwareConfig.CalOffset = ARC_AMPS_CAL_OFFSET_MAX;
						}else if (powerSupplies[PrimaryPS].HardwareConfig.CalOffset < ARC_AMPS_CAL_OFFSET_MIN) {
							powerSupplies[PrimaryPS].HardwareConfig.CalOffset = ARC_AMPS_CAL_OFFSET_MIN;
						}
						
					} 
					break;
	                
				case WfCalOffset_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						motors[Wf_0_Motor].HardwareConfig.CalOffset = (brmfloor((motors[Wf_0_Motor].HardwareConfig.CalOffset + WF_SPEED_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].HardwareConfig.CalOffset > WF_SPEED_CAL_OFFSET_MAX) {
							motors[Wf_0_Motor].HardwareConfig.CalOffset = WF_SPEED_CAL_OFFSET_MAX;
						}else if (motors[Wf_0_Motor].HardwareConfig.CalOffset < WF_SPEED_CAL_OFFSET_MIN) {
							motors[Wf_0_Motor].HardwareConfig.CalOffset = WF_SPEED_CAL_OFFSET_MIN;
						}

						motors[Wf_1_Motor].HardwareConfig.CalOffset = motors[Wf_0_Motor].HardwareConfig.CalOffset;
						
					} else if (Position[i] >= ticks) { //10
						
						motors[Wf_0_Motor].HardwareConfig.CalOffset = (brmfloor((motors[Wf_0_Motor].HardwareConfig.CalOffset - WF_SPEED_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (motors[Wf_0_Motor].HardwareConfig.CalOffset > WF_SPEED_CAL_OFFSET_MAX) {
							motors[Wf_0_Motor].HardwareConfig.CalOffset = WF_SPEED_CAL_OFFSET_MAX;
						}else if (motors[Wf_0_Motor].HardwareConfig.CalOffset < WF_SPEED_CAL_OFFSET_MIN) {
							motors[Wf_0_Motor].HardwareConfig.CalOffset = WF_SPEED_CAL_OFFSET_MIN;
						}

						motors[Wf_1_Motor].HardwareConfig.CalOffset = motors[Wf_0_Motor].HardwareConfig.CalOffset;
						
					} 
					break;
	                
				case HwCalOffset_Setting:
					
					if (Position[i] <= -ticks) { //-10
						
						powerSupplies[HotwirePS].HardwareConfig.CalOffset = (brmfloor((powerSupplies[HotwirePS].HardwareConfig.CalOffset + HW_AMPS_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].HardwareConfig.CalOffset > HW_AMPS_CAL_OFFSET_MAX) {
							powerSupplies[HotwirePS].HardwareConfig.CalOffset = HW_AMPS_CAL_OFFSET_MAX;
						}else if (powerSupplies[HotwirePS].HardwareConfig.CalOffset < HW_AMPS_CAL_OFFSET_MIN) {
							powerSupplies[HotwirePS].HardwareConfig.CalOffset = HW_AMPS_CAL_OFFSET_MIN;
						}
						
					} else if (Position[i] >= ticks) { //10
						
						powerSupplies[HotwirePS].HardwareConfig.CalOffset = (brmfloor((powerSupplies[HotwirePS].HardwareConfig.CalOffset - HW_AMPS_CAL_OFFSET_INCR) * ROUND_OF_TO_2_DECIMAL + 0.5) / ROUND_OF_TO_2_DECIMAL);
						
						Position[i] = 0;
						
						//check limits
						if (powerSupplies[HotwirePS].HardwareConfig.CalOffset > HW_AMPS_CAL_OFFSET_MAX) {
							powerSupplies[HotwirePS].HardwareConfig.CalOffset = HW_AMPS_CAL_OFFSET_MAX;
						}else if (powerSupplies[HotwirePS].HardwareConfig.CalOffset < HW_AMPS_CAL_OFFSET_MIN) {
							powerSupplies[HotwirePS].HardwareConfig.CalOffset = HW_AMPS_CAL_OFFSET_MIN;
						}
						
					} 
					break;


				//control	    
	                
				case PrimaryPowerSupplyType_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.PrimaryPSType >= LINCS500_Type) {
							machine.PrimaryPSType = 0;
						} else {
							machine.PrimaryPSType = machine.PrimaryPSType + 1;
						}

						AssignHardwareParam();

					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.PrimaryPSType <= Miller_Type) {
							machine.PrimaryPSType = LINCS500_Type;
						} else {
							machine.PrimaryPSType = machine.PrimaryPSType - 1;
						}

						AssignHardwareParam();
					
					}

					// Check Amp max according power supply type
					switch (machine.PrimaryPSType) {

    					case Miller_Type: 		arcMax = MILLER_PS_AMPS_AT_MAX_COMMAND_V; 		break;
						case GTII_Type: 		arcMax = GTII_PS_AMPS_AT_MAX_COMMAND_V; 		break;
						case GTVI_Type:			arcMax = GTVI_PS_AMPS_AT_MAX_COMMAND_V; 		break;
						case LINCS350_Type: 	arcMax = LINCS350_PS_AMPS_AT_MAX_COMMAND_V; 	break;
						case LINCS500_Type: 	arcMax = LINCS500_PS_AMPS_AT_MAX_COMMAND_V; 	break;

					}

					if(powerSupplies[PrimaryPS].Parameters.AmpsSetpoint > arcMax) 
							powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = arcMax;

					/////////////////////////////////////////////
					
					
					break;

				case HotwirePowerSupplyType_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.HotwirePSType == Acopian_Type) {
							machine.HotwirePSType = 0;
						} else {
							machine.HotwirePSType = machine.HotwirePSType + 1;
						}

					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.HotwirePSType == Jackle_Type) {
							machine.HotwirePSType = Acopian_Type;
						} else {
							machine.HotwirePSType = machine.HotwirePSType - 1;
						}
					
					}
					break;
					
				case ControlMode_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.ControlMode >= FourAxis) {  //end of enum list
							machine.ControlMode = Manual;  //beigining of enum list
						} else {
							machine.ControlMode = machine.ControlMode + 1;
						}
						
					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.ControlMode <= Manual) { //beigining of enum list
							machine.ControlMode = FourAxis;  //end of enum list
						} else {
							machine.ControlMode = machine.ControlMode - 1;
						}
					
					}
					break;
					

				case RunMode_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.RunMode == WF_And_WP_And_HW_Mode) {  //end of enum list
							machine.RunMode = WF_Mode;  //beigining of enum list
						} else {
							machine.RunMode = machine.RunMode + 1;
						}
						
					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.RunMode == WF_Mode) {  //beigining of enum list
							machine.RunMode = WF_And_WP_And_HW_Mode;  //end of enum list
						} else {
							machine.RunMode = machine.RunMode - 1;
						}
					
					}
					break;
					
				case LoadDefaults_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.LoadDefaultsMode == Yes_Load) {  //end of enum list
							machine.LoadDefaultsMode = No_Load;  //beigining of enum list
						} else {
							machine.LoadDefaultsMode = machine.LoadDefaultsMode + 1;
						}
						
					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (machine.LoadDefaultsMode == No_Load) { //beigining of enum list
							machine.LoadDefaultsMode = Yes_Load;  //end of enum list
						} else {
							machine.LoadDefaultsMode = machine.LoadDefaultsMode - 1;
						}
					
					}
					break;


				case MenuMode_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (menu[i].MenuMode == Configuration) {
							menu[i].MenuMode = 0;
						} else {
							menu[i].MenuMode = menu[i].MenuMode + 1;
						}

					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						
						if (menu[i].MenuMode == User) {
							menu[i].MenuMode = Configuration;
						} else {
							menu[i].MenuMode = menu[i].MenuMode - 1;
						}
					
					}
					break;

				case NEXT_Project_Setting:
					
					if (Position[i] <= -ticks) { //-20
						
						//set position and index to zero 
						Position[i] = 0;

						NextProjectConfig++;
						
						

					} else if (Position[i] >= ticks) { //20
						
						//set position and index to zero 
						Position[i] = 0;
						NextProjectConfig--;
										
					}

					if(NextProjectConfig >= project_lastProject)
						NextProjectConfig = project_lastProject - 1;

					
					break;		

					
				case ErrorCode_Ack:
					//just going into edit mode clears this
					break;


				default:
					break;
			}
					
		} else {
			//not in edit mode, cycle through menu[i]
	        
	        
			if (Position[i] <= -ticks) { //-20
				//going forward

				//set position and index to zero 
				Position[i] = 0;

				//check limits
				if (menu[i].DisplayCode >= Last_Display_Setting_Code-1) { //last item in enum list
					menu[i].DisplayCode = 0;  //wrap around to top of list
				} else {
					menu[i].DisplayCode = menu[i].DisplayCode + 1;
				}

				
				if (menu[i].MenuMode == User) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
						menu[i].DisplayCode == HwAmps_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
						menu[i].DisplayCode == HwStopRamp_Setting | 
						menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
						menu[i].DisplayCode == HwCalOffset_Setting | 
						menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting | 
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
						menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = MenuMode_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |
						menu[i].DisplayCode == ErrorCode_Ack ){
					

						//do nothing

					}
					 else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = WpFreq_Setting;
						
					}

				} else if (menu[i].MenuMode == Boost) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
						menu[i].DisplayCode == HwAmps_Setting |
						menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
						menu[i].DisplayCode == HwStopRamp_Setting | 
						menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
						menu[i].DisplayCode == HwCalOffset_Setting | 
						menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting | 
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
						menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = MenuMode_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |
						menu[i].DisplayCode == ErrorCode_Ack) {

						//do nothing

					
					} else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = WpFreq_Setting;
						
					}


				} else if (menu[i].MenuMode == Calibration) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
					menu[i].DisplayCode == HwAmps_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting |
						menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
					menu[i].DisplayCode == HwStopRamp_Setting) {

						menu[i].DisplayCode = ArcCalSlope_Setting;

					} else if (menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
					menu[i].DisplayCode == HwCalOffset_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting |
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
					menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = MenuMode_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |
						menu[i].DisplayCode == ErrorCode_Ack) {

						//do nothing

					}
					else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = WpFreq_Setting;
						
					}
					
				} else {

					//Configuration MenuMode ... do nothing

				}
	            
			} else if (Position[i] >= ticks) { //20
				//going backwards

				//set position and index to zero 
				Position[i] = 0;

				//check limits
				if (menu[i].DisplayCode == WpFreq_Setting) { //first item in enum list
					menu[i].DisplayCode = Last_Display_Setting_Code - 1;  //wrap around to bottom of list
				} else {
					menu[i].DisplayCode = menu[i].DisplayCode - 1;
				}


				if (menu[i].MenuMode == User) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
					menu[i].DisplayCode == HwAmps_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
						menu[i].DisplayCode == HwStopRamp_Setting | 
						menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
						menu[i].DisplayCode == HwCalOffset_Setting | 
						menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting | 
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
						menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = HwAmps_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |
						menu[i].DisplayCode == ErrorCode_Ack ) {

						//do nothing

					}else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = ErrorCode_Ack;
						
					}

				} else if (menu[i].MenuMode == Boost) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
						menu[i].DisplayCode == HwAmps_Setting |
						menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
					menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
						menu[i].DisplayCode == HwStopRamp_Setting | 
						menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
						menu[i].DisplayCode == HwCalOffset_Setting | 
						menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting | 
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
						menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = HwAmpsBoostLevel_2_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |
						menu[i].DisplayCode == ErrorCode_Ack) {

						//do nothing

					}else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = ErrorCode_Ack;
						
					}

				} else if (menu[i].MenuMode == Calibration) {

					if (menu[i].DisplayCode == WpFreq_Setting | 
						menu[i].DisplayCode == WfSpeed_Setting | 
						menu[i].DisplayCode == ArcAmps_Setting | 
					menu[i].DisplayCode == HwAmps_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == BoostLevel_1_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_1_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_1_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_1_Setting | 
						menu[i].DisplayCode == BoostLevel_2_Variable_Setting | 
						menu[i].DisplayCode == ArcAmpsBoostLevel_2_Setting | 
						menu[i].DisplayCode == WfSpeedBoostLevel_2_Setting | 
						menu[i].DisplayCode == WpFreqBoostLevel_2_Setting | 
						menu[i].DisplayCode == HwAmpsBoostLevel_2_Setting |
						menu[i].DisplayCode == ArcStartDelay_Setting | 
						menu[i].DisplayCode == WfStartDelay_Setting | 
						menu[i].DisplayCode == WpStartDelay_Setting | 
						menu[i].DisplayCode == HwStartDelay_Setting | 
						menu[i].DisplayCode == ArcStopDelay_Setting | 
						menu[i].DisplayCode == WfStopDelay_Setting | 
						menu[i].DisplayCode == WpStopDelay_Setting | 
						menu[i].DisplayCode == HwStopDelay_Setting | 
						menu[i].DisplayCode == ArcStartRamp_Setting | 
						menu[i].DisplayCode == WfStartRamp_Setting | 
						menu[i].DisplayCode == WpStartRamp_Setting | 
						menu[i].DisplayCode == HwStartRamp_Setting | 
						menu[i].DisplayCode == ArcStopRamp_Setting| 
						menu[i].DisplayCode == WfStopRamp_Setting | 
						menu[i].DisplayCode == WpStopRamp_Setting | 
					menu[i].DisplayCode == HwStopRamp_Setting) {

						menu[i].DisplayCode = HwAmps_Setting;

					} else if (menu[i].DisplayCode == ArcCalSlope_Setting | 
						menu[i].DisplayCode == WfCalSlope_Setting | 
						menu[i].DisplayCode == HwCalSlope_Setting | 
						menu[i].DisplayCode == ArcCalOffset_Setting | 
						menu[i].DisplayCode == WfCalOffset_Setting | 
					menu[i].DisplayCode == HwCalOffset_Setting) {

						//do nothing

					} else if (menu[i].DisplayCode == PrimaryPowerSupplyType_Setting | 
						menu[i].DisplayCode == HotwirePowerSupplyType_Setting | 
						menu[i].DisplayCode == ControlMode_Setting | 
						menu[i].DisplayCode == RunMode_Setting | 
					menu[i].DisplayCode == LoadDefaults_Setting) {

						menu[i].DisplayCode = HwCalOffset_Setting;

					} else if (menu[i].DisplayCode == MenuMode_Setting |						
						menu[i].DisplayCode == ErrorCode_Ack) {

						//do nothing

					}
					else if (menu[i].DisplayCode == PLC_Version |
						menu[i].DisplayCode == NEXT_Project_Setting) {

						menu[i].DisplayCode = ErrorCode_Ack;
						
					}
					
				} else {

					//Configuration MenuMode ... do nothing

				}

	            
			} 
			
	    
		}
		

		
		
		
		twirlCount = twirlCount + 1;
	                    
	                    
		//LCD - line 0
	                    
		if (twirlCount >= TWIRL_SPEED) {
			//rotate
			switch (twirl) {
				case '<':
					twirl = '|';
					twirlCount = 0;
					//twirlString = "\0000\n";
					break;
				case '|':
					twirl = '>';
					twirlCount = 0;
					//twirlString = "\0001\n";
					break;
				case '>':
					twirl = '+';
					twirlCount = 0;
					//twirlString = "\0002\n";
					break;
				case '+':
					twirl = '<';
					twirlCount = 0;
					//twirlString = "\0003\n";
					break;
				default:
					break;
			}
		}
	                    
	                    
	                   
		switch (master_sm.Step) {
	                            
			case PostCheck:
				xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, POST_CHECK_DISP_STRING, twirl);
				break;
			//case Ready:
			//xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, READY_DISP_STRING, twirl);
			//break;
			case Jogging:
				xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, JOGGING_DISP_STRING, twirl);
				break;
			case JoggingRev:
				xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, JOGGING_DISP_STRING, twirl);
				break;
			case Error:
				xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, ERROR_DISP_STRING, twirl);
				break;
			default: //all running modes (arc, wf, wp, hw)

				if(machine.EstopFault) {

					if(!menu[i].Blink && machine.EditMode)
						xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, EDIT_MODE_STRING, twirl);
					else
						xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, ESTOP_ACTIVE_STRING, twirl);

				}	

				else if(machine.EditMode && !menu[i].Blink){

					xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, EDIT_MODE_STRING, twirl);

				}

				else if (master_sm.Step == Ready && hardware.Outputs[PrimaryPS] == false && hardware.Outputs[HotwirePS] == false && motors[Wf_0_Motor].Misc.PID.enable == false && motors[Wp_0_Motor].Misc.PID.enable == false && motors[Wf_1_Motor].Misc.PID.enable == false && motors[Wp_1_Motor].Misc.PID.enable == false){

					
					xsprintf(statusString[i], "%s%s%c", STATUS_DISP_STRING, READY_DISP_STRING, twirl);

				} else {	

					//arc or rem
					if (machine.ControlMode == Manual) {

						if (powerSupplies[PrimaryPS].Command.command_sm.Step == Running_Power_Supply_State || hardware.Outputs[PrimaryPS] == true || menu[i].ArcDisplayBlinkEnable == true) {
	                                
							if ((menu[i].ArcDisplayBlinkEnable == false) && (powerSupplies[PrimaryPS].Command.command_sm.Step == Running_Power_Supply_State)) {
								xsprintf(arcStatusString, "%s", ARC_DISP_STRING);
							} else {
								if (menu[i].Blink == true) {
									xsprintf(arcStatusString, "%s", NO_ARC_DISP_STRING);
								} else {
									xsprintf(arcStatusString, "%s", ARC_DISP_STRING);
								}
							}
	                                
						} else {
							xsprintf(arcStatusString, "%s", NO_ARC_DISP_STRING);
						}

					} else if (machine.ControlMode == Automatic) {

						if (hardware.Inputs[RemoteSeqStart] || menu[i].RemDisplayBlinkEnable == true) {
	                                
							if (menu[i].RemDisplayBlinkEnable == false) {
								xsprintf(remStatusString, "%s", REM_DISP_STRING);
							} else {
								if (menu[i].Blink == true) {
									xsprintf(remStatusString, "%s", NO_REM_DISP_STRING);
								} else {
									xsprintf(remStatusString, "%s", REM_DISP_STRING);
								}
							}
	                                
						} else {
							xsprintf(remStatusString, "%s", NO_REM_DISP_STRING);
						}

					}
					 else if (machine.ControlMode == FourAxis) {

						if (hardware.Inputs[RemoteSeqStart] || menu[i].RemDisplayBlinkEnable == true) {
	                                
							if (menu[i].RemDisplayBlinkEnable == false) {
								xsprintf(remStatusString, "%s", REM_DISP_STRING);
							} else {
								if (menu[i].Blink == true) {
									xsprintf(remStatusString, "%s", NO_REM_DISP_STRING);
								} else {
									xsprintf(remStatusString, "%s", REM_DISP_STRING);
								}
							}
	                                
						} else {
							xsprintf(remStatusString, "%s", NO_REM_DISP_STRING);
						}

					}
	            

	                            
					//wf
					if (motors[Wf_0_Motor].Command.command_sm.Step == Running_Motor_State || motors[Wf_1_Motor].Command.command_sm.Step == Running_Motor_State || menu[i].WfDisplayBlinkEnable == true) {
	                                
						if (menu[i].WfDisplayBlinkEnable == false) {
							xsprintf(wfStatusString, "%s", WF_DISP_STRING);
						} else {
							if (menu[i].Blink == true) {
								xsprintf(wfStatusString, "%s", NO_WF_DISP_STRING);
							} else {
								xsprintf(wfStatusString, "%s", WF_DISP_STRING);
							}
						}
	                                
					} else {
						xsprintf(wfStatusString, "%s", NO_WF_DISP_STRING);
					}
	                            
					//wp
					if ( (motors[Wp_0_Motor].Command.command_sm.Step == Running_Motor_State || motors[Wp_0_Motor].Command.command_sm.Step == Running_Motor_State || motors[Wp_1_Motor].Command.command_sm.Step == Running_Motor_State) || menu[i].WpDisplayBlinkEnable == true) {
	                                
						if (menu[i].WpDisplayBlinkEnable == false) {
							xsprintf(wpStatusString, "%s", WP_DISP_STRING);
						} else {
							if (menu[i].Blink == true) {
								xsprintf(wpStatusString, "%s", NO_WP_DISP_STRING);
							} else {
								xsprintf(wpStatusString, "%s", WP_DISP_STRING);
							}
						}
	                                
					} else {
						xsprintf(wpStatusString, "%s", NO_WP_DISP_STRING);
					}
	                            
					//hw
					if (powerSupplies[HotwirePS].Command.command_sm.Step == Running_Power_Supply_State || menu[i].HwDisplayBlinkEnable == true) {
	                                
						if (menu[i].HwDisplayBlinkEnable == false) {
							xsprintf(hwStatusString, "%s", HW_DISP_STRING);
						} else {
							if (menu[i].Blink == true) {
								xsprintf(hwStatusString, "%s", NO_HW_DISP_STRING);
							} else {
								xsprintf(hwStatusString, "%s", HW_DISP_STRING);
							}
						}
	                                
					} else {
						xsprintf(hwStatusString, "%s", NO_HW_DISP_STRING);
					}
	            

					if (machine.ControlMode == Manual) {

						xsprintf(statusString[i], "%s%s/%s/%s/%s%c", STATUS_DISP_STRING, arcStatusString, wfStatusString, wpStatusString, hwStatusString, twirl);

					} else if (machine.ControlMode == Automatic) {

						xsprintf(statusString[i], "%s%s/%s/%s/%s%c", STATUS_DISP_STRING, remStatusString, wfStatusString, wpStatusString, hwStatusString, twirl);

					}
					else if (machine.ControlMode == FourAxis) {

						xsprintf(statusString[i], "%s%s/%s/%s/%s%c", STATUS_DISP_STRING, remStatusString, wfStatusString, wpStatusString, hwStatusString, twirl);

					}

				}
	                       
				break;
		}
	                    
	                  
	                    


		//LCD - line 1
	                    
		//no blink
		switch (menu[i].DisplayCode) {
			case WpFreq_Setting:
				xsprintf(varSting[i], "%s %8f", WPFREQ_DISP_STRING, motors[Wp_0_Motor].Parameters.SpeedSetpoint);
				break;
			case WfSpeed_Setting:
				xsprintf(varSting[i], "%s %8f", WFSPEED_DISP_STRING, motors[Wf_0_Motor].Parameters.SpeedSetpoint);
				break;
			case ArcAmps_Setting:
				xsprintf(varSting[i], "%s %8f", ARCAMPS_DISP_STRING, powerSupplies[PrimaryPS].Parameters.AmpsSetpoint);
				break;
			case HwAmps_Setting:
				xsprintf(varSting[i], "%s %8f", HWAMPS_DISP_STRING, powerSupplies[HotwirePS].Parameters.AmpsSetpoint);
				break;
			case BoostLevel_1_Variable_Setting:
				switch (machine.Boost1Type) {
					case ARC_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_BOOST_TYPE_DISP_STRINGG);
						break;
					case WF_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WF_BOOST_TYPE_DISP_STRING);
						break;
					case WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WP_BOOST_TYPE_DISP_STRING);
						break;
					case HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WF_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case WF_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WP_HW_Boost_Mode:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST1VAR_DISP_STRING, ARC_WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case BoostLevel_2_Variable_Setting:
				switch (machine.Boost2Type) {
					case ARC_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_BOOST_TYPE_DISP_STRINGG);
						break;
					case WF_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WF_BOOST_TYPE_DISP_STRING);
						break;
					case WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WP_BOOST_TYPE_DISP_STRING);
						break;
					case HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WF_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case WF_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WP_HW_Boost_Mode:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_HW_Boost_Type:
						xsprintf(varSting[i], "%s %s", BOOST2VAR_DISP_STRING, ARC_WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case ArcAmpsBoostLevel_1_Setting:
				xsprintf(varSting[i], "%s %8f", ARC_AMPS_BOOST1SET_DISP_STRING, powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint);
				break; 
			case ArcAmpsBoostLevel_2_Setting:
				xsprintf(varSting[i], "%s %8f", ARC_AMPS_BOOST2SET_DISP_STRING, powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint);
				break;  
			case WfSpeedBoostLevel_1_Setting:
				xsprintf(varSting[i], "%s %8f", WF_SPEED_BOOST1SET_DISP_STRING, motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint);
				break;
			case WfSpeedBoostLevel_2_Setting:
				xsprintf(varSting[i], "%s %8f", WF_SPEED_BOOST2SET_DISP_STRING, motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint);
				break;
			case WpFreqBoostLevel_1_Setting:
				xsprintf(varSting[i], "%s %8f", WP_FREQ_BOOST1SET_DISP_STRING, motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint);
				break;
			case WpFreqBoostLevel_2_Setting:
				xsprintf(varSting[i], "%s %8f", WP_FREQ_BOOST2SET_DISP_STRING, motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint);
				break;
			case HwAmpsBoostLevel_1_Setting:
				xsprintf(varSting[i], "%s %8f", HW_AMPS_BOOST1SET_DISP_STRING, powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint);
				break;
			case HwAmpsBoostLevel_2_Setting:
				xsprintf(varSting[i], "%s %8f", HW_AMPS_BOOST2SET_DISP_STRING, powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint);
				break;
	                            
	                            
			//---- delays
	                            
			case WfStartDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", WFSTARTDELAY_DISP_STRING, motors[Wf_0_Motor].Parameters.OnDelayTime);
				break;
			case WpStartDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", WPSTARTDELAY_DISP_STRING, motors[Wp_0_Motor].Parameters.OnDelayTime);
				break;
			case HwStartDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", HWSTARTDELAY_DISP_STRING, powerSupplies[HotwirePS].Parameters.OnDelayTime);
				break;
			case ArcStartDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", ARCSTARTDELAY_DISP_STRING, powerSupplies[PrimaryPS].Parameters.OnDelayTime);
				break;
	                            
			case WfStopDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", WFSTOPDELAY_DISP_STRING, motors[Wf_0_Motor].Parameters.OffDelayTime);
				break;	
			case WpStopDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", WPSTOPDELAY_DISP_STRING, motors[Wp_0_Motor].Parameters.OffDelayTime);
				break;
			case HwStopDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", HWSTOPDELAY_DISP_STRING, powerSupplies[HotwirePS].Parameters.OffDelayTime);
				break;
			case ArcStopDelay_Setting:
				xsprintf(varSting[i], "%s %8.1f", ARCSTOPDELAY_DISP_STRING, powerSupplies[PrimaryPS].Parameters.OffDelayTime);
				break;
	                            
	                            
			//---- ramps
	                            
			case WfStartRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", WFSTARTRAMP_DISP_STRING, motors[Wf_0_Motor].Parameters.OnRampTime);
				break;
			case WpStartRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", WPSTARTRAMP_DISP_STRING, motors[Wp_0_Motor].Parameters.OnRampTime);
				break;
			case HwStartRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", HWSTARTRAMP_DISP_STRING, powerSupplies[HotwirePS].Parameters.OnRampTime);
				break;
			case ArcStartRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", ARCSTARTRAMP_DISP_STRING, powerSupplies[PrimaryPS].Parameters.OnRampTime);
				break;
	                            
			case WfStopRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", WFSTOPRAMP_DISP_STRING, motors[Wf_0_Motor].Parameters.OffRampTime);
				break;	
			case WpStopRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", WPSTOPRAMP_DISP_STRING, motors[Wp_0_Motor].Parameters.OffRampTime);
				break;
			case HwStopRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", HWSTOPRAMP_DISP_STRING, powerSupplies[HotwirePS].Parameters.OffRampTime);
				break;
			case ArcStopRamp_Setting:
				xsprintf(varSting[i], "%s %8.1f", ARCSTOPRAMP_DISP_STRING, powerSupplies[PrimaryPS].Parameters.OffRampTime);
				break;
	                            
	                            
			//---- cal factors  
	    
			case ArcCalSlope_Setting:
				xsprintf(varSting[i], "%s %8.2f", ARCCAL_SLOPE_DISP_STRING, powerSupplies[PrimaryPS].HardwareConfig.CalSlope);
				break;
			case WfCalSlope_Setting:
				xsprintf(varSting[i], "%s %8.2f", WFCAL_SLOPE_DISP_STRING, motors[Wf_0_Motor].HardwareConfig.CalSlope);
				break;
			case HwCalSlope_Setting:
				xsprintf(varSting[i], "%s %8.2f", HWCAL_SLOPE_DISP_STRING, powerSupplies[HotwirePS].HardwareConfig.CalSlope);
				break;
	                      
			case ArcCalOffset_Setting:
				xsprintf(varSting[i], "%s %8.2f", ARCCAL_OFFSET_DISP_STRING, powerSupplies[PrimaryPS].HardwareConfig.CalOffset);
				break;
			case WfCalOffset_Setting:
				xsprintf(varSting[i], "%s %8.2f", WFCAL_OFFSET_DISP_STRING, motors[Wf_0_Motor].HardwareConfig.CalOffset);
				break;
			case HwCalOffset_Setting:
				xsprintf(varSting[i], "%s %8.2f", HWCAL_OFFSET_DISP_STRING, powerSupplies[HotwirePS].HardwareConfig.CalOffset);
				break;
	                      
	                            
			//---- controls 
	                            
			case PrimaryPowerSupplyType_Setting:
				switch (machine.PrimaryPSType) {
					case Miller_Type:
						xsprintf(varSting[i], "%s %s", PRIMARY_PS_TYPE_DISP_STRING, MILLER_PS_TYPE_DISP_STRING);
						break;
						
					case GTII_Type:
						xsprintf(varSting[i], "%s %s", PRIMARY_PS_TYPE_DISP_STRING, GTII_PS_TYPE_DISP_STRING);
						break;
						
					case GTVI_Type:
						xsprintf(varSting[i], "%s %s", PRIMARY_PS_TYPE_DISP_STRING, GTVI_PS_TYPE_DISP_STRING);
						break;

					case LINCS350_Type:
						xsprintf(varSting[i], "%s %s", PRIMARY_PS_TYPE_DISP_STRING, LINCS350_PS_TYPE_DISP_STRING);
						break;		
						
					case LINCS500_Type:
						xsprintf(varSting[i], "%s %s", PRIMARY_PS_TYPE_DISP_STRING, LINCS500_PS_TYPE_DISP_STRING);
						break;	
					
					default:
						break;
				}
				break;
			case HotwirePowerSupplyType_Setting:
				switch (machine.HotwirePSType) {
					case Jackle_Type:
						xsprintf(varSting[i], "%s %s", HOTWIRE_PS_TYPE_DISP_STRING, JACKLE_PS_TYPE_DISP_STRING);
						break;
					case Acopian_Type:
						xsprintf(varSting[i], "%s %s", HOTWIRE_PS_TYPE_DISP_STRING, ACOPIAN_PS_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case ControlMode_Setting:
				switch (machine.ControlMode) {
					case Manual:
						xsprintf(varSting[i], "%s %s", CONTROL_MODE_DISPLAY_STRING, MANUAL_CONTROL_MODE_DISP_STRING);
						break;
					case Automatic:
						xsprintf(varSting[i], "%s %s", CONTROL_MODE_DISPLAY_STRING, AUTOMATIC_CONTROL_MODE_DISP_STRING);
						break;
					case FourAxis:
						xsprintf(varSting[i], "%s %s", CONTROL_MODE_DISPLAY_STRING, FourAxis_CONTROL_MODE_DISP_STRING);
						break;
						
					default:
						break;
				}
				break;
			case RunMode_Setting:
				switch (machine.RunMode) {
					case WF_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WF_RUN_MODE_DISP_STRING);
						break;
					case WP_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WP_RUN_MODE_DISP_STRING);
						break;
					case HW_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, HW_RUN_MODE_DISP_STRING);
						break;
					case WF_And_WP_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WF_AND_WP_RUN_MODE_DISP_STRING);
						break;
					case WF_And_HW_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WF_AND_HW_RUN_MODE_DISP_STRING);
						break;
					case WP_And_HW_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WP_AND_HW_RUN_MODE_DISP_STRING);
						break;
					case WF_And_WP_And_HW_Mode:
						xsprintf(varSting[i], "%s %s", RUN_MODE_DISPLAY_STRING, WF_AND_WP_AND_HW_RUN_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case LoadDefaults_Setting:
				switch (machine.LoadDefaultsMode) {
					case No_Load:
						xsprintf(varSting[i], "%s %s", LOAD_DEFAULTS_MODE_DISPLAY_STRING, NO_LOAD_DEFAULTS_MODE_DISP_STRING);
						break;
					case Yes_Load:
						xsprintf(varSting[i], "%s %s", LOAD_DEFAULTS_MODE_DISPLAY_STRING, YES_LOAD_DEFAULTS_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case MenuMode_Setting:
				switch (menu[i].MenuMode) {
					case User:
						xsprintf(varSting[i], "%s %s", MENU_MODE_DISPLAY_STRING, USER_MENU_MODE_DISP_STRING);
						break;
					case Boost:
						xsprintf(varSting[i], "%s %s", MENU_MODE_DISPLAY_STRING, BOOST_MENU_MODE_DISP_STRING);
						break;
					case Calibration:
						xsprintf(varSting[i], "%s %s", MENU_MODE_DISPLAY_STRING, CAL_MENU_MODE_DISP_STRING);
						break;
					case Configuration:
						xsprintf(varSting[i], "%s %s", MENU_MODE_DISPLAY_STRING, CONFIG_MENU_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case ErrorCode_Ack:
				switch (machine.ErrorCode) {
					case No_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, NO_ERROR_STRING);
						break;
					case Test_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, TEST_ERROR_STRING);
						break;
					case WF_0_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WF_0_MOTOR_HIGH_AMPS_STRING);
						break;
					case WP_0_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WP_0_MOTOR_HIGH_AMPS_STRING);
						break;
					case WF_1_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WF_1_MOTOR_HIGH_AMPS_STRING);
						break;
					case WP_1_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WP_1_MOTOR_HIGH_AMPS_STRING);
						break;
					case Hardware_Not_Attached:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, HARDWARE_NOT_ATTACHED_STRING);
						break;
					default:
						break;
				}
				break;					
			default:
				break;
		}
	                    
	                    
	                    
	                    
		//blink
		switch (menu[i].DisplayCode) {
			case WpFreq_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.SpeedSetpoint);
				break;	
			case WfSpeed_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.SpeedSetpoint);
				break;
			case ArcAmps_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.AmpsSetpoint);
				break;
			case HwAmps_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.AmpsSetpoint);
				break;
			case BoostLevel_1_Variable_Setting:
				switch (machine.Boost1Type) {
					case ARC_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_BOOST_TYPE_DISP_STRINGG);
						break;
					case WF_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_BOOST_TYPE_DISP_STRING);
						break;
					case WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_BOOST_TYPE_DISP_STRING);
						break;
					case HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case WF_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WP_HW_Boost_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case BoostLevel_2_Variable_Setting:
				switch (machine.Boost2Type) {
					case ARC_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_BOOST_TYPE_DISP_STRINGG);
						break;
					case WF_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_BOOST_TYPE_DISP_STRING);
						break;
					case WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_BOOST_TYPE_DISP_STRING);
						break;
					case HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case WF_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WP_HW_Boost_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_WP_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case WF_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					case ARC_WF_WP_HW_Boost_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ARC_WF_WP_HW_BOOST_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case ArcAmpsBoostLevel_1_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint);
				break; 
			case ArcAmpsBoostLevel_2_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint);
				break;  
			case WfSpeedBoostLevel_1_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint);
				break;
			case WfSpeedBoostLevel_2_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint);
				break;
			case WpFreqBoostLevel_1_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint);
				break;
			case WpFreqBoostLevel_2_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint);
				break;
			case HwAmpsBoostLevel_1_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint);
				break;
			case HwAmpsBoostLevel_2_Setting:
				xsprintf(varStingBlink[i], "%s %8f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint);
				break;
	                            
	                            
			//---- delays
	                            
			case WfStartDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.OnDelayTime);
				break;
			case WpStartDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.OnDelayTime);
				break;
			case HwStartDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.OnDelayTime);
				break;
			case ArcStartDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.OnDelayTime);
				break;
	                            
			case WfStopDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.OffDelayTime);
				break;	
			case WpStopDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.OffDelayTime);
				break;
			case HwStopDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.OffDelayTime);
				break;
			case ArcStopDelay_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.OffDelayTime);
				break;
	                            
	                            
			//---- ramps
	                            
			case WfStartRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.OnRampTime);
				break;
			case WpStartRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.OnRampTime);
				break;
			case HwStartRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.OnRampTime);
				break;
			case ArcStartRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.OnRampTime);
				break;
	                            
			case WfStopRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wf_0_Motor].Parameters.OffRampTime);
				break;	
			case WpStopRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, motors[Wp_0_Motor].Parameters.OffRampTime);
				break;
			case HwStopRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[HotwirePS].Parameters.OffRampTime);
				break;
			case ArcStopRamp_Setting:
				xsprintf(varStingBlink[i], "%s %8.1f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].Parameters.OffRampTime);
				break;
	                            
	                            
			//---- cal factors  
	                            
			case ArcCalSlope_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].HardwareConfig.CalSlope);
				break;
			case WfCalSlope_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, motors[Wf_0_Motor].HardwareConfig.CalSlope);
				break;
			case HwCalSlope_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, powerSupplies[HotwirePS].HardwareConfig.CalSlope);
				break;

			case ArcCalOffset_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, powerSupplies[PrimaryPS].HardwareConfig.CalOffset);
				break;
			case WfCalOffset_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, motors[Wf_0_Motor].HardwareConfig.CalOffset);
				break;
			case HwCalOffset_Setting:
				xsprintf(varStingBlink[i], "%s %8.2f", VAR_BLINK_STRING, powerSupplies[HotwirePS].HardwareConfig.CalOffset);
				break;

	                            
	                            
			//---- controls      
	                            
			case PrimaryPowerSupplyType_Setting:
				switch (machine.PrimaryPSType) {
					case Miller_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, MILLER_PS_TYPE_DISP_STRING);
						break;
						
					case GTII_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, GTII_PS_TYPE_DISP_STRING);
						break;
						
					case GTVI_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, GTVI_PS_TYPE_DISP_STRING);
						break;


					case LINCS350_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, LINCS350_PS_TYPE_DISP_STRING);
						break;	

					case LINCS500_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, LINCS500_PS_TYPE_DISP_STRING);
						break;

						
					default:
						break;
				}
				break;	
			case HotwirePowerSupplyType_Setting:
				switch (machine.HotwirePSType) {
					case Jackle_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, JACKLE_PS_TYPE_DISP_STRING);
						break;
					case Acopian_Type:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, ACOPIAN_PS_TYPE_DISP_STRING);
						break;
					default:
						break;
				}
				break;	
			case ControlMode_Setting:
				switch (machine.ControlMode) {
					case Manual:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, MANUAL_CONTROL_MODE_DISP_STRING);
						break;
					case Automatic:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, AUTOMATIC_CONTROL_MODE_DISP_STRING);
						break;
					case FourAxis:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, FourAxis_CONTROL_MODE_DISP_STRING);
						break;
						
					default:
						break;
				}
				break;
			case RunMode_Setting:
				switch (machine.RunMode) {
					case WF_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_RUN_MODE_DISP_STRING);
						break;
					case WP_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_RUN_MODE_DISP_STRING);
						break;
					case HW_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, HW_RUN_MODE_DISP_STRING);
						break;
					case WF_And_WP_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_AND_WP_RUN_MODE_DISP_STRING);
						break;
					case WF_And_HW_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_AND_HW_RUN_MODE_DISP_STRING);
						break;
					case WP_And_HW_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WP_AND_HW_RUN_MODE_DISP_STRING);
						break;
					case WF_And_WP_And_HW_Mode:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, WF_AND_WP_AND_HW_RUN_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
			case LoadDefaults_Setting:
				switch (machine.LoadDefaultsMode) {
					case No_Load:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, NO_LOAD_DEFAULTS_MODE_DISP_STRING);
						break;
					case Yes_Load:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, YES_LOAD_DEFAULTS_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;
				
			case MenuMode_Setting:
				switch (menu[i].MenuMode) {
					case User:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, USER_MENU_MODE_DISP_STRING);
						break;
					case Boost:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, BOOST_MENU_MODE_DISP_STRING);
						break;
					case Calibration:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, CAL_MENU_MODE_DISP_STRING);
						break;
					case Configuration:
						xsprintf(varStingBlink[i], "%s %s", VAR_BLINK_STRING, CONFIG_MENU_MODE_DISP_STRING);
						break;
					default:
						break;
				}
				break;

		
			case ErrorCode_Ack://don't blink this variable, nothing to change...use varSting[i] not varStingBlink[i]
				switch (machine.ErrorCode) {
					case No_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, NO_ERROR_STRING);
						break;
					case Test_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, TEST_ERROR_STRING);
						break;
					case WF_0_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WF_0_MOTOR_HIGH_AMPS_STRING);
						break;
					case WP_0_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WP_0_MOTOR_HIGH_AMPS_STRING);
						break;
					case WF_1_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WF_1_MOTOR_HIGH_AMPS_STRING);
						break;
					case WP_1_Motor_High_Amps_Error:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, WP_1_MOTOR_HIGH_AMPS_STRING);
						break;
					case Hardware_Not_Attached:
						xsprintf(varSting[i], "%s %s", ERRORCODE_ACK_STRING, HARDWARE_NOT_ATTACHED_STRING);
						break;
					default:
						break;
				}
				break;	

			case PLC_Version:				

				xsprintf(varSting[i], "%s" , PLC_VERSION_STRING);
				
				break;	

			case NEXT_Project_Setting:				

				xsprintf(varSting[i], "Next: %s" , PROJECTS_TXT[NextProjectConfig]);
				xsprintf(varStingBlink[i], "Next: " );
				break;	
				
			default:
				break;
		}


		
	    
	   
		

		switch(LCD_Init_Step){

			case 0:

				Send_LCD_Cmd(&LCD_RS232[i] ,LCD_SET_CURSOR, 0);	

				LCD_Init_Step++;

				break;	

			case 1:

				Send_LCD_Cmd(&LCD_RS232[i] ,LCD_SET_CONTRAST, 40);	

				LCD_Init_Step++;

				break;	

			case 2:

				Send_LCD_Cmd(&LCD_RS232[i] ,LCD_SET_BACKLIGHT, 8);	

				LCD_Init_Step = 10;

				break;	

			case 10:

					
				//write to LCD

				memcpy(LCD_RS232[i].txData , LCD_CMD[LCD_SET_CURSOR].cmd , LCD_CMD[LCD_SET_CURSOR].len);

				p = LCD_RS232[i].txData + LCD_CMD[LCD_SET_CURSOR].len;
				
				if (menu[i].Edit == false) {

					strcpy(p , statusString[i]);
					LinePad(p  , 20);
					
					strcpy(p + 40 , varSting[i]);
					LinePad(p + 40 , 20);
					
					/*			
				
					memcpy (p, statusString[i], strlen(statusString[i]));  //line 1
					memset (p + 20, ' ', 20);	   //line 3
					memcpy (p + 40, varSting[i], strlen(varSting[i]));	   //line 2
					memset (p + 60, ' ', 20);	   //line 4

					*/
					   
				} else {

					strcpy(p , statusString[i]);
					LinePad(p  , 20);

					if (menu[i].Blink)
						strcpy(p + 40 , varSting[i]);
					else
						strcpy(p + 40 , varStingBlink[i]);
					
					LinePad(p + 40 , 20);
					

					/*
				
					if (menu[i].Blink == true) {
				
						memcpy (p, statusString[i], strlen(statusString[i]));			   //line 1
						memset (p + 20, ' ', 20);				   //line 3
						memcpy (p + 40, varStingBlink[i], strlen(varStingBlink[i]));	   //line 2
						memset (p + 60, ' ', 20);				   //line 4
				
					} else {
				
						memcpy (p, statusString[i], strlen(statusString[i]));  //line 1
						memset (p + 20, ' ', 20);	   //line 3
						memcpy (p + 40, varSting[i], strlen(varSting[i]));	   //line 2
						memset (p + 60, ' ', 20);	   //line 4
				
					}
					*/
				}				   
								   
				LCD_RS232[i].txDataLen = 83; //send length
				LCD_RS232[i].txRequest = 1; //send data flag
				

				break;	



			case 100: // For testing commands

				if(LCD_send_Cmd){
					LCD_send_Cmd = 0;
					//Send_LCD_Cmd(&LCD_RS232[i] ,LCD_SET_CURSOR, 0);
					Send_LCD_Cmd(&LCD_RS232[i] ,LCD_cmd_indx ,LCD_cmd_param);
				
				}
				
				break;	

						

		}
		
		SerialPortCyclic(&LCD_RS232[i]);

		_MenuButton[i] = menu[i].MenuButton;

	}

	
	

	//make copy to compare

	/*
	for (i=0; i<16; i++) {

		_hardware.Inputs[i] = hardware.Inputs[i];

	}
	*/
	
}


void _EXIT MenuExit(void)
{
	/* TODO: Add code here */
}


/*
========================================================================================
*/
void SerialPortInit(SerialPortDriver_typ *p, char *deviceStr, char *baudRateStr ) {
	/* Init Values */ 
	p->initError = 0; 
	p->rxReady = 0;
	p->rxError = 0;  
	p->txError = 0;  

	
	/* Port Parameters */
	strcpy(p->sDevice, deviceStr);
	strcpy(p->sMode, baudRateStr);
	//strcpy(p->sMode, "/BD=");
	//strcat(p->sMode, baudRateStr);
	//strcat(p->sMode, " /DB=8 /SB=1 /CCHAR1=0x0a");	/*Device sends: <string><CR<LF>, CR=0xd, LF=0xa*/

	/* Initialize Port Structure */ 
	p->Open.config 	= (UDINT) 0; 
	p->Open.device 	= (UDINT) p->sDevice; 
	p->Open.mode 	= (UDINT) p->sMode; 
	p->Open.enable 	= 1; 
  
  	/* Open Port */ 
	FRM_xopen(&p->Open); 							
	
	p->StatusOpen 	= p->Open.status; 							
	p->Ident 		= p->Open.ident; 							
	
	/* Check Port Open Status */  
	if ( p->Open.status ){
		p->initError = 1;
		p->Ident	= 0;
		return;
	}
		
	/* Initialize Get Buffer 	*/ 
	p->GetBuffer.enable 		= 1; 
	p->GetBuffer.ident 		= p->Ident; 
	/* Initialize Write Buffer 	*/ 
	p->Write.enable 			= 1; 
	p->Write.ident 			= p->Ident; 
	/* Initialize Release Output Buffer */ 
	p->ReleaseOutput.enable 	= 1; 
	p->ReleaseOutput.ident 	= p->Ident;
	/* Initialize Read */ 
	p->Read.enable 			= 1; 
	p->Read.ident 				= p->Ident; 
	/* Initialize Release Buffer */ 
	p->Release.enable 			= 1; 
	p->Release.ident 			= p->Ident; 
	/* Initialize Mode */ 
	p->Mode.enable 				= 1; 
	p->Mode.ident 				= p->Ident; 

}


/*
========================================================================================
*/
void SerialPortClose(SerialPortDriver_typ *p) {

	/* open error? -> stop! cant' close of never opened*/
	if ( p->initError || !p->Ident)
		return;

	/* Initialize Close */ 
	p->Close.enable 			= 1; 
	p->Close.ident 				= p->Ident; 

	FRM_close(&p->Close);
	

	/* Init Values */ 
	p->initError	= 0; 
	p->rxReady 		= 0;
	p->rxError 		= 0;  
	p->txError 		= 0;  
	p->Ident 		= 0;	

}

/*
========================================================================================
*/
void SerialPortCyclic(SerialPortDriver_typ *p) {

/*TODO: error handling!*/			 
	/* open error? -> stop! cant' do much if driver not ready*/
	if ( p->initError || !p->Ident)
		return;

	
	/* Write Message: */	
	if ( p->txRequest) { 
		/* Reset handshake flags*/
		p->txRequest  = 0; 
		p->txError = 0;  
		 			
		/* Get Send Buffer */ 
		FRM_gbuf(&p->GetBuffer); 
		p->pSendBuffer = (UDINT *) p->GetBuffer.buffer; 	/* Get Address 	*/ 
		p->SendLength = p->GetBuffer.buflng; 				/* Get Length 	*/ 
		p->StatusGetBuffer = p->GetBuffer.status; 			/* Get Status 	*/ 

/*TODO: error handling!*/			 
		/* Check Status */  
		if ( p->GetBuffer.status ){
			p->txError = 1;  
			return;
		}
					
	 	/* Write Data to Send Buffer */ 
	 	if(p->txDataLen)
	 		memcpy( p->pSendBuffer, p->txData, p->txDataLen ); 
	 	else
			memcpy( p->pSendBuffer, p->txData, strlen(p->txData) ); 

		 
		/* Write Port: Get Status */ 
		p->Write.buffer = (UDINT) p->pSendBuffer; 
		
		if(p->txDataLen)
			p->Write.buflng = p->txDataLen;
		else
			p->Write.buflng = strlen(p->txData);

		FRM_write(&p->Write); 				
		p->StatusWrite = p->Write.status; 	

		/* Check Status */  
		if( p->Write.status ) {
			
			/* Release Output Buffer */ 
			p->ReleaseOutput.buffer 		= (UDINT) p->pSendBuffer; 
			p->ReleaseOutput.buflng 		= p->SendLength; 
			FRM_robuf(&p->ReleaseOutput); 	
			p->StatusReleaseOutputBuffer 	= p->ReleaseOutput.status;			
			
			p->txError = 1;
		} 
		else{
			/* write ok; Message Sent; write buffer automaticaly released */
			p->txMessageCounter++;			
			p->txError = 0;
		}	
	} 

	
	/* Read Continuosly*/	 
	FRM_read(&p->Read); 		
	p->pReadBuffer = (UDINT *) p->Read.buffer; 		/* Get Address 	*/
	p->ReadLength = p->Read.buflng; 				/* Get Length 	*/ 
	p->StatusRead = p->Read.status; 				/* Get Status 	*/ 

	/* Check Status */   
	if ( !p->StatusRead ) {
		/* set handshake flag*/
		p->rxReady = 1; 
		p->rxError = 0;  
	 	memset( p->rxData, 0, sizeof(p->rxData) );
		/* Copy Read to Array */
		memcpy( p->rxData, (UDINT *) p->pReadBuffer, p->ReadLength);
		p->rxMessageCounter++;	
		memcpy( p->sInput, (UDINT *) p->pReadBuffer, p->ReadLength); 
		p->l = p->ReadLength;
		/* Release Read Buffer */
		p->Release.buffer 		= (UDINT) p->pReadBuffer; 
		p->Release.buflng 	= p->ReadLength; 
		FRM_rbuf(&p->Release); 						
		
	} 

	/*keep cheking for read errros; status=60->no data received*/
	if (p->Read.status && (p->Read.status != 60) ) {		
		p->rxError = 1;
	}
}

/********************************************/

void loadDefaults() {


	//initial settings
	//-------------------------------------------
	
	userValues.WF_Speed = WFSPEED_INITIAL_SP;
	userValues.WP_Freq = WPFREQ_INITIAL_SP;
	userValues.ARC_Amps = ARCAMPS_INITIAL_SP;
	userValues.HW_Amps = HWAMPS_INITIAL_SP;

	userValues.WF_Speed_Boost_Level_1 = BOOST_LEVEL_1_WF_INITIAL_SP;
	userValues.WF_Speed_Boost_Level_2 = BOOST_LEVEL_2_WF_INITIAL_SP;

	userValues.WP_Freq_Boost_Level_1 = BOOST_LEVEL_1_WP_INITIAL_SP;
	userValues.WP_Freq_Boost_Level_2 = BOOST_LEVEL_2_WP_INITIAL_SP;

	userValues.ARC_Amps_Boost_Level_1 = BOOST_LEVEL_1_ARC_INITIAL_SP;
	userValues.ARC_Amps_Boost_Level_2 = BOOST_LEVEL_2_ARC_INITIAL_SP;

	userValues.HW_Amps_Boost_Level_1 = BOOST_LEVEL_1_HW_INITIAL_SP;
	userValues.HW_Amps_Boost_Level_2 = BOOST_LEVEL_2_HW_INITIAL_SP;

	userValues.WF_On_Delay_Time = WFSTARTDELAY_INITIAL_SP;
	userValues.WP_On_Delay_Time = WPSTARTDELAY_INITIAL_SP;
	userValues.ARC_On_Delay_Time = ARCSTARTDELAY_INITIAL_SP;
	userValues.HW_On_Delay_Time = HWSTARTDELAY_INITIAL_SP;

	userValues.WF_Off_Delay_Time = WFSTOPDELAY_INITIAL_SP;
	userValues.WP_Off_Delay_Time = WPSTOPDELAY_INITIAL_SP;
	userValues.ARC_Off_Delay_Time = ARCSTOPDELAY_INITIAL_SP;
	userValues.HW_Off_Delay_Time = HWSTOPDELAY_INITIAL_SP;

	userValues.WF_On_Ramp_Time = WFSTARTRAMP_INITIAL_SP;
	userValues.WP_On_Ramp_Time = WPSTARTRAMP_INITIAL_SP;
	userValues.ARC_On_Ramp_Time = ARCSTARTRAMP_INITIAL_SP;
	userValues.HW_On_Ramp_Time = HWSTARTRAMP_INITIAL_SP;
	
	userValues.WF_Off_Ramp_Time = WFSTOPRAMP_INITIAL_SP;
	userValues.WP_Off_Ramp_Time = WPSTOPRAMP_INITIAL_SP;
	userValues.ARC_Off_Ramp_Time = ARCSTOPRAMP_INITIAL_SP;
	userValues.HW_Off_Ramp_Time = HWSTOPRAMP_INITIAL_SP;

	userValues.WF_Speed_Cal_Slope = WF_CAL_SLOPE_INITIAL_SP;
	userValues.WP_Speed_Cal_Slope = WP_CAL_SLOPE_INITIAL_SP;
	userValues.ARC_Amps_Cal_Slope = ARC_AMPS_CAL_SLOPE_INITIAL_SP;
	userValues.HW_Amps_Cal_Slope = HW_AMPS_CAL_SLOPE_INITIAL_SP;

	userValues.WF_Speed_Cal_Offset = WF_CAL_OFFSET_INITIAL_SP;
	userValues.WP_Speed_Cal_Offset = WP_CAL_OFFSET_INITIAL_SP;
	userValues.ARC_Speed_Cal_Offset = ARC_AMPS_CAL_OFFSET_INITIAL_SP;
	userValues.HW_Speed_Cal_Offset = HW_AMPS_CAL_OFFSET_INITIAL_SP;

	userValues.PrimaryPSType = PRIMARY_PS_TYPE_INITIAL_SP;
	userValues.ControlMode = CONTROL_MODE_INITIAL_SP;
	userValues.RunMode = RUN_MODE_INITIAL_SP;
	
	userValues.Boost1Type = BOOST_LEVEL_1_TYPE_INITIAL_SP; 
	userValues.Boost2Type = BOOST_LEVEL_2_TYPE_INITIAL_SP;

	userValues.FirstTimeThrough = true;

}


void LinePad(char* l , UINT len){
int i , k;

	k = strlen(l);

	if( k < len)		
		memset(l+k , ' ' , len -k);

return ;	

}

void loadUserValues() {

int i;	


	//initial settings
	//-------------------------------------------
	
	motors[Wf_0_Motor].Parameters.SpeedSetpoint = userValues.WF_Speed;
	motors[Wp_0_Motor].Parameters.SpeedSetpoint = userValues.WP_Freq;
	
	motors[Wf_1_Motor].Parameters.SpeedSetpoint = userValues.WF_Speed;
	motors[Wp_1_Motor].Parameters.SpeedSetpoint = userValues.WP_Freq;
	
	powerSupplies[PrimaryPS].Parameters.AmpsSetpoint = userValues.ARC_Amps;
	powerSupplies[HotwirePS].Parameters.AmpsSetpoint = userValues.HW_Amps;

	motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint = userValues.WF_Speed_Boost_Level_1;
	motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint = userValues.WF_Speed_Boost_Level_2;

	motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint = userValues.WP_Freq_Boost_Level_1;
	motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint = userValues.WP_Freq_Boost_Level_2;

	
	motors[Wf_1_Motor].Parameters.BoostLevel1SetPoint = userValues.WF_Speed_Boost_Level_1;
	motors[Wf_1_Motor].Parameters.BoostLevel2SetPoint = userValues.WF_Speed_Boost_Level_2;

	motors[Wp_1_Motor].Parameters.BoostLevel1SetPoint = userValues.WP_Freq_Boost_Level_1;
	motors[Wp_1_Motor].Parameters.BoostLevel2SetPoint = userValues.WP_Freq_Boost_Level_2;

	powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint = userValues.ARC_Amps_Boost_Level_1;
	powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint = userValues.ARC_Amps_Boost_Level_2;

	powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint = userValues.HW_Amps_Boost_Level_1;
	powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint = userValues.HW_Amps_Boost_Level_2;

	motors[Wf_0_Motor].Parameters.OnDelayTime = userValues.WF_On_Delay_Time;
	motors[Wp_0_Motor].Parameters.OnDelayTime = userValues.WP_On_Delay_Time;

	motors[Wf_1_Motor].Parameters.OnDelayTime = userValues.WF_On_Delay_Time;
	motors[Wp_1_Motor].Parameters.OnDelayTime = userValues.WP_On_Delay_Time;
	
	powerSupplies[PrimaryPS].Parameters.OnDelayTime = userValues.ARC_On_Delay_Time;
	powerSupplies[HotwirePS].Parameters.OnDelayTime = userValues.HW_On_Delay_Time;

	motors[Wf_0_Motor].Parameters.OffDelayTime = userValues.WF_Off_Delay_Time;
	motors[Wp_0_Motor].Parameters.OffDelayTime = userValues.WP_Off_Delay_Time;

	motors[Wf_1_Motor].Parameters.OffDelayTime = userValues.WF_Off_Delay_Time;
	motors[Wp_1_Motor].Parameters.OffDelayTime = userValues.WP_Off_Delay_Time;
	
	powerSupplies[PrimaryPS].Parameters.OffDelayTime = userValues.ARC_Off_Delay_Time;
	powerSupplies[HotwirePS].Parameters.OffDelayTime = userValues.HW_Off_Delay_Time;
	
	motors[Wf_0_Motor].Parameters.OnRampTime = userValues.WF_On_Ramp_Time;
	motors[Wp_0_Motor].Parameters.OnRampTime = userValues.WP_On_Ramp_Time;

	motors[Wf_1_Motor].Parameters.OnRampTime = userValues.WF_On_Ramp_Time;
	motors[Wp_1_Motor].Parameters.OnRampTime = userValues.WP_On_Ramp_Time;
	
	powerSupplies[PrimaryPS].Parameters.OnRampTime = userValues.ARC_On_Ramp_Time;
	powerSupplies[HotwirePS].Parameters.OnRampTime = userValues.HW_On_Ramp_Time;

	motors[Wf_0_Motor].Parameters.OffRampTime = userValues.WF_Off_Ramp_Time;
	motors[Wp_0_Motor].Parameters.OffRampTime = userValues.WP_Off_Ramp_Time;

	motors[Wf_1_Motor].Parameters.OffRampTime = userValues.WF_Off_Ramp_Time;
	motors[Wp_1_Motor].Parameters.OffRampTime = userValues.WP_Off_Ramp_Time;
	
	powerSupplies[PrimaryPS].Parameters.OffRampTime = userValues.ARC_Off_Ramp_Time;
	powerSupplies[HotwirePS].Parameters.OffRampTime = userValues.HW_Off_Ramp_Time;

	motors[Wf_0_Motor].HardwareConfig.CalSlope = userValues.WF_Speed_Cal_Slope;
	motors[Wp_0_Motor].HardwareConfig.CalSlope = userValues.WP_Speed_Cal_Slope;

	motors[Wf_1_Motor].HardwareConfig.CalSlope = userValues.WF_Speed_Cal_Slope;
	motors[Wp_1_Motor].HardwareConfig.CalSlope = userValues.WP_Speed_Cal_Slope;	
	
	powerSupplies[PrimaryPS].HardwareConfig.CalSlope = userValues.ARC_Amps_Cal_Slope;
	powerSupplies[HotwirePS].HardwareConfig.CalSlope = userValues.HW_Amps_Cal_Slope;

	motors[Wf_0_Motor].HardwareConfig.CalOffset = userValues.WF_Speed_Cal_Offset;
	motors[Wp_0_Motor].HardwareConfig.CalOffset = userValues.WP_Speed_Cal_Offset;

	motors[Wf_1_Motor].HardwareConfig.CalOffset = userValues.WF_Speed_Cal_Offset;
	motors[Wp_1_Motor].HardwareConfig.CalOffset = userValues.WP_Speed_Cal_Offset;
	
	powerSupplies[PrimaryPS].HardwareConfig.CalOffset = userValues.ARC_Speed_Cal_Offset;
	powerSupplies[HotwirePS].HardwareConfig.CalOffset = userValues.HW_Speed_Cal_Offset;

	machine.PrimaryPSType = userValues.PrimaryPSType;
	machine.ControlMode = userValues.ControlMode;
	machine.RunMode = userValues.RunMode;

	machine.Boost1Type = userValues.Boost1Type; 
	machine.Boost2Type = userValues.Boost2Type;

	AssignHardwareParam();

	
	

}


void saveUserValues() {


	//initial settings
	//-------------------------------------------
	
	userValues.WF_Speed = motors[Wf_0_Motor].Parameters.SpeedSetpoint;
	userValues.WP_Freq = motors[Wp_0_Motor].Parameters.SpeedSetpoint;
	userValues.ARC_Amps = powerSupplies[PrimaryPS].Parameters.AmpsSetpoint;
	userValues.HW_Amps = powerSupplies[HotwirePS].Parameters.AmpsSetpoint;

	userValues.WF_Speed_Boost_Level_1 = motors[Wf_0_Motor].Parameters.BoostLevel1SetPoint;
	userValues.WF_Speed_Boost_Level_2 = motors[Wf_0_Motor].Parameters.BoostLevel2SetPoint;

	userValues.WP_Freq_Boost_Level_1 = motors[Wp_0_Motor].Parameters.BoostLevel1SetPoint;
	userValues.WP_Freq_Boost_Level_2 = motors[Wp_0_Motor].Parameters.BoostLevel2SetPoint;

	userValues.ARC_Amps_Boost_Level_1 = powerSupplies[PrimaryPS].Parameters.BoostLevel1SetPoint;
	userValues.ARC_Amps_Boost_Level_2 = powerSupplies[PrimaryPS].Parameters.BoostLevel2SetPoint;

	userValues.HW_Amps_Boost_Level_1 = powerSupplies[HotwirePS].Parameters.BoostLevel1SetPoint;
	userValues.HW_Amps_Boost_Level_2 = powerSupplies[HotwirePS].Parameters.BoostLevel2SetPoint;

	userValues.WF_On_Delay_Time = motors[Wf_0_Motor].Parameters.OnDelayTime;
	userValues.WP_On_Delay_Time = motors[Wp_0_Motor].Parameters.OnDelayTime;
	userValues.ARC_On_Delay_Time = powerSupplies[PrimaryPS].Parameters.OnDelayTime;
	userValues.HW_On_Delay_Time = powerSupplies[HotwirePS].Parameters.OnDelayTime;

	userValues.WF_Off_Delay_Time = motors[Wf_0_Motor].Parameters.OffDelayTime;
	userValues.WP_Off_Delay_Time = motors[Wp_0_Motor].Parameters.OffDelayTime;
	userValues.ARC_Off_Delay_Time = powerSupplies[PrimaryPS].Parameters.OffDelayTime;
	userValues.HW_Off_Delay_Time = powerSupplies[HotwirePS].Parameters.OffDelayTime;

	userValues.WF_On_Ramp_Time = motors[Wf_0_Motor].Parameters.OnRampTime;
	userValues.WP_On_Ramp_Time = motors[Wp_0_Motor].Parameters.OnRampTime;
	userValues.ARC_On_Ramp_Time = powerSupplies[PrimaryPS].Parameters.OnRampTime;
	userValues.HW_On_Ramp_Time = powerSupplies[HotwirePS].Parameters.OnRampTime;
	
	userValues.WF_Off_Ramp_Time = motors[Wf_0_Motor].Parameters.OffRampTime;
	userValues.WP_Off_Ramp_Time = motors[Wp_0_Motor].Parameters.OffRampTime;
	userValues.ARC_Off_Ramp_Time = powerSupplies[PrimaryPS].Parameters.OffRampTime;
	userValues.HW_Off_Ramp_Time = powerSupplies[HotwirePS].Parameters.OffRampTime;

	userValues.WF_Speed_Cal_Slope = motors[Wf_0_Motor].HardwareConfig.CalSlope;
	userValues.ARC_Amps_Cal_Slope = powerSupplies[PrimaryPS].HardwareConfig.CalSlope;
	userValues.HW_Amps_Cal_Slope = powerSupplies[HotwirePS].HardwareConfig.CalSlope;

	userValues.WF_Speed_Cal_Offset = motors[Wf_0_Motor].HardwareConfig.CalOffset;
	userValues.ARC_Speed_Cal_Offset = powerSupplies[PrimaryPS].HardwareConfig.CalOffset;
	userValues.HW_Speed_Cal_Offset = powerSupplies[HotwirePS].HardwareConfig.CalOffset;

	userValues.PrimaryPSType = machine.PrimaryPSType;
	userValues.ControlMode = machine.ControlMode;
	userValues.RunMode = machine.RunMode;

	userValues.Boost1Type = machine.Boost1Type; 
	userValues.Boost2Type = machine.Boost2Type;

}


void Send_LCD_Cmd(SerialPortDriver_typ *rs, UINT cmd , char par){

	memcpy(rs->txData , LCD_CMD[cmd].cmd , LCD_CMD[cmd].len);
	rs->txDataLen = LCD_CMD[cmd].len;
	rs->txRequest = 1;
	
	if((    cmd == LCD_SET_CURSOR) 
		|| (cmd == LCD_SET_CONTRAST) 
		|| (cmd == LCD_SET_BACKLIGHT) )
		
		rs->txData[2] = par;

	return;
	


}


/*
			//arc or rem
            if (machine.ControlMode == Manual) {

            	if (powerSupplies[PrimaryPS].Command.command_sm.Step == Running_Power_Supply_State || menu[i].ArcDisplayBlinkEnable == true) {
                                
					if (menu[i].ArcDisplayBlinkEnable == false) {
						xsprintf(arcStatusString, "%s", ARC_DISP_STRING);
					} else {
						if (menu[i].Blink == true) {
							xsprintf(arcStatusString, "%s", NO_ARC_DISP_STRING);
						} else {
							xsprintf(arcStatusString, "%s", ARC_DISP_STRING);
						}
					}
                                
				} else {
					xsprintf(arcStatusString, "%s", NO_ARC_DISP_STRING);
				}

            } else if (machine.ControlMode == Automatic) {

            	if (hardware.Inputs[RemoteSeqStart] || menu[i].RemDisplayBlinkEnable == true) {
                                
					if (menu[i].RemDisplayBlinkEnable == false) {
						xsprintf(remStatusString, "%s", REM_DISP_STRING);
					} else {
						if (menu[i].Blink == true) {
							xsprintf(StatusString, "%s", NO_REM_DISP_STRING);
						} else {
							xsprintf(remStatusString, "%s", REM_DISP_STRING);
						}
					}
                                
				} else {
					xsprintf(remStatusString, "%s", NO_REM_DISP_STRING);
				}

            }
*/


void AssignHardwareParam(){
int i;

	for (i=0; i<NUM_POWER_SUPPLIES; i++) {	//PSs 0 through 2
	
		switch (i) {
				
			case PrimaryPS:
	
				switch (machine.PrimaryPSType) {

					case Miller_Type:

						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = MILLER_PS_AMPS_AT_MAX_COMMAND_V / MILLER_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = MILLER_PS_AMPS_AT_MAX_FEEDBACK_V / MILLER_PS_MAX_FEEDBACK_V;
						break;

					case GTII_Type:
						
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = GTII_PS_AMPS_AT_MAX_COMMAND_V / GTII_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = GTII_PS_AMPS_AT_MAX_FEEDBACK_V / GTII_PS_MAX_FEEDBACK_V;
						break;

					case GTVI_Type:
						
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = GTVI_PS_AMPS_AT_MAX_COMMAND_V / GTVI_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = GTVI_PS_AMPS_AT_MAX_FEEDBACK_V / GTVI_PS_MAX_FEEDBACK_V;
						break;

					case LINCS350_Type:
						
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = LINCS350_PS_AMPS_AT_MAX_COMMAND_V / LINCS350_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = LINCS350_PS_AMPS_AT_MAX_FEEDBACK_V / LINCS350_PS_MAX_FEEDBACK_V;
						break;	

					case LINCS500_Type:
						
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = LINCS500_PS_AMPS_AT_MAX_COMMAND_V / LINCS500_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = LINCS500_PS_AMPS_AT_MAX_FEEDBACK_V / LINCS500_PS_MAX_FEEDBACK_V;
						break;		

					default:
						break;
				}
					
				break;
				
			case HotwirePS:

				switch (machine.HotwirePSType) {

					case Jackle_Type:
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = JACKLE_PS_AMPS_AT_MAX_COMMAND_V / JACKLE_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = JACKLE_PS_AMPS_AT_MAX_FEEDBACK_V / JACKLE_PS_MAX_FEEDBACK_V;
						break;

					case Acopian_Type:
						powerSupplies[i].HardwareConfig.AmpsPerVoltCommand = ACCOPIAN_PS_AMPS_AT_MAX_COMMAND_V / ACCOPIAN_PS_MAX_COMMAND_V;
						powerSupplies[i].HardwareConfig.AmpsPerVoltFeedback = ACCOPIAN_PS_AMPS_AT_MAX_FEEDBACK_V / ACCOPIAN_PS_MAX_FEEDBACK_V;				
						
											
						break;
	
					default:
						break;
				}
	
				break;	
		
		}


	};


}

