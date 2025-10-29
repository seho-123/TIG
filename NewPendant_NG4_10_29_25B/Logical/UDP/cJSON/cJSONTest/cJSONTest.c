/********************************************************************
 * COPYRIGHT -- HP
 ********************************************************************
 * Program: cJSONTest
 * File: cJSONTest.c
 * Author: goran
 * Created: June 19, 2013
 ********************************************************************
 * Implementation of program cJSONTest
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
//#include <../../../bglib/bgTimer1ms.c>
#include <../../../bglib/bgsprintf.c>
//#include <../../../bglib/bgPVSerialize.c>
#include <../../../bglib/bgPVSerializeJSON.c>

_GLOBAL BOOL gDoit;
_GLOBAL BOOL gDoit1;
_GLOBAL BOOL gDoit2;
_GLOBAL BOOL gDoit3;
_GLOBAL STRING gPVName[31];
_GLOBAL STRING gOutputDbg[255];
_GLOBAL INT gArrBeg;
_GLOBAL INT gArrEnd;



_GLOBAL int gPV1num;
_GLOBAL REAL gPV2float;
_GLOBAL STRING gPV3str[128];
_GLOBAL int gMyCounter;

#define PAIPVILITE_KEY_VERSION	"paipvilite"
#define PAIPVILITE_KEY_METHOD	"method"
#define PAIPVILITE_KEY_PARAMS	"params"
#define PAIPVILITE_KEY_ID		"id"

#define PAIPVILITE_METHOD_GETSERVERINFO	"getserverinfo"
#define PAIPVILITE_METHOD_GETALLPVLISTS	"getallpvlists"
#define PAIPVILITE_METHOD_CREATEPVLIST	"createpvlist"
#define PAIPVILITE_METHOD_GETPVLIST	"getpvlist"
#define PAIPVILITE_METHOD_SETPVLISTPROP	"setpvlistproperties"
#define PAIPVILITE_METHOD_DELETEPVLIST	"deletepvlist"
#define PAIPVILITE_METHOD_SETPVLIST "setpvlist"
#define PAIPVILITE_METHOD_GETPVARRAY "getpvarray"

#define PAIPVILITE_KEY_PVLISTNAME "pvlistname"
#define PAIPVILITE_KEY_PVLIST "pvlist"
#define PAIPVILITE_KEY_PVNAMESLIST "listofpvnames"
#define PAIPVILITE_KEY_PVVALLIST "listofpvvalues"
#define PAIPVILITE_KEY_ACTIVE "active"
#define PAIPVILITE_KEY_MODE "mode"
#define PAIPVILITE_KEY_FORMAT "format"
#define PAIPVILITE_KEY_MAXFREQ "maxupdatefreq"
#define PAIPVILITE_KEY_MINFREQ "minupdatefreq"
#define PAIPVILITE_KEY_HOSTIP "hostipaddr"
#define PAIPVILITE_KEY_HOSTPORT "hostport"
#define PAIPVILITE_KEY_PVARRNAME "pvarrayname"
#define PAIPVILITE_KEY_PVARRIDXBEG "pvarridxbeg"
#define PAIPVILITE_KEY_PVARRIDXEND "pvarridxend"


#define PAIPVILITE_ARRAUTOUPDATE_RATE_ms 100
#define PAIPVILITE_ARRAUTOUPDATE_ITEMSPERREPLY 25
#define PAIPVILITE_ARRAUTOUPDATE_DLY_ms 200


/**
 Simple logging
 Log goes to an array of PV strings > see _LOCAL STRING log[][] variable below
 Log output is visible via Automation Studio watch window
 Example of a log statement used within the code is: 
 DBG(lgr, "This is log example. %d ", value1);
 To complety disable logging from this source file, change next line to: #if 0
IMPORTANT:
==========
DBGINIT() statement MUST BE PLACED as a first statement within the task _INIT function!
Example:
void _INIT myTaskINIT(void)
{
	//TODO: Add code here
	//init logger object
	DBGINIT(lgr, log, LOG_ENTRY_COUNT, LOG_ENTRY_LENGTH);
}
*/
#if 1
#include <../../../bglib/bgPVLogger2.c>
#define LOG_ENTRY_COUNT 40	//max number of log entries
#define LOG_ENTRY_LENGTH  256 //max length of individual log message. Longer msg are truncated
_LOCAL STRING tasklog[LOG_ENTRY_COUNT][LOG_ENTRY_LENGTH]; //log output visible from Automation Studio!
static bgPVLogger_typ lgr;	//PV logger object
#else
#define DBG(loggerInstance, format, args...) ;
#define DBGINIT(loggerInstance, logoutput, logEntryCount, logEntrySize)  ;
#endif


#include "cJSON.h"
#include <math.h>

#define DBL_EPSILON 2.2204460492503131E-16
#define INT_MAX 2147483647
#define INT_MIN -2147483647

//Forward declarations
static char *snprint_value(cJSON *item,int depth,int fmt, char* pDst, int dstSize, int* charsWritten);
void paiPviLiteGetPVArrayRply(char* pDst, char* pArrName, int arrIdxBeg, int arrIdxEnd, int rpcId, int blockAutoUpdateInit);

/*
snprint_xxx()
- set of functions that print json elements to a preallocated string buffer and wo. the 
need to use memory allocation
- functions are modified from the original that are supplied by the cJSON library
*/

/* Render the number nicely from the given item into a string. */
static char *snprint_number(cJSON *item, char* pDst, int dstSize, int* charsWritten)
{
	int offset = *charsWritten;
	char *str = pDst + offset;
	
	double d=item->valuedouble;
	if (fabs(((double)item->valueint)-d)<=DBL_EPSILON && d<=INT_MAX && d>=INT_MIN)
	{
		if (str){
			(*charsWritten) += sprintf2(str,"%d",item->valueint);
		} 
	}
	else
	{
		if (str)
		{
			if (fabs(floor(d)-d)<=DBL_EPSILON && fabs(d)<1.0e60){
				(*charsWritten) += sprintf2(str,"%.0f",d);
			}
			else if (fabs(d)<1.0e-6 || fabs(d)>1.0e9){
				(*charsWritten) += sprintf2(str,"%e",d);
			}			
			else{
				(*charsWritten) += sprintf2(str,"%f",d);
			}												
		}
	}
	return str;
}

/* Render the cstring provided to an escaped version that can be printed. */
static char *snprint_string_ptr(const char *str, char* pDst, int dstSize, int* charsWritten)
{
	const char *ptr;char *ptr2,*out;int len=0;unsigned char token;
	int offset = *charsWritten;
	
	if (!str) return "";
	ptr=str;while ((token=*ptr) && ++len) {if (strchr("\"\\\b\f\n\r\t",token)) len++; else if (token<32) len+=5;ptr++;}
	
	out=pDst + (*charsWritten);
	(*charsWritten) += len + 2;
	if (!pDst) return 0;

	ptr2=out;ptr=str;
	*ptr2++='\"';
	while (*ptr)
	{
		if ((unsigned char)*ptr>31 && *ptr!='\"' && *ptr!='\\') *ptr2++=*ptr++;
		else
		{
			*ptr2++='\\';
			switch (token=*ptr++)
			{
				case '\\':	*ptr2++='\\';	break;
				case '\"':	*ptr2++='\"';	break;
				case '\b':	*ptr2++='b';	break;
				case '\f':	*ptr2++='f';	break;
				case '\n':	*ptr2++='n';	break;
				case '\r':	*ptr2++='r';	break;
				case '\t':	*ptr2++='t';	break;
				default: sprintf2(ptr2,"u%04x",token);ptr2+=5;	break;	/* escape and print */
			}
		}
	}
	*ptr2++='\"';*ptr2++=0;
	return out;
}

/* Invote print_string_ptr (which is useful) on an item. */
static char *snprint_string(cJSON *item, char* pDst, int dstSize, int* printLen)	{return snprint_string_ptr(item->valuestring, pDst, dstSize, printLen);}

/* Render an array to text */
static char *snprint_array(cJSON *item, int depth,int fmt, char* pDst, int dstSize, int* printLen)
{
	char **entries;
	char *out=0,*ptr,*ret;int len=5;
	cJSON *child=item->child;
	int numentries=0,i=0,fail=0;
	
	/* How many entries in the array? */
	while (child) numentries++,child=child->next;
	/* Explicitly handle numentries==0 */
	if (!numentries)
	{
		//out=(char*)cJSON_malloc(3);
		out = pDst + (*printLen);
		if (out) strcpy(out,"[]");
		(*printLen) += 2;
		return out;
	}
		
	/* Compose the output array. */
	out = pDst + (*printLen);
	*out='['; (*printLen)++;
	ptr=out+1;*ptr=0;
	child = item->child;
	for (i=0;i<numentries;i++)
	{
		snprint_value(child,depth+1,fmt, pDst, dstSize, printLen);
		ptr = pDst + (*printLen);
		if (i!=numentries-1) {*ptr++=','; (*printLen)++; if(fmt){*ptr++=' ';(*printLen)++;}*ptr=0;}
		child = child->next;
	}
	*ptr++=']';*ptr++=0;(*printLen)++;
	return out;	
}


