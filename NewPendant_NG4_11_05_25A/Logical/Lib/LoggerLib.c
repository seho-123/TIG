/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Library: Logger
 * File: LoggerLib.c
 * Author: Christopher
 * Created: December 26, 2008
 *******************************************************************/
#ifndef _LOGGERLIB_C
	#define _LOGGERLIB_C

#include <bur/plctypes.h>


#include <..\..\Lib\LoggerLib.h>



	USINT buf1[MAXLOGS * ((MAXLOGINPUT+1) + sizeof(plcdt)+ sizeof(plctime))];
	USINT buf2[MAXLOGS * ((MAXLOGINPUT+1) + sizeof(plcdt)+ sizeof(plctime))];

	void SetTimeStamp(plctime t , plcdt dt, char * p , char * txt);

	//#include <..\globalvar.h>

	DINT LoggerInit(LOGMGR_obj *log ,UDINT *Id, char *dbName , UINT maxLogs){

		log->_int.MaxLogs = maxLogs;

		if(dbName == 0) if(Id == 0) return -200; 

		// New 8 Nov 19	
		if(*Id > maxLogs)
			*Id = 0;
		//////////////
		
		strcpy ( log->_int.dbName , dbName);
		if(Id == 0) return -100;  
		log->_int.pLogID  = (UDINT*) Id;	

		log->_int.recLen = MAXLOGINPUT + sizeof(plcdt)+ sizeof(plctime);
		

		/* Initialize info structure */
		log->_int.DOInfo.enable   		= 1;
		log->_int.DOInfo.pName    		= (UDINT) log->_int.dbName;
		/***/	

		/* Call  FUB  */
		/***/	
		DatObjInfo(&log->_int.DOInfo);
		log->_int.Status 	= log->_int.DOInfo.status;

		if (log->_int.Status == doERR_MODULNOTFOUND)  {
			log->_int.DataObjReady	= 0;
			log->Ready				= 0;
			log->_int.WriteData		= 0;
			log->_int.ReadData		= 0;
			log->_int.Create 		= 1;
			return -1;
		}
		else if( log->_int.MaxLogs * (sizeof( log->Logtxt_inp) + sizeof(plcdt) + sizeof(plctime)) != log->_int.DOInfo.len){
			log->_int.DataObjReady	= 0;
			log->Ready				= 0;
			log->_int.WriteData		= 0;
			log->_int.ReadData		= 0;
			log->_int.Create 		= 0;
			return -2;
			
		}
		else {
			log->_int.DataObjReady	= 1;
			log->Ready				= 1;
			log->_int.WriteData		= 0;
			log->_int.ReadData		= 1;
			log->_int.Create 		= 0;
			log->_int.dwIdent 		= log->_int.DOInfo.ident;
			return 1;
			
		}
	}	

	DINT LoggerSrv(LOGMGR_obj *log ){
	INT i , j , k , l;
	DTGetTime_typ DTGetTime_1;
	DTStructure _DTStruct;
	plcdt		_dt;
	plctime _t;
	char _buf[50];
	char *p;
	UDINT buf1Len , buf2Len , buf1Offset , buf2Offset;	


		// New 8 Nov 19
		if(!log->_int.pLogID)
			return -1;
		
		if(*log->_int.pLogID > log->_int.MaxLogs)
			*log->_int.pLogID = 0;
		////////////////////

		if( log->Cmd.EraseAllLogs)  {
			log->Cmd.EraseAllLogs = 0;
			log->_int.Create = 1;
			log->Cmd.Refresh = 1;

		}

		if ( log->_int.Create )  {
			(*log->_int.pLogID) = 0;
		   	log->_int.DOCreate.enable	   = 1;
		   	log->_int.DOCreate.grp 	   = 0;
		   	log->_int.DOCreate.pName	   = (UDINT) log->_int.dbName;
		   	log->_int.DOCreate.len 	   =  log->_int.MaxLogs * (sizeof( log->Logtxt_inp) + sizeof(plcdt)+ sizeof(plctime));
		   	log->_int.DOCreate.MemType    = doUSRROM;
		   	log->_int.DOCreate.Option	   = 0;
		   	log->_int.DOCreate.pCpyData   = 0;

		   	/* Call FUB */
		   	DatObjCreate(&log->_int.DOCreate);
		   	log->_int.Status   = log->_int.DOCreate.status;
		   	log->_int.dwIdent			   = log->_int.DOCreate.ident;
	 
		   	/* Verify status */
		   	if (log->_int.Status == 0) {
			   log->_int.DataObjReady 	= 1;
			   log->Ready				= 1;
		   	}
			else if(log->_int.Status == 65535) return 0; /** Function Busy - try again **/
		   	else if (log->_int.Status != 0xFFFF) {			  
		   	}
			log->_int.Create = 0;
		 }		   

		 if(!log->_int.DataObjReady) return -100;
		 
		/** *******************************/	
		if(log->Cmd.EnterLog ){
			log->Cmd.EnterLog = 0;

			if(log->Cmd.PauseLog) return 100;

			/* Get Date Time String */
			DTGetTime_1.enable = 1;
			DTGetTime(&DTGetTime_1);

			memset(log->_int.logBuf , 0 , sizeof(log->_int.logBuf));
			memcpy(log->_int.logBuf , &DTGetTime_1.DT1 , sizeof(plcdt));
			_t = clock_ms();
			memcpy(log->_int.logBuf + sizeof(plcdt) ,&_t , sizeof(plctime)); 

			if(strlen(log->Logtxt_inp) >= MAXLOGINPUT)
				log->Logtxt_inp[MAXLOGINPUT-1] = 0;

			strcpy(log->_int.logBuf + sizeof(plcdt)+ sizeof(plctime) , log->Logtxt_inp);

			/*
			if(strlen(log->Logtxt_inp)< MAXLOGINPUT)
				strcpy(log->_int.logBuf + sizeof(plcdt)+ sizeof(plctime) , log->Logtxt_inp);
			else
				strcpy(log->_int.logBuf , "log txt too long ");
			*/
				
			log->_int.DOWrite.enable	= 1;
		   	log->_int.DOWrite.ident		= log->_int.dwIdent;
		   	log->_int.DOWrite.Offset	= (*log->_int.pLogID)  * log->_int.recLen;
		   	log->_int.DOWrite.pSource	= (UDINT)log->_int.logBuf ;
		   	log->_int.DOWrite.len		= log->_int.recLen;
			
		   	/* Call FUB */
		    DatObjWrite(&log->_int.DOWrite);
		   	log->_int.Status   = log->_int.DOWrite.status;		
			if(log->_int.Status == 20605){
				LoggerInit(log , log->_int.pLogID , log->_int.dbName , log->_int.MaxLogs);
				return 1;
			}	
			else if(log->_int.Status == 65535) return 0; /** Function Busy - try again **/
				log->Cmd.EnterLog = 0;
				
			if(log->_int.Status == 0){
				(*log->_int.pLogID) ++;			
				SetTimeStamp(_t ,DTGetTime_1.DT1,log->LastLog , log->Logtxt_inp);
				
			}
			if((*log->_int.pLogID)> (log->_int.MaxLogs-1)) (*log->_int.pLogID) = 0;
				log->_int.DOWrite.enable	= 0;
				
			if(!log->Cmd.PauseRefreshNewLog)log->Cmd.Refresh = 1;
						
		}

		if((log->Cmd.scrollBy > log->_int.MaxLogs) ||
			(log->Cmd.scrollBy == 0)) log->Cmd.scrollBy = 1;	
		
		
		if(log->Cmd.ScrollUp){
			log->Cmd.ScrollUp = 0;
			log->curLog -= log->Cmd.scrollBy ;
			log->Cmd.Refresh = 1;
		}

		if(log->Cmd.ScrollDwn){
			log->Cmd.ScrollDwn = 0;
			log->curLog += log->Cmd.scrollBy ;
			log->Cmd.Refresh = 1;
		}

		if(log->Cmd.LastLogs){
			log->Cmd.LastLogs = 0;
			log->curLog = 0;
			log->Cmd.Refresh = 1;
		}
		
		
		if(log->Cmd.Refresh){	

			if(log->Cmd.PauseRefresh){
				log->Cmd.Refresh = 0;
				return 101;
			}	

			if(log->curLog > log->_int.MaxLogs) log->curLog = 0;

			if((*log->_int.pLogID) >= (log->curLog  + MAXLOGS)){
				buf1Len 	= MAXLOGS;
				buf1Offset 	= ((*log->_int.pLogID)  - log->curLog -  MAXLOGS) * log->_int.recLen ;
				buf2Len 	= 0;
				buf2Offset 	= 0;
			}

			else {
				if((*log->_int.pLogID)  >= log->curLog){
					buf1Len 	= (*log->_int.pLogID)  -  log->curLog; 
					buf1Offset 	= 0;
					buf2Len 	= MAXLOGS - buf1Len;
					buf2Offset 	= (log->_int.MaxLogs - buf2Len ) * log->_int.recLen;
				}
				else{
					buf1Len 	= 0;
					buf1Offset 	= 0;
					buf2Len		= MAXLOGS;
					buf2Offset	=(log->_int.MaxLogs - (log->curLog - (*log->_int.pLogID)) - MAXLOGS) * log->_int.recLen;
				}			
			}

			memset(&log->LogView , 0 , sizeof(log->LogView));	
			k = 0;

			/******* buf 1 **/
			if(buf1Len > 0){
				log->_int.DORead.enable	   		= 1;
				log->_int.DORead.ident 	   		= log->_int.dwIdent;
				log->_int.DORead.pDestination 	= (UDINT) &buf1;
				log->_int.DORead.len		  	= buf1Len * log->_int.recLen;
				log->_int.DORead.Offset	   		= buf1Offset;

				/* Call FUB */
				if(log->_int.DOWrite.enable	== 0){;
					DatObjRead(&log->_int.DORead);					
					log->_int.Status   = log->_int.DORead.status;
					if(log->_int.DORead.status == 0) log->_int.DORead.enable = 0;
					if(log->_int.Status == 20605){
						LoggerInit(log , log->_int.pLogID , log->_int.dbName , log->_int.MaxLogs);
						return 1;
					}	
				}
				
				if(log->_int.DORead.enable == 0){
					for(i = (buf1Len -1) ; i >= 0  ; i--){	
						if(k > (MAXLOGS -1)) break;

						p =  &buf1[i * log->_int.recLen];
						
						memcpy(&_dt , p , sizeof(plcdt));
						memcpy(&_t , p +  sizeof(plcdt), sizeof(plctime));
						
						SetTimeStamp(_t , _dt,&log->LogView[k] , &buf1[i * log->_int.recLen + sizeof(plcdt)+ sizeof(plcdt)]);				

						k++;
					}
				}
				else return 0;
			}
			
			/******* buf 2 **/
			if(buf2Len > 0){
				log->_int.DORead.enable	   		= 1;
				log->_int.DORead.ident 	   		= log->_int.dwIdent;
				log->_int.DORead.pDestination 	= (UDINT) &buf2;
				log->_int.DORead.len		  	= buf2Len * log->_int.recLen;
				log->_int.DORead.Offset	   		= buf2Offset;

				/* Call FUB */
				if(log->_int.DOWrite.enable	== 0){;
					DatObjRead(&log->_int.DORead);					
					log->_int.Status   = log->_int.DORead.status;
					if(log->_int.DORead.status == 0) log->_int.DORead.enable = 0;
					if(log->_int.Status == 20605){
						LoggerInit(log , log->_int.pLogID , log->_int.dbName , log->_int.MaxLogs);
						return 1;
					}	
				}

				if(log->_int.DORead.enable == 0){
					for(i = (buf2Len -1) ; i >= 0  ; i--){	
						if(k > (MAXLOGS -1)) break;

						p =  &buf2[i * log->_int.recLen];
						
						memcpy(&_dt , p , sizeof(plcdt));
						memcpy(&_t , p +  sizeof(plcdt), sizeof(plctime));
						
						SetTimeStamp(_t , _dt,&log->LogView[k] , &buf2[i * log->_int.recLen] + sizeof(plcdt)+ sizeof(plcdt));		
						
						k++;
					}
				}
				else return 0;
			}	

			log->Cmd.Refresh = 0;
			
		}
	}

	//int sprintf(char *out, const char *format, ...)

	int LogTxt(LOGMGR_obj *log , char *fmt, ...)
	{
	int ret ;
	char *pstr;
	va_list args;
	DTGetTime_typ DTGetTime_1;
	plcdt		_dt;
	plctime _t;
	char *where;
	int     retv;
  	va_list varg;



			if(!log->_int.DataObjReady) return -100;
			if(log->Cmd.PauseLog) 		return 102;

			where  = log->Logtxt_inp;
			va_start(varg, fmt);
			retv = xprint(xputs, &where, fmt, varg);
			*where = 0;
			va_end(varg);

			
			if(strlen(log->Logtxt_inp) >= MAXLOGINPUT)
				log->Logtxt_inp[MAXLOGINPUT-1] = 0;
				
			pstr = LEFT(log->Logtxt_inp , (strlen(log->Logtxt_inp) - 0));

				
			
			/* Get Date Time String */
			DTGetTime_1.enable = 1;
			DTGetTime(&DTGetTime_1);

			memset(log->_int.logBuf , 0 ,sizeof(log->_int.logBuf));
			memcpy(log->_int.logBuf , &DTGetTime_1.DT1 , sizeof(plcdt));
			_t = clock_ms();
			memcpy(log->_int.logBuf + sizeof(plcdt) ,&_t , sizeof(plctime)); 
			
			/*
			if(strlen(pstr)< MAXLOGINPUT){				
				xsprintf((log->_int.logBuf + 8),"%s",pstr );
											
			}
			else{
				strcpy(log->_int.logBuf +  sizeof(plcdt) +  sizeof(plctime) , "log txt too long ");
			}
			*/					

			xsprintf((log->_int.logBuf + 8),"%s",pstr );
			
			log->_int.DOWrite.enable	= 1;
		   	log->_int.DOWrite.ident		= log->_int.dwIdent;
		   	log->_int.DOWrite.Offset	= (*log->_int.pLogID) * log->_int.recLen;
		   	log->_int.DOWrite.pSource	= (UDINT)log->_int.logBuf ;
		   	log->_int.DOWrite.len		= log->_int.recLen;
			
		   	/* Call FUB */
			TRY_AGAIN:
		    DatObjWrite(&log->_int.DOWrite);
		   	log->_int.Status   = log->_int.DOWrite.status;	

			if (log->_int.Status != 0 && log->_int.Status != 0xFFFF) // Error
				return -1;

			
			
			if(log->_int.Status == 0){
				(*log->_int.pLogID) ++;			
				SetTimeStamp(_t , DTGetTime_1.DT1,log->LastLog , pstr);
				
			}
			else 
				goto TRY_AGAIN;
			
				

			if((*log->_int.pLogID)> (log->_int.MaxLogs-1)) (*log->_int.pLogID) = 0;
				log->_int.DOWrite.enable	= 0;
				
			if(!log->Cmd.PauseRefreshNewLog)log->Cmd.Refresh = 1;
			
	        return ret;
	}	


void SetTimeStamp(plctime t , plcdt dt, char * p , char * txt){

DTStructure _DTStruct;
TIMEStructure TimeStruct;


	DT_TO_DTStructure(dt, &_DTStruct); 
	TIME_TO_TIMEStructure(t, (UDINT) &TimeStruct); 
	//xsprintf(p,"%d-%02d:%02d:%02d-%d >%s" ,_DTStruct.day ,_DTStruct.hour , _DTStruct.minute , _DTStruct.second , TimeStruct.millisec , txt);
	//xsprintf(p,"%02d-%d >%s" , _DTStruct.second , TimeStruct.millisec , txt);
	xsprintf(p,"%02d:%02d:%02d-%d >%s" ,_DTStruct.hour , _DTStruct.minute , _DTStruct.second ,TimeStruct.millisec , txt);

	return;

}

#endif	
