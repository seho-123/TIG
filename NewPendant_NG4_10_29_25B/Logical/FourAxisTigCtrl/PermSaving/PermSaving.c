/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: PermSaving
 * File: PermSaving.c
 * Author: ckaczkowski
 * Created: January 4, 2017
 ********************************************************************
 * Implementation of program PermSaving
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\GlobDef.h>
#include <..\GlobDecl.h>
//#include <..\HotWire\GlobDef.h>  // To use hardware mapping in other projects

_LOCAL UDINT       		dwIdent;
 DatObjCreate_typ 	DOCreate;
DatObjWrite_typ 	DOWrite;
DatObjRead_typ  	DORead;
DatObjInfo_typ  	DOInfo;
DatObjDelete_typ     DODelete;

_LOCAL  CurCfg_typ *curCfg; 
_LOCAL	CurCalib_typ *curCalib;

Calib_Param_typ tempCalib[2];


_LOCAL UINT step;
_LOCAL plcstring PermName[32];
_LOCAL UDINT StartPointer , DataLen;
_LOCAL  UINT saveTmr;

_LOCAL plcbit KK_test;
void _INIT PermSavingINIT(void)
{

	
			
	curCfg = (UDINT)&CurCfg;
	curCalib = (UDINT)&CurCalib;
	

	strcpy(PermName , "PermObj");
	//StartPointer	= &NextProjectConfig;
	//StartPointer	= &NextProj_Padding;
	StartPointer	= &StartOfPermMemObj;
	//DataLen = (UDINT)&CurCalib - StartPointer;
		
	//DataLen = 47848; // with extended mapping values : y, z , etc
	DataLen = 48000; // AFTER ADDING PADDING AND MEMORY RESERVED SPACE - NOVEMBER , 2019
	ReadPermMemory_HS.Go = 0;
	ReadPermMemory_HS.Ready = 0;
	ReadPermMemory_HS.Error = 0;

	ReadCfgFile_HS.Go = 0;
	ReadCfgFile_HS.Ready = 0;
	ReadCfgFile_HS.Error = 0;

	SparePermObjSpace[0] = SparePermObjSpace[0];

	
	
}

