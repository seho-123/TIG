
#ifndef _ROBOT_H
#define _ROBOT_H

#include <bur/plctypes.h>

#include <runtime.h>
//#include 

#define ROBOT_INIT  	0
#define ROBOT_ERROR 	1
#define ROBOT_READY  	2
#define ROBOT_ARC_OK	3

#define ROBOT_ERR_NONE 		0
#define ROBOT_ERR_TIMEOUT 	1

#define LOG_MAX_LINE 30
#define LOG_MAX_LENGTH 255

typedef struct Log_typ{
	STRING log[LOG_MAX_LINE][LOG_MAX_LENGTH];
	UDINT pLog;
}Log_typ;



/* SetControlsActionRequest_typ */
typedef struct SetControlsActionRequest_typ{
	/**
	 * <pre>
	1 Byte	Parameter Mask
	 * </pre>
	 */
	USINT ParameterMask;

	/**
	 * <pre>
	2 Bytes	Weld Mode 	If parameter mask bit 0 set.
	 * </pre>
	 */
	UINT WeldMode;

	/**
	 * <pre>
	2 Bytes	Work-point	If parameter mask bit 1 set.
	 * </pre>
	 */
	UINT WorkPoint;

	/**
	 * <pre>
	2 Bytes	Trim	If parameter mask bit 2 set.
	 * </pre>
	 */
	UINT Trim;

	/**
	 * <pre>
	2 Bytes	Wave-Control 1	If parameter mask bit 3 set.
	 * </pre>
	 */
	UINT WaveControl1;

	/**
	 * <pre>
	2 Bytes	Wave-Control 2	If parameter mask bit 4 set.
	 * </pre>
	 */
	UINT WaveControl2;

	/**
	 * <pre>
	2 Bytes	Wave-Control 3	If parameter mask bit 5 set.
	 * </pre>
	 */
	UINT WaveControl3;

	/**
	 * <pre>
	2 Bytes	Wave-Control 4	If parameter mask bit 6 set.
	 * </pre>
	 */
	UINT WaveControl4;	

} SetControlsActionRequest_typ;

typedef struct WC_WeldSchedule_Param_typ{
	UINT WeldMode;
	UINT AttributeID;
	USINT ProcessType;
	USINT WireType;
	USINT WireSize;
} WC_WeldSchedule_Param_typ;

typedef struct WC_Control_Param_typ{
	USINT ParameterMask;
	UINT WeldMode;
	UINT Workpoint;
	UINT Trim;
	UINT WaveControl1;
	UINT WaveControl2;
	UINT WaveControl3;
	UINT WaveControl4;
}WC_Control_Param_typ;

typedef struct WC_Setup_typ{
	SetControlsActionRequest_typ ctrlParam; // Written to Weld Controller
	WC_WeldSchedule_Param_typ weldParam; // Read/Write
} WC_Setup_typ; 

/* High Speed Telemetry */
typedef struct WC_HS_Telemetry_typ{
	UINT current; // raw value
	UINT voltage; // raw value
	REAL fCurrent; // Scaled value
	REAL fVoltage; // scaled value
} WC_HS_Telemetry_typ;

/* High Speed Telemetry */
typedef struct WC_Data_typ{
	WC_HS_Telemetry_typ telemetry;	// High Speed Telemetry
} WC_Data_typ;

/* Weld Controller Structure */
typedef struct WC_typ{
	WC_Setup_typ setup; // Setup
	WC_Data_typ data;
	UINT WC_Ready_Status;
	UINT WC_Fault_Status;
	
} WC_typ;

//////////////////////////////////////
//////////////////////////////////////
typedef struct ARCLINK_PS_FBK_typ{

	UINT Error;
	UINT Status;
	float curArcVolt;
	float curArcAmp;
	USINT coolerSens_Status;
	USINT coolerPump_Status;

}ARCLINK_PS_FBK_typ;

