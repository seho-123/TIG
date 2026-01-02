#ifndef _NETWORK_ATTRIBUTE_H
#define _NETWORK_ATTRIBUTE_H



/* Attributes Numbers */
#define VENDOR_ID 0
#define VENDOR_NAME 1
#define MODULE_MODEL_NUMBER 2
#define MODULE_MODEL_NAME 3
#define CLASS_ID 4
#define CLASS_NAME 5
#define CLASS_REVISION 6
#define CLASS_TYPE 7
#define GROUP_SWITCH_SETTINGS 8
#define INSTANCE_SWITCH_SETTINGS 9
#define GROUP_ASSIGNMENTS 10
#define INSTANCE_ASSIGNMENTS 11
#define ARCLINK_REVISION 12
#define READY_STATUS 13
#define FAULT_STATUS 14
#define SILICON_SERIAL_NUMBER 16
#define NUMBER_OF_EMBEDDED_OBJECTS 17
#define EMBEDDED_OBJECT_NUMBER 18
#define SOFTWARE_VERSION 19
#define HARDWARE_REVISION 20
#define DIAGNOSTIC_STATUS 21
#define RESET 22
#define BUS_AVAILABLE 23
#define BOOTSTRAP 24
#define OBJECT_INDICATOR 25
#define NUMBER_OF_TIMES_MAPPED 26
#define MODULE_BASE_ADDRESS 27
#define NUMBER_OF_OBJECTS_MAPPED 28
#define HB_DISABLE 29
#define PAIRED_ITEMS_COUNT 50

/**
 * <pre>
 Attribute Number	Name	Description	Value Semantics	Required	Class	Saved in EEPROM	Type	Decimal Places	Units Description
0	Vendor ID	The vendor ID of the object	None	Y	Network status		UINT2	0	None
1	Vendor Name	The vendor name of the object	None	N	Network status		String	N/A	None
2	Module Model Number	The module model number	None	Y	Network status		String	N/A	None
3	Module Model Name	The module model name	None	N	Network status		String	N/A	None
4	Class ID	The class ID of the object	None	Y	Network status		UINT2	0	None
5	Class Name	The class name of the object	None	N	Network status		String	N/A	None
6	Class Revision	The class revision of the object	None	Y	Network status		UINT2	0	None
7	Class Type	The class type of the object.  The recommended class type for a robot is 2 for ?configuration? type. bit 0: controlled bit 1: config Y	Network status BMP1	N/A	None
8	Group Switch Settings	The hardware settings that define the object's welding group	bit 0: group 1 bit 2: group 3
bit 1: group 2 bit 3: group 4	Y	Network status		BMP1
	N/A	None
9	Instance Switch Settings	The hardware settings that define the object's instance number	None	Y	Network status		UINT1
	0	None
10	Group Assignments	The assigned settings define the object's welding group.  The assignment may be sourced by hardware settings.	bit 0: group 1 bit 2: group 3
bit 1: group 2 bit 3: group 4	Y	Network config	N	BMP1
	N/A	None
11	Instance Assignments	The assigned settings that define the object's instance number.  The assignment may be sourced by hardware settings.	None	Y	Network config	N	UINT1	0	None
12	Arclink Revision	The Arclink revision that the object was created for	None	Y	Network status		UINT2	0	None
13	Ready Status	Indicates that the object is ready to function in the welding context	bits 0-6: vendor specific code
bit 7: ready	Y	Network status	N	BMP1
	N/A	None
14	Fault Status	Indicates that the object can not or has failed to properly function in the welding context	bits 0-6: vendor specific code
bit 7: faulted	Y	Network status	N	BMP1
	N/A	None
16	Silicon Serial Number	Unique serial number assigned to each module	None	Y	Network status		UINT2		None
17	Number of Embedded Objects	The number of embedded objects in the module	None	Y	Network status	N	UINT1	0	None
18	Embedded Object Number	The embedded object offset within the module	None	Y	Network status	N	UINT1	0	None
19	Software Version	The vendor specific software revision of the object	None	Y	Network status		String	N/A	None
20	Hardware Revision	The vendor specific hardware revision of the object	None	Y	Network status		String	N/A	None
21	Diagnostic Status	Results of the object's diagnostics - vendor specific	None	Y	Network status		UINT4		None
22	Reset	Assertion resets communications to their unmapped state.  It is cleared after reset.	None	Y	Network config	N	BOOL
	N/A	None
23	Bus Available	Assertion forces the object to enter the bus available state	0: bus unavailable
1: bus available	Y	Network config	N	BOOL	N/A	None
24	Bootstrap	Forces the object to enter bootstrap state	None	N	Network config		BOOL	N/A	None
25	Object Indicator	Assertion causes module's communication LED to strobe fast.  Negation restores normal LED operation.	0: normal operation
1: strobe communication LED	Y	Network config	N	BOOL	N/A	None
26	Number of Times Mapped	The number of times the object has been mapped since power-up	None	N	Network status	N	UINT1	0	None
27	Module Base Address	The network address of the module's embedded object 0	None	N	Network status	N	UINT1	0	none
28	Number of Objects Mapped	Number of objects mapped by the communication master	None	N	Network status	N	UINT1	0	none
29	HB Disable	Disable heartbeat requests for the object	None	N	Network config	N	BOOL	N/A	none
50	Paired Items Count	Used by some equipment with multiple modules as a means to associate boards to each other (i.e. ?pair? them).	None	N	Network config	Y	UINT2
 * 
 * </pre>
 */
