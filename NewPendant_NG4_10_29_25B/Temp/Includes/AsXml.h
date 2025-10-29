/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _ASXML_
#define _ASXML_
#ifdef __cplusplus
extern "C" 
{
#endif

#include <bur/plctypes.h>

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif

/* Constants */
#ifdef _REPLACE_CONST
 #define xmlNODE_TYPE_ENDELEMENT 15U
 #define xmlNODE_TYPE_SIGN_WHITESPACE 14U
 #define xmlNODE_TYPE_WHITESPACE 13U
 #define xmlNODE_TYPE_COMMENT 8U
 #define xmlNODE_TYPE_PI 7U
 #define xmlNODE_TYPE_CDATA 4U
 #define xmlNODE_TYPE_TEXT 3U
 #define xmlNODE_TYPE_ELEMENT 1U
 #define xmlERR_UNSUPPORTED_ENCODING 33824U
 #define xmlERR_CONVERT_TEXT 33823U
 #define xmlERR_CONVERT 33822U
 #define xmlERR_INVALID_POSITION 33821U
 #define xmlERR_NO_ELEMENT_OPEN 33820U
 #define xmlERR_INVALID_IDENT 33819U
 #define xmlERR_XML_DATA_INVALID 33818U
 #define xmlERR_NO_ATTRIBUTES 33817U
 #define xmlERR_INVALID_ATTRIBUTE_INDEX 33816U
 #define xmlERR_VALUE_BUFFER_TOO_SMALL 33815U
 #define xmlERR_NAME_BUFFER_TOO_SMALL 33814U
 #define xmlERR_NO_FURTHER_XML_NODE 33813U
 #define xmlERR_GENERAL 33812U
 #define xmlERR_INVALID_PARAMETER 33811U
 #define xmlERR_MEMORY 33810U
#else
 _IEC_CONST unsigned short xmlNODE_TYPE_ENDELEMENT = 15U;
 _IEC_CONST unsigned short xmlNODE_TYPE_SIGN_WHITESPACE = 14U;
 _IEC_CONST unsigned short xmlNODE_TYPE_WHITESPACE = 13U;
 _IEC_CONST unsigned short xmlNODE_TYPE_COMMENT = 8U;
 _IEC_CONST unsigned short xmlNODE_TYPE_PI = 7U;
 _IEC_CONST unsigned short xmlNODE_TYPE_CDATA = 4U;
 _IEC_CONST unsigned short xmlNODE_TYPE_TEXT = 3U;
 _IEC_CONST unsigned short xmlNODE_TYPE_ELEMENT = 1U;
 _IEC_CONST unsigned short xmlERR_UNSUPPORTED_ENCODING = 33824U;
 _IEC_CONST unsigned short xmlERR_CONVERT_TEXT = 33823U;
 _IEC_CONST unsigned short xmlERR_CONVERT = 33822U;
 _IEC_CONST unsigned short xmlERR_INVALID_POSITION = 33821U;
 _IEC_CONST unsigned short xmlERR_NO_ELEMENT_OPEN = 33820U;
 _IEC_CONST unsigned short xmlERR_INVALID_IDENT = 33819U;
 _IEC_CONST unsigned short xmlERR_XML_DATA_INVALID = 33818U;
 _IEC_CONST unsigned short xmlERR_NO_ATTRIBUTES = 33817U;
 _IEC_CONST unsigned short xmlERR_INVALID_ATTRIBUTE_INDEX = 33816U;
 _IEC_CONST unsigned short xmlERR_VALUE_BUFFER_TOO_SMALL = 33815U;
 _IEC_CONST unsigned short xmlERR_NAME_BUFFER_TOO_SMALL = 33814U;
 _IEC_CONST unsigned short xmlERR_NO_FURTHER_XML_NODE = 33813U;
 _IEC_CONST unsigned short xmlERR_GENERAL = 33812U;
 _IEC_CONST unsigned short xmlERR_INVALID_PARAMETER = 33811U;
 _IEC_CONST unsigned short xmlERR_MEMORY = 33810U;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct xmlCreateMemoryReader
{
	/* VAR_INPUT (analog) */
	unsigned long pXmlMemory;
	unsigned long memorySize;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long ident;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlCreateMemoryReader_typ;

typedef struct xmlCloseMemoryReader
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlCloseMemoryReader_typ;

typedef struct xmlReadNextNode
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pName;
	unsigned long nameSize;
	unsigned long pValue;
	unsigned long valueSize;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long nodeType;
	unsigned long depth;
	unsigned long attributeCount;
	unsigned long neededNameSize;
	unsigned long neededValueSize;
	/* VAR_INPUT (digital) */
	plcbit enable;
	plcbit forceRead;
	plcbit skipSubtree;
	/* VAR_OUTPUT (digital) */
	plcbit isEmpty;
} xmlReadNextNode_typ;

typedef struct xmlReadAttributeNr
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long index;
	unsigned long pName;
	unsigned long nameSize;
	unsigned long pValue;
	unsigned long valueSize;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long neededNameSize;
	unsigned long neededValueSize;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlReadAttributeNr_typ;

typedef struct xmlCreateMemoryWriter
{
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long ident;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlCreateMemoryWriter_typ;

typedef struct xmlGetMemoryInfo
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	unsigned long pXmlData;
	unsigned long xmlDataSize;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlGetMemoryInfo_typ;

typedef struct xmlCloseMemoryWriter
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlCloseMemoryWriter_typ;

typedef struct xmlWriteStartDocument
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pVersion;
	unsigned long pEncoding;
	unsigned long pStandalone;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteStartDocument_typ;

typedef struct xmlWriteStartDocEnc
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pVersion;
	unsigned long pInputEncoding;
	unsigned long pOutputEncoding;
	unsigned long pStandalone;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteStartDocEnc_typ;

typedef struct xmlWriteEndDocument
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteEndDocument_typ;

typedef struct xmlWriteStartElement
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pName;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteStartElement_typ;

typedef struct xmlWriteAttribute
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pName;
	unsigned long pValue;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteAttribute_typ;

typedef struct xmlWriteElementText
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pText;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteElementText_typ;

typedef struct xmlWriteEndElement
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteEndElement_typ;

typedef struct xmlWriteComment
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pText;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteComment_typ;

typedef struct xmlWriteCData
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pCData;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWriteCData_typ;

typedef struct xmlWritePI
{
	/* VAR_INPUT (analog) */
	unsigned long ident;
	unsigned long pTarget;
	unsigned long pValue;
	/* VAR_OUTPUT (analog) */
	unsigned short status;
	/* VAR_INPUT (digital) */
	plcbit enable;
} xmlWritePI_typ;



/* Prototyping of functions and function blocks */
void xmlCreateMemoryReader(struct xmlCreateMemoryReader* inst);
void xmlCloseMemoryReader(struct xmlCloseMemoryReader* inst);
void xmlReadNextNode(struct xmlReadNextNode* inst);
void xmlReadAttributeNr(struct xmlReadAttributeNr* inst);
void xmlCreateMemoryWriter(struct xmlCreateMemoryWriter* inst);
void xmlGetMemoryInfo(struct xmlGetMemoryInfo* inst);
void xmlCloseMemoryWriter(struct xmlCloseMemoryWriter* inst);
void xmlWriteStartDocument(struct xmlWriteStartDocument* inst);
void xmlWriteStartDocEnc(struct xmlWriteStartDocEnc* inst);
void xmlWriteEndDocument(struct xmlWriteEndDocument* inst);
void xmlWriteStartElement(struct xmlWriteStartElement* inst);
void xmlWriteAttribute(struct xmlWriteAttribute* inst);
void xmlWriteElementText(struct xmlWriteElementText* inst);
void xmlWriteEndElement(struct xmlWriteEndElement* inst);
void xmlWriteComment(struct xmlWriteComment* inst);
void xmlWriteCData(struct xmlWriteCData* inst);
void xmlWritePI(struct xmlWritePI* inst);


#ifdef __cplusplus
};
#endif
#endif /* _ASXML_ */

                                                           