typedef struct ARCLINK_PS_Cmd_typ{
	UINT RUN;
	UINT COOLER;
	UINT RST;
	UINT Mode;
	float AmpSP;
	float BckAmpSP;
	float PulseFreq;
	float PulseWidth;

	float Trim;
	float UltimArc;
	float WFS;
	float Volts;
	float Pinch;

	UINT Development;
	UINT SendParam;
		

}ARCLINK_PS_Cmd_typ;

typedef struct ARCLINK_PS_INT_typ{

	UINT _RUN;
	UINT _COOLER;
	UINT _RST;
	UINT _Mode;
	float _AmpSP;
	float _BckAmpSP;
	float _PulseFreq;
	float _PulseWidth;

	float _Trim;
	float _UltimArc;
	float _WFS;
	float _Volts;
	float _Pinch;

}ARCLINK_PS_INT_typ;


/* ARC Link PS Cntrl Structure */
typedef struct ARCLINK_PS_CNTRL_typ{
	ARCLINK_PS_Cmd_typ  Cmd;	
	ARCLINK_PS_FBK_typ Fbk;
	ARCLINK_PS_INT_typ  Int;
} ARCLINK_PS_CNTRL_typ;

typedef struct ReadControlParameterData_Req_typ{
	/*
	Request The Attribute ID may be any of the WC‚Äôs control parameters (work-point, trim, wave-controls 1-4)
	2 Bytes Attribute ID
	2 Bytes Weld Mode
	*/
	UINT Attribute_ID;
	UINT Weld_Mode;
	
}ReadControlParameterData_Req_typ;

typedef struct ReadControlParameterData_typ{
/* 
	Response	The maximum/minimum values provide the largest possible range of the control parameter for the given Weld Mode 
	2 Bytes Maximum Value
	2 Bytes Minimum Value
	1 Byte	Units Code
	1 Byte	Decimal Places
	17 Bytes	Name (16 Characters + Null Terminator)
	Return Code 
	128 Invalid Mode
	129 Non Relevant Trim
	130 Invalid Attribute ID
*/
	UINT MaxVal;
	UINT MinVal;
	USINT UnitsCode;
	USINT DecimalPlace;
	plcstring Name[17];

	USINT ReturnCode;

	
}ReadControlParameterData_typ;