/* Render an object to text. */
static char *snprint_object(cJSON *item,int depth,int fmt, char* pDst, int dstSize, int* printLen)
{
	char **entries=0,**names=0;
	char *out=0,*ptr,*ret,*str;int len=7,i=0,j;
	cJSON *child=item->child;
	int numentries=0,fail=0, offset=0;
	/* Count the number of entries. */
	while (child) numentries++,child=child->next;
	/* Explicitly handle empty object case */
	if (!numentries)
	{
		//out=(char*)cJSON_malloc(fmt?depth+3:3);
		out = pDst + (*printLen);
		if (!pDst)	return 0;
		ptr=out;*ptr++='{';(*printLen)++;
		if (fmt) {*ptr++='\n'; (*printLen)++; for (i=0;i<depth-1;i++) {*ptr++='\t';(*printLen)++;}}
		*ptr++='}';*ptr++=0;(*printLen)++;
		return out;
	}
	
	/* Compose the output: */
	out = pDst + (*printLen);
	//DBG(lgr, "objInChrs:%d", (*printLen));

	*out='{';ptr=out+1;(*printLen)++;if (fmt){*ptr++='\n';(*printLen)++;}*ptr=0;
	child = item->child;
	for (i=0;i<numentries;i++)
	{
			if (fmt) for (j=0;j<depth;j++){ *ptr++='\t';(*printLen)++;}
			
			snprint_string_ptr(child->string, pDst, dstSize, printLen);
			offset = *printLen;
			ptr = pDst + offset;
			*ptr++=':'; (*printLen)++; if (fmt) {*ptr++='\t';(*printLen)++;}
					
			snprint_value(child, depth, fmt, pDst, dstSize, printLen);
			offset = *printLen;
			ptr = pDst + offset;
			
			if (i!=numentries-1){ *ptr++=','; (*printLen)++;}
			
			if (fmt){ *ptr++='\n';(*printLen)++;}*ptr=0;
			
			child = child->next;
	}
	
	//cJSON_free(names);cJSON_free(entries);
	if (fmt) for (i=0;i<depth-1;i++){ *ptr++='\t';(*printLen)++;}
	*ptr++='}';*ptr++=0;(*printLen)++;
	return out;	
}

/* Render a value to text. */
static char *snprint_value(cJSON *item,int depth,int fmt, char* pDst, int dstSize, int* charsWritten)
{
	char *out=pDst;
	int offset = (*charsWritten);
	if (!item) return 0;
	switch ((item->type)&255)
	{
		case cJSON_NULL:	strcpy(pDst, "null");	break;
		case cJSON_False:	strcpy(pDst, "false");break;
		case cJSON_True:	strcpy(pDst, "true"); break;
		case cJSON_Number:	snprint_number(item, pDst, dstSize, charsWritten);break;
		case cJSON_String:	snprint_string(item, pDst, dstSize, charsWritten);break;
		case cJSON_Array:	snprint_array(item,depth,fmt, pDst, dstSize, charsWritten);break;
		case cJSON_Object:	snprint_object(item,depth,fmt, pDst, dstSize, charsWritten);break;
	}
	offset = (*charsWritten);
	return out;
}



static cJSON* cJSON_PrintChildItem(cJSON* item)
{
	char txtBuff[128];
	cJSON* itemNext;
	int charsWritten = 0;
	
	if(!item) return item;
	
	switch(item->type){
		case cJSON_False:
		case cJSON_True:
		case cJSON_NULL:
		case cJSON_Number:
		case cJSON_String:
			if(item->string){
				DBG(lgr, "%s=%s", item->string, snprint_value(item, 0, 1, txtBuff, 128, &charsWritten) );
			}else{
				DBG(lgr, "%s", snprint_value(item, 0, 1, txtBuff, 128, &charsWritten) );
			}
			return item->next;
		break;
		case cJSON_Array:
		case cJSON_Object:
			if(item->type == cJSON_Array){
				DBG(lgr, "Arr:%s", item->string?item->string:"n.a.");
			}else{
				DBG(lgr, "Obj:%s", item->string?item->string:"n.a.");
			}
			itemNext = item->next;
			item = item->child;
			while (	item = cJSON_PrintChildItem(item) );
			return itemNext;
		break;
		default:
			return 0;
		break;
	}
	return 0;
}

#if 0
static void bgGetPVValueText(char* pvValue, UDINT data_typ, UDINT data_len, UINT dimension, UDINT adr)
{
/*
	if( dimension != 1)
		DBG("TODO: display multidimensional values...");
*/
	switch(data_typ){
		case PB_DT_STRUCT:
			//DBG("TODO: display struct values...");
		break;	
		case PB_DT_BOOL:
			sprintf2(pvValue, "%d", *( (char*)adr ));
		break;	
		case PB_DT_INT8:
			sprintf2(pvValue, "%d", *( (char*)adr) );
		break;	
		case PB_DT_INT16:
			sprintf2(pvValue, "%d", *( (short*)adr ) );
		break;	
		case PB_DT_INT32:
			sprintf2(pvValue, "%d", *( (int*)adr ));
		break;	
		case PB_DT_BYTE:
			sprintf2(pvValue, "%u", *( (unsigned char*)adr));
		break;	
		case PB_DT_WORD:
			sprintf2(pvValue, "%u", *( (unsigned short*)adr));
		break;	
		case PB_DT_LONG:
			sprintf2(pvValue, "%u", *( (unsigned int*)adr));
		break;	
		case PB_DT_FLOAT:
		{
			char temp[32];
			ftoa( *((float*)adr), (UDINT)temp);
			sprintf2(pvValue, "%s", temp);
		}
		break;	
		case PB_DT_VIS:
			sprintf2(pvValue, "%s", (char*)adr );
		break;	
		case PB_DT_OCTET:
			//DBG("TODO: display octet values...");
		break;	
		case PB_DT_DATE:
			//DBG("TODO: display date values...");
		break;	
		case PB_DT_TIME:
			//DBG("TODO: display time values...");
		break;	
		case PB_DT_DIFF:
			//DBG("TODO: display time-diff values...");
		break;	
		case PB_DT_BIT:
			//DBG("TODO: display bit values...");
		break;	
		case PB_DT_ARRAY:
			//DBG("TODO: display array values...");
		break;	
	}
}

static void bgSetPVValueText(char* pvValue, UDINT data_typ, UDINT data_len, UINT dimension, UDINT adr)
{
/*
	if( dimension != 1)
		DBG("TODO: display multidimensional values...");
*/
	switch(data_typ){
		case PB_DT_STRUCT:
			//DBG("TODO: set struct values...");
		break;	
		case PB_DT_BOOL:
			sscanf2(pvValue, "%d", (char*)adr );
		break;	
		case PB_DT_INT8:
			sscanf2(pvValue, "%d", (char*)adr );
		break;	
		case PB_DT_INT16:
			sscanf2(pvValue, "%d", (short*)adr  );
		break;	
		case PB_DT_INT32:
			sscanf2(pvValue, "%d", (int*)adr );
		break;	
		case PB_DT_BYTE:
			sscanf2(pvValue, "%u", (unsigned char*)adr);
		break;	
		case PB_DT_WORD:
			sscanf2(pvValue, "%u", (unsigned short*)adr);
		break;	
		case PB_DT_LONG:
			sscanf2(pvValue, "%u", (unsigned int*)adr);
		break;	
		case PB_DT_FLOAT:
		{
			 *((float*)adr) = atof((UDINT)pvValue);
		}
		break;	
		case PB_DT_VIS:
		{
			int ii=0;
			//copy string
			//don't copy over the size of the dest buffer!
			//terminate properly
			char* src=(char*)pvValue;
			char* dst=(char*)adr;			
			//DBG("GBTST= %s len=%d", src, data_len);
			for(ii=0; ii<data_len; ii++){
				dst[ii] = src[ii];
				//if end of src string > exit
				if(!src[ii])
					break;
			}
			//src longer than dst > terminate dst
			dst[data_len-1] = 0;
		}
		break;	
		case PB_DT_OCTET:
			//DBG("TODO: display octet values...");
		break;	
		case PB_DT_DATE:
			//DBG("TODO: display date values...");
		break;	
		case PB_DT_TIME:
			//DBG("TODO: display time values...");
		break;	
		case PB_DT_DIFF:
			//DBG("TODO: display time-diff values...");
		break;	
		case PB_DT_BIT:
			//DBG("TODO: display bit values...");
		break;	
		case PB_DT_ARRAY:
			//DBG("TODO: display array values...");
		break;	
	}
}
#endif

static UDINT paiPviLiteGetStringPVLRC(char* pPv){
	int ii;
	USINT lrc = 0x00;
	int bLength = strlen(pPv);
	
	for (ii = 0; ii < bLength; ii++)
	{
		lrc = (USINT)((lrc + (USINT)pPv[ii]) & 0xFF);
	}
	lrc = (USINT)(((lrc ^ 0xff) + 2) & 0xFF);

	return (UDINT)lrc;
}

