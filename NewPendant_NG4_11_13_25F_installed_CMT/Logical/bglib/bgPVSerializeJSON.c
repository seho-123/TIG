#ifndef _BG_SERIALIZEJSON_C
#define _BG_SERIALIZEJSON_C
/**
3/17/2015
-fixed serious bug that was causing incorrect setting of the integer PV types shorter than DINT/UDINT (i.e. SINT, USINT, INT, UINT)
*/
#ifdef WIN32
#include "string.h"
#else
#include <bur/plctypes.h>
#endif

#include "bgstring.h"
#include "bgsprintf.h"

#define PV_SERIALIZEJSON_ITEM_DELIM_CRLF "\r\n"
#define PV_SERIALIZEJSON_ITEM_DELIM_COMMA ","
//#define PV_SERIALIZE_STR_MARKUP "'"
#define PV_SERIALIZEJSON_STR_MARKUP "'"

#define PV_SERIALIZEJSON_ERR_ZEROPTR -1
#define PV_SERIALIZEJSON_ERR_PVNOTFOUND -2
#define PV_SERIALIZEJSON_ERR_PVADRNOTFOUND -3
#define PV_SERIALIZEJSON_ERR_OBUFOVERFLOW -4

#define PV_SERIALIZEJSON_ERR_LAST -10

typedef struct bgPVSerializeJSON_typ
{
	char* toOutStream;
	UDINT lengthOutStream;
	DINT outputCharCount;
	DINT errorCode;
	DINT arrayIndexFrom;
	DINT arrayIndexTo;
	char* pItemDelim;
	char* pStrMarkup;
	DINT lastElement;
	
}bgPVSerializeJSON_typ;



static void bgPVSubStructSerializeJSON(bgPVSerializeJSON_typ* p, char* pvNameRoot, char* pvName, UINT dimension);
static void bgPVSerializeJSON(bgPVSerializeJSON_typ* p, char* pvNameRoot, char* pvName);
static void bgPVArraySerializeJSONRootToFrom(bgPVSerializeJSON_typ* p, char* pvNameRoot, int dimension, int isArrayOfStruct, int isArrayOfStrings);


//#ifndef DBG
//#define DBG() ;
//#endif

#define PV_TYPE_MAX_INDEX	24
static const char *bgPVTypeStrings[PV_TYPE_MAX_INDEX + 1]={
	"STRUCT",	/*0*/
	"BOOL",		/*1*/
	"SINT",		/*2*/
	"INT",		/*3*/
	"DINT",		/*4*/
	"USINT",		/*5*/
	"UINT",		/*6*/
	"UDINT",		/*7*/
	"REAL",		/*8*/
	"STRING ",	/*9*/
	"ULINT",		/*10*/
	"DATETIME",	/*11*/
	"TIME",		/*12*/
	"DATE",		/*13*/
	"LREAL",		/*14*/
	"ARRAY",		/*15*/
	"TIME_OF_DAY",	/*16*/
	"BYTE",		/*17*/
	"WORD",		/*18*/
	"DWORD",	/*19*/
	"LWORD",	/*20*/
	"WSTRING",	/*21*/
	"?22",		/*22*/
	"LINT",		/*23*/
	};

