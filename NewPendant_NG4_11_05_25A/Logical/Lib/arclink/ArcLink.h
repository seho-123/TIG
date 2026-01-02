#ifndef _ARCLINK_H
#define _ARCLINK_H

#include <bur/plctypes.h>

#include <runtime.h>



#include <..\..\Lib\utilityLib\bgdebug.h>

#include <..\..\Lib\utilityLib\libRingBuffer.h>


#include <..\..\Lib\arclink\NetworkActions.h>
#include <..\..\Lib\arclink\NetworkAttribute.h>
#include <..\..\Lib\arclink\ArcLinkObject.h>
#include <..\..\Lib\arclink\Robot.h>

#include <..\..\Lib\arclink\WeldController.h>
#include <..\..\Lib\arclink\NetworkBlocks.h>
#include <..\..\Lib\arclink\Attribute.h>



//#include <..\Lib\utilityLib\bgdebug.h>

/*

#include <..\Lib\utilityLib\libRingBuffer.h>


#include <..\Lib\arclink\NetworkActions.h>
#include <..\Lib\arclink\NetworkAttribute.h>
#include <..\Lib\arclink\ArcLinkObject.h>
#include <..\Lib\arclink\Robot.h>

#include <..\Lib\arclink\WeldController.h>
#include <..\Lib\arclink\NetworkBlocks.h>
#include <..\Lib\arclink\Attribute.h>

*/


#define ARC_LINK_OBJECT_BUS_MASTER_ADDRESS 	0
#define ARC_LINK_HEADER_SIZE 				10
#define ARC_LINK_REQUEST_SIZE 				18 // Without Data
#define ARC_LINK_RESPONSE_SIZE 				6 // Without Data

#define ARC_LINK_TXT_BUF_LEN 	512
#define ARC_LINK_TXT_REC_NUM 	8
#define ARC_LINK_TELEMETRY_DESTINATION_ADDRESS 128
#define ACR_LINK_TELEMETRY_ALL_OBJECT_ADDRESS 254

/* Arclink Message  */
typedef struct Arc_Link_Internal_typ{
	UDINT MaxMsgDataLength;
	USINT *pInMsg;
	UDINT  InMsgLen;
	USINT *pOutMsg;
	UDINT  OutMsgLen;

	USINT  InMsgCnt;
	USINT  OutMsgCnt;
	
} Arc_Link_Internal_typ;

/**
 * @author Obed Bardales
 *         <p>
 *         <b>ArcLink UDP Message Header</b>, which is required in any
 *         ArcLink/XT transmission, has the following structure:
 *         </p>
 *         <br>
 *
 */
typedef struct Arc_Link_Header_typ
{	
		/**
	 * <b>Flags</b>: The Flags are a number of boolean settings which control
	 * the type and behavior of the given message packet. (See Table 13 2
	 * Message Header Flags Bit fields) <br>
	 * <b>Value / Range</b>: 0 to 65535 <br>
	 * <b>Size</b>: 2 <i>bytes</i><br>
	 * Bits Description<br>
	 * 0 (LSB) If not set, then the accompanying message is a request. If set,
	 * then the message is a response.<br>
	 * 1 If not set, then the accompanying message is a Server Command. If set,
	 * then the message is an ArcLink Command. ArcLink/XT does not use server
	 * commands; this bit must be set.<br>
	 * 2 Flag for whether Group Addressing is to be used. ArcLink/XT does not
	 * support Group requests; this bit must be cleared.<br>
	 * 3 Flag for whether Sessions should be used. ArcLink/XT does not use
	 * sessions; this bit must be cleared<br>
	 * 13 – 15 3-bit number (0 – 7) indicating the Last Error Type (to accompany
	 * LastError in the Message Header) (See Table 13 3 Error Types) <br>
	 * Table 13 2 Message Header Flags Bit fields <br>
	 * <br>
	 * Error Types<br>
	 * Type Description<br>
	 * 1 Port Error. Occurs in the event of a timeout or a framing error on a
	 * certain port.<br>
	 * 2 NA<br>
	 * 3 Windows Error. Occurs when a COM port fails, or on file access
	 * errors.<br>
	 * 4 Server Error. Occurs in the event of a timeout on the server side, a
	 * malformed ALTCP message, or a bad argument in a server command.<br>
	 * 5 ArcLink Error. Occurs on the ArcLink side only.<br>
	 * 6 WinSock Error. Occurs when a socket is closed unexpectedly. <br>
	 * Table 13 3 Error Types<br>
	 * 
	 * 
	 */
	UINT Flags;
	/**
	* <b>Sequence</b>: A simple number used to identify the process which
	* initially sent the request. Response should match request. Should not be
	* advanced on a message retry attempt. <br>
	* <b>Value / Range</b>: 0 to 65535 <br>
	* <b>Size</b>: 2 <i>bytes</i>
	*/
	UINT Sequence;
	/**
	 * <b>Last Error</b>: In responses, used to inform the application that an
	 * error occurred while processing the requested command. A value of 0
	 * implies no error. <br>
	 * <b>Value / Range</b>: 0 to 4G<br>
	 * <b>Size</b>: 4 <i>bytes</i>
	 */
	UDINT LastError;
	/**
	* <b>Data Length</b>: The size of the entire message packet, not including
	* this header. <br>
	* <b>Value / Range</b>: 0 to 65535<br>
	* <b>Size</b>: 2 <i>bytes</i>
	*/
	UINT DataLength;
} Arc_Link_Header_typ;