/* hash: compute hash value of string */
static UDINT hash(char *str)
{
	unsigned int h;
	unsigned char *p;
	unsigned int MULTIPLIER = 37;
	
	h = 0;
	for (p = (unsigned char*)str; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h; // or, h % ARRAY_SIZE;
}

static UDINT hash_array(char *str, int size)
{
	unsigned int h;
	unsigned char *p;
	unsigned int MULTIPLIER = 37;
	unsigned int ii;
	
	h = 0;
	for (p = (unsigned char*)str, ii=0; ii < size; p++, ii++)
		h = MULTIPLIER * h + *p;
	return h; // or, h % ARRAY_SIZE;
}


static void	paiPviLiteGetPVValue(paiPVIListItem_typ* pPvItem)
{
	int data_typ = pPvItem->pvType;
	UDINT pPv = pPvItem->pPvValue;
	UDINT* pPvOld = (UDINT)&pPvItem->pvValueOld;
	REAL* pPvOldFloat = (REAL*)&pPvItem->pvValueOldFloat;
	char* pPvStr = pPvItem->pvValueTxt;
	int data_len = sizeof(pPvItem->pvValueTxt);
	char pvStrOld[PAI_PVILIGHT_PV_VAL_TXT_LEN];
	//strcpy(pvStrOld, pPvStr);
	
	UDINT pvOldOld = (UDINT)pPvItem->pvValueOld;
	REAL pvOldFloatOld = (REAL)pPvItem->pvValueOldFloat;
	
	switch(data_typ){
		case PB_DT_STRUCT:
			//DBG("TODO: set struct values...");
			break;	
		case PB_DT_BOOL:
			(*pPvOld) = (UDINT)(*((BOOL*)pPv));
			break;	
		case PB_DT_INT8:
			(*pPvOld) = (UDINT)(*((SINT*)pPv));
			break;	
		case PB_DT_INT16:
			(*pPvOld) = (UDINT)(*((INT*)pPv));
			break;	
		case PB_DT_INT32:
			(*pPvOld) = (UDINT)(*((DINT*)pPv));
			break;	
		case PB_DT_BYTE:
			(*pPvOld) = (UDINT)(*((USINT*)pPv));
			break;	
		case PB_DT_WORD:
			(*pPvOld) = (UDINT)(*((UINT*)pPv));
			break;	
		case PB_DT_LONG:
			(*pPvOld) = (UDINT)(*((UDINT*)pPv));
			break;	
		case PB_DT_FLOAT:
			{
				//*((float*)adr) = atof((UDINT)pvValue);
				(*pPvOldFloat) = (float)*((float*)pPv);	 
			}
			break;	
		case PB_DT_VIS:
			{
				//For strings, use crc to detect string changes
				(*pPvOld) = hash((char*)pPv);
				
#if 0				
				int ii=0;
				//copy string
				//don't copy over the size of the dest buffer!
				//terminate properly
				char* src=(char*)pPv;
				char* dst=(char*)pPvStr;			
				//DBG("GBTST= %s len=%d", src, data_len);
				for(ii=0; ii<data_len; ii++){
					dst[ii] = src[ii];
					//if end of src string > exit
					if(!src[ii])
						break;
				}
				//src longer than dst > terminate dst
				dst[data_len-1] = 0;
#endif				
			}
			break;	
		case PB_DT_OCTET:
			//DBG("TODO: display octet values...");
			break;	
		case PB_DT_DATE:
			//DBG("TODO: display date values...");
			break;	
		case PB_DT_TIME:
			//DBG("TODO: display time values...");
			break;	
		case PB_DT_DIFF:
			//DBG("TODO: display time-diff values...");
			break;	
		case PB_DT_BIT:
			//DBG("TODO: display bit values...");
			break;	
		case PB_DT_ARRAY:
			//DBG("TODO: display array values...");
			break;	
	}

	//convert value to string
	if(data_typ != PB_DT_VIS){
	
		bgGetPVValueText(pPvItem->pvValueTxt,
			pPvItem->pvType,
			pPvItem->pvLen,
			pPvItem->pvDimension,
			pPvItem->pPvValue );
		
	}

	
	if( (pvOldOld != (*pPvOld)) || (pvOldFloatOld != (*pPvOldFloat)) /*|| strcmp(pvStrOld, pPvStr)*/ ){
		pPvItem->isChanged = 1;
	}
}

/*
Example:
{"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}
*/
static void paiPviLiteRpcCmdReplyError(char* pDst, char* errorText, int errorCode, int cmdId)
{
	if(!pDst || !errorText)
		return;
		
	sprintf2(pDst, "{\"paipvilite\": \"1.0\", \"error\": {\"code\" : %d, \"message\" : \"%s\"}, \"id\" : %d}", errorCode, errorText, cmdId);
}

/*
Example:
{"jsonrpc": "2.0", "result": 19, "id": 4}
*/
static void paiPviLiteRpcCmdReplyOk(char* pDst, int resultCode, int cmdId)
{
	if(!pDst)
		return;
		
	sprintf2(pDst, "{\"paipvilite\": \"1.0\", \"result\" : %d, \"id\" : %d}", resultCode, cmdId);
}

static void paiPviLiteRpcCmdReplyErrorSend(char* errorText, int errorCode, int rpcId)
{	
	paiPviLiteRpcCmdReplyError(gJsontextTxRply, errorText?errorText:"None", errorCode, rpcId);
	gTxRply = 1;
	
	gUdpClient.clientIf.iRmtUdpPort = 9998;
	strcpy(gUdpClient.clientIf.iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);
	gUdpClient.clientIf.iConnect = 1;
	gUdpClient.clientIf.iSend = 1;
}

static void	paiPviLiteRpcCmdReplyOkSend(int resultCode, int rpcId)
{
	paiPviLiteRpcCmdReplyOk(gJsontextTxRply, resultCode, rpcId);
	gTxRply = 1;

	gUdpClient.clientIf.iRmtUdpPort = 9998;
	strcpy(gUdpClient.clientIf.iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);
	gUdpClient.clientIf.iConnect = 1;
	gUdpClient.clientIf.iSend = 1;	
}

void paiPviLiteUpdatePVListValues()
{
	int ii, jj;

	//iterate all lists
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if( !gPaiPVILists.listCollection[ii].isUsed ){
			continue;
		}
		
		//iterate all items within a list
		for(jj=0; jj<=gPaiPVILists.listCollection[ii].itemCount; jj++){
			if( gPaiPVILists.listCollection[ii].itemList[jj].isError ){
				continue;
			}
			
			paiPviLiteGetPVValue(&gPaiPVILists.listCollection[ii].itemList[jj]);
			if(gPaiPVILists.listCollection[ii].itemList[jj].isChanged){
				gPaiPVILists.listCollection[ii].itemList[jj].isChanged = 0;
				gPaiPVILists.listCollection[ii].isChanged = 1;
			}	
		}
	}
}

void paiPviLiteSendEvents()
{
	int ii, jj;

	
	//iterate all lists
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
	
		//list is used & active > operate notification timers
		bgTimerCyclic(&gPaiPVILists.listCollection[ii].notificationTimerMax); 
		bgTimerCyclic(&gPaiPVILists.listCollection[ii].notificationTimerMin); 
	
		//if list not used or not active > skip, go to next
		if( !gPaiPVILists.listCollection[ii].isUsed || 
			!gPaiPVILists.listCollection[ii].isActive){
			continue;
		}

		
		switch(gPaiPVILists.listCollection[ii].notificationMode){
			//=====================================================================================
			case 1://periodic notifications only, uses min frequency
				if(!bgTimerIsTimeOut(&gPaiPVILists.listCollection[ii].notificationTimerMax)){
					continue;
				}
			break;
			//=====================================================================================
			case 2://periodic notifications only, uses max frequency
				if(!bgTimerIsTimeOut(&gPaiPVILists.listCollection[ii].notificationTimerMin)){
					continue;
				}
			break;
			//=====================================================================================
			default://on change + periodic notifications, uses both min & max frequency
				//min timer handles max frequency > skip further processing until this timer expires
				if(!bgTimerIsTimeOut(&gPaiPVILists.listCollection[ii].notificationTimerMin)){
					continue;
				}
				//check if entry has changed OR the max timer has expired (max timer handles min freq)
				if(!gPaiPVILists.listCollection[ii].isChanged && !bgTimerIsTimeOut(&gPaiPVILists.listCollection[ii].notificationTimerMax) && (gPaiPVILists.listCollection[ii].isActive == 1)){
					//restart the min timer
					bgTimerStart(&gPaiPVILists.listCollection[ii].notificationTimerMin, (int)(gPaiPVILists.listCollection[ii].notificationTimeMinPeriod_s * 1000) );
					continue;
				}		
			break;					
			//=====================================================================================
		}		
		
		//reset first scan update notification to 1 (back from 2 set during setpvlistproperties)
		gPaiPVILists.listCollection[ii].isActive = 1;
		
		//semaphor to coordinate UDP send
		if(	gTxEvent ){
			//trigger UDP send
			gUdpClient.clientIf.iRmtUdpPort = gPaiPVILists.listCollection[ii].iRmtUdpPort;
			strcpy(gUdpClient.clientIf.iRmtIPAddr, gPaiPVILists.listCollection[ii].iRmtIPAddr);
			gUdpClient.clientIf.iConnect = 1;
			gUdpClient.clientIf.iSend = 1;
			continue;
		}
		
		//restart the min timer
		bgTimerStart(&gPaiPVILists.listCollection[ii].notificationTimerMin, (int)(gPaiPVILists.listCollection[ii].notificationTimeMinPeriod_s * 1000) );
		//restart the max timer
		bgTimerStart(&gPaiPVILists.listCollection[ii].notificationTimerMax, (int)(gPaiPVILists.listCollection[ii].notificationTimeMaxPeriod_s * 1000) );
				
		DBG(lgr, "list='%s' %s", gPaiPVILists.listCollection[ii].listName, gPaiPVILists.listCollection[ii].isChanged?"onchange event":"timer event" );
		paiPviLiteRpcPvListEvent(gJsontext, &gPaiPVILists.listCollection[ii] );
		gPaiPVILists.listCollection[ii].isChanged = 0;
		gTxEvent = 1;
		
		//trigger UDP send
		gUdpClient.clientIf.iRmtUdpPort = gPaiPVILists.listCollection[ii].iRmtUdpPort;
		strcpy(gUdpClient.clientIf.iRmtIPAddr, gPaiPVILists.listCollection[ii].iRmtIPAddr);
		gUdpClient.clientIf.iConnect = 1;
		gUdpClient.clientIf.iSend = 1;
		continue;
	}
}		