static const char *bgGetPVTypeString(int pvTypeIndex){
	if(pvTypeIndex >= PV_TYPE_MAX_INDEX){
		return "TYPE=?";
	}

	return bgPVTypeStrings[pvTypeIndex];
}


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
			bgsprintf(pvValue, "%d", *( (char*)adr ));
			break;	
		case PB_DT_INT8:
			bgsprintf(pvValue, "%d", *( (char*)adr) );
			break;	
		case PB_DT_INT16:
			bgsprintf(pvValue, "%d", *( (short*)adr ) );
			break;	
		case PB_DT_INT32:
			bgsprintf(pvValue, "%d", *( (int*)adr ));
			break;	
		case PB_DT_BYTE:
			bgsprintf(pvValue, "%u", *( (unsigned char*)adr));
			break;	
		case PB_DT_WORD:
			bgsprintf(pvValue, "%u", *( (unsigned short*)adr));
			break;	
		case PB_DT_LONG:
			bgsprintf(pvValue, "%u", *( (unsigned int*)adr));
			break;	
		case PB_DT_FLOAT:
			{
				char temp[32];
				ftoa( *((float*)adr), (UDINT)temp);
				bgsprintf(pvValue, "%s", temp);
			}
			break;	
		case PB_DT_VIS:
			bgsprintf(pvValue, "%s", (char*)adr );
			if(!strcmp(pvValue, "GB123567")){
				int i;
				i = 100;
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
		case 14://double
			{
				char temp[32];
				double value_double = *((double*)(adr));
				float value_float = (float)value_double;
				ftoa( value_float, (UDINT)temp);
				bgsprintf(pvValue, "%s", temp);
			}		
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
			*((char*)adr) = (char)atoi((UDINT)pvValue);
			break;	
		case PB_DT_INT8:
			*((SINT*)adr) = (SINT)atoi((UDINT)pvValue);
			break;	
		case PB_DT_INT16:
			*((INT*)adr) = (INT)atoi((UDINT)pvValue);
			break;	
		case PB_DT_INT32:
			*((DINT*)adr) = (DINT)atoi((UDINT)pvValue);
			break;	
		case PB_DT_BYTE:
			*((USINT*)adr) = (USINT)atoi((UDINT)pvValue);
			break;	
		case PB_DT_WORD:
			*((UINT*)adr) = (UINT)atoi((UDINT)pvValue);
			break;	
		case PB_DT_LONG:
			*((UDINT*)adr) = (UDINT)atoi((UDINT)pvValue);
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
		case 14://double
			{
				float value_float = atof((UDINT)pvValue); 
				double value_double = (double)value_float;
				*((double*)adr) = value_double;
			}
			break;	
		case PB_DT_ARRAY:
			//DBG("TODO: display array values...");
			break;	
	}
}

int bgFindCharInStrRev(char *s, char chr)
{
	int ii;
	int lenStr;
	char *ptrStr;
	
	/*exit if null ptrs*/
	if(!s)
		return -1;
	
	lenStr = strlen(s);

	/*exit if zero strings*/
	if( !lenStr || !chr )
		return -1;

	/*search for char <chr> within the string <s> starting from the end*/
	for(ii=lenStr-1; ii>=0; ii--)
	{
		ptrStr = s + ii;
		if(*ptrStr == chr){
			return (ii);
		}
	}
	return -1;
}

static void bgPVWriteOutStream(bgPVSerializeJSON_typ* p, char* pvName, char* pvTextValue)
{
	int dotPosition;
	
	if(!p || !p->toOutStream || !pvTextValue || !p->pItemDelim)
		return;
	
	
	if(pvName){
		dotPosition = bgFindCharInStrRev(pvName, '.');
		if(dotPosition < 0){
			dotPosition=0; //dot not found > take the entire pvname
		}else{
			dotPosition += 1;//dot found > take only the pvname after the dot
		}
	
		strcat(p->toOutStream, PV_SERIALIZEJSON_STR_MARKUP);
		strcat(p->toOutStream, &pvName[dotPosition]);
		strcat(p->toOutStream, PV_SERIALIZEJSON_STR_MARKUP);
		strcat(p->toOutStream, "=");
		strcat(p->toOutStream, pvTextValue);
		
		p->outputCharCount += strlen(pvName);
		p->outputCharCount += 1; //  + '=' 
	}else{
		strcat(p->toOutStream, pvTextValue);	
	}
	
	if(!p->lastElement){
		strcat(p->toOutStream, p->pItemDelim);	// delimiter "\r\n" or ","	
	}
	p->outputCharCount += strlen(pvTextValue);
	if(!p->lastElement){
		p->outputCharCount += strlen(p->pItemDelim); //  + sizeof the delimiter string 
	}
		
	
	if(p->outputCharCount >= (p->lengthOutStream - 100) ){//TODO: needs to be a better way to handle output buffer overflow
		p->errorCode = PV_SERIALIZEJSON_ERR_OBUFOVERFLOW;
		return;
	}
}