typedef struct NetworkAttributes_typ {
	/**
	 * <b>Attribute Number</b> 0 <br>
	 * <b>Name</b> Vendor ID <br>
	 * <b>Description</b> The vendor ID of the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	UINT VendorID;

	/**
	 * <b>Attribute Number</b> 1 <br>
	 * <b>Name</b> Vendor Name <br>
	 * <b>Description</b> The vendor name of the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* VendorName;

	/**
	 * <b>Attribute Number</b> 2 <br>
	 * <b>Name</b> Module Model Number <br>
	 * <b>Description</b> The module model number <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* ModuleModelNumber;

	/**
	 * <b>Attribute Number</b> 3 <br>
	 * <b>Name</b> Module Model Name <br>
	 * <b>Description</b> The module model name <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* ModuleModelName;

	/**
	 * <b>Attribute Number</b> 4 <br>
	 * <b>Name</b> Class ID <br>
	 * <b>Description</b> The class ID of the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	UINT ClassID;

	/**
	 * <b>Attribute Number</b> 5 <br>
	 * <b>Name</b> Class Name <br>
	 * <b>Description</b> The class name of the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* ClassName;

	/**
	 * <b>Attribute Number</b> 6 <br>
	 * <b>Name</b> Class Revision <br>
	 * <b>Description</b> The class revision of the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	UINT ClassRevision;

	/**
	 * <b>Attribute Number</b> 7 <br>
	 * <b>Name</b> Class Type <br>
	 * <b>Description</b> The class type of the object. The recommended class
	 * type for a robot is 2 for ?configuration? type. <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * 	bit 0: controlled, bit 1: config
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BMP1 <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT ClassType;

	/**
	 * <b>Attribute Number</b> 8 <br>
	 * <b>Name</b> Group Switch Settings <br>
	 * <b>Description</b> The hardware settings that define the object's welding
	 * group <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * bit 0: group 1, bit 2: group 3, bit 1: group 2 bit 3: group 4
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> BMP1 <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT GroupSwitchSettings;

	/**
	 * <b>Attribute Number</b> 9 <br>
	 * <b>Name</b> Instance Switch Settings <br>
	 * <b>Description</b> The hardware settings that define the object's
	 * instance number <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT InstanceSwitchSettings;

	/**
	 * <b>Attribute Number</b> 10 <br>
	 * <b>Name</b>Group Assignments<br>
	 * <b>Description</b>The assigned settings define the object's welding
	 * group. The assignment may be sourced by hardware settings.<br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * bit 0: group 1, bit 2: group 3, bit 1: group 2, bit 3: group 4
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BMP1 <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT GroupAssignments;

	/**
	 * <b>Attribute Number</b> 11 <br>
	 * <b>Name</b> Instance Assignments <br>
	 * <b>Description</b> The assigned settings that define the object's
	 * instance number. The assignment may be sourced by hardware settings. <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT InstanceAssignments;

	/**
	 * <b>Attribute Number</b> 12 <br>
	 * <b>Name</b> Arclink Revision <br>
	 * <b>Description</b> The Arclink revision that the object was created for
	 * <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	UINT ArclinkRevision;

	/**
	 * <b>Attribute Number</b> 13 <br>
	 * <b>Name</b> Ready Status <br>
	 * <b>Description</b> Indicates that the object is ready to function in the
	 * welding context <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * bits 0-6: vendor specific code, bit 7: ready
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BMP1 <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT ReadyStatus;

	/**
	 * <b>Attribute Number</b> 14 <br>
	 * <b>Name</b> Fault Status <br>
	 * <b>Description</b> Indicates that the object can not or has failed to
	 * properly function in the welding context <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * bits 0-6: vendor specific code, bit 7: faulted
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BMP1 <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT FaultStatus;

	/**
	 * <b>Attribute Number</b> 16 <br>
	 * <b>Name</b> Silicon Serial Number <br>
	 * <b>Description</b> Unique serial number assigned to each module <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b> <br>
	 * <b>Units Description</b> None <br>
	 */
	UINT SiliconSerialNumber;

	/**
	 * <b>Attribute Number</b> 17 <br>
	 * <b>Name</b> Number of Embedded Objects <br>
	 * <b>Description</b> The number of embedded objects in the module <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT NumberofEmbeddedObjects;

	/**
	 * <b>Attribute Number</b> 18 <br>
	 * <b>Name</b> Embedded Object Number <br>
	 * <b>Description</b> The embedded object offset within the module <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT EmbeddedObjectNumber;

	/**
	 * <b>Attribute Number</b> 19 <br>
	 * <b>Name</b> Software Version <br>
	 * <b>Description</b> The vendor specific software revision of the object
	 * <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* SoftwareVersion;

	/**
	 * <b>Attribute Number</b> 20 <br>
	 * <b>Name</b> Hardware Revision <br>
	 * <b>Description</b> The vendor specific hardware revision of the object
	 * <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> String <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	char* HardwareRevision;

	/**
	 * <b>Attribute Number</b> 21 <br>
	 * <b>Name</b> Diagnostic Status <br>
	 * <b>Description</b> Results of the object's diagnostics - vendor specific
	 * <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> UINT4 <br>
	 * <b>Decimal Places</b> <br>
	 * <b>Units Description</b> None <br>
	 */
	UDINT DiagnosticStatus;

	/**
	 * <b>Attribute Number</b> 22 <br>
	 * <b>Name</b> Reset <br>
	 * <b>Description</b> Assertion resets communications to their unmapped
	 * state. It is cleared after reset. <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BOOL <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT Reset;

	/**
	 * <b>Attribute Number</b> 23 <br>
	 * <b>Name</b> Bus Available <br>
	 * <b>Description</b> Assertion forces the object to enter the bus available
	 * state <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * 0: bus unavailable, 1: bus available
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BOOL <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT BusAvailable;

	/**
	 * <b>Attribute Number</b> 24 <br>
	 * <b>Name</b> Bootstrap <br>
	 * <b>Description</b> Forces the object to enter bootstrap state <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> <br>
	 * <b>Type</b> BOOL <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT Bootstrap;

	/**
	 * <b>Attribute Number</b> 25 <br>
	 * <b>Name</b> Object Indicator <br>
	 * <b>Description</b> Assertion causes module's communication LED to strobe
	 * fast. Negation restores normal LED operation. <br>
	 * <b>Value Semantics</b>
	 * 
	 * <pre>
	 * 0: normal operation, 1: strobe communication LED
	 * </pre>
	 * 
	 * <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BOOL <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT ObjectIndicator;

	/**
	 * <b>Attribute Number</b> 26 <br>
	 * <b>Name</b> Number of Times Mapped <br>
	 * <b>Description</b> The number of times the object has been mapped since
	 * power-up <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT NumberofTimesMapped;

	/**
	 * <b>Attribute Number</b> 27 <br>
	 * <b>Name</b> Module Base Address <br>
	 * <b>Description</b> The network address of the module's embedded object 0 <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b>0<br>
	 * <b>Units Description</b> None <br>
	 */
	USINT ModuleBaseAddress;

	/**
	 * <b>Attribute Number</b> 28 <br>
	 * <b>Name</b> Number of Objects Mapped<br>
	 * <b>Description</b> Number of objects mapped by the communication master <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> N <br>
	 * <b>Class</b> Network status <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT1 <br>
	 * <b>Decimal Places</b> 0 <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT NumberofObjectsMapped;

	/**
	 * <b>Attribute Number</b> 29 <br>
	 * <b>Name</b> HB Disable <br>
	 * <b>Description</b> Disable heartbeat requests for the object <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> BOOL <br>
	 * <b>Decimal Places</b> N/A <br>
	 * <b>Units Description</b> None <br>
	 */
	USINT HBDisable;

	/**
	 * <b>Attribute Number</b> 50 <br>
	 * <b>Name</b> Paired Items Count <br>
	 * <b>Description</b> Used by some equipment with multiple modules as a means to associate boards to each other (i.e. ?pair? them). <br>
	 * <b>Value Semantics</b> None <br>
	 * <b>Required</b> Y <br>
	 * <b>Class</b> Network config <br>
	 * <b>Save in EEPROM</b> N <br>
	 * <b>Type</b> UINT2 <br>
	 * <b>Decimal Places</b>  <br>
	 * <b>Units Description</b>  <br>
	 */
	UINT PairedItemsCount;	

	
	
} NetworkAttributes_typ;

#endif /* _NETWORK_ATTRIBUTE_H */
