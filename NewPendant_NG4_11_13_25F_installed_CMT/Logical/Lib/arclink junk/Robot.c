/*
 * Handler.c
 *
 *  Created on: May 18, 2017
 *      Author: Obed Bardales
 */
#ifndef _ROBOT_C
#define _ROBOT_C


/* Handle Robot Cyclic Operations */
void handleRobotCyclicOps(Arc_Link_typ *arc){
	if(!arc) return;

	// Handle Mr.Robot interface 
	handleRobotInterface(arc);

	// "The difference is not one thing, it's everything!"
}

/* Get Network Configuration from Robot */
void getNetworkConfigurationAttribute(Arc_Link_typ *arc, USINT *ResponseData){
	Arc_Link_Request_typ *req;
	Arc_Link_Response_typ *r;
	Robot_typ *robot;
	ServiceID_typ *serviceID;
	ArcLinkObject_typ *arclink;
	USINT data[10]; // Temp Data place holder
	UINT txtlength; // text length 
	
	if(!arc) return;
	if(!ResponseData) return;

	robot = &arc->robot; // Reference to Robot
	req = &arc->msgIN.request;
	serviceID = &req->ServiceID;
	r = &arc->msgOUT.response;
	arclink = &robot->arclink;
	int dataSize = req->MaxResponseSize;
	
	/* Attribute Number */
	switch (serviceID->AttributeNumber) {

		case VENDOR_ID: /* 0 */
			// MaxDataSize = 2
			// Big Endian Format

			data[0] = (USINT) (arclink->network.VendorID >> 8);
			data[1] = (USINT) (arclink->network.VendorID >> 0);
			memcpy(ResponseData, data, 2);
			r->ResponseSize = 2;
		
			return;

		case CLASS_ID: /* 4 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.ClassID >> 8);
			data[1] = (USINT) (arclink->network.ClassID >> 0);
			memcpy(ResponseData, data, 2);
			r->ResponseSize = 2;
			return;

		case CLASS_TYPE: /* 7 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.ClassType >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;

		case GROUP_ASSIGNMENTS: /* 10 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.GroupAssignments >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;

		case INSTANCE_ASSIGNMENTS: /* 11 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.InstanceAssignments >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;

		case FAULT_STATUS: /* 14 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.FaultStatus >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;
		
		case SILICON_SERIAL_NUMBER: /* 16 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.SiliconSerialNumber >> 8);
			data[1] = (USINT) (arclink->network.SiliconSerialNumber >> 0);
			memcpy(ResponseData, data, 2);
			r->ResponseSize = 2;
			return;

		case NUMBER_OF_EMBEDDED_OBJECTS: /* 17 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.NumberofEmbeddedObjects >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;
		
		case SOFTWARE_VERSION: /* 19 */
			// MaxDataSize = 64
			// Big Endian Format
			txtlength = strlen(arclink->network.SoftwareVersion);
			// Swap Bytes before sendSuccesfullWriteAttributeConfirmation
			//swapBytes(arclink->network.SoftwareVersion, txtlength);
			memcpy(ResponseData, arclink->network.SoftwareVersion, txtlength);
			//swapBytes(arclink->network.SoftwareVersion, txtlength);
			r->ResponseSize = txtlength; 
			return;

		case BOOTSTRAP: /* 24 */
			// MaxDataSize = 2
			// Big Endian Format
			data[0] = (USINT) (arclink->network.Bootstrap >> 0);
			memcpy(ResponseData, data, 1);
			r->ResponseSize = 1;
			return;
		
		case PAIRED_ITEMS_COUNT: /* 50 */
			// MaxDataSize = 2
			// Big Endia Format
			data[0] = (USINT) (arclink->network.PairedItemsCount >> 8);
			data[1] = (USINT) (arclink->network.PairedItemsCount >> 0);
			memcpy(ResponseData, data, 2);
			r->ResponseSize = 2;
			return;
	}	
}