static bgTxtWriteOutStream(bgPVSerializeJSON_typ* p, char* pTxt)
{
	if(!p || !p->toOutStream || !pTxt)
		return;
		
	strcat(p->toOutStream, pTxt);
		
	p->outputCharCount += strlen(pTxt);
		
	if(p->outputCharCount >= (p->lengthOutStream - 100) ){//TODO: needs to be a better way to handle output buffer overflow
		p->errorCode = PV_SERIALIZEJSON_ERR_OBUFOVERFLOW;
		return;
	}
}

static int bgPVSerializeJSONIsArray(int data_typ, int data_len, int dimension)
{
	if(data_typ && (dimension > 1)){
		return 1;
	}
	return 0;
}

static void bgPVArraySerializeJSON(bgPVSerializeJSON_typ* p, char* pvNameRoot, char* pvName, int dimension, int isArrOfStruct, int isArrOfStrings)
{
	char pvArrNameRoot[128];
	bgPVSerializeJSON_typ pvSerObj;
	
	if(!pvName)
		return;
	
	bgTxtWriteOutStream(p, PV_SERIALIZEJSON_STR_MARKUP);	
	bgTxtWriteOutStream(p, pvName);	
	bgTxtWriteOutStream(p, PV_SERIALIZEJSON_STR_MARKUP);	
	bgTxtWriteOutStream(p, "=");	
	
	pvSerObj.toOutStream = p->toOutStream;
	pvSerObj.lengthOutStream = p->lengthOutStream;
	pvSerObj.outputCharCount = p->outputCharCount;
	pvSerObj.errorCode = 0;
	pvSerObj.arrayIndexFrom = 0;
	pvSerObj.arrayIndexTo = dimension - 1;
	pvSerObj.pItemDelim =  PV_SERIALIZEJSON_ITEM_DELIM_COMMA;
	pvSerObj.pStrMarkup =  PV_SERIALIZEJSON_STR_MARKUP;
	pvSerObj.lastElement = 0;
	
	
	strcpy(pvArrNameRoot, pvNameRoot);
	strcat(pvArrNameRoot, ".");
	strcat(pvArrNameRoot, pvName);
	
	bgPVArraySerializeJSONRootToFrom(&pvSerObj, pvArrNameRoot, dimension, isArrOfStruct, isArrOfStrings);
	
	p->outputCharCount = pvSerObj.outputCharCount;
	p->errorCode = pvSerObj.errorCode;
	

}


static void bgPVArraySerializeJSONRootToFrom(bgPVSerializeJSON_typ* p, char* pvNameRoot, int dimension, int isArrayOfStruct, int isArrayOfStrings)
{
	int ii = 0;
	// TODO: checking for max size of the strings that are copied to these arrays
	char structName[256];
	char structItemName[32];
	DINT arrLastElement;
/*
Array of objects:
 "myarray" = "[
 {ID=0, HeadID=0, var={Type=0, Format=0}}, 
 {}, 
 {}]"
		
*/
	
	
	arrLastElement = p->lastElement;
	p->lastElement = 0;
	bgTxtWriteOutStream(p, "[");	
	for(ii = p->arrayIndexFrom; (ii < dimension) && (ii <= p->arrayIndexTo); ii++){
		// NOTE: verify why PV_item destroys content of the buffer supplied as 1st param
		{
			char arrayIndexTxt[10];
			strcpy(structItemName, pvNameRoot);
			strcat(structItemName, "[");
			itoa(ii, (UDINT)arrayIndexTxt);
			strcat(structItemName, arrayIndexTxt);
			strcat(structItemName, "]");
		}
		
		if(isArrayOfStruct)
			bgTxtWriteOutStream(p, "{");	
		
		if((ii == (dimension-1)) || (ii == p->arrayIndexTo) ){
			p->lastElement = 1;
		}	
		
		bgPVSerializeJSON(p, structItemName, 0);
		
		if((ii == (dimension-1)) || (ii == p->arrayIndexTo) ){
			if(isArrayOfStruct)
				bgTxtWriteOutStream(p, "}");
		}else{
			if(isArrayOfStruct)
				bgTxtWriteOutStream(p, "},");
		}
		
		if(p->errorCode)
			return;	
	}	
	
	if(!isArrayOfStruct){
		//	p->outputCharCount -= strlen(p->pItemDelim);//workaroud: remove delim for the last array element
		//	p->toOutStream[p->outputCharCount] = 0; 					
	}
	
	bgTxtWriteOutStream(p, "]");	
	p->lastElement = arrLastElement;

	if(!isArrayOfStruct && !p->lastElement && !isArrayOfStrings){
		bgTxtWriteOutStream(p, p->pItemDelim);		
	}	
	
}