void paiPviLiteSendArrayAutoUpdates()
{
	int ii, jj;
	int arrIdxBeg;
	int arrIdxEnd;
	
	bgTimerCyclic(&gPaiPVILists.arrayListDlyTimer); 
	
	if(!bgTimerIsTimeOut(&gPaiPVILists.arrayListDlyTimer))
		return;
	
	//iterate all lists
	for(ii=0; ii<= PAI_PVILITE_ARRLIST_INDEX_MAX; ii++){
		//if list not used or not active > skip, go to next
		if( !gPaiPVILists.arrayListCollection[ii].isUsed){
			continue;
		}

	
		//updates are active > operate notification timers
		bgTimerCyclic(&gPaiPVILists.arrayListCollection[ii].notificationTimer); 
			
		if(!bgTimerIsTimeOut(&gPaiPVILists.arrayListCollection[ii].notificationTimer))
			return;
					
		//semaphor to coordinate UDP send
		if(	gTxRply ){
			return;
		}
		
		arrIdxBeg = gPaiPVILists.arrayListCollection[ii].arrIdxNext;		
				
		arrIdxEnd = arrIdxBeg + gPaiPVILists.arrayListCollection[ii].arrItemsPerUpdate;
		if(arrIdxEnd > (gPaiPVILists.arrayListCollection[ii].pvArrDimension - 1)){
			arrIdxEnd = gPaiPVILists.arrayListCollection[ii].pvArrDimension - 1;
			gPaiPVILists.arrayListCollection[ii].isUsed = 0;
		}
		gPaiPVILists.arrayListCollection[ii].arrIdxNext = arrIdxEnd + 1;
		
		DBG(lgr, "pvArrAutoUpdate PVarr='%s' begIdx=%d endIdx=%d", gPaiPVILists.arrayListCollection[ii].pvArrName, arrIdxBeg, arrIdxEnd);		
		paiPviLiteGetPVArrayRply(gJsontextTxRply, gPaiPVILists.arrayListCollection[ii].pvArrName, arrIdxBeg, arrIdxEnd, gPaiPVILists.arrayListCollection[ii].rpcId, 1);
		
		gTxRply = 1;
		//trigger UDP send
		gUdpClient.clientIf.iRmtUdpPort = gPaiPVILists.arrayListCollection[ii].iRmtUdpPort;
		strcpy(gUdpClient.clientIf.iRmtIPAddr, gPaiPVILists.arrayListCollection[ii].iRmtIPAddr);
		gUdpClient.clientIf.iConnect = 1;
		gUdpClient.clientIf.iSend = 1;
					
		//restart notify timer
		bgTimerStart(&gPaiPVILists.arrayListCollection[ii].notificationTimer, PAIPVILITE_ARRAUTOUPDATE_RATE_ms);		
		return;//NOTE: must exit here!
	}
}		


void paiPviLiteCreatePVList(cJSON* params, int rpcId)
{
	cJSON *item, *pvlistName, *pvListItems;
	int itemCount;
	int ii;
	int listIndex;
	UINT status; 
	
	DBG(lgr, "createpvlist BEGIN");
	
	if(!params){
		DBG(lgr, "createpvlist error: params not defined");
		paiPviLiteRpcCmdReplyErrorSend("parametars not defined", -9, rpcId);		
		return;		
	}
	
	pvlistName = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVLISTNAME);
	pvListItems = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVLIST);
	
	if(!pvlistName){
		DBG(lgr, "createpvlist error: list name not defined");
		paiPviLiteRpcCmdReplyErrorSend("list name not defined", -10, rpcId);		
		return;		
	}
	
	if(!pvListItems){
		DBG(lgr, "createpvlist error: pv list not defined");
		paiPviLiteRpcCmdReplyErrorSend("pv list not defined", -11, rpcId);		
		return;		
	}

	if(pvListItems->type != cJSON_Array){
		DBG(lgr, "createpvlist error: pv list shall be a string array");
		paiPviLiteRpcCmdReplyErrorSend("pv list shall be a string array", -12, rpcId);		
		return;		
	}
	
	itemCount = cJSON_GetArraySize(pvListItems);
	if(!itemCount){
		DBG(lgr, "createpvlist error: pv list is empty");
		paiPviLiteRpcCmdReplyErrorSend("pv list is empty", -13, rpcId);		
		return;		
	}
	
	if( gPaiPVILists.listCount > (PAI_PVILIGHT_LIST_INDEX_MAX+1) ){
		DBG(lgr, "createpvlist error: pv list mgr is full");
		paiPviLiteRpcCmdReplyErrorSend("pv list mgr is full", -14, rpcId);		
		return;		
	}
	
	listIndex = -1;
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if( gPaiPVILists.listCollection[ii].isUsed && !strcmp(gPaiPVILists.listCollection[ii].listName, pvlistName->valuestring)){
			//DBG(lgr, "createpvlist error: pv list name '%s' already exists", pvlistName->valuestring);
			//paiPviLiteRpcCmdReplyErrorSend("pv list name exists", -15, rpcId);		
			//return;
			listIndex = ii;
			break;	
			//NOTE: allow create to succed if the same name already exists > simply replace the list		
		}
	}
	
	if(listIndex < 0){
		//list does not exist > find an empty spot
		listIndex = -1;
		for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
			if( !gPaiPVILists.listCollection[ii].isUsed ){
				listIndex = ii;
				gPaiPVILists.listCollection[listIndex].isUsed = 1;
				strcpy(gPaiPVILists.listCollection[listIndex].listName, pvlistName->valuestring);
				break;
			}
	}
	}else{
		//list with the same name already exist > recreate the list using new data
	}
	
	
	if(listIndex < 0){
		return;
	}
	
	//mark all list items unused
	for(ii=0; ii<= PAI_PVILIGHT_LIST_ITEM_INDEX_MAX; ii++){
		gPaiPVILists.listCollection[listIndex].itemList[ii].isUsed = 0;
		gPaiPVILists.listCollection[listIndex].itemList[ii].isError = 0;
	}
	gPaiPVILists.listCollection[listIndex].itemCount = 0;
	
	//process all PV items from the rpc call and copy it localy
	item = pvListItems->child;
	ii = 0;
	while( item ){
		if(item->type != cJSON_String)
			continue;
			
		gPaiPVILists.listCollection[listIndex].itemList[ii].isUsed = 1;
		strcpy(gPaiPVILists.listCollection[listIndex].itemList[ii].pvName, item->valuestring);
		gPaiPVILists.listCollection[listIndex].itemCount++;
		item = item->next;
		ii++;
		if(ii > PAI_PVILIGHT_LIST_ITEM_INDEX_MAX){
			DBG(lgr, "createpvlist warning: pv list contains too many items");
			paiPviLiteRpcCmdReplyErrorSend("pv list with too many items", -16, rpcId);		
			break;			
		}
	}
	
	//discover all PV-s
	for(ii=0; ii<gPaiPVILists.listCollection[listIndex].itemCount; ii++){
		UDINT data_typ, data_len, dimension;
		status = PV_ninfo(gPaiPVILists.listCollection[listIndex].itemList[ii].pvName, &data_typ, &data_len, &dimension);
		if( status ){
			gPaiPVILists.listCollection[listIndex].itemList[ii].isError = 1;
			sprintf2(gPaiPVILists.listCollection[listIndex].itemList[ii].pvValueTxt, "PV Error=%d", status);
			continue;
		}
		gPaiPVILists.listCollection[listIndex].itemList[ii].pvType = data_typ;
		gPaiPVILists.listCollection[listIndex].itemList[ii].pvDimension = dimension;
		
		status = PV_xgetadr(gPaiPVILists.listCollection[listIndex].itemList[ii].pvName, 
		&gPaiPVILists.listCollection[listIndex].itemList[ii].pPvValue, &gPaiPVILists.listCollection[listIndex].itemList[ii].pvLen );
	
		if( status ){
			gPaiPVILists.listCollection[listIndex].itemList[ii].isError = 1;
			sprintf2(gPaiPVILists.listCollection[listIndex].itemList[ii].pvValueTxt, "PV Error=%d", status);
			continue;
		}	
	}
	
	//Set default properties of the PV list
	gPaiPVILists.listCollection[listIndex].isActive = 0;	
	gPaiPVILists.listCollection[listIndex].isChanged = 0;	
	gPaiPVILists.listCollection[listIndex].iRmtUdpPort = 0;	
	strcpy(gPaiPVILists.listCollection[listIndex].iRmtIPAddr, "");
	gPaiPVILists.listCollection[listIndex].notificationFormat = 0;	
	gPaiPVILists.listCollection[listIndex].notificationMode = 0;	
	gPaiPVILists.listCollection[listIndex].notificationTimeMinPeriod_s = 1.0;	
	gPaiPVILists.listCollection[listIndex].notificationTimeMaxPeriod_s = 10.0;	
	
	paiPviLiteRpcCmdReplyOkSend(0, rpcId);		
	
	DBG(lgr, "createpvlist END");
}

