/*
 * Handler.c
 *
 *  Created on: May 18, 2017
 *      Author: Obed Bardales
 */
#ifndef _HANDLER_C
#define _HANDLER_C



#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif

_GLOBAL UINT SEQUENCE_NUMBER;
UINT rand();

/*
	Handle ArcLink Request Messages
	
	@param *arc <b> reference to Arc_Link_typ containing required structures </b>
	@return void
	@revision 0
	by Obed Bardales
	+===== List of changes =====+
	- first revision
*/
void handleArcLinkRequests( Arc_Link_typ *arc) {

	Arc_Link_Request_typ *r; // temp reference to request
    Robot_typ *robot;

	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	/*
	 * Header size is 10 bytes array index 0 - 9, therefore incoming data
	 * i.e ArcLink Request comes in index 10
	 */
	DBG("Inside handler - AddressingMode: %d \r\n" , r->AddressingMode);
	/* Process Request based upon the addressing mode */
	switch (r->AddressingMode) {
		case ADDRESS_MODE_MASTER_SLAVE:
			handleMasterToSlaveRequest(arc); // [OB]
			break;

		case ADDRESS_MODE_MASTER_SLAVES:
			handleMasterToSlavesRequest(arc); // [OB]
			break;

		case ADDRESS_MODE_SLAVE_MASTER:
			break;

		case ADDRESS_MODE_PEER_PEER:
			handlePeerToPeerRequest(arc); // [OB]
			break;

		case ADDRESS_MODE_PEER_PEERS:
			handlePeerToPeersRequest(arc); // Chris need to implement this for telemetry
			break;

	}
}


/*
	Handle ArcLink MasterToSlaves Address Mode Message Request This helps to
	understand the Services supported by this object(Robot). It has been separated just
	for organizational purposes
	
	@param h
	@param r
*/
void handleMasterToSlavesRequest( Arc_Link_typ *arc) {

	Arc_Link_Request_typ *r; // temp reference to request


	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;

	DBG("Inside handleMasterToSlavesRequest - Service: %d \r\n" , r->Service);		
	// Handle the service Requested
	switch (r->Service) {
		case NETWORK_SERVICE_ATTRIBUTE_READ:
			break;

		case NETWORK_SERVICE_ATTRIBUTE_WRITE:
			handleWriteAttributeClassProperty(arc); // [OB]
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_ACTION:
			handleNetworkAction(arc); // [OB work in progress]
			break;

		case NETWORK_SERVICE_READ_BLOCK:
			break;

		case NETWORK_SERVICE_WRITE_BLOCK:
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_BLOCK_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_BLOCK_REPORT:
			break;
	}

}


/*
	Handle ArcLink MasterToSlave Address Mode Message Request This helps to
	understand the Services supported by this object(Robot). It has been separated just
	for organizational purposes
*/
void handleMasterToSlaveRequest( Arc_Link_typ *arc) {

	Arc_Link_Request_typ *r; // temp reference to request


	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;

	// Handle the service Requested
	switch (r->Service) {
		case NETWORK_SERVICE_ATTRIBUTE_READ:
			break;

		case NETWORK_SERVICE_ATTRIBUTE_WRITE:
			handleWriteAttributeClassProperty(arc); // [OB]
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_ACTION:
			handleNetworkAction(arc); 
			break;

		case NETWORK_SERVICE_READ_BLOCK:
			break;

		case NETWORK_SERVICE_WRITE_BLOCK:
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_BLOCK_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_BLOCK_REPORT:
			break;
	}
}



/*
	Handle ArcLink PeerToPeer Address Mode Message Request This helps to
	understand the Services supported by this object(Robot). It has been separated just
	for organizational purposes
*/
void handlePeerToPeerRequest( Arc_Link_typ *arc) {
		
	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;

	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;
	robot = &arc->robot;
		
	if (r->DestinationAddress == robot->arclink.network.ModuleBaseAddress) {

	} else{
		// [TODO: Develop Error of wrong destination address if necessary ]	
		DBG("Wrong Destination Address in handlePeerToPeerRequest. \r\n");		
		return; // Wrong destination
	}
		
	
	// Handle the service Requested
	switch (r->Service) {
		case NETWORK_SERVICE_ATTRIBUTE_READ:
			handleReadAttributeClassProperty(arc); // [OB]
			break;

		case NETWORK_SERVICE_ATTRIBUTE_WRITE:
			handleWriteAttributeClassProperty(arc); // [OB]
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_ACTION:
			handleNetworkAction(arc); 
			break;

		case NETWORK_SERVICE_READ_BLOCK:
			handleNetworkBlock(arc); // [OB]
			break;

		case NETWORK_SERVICE_WRITE_BLOCK:
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_BLOCK_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_BLOCK_REPORT:
			break;
	}

}

/*
	Handle ArcLink PeerToPeers Address Mode Message Request This helps to
	understand the Services supported by this object(Robot). It has been separated just
	for organizational purposes
*/
void handlePeerToPeersRequest( Arc_Link_typ *arc) {
		
	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;

	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;
	robot = &arc->robot;
		
	if (r->DestinationAddress == ARC_LINK_TELEMETRY_DESTINATION_ADDRESS || r->DestinationAddress == ACR_LINK_TELEMETRY_ALL_OBJECT_ADDRESS) {

	} else{
		// [TODO: Develop Error of wrong destination address if necessary ]	
		DBG("Wrong Destination Address in handlePeerToPeersRequest. \r\n");		
		return; // Wrong destination
	}
		
	
	// Handle the service Requested
	switch (r->Service) {
		case NETWORK_SERVICE_ATTRIBUTE_READ:
			//handleReadAttributeClassProperty(arc); // [OB] not used or required
			break;

		case NETWORK_SERVICE_ATTRIBUTE_WRITE:
			//handleWriteAttributeClassProperty(arc); // [OB] not used or required
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_REPORT:
			handleUnconfirmedEvent(arc);
			break;

		case NETWORK_SERVICE_ACTION:
			//handleNetworkAction(arc); // not used or required
			break;

		case NETWORK_SERVICE_READ_BLOCK:
			//handleNetworkBlock(arc); // not used or required
			break;

		case NETWORK_SERVICE_WRITE_BLOCK:
			break;

		case NETWORK_SERVICE_CONFIRMED_EVENT_BLOCK_REPORT:
			break;

		case NETWORK_SERVICE_UNCONFIRMED_EVENT_BLOCK_REPORT:
			handleUnconfirmedEventBlock(arc);
			break;
	}

}

/* handle unconfirmed Event Blocks, E.g. used with Telemetry. Needs to be implemented */
void handleUnconfirmedEventBlock(Arc_Link_typ *arc){
	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;
	ServiceID_typ *serviceID;

	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;
	serviceID = &arc->msgIN.request.ServiceID;
	robot = &arc->robot;
	DBG("Inside handleUnconfirmedEventBlock - serviceID->AttributeNumber: %d \r\n" , serviceID->AttributeNumber);		
	switch (serviceID->AttributeNumber) {
		case NETWORK_HIGH_SPEED_BLOCK_METER_EVENT_ACTION:
			// Process Telemetey Data
			processTelemetryEvent(arc); 
			break;
		case NETWORK_ENERGY_FB_EVENT_ACTION:
			processEnergyFBEvent(arc);
			break;
		case NETWORK_BLOCK_METER_EVENT_ACTION:
			processBlockMeterEvent(arc);
			break;
		case NETWORK_WELD_SCORE_EVENT_ACTION:
			processWeldScoreEvent(arc);
			break;
			
	}
}

/* handle unconfirmed Event, E.g. arc Status change */
void handleUnconfirmedEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;
	ServiceID_typ *serviceID;

	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;
	serviceID = &arc->msgIN.request.ServiceID;
	robot = &arc->robot;
	DBG_OB("Inside handleUnconfirmedEvent - serviceID->AttributeNumber: %d \r\n" , serviceID->AttributeNumber);		
	switch (serviceID->AttributeNumber) {
		case ARC_STATUS_CHANGE_EVENT:
			// Process unconfirmed event for ARC status
			processArcStatusChangeEvent(arc); 
			break;
		case NETWORK_ATTRIBUTE_PROPERTY_EVENT:
			// process ready, faults status
			processStatusEvent(arc);
			break;
	}
}

/* Process Telemetry Data */
void processTelemetryEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	DBG_OB("Received Telemetry Event\r\n" );		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG("Error: Receiving Telemetry Event with Request size\r\n" );	
		return;
	}

	// Per specification Request data size is 4 
	UINT Current = *(data + 1) + (*(data + 0 ) << 8); // Raw Values
	UINT Voltage = *(data + 3) + (*(data + 2) << 8); // Raw Values
		
	
	DBG_OB("Data:[%s,%d], [[%s,%d]", "Current", Current, "Voltage", Voltage);		
	
	robot->wc.data.telemetry.current = Current;
	robot->wc.data.telemetry.voltage = Voltage;
	robot->wc.data.telemetry.fCurrent = (float)(Current / 10.0);
	robot->wc.data.telemetry.fVoltage = (float)(Voltage / 10.0);
		
}


/* Process Energy FB Data */
void processEnergyFBEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;
	//
	/*	DBG_OB("Received Energy FB Event\r\n" )	*/	
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving Energy FB Event with Request size\r\n" )	
		return;
	}

	// Per specification Request data size is 4 
	UINT Energy = *(data + 1) + (*(data + 0 ) << 8); // Raw Values
	UINT ArcTime = *(data + 3) + (*(data + 2) << 8); // Raw Values
		
	
	//	DBG_OB("Data:[%s,%d], [[%s,%d]\r\n", "Energy", Energy, "Arc Time", ArcTime)		
		
}

