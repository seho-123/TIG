#ifndef _LIBPARSER_H
#define _LIBPARSER_H

/********************************************************
* DEFINITIONS:
*********************************************************/
#define MAX_BLOCK_LENGTH 		64
#define MAX_KEYWORD_LENGTH 		32
#define MAX_MACHCFG_FILE_SIZE	500000
#define MAX_PARAM_LENGTH 		MAX_BLOCK_LENGTH-MAX_KEYWORD_LENGTH

#define kwINDX 					"INDEX"
#define kwINDX_IDX				1

#define kwSERIALNUM				"SERIAL_NUMBER"
#define kwSERIALNUM_IDX			2

#define kwARCLINK_IP			"ARCLINK_IP"
#define kwARCLINK_IP_IDX		3


/// Travel   10 - 19
#define kwTVL_DIR				"TVL_DIRECTION"
#define kwTVL_DIR_IDX			10

#define kwTVL_ENC_TICKS			"TVL_TICKS_PER_UNIT"
#define kwTVL_ENC_TICKS_IDX		11

#define kwTVL_UNIT_DIST			"TVL_UNIT_DISTANCE"
#define kwTVL_UNIT_DIST_IDX		12

#define kwTVL_BTN_JOG_GAIN		"TVL_Btn_JOG_GAIN"
#define kwTVL_BTN_JOG_GAIN_IDX	13

#define kwTVL_JOG_MODE			"TVL_JOG_MODE"
#define kwTVL_JOG_MODE_IDX		14		
/////////////////////////	

/// TIG   20 - 29
#define kwTIG_MODE				"TIG_MODE"
#define kwTIG_MODE_IDX			20


#define MAX_KEYWORDS		30



typedef struct CFGKEY
{	plcstring sKey[MAX_KEYWORD_LENGTH];
	unsigned short Index;
	unsigned char Type;
} CFGKEY;

typedef struct ITEM_PARAM
{	plcstring sItem[MAX_KEYWORD_LENGTH+3];
	plcstring sParam[33];
	signed short Key;
} ITEM_PARAM;

typedef struct MACHCFG_ParserMgr
{	unsigned short _idx;
	unsigned long pMaster;
	unsigned short Blocks;
	unsigned short curLine;
	signed long Status;
	unsigned short Ignored;
	unsigned short Errors;
	unsigned short ErrorItem;
	unsigned long _keyword_adr;
	unsigned long f;
	unsigned long len;
} MACHCFG_ParserMgr;

DINT KeyWord_init( CFGKEY *kw0);
DINT ParseMachCfgFile( MACHCFG_ParserMgr *mgr, UDINT FileSize );


DINT ProcessMachCfgBlock( MACHCFG_ParserMgr *mgr, char* sBlock);
DINT GetIndx( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr );
UINT FindItemParam( USINT *s, ITEM_PARAM *ip );
DINT FindKeyWord( MACHCFG_ParserMgr *mgr, ITEM_PARAM *ip );
UDINT BothTrim(char* s);

DINT SetGenCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr );
DINT SetTVLCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr );
DINT SetTIGCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr );


#endif
