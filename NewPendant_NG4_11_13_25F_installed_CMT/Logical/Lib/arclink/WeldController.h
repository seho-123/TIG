#ifndef _WELD_CONTROLLER_H
#define _WELD_CONTROLLER_H

#include <bur/plctypes.h>

#include <runtime.h>

#define WELD_CONTROLLER_NETWORK_ADDRESS 1

//////////////////////////////////////////////////////////////////////
// Attributes
//////////////////////////////////////////////////////////////////////
/*
 * |Attribute Number|Name|Description|Value Semantics|Required|Class|Saved
 * in EEPROM|Type|Decimal Places|Units Description|
 */
/*
 * 
 * Create Ready Status Change Event for WC object Action 11 for attribute 13
 * 
 * 
 */
#define ATTRIBUTE_READY_STATUS_CHANGE_EVENT 13

/*
 * 
 * Create Fault Status Change Event for WC object Action 11 for attribute 14
 * 
 * 
 * 
 */
#define ATTRIBUTE_FAULT_STATUS_CHANGE_EVENT 14



#define ATTRIBUTE_COOLER_COMMAND 513

#define ATTRIBUTE_GAS_ENABLE_COMMAND 30725

#define ATTRIBUTE_GAS_ON_OFF_COMMAND 30723

/**
 * <pre>
 512	Output Command	Controls the welding output	0: Output Off
1: Output On	Y	command, sequence controlled		BOOL
N/A	none
 * </pre>
 */
#define ATTRIBUTE_OUTPUT_COMMAND 512

/**
 * <pre>
513	Weld Mode	Selects a weld mode	none	Y	configuration, sequence controlled		UINT2
0	none
 * </pre>
 */
#define ATTRIBUTE_WELD_MODE 513

/**
 * <pre>
517	Workpoint	Workpoint	none	Y	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_WORKPOINT 517

/**
 * <pre>
518	Arc Length Trim	Trims the output level reference	none	N	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_ARC_LENGTH_TRIM 518

/**
 * <pre>
519	Wave Control 1	Miscellaneous trim 1 setting	none	N	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_WAVE_CONTROL_1 519

/**
 * <pre>
520	Wave Control 2	Miscellaneous trim 2 setting	none	N	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_WAVE_CONTROL_2 520

/**
 * <pre>
521	Wave Control 3	Miscellaneous trim 3 setting	none	N	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_WAVE_CONTROL_3 521

/**
 * <pre>
522	Wave Control 4	Miscellaneous trim 4 setting	none	N	reference, sequence controlled		SINT2
	not specified
 * 
 * </pre>
 */
#define ATTRIBUTE_WAVE_CONTROL_4 522

/**
 * 515|Arc Voltage| Average arc voltage| none| Y |signal| SINT2 |1| volts|
 * 
 */
#define ATTRIBUTE_ARC_VOLTAGE 515

/*
 * 
 * 514|Arc Current|Average arc current|none|Y|signal|SINT2|1|amperes|
 * 
 */
#define ATTRIBUTE_ARC_CURRENT 514

/**
 * 30754|Weld Set Filename|Returns the ASCII filename of the weld set
 * resident in FLASH|String|N|Version Info|N|ASCII|N/A|none
 * 
 */
#define ATTRIBUTE_WELD_SET_FILENAME 30754

/**
 * <pre>
Weld-Controller High Speed Block Meter Event
Event ID	520
Description	Provides arc statistics at a higher rate than the Block Meter Event. This event is automatically broadcast to all members of Group 128. The event period is configurable through the use of the High Speed Meter Event Period attribute.
 * </pre>
 */
#define HIGH_SPEED_BLOCK_METER_EVENT 520

/**
 * Disable auto-stick check in WC object Write attribute 30770 to 1
 */
#define ATTRIBUTE_DISABLE_AUTO_STICK_CHECK 30770

/**
 * <pre>
 Setting	Object	Attribute	Description
HS Meter Frequency	WC	30781	Sets the broadcast period (frequency) of Event 520.  Increments of milliseconds.
HS Meter Filter	WC	30758	Sets the averaging period for current and voltage (send in event 520).  Increments of milliseconds.
 * </pre>
 */
#define ATTRIBUTE_HS_METER_FREQUENCY 30781

/**
 * <pre>
 Setting	Object	Attribute	Description
HS Meter Frequency	WC	30781	Sets the broadcast period (frequency) of Event 520.  Increments of milliseconds.
HS Meter Filter	WC	30758	Sets the averaging period for current and voltage (send in event 520).  Increments of milliseconds.
 * </pre>
 */
#define ATTRIBUTE_HS_FILTER 30758


#define ACTION_CONFIG_DATA_SEARCH 10  //Configuration Data Search ACTION   



