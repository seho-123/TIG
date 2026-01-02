               #include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>

#include <..\..\Lib\utilityLib\bgdebug.h>

#include <..\..\Lib\libParser\libparser.h>
#include <..\..\Lib\libParser\libparser.c>
#include <..\..\Lib\LoggerLib.c>

static plcstring WarnTxt[128];


#include <math.h>


_LOCAL FileOpen_typ   FOpen;
_LOCAL FileClose_typ  FClose;
_LOCAL FileCreate_typ FCreate;
_LOCAL FileRead_typ   FRead;
_LOCAL FileWrite_typ  FWrite;
_LOCAL FileDelete_typ FDelete;
//_LOCAL DirInfo_typ	   FDirInfo;
//_LOCAL DirRead_typ	   FFileInfo;

_LOCAL Cfg_typ *cfg;

_LOCAL USINT 	step;
_LOCAL USINT 	byErrorLevel;
_LOCAL UINT		wStatus;
_LOCAL UINT		wError;
_LOCAL UDINT	dwIdent;
_LOCAL UDINT	memlng;	

_LOCAL plcstring fileName[64];
//_LOCAL plcstring fileNewName[64];

_LOCAL plcstring deviceName[32];
//_LOCAL plcstring targetDeviceName[32];

_LOCAL UDINT pMem;
_LOCAL UDINT Len;
_LOCAL UDINT Offset;

_LOCAL plcstring OneLineBuf[256];
_LOCAL UINT ParamIndex;
_LOCAL UINT TypeIndex;

int GetNext_Param();

#define ParamList 		0
#define IntParamList 		1
#define EnumParamList	2
#define StringParamList	3


///  Cfg Parser
_LOCAL MACHCFG_ParserMgr	MachCfgMgr;
_LOCAL CFGKEY KeyWordCfg[MAX_KEYWORDS];

//////////////////////////////

void _INIT ProgTest_Init(void)
{

	//////////////////////////////
	MachCfgMgr._keyword_adr	= (UDINT)KeyWordCfg;
	MachCfgMgr.pMaster	= (UDINT)&Master[0];
	/* KeyWord Definition: Initialization*/
	KeyWord_init(KeyWordCfg );

}

