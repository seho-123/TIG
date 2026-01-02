#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <bur/plctypes.h>

#include <runtime.h>

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

#define SENSE_LEAD_ARCLINK_NETWORK_ADDRESS 3; // Find out which
														// address it is

/**
	 * <pre>
	 Close Electrode Remote Voltage Lead for SL object
	Write attribute 512 to 1
	 * 
	 * </pre>
	 */

#define ATTRIBUTE_REMOTE_VOLTAGE_LEAD  512;

#define BUS_MASTER_ARCLINK_NETWORK_ADDRESS 0; // Find out which
// address it is

/**
 * <pre>
Close Electrode Remote Voltage Lead for SL object
Write attribute 512 to 1
 * 
 * </pre>
 */
#define ATTRIBUTE_LINK_TIMEOUT_PERIOD 30;

#endif /* _ATTRIBUTE_H */
