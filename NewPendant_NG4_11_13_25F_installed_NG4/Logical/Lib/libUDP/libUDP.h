/* Automation Studio generated header file */
/* Do not edit ! */
/* libUDP  */

#ifndef _LIBUDP_
#define _LIBUDP_


/* Datatypes and datatypes of function blocks */
typedef struct Udp_Internal_typ
	{	unsigned char step;
	struct UdpOpen UdpOpen_0;
	struct UdpSend UdpSend_0;
	struct UdpRecv UdpRecv_0;
	struct UdpClose UdpClose_0;
} Udp_Internal_typ;

typedef struct Udp_Status_typ
	{	unsigned short error_number;
	plcbit error;
	plcbit ready;
	unsigned char counts;
} Udp_Status_typ;

typedef struct Udp_Config_typ
	{	plcstring ipAddress[21];
	unsigned short port;
	unsigned long bufferSize;
	signed char* buffer;
} Udp_Config_typ;

typedef struct Udp_Listener_Intrf_typ
	{	struct Udp_Status_typ oStatus;
	unsigned long oDataLen;
	unsigned long iRxCallback;
	unsigned long iRxRingBuf;
} Udp_Listener_Intrf_typ;

typedef struct Udp_Send_Intrf_typ
	{	struct Udp_Status_typ oStatus;
	plcbit iSendCMD;
	plcstring iIPaddress[21];
	unsigned short iPort;
	unsigned long iBufferSize;
	signed char* iBuffer;
	unsigned long iTxCallback;
} Udp_Send_Intrf_typ;

typedef struct UDP_typ
	{	struct Udp_Listener_Intrf_typ listener;
	struct Udp_Send_Intrf_typ send;
	struct Udp_Config_typ listener_config;
	struct Udp_Internal_typ internal;
	plcbit ResetCmd;
} UDP_typ;

#endif /* _LIBUDP_ */