void _CYCLIC ProgTest_Cyclic(void)
{

	cfg = (Cfg_typ*)Master[0].pCfg;

	

	switch(step){


		case 0:

			if(ReadCfgFile_HS.Go){

				ReadCfgFile_HS.Ready = 0;
				ReadCfgFile_HS.Error = 0;
				step = 100;

			}
			else if(Export_HMI_Names_File_HS.Go){

				Export_HMI_Names_File_HS.Ready = 0;
				Export_HMI_Names_File_HS.Error = 0;
				step = 1;

			}


		break;


		case 1:

			TypeIndex = ParamList;
			step ++;

		break;	
		
	
		case 2:

			strcpy(deviceName, "HMI");	

			switch(TypeIndex){


				case ParamList:

					strcpy(fileName, "ParamList_Float.csv");
					xsprintf(OneLineBuf , "Index , EnumText , Name ,  ShortName ,  LongName  \r\n");

				break;	


				case IntParamList:

					strcpy(fileName, "ParamList_Integer.csv");
					xsprintf(OneLineBuf , "Index , EnumText , Name ,   ShortName , LongName  \r\n");

				break;


				case EnumParamList:

					strcpy(fileName, "ParamList_Enumer.csv");
					xsprintf(OneLineBuf , "Index , EnumText , Name , Number of Enumer ,  Enum Txt #1 , Enum Txt #2 , Enum Txt #3 , Enum Txt #4 , Enum Txt #5\r\n");

				break;

				case StringParamList:

					strcpy(fileName, "ParamList_String.csv");
					xsprintf(OneLineBuf , "Index , EnumText , Name ,  ShortName , LongName  \r\n");

				break;

				default:

					strcpy(fileName, "None.csv");
					xsprintf(OneLineBuf , "None \r\n");


				break;	

			}

		
			
			pMem = (UDINT)OneLineBuf;
			Len = strlen(pMem);			
			ParamIndex = 0;
			Offset = 0;
			
			step = 10;

		break;	

		case 10: /**** open existing file ****/
			
			 FOpen.enable		= 1;
			 FOpen.pDevice   = (UDINT) deviceName;
			 FOpen.pFile = (UDINT) fileName;
			 FOpen.mode	  = FILE_RW;
			FileOpen(& FOpen);

			 dwIdent =  FOpen.ident;
			 wStatus =  FOpen.status;


			/* status (20708 -> File doesn't exist) */
			if ( wStatus == 20708)
			{
				step = 15;
			}
			else if ( wStatus == 0)
			{
				
				step = 20; // delete
								
			}
			else if ( wStatus != 65535)
			{
				 byErrorLevel = 1;
				step  = 254; // Error
				if ( wStatus == 20799)
				{
				   wError = FileIoGetSysError();
				}
			}
		break;

		case 15: /**** Create file ****/
			
			 FCreate.enable	= 1;
			 FCreate.pDevice = (UDINT) deviceName;
			 FCreate.pFile   = (UDINT) fileName;
			FileCreate(& FCreate);

			 dwIdent =  FCreate.ident;
			 wStatus =  FCreate.status;

			/* Verify status */
			if ( wStatus == 0)
			{
			 	step = 25; // write
			}
			else if ( wStatus != 65535)
			{
				 byErrorLevel = 2;
				step  = 254; // Error
				                    
				if ( wStatus == 20799)
				{
					 wError = FileIoGetSysError();
				}
			}
			
		break;


		case 20: // Delete File


			 FDelete.enable	 = 1;
			 FDelete.pDevice = (UDINT) deviceName;
			 FDelete.pName	=  (UDINT)fileName;
			/* Call FBK */
			FileDelete(& FDelete);
			/* Get status */
			 wStatus =  FDelete.status;



			/* Verify status */
			if ( wStatus == 0)		   {			
				
				step  = 10;		   	 
			}

			else if ( wStatus != 65535)
			{
			   byErrorLevel = 7;
			   step  = 254; // Error	
			  
			  if ( wStatus == 20799)
			  {
				  wError = FileIoGetSysError();
			  }
			}		



		break;	

		case 25: /**** Write data ****/
			
			 FWrite.enable   = 1;
			 FWrite.ident    =  dwIdent;
			 FWrite.offset   =  Offset;
			 FWrite.pSrc	   =  pMem;
			 FWrite.len	   =  Len;

			FileWrite(& FWrite);

			 wStatus =  FWrite.status;
			
			if ( wStatus == 0)
			{      

				switch(TypeIndex){

					
					case ParamList:

						if(GetNext_Param()<= 0)
			  				step = 30; // Done - Close file  			  	

					break;	

					case IntParamList:

						if(GetNext_IntParam()<= 0)
			  				step = 30; // Done - Close file  			  	

					break;

					case EnumParamList:

						if(GetNext_EnumParam()<= 0)
			  				step = 30; // Done - Close file  			  	

					break;

					case StringParamList:

						if(GetNext_StringParam()<= 0)
			  				step = 30; // Done - Close file  			  	

					break;



				}	
				
			}
			else if ( wStatus != 65535)
			{
				 byErrorLevel = 3;
				step  = 254; // Error
				if ( wStatus == 20799)
				{
				 	 wError = FileIoGetSysError();
				}
			}
		break;

     	
		case 30: /**** Close file ****/

			 FClose.enable	 = 1;
			 FClose.ident 	 =  dwIdent;
			FileClose(& FClose);

			 wStatus =  FClose.status;


			if ( wStatus == 0)
			{

				if(TypeIndex >= StringParamList)
			  		step  = 255; // Success	
			  		
			  	else{

					TypeIndex ++;
					step  = 2;

			  	}	
			}
			else if ( wStatus != 65535)
			{
			   byErrorLevel = 5;
			  step  = 254; // Error
			  if ( wStatus == 20799)
			  {
				  wError = FileIoGetSysError();
			  }
			}				  
		break;

		////////////////////////////////////////////////////
		/////   Reading Configuration


		case 100:

			strcpy(deviceName, "CFG");	
			strcpy(fileName, "Cfg.txt");
			Offset = 0;
			step++;

		break;


		case 101: /**** open existing file ****/
			
			 FOpen.enable		= 1;
			 FOpen.pDevice   = (UDINT) deviceName;
			 FOpen.pFile = (UDINT) fileName;
			 FOpen.mode	  = FILE_RW;
			FileOpen(& FOpen);

			 dwIdent =  FOpen.ident;
			 wStatus =  FOpen.status;


		 	if ( wStatus == 0)
			{
				pMem = 0;
				Len = 0;
				wStatus = TMP_alloc(FOpen.filelen , (void**)&pMem);
				
				if ( !wStatus) {
					Len = FOpen.filelen;					
					step++; 
				}
				else
					step  = 254; // Error
				
				
								
			}
			else if ( wStatus != 65535)
			{
				 byErrorLevel = 1;
				step  = 254; // Error
				if ( wStatus == 20799)
				{
				   wError = FileIoGetSysError();
				}
			}
		break;


		case 102: /**** Read data ****/
			
			 FRead.enable   = 1;
			 FRead.ident    =  dwIdent;
			 FRead.offset   =  Offset;
			 FRead.pDest   =  pMem;
			 FRead.len	   =  Len;

			FileRead(&FRead);

			 wStatus =  FWrite.status;
			
			if ( wStatus == 0)
			{  		
				step++; 
			
			}
			else if ( wStatus != 65535)
			{
				byErrorLevel = 1;
				step  = 254; // Error
				
				if ( wStatus == 20799)
				{
				   wError = FileIoGetSysError();
				}
			}

		break;	

		case 103:

			MachCfgMgr.f = pMem;
			ParseMachCfgFile( &MachCfgMgr, Len );
			step++;

		break;

		case 104:

			FClose.enable	 = 1;
			FClose.ident 	 =  dwIdent;
			FileClose(& FClose);

			 wStatus =  FClose.status;

			if ( wStatus == 0)
			{
				if(!TMP_free(Len , (void**) pMem)){
					pMem = 0;
					Len = 0;
					step = 255; 			
				}

			}
			else if ( wStatus != 65535)
			{
			   byErrorLevel = 5;
			  step  = 254; // Error
			  if ( wStatus == 20799)
			  {
				  wError = FileIoGetSysError();
			  }
			}	
			
		break;

		////////////////////////////////////////////////////
		case 254: // Error

			if(ReadCfgFile_HS.Go){

				ReadCfgFile_HS.Go = 0;
				ReadCfgFile_HS.Ready = 0;
				ReadCfgFile_HS.Error = 1;
				step = 0;

			}
			else if(Export_HMI_Names_File_HS.Go){

				Export_HMI_Names_File_HS.Go = 0;
				Export_HMI_Names_File_HS.Ready = 0;
				Export_HMI_Names_File_HS.Error = 1;
				step = 0;

			}


		break;

		case 255: // Done

			if(ReadCfgFile_HS.Go){

				ReadCfgFile_HS.Go = 0;
				ReadCfgFile_HS.Ready = 1;
				ReadCfgFile_HS.Error = 0;
				step = 0;

			}
			else if(Export_HMI_Names_File_HS.Go){

				Export_HMI_Names_File_HS.Go = 0;
				Export_HMI_Names_File_HS.Ready = 1;
				Export_HMI_Names_File_HS.Error = 0;
				step = 0;

			}

		break;	


		default:

			step = 0;
			
		break;

		
	}
	
}