/* Process Block meter Data */
void processBlockMeterEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	/*	DBG_OB("Received Block meter Event\r\n" )*/		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving block meter Event with Request size\r\n" )	
		return;
	}

	// Per specification Request data size is 8 
	UINT Status = *(data + 1) + (*(data + 0 ) << 8); // Raw Values
	UINT Average_I = *(data + 3) + (*(data + 2) << 8); // Raw Values
	UINT Average_V = *(data + 5) + (*(data + 4 ) << 8); // Raw Values
	UINT GSF = *(data + 7) + (*(data + 6) << 8); // Raw Values	
	//	
	//	DBG_OB("Data:[%s,%d], [[%s,%d]\r\n", "Status", Status, "Average_I", Average_I)	
	//	DBG_OB("Data:[%s,%d], [[%s,%d]\r\n", "Average_V", Average_V, "GSF", GSF)	
		
}

/* Process Weld Score Data */
void processWeldScoreEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	/*	DBG_OB("Received Weld Score Event\r\n" )*/		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving Weld Score Event with Request size\r\n" )	
		return;
	}

	// Per specification Request data size is 2 
	UINT Weld_Score = *(data + 1) + (*(data + 0 ) << 8); // Raw Values
	
	//	DBG_OB("Data:[%s,%d]", "Weld Score \r\n", Weld_Score)	
	
		
}


/* Process Telemetry Data for Arc Status Change Event */
void processArcStatusChangeEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	DBG_OB("Received Arc Status Change Event\r\n" );		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving Arc Status Change Event with Request size\r\n" );	
		return;
	}   

//	if (*(data + 1) & 0x01)
//		DBG_OB("Arc Voltage Present\r\n")   // add extra variables
//	else 
//		DBG_OB("Arc Voltage Not Present\r\n")
//	
//	
//	if (*(data + 1) >> 1 & 0x01)
//		DBG_OB("Arc Current Present\r\n")
//	else 
//		DBG_OB("Arc Current Not Present\r\n")
	
	
	
	if ((*(data + 1) & 0x01) && (*(data + 1) >> 1 & 0x01))
	{
		DBG_OB("Arc Active"); robot->Cntrl.Fbk.Status |= Arc_Active;
		log_event(&robot->eventlog , "Arc Active");
	}
	else 
	{
		robot->Cntrl.Fbk.Status &= ~Arc_Active;
	}
	
	if (!(*(data + 1) & 0x01) && (*(data + 1) >> 1 & 0x01))
	{
		DBG_OB("Arc Short"); robot->Cntrl.Fbk.Status |= Arc_Short;
		log_event(&robot->eventlog , "Arc Short");
	}
	else 
	{
		robot->Cntrl.Fbk.Status &= ~Arc_Short;
	}
		
	
	if ((*(data + 1) & 0x01) && !(*(data + 1) >> 1 & 0x01))
	{
		DBG_OB("Arc OCV"); robot->Cntrl.Fbk.Status |= Arc_OCV;
		log_event(&robot->eventlog , "Arc OCV");
	}
	else 
	{
		robot->Cntrl.Fbk.Status &= ~Arc_OCV;
	}
		
	
	if (!(*(data + 1) & 0x01) && !(*(data + 1) >> 1 & 0x01))
	{
		DBG_OB("Arc Off"); robot->Cntrl.Fbk.Status |= Arc_OFF;
		log_event(&robot->eventlog , "Arc Off");
	}
	else 
	{
		robot->Cntrl.Fbk.Status &= ~Arc_OFF;
	}
	
	
	
	//		
}

/* Process Telemetry Data for Ready Status Event */
void processStatusEvent(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;
	UINT Attribute;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;
	serviceID = &arc->msgIN.request.ServiceID;

	DBG_OB("Received Status Change\r\n" );		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving Ready Status Event with Request size\r\n" )	
		return;
	}   
    
	Attribute = *(data + 1) + (*(data + 0 ) << 8);   //Attribute service ID
	
	
	switch (Attribute){
		case READY_STATUS_EVENT:

			robot->wc.WC_Ready_Status = *(data+4) + (*(data + 3 ) << 8);
	
			if (*(data+4) == 0xff)   //	Attribute value
			{
				DBG_OB("WC is Ready\r\n"); robot->isReady = 1;
			}
			else {
				DBG_OB("WC is Not Ready\r\n"); robot->isReady = 0;
			}
			// TODO  details why is not ready 
			break;
		case FAULT_STATUS_EVENT:

			robot->wc.WC_Ready_Status = *(data+4) + (*(data + 3 ) << 8);
			
			if (*(data+4) == 0)   //	Attribute value
			{
				DBG_OB("WC is Not Faulted\r\n");robot->Cntrl.Fbk.Error &= ~PS_System_Fault;
			}
			else 
			{
				DBG_OB("WC is Faulted\r\n");robot->Cntrl.Fbk.Error |= PS_System_Fault;
				// TODO  build outgoing message (request ) to read attribute 30731
				readAttribute30731(arc);
				log_event(&robot->eventlog , "Power Supply is Faulted ");
			}
		default:
			break;
	}
}


/**
 * Handle Network Action
 * 
 * @param h
 * @param r
 */
void handleNetworkAction( Arc_Link_typ *arc) {

	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;
	ServiceID_typ *serviceID;
	
	if (!arc) return; // Prevent null pointer reference
	
	//	robot.ArcComTON.PT = ARCCOM_TIMEOUT;

	r = &arc->msgIN.request;
	serviceID = &arc->msgIN.request.ServiceID;
	robot = &arc->robot;
	DBG("Inside handleNetworkAction - serviceID->AttributeNumber: %d \r\n" , serviceID->AttributeNumber);		
	switch (serviceID->AttributeNumber) {
		case NETWORK_RECOGNITION_ACTION:
			processRecognitionActionArcLinkRequest(arc); 
			break;
	
		case NETWORK_ASSIGN_NETWORK_ADDRESS_ACTION:
			//if (robot->Cntrl.Cmd.RST){
				processAssignNetworkAddressAction(arc);
				robot->Cntrl.Cmd.RST = 0;
				//robot->Cntrl.Fbk.Error = 0;
				robot->Cntrl.Fbk.Status = 0;
				robot->miscSetup.state = 0; //reset misc state machine state
				log_event(&robot->eventlog , "Received RESET signal");
				robot->Comm.StopRespondHeartBeat = 0;
			//}// [OB]
			break;

		case NETWORK_ADD_TO_GROUP_ADDRESS_ACTION:
			processAddToGroupAddressAction(arc); // [OB]
			break;

		case NETWORK_GROUP_DATA_ACTION:
			processGroupDataAction(arc); // [OB]
			break;

		case NETWORK_NOP_ACTION:
			if (!robot->Comm.StopRespondHeartBeat)
				processNOP_ACTION(arc); // [OB to be Implemented]
			break;

		case NETWORK_GET_CONFIGURATION_OBJECT_CONTROLS_ACTION:
			processGetConfigurationObjectControlsAction(arc); // [OB]
			break;

		case NETWORK_ATTRIBUTE_SEARCH_ACTION:
			processAttributeSearchAction(arc); // [OB]
			break;

		case NETWORK_CREATE_EVENT_REPORT_ACTION:
			processCreateEventReportAction(arc); // [OB]
			break;

			
	}

}


/*
	Process Recognition Action Arc Link Request
*/
void processRecognitionActionArcLinkRequest( Arc_Link_typ *arc) {
	// Once RecognitionAction Request is received and decoded
	// Robot must reply to the server to get mapped
	if (!arc) return; // Prevent null pointer reference
	
	processRecognitionActionArcLinkResponse(arc);	
}

/*
	Create Recognition Action Arc Link Response
	[Pending Implementation ]
*/
void processRecognitionActionArcLinkResponse( Arc_Link_typ *arc) {
	Arc_Link_Response_typ 	*r; // temp reference to response
	Arc_Link_Header_typ 	*h; 	// temp reference to arcLink response Header 
	USINT data[5]; 				// 5 bytes per Manual Specification for Data size response
	Arc_Link_Internal_typ *internal;
	
	Robot_typ 				*robot;

	USINT 					*msgout; // Reference to Outgoing ArcLink Message Header and Response
	
	if (!arc) return; // Prevent null pointer reference
	
	robot =  &arc->robot;
			
	if (robot->isMapping | robot->isMapped) {
		// This is necessary for Mapping
		return;
	}

	r = &arc->msgOUT.response;
	h = &arc->msgOUT.header;
	//msgout = &arc->internal.pOutMsg;
	internal = &arc->internal;
	
		
	/*
	 * Header size is 10 bytes array index 0 - 9, therefore incoming data
	 * i.e ArcLink Response comes in index 10
	 */
	/* The ArcLink Network Address of the Object that responded */
	// In this case by default for initial config use the BUS_MASTER_ADDRESS
	r->ObjectAddress		= ARC_LINK_OBJECT_BUS_MASTER_ADDRESS;
	r->ObjectCount 			= 1;
	r->Status				= 0; // Means no Errors
	r->ResponseSize			= 5; // 5 bytes per Manual Specification
	
	UINT 	vendorID 		= (UINT) robot->arclink.network.VendorID; // Object Class, Address Robot
	UINT 	serialNumber	= (UINT) robot->arclink.network.SiliconSerialNumber;
	USINT 	numberObjects 	= 1;

	// Data must be sent in Big Endian format
	data[0] = (USINT) (vendorID >> 8);
	data[1] = (USINT) (vendorID >> 0);
	data[2] = (USINT) (serialNumber >> 8);
	data[3] = (USINT) (serialNumber >> 0);
	data[4] = (USINT) (numberObjects >> 0);

	// Copy data here. Up to maximun size
	//memcpy(&r->ResponseData , data , 5);
	
	// Create a Response header for reply
	h->Flags 		= 0x03; // Arclink Response 0b0011

	// It has to be the same as request header 
	h->Sequence 	= arc->msgIN.header.Sequence; // The sequence must match Request header
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_RESPONSE_SIZE + r->ResponseSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	internal->OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int ResponseSize =  r->ResponseSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);		
	swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	
	memcpy(internal->pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_RESPONSE_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE, data , ResponseSize);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	DBG("InsideprocessRecognitionActionArcLinkResponse \r\n" );		
	// Arclink Response is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 
}