/*
	ArcLink ServiceID
*/
typedef struct ServiceID_typ{
	UINT 	AttributeNumber;
	USINT	StateNumber;
	USINT	PropertyNumber;
} ServiceID_typ;

/**
 * @author Obed Bardales
 *         <p>
 *         <b>ArcLink Requests</b>, If the incoming message is an ArcLink
 *         Command (Bit 1 of the Flags is set), then the data immediately
 *         following the header will be an ArcLink Command Request.
 *         </p>
 *         <br>
 *
 */
typedef struct Arc_Link_Request_typ
{	
	/**
	 * <b>Timeout Time (10-ms ticks)</b>: Response time-out time for proxied CAN
	 * requests (default is 100), 20ms recommended.<br>
	 * <b>Value / Range</b>: 0 to 65535 <br>
	 * <b>Size</b>: 2 <i>bytes</i>
	 */
	UINT TimeoutTime;
	/**
	 * <b>Flags</b>: Several boolean settings to further configure the command
	 * request. (See Table 13.5 ArcLink Command Request Flags Bit fields)<br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i> Bits Description <br>
	 * (LSB) 0 - 1 Response Filter Type (0 to 3)<br>
	 * 0 = Match (default)<br>
	 * 1 = First-in<br>
	 * 2 Abort (default is false)<br>
	 * 3 Specifies whether the Timeout Time in the Command Request applies to
	 * each individual response (true) or all responses at once (false).<br>
	 *
	 */
	USINT Flags;
	/**
	 * <b>Retries</b>: Number of retry attempts for proxied CAN requests
	 * (default is 1)<br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT Retries;
	
	/**
	 * <b>Message Counter</b>: A simple number used to identify the process
	 * which initially sent the request. Response should match request. Should
	 * not be advanced on a message retry attempt. <br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT MessageCounter;

	/**
	 * <b>Addressing Mode</b>: Addressing Mode to use (default is peer-peer)
	 * (See Table 13 6 Addressing Modes) ArcLink/XT may not make requests using
	 * group addressing. <br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i><br>
	 * Mode Description <br>
	 * 0 Master-Slave<br>
	 * 1 Master-Slaves<br>
	 * 4 Slave-Master<br>
	 * 6 Peer-Peer<br>
	 * 7 Peer-Peers<br>
	 * 
	 */
	USINT AddressingMode;

	/**
	 * <b>Connection ID</b>: Connection ID to use (default is 15) (See Table
	 * 13.7 Connection IDs) <br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i><br>
	 * ID Description<br>
	 * 2 Active Master-Slave<br>
	 * 3 Active Slave-Master Active Peer-Peer<br>
	 * 15 General<br>
	 * 
	 */
	USINT ConnectionID;
	
	/**
	 * <b>Maximum Objs</b>: Maximum number of ArcLink Object respondents. If
	 * Group Addressing is not used, then this value must be set to 1.
	 * ArcLink/XT should set this field to 1. <br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT MaximumObjs;

	/**
	 * <b>Service ID</b>: (See Attribute Service ID)<br>
	 * <b>Value / Range</b>: ... <br>
	 * <b>Size</b>: 4 <i>bytes</i><br>
	 *
	 * Attributes are referenced using an attribute service ID. Table 15 2 shows
	 * that this structure includes an attribute number, a property number and a
	 * state number. The state is only relevant for attributes whose Attribute
	 * Class Property is set to 10 or 11, in which case the state field selects
	 * the state. If a state is not applicable for the attribute in question,
	 * the value in this field is ignored.<br>
	 * 
	 * |Attribute Number| State Number |Attribute Property Number E.g. Value, Low limit, High limit| <br>
	 * |2 bytes |1 byte |1 byte |<br>
	 * Table 15 2- Attribute Service ID
	 *
	 * 
	 */
	ServiceID_typ ServiceID;

	/**
	 * <b>Source Address</b>: Source address of the message.<br>
	 * <b>Value / Range</b>: 0 - 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT SourceAddress;

	/**
	 * <b>Destination Address</b>: Destination Address of the message. Either an
	 * ArcLink Object network address or an ArcLink Group Network address,
	 * depending on the Addressing Mode ArcLink/XT may not make requests using
	 * group addressing. <br>
	 * <b>Value / Range</b>: 0 - 255 <br>
	 * <b>Size</b>: 1 <i>byte</i><br>
	 * 
	 * <b>24. ArcLink Object Classes</b><br>
	 * Object Object Class Function<br>
	 * Bus Master (BM) 0 Assigns (“maps”) ArcLink addresses.<br>
	 * Weld-Controller (WC) 1 Contains power on/off and welding process
	 * adjustments for the welding power supply.<br>
	 * Wire Drive (WD) 2 Contains motor on/off and set wire feed speed command
	 * value.<br>
	 * User Interface 3 Manual user interface<br>
	 * Gas Controller (GC) 4 Contains the gas on/off command to control the gas
	 * solenoid.<br>
	 * Sense Lead (SL) 5 Controls remote torch voltage electrode sense
	 * connection<br>
	 * DeviceNet Gateway 6 DeviceNet Gateway<br>
	 * Reserved 7 <br>
	 * Ethernet Gateway (GW) 8 Ethernet Gateway. This is the gateway over which
	 * ArcLink/XT messages arrive.<br>
	 * Spool Gun 9 Spool Gun<br>
	 * Robot 10 Controls the other objects.<br>
	 * Phase Generator 11 Phase Generator. In ACDC systems, this determines the
	 * frequency and phase relationships (from arc to arc).<br>
	 * Arc Object 12 The Arc Object is used in previous generation ACDC
	 * systems.<br>
	 * Reserved 13 <br>
	 * Dual User Interface 14 Dual manual feeder user interface.<br>
	 * Pendent 15 Manual operator pendent.<br>
	 * Reserved 16 <br>
	 * Process Monitor 17 The Process Monitor object records welding data and
	 * enforces process limits (if set).<br>
	 * Spread Arc 18 The spread arc device oscillates the welding torch.<br>
	 * Flux Hopper 19 The flux hopper dispenses flux in SAW welding.<br>
	 * Tractor Travel 20 The tractor moves the welding torch.<br>
	 * Travel Carriage 21 The travel carriage moves the welding torch.<br>
	 * Remote 22 The remote reads analog inputs (for remote analog
	 * pendants).<br>
	 * Stick/TIG UI 23 The Stick/TIG UI is a limited function manual user
	 * interface.<br>
	 * STT Switch 24 The STT switch is used in the STT process.<br>
	 * AC Switch 25 The AC switch is used in AC processes.<br>
	 * 
	 */
	USINT DestinationAddress;

	/**
	 * <b>Service</b>: Number of the service to execute (See Appendix: Network
	 * Services) <br>
	 * <b>Value / Range</b>: 0 - 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT Service;

	/**
	 * <b>Max Response Size</b>: Maximum size of the response (in bytes) <br>
	 * <b>Value / Range</b>: 0 - 254 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT MaxResponseSize;

	/**
	 * <b>Request Size</b>: Size of outgoing ArcLink Data<br>
	 * <b>Value / Range</b>: 0 - 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT RequestSize;

	/**
	 * <b>Reserved</b>: A reserved byte<br>
	 * <b>Value / Range</b>: 0 - 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT Reserved;

	/**
	 * <b>OutgoingData</b>: Outgoing ArcLink data<br>
	 * <b>Value / Range</b>: ... <br>
	 * <b>Size</b>: 2+ <i>bytes</i>
	 */
	
	USINT* OutgoingData;
} Arc_Link_Request_typ;