/* Write Network Configuration Attribute */
BOOL writeNetworkConfigurationAttribute(Arc_Link_typ *arc){
	
	BOOL success = 1;
	
	UINT *data; 
	
	Arc_Link_Request_typ *req;
	ServiceID_typ *serviceID;
	Robot_typ *robot;
	ArcLinkObject_typ *arclink;
	if(!arc) return;
	
	req = &arc->msgIN.request;
	serviceID = &req->ServiceID;
	robot = &arc->robot;
	arclink = &robot->arclink;

	DBG("Inside writeNetworkConfigurationAttribute \r\n" );		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(req->RequestSize > 0){
	//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data 
	}else{
		DBG("Error: writeNetworkConfigurationAttribute, Error with Request size\r\n" );	
		return;
	}

	/* Attribute Number */
	switch (serviceID->AttributeNumber) {

	// Currently Supported Attributes by the Robot
	case RESET:
		arclink->network.Reset = *(data + 0);//data[0]; // Size is typically 1 byte
		arclink->network.BusAvailable = 0; // Robot must enter to unmapped state
		robot->isMapping = 0;
		robot->isMapped = 0;
		robot->Cntrl.Fbk.Status &= ~ArcLink_Mapped;	
		robot->Cntrl.Fbk.Status &= ~ArcLink_Mapping;	
		robot->isMiscSetupOneShot = 0;
		DBG("Setting Robot to Unmapped state\r\n" );	
		break;

	case GROUP_ASSIGNMENTS:
		arclink->network.GroupAssignments = *(data + 0);///data[0]; // Size is typically 1
													// byte

		break;

	case INSTANCE_ASSIGNMENTS:
		arclink->network.InstanceAssignments = *(data + 0);///data[0]; // Size is typically
														// 1 byte
		break;

	case BUS_AVAILABLE:
		arclink->network.BusAvailable = *(data + 0); //data[0]; // Size is typically 1 byte
		robot->isMapping 	= 0;
		robot->isMapped 	= 1;
		robot->Cntrl.Fbk.Status |= ArcLink_Mapped;
		robot->Cntrl.Fbk.Status &= ~ArcLink_Mapping;	
		DBG("Robot was mapped succesfully Świetny programista! \r\n" );	
		log_event(&robot->eventlog , "Arc Link Mapped");
		break;

	}

	return success; // Whether Attribute was written succesfully

}

void handleRobotInterface(Arc_Link_typ *arc){
	
	Robot_typ *robot;
	Robot_Intrf_typ *intrf;
	if(!arc) return;
	
	robot = &arc->robot;
	intrf = &robot->Intrf;

	// Robot must be Mapped with WeldController
	if(!robot->isMapped) return;


	//  Handle state changes from outside and provide feedback

	handleRobotCntrl(arc);
	error_status_handler(arc);
	ArcLink_Timer(arc);
	/////////	
	
	// Handle xCMD
	switch(intrf->xCMD) {
		/* Turn Power Supply ON */
		case ROBOT_CMD_PS_ON:
			writeOutputOnCommand(arc);
			DBG("Robot commanded Power Supply ON! \r\n" );	
			break;
	
		/* Turn Power Supply OFF */
		case ROBOT_CMD_PS_OFF:
			writeOutputOffCommand(arc);
			DBG("Robot commanded Power Supply OFF! \r\n" );	
			break;

		/* Set Controls Action Request */
		case ROBOT_SET_CONTROLS:
			createSetControlsAction(arc);
			DBG("Robot sent Set Controls Action Request! \r\n" );	
			break;

		case ROBOT_CMD_COOLER_ON:
			writeOutputCoolerOnCommand(arc);
			DBG("Robot commanded Cooler ON! \r\n" );

		break;

		case ROBOT_CMD_COOLER_OFF:
			writeOutputCoolerOffCommand(arc);
			DBG("Robot commanded Cooler OFF! \r\n" );

		break;

		case ROBOT_CMD_SEARCH_COOLER_ADR:
			action_ConfigDataSearch(arc);
			DBG("commanded Get Cooler Adr! \r\n" );
		break;


		case ROBOT_READ_COOLER_SENS:
			readAttribute_CoolerSens(arc);
			DBG("commanded Get Cooler Sens! \r\n" );
		break;

	}

	// Clear xCMD
	if (intrf->xCMD > 0) intrf->xCMD = 0;

}


