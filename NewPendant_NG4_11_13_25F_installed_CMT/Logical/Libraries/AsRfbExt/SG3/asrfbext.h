/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ASRFBEXT_
#define _ASRFBEXT_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Datatypes and datatypes of function blocks */
typedef struct AsRfbExtType
{	unsigned long pClient;
	unsigned long pHeader;
	unsigned long uiTimeOut;
	unsigned long uiTimeOutValue;
	unsigned short status;
} AsRfbExtType;

typedef struct AsRfbExt_typ
{	unsigned long pClient;
	unsigned long pHeader;
	unsigned long uiTimeOut;
	unsigned long uiTimeOutValue;
	unsigned short status;
} AsRfbExt_typ;



/* Prototyping of functions and function blocks */
unsigned short RfbExtInit(unsigned long pLib, unsigned long uiTimeOut);
unsigned short RfbExtConnect(unsigned long pLib, unsigned long pName);
unsigned short RfbExtSetClientCaption(struct AsRfbExt_typ* pLib, unsigned long pClientCaption);
unsigned short RfbExtSetPassword(struct AsRfbExt_typ* pLib, unsigned char mode, unsigned long pPassword);
unsigned short RfbExtKeyState(struct AsRfbExt_typ* pLib, unsigned char index, unsigned long pValue);
unsigned short RfbExtReconnect(struct AsRfbExt_typ* pLib, unsigned long pTargetIp);
unsigned short RfbExtStartProcess(struct AsRfbExt_typ* pLib, unsigned long pcmdLine);
unsigned short RfbExtProcessState(struct AsRfbExt_typ* pLib, unsigned long piProcessState);
unsigned short RfbExtNrClientsConnected(struct AsRfbExt_typ* pLib);
unsigned short RfbExtServerTimeout(struct AsRfbExt_typ* pLib, unsigned long timeout);
unsigned short RfbExtDisableClientLogin(struct AsRfbExt_typ* pLib);
unsigned short RfbExtDisconnectAllClients(struct AsRfbExt_typ* pLib);
unsigned short RfbExtEnableClientLogin(struct AsRfbExt_typ* pLib);
unsigned short RfbExtSetMaxClients(struct AsRfbExt_typ* pLib, unsigned char maxClients);
unsigned short RfbExtDataAge(struct AsRfbExt_typ* pLib, unsigned long pAge);
unsigned short RfbExtJoystickValue(struct AsRfbExt_typ* pLib, unsigned char index, unsigned long pValueX, unsigned long pValueY, unsigned long pValueZ);
unsigned short RfbExtResetHandwheelValue(struct AsRfbExt_typ* pLib, unsigned char index, signed long value);
unsigned short RfbExtHandwheelValue(struct AsRfbExt_typ* pLib, unsigned char index, unsigned long pValue);
unsigned short RfbExtPotiValue(struct AsRfbExt_typ* pLib, unsigned char index, unsigned long pValue);


__asm__(".section \".plc\"");

__asm__(".previous");

#ifdef __cplusplus
};
#endif
#endif /* _ASRFBEXT_ */

