
#ifndef _DATAMGRLIB_H
#define _DATAMGRLIB_H

#include <bur/plc.h>
#include <bur/plctypes.h>
#include <sys_lib.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Datatypes and datatypes of function blocks */
typedef struct DATA_IF
{	plcbit DataObjReady;
	plcbit WriteData;
	plcbit ReadData;
	plcbit CreateObj;
	plcbit DeleteObj;
	unsigned long DataLen;
	unsigned long Offset;
	unsigned short Status;
	unsigned char* pSource;
	unsigned char* pDest;
} DATA_IF;

typedef struct DataMgr_obj
{	struct DATA_IF* DataIF;
	plcstring ObjName[33];
	unsigned char MemType;
	plcbit _getInfo;
	unsigned long Ident;
	unsigned long ObjLen;
	plcbit ObjReady;
	unsigned short Status;
} DataMgr_obj;



/* Prototyping of functions and function blocks */
signed long DataMgr_Srv(struct DataMgr_obj* mgr);


__asm__(".section \".plc\"");

__asm__(".previous");


#endif /* _DATAMGRLIB_ */