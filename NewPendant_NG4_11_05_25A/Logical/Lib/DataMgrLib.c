/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Library: DataMgrLib
 * File: DataMgrLib.c
 * Author: Christopher
 * Created: January 16, 2009
 *******************************************************************/

#ifndef _DATAMGRLIB_C
#define _DATAMGRLIB_C


#include <..\lib\DataMgrLib.h>



DatObjCreate_typ    DOCreate;
DatObjWrite_typ 	DOWrite;
DatObjRead_typ  	DORead;
DatObjCopy_typ  	DOCopy;
DatObjInfo_typ  	DOInfo;
DatObjDelete_typ  	DODelete;


signed long DataMgr_Srv(struct DataMgr_obj* mgr){

	if(!mgr) return -101;
	if(!mgr->DataIF) 			return -102;
	if(strlen(mgr->ObjName)==0)	return -103;
	if(!mgr->DataIF->pSource) 	return -104;
	if(!mgr->DataIF->pDest)   	return -105;


 /******************/

	if(mgr->_getInfo){
		/***/
		/* Initialize info structure */
		DOInfo.enable   		= 1;
		DOInfo.pName    		= (UDINT)mgr->ObjName;
		DatObjInfo(&DOInfo);
		mgr->Status 	= DOInfo.status;

		if(mgr->Status == 65535){ return 1;}  /* Function Block Busy - try again */ 
		else{					
			if (mgr->Status == 20609U){	/*#define doERR_MODULNOTFOUND 20609U*/			
				mgr->ObjReady				= 0;
				mgr->DataIF->DataObjReady	= 0;
				mgr->DataIF->CreateObj		= 1;
				mgr->Ident					= 0 ;
			}
			else 
		    {	
		    	mgr->ObjReady				= 1;
				mgr->DataIF->DataObjReady	= 1;
				mgr->DataIF->CreateObj		= 0;
				mgr->Ident					= DOInfo.ident;
			}			
			mgr->DataIF->CreateObj		= 0;
			mgr->DataIF->DeleteObj		= 0;
			mgr->DataIF->WriteData		= 0;
			
			mgr->_getInfo				= 0;
		}
	}	

   	if ( mgr->DataIF->CreateObj)  {
		
		DOCreate.enable		= 1;
		DOCreate.grp       	= 0;
		DOCreate.pName  	= (UDINT)mgr->ObjName;
		DOCreate.len        = mgr->ObjLen;
		DOCreate.MemType    = mgr->MemType;
		
		DOCreate.Option     = 0;
		DOCreate.pCpyData   = 0;

		/* Call FUB */
		DatObjCreate(&DOCreate);
		mgr->Status 		= DOCreate.status;
		mgr->Ident			= DOCreate.ident;
  
		/* Verify status */
		if (mgr->Status == 0)	{
			mgr->DataIF->CreateObj	= 0;
			mgr->_getInfo			= 1;			
		}
		else if(mgr->Status == 65535) {return 1;}		/* Function Block Busy - try again */ 
		else if (mgr->Status != 0xFFFF) {
			mgr->DataIF->CreateObj		= 0;
			mgr->ObjReady				= 0;
			mgr->DataIF->DataObjReady	= 0;               /* Set error level for DatObjCreate */
		}
	}


	if(mgr->ObjReady){		
	  
		if (mgr->DataIF->ReadData)  {

			DORead.enable      	= 1;
			DORead.ident        = mgr->Ident;
			DORead.Offset       = mgr->DataIF->Offset;
			DORead.pDestination = (UDINT) mgr->DataIF->pDest;
			DORead.len      	= mgr->DataIF->DataLen;

			/* Call FUB */
			DatObjRead(&DORead);
			mgr->Status 	= DORead.status;
			if(mgr->Status  == 0) {mgr->DataIF->ReadData = 0;}
			else 				   return 1;
				
		}
		else if (mgr->DataIF->WriteData){	
			
			DOWrite.enable  		= 1;
			DOWrite.ident   		= mgr->Ident;
			DOWrite.Offset  		= mgr->DataIF->Offset;
			DOWrite.pSource 		= (UDINT) mgr->DataIF->pSource;
			DOWrite.len     		= mgr->DataIF->DataLen;

			/* Call FUB */
			DatObjWrite(&DOWrite);
			mgr->Status 	= DOWrite.status;
			if(mgr->Status == 0) {mgr->DataIF->WriteData	= 0;}
			else				 return 1;
		}
		
		else if(mgr->DataIF->DeleteObj)	{			
			
		   	/* Initialize delete structure */
		   	DODelete.enable = 1;
			DODelete.ident = mgr->Ident;
		   
		  	/* Call FUB */
		   	DatObjDelete(&DODelete);
	  
			/* Get status */
		   	mgr->Status	= DODelete.status;
	  
		   	/* Verify status */
			if (mgr->Status == 0)	{
				mgr->DataIF->DeleteObj 		= 0;	
				mgr->ObjReady				= 0;
				mgr->DataIF->DataObjReady	= 0;
				mgr->Ident					= 0;
			}
			else if(mgr->Status == 65535) {return 1;}		/* Function Block Busy - try again */ 
			else if (mgr->Status != 0xFFFF) {
				mgr->DataIF->DeleteObj 		= 0;	
				mgr->ObjReady				= 0;
				mgr->DataIF->DataObjReady	= 0;
				mgr->Ident					= 0;
			}
		}
		
	}
	
	mgr->DataIF->Status = mgr->Status; 
	return mgr->Status;
}
#endif

