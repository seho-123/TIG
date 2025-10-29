 /*
 * ringbuffer.c
 *
 *  Created on: Feb 7, 2017
 *      Author: obed
 */
 

#include <..\..\Lib\utilityLib\libRingBuffer.c>

#include <..\..\Lib\arclink\ArcLinkUtility.c>
#include <..\..\Lib\arclink\Robot.c>
#include <..\..\Lib\arclink\Handler.c>

/*

#include <..\Lib\utilityLib\libRingBuffer.c>

#include <..\Lib\arclink\ArcLinkUtility.c>
#include <..\Lib\arclink\Robot.c>
#include <..\Lib\arclink\Handler.c>
*/



/*
	Handle ArcLink Protocol Messages and Responses.
	Handle Mapping 
*/

void handleIncomingMessage(Arc_Link_typ *arc) ;
void handleIncomingRequest(Arc_Link_typ *arc) ;
void handleIncomingResponse(Arc_Link_typ *arc) ;
void initializeRobot(Arc_Link_typ *arc) ;

void ArcLink_init(Arc_Link_typ *arc , RingBuffer *bufIN, RingBuffer *bufOUT , UDINT  pInMsg  , UDINT  pOutMsg , UDINT recLen ){

	if(!arc)
		return;

	if(!bufIN)
		return;

	if(!bufOUT)
		return;

	arc->bufIN = bufIN;
	arc->bufOUT = bufOUT;

	arc->internal.pInMsg = pInMsg;
	arc->internal.pOutMsg = pOutMsg;

	arc->internal.InMsgLen = recLen;
	arc->internal.OutMsgLen = recLen;

	arc->internal.MaxMsgDataLength = recLen;

	initializeRobot(arc);

	return ;
}	



#if 0
Arc_Link_typ *ArcLink_init(UDINT recNum , UDINT recLen ){
UDINT status;
UDINT p ;	
Arc_Link_typ *arc;
		
		
		status = TMP_alloc(sizeof(Arc_Link_typ),(void**)&(arc));

		if(arc){
			
			arc->internal.MaxMsgDataLength = 0;			
			arc->bufIN = RingBuffer_createByRecords(recNum ,  recLen ); 
			arc->bufOUT = RingBuffer_createByRecords(recNum ,  recLen ); 

			status = TMP_alloc(sizeof(recLen),(void**)&(arc->internal.pInMsg));
			status = TMP_alloc(sizeof(recLen),(void**)&(arc->internal.pOutMsg));

			/* Initialize Robot */
			initializeRobot(arc);
		}
		else
			return 0;

		if(arc->bufIN && arc->bufOUT 
			&& arc->internal.pInMsg && arc->internal.pOutMsg){

			arc->internal.MaxMsgDataLength = recLen;
			return arc;

		}			
		else
			return 0;
}	
#endif

void ArcLink_cyclic(Arc_Link_typ *arc){

	/// Handle incoming messages from ring buffer
	handleIncomingMessage(arc);

	// Perform Robot Misc Setup Required for Telemetry upon Mapping completion
	performRobotMiscSetup(arc);

	// Enabled upon Mapping completion 
	handleRobotCyclicOps(arc);
}
	
 void handleIncomingMessage(Arc_Link_typ *arc) {
USINT *p;
BOOL isAResponse = 0; // Whether message is a request or response
Robot_typ *robot;
	
		robot = &arc->robot;

	if(!RingBuffer_isEmpty(arc->bufIN) && arc->internal.pInMsg) {
		
		arc->internal.InMsgLen  = RingBuffer_readRecord(arc->bufIN, arc->internal.pInMsg);

		arc->internal.InMsgCnt++;
		DBG("Get From Ring %d \r\n" , (USINT)arc->internal.InMsgCnt);
		
/*		DBG_OB("OBED Get From Ring %d \r\n" , (USINT)arc->internal.InMsgCnt);	*/	
		
		robot->Comm.HeartBeat_Received = 1;

		if(arc->internal.InMsgLen  >= 10){	

			p = arc->internal.pInMsg;
			
			arc->msgIN.header.Flags = GetUINT(p);
			arc->msgIN.header.Sequence = GetUINT(p+2);
			arc->msgIN.header.LastError = GetUDINT(p+4);
			arc->msgIN.header.DataLength = GetUINT(p+8);
			
			isAResponse = (arc->msgIN.header.Flags == 2) ? 0 : 1;			
			
		/*	DBG_OB("Incoming Msg Header - Flags: %d Sequence: %d  DataLength: %d \r\n" , arc->msgIN.header.Flags , arc->msgIN.header.Sequence , arc->msgIN.header.DataLength);*/
			
			if (isAResponse) {
				handleIncomingResponse(arc);
			} else {
				handleIncomingRequest(arc);
			}

			
		}
		else
			DBG("Incoming  Message Header - Fault  \r\n" );

	}		
}

