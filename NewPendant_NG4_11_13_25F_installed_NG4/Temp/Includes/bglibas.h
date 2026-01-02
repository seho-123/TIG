/* Automation Studio generated header file */
/* Do not edit ! */
/* bglibas  */

#ifndef _BGLIBAS_
#define _BGLIBAS_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "standard.h"
#endif
#ifdef _SG4
		#include "standard.h"
#endif
#ifdef _SGC
		#include "standard.h"
#endif


/* Datatypes and datatypes of function blocks */
typedef enum BGSSM_CONST_XYZ
{	BGSSM_FLAG_COUNT = 5,
	BGSSM_EVENT_COUNT = 5,
	BGSSM_COUNTER_COUNT = 5,
	BGSSM_TIMER_COUNT = 5,
	BGSSM_REGISTER_COUNT = 5
} BGSSM_CONST_XYZ;

typedef struct bgFIFO_typ
{	unsigned long iPtrPushData;
	unsigned long cfgEntrySize;
	unsigned long cfgMaxEntries;
	unsigned long oPtrPullData;
	unsigned long oFIFOCount;
	unsigned long oFIFOFull;
	unsigned long oPushIdx;
	unsigned long oPullIdx;
	unsigned long prvFIFOMemory;
} bgFIFO_typ;

typedef struct bgSSM_typ
{	unsigned long state;
	unsigned long stateNext;
	unsigned long stateRet;
	unsigned long stateOld;
	plcstring stateText[33];
	unsigned long pfunGetStateName;
	unsigned long pfunCyclicCallback;
	plcbit stateInit;
	unsigned long error;
	plcstring errorText[33];
	plcbit event[6];
	plcbit flag[6];
	unsigned long counter[6];
	unsigned long value[6];
	struct TON timer_x1ms[6];
	struct TON dlytimer_x1ms;
	unsigned long dlyCycles;
} bgSSM_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC unsigned long bgSSMInit(struct bgSSM_typ* p, unsigned long pfunGetStateName, unsigned long pfunCyclicCallback);
_BUR_PUBLIC unsigned long bgSSMCyclic(struct bgSSM_typ* p);
_BUR_PUBLIC unsigned long bgSSMStateNextTimeDelay(struct bgSSM_typ* p, unsigned long stateNext, unsigned long timeDelayX1ms);
_BUR_PUBLIC unsigned long bgSSMStateNextCycleDelay(struct bgSSM_typ* p, unsigned long stateNext, unsigned long delayCycles);
_BUR_PUBLIC unsigned long bgSSMStateNext(struct bgSSM_typ* p, unsigned long stateNext);
_BUR_PUBLIC plcbit bgSSMIsStateInit(struct bgSSM_typ* p);
_BUR_PUBLIC unsigned long bgSSMGetState(struct bgSSM_typ* p);
_BUR_PUBLIC plcstring* bgSSMGetStateText(struct bgSSM_typ* p);
_BUR_PUBLIC unsigned long bgFIFOInit(struct bgFIFO_typ* p, unsigned long entrySize, unsigned long entryTotalCount, unsigned long pFIFOmem);
_BUR_PUBLIC unsigned long bgFIFOPush(struct bgFIFO_typ* p, unsigned long pSrc, unsigned long srcSize);
_BUR_PUBLIC unsigned long bgFIFOPull(struct bgFIFO_typ* p, unsigned long pDst, unsigned long dstSize);
_BUR_PUBLIC unsigned long bgFIFOGetCount(struct bgFIFO_typ* p);
_BUR_PUBLIC unsigned long bgFIFOIsFull(struct bgFIFO_typ* p);
_BUR_PUBLIC unsigned long bgTimerCyclic(struct TON* pTON);
_BUR_PUBLIC unsigned long bgTimerStart(struct TON* pTON, unsigned long timeoutTimeX1ms);
_BUR_PUBLIC unsigned long bgTimerStop(struct TON* pTON);
_BUR_PUBLIC plcbit bgTimerIsEnabled(struct TON* pTON);
_BUR_PUBLIC plcbit bgTimerIsTimeOut(struct TON* pTON);
_BUR_PUBLIC unsigned long bgTimerTimeElapsed(struct TON* pTON);
_BUR_PUBLIC unsigned long bgTimerTimeRemaining(struct TON* pTON);


#ifdef __cplusplus
};
#endif
#endif /* _BGLIBAS_ */