static void bgPVSerializeJSON(bgPVSerializeJSON_typ* p, char* pvNameRoot, char* pvName)
{
	UINT status;
	UDINT data_typ;
	UDINT data_len;
	UINT dimension;
	UDINT adr;
	char structItemPVName[256];

	if( !pvName){
		strcpy(structItemPVName, pvNameRoot);
		
	}else{
		strcpy(structItemPVName, pvNameRoot);
		strcat(structItemPVName, ".");		
		strcat(structItemPVName, pvName);
	}


	status = PV_ninfo(structItemPVName, &data_typ, &data_len, &dimension);

	if( status ){
		p->errorCode = status;
		return;
	}
	
	if( bgPVSerializeJSONIsArray(data_typ, data_len, dimension) ){
		bgPVArraySerializeJSON(p, pvNameRoot, pvName, dimension, (data_typ==PB_DT_ARRAY)?1:0, 0);
		return;
	}
	
	status = PV_xgetadr(structItemPVName, &adr, &data_len);
	
	if( status ){
		p->errorCode = status;
		return;
	}

	if( (data_typ == PB_DT_STRUCT) ){
		//DBG("PV='%s' Typ=%s Len=%d Dim=%d" , pvnamecomplete, bgGetPVTypeString(data_typ), data_len, dimension);	
		
		if( (dimension == 1) && (data_len==4)){
			//enum data type!!! > will be handled below as a UDINT
			data_typ = PB_DT_INT32;
		}else{
			bgPVSubStructSerializeJSON(p, pvNameRoot, pvName, dimension);
			return;
		}
	}
	
	{
		char pvTextValue[256];
		int len = 0;
		
		if(data_typ == PB_DT_VIS){//STRING VALUES ONLY > marked with a special markup at the begining and end
			strcpy(pvTextValue, p->pStrMarkup);//set string begining markup (i.e. "\"")
			len = strlen(pvTextValue);
		}
		
		bgGetPVValueText(&pvTextValue[len], data_typ, data_len, dimension, adr);	
		
		if(data_typ == PB_DT_VIS){//STRING VALUES ONLY > marked with a special markup at the begining and end
			strcat(pvTextValue, p->pStrMarkup);//set string ending markup (i.e. "\"")
		}
		
		//DBG("stritemPV='%s' Typ=%s Len=%d Dim=%d Val=%s" , pvName, bgGetPVTypeString(data_typ), data_len, dimension, pvTextValue);
		bgPVWriteOutStream(p, pvName, pvTextValue);
	}

}


