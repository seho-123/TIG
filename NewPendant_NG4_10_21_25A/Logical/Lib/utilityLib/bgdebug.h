#ifndef _BG_DEBUG_H
#define _BG_DEBUG_H

#include <..\..\Lib\utilityLib\bgsprintf.h>
#include <..\..\Lib\utilityLib\bgringbuffer.h>
#include <..\..\Lib\utilityLib\bgstring.h>
//Include required .c library files here. That way it's not needed in AS.
#include <..\..\Lib\utilityLib\bgringbuffer.c>
#include <..\..\Lib\utilityLib\bgsprintf.c>
#include <..\..\Lib\utilityLib\bgstring.c>

//#define DBG_CK(format, args...) bgRingBuffPrintf(&gRingBuffer, format, ## args); //Telnet
//#define DBG_CK(format, args...)  xsprintf(WarnTxt , format, ## args);   ERRxwarning(0xC350, 0, WarnTxt); // Logger in AS
//#define DBG_LogMgr(format, args...)  xsprintf(WarnTxt , format, ## args);   LogTxt(&LogMgr, WarnTxt); // LogBook on Pendant

#define DBG_CK(format, args...) ;
#define DBG_LogMgr(format, args...)  ;



#ifdef WIN32
#define DBG printf
#define DBGMSG printf


#define DBGCRT printf
#define DBGERR printf
#define DBGWAR printf
#define DBGINF printf
#define DBGDBG printf

#else

#include <bur/plc.h>
//#include "bgsprintf.h"
//#include "bgringbuffer.h"

#define BG_DEBUG_LEVEL_CRITICAL		0
#define BG_DEBUG_LEVEL_ERROR		1
#define BG_DEBUG_LEVEL_WARNING		2
#define BG_DEBUG_LEVEL_INFO			3
#define BG_DEBUG_LEVEL_DEBUG		4

#define BG_DEBUG_UNUSED	0x0


/*
- To completly remove debug msgs from the source code use:
#define BG_DEBUG_ENABLE 0
*/
//#define BG_DEBUG_ENABLE 1

/*ptr to global ring buffer for debug messages*/
_GLOBAL bgRingBuffer_typ gRingBuffer;


#if BG_DEBUG_ENABLE == 1

//BG_DEBUG_ENABLE == 1 => debug messages present in the source code
#define DBG_OB(format, args...)  bgRingBuffPrintf(&gRingBuffer, format, ## args);
#define DBG(format, args...)  ;

//#define DBG(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_CRITICAL, BG_DEBUG_UNUSED, format, ## args);
#define DBGMSG(format, args...)	bgRingBuffPrintf(&gRingBuffer, format, ## args);


#define DBGCRT(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_CRITICAL, BG_DEBUG_UNUSED, format, ## args);
#define DBGERR(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_ERROR, BG_DEBUG_UNUSED, format, ## args);
#define DBGWAR(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_WARNING, BG_DEBUG_UNUSED, format, ## args);
#define DBGINF(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_INFO, BG_DEBUG_UNUSED, format, ## args);
#define DBGDBG(format, args...)	bgRingBuffPutDbgInfo(&gRingBuffer, __FILE__, __FUNCTION__, BG_DEBUG_LEVEL_DEBUG, BG_DEBUG_UNUSED, format, ## args);

#else	
/*BG_DEBUG_ENABLE == 0 => all debug messages removed from the source code*/

#define DBG_OB(format, args...)  	{};

#define DBG(format, args...)  		{};
#define DBGMSG(format, args...)  	{};

#define DBGCRT(format, args...)		{};
#define DBGERR(format, args...)		{};
#define DBGWAR(format, args...)		{};
#define DBGINF(format, args...)		{};
#define DBGDBG(format, args...)		{};

	
#endif /*#if BG_DEBUG_ENABLE*/

#endif /*#ifdef WIN32*/

#endif /*#ifndef _BG_DEBUG_H*/