/**
 * @author Obed Bardales
 *         <p>
 *         <b>ArcLink Response</b>, A response to an ArcLink Command request
 *         coming back from the machine must be preceded by an ALTCP header, as
 *         specified above. The header’s Flags must have bits 0 and 1 set to
 *         True in order to be identified as an ArcLink response. Immediately
 *         after the header, the ArcLink Command Response is structured as
 *         follows
 *         </p>
 *         <br>
 *
 */
typedef struct Arc_Link_Response_typ
{	
	/**
	 * <b>Object Count</b>: Number of objects that responded to the ArcLink
	 * request<br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT ObjectCount;
	/**
	* <b>Reserved</b>: A reserved byte<br>
	* <b>Value / Range</b>: 0 to 255 <br>
	* <b>Size</b>: 1 <i>byte</i>
	*/
	USINT Reserved;
	/**
	 * <b>Status</b>: If the request had caused an ArcLink error, the error will
	 * be recorded in this status byte. If there was no error, status will be
	 * 0.<br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT Status;
	/**
	 * <b>Object Address</b>: The ArcLink network address of the object that
	 * responded. <br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT ObjectAddress;
	/**
	 * <b>Response Size</b>: The size (in bytes) of the incoming ArcLink Data
	 * <br>
	 * <b>Value / Range</b>: 0 to 254 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT ResponseSize;
	/**
	 * <b>Reserved</b>: A reserved byte<br>
	 * <b>Value / Range</b>: 0 to 255 <br>
	 * <b>Size</b>: 1 <i>byte</i>
	 */
	USINT Reserved2;
	/**
	 * <b>Response Data</b>: Incoming ArcLink Data<br>
	 * <b>Value / Range</b>: ... <br>
	 * <b>Size</b>: 2+ <i>bytes</i>
	 */
	USINT* ResponseData;
} Arc_Link_Response_typ;