typedef struct WeldTableSearchRequest_typ {
	/**
	 * <pre>
	4 Bytes	Mode Descriptor- (see Table 7 7 Mode Descriptor Definitions)
	
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
	 * 
	 * </pre>
	 */
	UDINT ModeDescriptor;
		/**
	 * <pre>
	4 Bytes	Mode Descriptor Care; a ‚Äú1‚Äù in the corresponding bit to include that mode descriptor bit in the search.
	 * </pre>
	 */
	UDINT ModeDescriptorCare;
	/**
	 * <pre>
	2 Bytes	Starting Weld Mode Index.
	 * </pre>
	 */
	UINT StartWeldModeIndex;

	/**
	 * <pre>
	1 Byte	Maximum Number of Matches
	 * </pre>
	 */
	USINT MaxNumberMatches;
		/* GTAW Gas Tungsten Arc Welding(Pulsed and non-pulsed) */

	/**
	 * <pre>
	1 Byte	Process Type- (see Table 7 4 Process Type Definitions)
	General Process Type
	0 = SMAW	Shielded Metal Arc Welding
	1 = FCAW-S	Self Shielded Flux Cored Arc Welding 
	2 = GTAW	Gas Tungsten Arc Welding (pulsed and non-pulsed)
	3 = GMAW	Gas Metal Arc Welding (pulsed and short circuit)
	4 = GOUGE	Arc Gouging
	5 = PAW	Plasma Arc Welding
	6 = SAW	Submerged Arc Welding (series and non-series)
	7 = TSENSE	Touch Sensing
	8 = FCAW	Flux Cored Arc Welding (self and gas shielded)
	9 = FCAW-G	Gas  Shielded Flux Cored Arc Welding
	10 = GTAW-P	Pulsed Gas Tungsten Arc Welding
	11 = GMAW-P	Pulsed Gas Metal Arc Welding
	12 = GMAW-S	Short Circuit Gas Metal Arc Welding
	13 = SAW-S	Series Submerged Arc Welding
	Table 7 4 Process Type Definitions
	 * 
	 * </pre>
	 */
	USINT ProcessType;

		/**
	 * <pre>
	1 Byte	Wire Type- (see Table 7 3 Wire Type Definitions)
	Wire Type
	0= ------	1=N/A	2=Steel	3=Stainless	4=Aluminum
	5=CrNi Alloy	6=Ni Alloy	7=NiCr Alloy	8=Copper Ni	9=Si Bronze
	10=Al Bronze	11=Stick	12=Cored Wire	13=Metal Core	14=SelfShield
	15=Gas Shield	16=Carbon Arc	17=L-50/L-56	18=L-50	19=L-54
	20=L-56	21=L-60	22=L-70	23=L-50 Steel	24=L-54 Steel
	25=L-56 Steel	26=LA-71	27=LA-75	28=LA-81	29=LA-90
	30=LA-100	31=ER70S-X	32=ER70S-3	33=ER70S-4	34=ER70S-6
	35= Steel SG-1	36= Steel SG-2	37= Steel SG-3	38=2xx Stnles	39=3xx Stnles
	40=4xx Stnles	41=Soft Alum	42=Hard Alum	43=1100 Al	44=2319 Al
	45=4043 Al	46=4047 Al	47=4145 Al	48=5183 Al	49=5356 Al
	50=AlMg 5	51=AMg4.5Mn	52=AlMg 3	53=AlSi 5	54=Al 99.5
	55=NiMo-1	56=NiMo-2	57=NiMo-3	58=NiMo-7	59=NiCrMo-1
	60= NiCrMo-2	61= NiCrMo-3	62= NiCrMo-4	63= NiCrMo-7	64= NiCrMo-8
	65= NiCrMo-9	66=C-276	67=Copper	68=CuSi	69=CuSn
	70=CuNi	71=CuAl	72=NR-5	73=NR-131	74=NR-151
	75=NR-152	76=NR-202	77=NR-203 M	78=NR-203 Ni	79=NR-203NiC
	80=NR-204	81=NR-207	82=NR-211 MP	83=NR-232	84=NR-305
	85=NR-311	86=NR-311 Ni	87=NR-314	88=NR-400	89=NS-3M
	90=OS-70	91=OS HD-70	92=OS-71	93=OS-71M	94=OS 81B2-H
	95=OS 81K2-H	96=OS 81Nil-H	97=OS 91K2-H	98=MC-100	99=MC-120-55
	100=MC-409	101=MC702	102=MC710	103=MC-710-H	104=Other
	Table 7 3 Wire Type Definitions
	 * 
	 * </pre>
	 */
	USINT WireType;

	/**
	 * <pre>
	1 Byte	Wire Size- (see Table 7 2 Wire Size Definitions)
	Wire Size
	0= -----	1=N/A	2=0.025	3=0.030	4=0.035
	5=0.045	6=3/64	7=0.052	8=1/16	9=0.068
	10=0.072	11=5/64	12=3/32	13=7/64	14=0.120
	15=1/8	16=5/32	17=3/16	18=7/32	19=1/4
	20=5/16	21=3/8	22=0.8mm	23=0.9mm	24=1.0mm
	25=1.2mm	26=1.4mm	27=1.6mm	28=2.0mm	29=2.4mm
	30=2.5mm	31=3.2mm	32=4.0mm	33=5.0mm	34=5.6mm
	35=6.0mm	36=6.3mm			
	Table 7 2 Wire Size Definitions
	 * 
	 * </pre>
	 */
	USINT WireSize;

	/**
	 * <pre>
	1 Byte	Gas Type- (see Table 7 5 Gas Type Definitions)
	Gas Type
	0 = No Gas	1 = M3	2 = M2	4 = M1	8 = HE
	16 =  H	32 = CO2	64 = O2	128 = AR	
	Table 7 5 Gas Type Definitions
	 * 
	 * </pre>
	 */
	USINT GasType;
		/**
	 * <pre>
	1 Byte	Search Mask- (see Table 21 2- Weld Table Search Mask Bit Definitions); a‚Äù1‚Äù in the corresponding bit to include that request field in the search.
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
	 * 
	 * </pre>
	 */
	USINT SearchMask;
} WeldTableSearchRequest_typ;