void handleRobotCntrl(Arc_Link_typ *arc){
	
	Robot_typ *robot;
	Robot_Intrf_typ *intrf;
	if(!arc) return;
	
	robot = &arc->robot;
	intrf = &robot->Intrf;


	//*************** CMD  *****************************/
	//**************Highest priority CMD****************//
	if(robot->Cntrl.Cmd.COOLER && !robot->Cntrl.Int._COOLER){

		if(robot->miscSetup.CoolerAddr > 0){
			
			intrf->xCMD = ROBOT_CMD_COOLER_ON;
			robot->Cntrl.Int._COOLER = robot->Cntrl.Cmd.COOLER;

		}

	}	
	else if(robot->Cntrl.Cmd.RUN != robot->Cntrl.Int._RUN){

		
		if(robot->Cntrl.Cmd.RUN)
			intrf->xCMD = ROBOT_CMD_PS_ON;
		else
			intrf->xCMD = ROBOT_CMD_PS_OFF;

		robot->Cntrl.Int._RUN  = robot->Cntrl.Cmd.RUN;
	}
	else if(!robot->Cntrl.Cmd.COOLER && robot->Cntrl.Int._COOLER){


		if(robot->miscSetup.CoolerAddr > 0){

			intrf->xCMD = ROBOT_CMD_COOLER_OFF;
			robot->Cntrl.Int._COOLER  = robot->Cntrl.Cmd.COOLER;

		}

	}	
	//robot->Cntrl.Int._RUN  = robot->Cntrl.Cmd.RUN;
	//robot->Cntrl.Int._COOLER  = robot->Cntrl.Cmd.COOLER;
	
	//********************************************************//

	if(!intrf->xCMD){
		
		if((robot->Cntrl.Cmd.Mode != robot->Cntrl.Int._Mode )
			|| (robot->Cntrl.Cmd.AmpSP != robot->Cntrl.Int._AmpSP )
			|| (robot->Cntrl.Cmd.BckAmpSP != robot->Cntrl.Int._BckAmpSP )
			|| (robot->Cntrl.Cmd.PulseFreq != robot->Cntrl.Int._PulseFreq )
			|| (robot->Cntrl.Cmd.PulseWidth != robot->Cntrl.Int._PulseWidth )){


				// Prepare data  - please Obed do it

				switch(robot->Cntrl.Cmd.Mode){

					case 3:
						/*
						1 Byte	Parameter Mask
						2 Bytes	Weld Mode 	If parameter mask bit 0 set.
						2 Bytes	Work-point	If parameter mask bit 1 set.
						2 Bytes	Trim	If parameter mask bit 2 set.
						2 Bytes	Wave-Control 1	If parameter mask bit 3 set.
						2 Bytes	Wave-Control 2	If parameter mask bit 4 set.
						2 Bytes	Wave-Control 3	If parameter mask bit 5 set.
						2 Bytes	Wave-Control 4	If parameter mask bit 6 set.
						*/
						// Arclink Set Control Action Sample for Mode 3 to give initialization values
						robot->wc.setup.ctrlParam.ParameterMask = 0x23; // 0b0100011; // change accordingly for Mode 3
						UINT *mode 	= &robot->Cntrl.Cmd.Mode; // Mode
						UINT ampSP = (UINT)robot->Cntrl.Cmd.AmpSP; //amps	
						robot->wc.setup.ctrlParam.WeldMode = *mode;
						robot->wc.setup.ctrlParam.WorkPoint = ampSP;
						robot->wc.setup.ctrlParam.WaveControl3 = 1; // Hot pulse. Need info from Lincoln
						// Message Ready to be sent

						// This command write Controls Parameter into Power Supply
						intrf->xCMD = ROBOT_SET_CONTROLS;
					break;

				}

				
		}
		robot->Cntrl.Int._Mode 			= robot->Cntrl.Cmd.Mode;	
		robot->Cntrl.Int._AmpSP 		= robot->Cntrl.Cmd.AmpSP; 
		robot->Cntrl.Int._BckAmpSP 		= robot->Cntrl.Cmd.BckAmpSP; 
		robot->Cntrl.Int._PulseFreq 	= robot->Cntrl.Cmd.PulseFreq; 
		robot->Cntrl.Int._PulseWidth 	= robot->Cntrl.Cmd.PulseWidth; 	

	}
	//********************************************************//

	//*****************Lowest Priority CMD ********************//
	if(!intrf->xCMD){
		if(robot->Cntrl.Cmd.RST != robot->Cntrl.Int._RST){

			robot->Cntrl.Fbk.Status = 0;
			robot->miscSetup.state = 0; //reset misc state machine state
			robot->miscSetup.CoolerAddr = 0;
			robot->isMapping = 0;
			robot->isMapped = 0;	    //clear flag, set to unmapped status, stop timer
			robot->isMiscSetupOneShot = 0;
			robot->Comm.ArcComTimer.IN = 0;
			robot->Comm.StopRespondHeartBeat = 1; //halt heartbeat,force master to send out RESET request

			robot->Cntrl.Int._COOLER = 0;
			
		}
		robot->Cntrl.Int._RST 			= robot->Cntrl.Cmd.RST; 

	}

	//*************** FEEDBACK  *****************************/

	robot->Cntrl.Fbk.curArcVolt = robot->wc.data.telemetry.fVoltage;
	robot->Cntrl.Fbk.curArcAmp  = robot->wc.data.telemetry.fCurrent;
	//robot->Cntrl.Fbk.Error      =  ???????
	//robot->Cntrl.Fbk.Status      =  ???????	

	//********************************************************//


	return;
	
}