/* Arclink Message  */
typedef struct Arc_Link_Message_typ{
	Arc_Link_Header_typ header;
	Arc_Link_Request_typ request;
	Arc_Link_Response_typ response;
} Arc_Link_Message_typ;

typedef struct PendingResponse_typ{
	BOOL WriteAttribute30770;   //1 Enable 
	BOOL WriteAttribute30781;   //1 Enable 
	BOOL WriteAttribute30758;   //1 Enable 
	BOOL WriteAttribute30;
	BOOL CreateReadyReport;
	BOOL CreateFaultReport;
	
	UINT fsSequence;  // Sequence tracked for attribute 30731 (fault status)
	UINT sqConfigDataSearchAction;
	UINT sqReadCoolerSens;
	UINT WriteAttribute30770Sequence; 
	UINT WriteAttribute30781Sequence; 
	UINT WriteAttribute30758Sequence; 
	UINT WriteAttribute30Sequence; 
	UINT CreateReadyReportSequence; 
	UINT CreateFaultReportSequence;
	UINT CreateReadControlParamDataAction;
	UINT sqConfigDataSearchAction_GasSol;
	
} PendingResponse_typ;


typedef struct Arc_Link_typ
{	
	RingBuffer *bufIN; 		// Incoming Messages RingBuffer pointer
	RingBuffer *bufOUT; 	// Outgoing Messages RingBuffer pointer	
	Arc_Link_Message_typ msgIN;		// Placeholder
	Arc_Link_Message_typ msgOUT;	// Placeholder
	Arc_Link_Internal_typ internal; // Internal
	Robot_typ robot; // Robot reference to this plc
	PendingResponse_typ PendingResponse; // pending response 
} Arc_Link_typ;

/* Prototypes */
void ArcLink_cyclic(Arc_Link_typ *arc);
void ArcLink_init(Arc_Link_typ *arc , RingBuffer *bufIN, RingBuffer *bufOUT , UDINT  pInMsg  , UDINT  pOutMsg , UDINT recLen );