void writeMessageToOutgoingRingBuffer( Arc_Link_typ *arc) {
	if (!arc) return; // Prevent null pointer reference

	
	if(arc->bufOUT){		
		if(!RingBuffer_isFull(arc->bufOUT)){			
			RingBuffer_writeRecord(arc->bufOUT, arc->internal.pOutMsg, arc->internal.OutMsgLen);		
		}
	}
}


/*
	Process Assign Network Action
	[Pending Implementation ]
*/
void processAssignNetworkAddressAction( Arc_Link_typ *arc) {
	Arc_Link_Request_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	ServiceID_typ 			*serviceID;	
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.request;
	robot = &arc->robot;

	DBG("Recevied Assign Network Address Action \r\n" );		
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data go back here and test
	}else{
		DBG("Error: Assign Network Address Action, Error with Request size\r\n" );	
		return;
	}

	// Per specification Request data size is 5 
	UINT vendorID = *(data + 1) + (*(data + 0 ) << 8);
	UINT siliconSerialID = *(data + 3) + (*(data + 2) << 8);
	USINT BaseAddress = *(data + 4);
	

	// Assign new BaseAddress to this(Robot) ArcLink Object
	if (robot->arclink.network.SiliconSerialNumber == siliconSerialID) {
		robot->arclink.network.ModuleBaseAddress = BaseAddress;
	}

	//Log.d(TAG, String.format("Data:[%1$s,%4$#x], [[%2$s,%5$#x], [%3$s,%6$#x]]", "vendorID", "siliconSerialID",
	//		"BaseAddress", vendorID, siliconSerialID, BaseAddress));

	DBG("Data:[%s,%x], [[%s,%x], [%s,%x]]", "vendorID", vendorID, "siliconSerialID", siliconSerialID,"BaseAddress", BaseAddress);		
	DBG("Object Address:%d, assigned to this PLC(Robot)", BaseAddress);

	sendSuccesfullWriteAttributeConfirmation(arc); // implement this
	robot->isMapping = 1; // Indicate that mapping is in progress
	robot->Cntrl.Fbk.Status |= ArcLink_Mapping;
}

/* 	Send succesful Write Attribute Confirmation
	This just tells the Bus Master that the message was received and processed succesfully.
*/
void sendSuccesfullWriteAttributeConfirmation( Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to response
	Arc_Link_Header_typ 	*h; 	// temp reference to arcLink response Header 
	USINT data[5]; 				// 5 bytes per Manual Specification for Data size response
	Arc_Link_Internal_typ *internal;
	
	Robot_typ 				*robot;

	USINT 					*msgout; // Reference to Outgoing ArcLink Message Header and Response
	
	if (!arc) return; // Prevent null pointer reference
	
	robot =  &arc->robot;
	r = &arc->msgOUT.response;
	h = &arc->msgOUT.header;
	//msgout = &arc->internal.pOutMsg;
	internal = &arc->internal;
		
	/*
	 * Header size is 10 bytes array index 0 - 9, therefore incoming data
	 * i.e ArcLink Response comes in index 10
	 */
	/* The ArcLink Network Address of the Object that responded */
	// In this case by default for initial config use the BUS_MASTER_ADDRESS
	/* The ArcLink Network Address of the Object that responded */
	r->ObjectAddress		= robot->arclink.network.ModuleBaseAddress;
	r->ObjectCount 			= 1;
	r->Status				= 0; // Means no Errors
	r->ResponseSize			= 0; // 5 bytes per Manual Specification
	
	// Data must be sent in Big Endian format

	// Create a Response header for reply
	h->Flags 		= 0x03; // Arclink Response 0b0011

	// It has to be the same as request header 
	h->Sequence 	= arc->msgIN.header.Sequence; // The sequence must match Request header
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_RESPONSE_SIZE + r->ResponseSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	internal->OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int ResponseSize =  r->ResponseSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	
	memcpy(internal->pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_RESPONSE_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE, data , ResponseSize);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	DBG("Inside sendSuccesfullWriteAttributeConfirmation \r\n" );		
	// Arclink Response is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 
}

/* Handle Network Block Action */
void handleNetworkBlock(Arc_Link_typ *arc){
	Arc_Link_Request_typ *r; // temp reference to request
	Robot_typ *robot;

	if (!arc) return; // Prevent null pointer reference

	r = &arc->msgIN.request;
	robot = &arc->robot;

	ServiceID_typ serviceID = r->ServiceID;
	
	switch (serviceID.AttributeNumber) {

		case CONFIGURATION_DATA_BLOCK:
			processReadNetworkConfigurationDataBlock(arc);
			break;
	}
}

/* Network block 0*/
void processReadNetworkConfigurationDataBlock(Arc_Link_typ *arc) {
	if(!arc) return;
	processReadNetworkConfigurationDataBlockResponse(arc);
}

/* Read Network Configuration Data Response acknowledge */
void processReadNetworkConfigurationDataBlockResponse(Arc_Link_typ *arc){
	Arc_Link_Response_typ	*r; // temp reference to response
	Arc_Link_Header_typ 	*h; 	// temp reference to arcLink response Header 
	USINT data[5];				// 5 bytes per Manual Specification for Data size response
	Arc_Link_Internal_typ *internal;
	
	Robot_typ				*robot;

	USINT					*msgout; // Reference to Outgoing ArcLink Message Header and Response
	
	if (!arc) return; // Prevent null pointer reference
	
	robot =  &arc->robot;

	r = &arc->msgOUT.response;
	h = &arc->msgOUT.header;
	//msgout = &arc->internal.pOutMsg;
	internal = &arc->internal;
	
		
	/*
	 * Header size is 10 bytes array index 0 - 9, therefore incoming data
	 * i.e ArcLink Response comes in index 10
	 */
	/* The ArcLink Network Address of the Object that responded */
	r->ObjectAddress		= robot->arclink.network.ModuleBaseAddress;
	r->ObjectCount			= 1;
	r->Status				= 0; // Means no Errors
	r->ResponseSize 		= 5; // 5 bytes per Manual Specification
	
	UINT	vendorID		= (UINT) robot->arclink.network.VendorID; // Object Class, Address Robot
	UINT	serialNumber	= (UINT) robot->arclink.network.SiliconSerialNumber;
	USINT	numberObjects	= 1;

	/**
	 * <pre>
	 Attribute Number 	(name)				Byte Offset		Bit Offset
			4 			(Class ID)				0				0
			10 			(Group Assignments)		2				0
			11 			(Instance Assignments)	3				0
			7 			(Class Type)			4				0
	 * 
	 * </pre>
	 */
	// Data must be sent in Big Endian format
	data[0] = (USINT) (robot->arclink.network.ClassID >> 8);
	data[1] = (USINT) (robot->arclink.network.ClassID >> 0);
	data[2] = (USINT) (robot->arclink.network.GroupAssignments >> 0);
	data[3] = (USINT) (robot->arclink.network.InstanceAssignments >> 0);
	data[4] = (USINT) (robot->arclink.network.ClassType >> 0);

	// Copy data here. Up to maximun size
	//memcpy(&r->ResponseData , data , 5);
	
	// Create a Response header for reply
	h->Flags		= 0x03; // Arclink Response 0b0011

	// It has to be the same as request header 
	h->Sequence 	= arc->msgIN.header.Sequence; // The sequence must match Request header
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_RESPONSE_SIZE + r->ResponseSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	internal->OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int ResponseSize =	r->ResponseSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);		
	swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's done above
	
	memcpy(internal->pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_RESPONSE_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE, data , ResponseSize);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	DBG("processReadNetworkConfigurationDataBlockResponse \r\n" ); 	
	// Arclink Response is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 
}

/**
 * Attribute Class Property Indicates the context of an attribute’s effect
 * on object behavior. The value of attribute’s Class property may not
 * change at run-time. Some Attribute Class properties imply that the
 * attribute value may not be written (Read Only). If a write to a Read-Only
 * attribute is attempted, an error will be returned.
 * 
 * @param h
 * @param r
 */
