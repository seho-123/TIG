/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1756997942_1_
#define _BUR_1756997942_1_

#include <bur/plctypes.h>

/* Datatypes and datatypes of function blocks */
typedef enum applicationGlobal_enums
{	PAI_PVILIGHT_LIST_ITEM_INDEX_MAX = 127,
	PAI_PVILIGHT_LIST_INDEX_MAX = 31,
	PAI_PVILITE_ARRLIST_INDEX_MAX = 19,
	PAI_PVILIGHT_PV_NAME_LEN = 255,
	PAI_PVILIGHT_UDP_MSG_SIZE_MAX = 9000,
	PAI_PVILIGHT_PV_VAL_TXT_LEN = 31
} applicationGlobal_enums;

typedef struct ctrlInterfaceMsgHeader_typ
{	plcstring msgVersion[32];
	unsigned long msgCounter;
	unsigned long msgEchoRequest;
	unsigned long msgEchoResponse;
} ctrlInterfaceMsgHeader_typ;

typedef struct ctrlInterfaceCmd_typ
{	struct ctrlInterfaceMsgHeader_typ header;
} ctrlInterfaceCmd_typ;

typedef struct ctrlInterfaceStatus_typ
{	struct ctrlInterfaceMsgHeader_typ header;
} ctrlInterfaceStatus_typ;

typedef struct paiPVIGetArrItem_typ
{	unsigned long isUsed;
	unsigned long isError;
	unsigned long isChanged;
	plcstring pvArrName[256];
	unsigned long pvArrDimension;
	unsigned long arrIdxNext;
	unsigned long arrItemsPerUpdate;
	struct TON notificationTimer;
	unsigned long arrItemAproxSizeJSON;
	unsigned long updateState;
	unsigned long rpcId;
	plcstring iRmtIPAddr[33];
	unsigned short iRmtUdpPort;
} paiPVIGetArrItem_typ;

typedef struct paiPVIListItem_typ
{	unsigned long isUsed;
	unsigned long isError;
	unsigned long isChanged;
	plcstring pvName[256];
	unsigned long pvType;
	unsigned long pvDimension;
	unsigned long pvLen;
	unsigned long pPvValue;
	unsigned long pvValueOld;
	float pvValueOldFloat;
	plcstring pvValueTxt[32];
} paiPVIListItem_typ;

typedef struct paiPVIList_typ
{	unsigned long isUsed;
	unsigned long isActive;
	unsigned long isChanged;
	unsigned long isNotification;
	plcstring iRmtIPAddr[33];
	unsigned short iRmtUdpPort;
	plcstring listName[256];
	unsigned long notificationMode;
	unsigned long notificationFormat;
	float notificationTimeMinPeriod_s;
	float notificationTimeMaxPeriod_s;
	struct TON notificationTimerMin;
	struct TON notificationTimerMax;
	unsigned long itemCount;
	struct paiPVIListItem_typ itemList[128];
	unsigned long pJson;
} paiPVIList_typ;

typedef struct paiPVIListCollection_typ
{	unsigned long listCount;
	struct paiPVIList_typ listCollection[32];
	struct TON arrayListDlyTimer;
	struct paiPVIGetArrItem_typ arrayListCollection[20];
} paiPVIListCollection_typ;






__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/UDP/udpApp.typ\\\" scope \\\"global\\\"\\n\"");

/* Additional IEC dependencies */
__asm__(".ascii \"iecdep \\\"Logical/UDP/paiUDPService/paiUDPService.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1756997942_1_ */