static void bgPVSubStructSerializeJSON(bgPVSerializeJSON_typ* p, char* pvNameRoot, char* pvName, UINT dimension)
{
/*
Example:
typedef struct str1_typ{
UINT val1;
UINT val2;
}str1_typ;

typedef struct str2_typ{
UINT val3;
}str2_typ;

typedef struct str3_typ{
UINT val4;
struct str1_typ s1;
struct str2_typ s2;
}str3_typ;

_GLOBAL str3_typ test;

All available PV Names are:
test.val4
test.s1.val1
test.s1.val2
test.s2.val3;

Top PV Name (pvNameRoot):
test

During iteration, followin values will appear for 'structItemName':
s1
s2
val4

During iteration, following values will appear for 'structItemPVNameNoRoot':
val4
s1.val1
s1.val2
s2.val3

*/
	int ii = 0;
	// TODO: checking for max size of the strings that are copied to these arrays
	char structName[256];
	char structItemName[32];
	char structItemPVNameNoRoot[256];
	UINT status;
	UDINT data_typ;
	UDINT data_len;
	UDINT adr;

	if(pvName){
		bgTxtWriteOutStream(p, PV_SERIALIZEJSON_STR_MARKUP);	
		bgTxtWriteOutStream(p, pvName);	
		bgTxtWriteOutStream(p, PV_SERIALIZEJSON_STR_MARKUP);	
		bgTxtWriteOutStream(p, "={");	
	}
	
	for(ii=0; ii < dimension; ii++){
		// NOTE: verify why PV_item destroys concent of the buffer supplied as 1st param
		if( !pvName){
			strcpy(structName, pvNameRoot);
		}else{
			strcpy(structName, pvNameRoot);
			strcat(structName, ".");
			strcat(structName, pvName);
		}
		
		//DBG("1)bgPVSubStr.. ii=%d str=%s item=%s", ii, structName, structItemName);
		status = PV_item(structName, ii, structItemName);
		//DBG("2)bgPVSubStr.. ii=%d str=%s item=%s", ii, structName, structItemName);
		
		if(status && (status != 14713)){
			// TODO: error handling
			//DBG("bgPVSubStr.. ii=%d str=%s item=%s err=%d", ii, structName, structItemName, status);
			p->errorCode = status;
			return;
		}
		
		//DBG("item=%d, name=%s", ii, structItemName);
		// TODO: check for max length of the PV name
		if(!pvName){
			strcpy(structItemPVNameNoRoot, structItemName);
		}else{
			if(status == 14713){//14713 is to handle enum type??? find out if there is a better solutiuon
				//following code is to handle an enum type that is reported as a structure (type 0). However, there are no structure elements and is not clear how to access values???
				char pvTextValue[64];
		
				status = PV_ninfo(structName, &data_typ, &data_len, &dimension);
				status = PV_xgetadr(structName, &adr, &data_len);
				//based on the reported lenght, try to guess the type of the enum
				if(data_len == 1){
					data_typ = PB_DT_BYTE;	
				}else
					if(data_len == 2){
					data_typ = PB_DT_WORD;					
				}else{
					data_typ = PB_DT_LONG;					
				}
				bgGetPVValueText(&pvTextValue[0], data_typ, data_len, dimension, adr);
				if(ii == (dimension - 1)){
					p->lastElement = 1;
				}
				else{
					p->lastElement = 0;
				}	
				bgPVWriteOutStream(p, pvName, pvTextValue);
				return;				
			}	
			
			strcpy(structItemPVNameNoRoot, pvName);
			strcat(structItemPVNameNoRoot, ".");
			strcat(structItemPVNameNoRoot, structItemName);			
		}

		if(ii == (dimension - 1)){
			p->lastElement = 1;
		}
		else{
			p->lastElement = 0;
		}	
		bgPVSerializeJSON(p, pvNameRoot, structItemPVNameNoRoot);
		
		if(p->errorCode)
			return;
	}
	
	if(pvName){
		bgTxtWriteOutStream(p, "}");	
	}
}