void handleReadAttributeClassProperty(Arc_Link_typ *arc) {
	Arc_Link_Request_typ *r;
	ServiceID_typ *serviceID;

	if(!arc) return;

	DBG("Inside handleReadAttributeClassProperty \r\n");
	r = &arc->msgIN.request;
	serviceID = &r->ServiceID;

	switch (serviceID->PropertyNumber) {
		case PROPERTY_ID_VALUE:
			handleReadAttributeValue(arc);
			break;
		case PROPERTY_ID_CLASS:
			break;
		case PROPERTY_ID_SAVED_IN_E2:
			break;
		case PROPERTY_ID_ATTRIBUTE_DATA_TYPE:
			break;
		case PROPERTY_ID_UNIT_CODE:
			break;
		case PROPERTY_ID_HIGH_LIMIT:
			break;
		case PROPERTY_ID_LOW_LIMIT:
			break;
		case PROPERTY_ID_DECIMAL_PLACES:
			break;
		case PROPERTY_ID_NAME:
			break;
	}

}

/* Handle Read Attribute Value */
void handleReadAttributeValue(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*req; // temp reference to request
	Arc_Link_Response_typ	*r; // temp reference to response
	Arc_Link_Header_typ 	*h; 	// temp reference to arcLink response Header 
	USINT data[5];				// 5 bytes per Manual Specification for Data size response
	Arc_Link_Internal_typ *internal;
	
	Robot_typ				*robot;

	USINT					*msgout; // Reference to Outgoing ArcLink Message Header and Response

	ServiceID_typ *serviceID;

	USINT *ResponseData; // temp pointer to Data
	
	if(!arc) return;

	req = &arc->msgIN.request;
	serviceID = &req->ServiceID; // ServiceID from Request.
	internal = &arc->internal;
	r = &arc->msgOUT.response;
	h = &arc->msgOUT.header;
	
	r->ObjectCount 	= 1;
	r->Status 		= 0;
	/* The ArcLink Network Address of the Object that is responding */
	r->ObjectAddress = req->DestinationAddress; // Should be the same as local base address 
	r->ResponseSize = req->MaxResponseSize; 	// per specification

	robot = &arc->robot;
	
	ResponseData = internal->pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE;
	DBG("Inside handleReadAttributeValue\r\n" ); 	
	switch (serviceID->PropertyNumber) {
		case CLASS_NETWORK_CONFIGURATION:
			// Get Robot Network Configuration Attribute
			getNetworkConfigurationAttribute(arc, ResponseData); // From Robot.c
			break;

		case CLASS_NETWORK_STATUS:
			break;

		case CLASS_COMMAND:
			break;

		case CLASS_REFERENCE:
			break;

		case CLASS_CONFIGURATION:
			break;

		case CLASS_SIGNAL:
			break;

		case CLASS_STATUS:
			break;

		case CLASS_SEQUENCE_CONTROLLED_COMMAND:
			break;

		case CLASS_SEQUENCE_CONTROLLED_REFERENCE:
			break;

		case CLASS_SEQUENCE_CONTROLLED_CONFIGURATION:
			break;

		case CLASS_COMMAND_WITH_STATE_MEMORY:
			break;

		case CLASS_REFERENCE_WITH_STATE_MEMORY:
			break;

		case CLASS_DEBUG:
			break;

		case CLASS_SETUP:
			break;

		case CLASS_VERSION:
			break;
	}

	// Data was already copied inside getNetworkConfigurationAttribute(arc, ResponseData); 
	
	// Create a Response header for reply
	h->Flags		= 0x03; // Arclink Response 0b0011

	// It has to be the same as request header 
	h->Sequence 	= arc->msgIN.header.Sequence; // The sequence must match Request header
	h->LastError	= 0;
	// r->ResponseSize Changed insied getNetworkConfigurationAttribute(arc, ResponseData);
	h->DataLength	= ARC_LINK_RESPONSE_SIZE + r->ResponseSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	internal->OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int ResponseSize =	r->ResponseSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);		
	swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's done above
	
	memcpy(internal->pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_RESPONSE_SIZE);
	//memcpy(internal->pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE, data , ResponseSize);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkResponseBytes(r);	// Not includes data yet. Since it's down above
	DBG("Processed getNetworkConfigurationAttribute(arc, ResponseData); \r\n" ); 	
	// Arclink Response is ready to be sent
	writeMessageToOutgoingRingBuffer(arc);
}


void handleWriteAttributeClassProperty(Arc_Link_typ *arc){
	Arc_Link_Request_typ *r;
	ServiceID_typ *serviceID;

	if(!arc) return;

	DBG("Inside handleWriteAttributeClassProperty \r\n");
	r = &arc->msgIN.request;
	serviceID = &r->ServiceID;

	switch (serviceID->PropertyNumber) {
		case PROPERTY_ID_VALUE:
			handleWriteAttributeValue(arc);
			break;
		case PROPERTY_ID_CLASS:
			break;
		case PROPERTY_ID_SAVED_IN_E2:
			break;
		case PROPERTY_ID_ATTRIBUTE_DATA_TYPE:
			break;
		case PROPERTY_ID_UNIT_CODE:
			break;
		case PROPERTY_ID_HIGH_LIMIT:
			break;
		case PROPERTY_ID_LOW_LIMIT:
			break;
		case PROPERTY_ID_DECIMAL_PLACES:
			break;
		case PROPERTY_ID_NAME:
			break;
	}




}

/* Handle Write Attribute value Service */
void handleWriteAttributeValue(Arc_Link_typ *arc){
	Arc_Link_Request_typ 	*req; // temp reference to request
	ServiceID_typ *serviceID;

	if(!arc) return;

	req = &arc->msgIN.request;
	serviceID = &req->ServiceID; // ServiceID from Request.

	DBG("Inside handleWriteAttributeValue\r\n" ); 	
	
	switch (serviceID->PropertyNumber) {
		case CLASS_NETWORK_CONFIGURATION:
			// write network parameters in Robot
			if (writeNetworkConfigurationAttribute(arc))  // From Robot.c
			{
				sendSuccesfullWriteAttributeConfirmation(arc);	
			}else{
				DBG("Something went wrong while writing Attribute Value\r\n" ); 	
			}


			break;

		//case CLASS_NETWORK_STATUS:
		//	break;

		case CLASS_COMMAND:
			break;

		case CLASS_REFERENCE:
			break;

		case CLASS_CONFIGURATION:
			break;

		//case CLASS_SIGNAL:
		//	break;

		//case CLASS_STATUS:
		//	break;

		case CLASS_SEQUENCE_CONTROLLED_COMMAND:
			break;

		case CLASS_SEQUENCE_CONTROLLED_REFERENCE:
			break;

		case CLASS_SEQUENCE_CONTROLLED_CONFIGURATION:
			break;

		case CLASS_COMMAND_WITH_STATE_MEMORY:
			break;

		case CLASS_REFERENCE_WITH_STATE_MEMORY:
			break;

		case CLASS_DEBUG:
			break;

		case CLASS_SETUP:
			break;

		//case CLASS_VERSION:
		//	break;
	}
}

/* Action 5 process Add To Group Address Action */
void processAddToGroupAddressAction(Arc_Link_typ *arc){
	if(!arc) return;

	DBG("Received Add To Group Address Action\r\n" ); 
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

	// Copy Data if available to the Incoming data pointer
	if(req->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data 
	}else{
		DBG("Error: processAddToGroupAddressAction, Error with Request size\r\n" );	
		return;
	}

	USINT GroupAddress = *(data + 0);
	
	// Assign new GroupAddres to this(Robot) ArcLink Object
	arclink->network.GroupAssignments = GroupAddress;

	sendSuccesfullWriteAttributeConfirmation(arc);
} 

/* Action 2, Process Group Data Action */
void processGroupDataAction(Arc_Link_typ *arc){
	if(!arc) return;

	DBG("Received processGroupDataAction\r\n" ); 
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

	// Copy Data if available to the Incoming data pointer
	if(req->RequestSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE); // Pointer to data 
	}else{
		DBG("Error: processAddToGroupAddressAction, Error with Request size\r\n" );	
		return;
	}

	/* This only provides information of all arclink objects connected to Bus Master */
	/* Do something here only if neccessary. For now just reply back to it */
	sendSuccesfullWriteAttributeConfirmation(arc);
}

/* Action 19, Heart bit reply neccesary for Arclink Communication */
void processNOP_ACTION(Arc_Link_typ *arc) {
	if(!arc) return;
	sendSuccesfullWriteAttributeConfirmation(arc);
}

/* Action 32 */
void processGetConfigurationObjectControlsAction(Arc_Link_typ *arc){
	if(!arc) return;
	sendSuccesfullWriteAttributeConfirmation(arc);
} 

/* Action 11 */
void processCreateEventReportAction(Arc_Link_typ *arc){
	if(!arc) return;
	sendSuccesfullWriteAttributeConfirmation(arc);
}

/* Action 8 */
void processAttributeSearchAction(Arc_Link_typ *arc){
	if(!arc) return;
	sendSuccesfullWriteAttributeConfirmation(arc);
}

//////////////////////////////////////////////////////////////////////
// Arclink Miscellanious Setup functions
//////////////////////////////////////////////////////////////////////
/* Perform Misc Setup required for Telemetry */
void performRobotMiscSetup(Arc_Link_typ *arc){
	Robot_typ *robot;
	
	if(!arc) return;
	
	robot = &arc->robot;

	if (robot->isMapped && !robot->isMiscSetupOneShot) {
		/*
		 * Perform Misc Setup for Telemetry
		 */
		robot->isMiscSetupOneShot = 1;
		robot->miscSetup.running = 1; // Enable state machine
//		robot->Comm.Oneshot = 1; //for heartbeat watchdog
	}

	// Perform miscSetup when active
	miscSetup(arc);
	
}