//////////////////////////////////////////////////////////////////////
// ArcLink Event Codes 
// The following table lists the event codes that can be logged in Event Log postings
//////////////////////////////////////////////////////////////////////
#define	ARC_LINK_EVENT_11	11	/* 	Internal request failed to generate a response */
#define	ARC_LINK_EVENT_12	12	/* 	Internal attribute read/write failed */
#define	ARC_LINK_EVENT_13	13	/* 	Values/settings saved in the system could not be restored (corrupt)*/
#define	ARC_LINK_EVENT_14	14	/* 	Values/settings saved in the system could not be restored (primary section) */
#define	ARC_LINK_EVENT_15	15	/* 	Values/settings saved in the system could not be restored (secondary section) */
#define	ARC_LINK_EVENT_16	16	/* 	Values/settings that are saved could not be restored during powerup */
#define	ARC_LINK_EVENT_17	17	/* 	Serial port communication timed out*/
#define	ARC_LINK_EVENT_18	18	/* 	ArcLink auto-mapping failed*/
#define	ARC_LINK_EVENT_19	19	/* 	Excessive EEPROM saves detected for a certain attribute */
#define	ARC_LINK_EVENT_21	21	/* 	Device that controls sequence of the welding operation halted due to an error*/
#define	ARC_LINK_EVENT_22	22	/* 	Device that controls the sequence of the welding operation halted due to an error response when it tried to update a device's settings*/
#define	ARC_LINK_EVENT_23	23	/* 	Weld Sequencer could not change settings on a controlled object during welding*/
#define	ARC_LINK_EVENT_24	24	/* 	Error occurred while the weld sequencer was changing the weld mode*/
#define	ARC_LINK_EVENT_25	25	/* 	When updating non-state memory attributes via sequencer action, allow all but mode changing*/
#define	ARC_LINK_EVENT_26	26	/* 	Problem with paired mapping*/
#define	ARC_LINK_EVENT_27	27	/* 	Read/write request failed during paired mapping*/
#define	ARC_LINK_EVENT_28	28	/* 	Output locked out in the Weld Controller*/
#define	ARC_LINK_EVENT_29	29	/* 	Low-level DSP main control routine could not complete all of its tasks in time*/
#define	ARC_LINK_EVENT_31	31	/* 	Actual input current exceeded the input current limit*/
#define	ARC_LINK_EVENT_32	32	/* 	Capacitor bank A fell below the low voltage limit*/
#define	ARC_LINK_EVENT_33	33	/* 	Capacitor bank B fell below the low voltage limit*/
#define	ARC_LINK_EVENT_34	34	/* 	Capacitor bank A exceeded the high voltage limit*/
#define	ARC_LINK_EVENT_35	35	/* 	Capacitor bank B exceeded the high voltage limit*/
#define	ARC_LINK_EVENT_36	36	/* 	System detected a temperature level beyond the normal system operating limit*/
#define	ARC_LINK_EVENT_37	37	/* 	System failed to initialize inverter hardware during startup*/
#define	ARC_LINK_EVENT_38	38	/* 	Momentary AC dropout*/
#define	ARC_LINK_EVENT_39	39	/* 	Miscellaneous hardware fault occurred whose source cannot be determined*/
#define	ARC_LINK_EVENT_41	41	/* 	Long-term average secondary (weld) current limit has been exceeded*/
#define	ARC_LINK_EVENT_42	42	/* 	System detected a ground fault has occurred*/
#define	ARC_LINK_EVENT_43	43	/* 	Voltage difference between Capacitor bank A and B was greater than the allowable limit*/
#define	ARC_LINK_EVENT_44	44	/* 	Main processor in the system did not respond to another processor within the allotted time*/
#define	ARC_LINK_EVENT_45	45	/* 	Welding voltage was above limit*/
#define	ARC_LINK_EVENT_46	46	/* 	Output amperage limit exceeded*/
#define	ARC_LINK_EVENT_47	47	/* 	Miscellaneous hardware fault (capacitor fault or heart beat fault)*/
#define	ARC_LINK_EVENT_48	48	/* 	Miscellaneous hardware fault (main contactor open)*/
#define	ARC_LINK_EVENT_49	49	/* 	Single phase input operation has been detected*/
#define	ARC_LINK_EVENT_51	51	/* 	Weld Controller failed to set weld sequencer schedule memory*/
#define	ARC_LINK_EVENT_52	52	/* 	Digital Signal Processor (DSP) in the system is no longer responding to heartbeat request from the main processor*/
#define	ARC_LINK_EVENT_53	53	/* 	System detected the loss of a voltage sense lead*/
#define	ARC_LINK_EVENT_54	54	/* 	Long-term average secondary (weld) current limit has been exceeded*/
#define	ARC_LINK_EVENT_55	55	/* 	Communication with DSP failed*/
#define	ARC_LINK_EVENT_56	56	/* 	Communication with Chopper failed*/
#define	ARC_LINK_EVENT_57	57	/* 	Function Generator reported an error*/
#define	ARC_LINK_EVENT_58	58	/* 	Primary input power board not ready*/
#define	ARC_LINK_EVENT_59	59	/* 	Reduced input power detected*/
#define	ARC_LINK_EVENT_61	61	/* 	Step value for either upslope or downslope was calculated to zero*/
#define	ARC_LINK_EVENT_62	62	/* 	Weld Sequencer did not get a positive response after sending out Weld Cycle Data twice*/
#define	ARC_LINK_EVENT_63	63	/* 	Problem with submerged arc Slave device*/
#define	ARC_LINK_EVENT_71	71	/* 	Secondary over-power*/
#define	ARC_LINK_EVENT_72	72	/* 	Unsupported hardware revision*/
#define	ARC_LINK_EVENT_81	81	/* 	Feed head experienced a motor overload fault*/
#define	ARC_LINK_EVENT_82	82	/* 	Feed head experienced a motor over current fault*/
#define	ARC_LINK_EVENT_83	83	/* 	Wire drive shutdown bit 1 circuit open*/
#define	ARC_LINK_EVENT_84	84	/* 	Wire drive shutdown bit 2 circuit open (Water Fault)*/
#define	ARC_LINK_EVENT_85	85	/* 	Object could not find the Arc Control Object to send the I/O trigger command*/
#define	ARC_LINK_EVENT_86	86	/* 	Wire Drive input voltage too high*/
#define	ARC_LINK_EVENT_87	87	/* 	Wire Drive failed to find another object*/
#define	ARC_LINK_EVENT_88	88	/* 	Wire Drive could not send the trigger to the specified object in the event data.*/
#define	ARC_LINK_EVENT_89	89	/* 	Wire Drive failed to find the Production Monitoring object for profile selection based on digital inputs*/
#define	ARC_LINK_EVENT_91	91	/* 	Sense Lead object could not update the weld sequencer's or the weld controller's polarity attribute.*/
#define	ARC_LINK_EVENT_92	92	/* 	Remote shutdown of Weld Controller*/
#define	ARC_LINK_EVENT_95	95	/* 	Spool Gun shutdown due to motor drawing excessive current*/
#define	ARC_LINK_EVENT_96	96	/* 	Weld Controller failed to read WFS decimal places*/