DINT bgPVRootArraySerializeJSON(char* pvNameRoot, char* toOutStream, UDINT lengthOutStream, UDINT begIndex, UDINT endIndex)
{
	UINT status;
	UDINT data_typ;
	UDINT data_len;
	UINT dimension;
	UDINT adr;
	DINT outputCharCount = 0;

	
	bgPVSerializeJSON_typ pvSerObj;
	
	if(!pvNameRoot || !toOutStream){
		return PV_SERIALIZEJSON_ERR_ZEROPTR;
	}
	
	if(toOutStream){
		strcpy(toOutStream, "");
	}
	pvSerObj.toOutStream = toOutStream;
	pvSerObj.lengthOutStream = lengthOutStream;
	pvSerObj.outputCharCount = 0;
	pvSerObj.errorCode = 0;
	pvSerObj.arrayIndexFrom = begIndex;
	pvSerObj.arrayIndexTo = endIndex;
	pvSerObj.pItemDelim =  PV_SERIALIZEJSON_ITEM_DELIM_COMMA;
	pvSerObj.pStrMarkup =  PV_SERIALIZEJSON_STR_MARKUP;
	pvSerObj.lastElement = 0;
	
	status = PV_ninfo(pvNameRoot, &data_typ, &data_len, &dimension);
	if( status ){
		return PV_SERIALIZEJSON_ERR_PVNOTFOUND;
	}

	
	status = PV_xgetadr(pvNameRoot, &adr, &data_len);
	if( status ){
		return PV_SERIALIZEJSON_ERR_PVADRNOTFOUND;
	}

	if(data_typ == PB_DT_STRUCT){
		//DBG("TOPPV='%s' Typ=%s Len=%d Dim=%d" , pvNameRoot, bgGetPVTypeString(data_typ), data_len, dimension);
		bgPVSubStructSerializeJSON(&pvSerObj, pvNameRoot, 0, dimension);
	}else
	if(data_typ == PB_DT_ARRAY){
		bgPVArraySerializeJSONRootToFrom(&pvSerObj, pvNameRoot, dimension, 1, 0);
	}else
	if(data_typ == PB_DT_VIS){//visible string
		bgPVArraySerializeJSONRootToFrom(&pvSerObj, pvNameRoot, dimension, 0, 1);
	}else
	if( ((data_typ == PB_DT_INT16) || (data_typ == PB_DT_INT32) || (data_typ == PB_DT_WORD) || (data_typ == PB_DT_LONG))&& (dimension > 1)){//aray of INT16
		bgPVArraySerializeJSONRootToFrom(&pvSerObj, pvNameRoot, dimension, 0, 1);
	}else{
		char pvTextValue[64];		
		bgGetPVValueText(&pvTextValue[0], data_typ, data_len, dimension, adr);		
		//DBG("TOPPV NOT A STRUCT! PV='%s' Typ=%s Len=%d Dim=%d Val=%s" , pvNameRoot, bgGetPVTypeString(data_typ), data_len, dimension, pvTextValue);	
	}

	if(pvSerObj.errorCode){
		if(pvSerObj.errorCode < 0){
			return pvSerObj.errorCode;
		}
		return (pvSerObj.errorCode * (-1));
	}
	
	return pvSerObj.outputCharCount;
}


