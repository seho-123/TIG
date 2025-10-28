#ifndef _NETWORK_ACTIONS_H
#define _NETWORK_ACTIONS_H


/* Network Actions */
/**
 * <pre>
Recognition Action
Action ID	0
Description	Used for new module recognition.  An object that has already been mapped by the bus master must not respond.
Required	Yes
Request	
No Data	
Response 	Response includes information unique to that module.
2 Bytes	Vendor ID Number
2 Bytes	Serial Number
1 Byte	Number of object?s (includes EO0)
Action Specific
Return Code	
128	Recognition request invalid
 * 
 * 
 * 
 * </pre>
 * 
 */
#define NETWORK_RECOGNITION_ACTION 0

/**
 * <pre>
Assign Network Address Action
Action ID	1
Description	Assign a network address to all object?s in a node
Required	Yes
Request	
2 Bytes	Vendor ID Number
2 Bytes 	Serial Number
1 Byte	Network Address of  EO0 
Response 	Only objects with matching vendor ID and Serial Number should process and respond to this request.
No Data	
Action Specific
Return Code	
128	Network address assignment invalid
 * 
 * 
 * </pre>
 */
#define NETWORK_ASSIGN_NETWORK_ADDRESS_ACTION 1

/**
 * <pre>
Group Data
Action ID	2
Required	Yes
Description 	Sent by the communications master to each embedded object (prior to objects being made bus available).  Contains information about every object in the same group. 
Request	
1 Byte	Group 
1 Byte	Object Instance
1 Byte	Group Mask
1 Byte	Object Network Address
2 Bytes	Object Class
Response	No Data.
 * 
 * </pre>
 */
#define NETWORK_GROUP_DATA_ACTION 2

/**
 * <pre>
Add Group Address                                                                                                                
Action ID	5
Required	Yes
Description 	Adds a group address to another object. 
Request	The group addresses being added.
1 Byte	Group Address to Add
Response	No data.
Response Codes	
128	Maximum number of groups exceeded.
131	Reserved group address.
Table 18 4 Add to Group Address Action (5)
 * </pre>
 */
#define NETWORK_ADD_TO_GROUP_ADDRESS_ACTION 5

/**
 * <pre>
Attribute Search                                                                                                                      
Action ID	8
Required	Yes
Description	Returns the attribute ID numbers (if any) of all attributes that meet the search parameters in the request.  Search parameters are a list of attribute properties ANDed together. Up to the specified maximum will be returned.
Request	The criteria that determine which (if any) attributes will be returned by the search.  Also includes the attribute number to begin searching from an the maximum number of attribute Ids to return.
2 Bytes 	Attribute Id Number That Search Will Start at
1 Byte 	Search Parameter Mask
1 Byte 	Attribute Class
1 Byte 	Attribute Type
1 Byte 	Saved in E2 
1 Byte	Maximum
Response	An array of 0 or more (up to maximum) attribute Ids that were found to match the criteria.
2 Bytes * N	Attribute Number or Numbers Matching Search Criteria
Table 18 5 Attribute Search Action (8)

Search Parameter Mask for Attribute Search
BIT	SEARCH PARAMETER	VALUE
3-7	Not Used	Don?t Care
2	Attribute Class	0=Don?t Care  1=Care
1	Attribute Type	0=Don?t Care  1=Care
0	Saved in E2 	0=Don?t Care  1=Care
 * 
 * 
 * 
 * </pre>
 * 
 * 
 */
#define NETWORK_ATTRIBUTE_SEARCH_ACTION 8

/**
 * <pre>
 * Configuration Data Search                                                                                                     
Action ID	10
Required	Yes
Description	Returns the network addresses (if any) of objects that meet the search parameters.   Search parameters are ANDed together. Up to the specified maximum will be returned.
Request	
1 Byte	Search Parameter Mask
1 Byte	Group Assignment
2 Bytes	Object Class
1 Byte	Object Instance
1 Byte	Object Class Type
1 Byte	Maximum
1 Byte	Starting Network Address
Response	
1 Byte * N	Network Address or Addresses
Table 18 6 Configuration Data Search Action (10)

Search Parameter Mask for Configuration Data Search
BIT	SEARCH PARAMETER	VALUE
7-4	Not Used	Don?t Care
3	Group Assignment	0=Don?t Care  1=Care
2	Object Class	0=Don?t Care  1=Care
1	Object Instance	0=Don?t Care  1=Care
0	Object Class Type	0=Don?t Care  1=Care
 * 
 * </pre>
 */
#define NETWORK_CONFIGURATION_DATA_SEARCH_ACTION 10