/* Miscellanious Setup */
void miscSetup(Arc_Link_typ *arc){
	Robot_typ *robot;
	Arc_Link_Misc_Setup_typ *s;
	PendingResponse_typ *pr;
	
	if(!arc) return;

	robot = &arc->robot;
	s = &robot->miscSetup;
	pr = &arc->PendingResponse;
	

	// If MiscSetup is Active
	if(s->running){

		switch(s->state)
		{
			case 0:
				s->state++;
				break;
			
			/*
			 * Create Ready Status Change Event for WC object Action 11 for
			 * attribute 13
			 **/
			case 1:
				s->er.AttributeIDNumber = 13;
				s->er.SequenceState = 0;
				s->er.PropertyIDNumber = PROPERTY_ID_VALUE;
				// Bit 7: Confirmed
				// Bit 6 - 5: 3 Trigger Type Change in value or State
				// Bit 4 -0: Descriptor not uaws
				s->er.TriggerOptions = 0xE0; //0b11100000; change to 0x00 unconfirmed event and periodically
				
				/**
				* 
				* <pre>
				If periodic option, then = period
				If compare value, then = comparison value
				If range compare, then = low limit, high limit
				If change of value or state, then not used
				* </pre>
				*/
				// er.Data = new byte[2]; // Not used
	
				/* Create Fault Status Change Event for WC object Action 11 for
				* attribute 14
				//			 * 
				//			 **/  	//createEventReport(arc);
				//				    s->state++;
				
				
				if (!pr->CreateReadyReport){  //  start writing  
					createEventReport(arc);
					pr->CreateReadyReport = 1;
				}
				
				if (!pr->CreateReadyReportSequence){  //sucessfully got response move to next step
					s->state++;
					pr->CreateReadyReport = 0;
				}
				break;
			case 2:
				s->er.AttributeIDNumber = 14;
				s->er.SequenceState = 0;
				s->er.PropertyIDNumber = PROPERTY_ID_VALUE;
				// Bit 7: Confirmed
				// Bit 6 - 5: 3 Trigger Type Change in value or State
				// Bit 4 -0: Descriptor not uaws
				
				s->er.TriggerOptions = 0xE0; //0b11100000; change to 0x00 unconfirmed event and periodically
				
				/**
				* 
				* <pre>
				If periodic option, then = period
				If compare value, then = comparison value
				If range compare, then = low limit, high limit
				If change of value or state, then not used
				* </pre>
				*/
				// er.Data = new byte[2]; // Not used
				//            	createEventReport(arc);
				//
				//		    	s->state = 4;
				
				
				if (!pr->CreateFaultReport){  //  start writing  
					createEventReport(arc);
					pr->CreateFaultReport = 1;
				}
				
				if (!pr->CreateFaultReportSequence){  //sucessfully got response move to next step
					s->state = 4;
					pr->CreateFaultReport = 0;
				}
							
				break;	

			/**
			 * Close Electrode Remote Voltage Lead for SL object Write attribute
			 * 512 to 1, Not used
			 */
			case 3:
				writeTelemetryAttribute512(arc);
				s->state++;
				break;

			/**
			 * Disable auto-stick check in WC object Write attribute 30770 to 1
			 * 
			 */
			case 4:
				//				writeTelemetryAttribute30770(arc);
				//				s->state++;
				if (!pr->WriteAttribute30770){  //  start writing  
					writeTelemetryAttribute30770(arc);
					pr->WriteAttribute30770 = 1;
				}
				
				if (!pr->WriteAttribute30770Sequence){  //sucessfully got response move to next step
					s->state++;
					pr->WriteAttribute30770 = 0;
				}
				break;

			/*
			 * Configure current and voltage feedback in WC object for 250hz
			 * (4ms) and 75ms filter Write attribute 30781 to 4 Write attribute
			 * 30758 to 75
			 * 
			 */
			case 5:
				//				writeTelemetryAttribute30781(arc);
				//				s->state++;
				if (!pr->WriteAttribute30781){  //  start writing  
					writeTelemetryAttribute30781(arc);
					pr->WriteAttribute30781 = 1;
				}
				
				if (!pr->WriteAttribute30781Sequence){  //sucessfully got response move to next step
					s->state++;
					pr->WriteAttribute30781 = 0;
				}
				break;
			
			case 6:
				//				writeTelemetryAttribute30758(arc);
				//				s->state++;
				if (!pr->WriteAttribute30758){  //  start writing  
					writeTelemetryAttribute30758(arc);
					pr->WriteAttribute30758 = 1;
				}
				
				if (!pr->WriteAttribute30758Sequence){  //sucessfully got response move to next step
					s->state++;
					pr->WriteAttribute30758 = 0;
				}
				break;

			case 7:
				//				writeTelemetryBusMasterAttribute30(arc);
				//				s->state++;
				if (!pr->WriteAttribute30){  //  start writing  
					writeTelemetryBusMasterAttribute30(arc);
					pr->WriteAttribute30 = 1;
				}
				
				if (!pr->WriteAttribute30Sequence){  //sucessfully got response move to next step					
					pr->WriteAttribute30 = 0;

					action_ConfigDataSearch_Cooler(arc);
					DBG("commanded Get Cooler Adr! \r\n" );

					s->state++;
				}
				break;


			case 8:


				if (!pr->sqConfigDataSearchAction){  //sucessfully got response move to next step					
					pr->sqConfigDataSearchAction = 0;

					action_ConfigDataSearch_GasSol(arc);
					DBG("commanded Get Gas Sol  Adr! \r\n" );
					
					s->state++;

				}

			break;



			

			// End of miscellanious setup	
			case 9:

				if(!pr->sqConfigDataSearchAction_GasSol){
					pr->sqConfigDataSearchAction_GasSol = 0;
					s->running = 0;
					log_event(&robot->eventlog , "Misc Setup Succeed");

					s->state++;
				}
				
			break;

			case 10:


			break;
			
		}


	

	}
}

/* Create Event Report */
void createEventReport(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	Event_Report_Request_typ *er;
	PendingResponse_typ *pr; 

	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	pr = &arc->PendingResponse;
	
	if(!arc) return;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;
	er = &robot->miscSetup.er;
	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = NETWORK_CREATE_EVENT_REPORT_ACTION;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 6; // Size of Arc_Link_Event_Report_Request_typ 
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	// Not includes data yet. Since it's down above
	swapArcLinkEventReportRequestBytes(er);
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, er , RequestSize);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	// Not includes data yet. Since it's down above
	//swapArcLinkEventReportRequestBytes(er);
	
	DBG("Inside createEventReport \r\n" );		
	// Arclink Response is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 
}



/* write Telemetry Attribute 512 */
void writeTelemetryAttribute512(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	
	if(!arc) return;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_REMOTE_VOLTAGE_LEAD;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = SENSE_LEAD_ARCLINK_NETWORK_ADDRESS;
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data = 1; // ON
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 
}




/* writeTelemetryAttribute30770 */
void writeTelemetryAttribute30770(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_DISABLE_AUTO_STICK_CHECK;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data = 1; // ON
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->WriteAttribute30770Sequence = 	h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* writeTelemetryAttribute30781 */
// ATTRIBUTE_HS_METER_FREQUENCY 
void writeTelemetryAttribute30781(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	
	PendingResponse_typ *pr; 
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_HS_METER_FREQUENCY;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data = 12; // 4 ms
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->WriteAttribute30781Sequence = 	h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* writeTelemetryAttribute30758 */
void writeTelemetryAttribute30758(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	
	PendingResponse_typ *pr; 
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_HS_FILTER;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 2; // Size of Arc_Link_Event_Report_Request_typ 
	UINT filter = 75; 
	USINT data[2]; // 4 ms
	data[0] = (USINT)(filter >> 8);
	data[1] = (USINT)(filter >> 0);
	
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, data , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->WriteAttribute30758Sequence = 	h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* writeTelemetry BusMasterAttribute30 */
void writeTelemetryBusMasterAttribute30(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr;
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_LINK_TIMEOUT_PERIOD;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = BUS_MASTER_ARCLINK_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 2; // Size of Arc_Link_Event_Report_Request_typ 
	UINT time = 5; 
	USINT data[2]; // 
	data[0] = (USINT)(time >> 8);
	data[1] = (USINT)(time >> 0);
	
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, data , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->WriteAttribute30Sequence = h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

UINT rand()
{
	// If you want random number used below commented code
	/*
	UINT lfsr = 0xACE1;
	UINT bit;

	bit	= ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
	return lfsr =  (lfsr >> 1) | (bit << 15);
	*/

	// In this case we want an incrental Sequence counter

	SEQUENCE_NUMBER++;

	if(SEQUENCE_NUMBER == 0)
		SEQUENCE_NUMBER = 1;
	
	return SEQUENCE_NUMBER;
	
}

//////////////////////////////////////////////////////////////////////
// Robot specific functions Commands
//void writeOutputGasEnableCommand(Arc_Link_typ *arc);
//void writeOutputGasOnCommand(Arc_Link_typ *arc);
//void writeOutputGasOffCommand(Arc_Link_typ *arc);
//////////////////////////////////////////////////////////////////////
/* Turn Gas Enable Manual*/
void writeOutputGasEnableCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_GAS_ENABLE_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.GasSolAddr ;// Gas Sol Address
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 1; // ON Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

//////////////////////////////////////////////////////////////////////
/* Turn Gas On Command*/
void writeOutputGasOnCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_GAS_ON_OFF_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.GasSolAddr ;// Gas Sol Address
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 1; // ON Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/////////////////////////////////////
void writeOutputGasOffCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_GAS_ON_OFF_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.GasSolAddr ;// Gas Sol Address
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 0; // OFF Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

//////////////////////////////////////////////////////////////////////
/* Turn Weld Cooler  ON*/
void writeOutputCoolerOnCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_COOLER_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.CoolerAddr ;// Cooler Address
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 1; // ON Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* Turn Weld Cooler  ON*/
void writeOutputCoolerOffCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_COOLER_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.CoolerAddr ;// Cooler Address
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 0; // OFF Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

/*	
	Hi Josh,
	 
	The following is from our engineering team regarding this request:
	 
	It’s Group 1, Class ID 29, Attribute 513.  Value of 1 means pump is on and value of 0 means pump is off.
	 
	Let me know if your team needs additional info in order to get control of the Cool Arc 55S, but this should be all the info needed.
*/
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* Turn Weld Controller Power Supply ON*/
void writeOutputOnCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_OUTPUT_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 1; // ON Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* Turn Weld Controller Power Supply OFF */
void writeOutputOffCommand(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;

	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;


	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_OUTPUT_COMMAND;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_WRITE;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 1; // Size of Arc_Link_Event_Report_Request_typ 
	USINT data; // 
	data = 0; // OFF Command

	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data , r->RequestSize);

	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError	= 0;
	h->DataLength	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;

	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r); 

	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);

	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r); 

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}