DINT bgPVStructSerializeJSON(char* pvNameRoot, char* toOutStream, UDINT lengthOutStream)
{
	UINT status;
	UDINT data_typ;
	UDINT data_len;
	UINT dimension;
	UDINT adr;
	DINT outputCharCount = 0;
	bgPVSerializeJSON_typ pvSerObj;
	
	if(!pvNameRoot || !toOutStream){
		return PV_SERIALIZEJSON_ERR_ZEROPTR;
	}
	
	if(toOutStream){
		strcpy(toOutStream, "");
	}
	pvSerObj.toOutStream = toOutStream;
	pvSerObj.lengthOutStream = lengthOutStream;
	pvSerObj.outputCharCount = 0;
	pvSerObj.errorCode = 0;
	pvSerObj.pItemDelim =  PV_SERIALIZEJSON_ITEM_DELIM_CRLF;
	pvSerObj.pStrMarkup =  "";
	
	status = PV_ninfo(pvNameRoot, &data_typ, &data_len, &dimension);
	if( status ){
		return PV_SERIALIZEJSON_ERR_PVNOTFOUND;
	}
	
	status = PV_xgetadr(pvNameRoot, &adr, &data_len);
	if( status ){
		return PV_SERIALIZEJSON_ERR_PVADRNOTFOUND;
	}

		
	if(data_typ == PB_DT_STRUCT){
		//DBG("TOPPV='%s' Typ=%s Len=%d Dim=%d" , pvNameRoot, bgGetPVTypeString(data_typ), data_len, dimension);
		bgPVSubStructSerializeJSON(&pvSerObj, pvNameRoot, 0, dimension);
	}else
	if(data_typ == PB_DT_ARRAY){
		pvSerObj.arrayIndexFrom = 0;
		pvSerObj.arrayIndexTo = dimension - 1;		
		bgPVArraySerializeJSONRootToFrom(&pvSerObj, pvNameRoot, dimension, 1, 0);
	}
	else{
		char pvTextValue[64];		
		bgGetPVValueText(&pvTextValue[0], data_typ, data_len, dimension, adr);		
		//DBG("TOPPV NOT A STRUCT! PV='%s' Typ=%s Len=%d Dim=%d Val=%s" , pvNameRoot, bgGetPVTypeString(data_typ), data_len, dimension, pvTextValue);	
	}

	if(pvSerObj.errorCode){
		if(pvSerObj.errorCode < 0){
			return pvSerObj.errorCode;
		}
		return (pvSerObj.errorCode * (-1));
	}
	
	return pvSerObj.outputCharCount;
}

//Maximum number of PV-s per file that can be deserialized!!!
#define BGPV_DESER_MAXPVCOUNT 1000

DINT bgPVStructDeserializeJSON(char* pvNameRoot, char* fromStream, UDINT lengthStream)
{
	UDINT ii;
	bgStringListEx_typ lineList1;
	bgStringListEx_typ lineSplit1;
	char* memLineList[BGPV_DESER_MAXPVCOUNT];
	char* memLineSplit[10];
	char structItemPVName[256];

	UINT status;
	UDINT data_typ;
	UDINT data_len;
	UINT dimension;
	UDINT adr;
	UDINT errorCounter = 0;

	bgStringListLinesEx(&lineList1, fromStream, (char*)memLineList, BGPV_DESER_MAXPVCOUNT*sizeof(char*));
	for(ii=0; ii<lineList1.itemCount; ii++){
		//DBG("ln%d: %s", ii, lineList1.items[ii]);		
		bgStringSplitLineEx(&lineSplit1, '=',  (char*)lineList1.items[ii], (char*)memLineSplit, (int)10*sizeof(char*));
		//DBG("ln%d: key=%s val=%s", ii, lineSplit1.items[0], lineSplit1.items[1]);
		
		strcpy(structItemPVName, pvNameRoot);
		strcat(structItemPVName, ".");
		strcat(structItemPVName, (char*)lineSplit1.items[0]);
		
		//DBG("PV=%s Val=%s", structItemPVName, lineSplit1.items[1]);
		status = PV_ninfo(structItemPVName, &data_typ, &data_len, &dimension);


		if( status ){
			// TODO: error handling
			//DBG("bgPVStructDes. PV='%s'; Error#=%d", structItemPVName, status);
			errorCounter++;
			continue;
		}

	
		status = PV_xgetadr(structItemPVName, &adr, &data_len);
	
		if( status ){
			// TODO: error handling
			//DBG("bgPVStructDes. Address for PV='%s'; Error=%d", structItemPVName, status);
			errorCounter++;
			continue;
		}

		bgSetPVValueText((char*)lineSplit1.items[1], data_typ, data_len, dimension, adr);		
	}


	// TODO: return value in case of errors; detected version changes, etc.

	return errorCounter;
	
}

#endif //_BG_SERIALIZE_C
/*EOF*/