void handleIncomingRequest(Arc_Link_typ *arc) {
UDINT msgLen;	
USINT *p;

	p = arc->internal.pInMsg; 

	if(arc->internal.InMsgLen >= 18 ){
		p+= 10;


		arc->msgIN.request.TimeoutTime = GetUINT(p);

		memcpy(&arc->msgIN.request.Flags , p+2 , 6);

		arc->msgIN.request.ServiceID.AttributeNumber  = GetUINT(p + 8);

		memcpy(&arc->msgIN.request.ServiceID.StateNumber , p+10 , 8);	
		
//		DBG("Incoming Request Header - Flags: %d  \r\n" , arc->msgIN.request.Flags);
//		DBG("Req TimeoutTimeFlags: %d\r\n" , arc->msgIN.request.TimeoutTime);
//		DBG("Req Flags: %d\r\n" , arc->msgIN.request.Flags);
//		DBG("Req Retries: %d\r\n" , arc->msgIN.request.Retries);
//		DBG("Req Message Counter: %d\r\n" , arc->msgIN.request.MessageCounter);
//		DBG("Req AddressingMode: %d\r\n" , arc->msgIN.request.AddressingMode);
//		DBG("Req ConnectionID: %d\r\n" , arc->msgIN.request.ConnectionID);
//		DBG("Req MaximumObjs: %d\r\n" , arc->msgIN.request.MaximumObjs);
//		DBG("Req ServiceID.AttributeNumber: %d\r\n" , arc->msgIN.request.ServiceID.AttributeNumber);
//		DBG("Req ServiceID.StateNumber: %d\r\n" , arc->msgIN.request.ServiceID.StateNumber);
//		DBG("Req ServiceID.PropertyNumber: %d\r\n" , arc->msgIN.request.ServiceID.PropertyNumber);
//		DBG("Req SourceAddress: %d\r\n" , arc->msgIN.request.SourceAddress);
//		DBG("Req DestinationAddress: %d\r\n" , arc->msgIN.request.DestinationAddress);
//		DBG("Req Service: %d\r\n" , arc->msgIN.request.Service);
//		DBG("Req MaxResponseSize: %d\r\n" , arc->msgIN.request.MaxResponseSize);
//		DBG("Req RequestSize: %d\r\n" , arc->msgIN.request.RequestSize);
//		DBG("Req Reserved: %d\r\n" , arc->msgIN.request.Reserved);		
			
		handleArcLinkRequests(arc); // Hnadle arcLink Requests
	}
	else
		DBG("Incoming Request Header - Fault  \r\n" );

	


}
void handleIncomingResponse(Arc_Link_typ *arc){
USINT *p;

	p = arc->internal.pInMsg;

	DBG_OB("Incoming Response Header\r\n" )

	UDINT msgLen;	

	if(arc->internal.InMsgLen >= 6 ){
		p+= 10;


		arc->msgIN.response.ObjectCount = *p;
		
		arc->msgIN.response.Status = *(p+2);
		
		arc->msgIN.response.ObjectAddress = *(p+3);
		
		arc->msgIN.response.ResponseSize = *(p+4);

		/*		memcpy(arc->msgIN.response.ResponseData , p+6 , arc->msgIN.response.ResponseSize);	*/	

		handleArcLinkResponse(arc); // Handle arcLink Response
	}
	else
		DBG_OB("Incoming Response Header - Fault  \r\n" );
	
}

/* Initialize Robot Required Configuration */
void initializeRobot(Arc_Link_typ *arc){
	if(!arc) return;

	Robot_typ *robot = &arc->robot;
	ArcLinkObject_typ *arclink = &robot->arclink;
		
	/* Preinitialize This Robot Parameter */
	/* Network Config and Status */
	arclink->network.ClassID = ROBOT_CLASS; // (IS_ROBOT) ? ROBOT_CLASS: PENDANT_CLASS;
	arclink->network.VendorID = ROBOT_CLASS; // (IS_ROBOT) ? ROBOT_CLASS: PENDANT_CLASS;
	arclink->network.VendorName = "AZZ-WSI";
	arclink->network.ModuleModelNumber = "007";
	arclink->network.ModuleModelName = "Universal Controller";

	/**
	 * <pre>
	The recommended class type for a robot is 2 for �configuration� type.
	bit 0: controlled
	bit 1: config
	 * </pre>
	 */
	arclink->network.ClassType = ROBOT_CLASS_TYPE; // (IS_ROBOT) ? ROBOT_CLASS_TYPE: PENDANT_CLASS_TYPE; /*
									 /*
									 * Robot is config according to Mapping
									 * instructions
									 */
	arclink->network.GroupSwitchSettings = 0; // Group 1
	arclink->network.InstanceSwitchSettings = 0; // Check this
	arclink->network.GroupAssignments = 0; // Group 1
	arclink->network.InstanceAssignments = 0;
	arclink->network.ArclinkRevision = 1;
	arclink->network.ReadyStatus = (USINT) (1 << 8);//(byte) 0b10000000;
	arclink->network.FaultStatus = 0;
	arclink->network.SiliconSerialNumber = 0xF000;
	arclink->network.NumberofEmbeddedObjects = 1;
	arclink->network.EmbeddedObjectNumber = 1;

	///////////////////////////////// "123456789A123456789B123456789C123456789D123456789E123456789F1234"
	arclink->network.SoftwareVersion = "SW:AZZ 1.0";
	arclink->network.HardwareRevision = "HW:AZZ 1.0";
	arclink->network.DiagnosticStatus = 0;
	arclink->network.BusAvailable = 0;
	arclink->network.Bootstrap = 0;
	arclink->network.ObjectIndicator = 0;
	arclink->network.NumberofTimesMapped = 0;
	arclink->network.ModuleBaseAddress = 6; // temp for testing
	arclink->network.NumberofObjectsMapped = 0;
	arclink->network.HBDisable = 0;
	arclink->network.PairedItemsCount = 0x0; // tested it works ;)

	// Initialization Flag
	robot->isMapped  = 0;
	robot->isMapping = 0;
	robot->isMiscSetupOneShot = 0;
	robot->Cntrl.Fbk.Status &= ~ArcLink_Mapped;
	robot->Cntrl.Fbk.Status &= ~ArcLink_Mapping;
}