//////////////////////////////////////////////////////////////////////
// Robot structure definition
//////////////////////////////////////////////////////////////////////

/* Arc_Link_Event_Request_Report */
typedef struct Event_Report_Request_typ{
	
	/**
	 * 2 Bytes Attribute ID Number
	 */
	UINT AttributeIDNumber;

	/**
	 * 1 Byte Sequence State (only relevant for sequence attributes)
	 */
	USINT SequenceState;

	/**
	 * 1 Byte Property ID Number
	 */
	USINT PropertyIDNumber;

	/**
	 * 1 Byte Trigger Options
	 * 
	 * <pre>
	 * Trigger Event Options
	Bit 7	
	CONFIRM 

	Bit 6	5	
	TRIGGER TYPE	

	Bit 4	3	2	1	0
	DESCRIPTORS

	0 = No
	1=Yes
	(see note)	0 = Periodic	Not Used
	1 = Value Compare	0 = Attribute equals Comparison Value
	1 = Attribute does not equal Comparison Value
	2 = Attribute > Comparison Value
	3 = Attribute < Comparison Value
	4 = Attribute >= Comparison Value
	5 = Attribute <= Comparison Value
	2 = Range Compare	0 = In Range
	1 = Out of Range
	3 = Change in Value or State	Not Used
	Note: If Confirm = No, then the event will be report will use the Unconfirmed Event report service.  If Confirm = Yes, then the event will use the confirmed Event report service.
	 * 
	 * </pre>
	 */
	USINT TriggerOptions;
	
	/**
	 * 1 Byte Reserved
	 */
	USINT Reserved;

	/**
	 * N Data Bytes
	 */
	USINT *Data;
} Event_Report_Request_typ;

/* Arc Link Miscellanious Setup Required for telemetry */
typedef struct Arc_Link_Misc_Setup_typ {
	BOOL running;
	USINT state;
	USINT CoolerAddr;
	USINT GasSolAddr;
	Event_Report_Request_typ er;
} Arc_Link_Misc_Setup_typ;


/* Robot Interface */
typedef struct Robot_Intrf_typ{
	UINT xCMD;
	
} Robot_Intrf_typ;

typedef struct Robot_Comm_typ{
	TON_typ ArcComTimer;
	TON_typ CoolerSensPullTimer;
	ReadControlParameterData_Req_typ CntrlParamReq;
	ReadControlParameterData_typ CntrlParamData;
	//TIME ArcComTimer_PT;
	UDINT CoolerSensPull_PT;
	UDINT PT;
	BOOL ArcLinkActive;
	BOOL HeartBeat_Received;
	BOOL Oneshot;
	BOOL SysOn;	
	BOOL ClearError;
	BOOL StopRespondHeartBeat;
} Robot_Comm_typ;

typedef struct Robot_typ
{
	ArcLinkObject_typ arclink;	
	//WeldController_typ wc

	// Internal flags
	BOOL isMapped;
	BOOL isMapping;
	BOOL isMiscSetupOneShot; 
	BOOL ArcLinkActive;
	BOOL isReady; // ready status from PS
	Arc_Link_Misc_Setup_typ miscSetup;
	Robot_Intrf_typ Intrf;
	WC_typ wc; // WeldController object 
	ARCLINK_PS_CNTRL_typ Cntrl;
	Robot_Comm_typ Comm;
	UINT ReqDataSize;
	USINT ReqData_raw[32];
	UINT ResDataSize;
	USINT ResData_raw[32];
	Log_typ errorlog;
	Log_typ eventlog;
} Robot_typ;