#define ATTRIBUTE_FAULT 30731
/**
 * <pre>
 * Weld Table Search Action
Action ID	512
Description	This action searches the welding procedure database for Weld Modes that match the search criteria.  N represents the number of successful ‘hits’ to the search.  N<= Maximum Number of Matches.
Required	Yes
Request	The Mode Descriptor Care field has the same structure as the Mode Descriptor and provides the ability to search on contents of the Mode Descriptor.

Fields that will be matched are determined by the Search Mask

The maximum number of matches must be less than 127.  This field exists for clients that do not have memory resources to buffer all 127 possible modes (each a short).

If the maximum number of modes (specified in the maximum request parameter) is received then a subsequent call(s) can be made to obtain the remaining.  Example: first call has max number of modes set to 100.  If the search response contains 100 modes, with the last mode of the 100180, then a subsequent call could be made with starting weld mode index set to 181.
4 Bytes	Mode Descriptor- (see Table 7 7 Mode Descriptor Definitions)

4 Bytes	Mode Descriptor Care a “1” in the corresponding bit to include that mode descriptor bit in the search.
2 Bytes	Starting Weld Mode Index.  
1 Byte	Maximum Number of Matches
1 Byte	Process Type- (see Table 7 4 Process Type Definitions)

1 Byte	Wire Type- (see Table 7 3 Wire Type Definitions)

1 Byte	Wire Size- (see Table 7 2 Wire Size Definitions) 

1 Byte	Gas Type- (see Table 7 5 Gas Type Definitions)

1 Byte	Search Mask- (see Table 21 2- Weld Table Search Mask Bit Definitions) a”1” in the corresponding bit to include that request field in the search.
Response 	The response is an array of 0 to maximum matches to be returned that were found to match the search criteria.
2 Bytes * N	Weld Mode Number
Action Specific
Return Code	
None	
Table 21 1 WC Weld Table Search Action (512)

Search Mask
Bit 7	Reserved
Bit 6	Reserved
Bit 5	Reserved
Bit 4	Mode Descriptor
Bit 3	Gas Type
Bit 2	Wire Size
Bit 1	Wire Type
Bit 0	Process Type
Table 21 2- Weld Table Search Mask Bit Definitions
 * </pre>
 */
#define WELD_TABLE_SEARCH_ACTION 512

/**
 * <pre>
 * Read Procedure Data Action

Action ID	513
Description	Read information concerning the Weld Mode in the request
Required	Yes
Request	
2 Bytes	Weld Mode
Response 	
4 Bytes	Mode Descriptor- (see Table 7 7 Mode Descriptor Definitions)

2 Bytes	Process Type- (see Table 7 4 Process Type Definitions)

2 Bytes	work-point Low Limit
2 Bytes	work-point High Limit
2 Bytes	Reserved
1 Byte	Wire Type- (see Table 7 3 Wire Type Definitions)

1 Byte	Wire Size- (see Table 7 2 Wire Size Definitions)

1 Byte	Gas Type- (see Table 7 5 Gas Type Definitions)

17 Bytes	Process Name (16 Characters + Null Terminator)
17 Bytes	Process Description (16 Characters + Null Terminator)
17 Bytes	Gas Description (16 Characters + Null Terminator)

Action Specific
Return Code	
128	Invalid Mode
Table 21 3 WC Read Procedure data Action (513)
 * </pre>
 */
#define READ_PROCEDURE_DATA_ACTION 513

/**
 * <pre>
 Read Mode Data Action
Action ID	514
Description	Read specific table information concerning the controls that are utilized by the Weld Mode passed in the request.
Required	Yes
Request	
2 Bytes	Weld Mode
Response 	The Boolean ‘Attached’ parameter indicates that for this Weld Mode, the control parameter is dependent on the value of work-point

The Synergic Records Count (N) determines the number of synergic record structures returned
4 Bytes	Mode Descriptor- (see Table 7 7 Mode Descriptor Definitions)

4 Bytes	Attribute Relevance Mask- (see Attribute Relevance)

2 Bytes	Function Attribute Relevance Mask- (see Table 7 9- Function Output Relevance Mask Bit Definitions)

2 Bytes	Process Type- (see Table 7 4 Process Type Definitions)

2 Bytes	Synergic Records Count- N
1 Byte	Trim is Work-point Dependent
1 Byte	Wave-Control 1 Work-point Dependent
1 Byte	Wave-Control 2 Work-point Dependent
1 Byte	Wave-Control 3 Work-point Dependent
1 Byte	Wave-Control 4 Work-point Dependent
1 Byte	Align Byte (No Data)
N*8 Bytes	2 Bytes	Synergic Record Attribute ID
2 Bytes	Synergic Record Attribute State Mask (not applicable for ArcLink/XT)
2 Bytes	Synergic Record Target Class
2 Bytes	Synergic Record Target Instance
Action Specific Return Code	
128	Invalid Mode
Table 21 4 WC Read Mode Data Action (514)


Mode Descriptor
Bit 15	Voltage Sense  From Vsense Circuit
Bit 14	Voltage Sense From Studs
Bit 13	Reserved
Bit 12	Reserved
Bit 11	Non Synergic STT
Bit 10	AC Procedure
Bit 9	High Frequency
Bit 8	STT Procedure
Bit 7	Test Procedure
Bit 6	Reserved
Bit 5	Reserved
Bit 4	Reserved
Bit 3	Pulse Procedure
Bit 2	Gas Required
Bit 1	Wire Required
Bit 0	Synergic Wire Feed Speed
Table 7 7 Mode Descriptor Definitions


Function Attribute Relevance Mask
Bits 4-15	Reserved
Bit 3	Function Output 4 Relevance
Bit 2	Function Output 3 Relevance
Bit 1	Function Output 2 Relevance
Bit 0	Function Output 1 Relevance
Table 7 9- Function Output Relevance Mask Bit Definitions

General Process Type
0 SMAW	Shielded Metal Arc Welding
1 FCAW-S	Self Shielded Flux Cored Arc Welding 
2 GTAW	Gas Tungsten Arc Welding (pulsed and non-pulsed)
3 GMAW	Gas Metal Arc Welding (pulsed and short circuit)
4 GOUGE	Arc Gouging
5 PAW	Plasma Arc Welding
6 SAW	Submerged Arc Welding (series and non-series)
7 TSENSE	Touch Sensing
8 FCAW	Flux Cored Arc Welding (self and gas shielded)
9 FCAW-G	Gas  Shielded Flux Cored Arc Welding
10 GTAW-P	Pulsed Gas Tungsten Arc Welding
11 GMAW-P	Pulsed Gas Metal Arc Welding
12 GMAW-S	Short Circuit Gas Metal Arc Welding
13 SAW-S	Series Submerged Arc Welding
Table 7 4 Process Type Definitions
 * 
 * 
 * </pre>
 */
