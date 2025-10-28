/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1756997942_5_
#define _BUR_1756997942_5_

#include <bur/plctypes.h>

/* Datatypes and datatypes of function blocks */
typedef enum udpConst_typ
{	UDP_IP_ADDR_LEN = 32,
	UDP_SERVER_RXBUF_SIZE = 32000,
	UDP_CLIENT_TXBUF_SIZE = 32000
} udpConst_typ;

typedef struct udpIPAddress_typ
{	unsigned char digit3;
	unsigned char digit2;
	unsigned char digit1;
	unsigned char digit0;
	plcstring text[21];
} udpIPAddress_typ;

typedef struct udpSystemConfig_typ
{	unsigned long magicNumber;
	unsigned char udpClientMode;
	unsigned char udpClientUpdateRate;
	unsigned short udpLocalServerPortNumber;
	unsigned short udpRemoteHostPortNumber;
	struct udpIPAddress_typ udpRemoteHostIpAddress;
	unsigned long updInterfaceType;
} udpSystemConfig_typ;

typedef struct udpStatistics_typ
{	unsigned long datagramCountTotal;
	unsigned long datagramPerSec;
	unsigned long datagramPerSecMax;
	unsigned long bytesTotal;
	unsigned long bytesPerSec;
	unsigned long bytesPerSecMax;
	unsigned long errorCountTotal;
} udpStatistics_typ;

typedef struct udpClientInterface_typ
{	plcbit iConnect;
	plcstring iRmtIPAddr[33];
	unsigned short iRmtUdpPort;
	unsigned char iMode;
	plcbit iSend;
	plcbit iSendEcho;
	plcstring iSendEchoRmtIpAddr[33];
	unsigned char iUpdateRate;
	unsigned char iTaskCycleTimeXms;
	unsigned long iTxDataAdr;
	unsigned long iTxDataLen;
	unsigned long iTxCallback;
	plcbit oError;
	plcstring oRmtIpAddr[33];
	unsigned short oRmtUdpPort;
	plcbit oConnected;
	plcbit oSendDone;
} udpClientInterface_typ;

typedef struct udpServerInterface_typ
{	plcbit iEnable;
	unsigned short iUdpServerPort;
	unsigned long iRxCallback;
	unsigned long iRxTimeoutXms;
	plcbit oRxIsTimeout;
	plcbit oError;
	plcbit oRxReady;
	plcstring oRxRmtIP[33];
	unsigned short oRxRmtPort;
	unsigned long oRxDataLen;
	unsigned long oRxDataLenMax;
	unsigned long oRxDataAdr;
} udpServerInterface_typ;

typedef struct udpFBKs_typ
{	struct UdpOpen open;
	struct UdpClose close;
	struct UdpConnect connect;
	struct UdpDisconnect disconnect;
	struct UdpRecv recv;
	struct UdpSend send;
	struct UdpIoctl ioctl;
} udpFBKs_typ;

typedef struct udpClient_typ
{	struct udpClientInterface_typ clientIf;
	struct udpFBKs_typ client;
	struct udpStatistics_typ txStats;
	struct bgSSM_typ sm;
	unsigned char txbuf[32001];
} udpClient_typ;

typedef struct udpServer_typ
{	struct udpServerInterface_typ serverIf;
	struct udpFBKs_typ server;
	struct udpStatistics_typ rxStats;
	struct bgSSM_typ sm;
	plcstring rmtIPAddr[33];
	unsigned long rxlen;
	unsigned char rxbuf[32001];
} udpServer_typ;






__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/UDP/paiUDPService/paiUDPService.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1756997942_5_ */

