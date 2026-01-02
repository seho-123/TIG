/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: FileOper
 * File: FileOper.c
 * Author: ckaczkowski
 * Created: January 07, 2014
 ********************************************************************
 * Implementation of program FileOper
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\LoggerLib.c>



//#include <string.h>

_GLOBAL INT PleaseShowFileMessage;

#define MAX_XML_FILE_SIZE 10000000
_LOCAL FILE_RW_typ  *pFileRW ;
//_LOCAL XML_Method_typ *pXMLMethod;

_LOCAL plcstring byData[100];
_LOCAL plcstring topName[64];
_LOCAL signed long status_xml;
_LOCAL UINT ReadMode;
_LOCAL UINT WriteMode;

_LOCAL plcstring _fileName[64];
_LOCAL USINT load_after_save;
_LOCAL UDINT pos; 

_LOCAL UDINT SetupLocation , SetupSize;
_LOCAL UDINT SetupFullLocation , SetupFullSize;
_LOCAL UDINT CalibrLocation , CalibrSize;

Calib_Param_typ tempCalib[2];


_LOCAL plcbit KK_test;
_LOCAL USINT KK_Chain[32] , KK_Chain_Indx;
USINT _CMD ;
UINT _XMLHelper_Cmd;
_LOCAL plcstring CMD_from_Obed[128];
_LOCAL plcbit KK_OneShot;


/*
FileOpen_typ   FOpen;
FileClose_typ  FClose;
FileCreate_typ FCreate;
FileRead_typ   FRead;
FileWrite_typ  FWrite;
FileDelete_typ FDelete;
*/

void _Open(FILE_RW_typ *f);
void _Close(FILE_RW_typ *f);
void _Create(FILE_RW_typ *f);
void _Read(FILE_RW_typ *f);
void _Write(FILE_RW_typ *f);
void _Delete(FILE_RW_typ *f);
void _ReadDir(FILE_RW_typ *f);
void _ReadFileInfo(FILE_RW_typ *f);
void _Rename(FILE_RW_typ *f);
void _Copy(FILE_RW_typ *f);

void DeleteExtension(char *p);
plcbit Find_ext_txt(char *name , char* ext);



void _INIT FileOperINIT( void )
{
  /* Initialize variables */

  
  strcpy(File_RW.fileName , "default");
  strcpy(File_RW.deviceName, "WeldPrograms");	 
  

  File_RW.Offset = 0;

  File_RW.Mode = F_MODE_XML;

  SetupFullLocation	= &StartOfPermMemObj;	
  SetupFullSize = 48000; // AFTER ADDING PADDING AND MEMORY RESERVED SPACE - NOVEMBER , 2019

//pFileRW->pMem = (UDINT)&CurCfg;
//pFileRW->Offset = 0; 
//pFileRW->Len = sizeof(CurCfg);

 SetupLocation = (UDINT)&CurCfg;
 SetupSize = sizeof(CurCfg);

  CalibrLocation  = (UDINT)&CurCalib;
  //CalibrSize = 2364;
  CalibrSize = sizeof(CurCalib);
 
}