/**
 * <pre>
Create Event Report                                                                                                                
Action ID	11
Required	No
Description	Creates an event report of an attribute based on a trigger.  The trigger is either a periodic rate, a value/range comparison or a change of state of the specified attribute.  When triggered the same attribute?s value is sent out as an event.
Request	
2 Bytes	Attribute ID Number
1 Byte	Sequence State  (only relevant for sequence attributes)
1 Byte	Property ID Number
1 Byte	Trigger Options
1 Byte	Reserved
N Data Bytes	If periodic option, then period
If compare value, then comparison value
If range compare, then low limit, high limit
If change of value or state, then not used
Response	No Data.
Response Codes	
128	Maximum number of allowable events exceeded.
 * 
 * 
 * </pre>
 */
#define NETWORK_CREATE_EVENT_REPORT_ACTION 11

/**
 * <pre>
Get Next Group Address                                                                                                         
Action ID	15
Required	Yes
Description 	Return the next available group address from the master.
Request	No data.
Response	
1 Byte	Next Available Group Address
Table 18 9 Get Next Group Address Action (15)
 * </pre>
 */
#define NETWORK_GET_NEXT_GROUP_ADDRESS_ACTION 15

/**
 * <pre>
Get Next Block ID Number                                                                                                     
Action ID	16
Required	Yes
Description 	Return the next available block ID from the master.
Request	No data.
Response	
2 Bytes	Next Available Block ID Number
Table 18 10 Get Next Block ID Action (16)
 * </pre>
 */
#define NETWORK_GET_NEXT_BLOCK_ID_NUMBER_ACTION 16

/**
 * <pre>
NOP 
Action ID	19
Required	Yes
Description 	Requires no action by object receiving the request other than the normal positive or negative responses.
Request	
N bytes of data to be echoed. 	
Response	
N bytes of data echoed. 	
Table 18 11 NOP Action (19)
 * </pre>
 */
#define NETWORK_NOP_ACTION 19

/**
 * <pre>
Create Block Number Format                                                                                                 
Action ID	20
Required	Yes
Description	Create a format to exchange a block of data.  
Request	
2 Bytes 	Block ID

repeated for each 
attribute in the block
	2 Bytes 	Attribute ID Number
	1 Byte	Sequence State  (only relevant for sequence attributes)
	1 Byte   	Property ID Number
	1 Byte	Bit offset in the specified byte of the specified attribute property value. (only relevant for booleans)
	1 Byte	Byte offset in the block of the specified attribute property value.
	Attribute ID ...	
Response	No Data.
Response Codes	
128	Maximum number of blocks exceeded.
129	Block already defined.
130	Data overlap.
131	Block ID is reserved.
Table 18 12 Create Block Format Action (20)
 * 
 * </pre>
 */
#define NETWORK_CREATE_BLOCK_NUMBER_FORMAT_ACTION 20

/**
 * <pre>
 * Get Configuration Object Controls
Action ID	32
Required	No
Description 	For configuration control type objects only.   This action allows the Weld Sequencer to determine which sequence controlled commands and references (see Table 14 2- Attribute Class Codes) the configuration object has the ability to control.

This action is not relevant for robotic applications (since they don?t use or control the weld sequencer).
Request	
2 Bytes	Controlled object class.
1 Byte	Controlled object group.
1 Byte	Controlled object instance.
1 Byte	Weld Sequencer Schedule (A or B)
1 Byte	Unused.
N x 2 Bytes	Controlled object attributes that could be controlled.
Response	
N x 2 Bytes	Control mask (bit 0 corresponds to sequencer state 0) for each of the sequence controlled attributes (passed in request) that the configuration device can control.

For robotic applications a no data positive response should be returned.
Table 18 14 Get Configuration Object Controls
 * 
 * 
 * 
 * </pre>
 */
#define NETWORK_GET_CONFIGURATION_OBJECT_CONTROLS_ACTION 32

/* High Speed Block Meter Unconfirmed Event */
#define NETWORK_HIGH_SPEED_BLOCK_METER_EVENT_ACTION 520

/* Energy FB Unconfirmed Event*/
#define NETWORK_ENERGY_FB_EVENT_ACTION 516 

/* Block Meter Unconfirmed Event  */
#define NETWORK_BLOCK_METER_EVENT_ACTION 512 

/* Weld Score Unconfirmed Event  */
#define NETWORK_WELD_SCORE_EVENT_ACTION 518 

/* Arc Status Unconfirmed Event */
#define ARC_STATUS_CHANGE_EVENT 514

/* Ready, Fault Status Event */
#define NETWORK_ATTRIBUTE_PROPERTY_EVENT   0
  /* Atrributes for Event 0 */
#define READY_STATUS_EVENT 13
#define FAULT_STATUS_EVENT 14

#endif /* _NETWORK_ACTIONS_H */