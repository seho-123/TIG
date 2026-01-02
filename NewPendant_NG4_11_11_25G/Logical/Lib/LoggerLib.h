/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Library: Logger
 * File: LoggerLib.h
 * Author: Christopher
 * Created: December 26, 2008
 *******************************************************************/

#include <bur/plctypes.h>



//#include <..\bglib\bgsprintf.h>


#ifndef _LOGGERLIB_H
  	#define _LOGGERLIB_H

	#include <dataobj.h>
	#include <AsTime.h>
	#include <..\..\lib\xstring.c>
	


	#define MAXLOGS				20 // No of logs in View
	#define MAXLOGLEN			96 // Max log Len in View 
	#define MAXLOGINPUT			48 // Log Rec len


	typedef struct LOGMGRCMD_obj
	{	
		UINT scrollBy;
		plcbit ScrollUp;
		plcbit ScrollDwn;
		plcbit LastLogs;
		plcbit EnterLog;
		plcbit Refresh;
		plcbit PauseRefresh;
		plcbit PauseLog;
		plcbit PauseRefreshNewLog;
		plcbit EraseAllLogs;
		
	}LOGMGRCMD_obj;

	typedef struct LOGMGRINT_obj
	{	
		UDINT *pLogID;
		plcbit DataObjReady;
		plcbit WriteData;
		plcbit ReadData;
		plcbit Create;
		unsigned short Status;
		UINT MaxLogs;
		char  logBuf[4+4+MAXLOGINPUT+1];
		plcstring  dbName[12+1];	
		UDINT recLen;
	
		UDINT       		dwIdent;
		DatObjCreate_typ 	DOCreate;
		DatObjWrite_typ 	DOWrite;
		DatObjRead_typ  	DORead;
		DatObjInfo_typ  	DOInfo;
			
	}LOGMGRINT_obj;

	typedef struct LOGMGR_obj
	{	
		plcstring LogView[MAXLOGS][MAXLOGLEN+1];
		plcstring LastLog[MAXLOGLEN+1];
		plcstring Logtxt_inp[128];
		plcbit	  Ready;	
		UINT 	  curLog;
		LOGMGRINT_obj _int;	
		LOGMGRCMD_obj Cmd;	
		
	}LOGMGR_obj;

	/****************************************/
	/**    Functions Prototypes            **/

	DINT LoggerInit(LOGMGR_obj *log ,UDINT *Id, char *dbName , UINT maxLogs);
	DINT LoggerSrv(LOGMGR_obj *log);

	int LogTxt(LOGMGR_obj *log , char *fmt, ...);


	/****************************************/

#endif	