void _CYCLIC FileOperCYCLIC( void )
{
char tempStr[64];
UINT status;

	//return;

	pFileRW = (FILE_RW_typ*) &File_RW;

	if((pFileRW->CMD == 0) 
		&& (pFileRW->_int.step == 0)
		&&(XMLHelper.xCmd == 0)){

		pFileRW->Tmr.IN = 0;
		pFileRW->Tmr.Q = 0;


	}

	else if(TimerIsTimeOut(&pFileRW->Tmr)){

		pFileRW->Tmr.IN = 0;
		pFileRW->Tmr.Q = 0;

		if(XMLHelper.xCmd)
			pFileRW->_int.wStatus = 1981; // operation timeout
		else	
			pFileRW->_int.wStatus = 1957; // operation timeout
			
		pFileRW->_int.step  = 254; // error					
		
	}

	if(!_XMLHelper_Cmd && XMLHelper.xCmd){
		
		TimerStart(&pFileRW->Tmr, 15.0); // 15 sec to run command 	

	}
	_XMLHelper_Cmd = XMLHelper.xCmd;
	 	

 	 if(pFileRW->_int.step == 0){	


	 	/*
		  if(pFileRW->CMD > 0){		

			
			KK_Chain[KK_Chain_Indx] = pFileRW->CMD;
				KK_Chain_Indx++;

				if(KK_Chain_Indx >=32)
					KK_Chain_Indx = 0;
		  	

		  }		
		
			if(pFileRW->CMD && !KK_OneShot ){

				KK_OneShot = 1;

				xsprintf(CMD_from_Obed , "%d: Old: %s New: %s " , (DINT)pFileRW->CMD , pFileRW->fileName , pFileRW->fileNewName  );

				xsprintf(File_RW.MsgText , "%d: %s / %s " , (DINT)pFileRW->CMD , pFileRW->fileName , pFileRW->fileNewName  );

				LogTxt(&LogMgr , File_RW.MsgText );


			}
			*/

		if(pFileRW->CMD && !_CMD){

	

			switch(pFileRW->CMD){

				
				/* Nieskonczone
				case F_LOAD_CURCFG:


					pFileRW->_int.wStatus = 1987; // operation timeout
				
					pFileRW->_int.step  = 254; // error		
					
					pFileRW->CMD = 0;

					
				break;
				
				*/
				

				default:
					TimerStart(&pFileRW->Tmr, 10.0); // 15 sec to run command 	
				break;
			}
			


		}
		
		_CMD = pFileRW->CMD;

		

		  switch(pFileRW->CMD){


			case F_READ:
						
				pFileRW->Error = 0;			
				pFileRW->Done  = 0;	
				pFileRW->_int.step = 1;
				ReadMode = F_MODE_BINARY;
				
				
			break;	


			case F_WRITE:

				strcpy(pFileRW->fileName , pFileRW->fileNewName);

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 10;

				

			break;	

			case F_DELETE:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 70;

			break;	

			case F_READ_DIR:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 60;

			break;	

			case F_READ_FILE_INF:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 61;

			break;	

			case F_READ_DIR_FILELIST:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;		

				pFileRW->_int.step = 62;

			break;	
			

			case F_FREE_MEM:

				if(!TMP_free(pFileRW->_int.memlng , (void**) pFileRW->pMem)){
					pFileRW->pMem = 0;
					pFileRW->_int.memlng = 0;
					pFileRW->Len = 0;
					pFileRW->FileLocked = 0;
					strcpy(pFileRW->MsgText , "");
				}
			
			break;

			case F_READ_FILE_TO_TMP:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 20;

			break;	


			
			case F_SAVE_AS_TMP:

				strcpy(pFileRW->fileName , pFileRW->fileNewName);
			
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 30;

			break;	

			case F_LOAD_CURCFG:
				
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;

				pFileRW->_int.PathDoneCnt = 0;

				if(strstr(pFileRW->fileName , PRG_FILE_EXT)){

					ReadMode = F_MODE_XML;
					pFileRW->_int.step = 1;


				}
				else{

					ReadMode = F_MODE_BINARY;				
					pFileRW->_int.step = 40;

				}


				/*
				if(Master[0].AndIntrf[0].UserLevel >= 15){ // High engineer
				
					strcpy(_fileName , pFileRW->fileName );
					strcpy(pFileRW->fileName , CurCfg.CurProgramName);
					pFileRW->Error = 0;
					pFileRW->Done  = 0;
					pFileRW->_int.step = 50;

					load_after_save = 1;
				}
				*/	
				

			break;	

			case F_SAVE_SETUP_BIN:
			case F_SAVE_CURCFG:
			case F_SAVE_CURCFG_BIN:

				switch(pFileRW->CMD){

					case F_SAVE_SETUP_BIN: WriteMode = F_MODE_BINARY ; break;
					
					case F_SAVE_CURCFG_BIN: WriteMode = F_MODE_BINARY ; break;
					
					case F_SAVE_CURCFG: WriteMode = F_MODE_XML ; break;


				}

				if(strlen(CurCfg.CurProgramName) == 0)
					strcpy(CurCfg.CurProgramName, "Default");

				

				if(WriteMode == F_MODE_XML){

					DeleteExtension(CurCfg.CurProgramName);
					strcpy(pFileRW->fileNewName , CurCfg.CurProgramName);
					strcat(pFileRW->fileNewName , PRG_FILE_EXT);				
					
				}
				else
					strcpy(pFileRW->fileNewName , CurCfg.CurProgramName);
				

				if(pFileRW->CMD == F_SAVE_SETUP_BIN)
					strcpy(pFileRW->fileNewName , "setup");
					

				
				strcpy(pFileRW->fileName , pFileRW->fileNewName);
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 50;

			break;	

			case F_SAVE_AS_CALIB_BIN:
			case F_SAVE_AS_FULL_BIN:
			case F_SAVE_AS_CURCFG_BIN:
			case F_SAVE_AS_CURCFG:

				
				switch(pFileRW->CMD){
				
					case F_SAVE_AS_CURCFG_BIN: WriteMode = F_MODE_BINARY ; break;

					case F_SAVE_AS_FULL_BIN: WriteMode = F_MODE_BINARY_FULL ; break;	

					case F_SAVE_AS_CALIB_BIN: WriteMode = F_MODE_BINARY_CALIB ; break;
					
					case F_SAVE_AS_CURCFG: WriteMode = F_MODE_XML ; break; // To dziwne


				}

				if(strlen(CurCfg.CurProgramName) == 0)
					strcpy(CurCfg.CurProgramName, "Default");

				if(!pFileRW->DO_NOT_Del_Extension)	
					DeleteExtension(pFileRW->fileNewName);

				if(WriteMode == F_MODE_XML)
					strcat(pFileRW->fileNewName , PRG_FILE_EXT);				
				
				strcpy(pFileRW->fileName , pFileRW->fileNewName);			
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 50;

			break;	

			

			case F_COPY:

				if(strlen(pFileRW->fileNewName) == 0)
					strcpy(pFileRW->fileNewName, "Default");

				DeleteExtension(pFileRW->fileNewName);
				
				if(strstr(pFileRW->fileName , PRG_FILE_EXT))
					strcat(pFileRW->fileNewName , PRG_FILE_EXT);	
					

				strcpy(pFileRW->targetDeviceName, pFileRW->deviceName);

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 80;

			break;	


			case F_RENAME:

				if(strlen(pFileRW->fileNewName) == 0)
					strcpy(pFileRW->fileNewName, "Default");

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 90;

			break;			

			case F_OPEN_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 120;
				
			break;
			
			case F_CREATE_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 130;

			break;	

			
			case F_WRITE_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 140;
				
			break;

			
			case F_CLOSE_ONLY:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 150;
				
			break;


			case F_READ_FILES_CHAIN:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;

				strcpy(pFileRW->deviceName, "WeldPrograms");
		
				strcpy(gDirFileList, "filechain:" );								
				pos = strlen(gDirFileList);		

				
				if(strstr(pFileRW->fileName , PRG_FILE_EXT)){
					
					ReadMode = F_MODE_XML;
					pFileRW->_int.step = 1;

				}
				else{

					pFileRW->_int.step = 100; //F_GET_NEXT_FILE

				}			

				
			break;

			case F_GET_NEXT_FILE:
				
				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;
				pFileRW->_int.step = 100;


				if(strstr(pFileRW->fileName , PRG_FILE_EXT)){

					ReadMode = F_MODE_XML;
					pFileRW->_int.step = 1;


				}
				else{

					pFileRW->_int.step = 100;

				}

			break;	

			case F_SET_NEXT_FILE:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;

				
				if(pFileRW->Mode == F_MODE_XML)
					pFileRW->_int.step = 210; //F_SET_NEXT_FILE
					
				else	
					pFileRW->_int.step = 110;

			break;	


			case F_EDIT_CHAIN:

				pFileRW->Error = 0;
				strcpy(pFileRW->MsgText , "");
				pFileRW->Done  = 0;

				strcpy(pFileRW->deviceName, "WeldPrograms");


				if(pFileRW->Mode == F_MODE_XML){

					if(strstr(File_RW.fileNewName , PRG_FILE_EXT)){

						strcpy(topName, File_RW.fileName); 
						strcpy(pFileRW->fileName, File_RW.fileNewName);

						//strcpy(pFileRW->FileSchedule.nextFileName , File_RW.fileNewName); 				
						
						pFileRW->_int.step = 210; //F_SET_NEXT_FILE

					}
					else					
						pFileRW->_int.step = 254;  // error


				}else {
				
					strcpy(topName, File_RW.fileName); 
					strcpy(pFileRW->fileName, File_RW.fileNewName);

					//strcpy(pFileRW->FileSchedule.nextFileName , File_RW.fileNewName); 
					
					pFileRW->_int.step = 110; //F_SET_NEXT_FILE	
				
				}
				
				
			break;

			
			

			default:

				if(pFileRW->CMD)
					pFileRW->_int.step = 254;  // error

			break;	

			
		  }

  }

		
  switch (pFileRW->_int.step )
  {
  
		case 0: /**** First step ****/

			pFileRW->CMD = 0;
					
		break;


		///////////   F_READ //////////////////////

		case 1: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step  = 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){

				/*

				// Null Pointer: Allocate 
				if ( !pFileRW->pMem) {
					pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);
					pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;
				}
				///Pointer Assigned: Clear
				else if(pFileRW->_int.FOpen.filelen){
					
					if(!TMP_free(pFileRW->_int.memlng , (void**) pFileRW->pMem)){  
						pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);   
						pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;
					}
					else {
						pFileRW->_int.step  = 254;
						break;

					}
						
				}
				*/

				pFileRW->pMem = 0;
				pFileRW->Len = 0;

				pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);
				pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;

				
				if ( !pFileRW->_int.wStatus) {
					pFileRW->Len = pFileRW->_int.FOpen.filelen;
					
					pFileRW->_int.step++;;
				}
				else
					pFileRW->_int.step  = 254;

				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
     
		case 2: /**** Read data from file ****/

			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				
				if(ReadMode == F_MODE_XML){

				
					if((pFileRW->CMD == F_GET_NEXT_FILE) || (pFileRW->CMD == F_READ_FILES_CHAIN))
					
						XMLHelper.xCmd = XML_CMD_GET_NEXT_PROGR;
					
					else					
						XMLHelper.xCmd = XML_CMD_READ;
					

				}	
				
				
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
			
		break;	

		
		case 3: /**** Close file ****/

			if(ReadMode == F_MODE_XML){

				if(XMLHelper.xCmd) // let finish read elements
					break; 

			}

			_Close(pFileRW);
			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0){
		 	
		   	  //pFileRW->FileLocked = 1;

			  
				if(ReadMode == F_MODE_XML){	

					if((pFileRW->CMD == F_GET_NEXT_FILE) || (pFileRW->CMD == F_READ_FILES_CHAIN)){

						strcpy(	pFileRW->FileSchedule.nextFileName , XMLHelper.NextProgramName);
						pFileRW->FileSchedule.WeldProgramRepeat = XMLHelper.WeldProgramRepeat;						

						 if(pFileRW->CMD == F_READ_FILES_CHAIN)
							pFileRW->_int.step  = 250; 
						 
						 else
						 	pFileRW->_int.step  = 255; // Success	

					}
					else{

						strcpy(tempStr , pFileRW->fileName);
						DeleteExtension(tempStr);
					
						strcpy(CurCfg.WeldProgramName , tempStr);
						strcpy(CurCfg.CurProgramName , tempStr);

						pFileRW->NewFileLoaded = 1;

						pFileRW->_int.step  = 255; // Success	
						
					}

					if(!TMP_free(pFileRW->_int.memlng , (void**) pFileRW->pMem)){
						pFileRW->pMem = 0;
						pFileRW->_int.memlng = 0;
						pFileRW->Len = 0;
						pFileRW->FileLocked = 0;
					}

			   
				}
				else {	

					//pFileRW->pMem = 0;					
					pFileRW->_int.step  = 255; // Success	

				}
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;

		
		////////   F_WRITE  //////////////////////

		case 10: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus == 0)
			{
				
				pFileRW->_int.step++;
				pFileRW->_int.step++;
				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				pFileRW->_int.step  = 254; // Error
			if (pFileRW->_int.wStatus == 20799)
			{
			  pFileRW->_int.wError = FileIoGetSysError();
			}
			}
		break;

		case 11: /**** Create file ****/
			
			_Create(pFileRW);

			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
			 	pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 2;
				pFileRW->_int.step  = 254; // Error
				                    
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

		case 12: /**** Write data ****/

			_Write(pFileRW);
			
			if (pFileRW->_int.wStatus == 0)
			{          
			  	pFileRW->_int.step++;  
			  
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 3;
				pFileRW->_int.step  = 254; // Error
				if (pFileRW->_int.wStatus == 20799)
				{
				 	pFileRW->_int.wError = FileIoGetSysError();
				}
			}
		break;

     	
		case 13: /**** Close file ****/

			_Close(pFileRW);
	
		   if (pFileRW->_int.wStatus == 0)
		   {
			  pFileRW->_int.step  = 255; // Success	
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}				  
		break;

	////////////////  F_READ_FILE_TO_TMP //////////////////////// 


		case 20: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step	= 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){

				
				if(pFileRW->_int.FOpen.filelen == sizeof(pFileRW->curCfgTemp)){
					
					pFileRW->pMem = (UDINT)&pFileRW->curCfgTemp;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen; 				
					pFileRW->_int.step++;

					
				}
				else{
					pFileRW->_int.byErrorLevel = 7;
					pFileRW->_int.step	= 254;
				}	

				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
	 
		case 21: /**** Read data from file ****/
			
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	

		
		case 22: /**** Close file ****/

			_Close(pFileRW);
			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {	
			  //pFileRW->FileLocked = 1;
			  pFileRW->_int.step  = 255; // Success 
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;

		
		////////   F_SAVE_AS_TMP  //////////////////////

		case 30: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus == 0)
			{				
				pFileRW->_int.step += 2; // file exists
							
			}
			else if (pFileRW->_int.wStatus == fiERR_FILE_NOT_FOUND)
			{
				pFileRW->_int.step += 1; // create file 
			}
			 
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				pFileRW->_int.step	= 254; // Error 
				
			if (pFileRW->_int.wStatus == 20799)
			{
			  pFileRW->_int.wError = FileIoGetSysError();
			}
			}
		break;

		case 31: /**** Create file ****/
			
			_Create(pFileRW);

			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 2;
				pFileRW->_int.step	= 254; // Error 
									
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

		case 32: /**** Write data ****/

			pFileRW->pMem = (UDINT)&pFileRW->curCfgTemp;
			pFileRW->Offset = 0; 
			pFileRW->Len = sizeof(pFileRW->curCfgTemp);

			_Write(pFileRW);
			
			if (pFileRW->_int.wStatus == 0)
			{		   
				pFileRW->_int.step++;  
			  
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 3;
				pFileRW->_int.step	= 254; // Error 
				
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
		break;

		
		case 33: /**** Close file ****/

			_Close(pFileRW);

			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {	
		   		pFileRW->_int.step	= 255; // Success	

		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}				  
		break;


	/////////////////////////////////

		
			
	///////////   F_READ_CURCFG //////////////////////

		case 40: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step  = 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){


				
				memcpy(&tempCalib[0] , &CurCalib.Points[3] , sizeof(Calib_Param_typ)); // CALIB_TIG_AMPS_CMD
				memcpy(&tempCalib[1] , &CurCalib.Points[4] , sizeof(Calib_Param_typ)); // CALIB_HW_AMPS_CMD

				/*
				if(pFileRW->_int.FOpen.filelen == sizeof(CurCfg)){
					
					pFileRW->pMem = (UDINT)&CurCfg;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen;					
					pFileRW->_int.step++;

					
				}else 
			 	*/


				if(pFileRW->_int.FOpen.filelen == CalibrSize){
					
					pFileRW->pMem = CalibrLocation;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen;					
					pFileRW->_int.step++;

					
				}			
				else if(pFileRW->_int.FOpen.filelen == SetupFullSize){
					
					pFileRW->pMem = SetupFullLocation;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen;					
					pFileRW->_int.step++;

					
				}				
				else if(pFileRW->_int.FOpen.filelen == SetupSize){
					
					pFileRW->pMem = SetupLocation;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen;					
					pFileRW->_int.step++;

					
				}
				else{
					pFileRW->_int.byErrorLevel = 7;
					pFileRW->_int.step  = 254;
				}	

				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
     
		case 41: /**** Read data from file ****/
			
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{

				memcpy(&CurCalib.Points[3] , &tempCalib[0]  , sizeof(Calib_Param_typ)); // CALIB_TIG_AMPS_CMD
				memcpy(&CurCalib.Points[4] , &tempCalib[1]  , sizeof(Calib_Param_typ)); // CALIB_HW_AMPS_CMD

				if(!pFileRW->DO_NOT_Del_Extension)	{
					
					DeleteExtension( pFileRW->fileName);

					if(Find_ext_txt(File_RW.fileName , "_state"))
						File_RW.fileName[strlen(File_RW.fileName) - 6] = 0;

					if(Find_ext_txt(File_RW.fileName , "_setup"))
						File_RW.fileName[strlen(File_RW.fileName) - 6] = 0;

					if(Find_ext_txt(File_RW.fileName , "_calib"))
						File_RW.fileName[strlen(File_RW.fileName) - 6] = 0;

					if(Find_ext_txt(File_RW.fileName , "_prg"))
						File_RW.fileName[strlen(File_RW.fileName) - 4] = 0;
				
				}

				CurCfg.Dirty = 0;


				if(pFileRW->_int.FOpen.filelen != CalibrSize){

					strcpy(CurCfg.WeldProgramName , pFileRW->fileName);
					strcpy(CurCfg.CurProgramName , pFileRW->fileName);
					pFileRW->NewFileLoaded = 1;

				}

				pFileRW->_int.step++;
				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	

		
		case 42: /**** Close file ****/

			_Close(pFileRW);

			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {	
		   	  //pFileRW->FileLocked = 1;
		   	  pFileRW->_int.step  = 255; // Success	
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;

		
		////////   F_SAVE_CURCFG  //////////////////////

		case 50: /**** open existing file ****/
			
			_Open(pFileRW);
			

			 if (pFileRW->_int.wStatus == ERR_OK)
			{
				if(pFileRW->CMD == F_SAVE_AS_CURCFG){

					pFileRW->_int.byErrorLevel = 1;
					pFileRW->_int.wError = 0x0001;
					pFileRW->_int.step  = 254; // Error	
				}					
				else{						
					
					pFileRW->_int.step ++;

				}
							
			}
			else if (pFileRW->_int.wStatus == fiERR_FILE_NOT_FOUND )
			{

				pFileRW->_int.step += 2;
			
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				pFileRW->_int.step  = 254; // Error	
				
			if (pFileRW->_int.wStatus == 20799)
			{
			  pFileRW->_int.wError = FileIoGetSysError();
			}
			}
		break;

		case 51: /**** Delete file ****/

			_Delete(pFileRW);
			
			/* Verify status */
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)		   {			
				
		   	 	pFileRW->_int.step ++;		   	 
		   }
			
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 7;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}		
		break;

		case 52: /**** Create file ****/
			
			_Create(pFileRW);

			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				if(WriteMode == F_MODE_XML)
					XMLHelper.xCmd = XML_CMD_WRITE;	
				
			 	pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 2;
				pFileRW->_int.step  = 254; // Error	
				                    
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

		case 53: /**** Write data ****/

			if(WriteMode == F_MODE_XML){

				if(XMLHelper.xCmd == 0){
					
					pFileRW->pMem = XMLHelper.pMemory;
					pFileRW->Offset = 0; 
					pFileRW->Len = XMLHelper.MemorySize;

				}
				else
					break;
				
			}else{


				if(WriteMode == F_MODE_BINARY_CALIB){

					
					pFileRW->pMem = CalibrLocation;
					pFileRW->Offset = 0; 
					pFileRW->Len = CalibrSize;
				}

				else if(WriteMode == F_MODE_BINARY_FULL){

					
					pFileRW->pMem = SetupFullLocation;
					pFileRW->Offset = 0; 
					pFileRW->Len = SetupFullSize;
				}
				else{

					pFileRW->pMem = SetupLocation;
					pFileRW->Offset = 0; 
					pFileRW->Len = SetupSize;


				}
			}
			

			_Write(pFileRW);
			
			if (pFileRW->_int.wStatus == 0)
			{  
				if(WriteMode == F_MODE_XML){
					
					XMLHelper.xCmd = XML_CMD_CLOSE_WRITER;
					
				}
								
			  	pFileRW->_int.step++;  
			  
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				if(WriteMode == F_MODE_XML){
					
					XMLHelper.xCmd = XML_CMD_CLOSE_WRITER;
					
				}
								
				pFileRW->_int.byErrorLevel = 3;
				pFileRW->_int.step  = 254; // Error	
				
				if (pFileRW->_int.wStatus == 20799)
				{
				 	pFileRW->_int.wError = FileIoGetSysError();
				}
			}
		break;

     	
		case 54: /**** Close file ****/

			_Close(pFileRW);

			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {
		   		
				CurCfg.Dirty = 0;

				if(load_after_save){

					strcpy(pFileRW->fileName , _fileName  );	
					pFileRW->Error = 0;
					pFileRW->Done  = 0;
					pFileRW->_int.step = 40;

					load_after_save = 0;

				}
				else {

					if((pFileRW->CMD != F_SAVE_SETUP_BIN)
						&& (pFileRW->CMD != F_SAVE_AS_CURCFG_BIN)
						&& (pFileRW->CMD != F_SAVE_AS_FULL_BIN)
						&& (pFileRW->CMD != F_SAVE_AS_CALIB_BIN)){
						
						DeleteExtension(pFileRW->fileName);
						strcpy(CurCfg.CurProgramName , pFileRW->fileName);
											 
					}
					
					pFileRW->_int.step = 62; // get new directory info into string	
				

				}
				
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}				  
		break;

		//////////////////////////////////////////////////////

		case 60: /**** Get directory info ****/

			_ReadDir(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {
				//clear current list
				gDirFileList[0]=0;//TODO: may need to PV used for storing the dir list 			  

			  if( pFileRW->_int.FDirInfo.filenum > 0){
			  	pFileRW->_int.FFileInfo.entry = 0;			  
			  	pFileRW->_int.step  = 61; // Success	
			  }
			  else
			  	pFileRW->_int.step  = 255; // Success	
		   }
			
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			

		break;


		case 61: /**** Get file info ****/

			_ReadFileInfo(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {
		   	  pFileRW->_int.step  = 255; // Success	
		   }
			
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			

		break;
		

		case 62: /**** Get directory info > same as step 60 ****/

			_ReadDir(pFileRW);

		
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {

			  if( pFileRW->_int.FDirInfo.filenum > 0){
			  	pFileRW->_int.FFileInfo.entry = 0;
		
				strcpy(gDirFileList,  "filelist:" );								
				pos = strlen(gDirFileList);	

				gFileList_Indx = 0;
				memset(&gNewFileList[0][0] , 0 , sizeof(gNewFileList));
				
			  	pFileRW->_int.step  = 63; // Success	
			  }
			  else
			  	pFileRW->_int.step  = 255; // Success	
		   }
			
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			

		break;


		case 63: /**** Get file info > similar to step 61****/

			_ReadFileInfo(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0){
			
			  if( !pFileRW->_int.FDirInfo.filenum){
		   	  	pFileRW->_int.step  = 255; // Success
				break;				  			  
			  }

			  if(pos >= (DIR_FILELIST_MAX_SIZE - 100) ){
                      strcat(gDirFileList, "WARNING: file list not complete");                     
                      pFileRW->_int.step  = 255; // Success
                      break;
              }
			  
			  else{

				if(gFileList_Indx < 500)
					strcpy(&gNewFileList[gFileList_Indx++] , pFileRW->FileInfo[0].Filename );

			  
				xsprintf(tempStr , "%s," , pFileRW->FileInfo[0].Filename);
				strcpy(&gDirFileList[pos],tempStr);
				pos += strlen(tempStr); 


				/*

				if(pFileRW->Mode == F_MODE_XML){

					if(strstr(pFileRW->FileInfo[0].Filename , PRG_FILE_EXT)){

						xsprintf(tempStr , "%s," , pFileRW->FileInfo[0].Filename);
						strcpy(&gDirFileList[pos],tempStr);
						pos += strlen(tempStr); 


					}

				}
				else if(pFileRW->Mode == F_MODE_BINARY){

					if(!strstr(pFileRW->FileInfo[0].Filename , ".")){

						xsprintf(tempStr , "%s," , pFileRW->FileInfo[0].Filename);
						strcpy(&gDirFileList[pos],tempStr);
						pos += strlen(tempStr); 
					}


				}
				 */				
				
				

			  }
	  				
			
			  if( pFileRW->_int.FFileInfo.entry < (pFileRW->_int.FDirInfo.filenum - 1) ){	  
	  				  
                  
			  	pFileRW->_int.FFileInfo.entry++;			  
			  	pFileRW->_int.step  = 63; // Success: continue reading file info for all files
				
				//reset FBK to enable subsequent reads
				pFileRW->_int.FFileInfo.enable	= 0;
				/* Call FBK to disable it*/
				DirRead(&pFileRW->_int.FFileInfo);	

				
			  }else{			  
			  				  
		   	  	pFileRW->_int.step  = 255; // Success				  
			  }	
			  
		   }
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			

		break;
		
		////////////////////////////////////////////////////////
		// F_DELETE
		
		case 70: /**** Delete file ****/

			_Delete(pFileRW);
			
			/* Verify status */
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {
				
				
		   	  pFileRW->_int.step = 62; // get new directory info into string
		   	 
		   }
			
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 7;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}		
		break;

		

		////////////////////////////////////////////////////////
		// F_COPY
		
		case 80: /**** Copy file ****/

			_Copy(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {
			 	pFileRW->_int.step = 62; // get new directory info into string
			  //pFileRW->_int.step  = 255; // Success	
		   }
			
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			
		break;

		////////////////////////////////////////////////////////
		// F_RENAME
		
		case 90: /**** Rename  file ****/

			_Rename(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
		   {
		   	 pFileRW->_int.step = 62; // get new directory info into string
		   	 // pFileRW->_int.step  = 255; // Success	
		   }
			
			else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 6;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}			
		break;


		

		////////////////////////////////////////////////////////
		// F_GET_NEXT_FILE
		
		case 100: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step	= 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){

				
				if(pFileRW->_int.FOpen.filelen == sizeof(pFileRW->curCfgTemp)){
					
					//pFileRW->pMem = (UDINT)&pFileRW->curCfgTemp;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen; 				
					pFileRW->_int.step++;

					
				}
				else{
					pFileRW->_int.byErrorLevel = 7;
					pFileRW->_int.step	= 254;
				}	

				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
	 
		case 101: /**** Read data from file ****/

			pFileRW->pMem = pFileRW->FileSchedule.nextFileName;
			pFileRW->Len =  sizeof(pFileRW->curCfgTemp.NextProgramName);
			pFileRW->Offset = (UDINT)pFileRW->curCfgTemp.NextProgramName - (UDINT)&pFileRW->curCfgTemp;			
			
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error

				pFileRW->pMem = 0;
				pFileRW->Len = 0;
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;		


		case 102: /**** Read data from file ****/
		
			pFileRW->pMem = &pFileRW->FileSchedule.WeldProgramRepeat;
			pFileRW->Len =	sizeof(float);
			pFileRW->Offset = pFileRW->_int.OffsetToWeldProgramRepeat;
			
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				//pFileRW->_int.step++;
				//pFileRW->FileSchedule.WeldProgramRepeat = 12;
				pFileRW->_int.step = 104; // Do not read LinkMode it is not necessery 
				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;

				pFileRW->pMem = 0;
				pFileRW->Len = 0;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	
		

		case 103: /**** Read data from file ****/
		
			pFileRW->pMem = &pFileRW->FileSchedule.WeldProgramLinkMode;
			pFileRW->Len =	sizeof(DINT);
			pFileRW->Offset = pFileRW->_int.OffsetToWeldProgramLinkMode;
			
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;

				pFileRW->pMem = 0;
				pFileRW->Len = 0;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	

		
		case 104: /**** Close file ****/

			_Close(pFileRW);
	
		   if (pFileRW->_int.wStatus == 0)
		   {	
			  //pFileRW->FileLocked = 1;

			  	pFileRW->pMem = 0;
				pFileRW->Len = 0;

			  if(pFileRW->CMD == F_READ_FILES_CHAIN)
				pFileRW->_int.step  = 160; 
			  
			  else
			  	
			  	pFileRW->_int.step  = 255; // Success 
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;

			  	pFileRW->pMem = 0;
				pFileRW->Len = 0;
				
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;

		////////////////////////////////////////////////////////
		// F_SET_NEXT_FILE
		
		case 110: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step	= 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){


				if(!strcmp(pFileRW->fileName , CurCfg.CurProgramName)){

					strcpy(CurCfg.NextProgramName , pFileRW->curCfgTemp.NextProgramName);

				}

				
				if(pFileRW->_int.FOpen.filelen == sizeof(pFileRW->curCfgTemp)){
					
					pFileRW->pMem = (UDINT)&pFileRW->curCfgTemp;
					pFileRW->Offset = 0;
					pFileRW->Len = pFileRW->_int.FOpen.filelen; 				
					pFileRW->_int.step++;

					
				}
				else{
					pFileRW->_int.byErrorLevel = 7;
					pFileRW->_int.step	= 254;
				}	

				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
	 
		case 111: /**** Write data to file ****/

			pFileRW->pMem = pFileRW->FileSchedule.nextFileName;
			pFileRW->Len =  sizeof(pFileRW->curCfgTemp.NextProgramName);
			pFileRW->Offset = (UDINT)pFileRW->curCfgTemp.NextProgramName - (UDINT)&pFileRW->curCfgTemp;
			
			_Write(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				//pFileRW->_int.step++; // this was before
				pFileRW->_int.step = 114; // Only NextProgramName is updated
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	


		case 112: /**** Write data to file ****/
		
			pFileRW->pMem = &pFileRW->FileSchedule.WeldProgramLinkMode;
			pFileRW->Len =	sizeof(DINT);
			pFileRW->Offset = pFileRW->_int.OffsetToWeldProgramLinkMode;
			
			
			_Write(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	


		case 113: /**** Write data to file ****/
		
			pFileRW->pMem = &pFileRW->FileSchedule.WeldProgramRepeat;
			pFileRW->Len =	sizeof(float);
			pFileRW->Offset = pFileRW->_int.OffsetToWeldProgramRepeat;
						
			_Write(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	

		
		case 114: /**** Close file ****/

			_Close(pFileRW);
	
		   if (pFileRW->_int.wStatus == 0)
		   {	
			  //pFileRW->FileLocked = 1;
			  
			  //pFileRW->_int.step  = 255; // Success

			  if((pFileRW->CMD == F_EDIT_CHAIN)
			  	&& (topName[0] != ':') && (strlen(pFileRW->FileSchedule.nextFileName))){	
				
				
				strcpy(pFileRW->fileName , pFileRW->FileSchedule.nextFileName);
				strcpy(pFileRW->FileSchedule.nextFileName , "");

				pFileRW->_int.step  = 0;
			  	pFileRW->CMD = F_SET_NEXT_FILE;
				
				
			  }
			  else {
			  	
			  	//pFileRW->_int.step  = 255; // Success 

				if( strlen(topName)){

					if(topName[0] == ':')
						strcpy(pFileRW->fileName ,&topName[1]);
					else	
						strcpy(pFileRW->fileName ,topName);
						
					pFileRW->_int.step  = 0;
				  	pFileRW->CMD = F_READ_FILES_CHAIN;

				}
				else
					pFileRW->_int.step  = 255; // Success 
				
			  }	

			
			 
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;

		//////////////////////////////////////////////////////

		case 120: /** OPEN ONLY **/
			
			_Open(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	

		//////////////////////////////////////////////////////

		case 130: /** Create ONLY **/
			
			_Create(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	

		//////////////////////////////////////////////////////

		case 140: /** Write ONLY **/
			
			_Write(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step  = 254; // Error
				
				if (pFileRW->_int.wStatus == 20799)				
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	

		
		//////////////////////////////////////////////////////
		
		case 150: /** Close ONLY **/
			
			_Close(pFileRW);
				
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				 pFileRW->_int.step  = 255; // Success 
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}

		break;	



		//////////////////////////////////////////////////////
		
		case 160: /** Read Files Chain **/

			
			if(pos < (DIR_FILELIST_MAX_SIZE - 32)){


				xsprintf(tempStr , "%s:%f," , pFileRW->fileName ,pFileRW->FileSchedule.WeldProgramRepeat);
				strcpy(&gDirFileList[pos],tempStr);
				pos += strlen(tempStr);
				

				if((strlen(pFileRW->FileSchedule.nextFileName) == 0) // Dead end - no next file
					|| (strstr(gDirFileList , pFileRW->FileSchedule.nextFileName))){ // FileName does  exist in files chain - infinity loop

					if(strlen(pFileRW->FileSchedule.nextFileName)){ 
						xsprintf(tempStr , "%s:%f," , pFileRW->FileSchedule.nextFileName , 0 );
						strcpy(&gDirFileList[pos],tempStr);
						pos += strlen(tempStr);
					}					

					pFileRW->_int.step  = 255; // Success 

				}
				else {					
					

					strcpy(pFileRW->fileName ,pFileRW->FileSchedule.nextFileName);
					
					pFileRW->_int.step = 100; //F_GET_NEXT_FILE
					
				}			

				
			
			
			}
			else{ // buffer full
				
				pFileRW->_int.step = 254;  // error
				
			}		
						

		break;	

		////////////////////////////////////////////////////////
		// F_SET_NEXT_FILE XML
		
		case 210: /**** open existing file ****/
			
			_Open(pFileRW);

			/* status (20708 -> File doesn't exist) */
			if (pFileRW->_int.wStatus == 20708)
			{
				pFileRW->_int.step	= 254; // Error
			}
			else if (pFileRW->_int.wStatus == 0){			


				
				/*				
				// Null Pointer: Allocate 
				if ( !pFileRW->pMem) {
					pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);
					pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;
				}
				// Pointer Assigned: Clear 
				else if(pFileRW->_int.FOpen.filelen){
					if(!TMP_free(pFileRW->_int.memlng , (void**) pFileRW->pMem)){  
						pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);   
						pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;
					}
					else {
						pFileRW->_int.step  = 254;
						break;

					}
						
				}
				*/
				
				pFileRW->pMem = 0;
				pFileRW->Len = 0;

				pFileRW->_int.wStatus = TMP_alloc(pFileRW->_int.FOpen.filelen , (void**)&pFileRW->pMem);
				pFileRW->_int.memlng = pFileRW->_int.FOpen.filelen;

				
				if ( !pFileRW->_int.wStatus) {
					pFileRW->Len = pFileRW->_int.FOpen.filelen;
					
					pFileRW->_int.step++;;
				}
				else
					pFileRW->_int.step  = 254;
				
				
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 1;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
				  pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;
	 
		
		case 211: /**** Read data from file ****/
		
			_Read(pFileRW);
			
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)
			{
				DeleteExtension(pFileRW->FileSchedule.nextFileName);				
				
				strcpy(XMLHelper.NextProgramName , pFileRW->FileSchedule.nextFileName);				
				
				XMLHelper.xCmd = XML_CMD_SET_NEXT_PROGR;					
				
				pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 4;
				
				pFileRW->_int.step	= 254; // Error
				
				if (pFileRW->_int.wStatus == 20799) 			
					pFileRW->_int.wError = FileIoGetSysError();
				
			}
		break;	

		
		case 212: /**** Close file ****/

			if(XMLHelper.xCmd) // let finish read elements
				break; 


			_Close(pFileRW);
	
		   if (pFileRW->_int.wStatus == 0){
			
			
					
				if(!TMP_free(pFileRW->_int.memlng , (void**) pFileRW->pMem)){
					pFileRW->pMem = 0;
					pFileRW->_int.memlng = 0;
					pFileRW->Len = 0;
					pFileRW->FileLocked = 0;
				}		
							 
				

				pFileRW->_int.step = 215; // Success	then write file
			  
		   }
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)			  
				 pFileRW->_int.wError = FileIoGetSysError();
			  
			}				  
		break;	
		

		case 215: /**** Delete file ****/

			_Delete(pFileRW);
			
			/* Verify status */
			/* Verify status */
			if (pFileRW->_int.wStatus == 0)		   {			
				
		   	 	pFileRW->_int.step ++;		   	 
		   }
			
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 7;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}		
		break;

		case 216: /**** Create file ****/
			
			_Create(pFileRW);

			/* Verify status */
			if (pFileRW->_int.wStatus == 0)			{
				
			 	pFileRW->_int.step++;
			}
			else if (pFileRW->_int.wStatus != 65535)
			{
				pFileRW->_int.byErrorLevel = 2;
				pFileRW->_int.step  = 254; // Error	
				                    
				if (pFileRW->_int.wStatus == 20799)
				{
					pFileRW->_int.wError = FileIoGetSysError();
				}
			}
			
		break;

		case 217: /**** Write data ****/			
					
			pFileRW->pMem = XMLHelper.pMemory;
			pFileRW->Offset = 0; 
			pFileRW->Len = XMLHelper.MemorySize;			
			

			_Write(pFileRW);
			
			if (pFileRW->_int.wStatus == 0)
			{  
				XMLHelper.xCmd = XML_CMD_CLOSE_WRITER;
				pFileRW->_int.step++;  
			  
			}
			else if (pFileRW->_int.wStatus != 65535)		{
				
					
				XMLHelper.xCmd = XML_CMD_CLOSE_WRITER;				
				
				pFileRW->_int.byErrorLevel = 3;
				pFileRW->_int.step  = 254; // Error	
				
				if (pFileRW->_int.wStatus == 20799)
				{
				 	pFileRW->_int.wError = FileIoGetSysError();
				}
			}
		break;

     	
		case 218: /**** Close file ****/

			_Close(pFileRW);

			//pFileRW->_int.PathDoneCnt = 0;
	
		   if (pFileRW->_int.wStatus == 0)
		   {

		   		if( strlen(topName)){

					if(topName[0] == ':')
						strcpy(pFileRW->fileName ,&topName[1]);
					else	
						strcpy(pFileRW->fileName ,topName);
						
					pFileRW->_int.step  = 0;
				  	pFileRW->CMD = F_READ_FILES_CHAIN;

				}
				else
					pFileRW->_int.step  = 255; // Success 
					
		   		//DeleteExtension(pFileRW->fileName);
				//strcpy(CurCfg.CurProgramName , pFileRW->fileName);
				// pFileRW->_int.step  = 255; // Success		
					
			}
			  
		   else if (pFileRW->_int.wStatus != 65535)
		   {
			  pFileRW->_int.byErrorLevel = 5;
			  pFileRW->_int.step  = 254; // Error	
			  
			  if (pFileRW->_int.wStatus == 20799)
			  {
				 pFileRW->_int.wError = FileIoGetSysError();
			  }
			}				  
		break;

		
		
		//////////////////////////////////////////////////////
				
		case 250: /** Read Files Chain **/
			
			if(pos < (DIR_FILELIST_MAX_SIZE - 32)){


				xsprintf(tempStr , "%s:%f," , pFileRW->fileName ,pFileRW->FileSchedule.WeldProgramRepeat);
				strcpy(&gDirFileList[pos],tempStr);
				pos += strlen(tempStr);

				status = 0;

				if((strlen(pFileRW->FileSchedule.nextFileName) == 0) || !strcmp(pFileRW->FileSchedule.nextFileName, PRG_NO_NEXT)) {// Dead end - no next file
					status =1;
					strcpy(tempStr , "");
				}
				else{

					strcpy(tempStr , pFileRW->FileSchedule.nextFileName);
					DeleteExtension(pFileRW->FileSchedule.nextFileName);
					strcat(pFileRW->FileSchedule.nextFileName ,PRG_FILE_EXT);

					if(strstr(gDirFileList , pFileRW->FileSchedule.nextFileName)) // FileName does  exist in files chain - infinity loop
						status = 1;

				}
				
				if(status){

					
					if(strlen(tempStr)){ 						
						 
						xsprintf(tempStr , "%s:%f," , pFileRW->FileSchedule.nextFileName , 0 );
						strcpy(&gDirFileList[pos],tempStr);
						pos += strlen(tempStr);
						
					}	
					

					pFileRW->_int.step	= 255; // Success 

				}
				else {					
					

					strcpy(pFileRW->fileName ,pFileRW->FileSchedule.nextFileName);										
					pFileRW->_int.step = 1; //F_GET_NEXT_FILE XML
					
				}			

				
			
			
			}
			else{ // buffer full
				
				pFileRW->_int.step = 254;  // error
				
			}		
						

		break;	



		////////////////////////////////////////////////////////
		// Error

		case 254: /**** Error ****/

			
			//Errors(&ErrorMgr, ERROR_FATAL ,ERROR_FILE_RW,0);		


			switch(pFileRW->_int.wStatus){

				case 1957: //XML parser timeou
					strcpy(pFileRW->MsgText , "Err: File TimeOut"); break;		

				case 1981: //XML parser timeou
					strcpy(pFileRW->MsgText , "Err: File Corrupted"); break;	


				case 1987: //XML parser timeou
					strcpy(pFileRW->MsgText , "Err: Mach Run"); break;


				case 9999: //ERR_NOTIMPLEMENTED
					strcpy(pFileRW->MsgText , "Err: not Implemented"); break;				

				case 20700: //fiERR_INVALID_PATH 
					strcpy(pFileRW->MsgText , "Err: Invalid Path"); break;

				case 20705: //fiERR_EXIST
					strcpy(pFileRW->MsgText , "Err: File Already Exist"); break;				

				case 20799: //fiERR_SYSTEM
					strcpy(pFileRW->MsgText , "Err: System "); break;

				case 20798://fiERR_DEVICE_MANAGER
					strcpy(pFileRW->MsgText , "Err: Device Manager"); break;

				case 20797: //fiERR_DEVICE_DRIVER
					strcpy(pFileRW->MsgText , "Err: Device Driver"); break;

				case 20725: //fiERR_DIR_ALREADY_EXIST
					strcpy(pFileRW->MsgText , "Err: Directory Already Exist"); break;

				case 20724: //fiERR_DIR_NOT_EMPTY
					strcpy(pFileRW->MsgText , "Err: Directory Not Empty"); break;

				case 20723: //fiERR_DIR_NOT_EXIST
					strcpy(pFileRW->MsgText , "Err: Directory Not Exist"); break;

				case 20722: //fiERR_INVALID_DIRECTORY
					strcpy(pFileRW->MsgText , "Err: Invalid Directory"); break;

				case 20721:	//fiERR_FILE_NOT_OPENED
					strcpy(pFileRW->MsgText , "Err: File Not Opened"); break;

				case 20718:	//fiERR_COM_FILE_IOCTL
					strcpy(pFileRW->MsgText , "Err: File IO Control"); break;

				case 20717: //fiERR_COM_FILE_WRITE
					strcpy(pFileRW->MsgText , "Err: File Write"); break;

				case 20716:	//fiERR_COM_FILE_READ
					strcpy(pFileRW->MsgText , "Err: File Read"); break;

				case 20715:	//fiERR_COM_FILE_CLOSE
					strcpy(pFileRW->MsgText , "Err: File Close"); break;

				case 20714:	//fiERR_COM_FILE_OPEN
					strcpy(pFileRW->MsgText , "Err: File Open"); break;

				case 20708:	//fiERR_FILE_NOT_FOUND
					strcpy( pFileRW->MsgText , "Err: File Not Found"); break;

		
				default: xsprintf(pFileRW->MsgText , "Err.Cmd: %d Code %d" , pFileRW->CMD , pFileRW->_int.wStatus); break;

				

			}

			PleaseShowFileMessage = 1;	

			XMLHelper.xCmd = 0;
			XMLHelper.state = 0;
			XMLHelper.runState = 0;
			XMLHelper.errorState = 0;

			pFileRW->Error = 1;
			pFileRW->Done  = 1;	
			pFileRW->_int.step  = 0; 
			pFileRW->CMD = 0;
		

		break;	

		case 255: /**** Success****/

			pFileRW->Error = 0;
			pFileRW->Done  = 1;				
			pFileRW->_int.step  = 0;

			
			switch(	pFileRW->CMD){

				case F_READ: 				xsprintf(pFileRW->MsgText , "F_READ: %s" , pFileRW->fileName); PleaseShowFileMessage = 1; break;  
				case F_WRITE: 				xsprintf(pFileRW->MsgText , "F_WRITE: %s" , pFileRW->fileName);  PleaseShowFileMessage = 1; break;  
				case F_DELETE: 				xsprintf(pFileRW->MsgText , "F_DELETE: %s" , pFileRW->fileName);  PleaseShowFileMessage = 1; break;  
				case F_FREE_MEM: 			xsprintf(pFileRW->MsgText , "F_FREE_MEM"); break;  
								
				case F_LOAD_CURCFG:

					if(pFileRW->_int.FOpen.filelen == CalibrSize)
						xsprintf(pFileRW->MsgText , "F_LOAD_CALIB: %s" ,  pFileRW->fileName);  
								
					else if(pFileRW->_int.FOpen.filelen == SetupFullSize)						
						xsprintf(pFileRW->MsgText , "F_LOAD_STATE: %s" ,  pFileRW->fileName);  

									
					else if(pFileRW->_int.FOpen.filelen == SetupSize)						
						xsprintf(pFileRW->MsgText , "F_LOAD_SETUP: %s" ,  pFileRW->fileName); 
						
					else
						xsprintf(pFileRW->MsgText , "F_LOAD_CURCFG ??: %s" ,  pFileRW->fileName); 	

					PleaseShowFileMessage = 1;
						

				break;
				
				case F_SAVE_CURCFG: 		xsprintf(pFileRW->MsgText , "F_SAVE_CURCFG: %s" , pFileRW->fileName);  PleaseShowFileMessage = 1; break;  
				case F_SAVE_AS_CURCFG: 		xsprintf(pFileRW->MsgText , "F_SAVE_AS_CURCFG: %s" , pFileRW->fileNewName); PleaseShowFileMessage = 1;  break;  
				case F_SAVE_AS_CURCFG_BIN: 	xsprintf(pFileRW->MsgText , "F_SAVE_AS_BIN: %s", pFileRW->fileNewName);  PleaseShowFileMessage = 1; break;  
				case F_SAVE_AS_FULL_BIN: 	xsprintf(pFileRW->MsgText , "F_SAVE_AS_FULL_BIN: %s", pFileRW->fileNewName);  PleaseShowFileMessage = 1; break;  
				case F_SAVE_AS_CALIB_BIN: 	xsprintf(pFileRW->MsgText , "F_SAVE_AS_CALIB_BIN: %s", pFileRW->fileNewName);  PleaseShowFileMessage = 1; break;  
								
				case F_RENAME: 				xsprintf(pFileRW->MsgText , "F_RENAME: %s" , pFileRW->fileName);  PleaseShowFileMessage = 1; break;  
				case F_COPY: 				xsprintf(pFileRW->MsgText , "F_COPY: %s" , pFileRW->fileName);  PleaseShowFileMessage = 1; break;  
				case F_GET_NEXT_FILE: 		xsprintf(pFileRW->MsgText , "F_GET_NEXT_FILE: %s" , pFileRW->fileName); break;  	
				case F_SET_NEXT_FILE: 		xsprintf(pFileRW->MsgText , "F_SET_NEXT_FILE: %s" , pFileRW->fileName); break;  
				case F_READ_FILE_TO_TMP: 	xsprintf(pFileRW->MsgText , "F_READ_FILE_TO_TMP"); break;  
				case F_SAVE_AS_TMP: 		xsprintf(pFileRW->MsgText , "F_SAVE_AS_TMP"); break;  
								
				case F_READ_DIR: 			xsprintf(pFileRW->MsgText , "F_READ_DIR"); break;  
				case F_READ_FILE_INF: 		xsprintf(pFileRW->MsgText , "F_READ_FILE_INF"); break;  
				case F_READ_DIR_FILELIST: 	xsprintf(pFileRW->MsgText , "F_READ_DIR_FILELIST"); break;  

				
				case F_READ_FILES_CHAIN:	xsprintf(pFileRW->MsgText , "Done: F_READ_FILES_CHAIN"); break;	


			}
			
			pFileRW->CMD = 0;
			

		break;

			
  }

	TimerExecute(&pFileRW->Tmr);

}


void _Open(FILE_RW_typ *f)
{
	
	f->_int.FOpen.enable		= 1;
	f->_int.FOpen.pDevice   = (UDINT)f->deviceName;
	f->_int.FOpen.pFile = (UDINT)f->fileName;
	f->_int.FOpen.mode	  = FILE_RW;
	FileOpen(&f->_int.FOpen);

	f->_int.dwIdent = f->_int.FOpen.ident;
	f->_int.wStatus = f->_int.FOpen.status;

	return;
}
void _Close(FILE_RW_typ *f){

	f->_int.FClose.enable	 = 1;
	f->_int.FClose.ident 	 = f->_int.dwIdent;
	FileClose(&f->_int.FClose);

	f->_int.wStatus = f->_int.FClose.status;

	
	return;

		   
}
void _Create(FILE_RW_typ *f){

	
	f->_int.FCreate.enable	= 1;
	f->_int.FCreate.pDevice = (UDINT) f->deviceName;
	f->_int.FCreate.pFile   = (UDINT)f->fileNewName;
	FileCreate(&f->_int.FCreate);

	f->_int.dwIdent = f->_int.FCreate.ident;
	f->_int.wStatus = f->_int.FCreate.status;

	
	return;
	
}
void _Read(FILE_RW_typ *f){

	f->_int.FRead.enable	  = 1;
	f->_int.FRead.ident 	= f->_int.dwIdent;
	f->_int.FRead.offset	= f->Offset;
	f->_int.FRead.pDest 	= f->pMem;
	f->_int.FRead.len		= f->Len;
	/* Call FBK */
	FileRead(&f->_int.FRead);
	/* Get status */
	f->_int.wStatus = f->_int.FRead.status;

	
	return;
			
}
void _Write(FILE_RW_typ *f){

	f->_int.FWrite.enable   = 1;
	f->_int.FWrite.ident    = f->_int.dwIdent;
	f->_int.FWrite.offset   = f->Offset;
	f->_int.FWrite.pSrc	   = f->pMem;
	f->_int.FWrite.len	   = f->Len;

	FileWrite(&f->_int.FWrite);

	f->_int.wStatus = f->_int.FWrite.status;

	
	return;
	

}


void _Rename(FILE_RW_typ *f){

	
	f->_int.FRename.enable 	= 1;
	f->_int.FRename.pDevice 	= (UDINT) f->deviceName;
	f->_int.FRename.pName		=  (UDINT)f->fileName;
	f->_int.FRename.pNewName 	=  (UDINT)f->fileNewName;

	FileRename(&f->_int.FRename);
	
	f->_int.wStatus = f->_int.FRename.status;
	
}
void _Copy(FILE_RW_typ *f){

	f->_int.FCopy.enable 	= 1;
	f->_int.FCopy.pSrcDev	= (UDINT) f->deviceName;
	f->_int.FCopy.pSrc		=  (UDINT)f->fileName;
	
	if(f->targetDeviceName)
		f->_int.FCopy.pDestDev	= (UDINT) f->targetDeviceName;
	else
		f->_int.FCopy.pDestDev = (UDINT) f->deviceName;
	
	f->_int.FCopy.pDest		=  (UDINT)f->fileNewName;
	f->_int.FCopy.option    = FILE_OW_ONCE;
	

	FileCopy(&f->_int.FCopy);
	
	f->_int.wStatus = f->_int.FCopy.status;
}

void _Delete(FILE_RW_typ *f){

	f->_int.FDelete.enable	 = 1;
	f->_int.FDelete.pDevice = (UDINT) f->deviceName;
	f->_int.FDelete.pName	=  (UDINT)f->fileName;
	/* Call FBK */
	FileDelete(&f->_int.FDelete);
	/* Get status */
	f->_int.wStatus = f->_int.FDelete.status;

	
	return;
}

void _ReadDir(FILE_RW_typ *f){

	
	/* Initialize info structure */
	f->_int.FDirInfo.enable	  = 1;
	f->_int.FDirInfo.pDevice = (UDINT) f->deviceName;
	f->_int.FDirInfo.pPath 	= 0;

	/* Call FBK */
	DirInfo(&f->_int.FDirInfo);

	/* Get status */
	f->_int.wStatus = f->_int.FDirInfo.status;

	
	return;
			
}
void _ReadFileInfo(FILE_RW_typ *f){

	/* Initialize read directory structure */
	f->_int.FFileInfo.enable	= 1;
	f->_int.FFileInfo.pDevice = (UDINT) f->deviceName;;
	f->_int.FFileInfo.pPath 	= 0;

	if(f->_int.FFileInfo.entry >= f->_int.FDirInfo.filenum)
		f->_int.FFileInfo.entry = 0;
	
	f->_int.FFileInfo.option	= FILE_FILE;
	f->_int.FFileInfo.pData 	= (UDINT) &f->FileInfo[0];
	f->_int.FFileInfo.data_len	= sizeof (f->FileInfo[0]);

	/* Call FBK */
	DirRead(&f->_int.FFileInfo);

	/* Get status */
	f->_int.wStatus = f->_int.FFileInfo.status;
	return;
			
}

void DeleteExtension(char *p){

UINT len , i;

	len = strlen(p);

	for(i=0 ; i < len ; i++){

		if(p[i]=='.'){

			p[i] = 0;
			break;

		}

	}

	return;

}
plcbit Find_ext_txt(char *name , char* ext){
UINT len , ext_len;

		len = strlen(name);
		
		ext_len = strlen(ext);
		
		if(len >= ext_len){

			if(strcmp(&name[len-ext_len] ,ext) == 0)
				return 1;
			else
				return 0;
				
		}
		else
			return 0;


}



