
#ifndef _HANDLER_H
#define _HANDLER_H

#include <bur/plctypes.h>

#include <runtime.h>


_GLOBAL static UINT SEQUENCE_NUMBER;




/* Function Prototypes */
void handleArcLinkResponse ( Arc_Link_typ *arc);
void handleArcLinkRequests( Arc_Link_typ *arc);
void handleMasterToSlavesRequest( Arc_Link_typ *arc); 
void handleMasterToSlaveRequest( Arc_Link_typ *arc);
void handlePeerToPeerRequest( Arc_Link_typ *arc); 
void handlePeerToPeersRequest( Arc_Link_typ *arc); 

/* Network Services */
void handleNetworkAction( Arc_Link_typ *arc); 

/* Network Actions */
void processRecognitionActionArcLinkRequest( Arc_Link_typ *arc); 
void processRecognitionActionArcLinkResponse( Arc_Link_typ *arc); 
void processAssignNetworkAddressAction( Arc_Link_typ *arc); 
void processAddToGroupAddressAction(Arc_Link_typ *arc); /* Action 5 */
void processGroupDataAction(Arc_Link_typ *arc); /* Action 2, Process Group Data Action */
void processNOP_ACTION(Arc_Link_typ *arc); /* Action 19 */ 
void processGetConfigurationObjectControlsAction(Arc_Link_typ *arc); /* Action 32 */
void processCreateEventReportAction(Arc_Link_typ *arc); /* Action 11*/
void processAttributeSearchAction(Arc_Link_typ *arc); /* Action 8 */

/* Network Blocks */
void handleNetworkBlock(Arc_Link_typ *arc); 

/* Network block 0*/
void processReadNetworkConfigurationDataBlock(Arc_Link_typ *arc);
/* Read Network Configuration Data Response acknowledge */
void processReadNetworkConfigurationDataBlockResponse(Arc_Link_typ *arc);

/* Read Attributes */
void handleReadAttributeClassProperty(Arc_Link_typ *arc);
void handleReadAttributeValue(Arc_Link_typ *arc);

/* Write Atributes */
void handleWriteAttributeClassProperty(Arc_Link_typ *arc);
void handleWriteAttributeValue(Arc_Link_typ *arc);

/* handle unconfirmed Event Blocks, E.g. used with Telemetry */
void handleUnconfirmedEventBlock(Arc_Link_typ *arc);
void processTelemetryEvent(Arc_Link_typ *arc);
void processEnergyFBEvent(Arc_Link_typ *arc);
void processBlockMeterEvent(Arc_Link_typ *arc);
void processWeldScoreEvent(Arc_Link_typ *arc);

/* handle unconfirmed Event, E.g. arc change status*/
void handleUnconfirmedEvent(Arc_Link_typ *arc);
void processArcChangeStatusEvent(Arc_Link_typ *arc);

/* Key Methods */
void sendSuccesfullWriteAttributeConfirmation( Arc_Link_typ *arc); 

/* write Message to Outgoing RingBuffer */
void writeMessageToOutgoingRingBuffer( Arc_Link_typ *arc);

/* Perform Misc Setup required for Telemetry */
void performRobotMiscSetup(Arc_Link_typ *arc);
void miscSetup(Arc_Link_typ *arc);
void createEventReport(Arc_Link_typ *arc);

/* Robot specific Commands */
void writeOutputOnCommand(Arc_Link_typ *arc);
void writeOutputOffCommand(Arc_Link_typ *arc);

void writeOutputCoolerOnCommand(Arc_Link_typ *arc);
void writeOutputCoolerOffCommand(Arc_Link_typ *arc);


void createSetControlsAction(Arc_Link_typ *arc);
/* Create Weld Table Action To be tested */
void createWeldTableAction(Arc_Link_typ *arc);

//arc status change event
void processArcStatusChangeEvent(Arc_Link_typ *arc);

// process ready, faults status
void processStatusEvent(Arc_Link_typ *arc);

/* Read Attribute 30731 */
void readAttribute30731(Arc_Link_typ *arc);

/* handle fault status response */
void handleResponseAttribute30731(Arc_Link_typ *arc);


void action_ConfigDataSearch(Arc_Link_typ *arc);
/* handle Config Data Search response -  cooler Address*/
void handleResponseConfigDataSearchAction(Arc_Link_typ *arc);


void readAttribute_CoolerSens(Arc_Link_typ *arc);
void handleResponse_ReadCoolerSens(Arc_Link_typ *arc);



/*
	Error Logger by Obed Rev 1.0
*/
void log_error(Log_typ* err, char* msg);

#endif /* _HANDLER_H */