//////////////////////////////////////////////////////////////////////
// ArcLink Error Codes 
// The following table lists the event codes that can be logged in Event Log postings
//////////////////////////////////////////////////////////////////////
#define	ARC_LINK_ERROR_0	0	/*	Success (no message error). */
#define	ARC_LINK_ERROR_1  	1	/*	Attribute not found. */
#define ARC_LINK_ERROR_2	2	/* 	Attribute value is read-only. */
#define ARC_LINK_ERROR_3	3 	/* 	Attribute Value (being written) is out of limits.*/
#define ARC_LINK_ERROR_4	4	/* 	Attribute Value (being written) is above high limit.*/
#define ARC_LINK_ERROR_5	5	/* 	Attribute Value (being written) is below low limit.*/
#define ARC_LINK_ERROR_8	8	/*	ArcLink object not found (bad ArcLink object address).*/
#define ARC_LINK_ERROR_9	9	/*	Attribute property specified is invalid.*/
#define ARC_LINK_ERROR_11	11	/*	Attribute property is not applicable (e.g. units code for Boolean type).  Only while reading.*/
#define ARC_LINK_ERROR_12	12	/*	Attribute property is not supported (e.g. invalid type id, or not applicable).  For reads or writes.*/
#define ARC_LINK_ERROR_13	13	/*	Attribute property is read-only.*/
#define ARC_LINK_ERROR_14	14	/*	Attribute is invalid.*/
#define ARC_LINK_ERROR_15	15	/*	ArcLink Object messages could not be relayed over Ethernet.*/
#define ARC_LINK_ERROR_16 	16	/*	ArcLink service busy (message should be re-sent).*/
#define ARC_LINK_ERROR_17	17	/*	ArcLink service busy (message should be re-sent).*/
#define ARC_LINK_ERROR_18	18	/*	ArcLink service busy (message should be re-sent).*/
#define ARC_LINK_ERROR_20	20	/*	ArcLink Block ID not found (for block read or write service).*/
#define ARC_LINK_ERROR_21	21	/*	ArcLink Action ID not found (for action service).*/
#define ARC_LINK_ERROR_22	22	/*	ArcLink Group Address not found.*/
#define ARC_LINK_ERROR_23	23	/*	ArcLink Event ID not found.*/
#define ARC_LINK_ERROR_24	24	/*	ArcLink Service not supported.*/
#define ARC_LINK_ERROR_25	25	/*	ArcLink Service not available.*/
#define ARC_LINK_ERROR_26	26	/*	Fragment buffer busy (message should be re-sent).*/
#define ARC_LINK_ERROR_27	27	/*	Fragment out of sequence (message should be re-started).*/
#define ARC_LINK_ERROR_28	28	/*	Fragment receive time out (message should be re-started).*/
#define ARC_LINK_ERROR_29	29	/*	Fragment message invalid.*/
#define ARC_LINK_ERROR_30	30	/*	Message request size is invalid.*/
#define ARC_LINK_ERROR_31	31	/*	Message response buffer was too small.*/
#define ARC_LINK_ERROR_33	33	/*	Message time out (message should be re-started).*/
#define ARC_LINK_ERROR_34	34	/*	Write failed.*/
#define ARC_LINK_ERROR_35	35	/*	Read failed.*/
#define	ARC_LINK_ERROR_36	36	/*	Invalid selection.*/
#define	ARC_LINK_ERROR_37	37	/*	ArcLink Object was busy (message should be re-sent). */