void paiPviLiteGetPVList(cJSON* params, int rpcId)
{

}

void paiPviLiteGetAllPVLists(cJSON* params, int rpcId)
{
	int ii, jj;
	char* pDst = gJsontextTxRply;
	int resultCode = 0;
	int itemCount = 0;
	char text[32];
	
	//define start of the message
	sprintf2(pDst, "{\"paipvilite\": \"1.0\", \"result\": {\"code\" : %d, \"namespvlists\" : [", 0);
	
	itemCount = 0;	
	for(ii=0; ii<=PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if(!gPaiPVILists.listCollection[ii].isUsed){
			continue;
		} 
		itemCount++;
	}
	
	if(itemCount){
		//add pv list names as list items
		for(ii=0, jj=0; (ii<=PAI_PVILIGHT_LIST_INDEX_MAX) && (jj<itemCount); ii++){
			if(!gPaiPVILists.listCollection[ii].isUsed){
				continue;
			} 
			strcat(pDst, "\"");
			strcat(pDst, gPaiPVILists.listCollection[ii].listName );
			if(jj < (itemCount-1)){
				strcat(pDst, "\",");
			}else{
				strcat(pDst, "\"");		
			}
			jj++;
		}
	}
	
	strcat(pDst, "]}, \"id\" : ");
	sprintf2(text, "%d}", rpcId);
	strcat(pDst, text);
	
	gTxRply = 1;

	gUdpClient.clientIf.iRmtUdpPort = 9998;
	strcpy(gUdpClient.clientIf.iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);
	gUdpClient.clientIf.iConnect = 1;
	gUdpClient.clientIf.iSend = 1;		
}

void paiPviLiteGetServerInfo(cJSON* params, int rpcId)
{
	int ii;
	char* pDst = gJsontextTxRply;
	int resultCode = 0;
	
	sprintf2(pDst, "{\"paipvilite\": \"1.0\", \"result\" : {\"code\":%d, \"serverUDPPort\":%d, \"maxUDPMsgLen\":%d, \"maxNumberPvLists\":%d, \"maxItemsPerPVList\":%d, \"maxPVNameLen\":%d}, \"id\" : %d}", 
	resultCode,
	9999,
	PAI_PVILIGHT_UDP_MSG_SIZE_MAX,
	PAI_PVILIGHT_LIST_INDEX_MAX + 1,
	PAI_PVILIGHT_LIST_ITEM_INDEX_MAX + 1,
	PAI_PVILIGHT_PV_NAME_LEN,
	rpcId);
	
	gTxRply = 1;

	gUdpClient.clientIf.iRmtUdpPort = 9998;
	strcpy(gUdpClient.clientIf.iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);
	gUdpClient.clientIf.iConnect = 1;
	gUdpClient.clientIf.iSend = 1;	
}


void paiPviLiteDeletePVList(cJSON* params, int rpcId)
{
	cJSON *item, *pvlistName, *pvListItems;
	int itemCount;
	int ii;
	int listIndex;
	UINT status; 
	
	DBG(lgr, "deletepvlist BEGIN");
	
	if(!params){
		DBG(lgr, "deletepvlist error: params not defined");
		paiPviLiteRpcCmdReplyErrorSend("parametars not defined", -9, rpcId);		
		return;		
	}
	
	pvlistName = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVLISTNAME);
	
	if(!pvlistName){
		DBG(lgr, "deletepvlist error: list name not defined");
		paiPviLiteRpcCmdReplyErrorSend("list name not defined", -10, rpcId);		
		return;		
	}
	
	listIndex = -1;
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if( gPaiPVILists.listCollection[ii].isUsed && !strcmp(gPaiPVILists.listCollection[ii].listName, pvlistName->valuestring)){
			listIndex = ii;
			break;			
		}
	}
	if(listIndex < 0){
		DBG(lgr, "deletepvlist error: pv list name '%s' not found", pvlistName->valuestring);
		paiPviLiteRpcCmdReplyErrorSend("pv list name not found", -17, rpcId);		
		return;			
	}
	
	gPaiPVILists.listCollection[listIndex].isUsed = 0;
	
	//Fix a problem for a list create > delete > create  sequence.
	//Destroy old values in order to trigger an immmidate updates ones the list get created again following a delete.
	for(ii=0; ii<PAI_PVILIGHT_LIST_ITEM_INDEX_MAX; ii++){
		gPaiPVILists.listCollection[listIndex].itemList[ii].pvValueOld = 0;
		gPaiPVILists.listCollection[listIndex].itemList[ii].pvValueOldFloat = 0.0;
		gPaiPVILists.listCollection[listIndex].itemList[ii].pvValueTxt[0] = 0;			
	}
	 	
	paiPviLiteRpcCmdReplyOkSend(0, rpcId);		
	DBG(lgr, "deletepvlist END");
}

void paiPviLiteSetPVListProps(cJSON* params, int rpcId)
{
	cJSON *item, *pvlistName, *pvListItems, *pActive, *pMode, *pFormat, *pIPAddr, *pIPPort, *pMinFreq, *pMaxFreq;
	int itemCount;
	int ii;
	int listIndex;
	UINT status; 
	int itemFound;
	
	DBG(lgr, "setpvlistproperties BEGIN");
	
	if(!params){
		DBG(lgr, "setpvlistprops error: params not defined");
		paiPviLiteRpcCmdReplyErrorSend("parametars not defined", -9, rpcId);		
		return;		
	}
	
	pvlistName = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVLISTNAME);
	pActive = cJSON_GetObjectItem(params, PAIPVILITE_KEY_ACTIVE);
	pMode = cJSON_GetObjectItem(params, PAIPVILITE_KEY_MODE);
	pFormat = cJSON_GetObjectItem(params, PAIPVILITE_KEY_FORMAT);
	pMaxFreq = cJSON_GetObjectItem(params, PAIPVILITE_KEY_MAXFREQ);
	pMinFreq = cJSON_GetObjectItem(params, PAIPVILITE_KEY_MINFREQ);
	pIPAddr = cJSON_GetObjectItem(params, PAIPVILITE_KEY_HOSTIP);
	pIPPort = cJSON_GetObjectItem(params, PAIPVILITE_KEY_HOSTPORT);
	
	
	if(!pvlistName){
		DBG(lgr, "setpvlistprops error: list name not defined");
		paiPviLiteRpcCmdReplyErrorSend("list name not defined", -10, rpcId);		
		return;		
	}
	
	listIndex = -1;
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if( gPaiPVILists.listCollection[ii].isUsed && !strcmp(gPaiPVILists.listCollection[ii].listName, pvlistName->valuestring)){
			listIndex = ii;
			break;			
		}
	}
	if(listIndex < 0){
		DBG(lgr, "setpvlistprops error: pv list name '%s' not found", pvlistName->valuestring);
		paiPviLiteRpcCmdReplyErrorSend("pv list name not found", -17, rpcId);		
		return;			
	}
	
	
	//list found > process parametars from the rpc and apply it to the list
	if(pActive){
		gPaiPVILists.listCollection[listIndex].isActive = pActive->valueint ? 1 : 0;
		DBG(lgr, "setpvlistprops: list='%s', Active=%d", pvlistName->valuestring, gPaiPVILists.listCollection[listIndex].isActive);
	}

	if(pMode){
		if(pMode->valueint > 2){
			pMode->valueint = 0;
		}
		gPaiPVILists.listCollection[listIndex].notificationMode = pMode->valueint;
		DBG(lgr, "setpvlistprops: list='%s', Mode=%d", pvlistName->valuestring, gPaiPVILists.listCollection[listIndex].notificationMode);
	}	

	if(pFormat){
		if(pFormat->valueint > 1){
			pFormat->valueint = 0;
		}
		gPaiPVILists.listCollection[listIndex].notificationFormat = pFormat->valueint;
		DBG(lgr, "setpvlistprops: list='%s', Format=%d", pvlistName->valuestring, gPaiPVILists.listCollection[listIndex].notificationFormat);
	}	

	if(pMaxFreq){
		if(pMaxFreq->valuedouble > 100.0){
			pMaxFreq->valuedouble = 100.0;
		}
		if(pMaxFreq->valuedouble < 0.1){
			pMaxFreq->valuedouble = 0.1;
		}
		gPaiPVILists.listCollection[listIndex].notificationTimeMinPeriod_s = (1.0 / pMaxFreq->valuedouble);
		DBG(lgr, "setpvlistprops: list='%s'", pvlistName->valuestring);
	}	
	
	if(pMinFreq){
		if(pMinFreq->valuedouble < 0.01){
			pMinFreq->valuedouble = 0.01;
		}
		if(pMinFreq->valuedouble > pMaxFreq->valuedouble){
			pMinFreq->valuedouble = pMaxFreq->valuedouble;
		}
		gPaiPVILists.listCollection[listIndex].notificationTimeMaxPeriod_s = (1.0 / pMinFreq->valuedouble);
		DBG(lgr, "setpvlistprops: list='%s'", pvlistName->valuestring);
	}	

		
	if(pIPAddr && pIPAddr->valuestring){
		strcpy(gPaiPVILists.listCollection[listIndex].iRmtIPAddr,  pIPAddr->valuestring);	
		DBG(lgr, "setpvlistprops: list='%s', rmtIpAddr=%s", pvlistName->valuestring, gPaiPVILists.listCollection[listIndex].iRmtIPAddr);
	}	

	if(pIPPort){
		gPaiPVILists.listCollection[listIndex].iRmtUdpPort = pIPPort->valueint;
		DBG(lgr, "setpvlistprops: list='%s', rmtUdpPort=%d", pvlistName->valuestring, gPaiPVILists.listCollection[listIndex].iRmtUdpPort);
	}	

	if(gPaiPVILists.listCollection[listIndex].isActive){
		DBG(lgr, "setpvlistprops: start update timers");
		gPaiPVILists.listCollection[listIndex].isActive = 2;//send first update immidiately!!!
		bgTimerStart(&gPaiPVILists.listCollection[listIndex].notificationTimerMin, (int)(gPaiPVILists.listCollection[listIndex].notificationTimeMinPeriod_s * 1000) );
		bgTimerStart(&gPaiPVILists.listCollection[listIndex].notificationTimerMax, (int)(gPaiPVILists.listCollection[listIndex].notificationTimeMaxPeriod_s * 1000) );
	}
	
	paiPviLiteRpcCmdReplyOkSend(0, rpcId);		
	DBG(lgr, "setpvlistproperties END");
}

