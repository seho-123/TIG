/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1761744881_6_
#define _BUR_1761744881_6_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
 #define gUdpClient_DefaultPort 9998U
 #define gUdpClient_TaskCycleTimeXms 1U
 #define gUdpServer_DefaultPort 9999U
#else
 _GLOBAL_CONST unsigned long gUdpClient_DefaultPort;
 _GLOBAL_CONST unsigned long gUdpClient_TaskCycleTimeXms;
 _GLOBAL_CONST unsigned long gUdpServer_DefaultPort;
#endif


/* Variables */
_GLOBAL struct udpClient_typ gUdpClient;
_GLOBAL struct udpServer_typ gUdpServer;
_GLOBAL_RETAIN struct udpSystemConfig_typ gUdpSystemCfg;
_GLOBAL plcstring gUdpClient_DefaultHostAdr[33];





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/UDP/paiUDPService/paiUDPService.var\\\" scope \\\"global\\\"\\n\"");

/* Additional IEC dependencies */
__asm__(".ascii \"iecdep \\\"Logical/UDP/paiUDPService/paiUDPService.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1761744881_6_ */