/* This Action Set the Weld Mode and important control parameters */
void createSetControlsAction(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	SetControlsActionRequest_typ *scar; // Reference to control parameters
	
	if(!arc) return;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;
	scar = &robot->wc.setup.ctrlParam;
	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = SET_CONTROLS_ACTION;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;

	

	/**
	* <pre>
	* 1 Byte	Parameter Mask
	2 Bytes	Weld Mode 	If parameter mask bit 0 set.
	2 Bytes	Work-point	If parameter mask bit 1 set.
	2 Bytes	Trim	If parameter mask bit 2 set.
	2 Bytes	Wave-Control 1	If parameter mask bit 3 set.
	2 Bytes	Wave-Control 2	If parameter mask bit 4 set.
	2 Bytes	Wave-Control 3	If parameter mask bit 5 set.
	2 Bytes	Wave-Control 4	If parameter mask bit 6 set.
	* </pre>
	*/
	// scar->ParameterMask = 0x23;//0b0100011; // change accordingly for Mode 3
	// Weld Mode, Work-Point, Trim, WaveControlX are set externally
	swapSetControlsActionRequest(scar);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &scar->ParameterMask , 1);

	// ARC_LINK_REQUEST_SIZE + 1 represents ParameterMask
	int offset = 1; // One due ParameterMask 

	
	
	if((scar->ParameterMask & 0x1 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WeldMode , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x2 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WorkPoint , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x4 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->Trim , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x8 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WaveControl1 , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x10 ) > 0){	
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WaveControl2 , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x20 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WaveControl3 , 2); offset +=2; // Increase offset
	}
	if((scar->ParameterMask & 0x40 ) > 0){
		memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE + offset, &scar->WaveControl4 , 2); offset +=2; // Increase offset
	}
	
	swapSetControlsActionRequest(scar);
	//r->OutgoingData = er.encode();
	r->RequestSize = offset; // Increased based on parameter Mask. Thus use offset + 1
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

/* Create Weld Table Action To be tested */
void createWeldTableAction(Arc_Link_typ *arc) {
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	WeldTableSearchRequest_typ wtsr;
	
	if(!arc) return;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;
	
	// Create WeldTableSearch ACTION
	wtsr.ModeDescriptor = 0xC80E; //0b1100100000001110;
	wtsr.ModeDescriptorCare =0xC80E;//0b1100100000001110;
	wtsr.WireType = robot->wc.setup.weldParam.WireType; // Take value from user input
	wtsr.WireSize = robot->wc.setup.weldParam.WireSize; // Take value from user input
	wtsr.StartWeldModeIndex = 0;
	wtsr.MaxNumberMatches = 100;
	/* GTAW Gas Tungsten Arc Welding(Pulsed and non-pulsed) */
	wtsr.ProcessType = robot->wc.setup.weldParam.ProcessType; // Take value from user input
	wtsr.GasType = 0;
	wtsr.SearchMask = 0x01; //0b00000001; // Filter by Process Type

	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = WELD_TABLE_SEARCH_ACTION;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = sizeof(WeldTableSearchRequest_typ); // Size of WeldTableSearchRequest_typ
	
	swapWeldTableSearchRequest(&wtsr);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &wtsr , r->RequestSize);
	swapWeldTableSearchRequest(&wtsr);
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc);
}

void createReadCntrlParamDataAction(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 

	ReadControlParameterData_Req_typ readReq;

	/*
	Request The Attribute ID may be any of the WC’s control parameters (work-point, trim, wave-controls 1-4)
	2 Bytes Attribute ID
	2 Bytes Weld Mode
	*/
	
	if(!arc) return;

	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	pr = &arc->PendingResponse;

	readReq.Attribute_ID = robot->Comm.CntrlParamReq.Attribute_ID;
	readReq.Weld_Mode = robot->Comm.CntrlParamReq.Weld_Mode;
	swapBytes((USINT *) &readReq.Attribute_ID , 2);
	swapBytes((USINT *) &readReq.Weld_Mode , 2);
	
	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = WC_READ_CONTROL_PARAMETER_DATA_ACTION;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;
	

	//r->OutgoingData = er.encode();
	r->RequestSize = sizeof(ReadControlParameterData_Req_typ); // 
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &readReq , r->RequestSize);

	// For diagnostic only
	robot->ReqDataSize = r->RequestSize;
	if(robot->ReqDataSize < 32)
		memcpy(robot->ReqData_raw , &readReq , robot->ReqDataSize);
	//////////////////////
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->CreateReadControlParamDataAction = 	h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc);	

	log_event(&robot->eventlog , "Read WC Param Data");
	
}

#endif /* _HANDLER_C */


/* readAttribute30731 */
void readAttribute30731(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ATTRIBUTE_FAULT;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_READ;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 0; // Size of Arc_Link_Event_Report_Request_typ 
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->fsSequence = h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 
}

	
void handleArcLinkResponse( Arc_Link_typ *arc) {

	Arc_Link_Response_typ *r; // temp reference to response
	PendingResponse_typ *pr; 
	Arc_Link_Header_typ *h;

	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	pr = &arc->PendingResponse;
	h = &arc->msgIN.header;
    	
	/*
	 * Header size is 10 bytes array index 0 - 9, therefore incoming data
	 * i.e ArcLink Reponse comes in index 10
	 */
//	DBG_OB("Inside handler - Object Address: %d \r\n" , r->ObjectAddress);
//		DBG_OB("response status %d \r\n" , r->Status);
	DBG_OB("response size %d \r\n" , r->ResponseSize);
	
	// Handle Messages from Power Supply
	if (r->ObjectAddress == WELD_CONTROLLER_NETWORK_ADDRESS) {

		
	
		if (h->Sequence == pr->fsSequence){
			handleResponseAttribute30731(arc);    //parse 4 byte faults status
			pr->fsSequence = 0; // Reset flag
		}
		if (h->Sequence == pr->WriteAttribute30770Sequence){
			if (!r->Status) // write success
				pr->WriteAttribute30770Sequence = 0; // Reset flag
			else // failed. set resend flag 
				pr->WriteAttribute30770 = 0;
		}	
		if (h->Sequence == pr->WriteAttribute30781Sequence){
			if (!r->Status) // write success
				pr->WriteAttribute30781Sequence = 0; // Reset flag
			else // failed. set resend flag 
				pr->WriteAttribute30781 = 0;
		}	
		if (h->Sequence == pr->WriteAttribute30758Sequence){
			if (!r->Status) // write success
				pr->WriteAttribute30758Sequence = 0; // Reset flag
			else // failed. set resend flag 
				pr->WriteAttribute30758 = 0;
		}	
		if (h->Sequence == pr->CreateReadyReportSequence){
			if (!r->Status) // write success
				pr->CreateReadyReportSequence = 0; // Reset flag
			else // failed. set resend flag 
				pr->CreateReadyReport = 0;
		}	
		if (h->Sequence == pr->CreateFaultReportSequence){
			if (!r->Status) // write success
				pr->CreateFaultReportSequence = 0; // Reset flag
			else // failed. set resend flag 
				pr->CreateFaultReport = 0;
		}	

		if(h->Sequence == pr->sqConfigDataSearchAction){ //finding Cooler Address
			handleResponseConfigDataSearchAction(arc);
			pr->sqConfigDataSearchAction = 0;
		}

		if(h->Sequence == pr->sqConfigDataSearchAction_GasSol){ //finding Gas Sol Address
			handleResponseConfigDataSearchAction_GasSol(arc);
			pr->sqConfigDataSearchAction_GasSol = 0;
		}
		
		if(h->Sequence == pr->CreateReadControlParamDataAction){ //ReadWCParamDataAction 
			handleResponseReadControlParamDataAction(arc);
			pr->CreateReadControlParamDataAction = 0;
		}

		
	}
	
	// Bus Master Address is Zero
	if (r->ObjectAddress == 0 )
	{
		if(h->Sequence == pr->WriteAttribute30Sequence){
			if(!r->Status)
				pr->WriteAttribute30Sequence = 0; // Reset flag
			else // failed. set resend flag
				pr->WriteAttribute30 = 0;
		}		
	}

	////////////////////////////////////////////////////////////////	
	// Cooler Address
	if (r->ObjectAddress == arc->robot.miscSetup.CoolerAddr){
	
		if(h->Sequence == pr->sqReadCoolerSens){ //read Cooler Sens
			handleResponse_ReadCoolerSens(arc);
			pr->sqReadCoolerSens = 0;
		}
	}
	
}