#define READ_MODE_DATA_ACTION 514

/**
 * <pre>
 * Read Control Parameter Info Action
Action ID	515
Description	Returns details about a WC control parameter for a given Weld Mode
Required	Yes
Request	The Attribute ID may be any of the WC’s control parameters (work-point, trim, wave-controls 1-4)
2 Bytes	Attribute ID
2 Bytes	Weld Mode
Response 	The maximum/minimum values provide the largest possible range of the control parameter for the given Weld Mode 
2 Bytes	Maximum Value
2 Bytes	Minimum Value
1 Byte	Units Code
1 Byte	Decimal Places
17 Bytes	Name (16 Characters + Null Terminator)
Return Code	
128	Invalid Mode
129	Non Relevant Trim
130	Invalid Attribute ID
Table 21 5 WC Read Control Parameter Data Action (515)
 * 
 * </pre>
 * 
 */
#define READ_CONTROL_PARAMETER_DATA_ACTION 515
	
/**
 * <pre>
 * 	
	Set Controls Action
	Action ID	520
	Description	Sets mode and from 1 to 6 of the WC’s process inputs: work-point, trim, Wave-Control 1, 2, 3 and 4.
	  
	This is a variable length request.  The size is based on the parameter mask field which indicates which of the other request fields are present.

	Returns synergic wire feed speed.  If parameter mask bit 7 (most significant) is set then the WC will proxy the synergic wire feed speed to the drive.

	If action response is success, then the response message may contain limit errors (if any) (see Table 23 1 Common ArcLink Message Error Response Codes) for the individual process parameters (e.g. Trim too low).  In this case the Return Code Mask will have a bit set (for each parameter out of limit).  Thus the response size (for success) may vary in size.
	Required	Yes
	Request	
	1 Byte	Parameter Mask
	2 Bytes	Weld Mode 	If parameter mask bit 0 set.
	2 Bytes	Work-point	If parameter mask bit 1 set.
	2 Bytes	Trim	If parameter mask bit 2 set.
	2 Bytes	Wave-Control 1	If parameter mask bit 3 set.
	2 Bytes	Wave-Control 2	If parameter mask bit 4 set.
	2 Bytes	Wave-Control 3	If parameter mask bit 5 set.
	2 Bytes	Wave-Control 4	If parameter mask bit 6 set.
	Response 	
	2 Bytes	Synergic Wire Feed Speed
	1 Byte	Return Code Mask
	1 Byte	Mode Error Return Code	If mask bit 0 set.
	1 Byte	Work-point Error Return Code	If mask bit 1 set.
	1 Byte	Trim Error Return Code	If mask bit 2 set.
	1 Byte	Wave-Control 1 Error Return Code	If mask bit 3 set.
	1 Byte	Wave-Control 2 Error Return Code	If mask bit 4 set.
	1 Byte	Wave-Control 3 Error Return Code	If mask bit 5 set.
	1 Byte	Wave-Control 4 Error Return Code	If mask bit 6 set.
	Action Specific Return Code	
	128	Mode does not exist
	129	Mode is non-synergic (and parameter mask had bit 7 set)
	130	Invalid Work-point
	131	Problem writing synergic wfs to drive
	132	WD not mapped
	133	Too many wire drives mapped
	Table 21 7 Set Controls Action (520)
 * </pre>
 */


#define WC_READ_CONTROL_PARAMETER_DATA_ACTION 515  // Table 21 5 -  WC Read Control Parameter Data Action (515)

#define SET_CONTROLS_ACTION 520



#endif /* _WELD_CONTROLLER_H */