/*
Example:	
request:	
{"paipvilite": "1.0", "method": "setpvlist", "params": {"pvlistname": "pvitest1", "pvlistitems" : ["gPV1num","gPV2float","gPV3str"], "pvlistitemvalues" : [345, 3.140000, "Hello paiPVILite"], "id": 9}
Response- ok:	
{"paipvilite": "1.0", "result" : 0, "id" : 9}
Response - error:	
{"paipvilite": "1.0", "error": {"code" : -15, "message" : "pv list name does not exist"}, "id" : 9}
*/
void paiPviLiteSetPVList(cJSON* params, int rpcId)
{
	cJSON *item, *itemValue, *pvlistName, *pvListItems, *pvListItemValues;
	int pvNamesCount;
	int pvValuesCount;
	int ii;
	int listIndex, listItemIndex;
	UINT status; 
	int itemFound;
	
	DBG(lgr, "setpvlist BEGIN");
	
	if(!params){
		DBG(lgr, "setpvlist error: params not defined");
		paiPviLiteRpcCmdReplyErrorSend("parametars not defined", -9, rpcId);		
		return;		
	}
	
	pvlistName = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVLISTNAME);
	pvListItems = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVNAMESLIST);
	pvListItemValues = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVVALLIST);
	
	if(!pvlistName){
		DBG(lgr, "setpvlist error: list name not defined");
		paiPviLiteRpcCmdReplyErrorSend("list name not defined", -10, rpcId);		
		return;		
	}

	if(!pvListItems){
		DBG(lgr, "setpvlist error: PV items not defined");
		paiPviLiteRpcCmdReplyErrorSend("PV items not defined", -11, rpcId);		
		return;		
	}

	if(!pvListItemValues){
		DBG(lgr, "setpvlist error: PV values not defined");
		paiPviLiteRpcCmdReplyErrorSend("PV values not defined", -12, rpcId);		
		return;		
	}
	
	if(pvListItems->type != cJSON_Array){
		DBG(lgr, "setpvlist error: pv names list shall be a string array");
		paiPviLiteRpcCmdReplyErrorSend("pv list shall be a string array", -13, rpcId);		
		return;		
	}
	
	if(pvListItemValues->type != cJSON_Array){
		DBG(lgr, "setpvlist error: pv values list shall be a string array");
		paiPviLiteRpcCmdReplyErrorSend("pv val list shall be an array of values", -14, rpcId);		
		return;		
	}
	
	pvNamesCount = cJSON_GetArraySize(pvListItems);
	pvValuesCount = cJSON_GetArraySize(pvListItemValues);
	if(pvNamesCount != pvValuesCount){
		DBG(lgr, "setpvlist error: different number of items in pv names and pv values lists");
		paiPviLiteRpcCmdReplyErrorSend("different number of items in PV names and value lists", -15, rpcId);		
		return;		
	}
	
	//find requested PV list by name	
	listIndex = -1;
	for(ii=0; ii<= PAI_PVILIGHT_LIST_INDEX_MAX; ii++){
		if( gPaiPVILists.listCollection[ii].isUsed && !strcmp(gPaiPVILists.listCollection[ii].listName, pvlistName->valuestring)){
			listIndex = ii;
			break;			
		}
	}
	if(listIndex < 0){
		DBG(lgr, "setpvlist error: pv list name '%s' not found", pvlistName->valuestring);
		paiPviLiteRpcCmdReplyErrorSend("pv list name not found", -17, rpcId);		
		return;			
	}
	
	
	//list found > process parametars from the rpc and apply it
	item = pvListItems->child;
	itemValue = pvListItemValues->child;
	ii = 0;
	{
		char txt[128];
		char errorReply[300];
		errorReply[0] = 0;
		int jj;
		
		while( item ){
			//check the type of the PV name
			if(item->type != cJSON_String){
				sprintf2(txt, "pv name err @ %d,", ii);
				DBG(lgr, "setpvlist error: %s", txt);
				if(strlen(errorReply) < 200){
					strcat(errorReply, txt);
				}else{
					strcat(errorReply, "too many errors! call interrupted");
					paiPviLiteRpcCmdReplyErrorSend(errorReply, -18, rpcId);		
					return;			
				}
				continue;
			}
		
			//find a PV name in the PV list
			listItemIndex = -1;
			for(jj=0; jj<= PAI_PVILIGHT_LIST_ITEM_INDEX_MAX; jj++){
				if( !strcmp(gPaiPVILists.listCollection[listIndex].itemList[jj].pvName, item->valuestring)){
					listItemIndex = jj;
					break;			
				}
			}
			
			if(listItemIndex < 0){
				sprintf2(txt, "pv name %s @ %d not found,", item->valuestring, ii);
				DBG(lgr, "setpvlist error: pv name '%s' (@%d) not found", item->valuestring, ii);
				if(strlen(errorReply) < 200){
					strcat(errorReply, txt);
				}else{
					strcat(errorReply, "too many errors! call interrupted");
					paiPviLiteRpcCmdReplyErrorSend(errorReply, -18, rpcId);		
					return;			
				}
				continue;
			}
			
			//PV found! set it to a requested value
			bgStrCpyN(txt, itemValue->valuestring, 128);
			if(itemValue->type == cJSON_False){
				bgStrCpyN(txt, "0", 128);	
			}else
			if(itemValue->type == cJSON_True){
				bgStrCpyN(txt, "1", 128);	
			}else
			if(itemValue->type == cJSON_NULL){
				bgStrCpyN(txt, "0", 128);	
			}else
			if(itemValue->type == cJSON_Number){
				if(gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].pvType == PB_DT_FLOAT){
					sprintf2(txt, "%f", itemValue->valuedouble);					
				}else{
					sprintf2(txt, "%d", itemValue->valueint);					
				}
			}else
			if(itemValue->type == cJSON_String){
				bgStrCpyN(txt, itemValue->valuestring, 128);	
			}else
			if(itemValue->type == cJSON_Array){
				bgStrCpyN(txt, "0", 128);	
			}else
			if(itemValue->type == cJSON_Object){
				bgStrCpyN(txt, "0", 128);	
			}else{
				bgStrCpyN(txt, "0", 128);	
			}			



			if(gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].isUsed ){
				DBG(lgr, "GBsetpvlist: idx=%d pvn=%s pvv=%s ", listItemIndex, item->valuestring, txt );
				//apply new value only when item is used
				bgSetPVValueText(txt,
								 gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].pvType,
								 gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].pvLen,
								 gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].pvDimension,
								 gPaiPVILists.listCollection[listIndex].itemList[listItemIndex].pPvValue);
			}
			
			item = item->next;
			itemValue = itemValue->next;
			ii++;
		}
	}

//===========================	
	
	paiPviLiteRpcCmdReplyOkSend(0, rpcId);		
	DBG(lgr, "setpvlist END");
}