void handleResponseAttribute30731(Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	robot = &arc->robot;
	
	
	DBG_OB("Received response for fault status \r\n" );	
	log_event(&robot->eventlog , "Received detailed fault status from PS");

	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->ResponseSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving fault status with Response size \r\n");	
		return;
	}   
	
	if (*data & 0x01){                             // first byte, only first bit meaningful  MSB
		DBG_OB("24 Single Phase Input Power \r\n"); log_error(&robot->errorlog , "24 Single Phase Input Power"); } 

	if (*data >> 1 & 0x01){                             
		DBG_OB("25 Reserved \r\n"); log_error(&robot->errorlog , "25 Reserved"); }  

	if (*data >> 2 & 0x01){                             
		DBG_OB("26 Reserved \r\n"); log_error(&robot->errorlog , "26 Reserved"); } 

	if (*data >> 3 & 0x01){                             
		DBG_OB("27 Reserved \r\n"); log_error(&robot->errorlog , "27 Reserved"); } 

	if (*data >> 4 & 0x01){                             
		DBG_OB("28 Reserved \r\n"); log_error(&robot->errorlog , "28 Reserved"); } 

	if (*data >> 5 & 0x01){                             
		DBG_OB("29 Reserved \r\n"); log_error(&robot->errorlog , "29 Reserved"); } 

	if (*data >> 6 & 0x01){                             
		DBG_OB("30 Reserved \r\n"); log_error(&robot->errorlog , "30 Reserved"); } 

	if (*data >> 7 & 0x01){                             
		DBG_OB("31 Reserved \r\n"); log_error(&robot->errorlog , "31 Reserved"); } 


		
	if (*(data + 1) & 0x01){                // second byte, first bit
		DBG_OB("16	Control PC Board Problem\r\n"); log_error(&robot->errorlog , "16 Control PC Board Problem"); } 
		
	if (*(data + 1) >> 1 & 0x01) {                
		DBG_OB("17	Control PC Board Problem\r\n");log_error(&robot->errorlog , "17 Control PC Board Problem");}
		
	if (*(data + 1) >> 2 & 0x01) {                
		DBG_OB("18	Control PC Board Problem\r\n");log_error(&robot->errorlog , "18 Control PC Board Problem");}
		
	if (*(data + 1) >> 3 & 0x01){                 
		DBG_OB("19	Control PC Board Problem\r\n");log_error(&robot->errorlog , "19 Control PC Board Problem");}
		
	if (*(data + 1) >> 4 & 0x01){                 
		DBG_OB("20	Control PC Board Problem\r\n");log_error(&robot->errorlog , "20 Control PC Board Problem");}
		
	if (*(data + 1) >> 5 & 0x01){                 
		DBG_OB("21	Control PC Board Problem\r\n");log_error(&robot->errorlog , "21 Control PC Board Problem");}
		
	if (*(data + 1) >> 6 & 0x01){                 
		DBG_OB("22	Secondary Over Power\r\n");log_error(&robot->errorlog , "22 Secondary Over Power");}
		
	if (*(data + 1) >> 7 & 0x01){                 
		DBG_OB("23	Control PC Board Problem\r\n");log_error(&robot->errorlog , "23 Control PC Board Problem");}
		
	if (*(data + 2) & 0x01){                 // third byte, first bit
		DBG_OB("8	Primary Over Current\r\n");log_error(&robot->errorlog , "8 Primary Over Current");}
		
	if (*(data + 2) >> 1 & 0x01){                 
		DBG_OB("9	Inverter Capacitor Delta Voltage\r\n");log_error(&robot->errorlog , "9 Inverter Capacitor Delta Voltage");}
		
	if (*(data + 2) >> 2 & 0x01){                 
		DBG_OB("10	Control PC Board Problem\r\n");log_error(&robot->errorlog , "10 Control PC Board Problem");}
		
	if (*(data + 2) >> 3 & 0x01){                 
		DBG_OB("11	Control PC Board Problem\r\n");log_error(&robot->errorlog , "11 Control PC Board Problem");}
		
	if (*(data + 2) >> 4 & 0x01){                 
		DBG_OB("12	Control PC Board Problem\r\n");log_error(&robot->errorlog , "12 Control PC Board Problem");}
		
	if (*(data + 2) >> 5 & 0x01){                
		DBG_OB("13	Inverter PreCharge Failed\r\n");log_error(&robot->errorlog , "13 Inverter PreCharge Failed");}
		
	if (*(data + 2) >> 6 & 0x01){                
		DBG_OB("14	ACDC Paralleled Power Source Fault\r\n"); log_error(&robot->errorlog , "14 ACDC Paralleled Power Source Fault");
	}
		
	if (*(data + 2) >> 7 & 0x01){                 
		DBG_OB("15	Control PC Board Problem\r\n");log_error(&robot->errorlog , "15 Control PC Board Problem");}
		
		
	if (*(data + 3) & 0x01){                 // four byte, first bit
		DBG_OB("0	Secondary Over Current\r\n");log_error(&robot->errorlog , "0 Secondary Over Current");}

	if (*(data + 3) >> 1 & 0x01){                 
		DBG_OB("1 Reserved\r\n");log_error(&robot->errorlog , "1 Reserved");}

	if (*(data + 3) >> 2 & 0x01){                 
		DBG_OB("2 Reserved \r\n");log_error(&robot->errorlog , "2 Reserved");}

	if (*(data + 3) >> 3 & 0x01){                 
		DBG_OB("3	Thermal\r\n");log_error(&robot->errorlog , "3 Thermal");}
		
	if (*(data + 3) >> 4 & 0x01){                 
		DBG_OB("4	Inverter Capacitor Bank B Over Voltage\r\n");log_error(&robot->errorlog , "4 Inverter Capacitor Bank B Over Voltage");}
		
	if (*(data + 3) >> 5 & 0x01){                 
		DBG_OB("5	Inverter Capacitor Bank A Over Voltage\r\n");log_error(&robot->errorlog , "5 Inverter Capacitor Bank A Over Voltage");}
		
	if (*(data + 3) >> 6 & 0x01){               
		DBG_OB("6	Inverter Capacitor Bank B Under Voltage\r\n");log_error(&robot->errorlog , "6 Inverter Capacitor Bank B Under Voltage");}
		
	if (*(data + 3) >> 7 & 0x01){                 
		DBG_OB("7	Inverter Capacitor Bank A Under Voltage\r\n");log_error(&robot->errorlog , "7 Inverter Capacitor Bank A Under Voltage");}
		
	//			
		
	//		Bit	Condition
	//		0	Secondary Over Current
	//		1	Reserved
	//		2	Reserved
	//		3	Thermal
	//		4	Inverter Capacitor Bank B Over Voltage
	//		5	Inverter Capacitor Bank A Over Voltage
	//		6	Inverter Capacitor Bank B Under Voltage
	//		7	Inverter Capacitor Bank A Under Voltage
	//		8	Primary Over Current
	//		9	Inverter Capacitor Delta Voltage
	//		10	Control PC Board Problem
	//		11	Control PC Board Problem
	//		12	Control PC Board Problem
	//		13	Inverter PreCharge Failed
	//		14	ACDC Paralleled Power Source Fault
	//		15	Control PC Board Problem
	//		16	Control PC Board Problem
	//		17	Control PC Board Problem
	//		18	Control PC Board Problem
	//		19	Control PC Board Problem
	//		20	Control PC Board Problem
	//		21	Control PC Board Problem
	//		22	Secondary Over Power
	//		23	Control PC Board Problem
	//		24	Single Phase Input Power (some models)
	//		25	Reserved
	//		26	Reserved
	//		27	Reserved
	//		28	Reserved
	//		29	Reserved
	//		30	Reserved
	//		31	Reserved

		
}
void readAttribute_CoolerSens(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 
	
	if(!arc) return;
	
	pr = &arc->PendingResponse;
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = 0x7B09;
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	//The attribute for the pump status (separate from the pump command) is 0x7875, and the attribute for the flow status is 0x7B09.

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = robot->miscSetup.CoolerAddr ;// Weld Controller
	r->Service = NETWORK_SERVICE_ATTRIBUTE_READ;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 0; // Size 
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	pr->sqReadCoolerSens = h->Sequence;
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent
	writeMessageToOutgoingRingBuffer(arc); 

	log_event(&robot->eventlog , "Get Cooler Sens REQ");
}