/* ready status  */
typedef struct ReadyStatus_typ {
	BOOL ArcLink;  //0 ñ when bus is NOT available 
	BOOL NoFault;  //0 ñ when WC Fault Status Attribute is non-zero 
	BOOL IO; //0 ñ when Touch Sensing (see Touch Sense) or momentarily while the inverter is initializing.
	BOOL Config; // always 1
	BOOL Reserved_1; // always 1
	BOOL Reserved_2; // always 1
	BOOL Reserved_3; // always 1
	BOOL ReadyCoil; //0 ñ when any of bit 0 through 6 are 0 (i.e. logical AND)
} ReadyStatus_typ;

// Fault Status
typedef struct FaultStatus_typ {
	BOOL ArcLink;  //always 0
	BOOL Reserved;  //always 0
	BOOL IO; //0 ñ when all System Faults 
	BOOL Config; // always 0
	BOOL Reserved_1; // always 0
	BOOL Reserved_2; // always 0
	BOOL Reserved_3; // always 0
	BOOL FaultCoil; // 1 ñ when any of bit 0 through 6 are 1 (i.e. logical OR)
} FaultStatus_typ;


/* Status Bit mask for robot interface feedback need to be discussed*/ 
#define PS_Ready 0x0001
#define Arc_OFF 0x0002
#define Arc_OCV 0x0004
#define Arc_Short 0x0008
#define Arc_Active 0x0010
#define ArcLink_Active 0x0020
#define ArcLink_Mapping 0x0040
#define ArcLink_Mapped 0x0080


//Error bit mask 
#define PS_System_Fault 0x0001
#define ArcLink_Com_Failed 0x0002


//////////////////////////////////////////////////////////////////////
// Robot Config Flags
//////////////////////////////////////////////////////////////////////
#define IS_ROBOT 1;
#define ROBOT_CLASS 10;
#define ROBOT_CLASS_TYPE 2 // 0b10 Configuration Type
#define PENDANT_CLASS 15;
#define PENDANT_CLASS_TYPE 2 // 0b01 Controlled Type

//////////////////////////////////////////////////////////////////////
// Robot Interface Commands
//////////////////////////////////////////////////////////////////////
#define ROBOT_CMD_PS_ON 			1	// Turn Power Supply ON	
#define ROBOT_CMD_PS_OFF 			2	// Turn Power Supplt OFF
#define ROBOT_SET_CONTROLS			3	// Set Controls Action Request

#define ROBOT_CMD_COOLER_ON			4	// Turn On Cooler
#define ROBOT_CMD_COOLER_OFF		5	// Turn On Cooler

#define ROBOT_CMD_SEARCH_COOLER_ADR	6	// Search for Cooler Address
#define ROBOT_READ_COOLER_SENS		7	// Search for Cooler Address

#define ROBOT_READ_WC_PARAM_DATA	8  // Read WC Parameter Data 


#define ROBOT_CMD_SEARCH_GAS_SOL_ADR	9	// Search for Gas Sol Address
#define ROBOT_CMD_GAS_SOL_ENABLE		10	// Make  Gas Sol ‚ÄúManual Valve‚Äù
#define ROBOT_CMD_GAS_SOL_ON			11	// Turn On Gas Sol
#define ROBOT_CMD_GAS_SOL_OFF			12	// Turn Off Gas Sol

/*
For the solenoid in the machine that is part of the Stick/TIG UI/Remote/Gas Solenoid kit (K3001-X kit).
This is controlled by 2 main attributes in the Remote Control object (object class ID 22). 
These attributes are 30725 which needs to be set to true in order for the solenoid function to be enabled at all,
and then the solenoid is commanded on and off by attribute 30723. 
Note that the UI will attempt to write the value of 30725 at startup based on the setting of P.4 
which is the solenoid config. Setting this to ‚ÄúPowersource Solenoid‚Äù via Power Wave Manager 
or the UI panel instead of ‚ÄúManual Valve‚Äù will cause it to set 30725 correctly for you. 
You can also change this P. menu setting by writing a value of 2 to attribute 30800 on the UI object (class ID 23).

*/

#endif /* _ROBOT_H */