/*
Request:
{"paipvilite": "1.0", "method": "getpvarray", "params": {"pvarrayname": <string>, "pvarridxbeg":<num>, "pvarridxend":<num>}, "id": <calId>}


Response:
{"paipvilite": "1.0", "result" : {"pvarrayname": <string>, "pvarridxbeg":<num>, "pvarridxend":<num>, “pvarr”:<serialized_comma_delimited_pv_array_string>}, "id" : <callId>}

*/
void paiPviLiteGetPVArrayRply(char* pDst, char* pArrName, int arrIdxBeg, int arrIdxEnd, int rpcId, int blockAutoUpdateInit)
{
	int itemCount;
	DINT charsWritten;
	char tempTxt[128];
	int ii;
	int found=0;

	sprintf2(pDst, "{\"paipvilite\": \"1.0\", \"result\" : {\"method\":\"%s\", \"pvarrname\":\"%s\", \"pvarridxbeg\":%d, \"pvarridxend\":%d, \"pvarr\":\"",
		PAIPVILITE_METHOD_GETPVARRAY,
		pArrName,
		arrIdxBeg,
		arrIdxEnd);
	
	itemCount = strlen(pDst);
	//charsWritten = bgPVRootArraySerialize(pArrName->valuestring, (UDINT)&pDst[itemCount], PAI_PVILIGHT_UDP_MSG_SIZE_MAX - itemCount - 25, arrIdxBeg, arrIdxEnd);
	charsWritten = bgPVRootArraySerializeJSON(pArrName, (UDINT)&pDst[itemCount], PAI_PVILIGHT_UDP_MSG_SIZE_MAX - itemCount - 25, arrIdxBeg, arrIdxEnd);
	//charsWritten = sprintf2(&pDst[itemCount], "Hello!");//test only
	
	if(charsWritten < 0){
		DBG(lgr, "getpvarray: can't serilize pvarray (err=%d)", charsWritten);
		paiPviLiteRpcCmdReplyErrorSend("getpvarray error", charsWritten, rpcId);		
		return;		
	}
	
	sprintf2(&tempTxt[0], "\"}, \"id\" : %d}", rpcId);
	strcat(pDst, tempTxt);
	
	DBG(lgr, "charswwritten=%d, totalsize=%d", charsWritten, strlen(pDst));

	if(blockAutoUpdateInit)
		return;
	
	//support for automatic updates of long arrays > triggered after request when arrIdxBeg == arrIdxEnd	
	if(arrIdxBeg != arrIdxEnd)
		return;
		
	
	//find a first available entry for array update data	
	for(ii=0; ii<=PAI_PVILITE_ARRLIST_INDEX_MAX; ii++){
		if(!gPaiPVILists.arrayListCollection[ii].isUsed){
			found = 1;
			break;
		}
	}

	if(!found)
		return;
		
		
	gPaiPVILists.arrayListCollection[ii].isUsed = 1;
	strcpy(gPaiPVILists.arrayListCollection[ii].iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);	
	gPaiPVILists.arrayListCollection[ii].iRmtUdpPort = 9998;
	bgTimerStart(&gPaiPVILists.arrayListCollection[ii].notificationTimer, PAIPVILITE_ARRAUTOUPDATE_RATE_ms);
	gPaiPVILists.arrayListCollection[ii].arrIdxNext = 0;
	gPaiPVILists.arrayListCollection[ii].arrItemsPerUpdate = PAIPVILITE_ARRAUTOUPDATE_ITEMSPERREPLY;
	gPaiPVILists.arrayListCollection[ii].pvArrDimension = 1;
	strcpy(gPaiPVILists.arrayListCollection[ii].pvArrName, pArrName);
	gPaiPVILists.arrayListCollection[ii].arrItemAproxSizeJSON = charsWritten;
	gPaiPVILists.arrayListCollection[ii].rpcId = rpcId;
	{
	UDINT data_typ;
	UDINT data_len;
	UINT dimension;
	UINT status = PV_ninfo(pArrName, &data_typ, &data_len, &dimension);
	gPaiPVILists.arrayListCollection[ii].pvArrDimension = dimension;
	}
	
	//Recalculate stuff based on the estimatede size of json msg required to transfer the array elements
	{
		int aproxMsgSizeMax =  gPaiPVILists.arrayListCollection[ii].arrItemAproxSizeJSON * gPaiPVILists.arrayListCollection[ii].pvArrDimension;
		
		//TODO: change following and use actual buffer size / max allowed msg size
		if(aproxMsgSizeMax < 4000){
//			gPaiPVILists.arrayListCollection[ii].arrItemsPerUpdate = gPaiPVILists.arrayListCollection[ii].pvArrDimension;	
		}

	}
	
	bgTimerStart(&gPaiPVILists.arrayListDlyTimer, PAIPVILITE_ARRAUTOUPDATE_DLY_ms); 
	DBG(lgr, "getpvarray AUTOUPDATE Start");
}

void paiPviLiteGetPVArray(cJSON* params, int rpcId)
{
	cJSON *item, *pArrName, *pArrBeg, *pArrEnd;
	int itemCount;
	int ii;
	int arrIdxBeg = 0;//init to 0 > if beg idx not defined, start fromn 0
	int arrIdxEnd = 100000;//init to a huge value > if end idx not defined, return entire array	
	UINT status; 
	char* pDst = gJsontextTxRply;
	int resultCode = 0;
	DINT charsWritten;
	char tempTxt[128];
	
	DBG(lgr, "getpvarray BEGIN");
	
	if(!params){
		DBG(lgr, "getpvarray error: params not defined");
		paiPviLiteRpcCmdReplyErrorSend("parametars not defined", -9, rpcId);		
		return;		
	}
	
	pArrName = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVARRNAME);
	pArrBeg = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVARRIDXBEG);
	pArrEnd = cJSON_GetObjectItem(params, PAIPVILITE_KEY_PVARRIDXEND);
	
	if(!pArrName){
		DBG(lgr, "getpvarray error: arr name not defined");
		paiPviLiteRpcCmdReplyErrorSend("array name not defined", -10, rpcId);		
		return;		
	}
	
	if(pArrBeg && (pArrBeg->valueint >= 0)){
		arrIdxBeg = pArrBeg->valueint;
	}	

	if(pArrEnd && (pArrEnd->valueint >= 0)){
		arrIdxEnd = pArrEnd->valueint;
	}	
	DBG(lgr, "getpvarray: arr='%s'[%d..%d]", pArrName->valuestring, arrIdxBeg, arrIdxEnd);
	//============================================================

	paiPviLiteGetPVArrayRply(pDst, pArrName->valuestring, arrIdxBeg, arrIdxEnd, rpcId, 0);
	gTxRply = 1;
	gUdpClient.clientIf.iRmtUdpPort = 9998;
	strcpy(gUdpClient.clientIf.iRmtIPAddr, gUdpServer.serverIf.oRxRmtIP);
	gUdpClient.clientIf.iConnect = 1;
	gUdpClient.clientIf.iSend = 1;		
	DBG(lgr, "getpvarray END");
		
}


void paiPviLiteParseRpc(char *text)
{
	char *out;
	cJSON *json, *item, *rpcVer, *rpcMethod, *rpcParams, *rpcId;
	int itemCount;
	int charsWritten;
	
	json=cJSON_Parse(text);
	
	if (!json) {
		DBG(lgr, "Error before: [%s]\n",cJSON_GetErrorPtr());
		return;
	}

	item = json;
	while( item = cJSON_PrintChildItem(item) );
			
	itemCount = cJSON_GetArraySize(json);
	DBG(lgr, "itemCount: %d",itemCount);

	if(itemCount < 2){
		DBG(lgr, "Incorrect item count");
		goto exit_freemem;	
	}
		
	rpcVer = cJSON_GetObjectItem(json, PAIPVILITE_KEY_VERSION);
	rpcMethod = cJSON_GetObjectItem(json, PAIPVILITE_KEY_METHOD);
	rpcParams = cJSON_GetObjectItem(json, PAIPVILITE_KEY_PARAMS);
	rpcId = cJSON_GetObjectItem(json, PAIPVILITE_KEY_ID);
	
	if(!rpcVer){
		DBG(lgr, "Not a paipvilite call");
		goto exit_freemem;	
	}

	if(!rpcMethod){
		DBG(lgr, "paipvilite method not specified");
		goto exit_freemem;	
	}else{
		if(rpcMethod->type != cJSON_String){
			DBG(lgr, "specified method is not a string");
			goto exit_freemem;	
		}
	}

	//dispatch a method call
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_CREATEPVLIST)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);
		paiPviLiteCreatePVList(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_GETPVLIST)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteGetPVList(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_SETPVLISTPROP)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteSetPVListProps(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_SETPVLIST)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteSetPVList(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_GETSERVERINFO)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteGetServerInfo(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_GETALLPVLISTS)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteGetAllPVLists(rpcParams, rpcId->valueint);	
	}else
	if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_GETPVARRAY)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteGetPVArray(rpcParams, rpcId->valueint);	
	}else
		if(!strcmp(rpcMethod->valuestring, PAIPVILITE_METHOD_DELETEPVLIST)){
		DBG(lgr, "calling '%s' method!", rpcMethod->valuestring);	
		paiPviLiteDeletePVList(rpcParams, rpcId->valueint);	
	}else{
		DBG(lgr, "method '%s' is not supported !", rpcMethod->valuestring);	
	}	
	
exit_freemem:

	cJSON_Delete(json);
		
}