int GetNext_Param(){
Cfg_typ* cfg;
Scaled_Var_obj *var;

	if(Master[0].varLastNum == 0)
		return -1;

	if(!Master[0].pCfg)
		return -2;

	if(!Master[0].pScaledVar)
		return -3;

	cfg = Master[0].pCfg;
	var = (Scaled_Var_obj*)  Master[0].pScaledVar;
	

	if(ParamIndex < Master[0].varLastNum){

		pMem = (UDINT)OneLineBuf;		
		Offset += Len;

		while((strlen(var[ParamIndex].enumTXT) == 0) &&  ParamIndex < Master[0].varLastNum){

			ParamIndex++;
		}	

		
		xsprintf(OneLineBuf , "%d , %s, %s , Please  Add ShortName , Please  Add LongName  \r\n" , ParamIndex , var[ParamIndex].enumTXT ,var[ParamIndex].name );
		Len = strlen(OneLineBuf);

		ParamIndex ++;
			
		return 1;
	}
	else
		return 0;
}


int GetNext_IntParam(){
Cfg_typ* cfg;
Scaled_Var_obj *var;

	if(Master[0].varIntLastNum == 0)
		return -1;

	if(!Master[0].pCfg)
		return -2;	

	cfg = Master[0].pCfg;	

	if(ParamIndex < Master[0].varIntLastNum){

		pMem = (UDINT)OneLineBuf;		
		Offset += Len;
		
		while((strlen(cfg->IntVar[ParamIndex].enumTXT) == 0) &&  ParamIndex < Master[0].varIntLastNum){

			ParamIndex++;
		}	

		
		xsprintf(OneLineBuf , "%d , %s, %s , Please  Add ShortName , Please  Add LongName  \r\n" , ParamIndex , cfg->IntVar[ParamIndex].enumTXT ,cfg->IntVar[ParamIndex].name);
		Len = strlen(OneLineBuf);

		ParamIndex ++;
			
		return 1;
	}
	else
		return 0;
}