//////////////////////////////////////////////////////////////////////
// Attribute Value Property ID Codes
//////////////////////////////////////////////////////////////////////
/*
 * Value 0 Defined by Attribute Data Type
 */
#define PROPERTY_ID_VALUE 0

/**
 * Class 1 Unsigned Int (1)
 */
#define PROPERTY_ID_CLASS 1

/**
 * Saved In E2 2 Boolean
 */
#define PROPERTY_ID_SAVED_IN_E2 2

/**
 * Attribute Data Type 3 Unsigned Int (1)
 */
#define PROPERTY_ID_ATTRIBUTE_DATA_TYPE 3

/**
 * Unit Code 4 Unsigned Int (1)
 */
#define PROPERTY_ID_UNIT_CODE 4

/**
 * High Limit 5 Defined by Attribute Data Type
 */
#define PROPERTY_ID_HIGH_LIMIT 5

/**
 * Low Limit 6 Defined by Attribute Data Type
 */
#define PROPERTY_ID_LOW_LIMIT 6

/**
 * Decimal Places 7 Unsigned Int (1)
 */
#define PROPERTY_ID_DECIMAL_PLACES 7

/**
 * Name 8 String (1-254)
 */
#define PROPERTY_ID_NAME 8

//////////////////////////////////////////////////////////////////////
// Attribute Data Type Property, ID Codes
//////////////////////////////////////////////////////////////////////
/**
 * <b>Boolean</b>:<br>
 * <i>size: 1 byte</i>
 */
#define DATA_TYPE_BOOLEAN 0

/**
 * <b>Enumerated 1</b>:<br>
 * <i>size: 1 byte</i>
 */
#define DATA_TYPE_ENUMERATED_1 1

/**
 * <b>Enumerated 2</b>:<br>
 * <i>size: 2 bytes</i>
 */
#define DATA_TYPE_ENUMERATED_2 2

/**
 * <b>Enumerated 4</b>:<br>
 * <i>size: 4 bytes</i>
 */
#define DATA_TYPE_ENUMERATED_4 3

/**
 * <b>Bit Mapped 1</b>:<br>
 * <i>size: 1 byte</i>
 */
#define DATA_TYPE_BIT_MAPPED_1 4

/**
 * <b>Bit Mapped 2</b>:<br>
 * <i>size: 2 bytes</i>
 */
#define DATA_TYPE_BIT_MAPPED_2 5

/**
 * <b>Bit Mapped 4</b>:<br>
 * <i>size: 4 bytes</i>
 */
#define DATA_TYPE_BIT_MAPPED_4 6

/**
 * <b>Signed Integer 1</b>:<br>
 * <i>size: 1 byte</i>
 */
#define DATA_TYPE_SIGNED_INTEGER_1 7

/**
 * <b>Signed Integer 2</b>:<br>
 * <i>size: 2 bytes</i>
 */
#define DATA_TYPE_SIGNED_INTEGER_2 8

/**
 * <b>Signed Integer 4</b>:<br>
 * <i>size: 4 bytes</i>
 */
#define DATA_TYPE_SIGNED_INTEGER_4 9

/**
 * <b>Unsigned Integer 1</b>:<br>
 * <i>size: 1 byte</i>
 */
#define DATA_TYPE_UNSIGNED_INTEGER_1 10

/**
 * <b>Unsigned Integer 2</b>:<br>
 * <i>size: 2 bytes</i>
 */
#define DATA_TYPE_UNSIGNED_INTEGER_2 11

/**
 * <b>Unsigned Integer 4</b>:<br>
 * <i>size: 4 bytes</i>
 */
#define DATA_TYPE_UNSIGNED_INTEGER_4 12

/**
 * <b>Floating Point</b>:<br>
 * <i>size: 4 bytes</i>
 */
#define DATA_TYPE_FLOATING_POINT 13

/**
 * <b>ASCII String </b>:<br>
 * <i>size: (0 - 254) bytes</i>
 */
#define DATA_TYPE_ASCII_STRING 14

//////////////////////////////////////////////////////////////////////
// Attribute Class Codes
//////////////////////////////////////////////////////////////////////
/**
 * <b>Network Configuration</b>:<br>
 * Inputs related to the network. E.g. Group Assignment <br>
 * 
 */
#define CLASS_NETWORK_CONFIGURATION 0

/**
 * <b>Network Status</b>:<br>
 * Outputs describing information related to the network. E.g. Network
 * Version Number <br>
 * <i>Read Only</i>
 */
#define CLASS_NETWORK_STATUS 1

/**
 * <b>Command</b>:<br>
 * Inputs used to activate an object?s behavior. Changes generally have an
 * immediate effect. E.g. Gas Purge<br>
 * 
 */