void paiPviLiteRpcPvListEvent(char* pDst, paiPVIList_typ* pPvList)
{
	int ii;
	
	//define start of the message
	strcpy(pDst, "{\"paipvilite\": \"1.0\", \"method\": \"pvlistevent\", \"params\": {\"pvlistname\" : \"");
	strcat(pDst, pPvList->listName);
	strcat(pDst, "\", \"pvlistitems\" : [");
	//add pvnames as list items
	for(ii=0; ii<pPvList->itemCount; ii++){
		strcat(pDst, "\"");
		strcat(pDst, pPvList->itemList[ii].pvName );
		if(ii < (pPvList->itemCount-1)){
			strcat(pDst, "\",");
		}else{
			strcat(pDst, "\"");		
		}
	}
	strcat(pDst, "], \"pvlistitemvalues\" : [");
	//add pv values as list items
	for(ii=0; ii<pPvList->itemCount; ii++){
	
		if(pPvList->itemList[ii].isError || pPvList->itemList[ii].pvType == PB_DT_VIS){
			strcat(pDst, "\"");
			
			if(pPvList->itemList[ii].isError){
				strcat(pDst, pPvList->itemList[ii].pvValueTxt);				
			}else{
				//Copy string PV directly into JSON msg
				//TODO: limit size of the string variable depending of the size of the JSON msg output buffer
				strcat(pDst, pPvList->itemList[ii].pPvValue);								
			}
			
			if(ii < (pPvList->itemCount-1)){
				strcat(pDst, "\",");
			}else{
				strcat(pDst, "\"");		
			}
		}else{
			if(pPvList->itemList[ii].pvType == PB_DT_FLOAT){
				sprintf2(pDst + strlen(pDst), "\"%f\"", pPvList->itemList[ii].pvValueOldFloat);	
			}else{
				sprintf2(pDst + strlen(pDst), "\"%d\"", pPvList->itemList[ii].pvValueOld);	
			}
			if(ii < (pPvList->itemCount-1)){
				strcat(pDst, ",");
			}else{
				strcat(pDst, "");		
			}
		}	
	}
	strcat(pDst, "]}}");

#if 0		
	cJSON *root,*params,*pvlist,*pvlistvalues, *id,*item; char *out;int i;	/* declare a few. */
	int charsWritten = 0;
	
	/* Our "Video" datatype: */
	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "paipvilite", cJSON_CreateString("1.0"));
	cJSON_AddItemToObject(root, "method", cJSON_CreateString("pvlistevent"));
	cJSON_AddItemToObject(root, "params", params=cJSON_CreateObject());
	cJSON_AddStringToObject(params,"pvlistname", "test1");
	cJSON_AddStringToObject(params,"eventtype", "change");
	cJSON_AddItemToObject(params, "pvlist", pvlist=cJSON_CreateArray());
	cJSON_AddItemToObject(params, "pvlistvalues", pvlistvalues=cJSON_CreateArray());
	for(ii=0; ii<pPvList->itemCount; ii++){
		cJSON_AddItemToArray(pvlist, cJSON_CreateString(pPvList->itemList[ii].pvName));
		if(pPvList->itemList[ii].pvType == PB_DT_VIS){
			cJSON_AddItemToArray(pvlist, cJSON_CreateString(pPvList->itemList[ii].pvValueTxt));
		}else{
			if(pPvList->itemList[ii].pvType == PB_DT_FLOAT){
				cJSON_AddItemToArray(pvlist, cJSON_CreateNumber(pPvList->itemList[ii].pvValueOldFloat));
			}else{
				cJSON_AddItemToArray(pvlist, cJSON_CreateNumber(pPvList->itemList[ii].pvValueOld));
			}
		}
	}
	//cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV1num"));
	//cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV2float"));
	//cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV3str"));
	
	cJSON_AddNumberToObject(root,"id",		1);

	item = root;
	while( item = cJSON_PrintChildItem(item) );
	
	charsWritten = 0;
	snprint_value(root, 0, 0, text, 256, &charsWritten);
	DBG(lgr, "chars written= %d", charsWritten);	
#endif	
}


void paiPviLiteCreateRpc(char *text)
{
	cJSON *root,*params,*pvlist,*id,*item;char *out;int i;	/* declare a few. */
	int charsWritten = 0;
	/* Here we construct some JSON standards, from the JSON site. */
	
	/* Our "Video" datatype: */
	root=cJSON_CreateObject();	
	cJSON_AddItemToObject(root, "paipvilite", cJSON_CreateString("1.0"));
	cJSON_AddItemToObject(root, "method", cJSON_CreateString("setpvlist"));
	cJSON_AddItemToObject(root, "params", params=cJSON_CreateObject());
	cJSON_AddStringToObject(params,"pvlistname", "test1");
	cJSON_AddItemToObject(params, "pvlist", pvlist=cJSON_CreateArray());
	cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV1num"));
	cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV2float"));
	cJSON_AddItemToArray(pvlist, cJSON_CreateString("gPV3str"));
	cJSON_AddItemToArray(pvlist, cJSON_CreateString("gDummyNotExist"));
	cJSON_AddNumberToObject(root,"id",		1);

	item = root;
	while( item = cJSON_PrintChildItem(item) );
	
	charsWritten = 0;
	snprint_value(root, 0, 0, text, 256, &charsWritten);
	DBG(lgr, "chars written= %d", charsWritten);	
}

static hashCalcInit()
{
	//Hash calculation stuff
	mHashCalcDone = 0;
	bgTimerStart(&mHashCalcDlyTmr, 4000);
}

static void hashCalcCyclic()
{
	
	if(mHashCalcDone)
		return;
	
	bgTimerCyclic(&mHashCalcDlyTmr);

	if( !bgTimerIsTimeOut(&mHashCalcDlyTmr) )
		return;

	

}


#define OUT_BUF_LEN 400000
static char locOutBuffer[OUT_BUF_LEN];

void _INIT cJSONTestINIT(void)
{
	/* TODO: Add code here */
	//init logger object
	DBGINIT(lgr, tasklog, LOG_ENTRY_COUNT, LOG_ENTRY_LENGTH);
	
	strcpy(gJsontext, "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}");
	//strcpy(gJsontext, "{\n\"name\": \"Test2\", \n\"format\": {\"type\": \"rect\", \n\"width\":  [1920, 100, 200], \n\"height\":     [1080, 40, 40], \n\"interlace\":  false,\"frame rate\": 24\n}\n}");

	//strcpy(gJsontext, "[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n");
	
	strcpy(gJsontext, "{\"paipvilite\": \"1.0\", \"method\": \"setpvlist\", \"params\": {\"pvlistname\" : \"test1\", \"pvlistitems\" : [\"var1\", \"var2\"]}, \"id\": 0}");

/*
{"jsonrpc": "2.0", "result": 19, "id": 4}
{"jsonrpc": "2.0", "error": {"code": -32700, "message": "Parse error"}, "id": null}
*/	
	
	gPV1num = 345;
	gPV2float = 3.14;
	strcpy(gPV3str, "Hello paiPVILite");
	gMyCounter = 0;
	
	
	hashCalcInit();
}


void _CYCLIC cJSONTestCYCLIC(void)
{
	/* TODO: Add code here */
	if(gDoit){
		gDoit = 0;
		DBG(lgr, "==From JSON==");
		paiPviLiteParseRpc(gJsontext);
		DBG(lgr, "==Done.==");
	}

	if(gDoit1){
		gDoit1 = 0;
		DBG(lgr, "==To JSON==");
		paiPviLiteCreateRpc(gJsontext);
		DBG(lgr, "==From JSON==");
		paiPviLiteParseRpc(gJsontext);
		DBG(lgr, "==Done.==");
		
	}
	
	if(gDoit2){
		gDoit2 = 0;
		DBG(lgr, "==Update PVs BEG==");
		paiPviLiteUpdatePVListValues();
		paiPviLiteSendEvents();
		DBG(lgr, "==Update PVs END==");
	}

	if(gDoit3){	
		gDoit3 = 0;
		//DINT charsWritten = bgPVStructSerialize(gPVName, (UDINT)&locOutBuffer[0], OUT_BUF_LEN);
		DINT charsWritten = bgPVRootArraySerializeJSON(gPVName, (UDINT)&locOutBuffer[0], OUT_BUF_LEN, gArrBeg, gArrEnd);
		
		if(  charsWritten > 0){
			DBG(lgr,"SerOK! PV=%s size=%d", gPVName, charsWritten);
			memcpy(gOutputDbg, locOutBuffer, 253);
			gOutputDbg[254]=0;
		}else{
			DBG(lgr,"Can't serialize pv=%s, err=%d", gPVName, charsWritten);
		}
	}
	
	
	//application logic - en example of handling a UDP rx event	
	if(gUdpServer.serverIf.oRxReady){
		gUdpServer.serverIf.oRxReady = 0;
		
		DBG(lgr, "==Msg Rx==");
		paiPviLiteParseRpc(gJsontextRx);
		DBG(lgr, "==Msg Rx Done.==");
		
		DBG(lgr, "==Update PVs BEG==");
		paiPviLiteUpdatePVListValues();
		paiPviLiteSendEvents();
		DBG(lgr, "==Update PVs END==");
	}
	
	//application logic - on send complete, copy status into last statusSent
	if(gUdpClient.clientIf.oSendDone){
		gUdpClient.clientIf.oSendDone = 0;		
		//save last transmitted data
		//DBG(lgr, "==Update PVs BEG==");
		//paiPviLiteUpdatePVListValues();
		//paiPviLiteSendEvents();
		//DBG(lgr, "==Update PVs END==");
	}

	
	paiPviLiteUpdatePVListValues();
	paiPviLiteSendEvents();
	paiPviLiteSendArrayAutoUpdates();	
	
	gMyCounter++;
	
	hashCalcCyclic();
}