void action_ConfigDataSearch_GasSol(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 
	
	if(!arc) return;

	pr = &arc->PendingResponse;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ACTION_CONFIG_DATA_SEARCH; //Configuration Data Search ACTION                                                                                                    
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS; //BUS_MASTER_ARCLINK_NETWORK_ADDRESS;
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 8; // 
	USINT data[8];

	/*	
	Request 
	1 Byte	Search Parameter Mask
	1 Byte	Group Assignment
	2 Bytes Object Class
	1 Byte	Object Instance
	1 Byte	Object Class Type
	1 Byte	Maximum
	1 Byte	Starting Network Address
*/	

	data[0] = 0x4; //Mask - Attribute Class
	
	data[1] = 0;
	
	data[2] = 0; //Bytes Object Class
	//data[3] = 29; //Bytes Object Class - Cooler
	data[3] = 22; //Bytes Object Class - Remote Control object (object class ID 22)

	data[4] = 0;
	data[5] = 0;
	data[6] = 1;
	data[7] = 1;
	
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data[0] , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	pr->sqConfigDataSearchAction_GasSol = 	h->Sequence;

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 

	log_event(&robot->eventlog , "Get Cooler Adr REQ");
}

void action_ConfigDataSearch_Cooler(Arc_Link_typ *arc){
	// Create a Request Message and header
	Robot_typ *robot;
	Arc_Link_Request_typ *r;
	Arc_Link_Header_typ *h;
	PendingResponse_typ *pr; 
	
	if(!arc) return;

	pr = &arc->PendingResponse;
	
	robot = &arc->robot;
	r = &arc->msgOUT.request;
	h = &arc->msgOUT.header;

	
	// Create a UDP Request Header with all the required info
	r->TimeoutTime = 10; // 10 ms
	r->Flags = 0;
	r->Retries = 1;
	r->MessageCounter = 0;
	r->AddressingMode = 6;
	r->ConnectionID = 15;
	r->MaximumObjs = 1;
	r->ServiceID.AttributeNumber = ACTION_CONFIG_DATA_SEARCH; //Configuration Data Search ACTION                                                                                                    
	r->ServiceID.StateNumber = 0;
	r->ServiceID.PropertyNumber = PROPERTY_ID_VALUE;

	r->SourceAddress = robot->arclink.network.ModuleBaseAddress;
	r->DestinationAddress = WELD_CONTROLLER_NETWORK_ADDRESS; //BUS_MASTER_ARCLINK_NETWORK_ADDRESS;
	r->Service = NETWORK_SERVICE_ACTION;
	r->MaxResponseSize = 254;

	//r->OutgoingData = er.encode();
	r->RequestSize = 8; // 
	USINT data[8];

	/*	
	Request 
	1 Byte	Search Parameter Mask
	1 Byte	Group Assignment
	2 Bytes Object Class
	1 Byte	Object Instance
	1 Byte	Object Class Type
	1 Byte	Maximum
	1 Byte	Starting Network Address
*/	

	data[0] = 0x4; //Mask - Attribute Class
	
	data[1] = 0;
	
	data[2] = 0; //Bytes Object Class
	data[3] = 29; //Bytes Object Class - Cooler
	//data[3] = 22; //Bytes Object Class - Remote Control object (object class ID 22)

	data[4] = 0;
	data[5] = 0;
	data[6] = 1;
	data[7] = 1;
	
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_REQUEST_SIZE, &data[0] , r->RequestSize);
	
	// Data must be sent in Big Endian format
	// Create a Response header for reply
	h->Flags 		= 0x02; // Arclink Request 0b0010

	// It has to be the same as request header 
	h->Sequence 	= rand();
	h->LastError 	= 0;
	h->DataLength 	= ARC_LINK_REQUEST_SIZE + r->RequestSize; // This depends on
	// size of data
	// being sent
	// 6(ARcklink
	// Response size +
	// Data size)

	/* Now encode ArcLink Header and Response */
	//arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength;
	arc->internal.OutMsgLen = ARC_LINK_HEADER_SIZE + h->DataLength; 

	pr->sqConfigDataSearchAction = 	h->Sequence;

	// Temp Data
	int RequestSize = r->RequestSize;
	
	// Swap bytes into Big Endian Format
	swapArcLinkHeaderBytes(h);
	swapArcLinkRequestBytes(r);	
	
	memcpy(arc->internal.pOutMsg, h , ARC_LINK_HEADER_SIZE);
	memcpy(arc->internal.pOutMsg + ARC_LINK_HEADER_SIZE, r , ARC_LINK_REQUEST_SIZE);
	
	// Swap bytes into Big Endian Format
	//swapArcLinkHeaderBytes(h);		
	//swapArcLinkRequestBytes(r);	

	// Arclink Request is ready to be sent

	writeMessageToOutgoingRingBuffer(arc); 

	log_event(&robot->eventlog , "Get Cooler Adr REQ");
}

void handleResponse_ReadCoolerSens(Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	robot = &arc->robot;
	
	
	DBG_OB("Received response for Cooler Search Address \r\n" );	
	
	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->ResponseSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving fault status with Response size \r\n");
		log_error(&robot->errorlog , "Cooler Sens Resp Fault");
		return;
	}  


	//robot->miscSetup.CoolerAddr = data[0];
	//log_event(&robot->eventlog , "Got Cooler Adr RESP"); 

	robot->Cntrl.Fbk.coolerSens_Status = data[0];
	log_event(&robot->eventlog , "Got Cooler Sens"); 

}

void handleResponseReadControlParamDataAction(Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	robot = &arc->robot;
	
	
	DBG_OB("Received response -Read WC Param Data Action\r\n" );	
	//log_event(&robot->eventlog , "Received Cooler Address from PS");

	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->ResponseSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE); // Pointer to data go back here and test
		log_event(&robot->eventlog , "Got WC Param Data"); 
	}else{
		//DBG_OB("Error: Receiving fault status with Response size \r\n");
		log_error(&robot->errorlog , "Get WC Param Data Fault");
		return;
	}  

	memset (&robot->Comm.CntrlParamData , 0 , sizeof(ReadControlParameterData_typ));
	memset (&robot->ResData_raw , 0 , sizeof(robot->ResData_raw));
	robot->ResDataSize = 0;
	
	if(r->ResponseSize == 1){ // return code

		robot->Comm.CntrlParamData.ReturnCode = *data;
	}
	else if((r->ResponseSize > 0)
		&&(r->ResponseSize < 32)){		
		robot->ResDataSize = (UINT)r->ResponseSize;
		memcpy(robot->ResData_raw , data , r->ResponseSize);
	}
	else
		return;
			
	
	memcpy(&robot->Comm.CntrlParamData.MaxVal , (data+0) , 2);
	swapBytes((USINT *)&robot->Comm.CntrlParamData.MaxVal, 2);
	
	memcpy(&robot->Comm.CntrlParamData.MinVal , (data +2) , 2);
	swapBytes((USINT *)&robot->Comm.CntrlParamData.MinVal, 2);
	
	robot->Comm.CntrlParamData.UnitsCode = (USINT)*(data+4);
	
	robot->Comm.CntrlParamData.DecimalPlace = (USINT)*(data+5);	
	memcpy(robot->Comm.CntrlParamData.Name ,  data+6 , 17);
	//swapBytes(robot->Comm.CntrlParamData.Name, 17);



}

void handleResponseConfigDataSearchAction_GasSol(Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	robot = &arc->robot;
	
	
	DBG_OB("Received response for Cooler Search Address \r\n" );	
	//log_event(&robot->eventlog , "Received Cooler Address from PS");

	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->ResponseSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving fault status with Response size \r\n");
		log_error(&robot->errorlog , "Gas Sol Addr Fault");
		return;
	}  


	robot->miscSetup.GasSolAddr = data[0];
	log_event(&robot->eventlog , "Got Gas Sol Adr RESP"); 



}

void handleResponseConfigDataSearchAction(Arc_Link_typ *arc){
	Arc_Link_Response_typ 	*r; // temp reference to request
	Robot_typ 				*robot;
	USINT *data;

	
	if (!arc) return; // Prevent null pointer reference
	r = &arc->msgIN.response;
	robot = &arc->robot;
	
	
	DBG_OB("Received response for Cooler Search Address \r\n" );	
	//log_event(&robot->eventlog , "Received Cooler Address from PS");

	//Log.d(TAG, "Recevied Assign Network Address Action");

	// Copy Data if available to the Incoming data pointer
	if(r->ResponseSize > 0){
		//byte[] data = r.OutgoingData; // incominng data in Big endian format
		data = (arc->internal.pInMsg + ARC_LINK_HEADER_SIZE + ARC_LINK_RESPONSE_SIZE); // Pointer to data go back here and test
	}else{
		DBG_OB("Error: Receiving fault status with Response size \r\n");
		log_error(&robot->errorlog , "Cooler Addr Fault");
		return;
	}  


	robot->miscSetup.CoolerAddr = data[0];
	log_event(&robot->eventlog , "Got Cooler Adr RESP"); 



}
	
/*
	Error Logger by Obed Rev 1.0
*/
void log_error(Log_typ* err, char* msg){
	if(!err) return;

	// Healthy Inialization
	if(!err->pLog){
		err->pLog = &err->log;
	}
	
	memmove((void *)(err->pLog + LOG_MAX_LENGTH), (void *)err->pLog, (LOG_MAX_LENGTH * (LOG_MAX_LINE - 1) ));
	memmove( (void *)(err->pLog), (void *)msg, (LOG_MAX_LENGTH) );	
}


void log_event(Log_typ* event, char* msg){
	if(!event) return;

	// Healthy Inialization
	if(!event->pLog){
		event->pLog = &event->log;
	}
	
	memmove((void *)(event->pLog + LOG_MAX_LENGTH), (void *)event->pLog, (LOG_MAX_LENGTH * (LOG_MAX_LINE - 1) ));
	memmove( (void *)(event->pLog), (void *)msg, (LOG_MAX_LENGTH) );	
}