int GetNext_EnumParam(){
Cfg_typ* cfg;
Scaled_Var_obj *var;
UINT i;
char* p;
char txt[32];

	if(Master[0].varLastNum == 0)
		return -1;

	if(!Master[0].pCfg)
		return -2;


	cfg = Master[0].pCfg;

	memset(OneLineBuf , 0, sizeof(OneLineBuf));

	if(ParamIndex < (Master[0].varEnumLastNum -1)){

		pMem = (UDINT)OneLineBuf;		
		Offset += Len;

		while((strlen(cfg->EnumVar[ParamIndex].enumTXT) == 0) &&  ParamIndex < Master[0].varEnumLastNum){

			ParamIndex++;
		}

		xsprintf(OneLineBuf , " %d , %s , %s , %d" , ParamIndex , cfg->EnumVar[ParamIndex].enumTXT ,  cfg->EnumVar[ParamIndex].name , cfg->Param_ENUM[ParamIndex].max +1);

		//p = OneLineBuf + strlen(OneLineBuf);	
				
		for(i = 0 ; i <= cfg->Param_ENUM[ParamIndex].max; i++){	
			
			xsprintf(txt, ", %s" ,  cfg->TXT_Resources[cfg->EnumVar[ParamIndex].TxtIndx + i] );
			strcat(OneLineBuf , txt);
			//p +=  strlen(txt);
		}	
		p = OneLineBuf + strlen(OneLineBuf);
		xsprintf(p ," \r\n" );
		Len = strlen(OneLineBuf);
	
		ParamIndex ++;
			
		return 1;
	}
	else
		return 0;
}

int GetNext_StringParam(){
Cfg_typ* cfg;
Scaled_Var_obj *var;

	if(Master[0].varIntLastNum == 0)
		return -1;

	if(!Master[0].pCfg)
		return -2;	

	cfg = Master[0].pCfg;	

	if(ParamIndex < Master[0].varStrLastNum){

		pMem = (UDINT)OneLineBuf;		
		Offset += Len;
		
		while((strlen(cfg->StringVar[ParamIndex].enumTXT) == 0) &&  ParamIndex < Master[0].varStrLastNum){

			ParamIndex++;
		}	

		
		xsprintf(OneLineBuf , "%d , %s, %s ,  Please  add ShortName , Please  add LongName  \r\n" , ParamIndex , cfg->StringVar[ParamIndex].enumTXT ,cfg->StringVar[ParamIndex].name);
		Len = strlen(OneLineBuf);

		ParamIndex ++;
			
		return 1;
	}
	else
		return 0;
}


////////////////////////////////////////////////////////////////////////////