void _CYCLIC PermSavingCYCLIC(void)
{
	

	if(ReadPermMemory_HS.Go){
		ReadPermMemory_HS.Go = 0;
		ReadPermMemory_HS.Ready = 0;
		ReadPermMemory_HS.Error = 0;
		step = 1;

	}

	if(KK_test){
		
		ReadPermMemory_HS.Ready = 1;
		return;
	}

	switch(step){


		case 0:
			
			ReadPermMemory_HS.Ready = 0;
			ReadPermMemory_HS.Error = 0;

			step ++;


		break;	


		case 1:		

			xsprintf(PermName , "PermObj_%d" ,NextHeadConfig_Indx );

			/* Initialize info structure */
			DOInfo.enable   		= 0;
			DOInfo.pName    		= (UDINT) PermName;
			DatObjInfo(&DOInfo);
			DOInfo.enable   		= 1;
			/***/	
			step ++;  			

		break;	


		case 2:

			/* Call  FUB  */
			/***/	
			DatObjInfo(&DOInfo);
			if (DOInfo.status == doERR_MODULNOTFOUND) 				
				step = 20; // Not exist - create new 

			else if(DOInfo.status != 65535){
				
				if(DataLen != DOInfo.len)
					step = 10; // wrong size - delete then create

				else 
					step ++;  // OK - Read Values

			}
			
		break;


		case 3:

			memcpy(&tempCalib[0] , &CurCalib.Points[CALIB_TIG_AMPS_CMD] , sizeof(Calib_Param_typ));
			memcpy(&tempCalib[1] , &CurCalib.Points[CALIB_HW_AMPS_CMD] , sizeof(Calib_Param_typ));

			DORead.enable	   		= 0;
			DORead.ident 	   		= DOInfo.ident;
			DORead.pDestination 	= StartPointer;
			DORead.len		  		= DataLen;
			DORead.Offset	   		= 0;	
			DatObjRead(&DORead);
			DORead.enable	   		= 1;

			step ++; 

		break;	

		case 4:

			/* Call FUB */
			DatObjRead(&DORead);

			if(DORead.status == 65535)
				break;

			else if(DORead.status != 0) // Error
				step = 10; // error - delete then create

			else{ // Read OK - start writing in elapse time

				memcpy(&CurCalib.Points[CALIB_TIG_AMPS_CMD] , &tempCalib[0]  , sizeof(Calib_Param_typ));
				memcpy(&CurCalib.Points[CALIB_HW_AMPS_CMD] , &tempCalib[1]  , sizeof(Calib_Param_typ));

				ReadPermMemory_HS.Go = 0;
				ReadPermMemory_HS.Ready = 1;
				ReadPermMemory_HS.Error = 0;
				saveTmr = 0;				
				step = 50;
			}	
			
		break;	


		case 10:

			DODelete.enable = 0;
			DODelete.ident = DOInfo.ident;
			DatObjDelete(&DODelete);
			DODelete.enable = 1;
			step ++; 			

		break;	


		case 11:

			DatObjDelete(&DODelete);
			if(DODelete.status == 0)
				step = 20;


		break;

		case 20:

			DOCreate.enable	   	= 0;
		   	DOCreate.grp 	   	= 0;
		   	DOCreate.pName	   	= (UDINT) PermName;
		   	DOCreate.len 	   	=  DataLen;
		   	DOCreate.MemType    	= doUSRROM;
		   	DOCreate.Option	   	= 0;
		   	DOCreate.pCpyData   	= StartPointer;
			DatObjCreate(&DOCreate);
			DOCreate.enable	   	= 1;

			step ++; 

		break;	


		case 21:

			/* Call FUB */
		   	DatObjCreate(&DOCreate);
			if(DOCreate.status == 0xFFFF)
				break;
			
			else if(DOCreate.status != 0) // error - start again
				step = 1;

			else{ // Create OK - read info
				
				ReadPermMemory_HS.Go = 0;
				ReadPermMemory_HS.Ready = 1;
				ReadPermMemory_HS.Error = 0;
				saveTmr = 0;
				//step = 1;
				step = 100;

			}

		break;	


		case 50:

			//ReadCfgFile_HS.Go = 1;
			ReadCfgFile_HS.Error = 0;
			ReadCfgFile_HS.Ready = 0;
			step++;


		break;

		case 51:

			//if(ReadCfgFile_HS.Ready || ReadCfgFile_HS.Error)
			strcpy(ArcLinkPS_IP_Address , "192.168.10.2" );
			step++;

		break;

		case 52:

			step = 100;

		break;

		case 100:

			saveTmr++;

			if(saveTmr> 200) {// 50 ms * 200 =  10 sec 
				saveTmr = 0;

				if(!STOP_SavingData)
					step ++;

			}

		break;	


		case 101:

			DOWrite.enable	   		= 0;
			DOWrite.ident 	   		= DOInfo.ident;
			DOWrite.pSource 		= StartPointer;
			DOWrite.len		 		= DataLen;
			DOWrite.Offset	 		= 0;		
			DatObjWrite(&DOWrite);
			DOWrite.enable	   		= 1;

			step ++; 

		break;

		case 102:
			
			/* Call FUB */
			DatObjWrite(&DOWrite);

			if(DOWrite.status == 65535)
				break;

			else if(DOWrite.status != 0) // Error
				step = 10; // error - delete then create

			else{ // Write OK - start writing in elapse time

				saveTmr = 0;
				step = 100;
			}	

		break;	

		case 110: // Trap


		break;


		default:

			//ReadPermMemory_HS.Ready = 0;

		break;	
	}	


	
}