#define CLASS_COMMAND 2

/**
 * <b>Reference</b>:<br>
 * Inputs which are set points. Changes generally have an immediate effect.
 * E.g. Cold Inch<br>
 * 
 */
#define CLASS_REFERENCE 3

/**
 * <b>Reference</b>:<br>
 * Inputs which alter the object?s general behavior. E.g. Motor Control-Loop
 * Mode<br>
 * 
 */
#define CLASS_CONFIGURATION 4

/**
 * <b>Signal</b>:<br>
 * Outputs used for metering and monitoring. E.g. Actual Arc Current <br>
 * <i>Read Only</i>
 */
#define CLASS_SIGNAL 5

/**
 * <b>Status</b>:<br>
 * Outputs used for metering and monitoring. E.g. Faults<br>
 * <i>Read Only</i>
 */
#define CLASS_STATUS 6

/**
 * <b>Sequence Controlled Command</b>:<br>
 * Command to be controlled by the Sequencer object (object class 0). E.g.
 * Motor On/Off<br>
 */
#define CLASS_SEQUENCE_CONTROLLED_COMMAND 7

/**
 * <b>Sequence Controlled Reference</b>:<br>
 * Reference to be controlled by the Sequencer object (object class 0). E.g.
 * Commanded WFS<br>
 */
#define CLASS_SEQUENCE_CONTROLLED_REFERENCE 8

/**
 * <b>Sequence Controlled Configuration</b>:<br>
 * Configuration to be controlled by the Sequencer object (object class 0).
 * E.g. Weld Procedure <br>
 */
#define CLASS_SEQUENCE_CONTROLLED_CONFIGURATION 9

/**
 * <b>Command with State Memory</b>:<br>
 * Command which has state memory. The sequencer object creates attributes
 * of this class from attributes of class 7. The state portion of the
 * service ID is relevant for the read and write services.<br>
 */
#define CLASS_COMMAND_WITH_STATE_MEMORY 10

/**
 * <b>Reference with State Memory</b>:<br>
 * Reference which has state memory. The sequencer object creates attributes
 * of this class from attributes of class 8. The state portion of the
 * service ID is relevant for the read and write services.<br>
 */
#define CLASS_REFERENCE_WITH_STATE_MEMORY 11

/**
 * <b>Debug</b>:<br>
 * Used (primarily) during manufacturing.
 */
#define CLASS_DEBUG 12

/**
 * <b>Setup</b>:<br>
 */
#define CLASS_SETUP 13

/**
 * <b>Version</b>:<br>
 * Version related information.<br>
 * <i>Read Only</i>
 */
#define CLASS_VERSION 14


//////////////////////////////////////////////////////////////////////
// Addresing mode
//////////////////////////////////////////////////////////////////////
#define ADDRESS_MODE_MASTER_SLAVE	 0
#define ADDRESS_MODE_MASTER_SLAVES	 1
#define ADDRESS_MODE_SLAVE_MASTER	 4
#define ADDRESS_MODE_PEER_PEER		 6
#define ADDRESS_MODE_PEER_PEERS 	 7

//////////////////////////////////////////////////////////////////////
// Network Services Codes
//////////////////////////////////////////////////////////////////////
/**
 * <b>Attribute Read</b>:<br>
 */
#define NETWORK_SERVICE_ATTRIBUTE_READ 0

/**
 * <b>Attribute Write</b>:<br>
 */
#define NETWORK_SERVICE_ATTRIBUTE_WRITE 1

/**
 * <b>Confirmed Event Report</b>:<br>
 */
#define NETWORK_SERVICE_CONFIRMED_EVENT_REPORT 2

/**
 * <b>Unconfirmed Event Report</b>:<br>
 */
#define NETWORK_SERVICE_UNCONFIRMED_EVENT_REPORT 3

/**
 * <b>Action</b>:<br>
 */
#define NETWORK_SERVICE_ACTION 4

/**
 * <b>Read Block</b>:<br>
 */
#define NETWORK_SERVICE_READ_BLOCK 5

/**
 * <b>Write Block</b>:<br>
 */
#define NETWORK_SERVICE_WRITE_BLOCK 6

/**
 * <b>Confirmed Event Block Report</b>:<br>
 */
#define NETWORK_SERVICE_CONFIRMED_EVENT_BLOCK_REPORT 7

/**
 * <b>Unconfirmed Event Block Report</b>:<br>
 */
#define NETWORK_SERVICE_UNCONFIRMED_EVENT_BLOCK_REPORT 8


//#include <..\..\Lib\arclink\Handler.h>


#endif /* _ARCLINK_H */