void error_status_handler(Arc_Link_typ *arc)
{

	Robot_typ *robot;
	if(!arc) return;
	
	robot = &arc->robot;

	// System is ready when mapped, ready status from PS is set and Arc Link Active, could add no system fault here
	if ((robot->isReady) && (robot->isMapped) && (robot->Comm.ArcLinkActive) && !robot->Cntrl.Fbk.Error)
	{
		robot->Cntrl.Fbk.Status |= PS_Ready;
//		log_event(&robot->eventlog , "Power supply is ready");
	}
		
	else 
		robot->Cntrl.Fbk.Status &= ~PS_Ready;
	
	/*
	if ((robot->Cntrl.Fbk.Error >> 1 & 0x0001) || (robot->Cntrl.Fbk.Error >> 2 & 0x0001))  //either system fault from PS or network error will generate fault condition
		robot->Cntrl.Fbk.Error |= PS_Error_Present;
	else 
		robot->Cntrl.Fbk.Error &= ~PS_Error_Present;
	*/
}
void ArcLink_Timer(Arc_Link_typ *arc){
//  
	Robot_typ *robot;
	if(!arc) return;
	
	robot = &arc->robot;
	
	robot->Comm.PT = 2000; // 2s  timeout 

	robot->Comm.CoolerSensPull_PT = 500; // 0.5s  timer
	
    robot->Comm.ArcComTimer.PT = robot->Comm.PT;
	
	if (robot->Comm.HeartBeat_Received && (robot->Comm.ArcComTimer.ET > 0)) //everytime got heartbeat, reset timer, to prevent trigger timeout
	    robot->Comm.ArcComTimer.IN = 0;
	
	else if (robot->Comm.HeartBeat_Received && (robot->Comm.ArcComTimer.ET == 0)){ //start timer, reset heartbeat flag
		if (robot->isMapped){  //
			robot->Comm.ArcComTimer.IN = 1;
		}	    
		if (robot->Comm.ClearError)    // clear error once getting connectivity from PS
		{
			robot->Comm.ClearError = 0;
			robot->Cntrl.Fbk.Error = 0;
			log_event(&robot->eventlog , "Network is active");
		}
		robot->Comm.HeartBeat_Received = 0;   //reset flag to start watchdog
	
	}

	TON(&robot->Comm.ArcComTimer);  //2 seconds Watchdog 

    if (!robot->Comm.ArcComTimer.Q)	
	robot->Comm.ArcLinkActive = 1;  // 1 Active
	else 
	robot->Comm.ArcLinkActive = 0;	//timeout
	
	if (!robot->Comm.ArcLinkActive)
	{
		DBG_OB("Arc Link Disconnected\r\n"); 
		robot->Cntrl.Fbk.Error |= ArcLink_Com_Failed;
		robot->Cntrl.Fbk.Status = 0;
		robot->isMapping = 0;
		robot->isMapped = 0;	    //clear flag, set to unmapped status, stop timer
		robot->isMiscSetupOneShot = 0;
		robot->Comm.ArcComTimer.IN = 0;
		robot->Comm.ClearError = 1;	 //set flag when getting communication back, reset error
		log_event(&robot->eventlog , "Network is down");
	}
	else if (robot->isMapped)
	{
	/*	DBG_OB("Arc Link Connected\r\n"); */
		robot->Cntrl.Fbk.Error &= ~ArcLink_Com_Failed;
		robot->Cntrl.Fbk.Status |= ArcLink_Active;

		
	}

	// Pull Cooler Sens Status

	if((robot->miscSetup.CoolerAddr > 0)
		&& (robot->Comm.CoolerSensPull_PT > 0)){

		robot->Comm.CoolerSensPullTimer.PT = robot->Comm.CoolerSensPull_PT;	

		if(robot->isMapped && robot->Comm.ArcLinkActive){

			if(!robot->Comm.CoolerSensPullTimer.IN){
				readAttribute_CoolerSens(arc);
				DBG("commanded Get Cooler Sens! \r\n" );
				robot->Comm.CoolerSensPullTimer.IN = 1;
				//TON(&robot->Comm.CoolerSensPullTimer);
			}
			else if(robot->Comm.CoolerSensPullTimer.Q){

				robot->Comm.CoolerSensPullTimer.IN = 0;
				//TON(&robot->Comm.CoolerSensPullTimer);

			}
			//else
				//TON(&robot->Comm.CoolerSensPullTimer);

		}

		TON(&robot->Comm.CoolerSensPullTimer);
	}
	
}
#endif /* _ROBOT_C */