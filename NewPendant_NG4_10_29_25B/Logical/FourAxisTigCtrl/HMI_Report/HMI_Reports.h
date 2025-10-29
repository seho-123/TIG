#include <math.h>



#include <..\GlobDef.h>
#include <..\..\Lib\WSI_Lib.c>

#include <..\GlobDecl.h>
#include <..\Hardcoded.c>
#include <..\HMI_Cntrl.c>
#include <..\HeadConfig.c>


#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\LCDLib.c> 
#include <..\..\Lib\LoggerLib.c>

#ifndef _HMI_REPORTS_
#define _HMI_REPORTS_


/*********************************************************************/
typedef struct FILEIO2_RW_HS_INT_typ
{
	UINT 		   step;
	USINT          byErrorLevel;
	UINT           wStatus;
	UINT           wError;
	UDINT          dwIdent;
	UDINT 		   memlng;	
	
	FileOpen_typ   FOpen;
	FileClose_typ  FClose;
	FileCreate_typ FCreate;
	FileRead_typ   FRead;
	FileWrite_typ  FWrite;
	FileDelete_typ FDelete;
	DirInfo_typ	   FDirInfo;
	DirRead_typ	   FFileInfo;

	FileRename_typ FRename;
	FileCopy_typ   FCopy;

	//UDINT OffsetToWeldProgramLinkMode;
	//UDINT OffsetToWeldProgramRepeat;	

	//UINT  PathDoneCnt;
	
		

}FILEIO2_RW_HS_INT_typ;

typedef struct FILE2_RW_typ
{

	USINT CMD;
	plcbit Done;
	plcbit Error;
	plcbit FileLocked;
	plcbit NewFileLoaded;
	USINT Mode;


	plcstring fileName[64];
	plcstring fileNewName[64];
	plcstring fileExten[6];
	
	plcstring deviceName[32];
	plcstring targetDeviceName[32];

	UINT TargetDevice_Indx;
	UINT SourceDevice_Indx;

	UINT DirList_Page[2];
	

	plcstring MsgText[128];
	UDINT FilesNum[2];
	UINT DirList_Page_Num[2];
	

	fiDIR_READ_DATA  FileInfo[1];

	TON_typ Tmr;
	

	UDINT pMem;
	UDINT Len;
	UDINT Offset;


	FILEIO2_RW_HS_INT_typ _int;


}FILE2_RW_typ;


/***********************************************/
typedef struct Weld_Report_Line_typ
{
	plcstring WelderStamp[16];
	DATE_AND_TIME TimeStamp;
	DATE_AND_TIME TimeStampStart;
	plcstring ToolHeadID[8];
	UINT BeadNo;
	float PrimaryAmps;
	float PrimaryWireSpeed;
	float PrimaryVolts;
	plcstring  Dwell_Excursion[16];
	float Travel_Tungsten;
	float PowerRatio;
	float HeatInput;
	plcstring InetrpassTemp[8];
	plcstring  Comments[32];	

}Weld_Report_Line_typ;

typedef struct Weld_Report_Head_typ
{
plcstring WELD_LOG[16];
plcstring WELD_No[16];
plcstring WIRE_HEAT_No[16];
plcstring MACHINE_Id[16];
UINT LAYER_No;
plcstring WPS[16];
plcstring M_TE[16];
plcstring Cal_Due[16];	
float WireDim;
USINT TempEvery;

}Weld_Report_Head_typ;


typedef struct Weld_Report_typ
{
plcstring FileName[64];
UINT LINES_No;
UINT LINES_Offset;
UINT ActLINE;
UINT BEADS_Cnt;
UINT Report_Type;
Weld_Report_Head_typ Head;	
Weld_Report_Line_typ Line[16];


}Weld_Report_typ;

/*
typedef struct Weld_Report_Line_typ
{
	plcstring delimit_begin[4];
	plcstring WelderStamp[16];
	plcstring delimit_1[4];
	plcstring TimeStamp[16];
	plcstring delimit_2[4];
	plcstring BeadNo[8];
	plcstring delimit_3[4];
	plcstring PrimaryAmps[8];
	plcstring PrimaryWireSpeed[8];
	plcstring PrimaryVolts[8];
	plcstring Dwell_Excursion[16];
	plcstring Travel_Tungsten;
	float PowerRatio;
	float InetrpassTemp;
	plcstring  Comments[32];	

}Weld_Report_Line_typ;

*/




/***********************************************/

//#define PRG_FILE_EXT ".rec"
//#define PRG_NO_NEXT "None"

/*

#define F_READ 				1
#define F_WRITE 			2
#define F_DELETE			3
#define F_FREE_MEM 			4

#define F_RENAME			8
#define F_COPY				9

#define F_READ_DIR 			20
#define F_READ_FILE_INF 	21
#define F_READ_DIR_FILELIST 22

#define F_OPEN_ONLY 		30
#define F_CREATE_ONLY 		31
#define F_WRITE_ONLY 		32
#define F_CLOSE_ONLY 		33
#define F_READ_ONLY 		34

*/


#define CF_DEV_INDX 0
#define	USB_DEV_INDX 1

#define VISIBLE 	0
#define INVISIBLE 	1
/******************************************************/
_GLOBAL_RETAIN Weld_Report_typ Weld_Rep;

_GLOBAL INT PleaseShowFileMessage;

//#define MAX_XML_FILE_SIZE 10000
#define MAX_FILES_IN_DIR		20
//#define DIR_FILELIST_MAX_SIZE 1024

_LOCAL FILE2_RW_typ  *pFileRW ;

_LOCAL FILE2_RW_typ File_RW2;

_LOCAL UDINT pRec;
_LOCAL UDINT recLen;

_LOCAL plcstring _fileName[64];
//_LOCAL USINT load_after_save;
_LOCAL UDINT pos; 
_LOCAL UINT indx;


USINT _CMD ;

char tempStr[64];





//////////////////
void _Open(FILE2_RW_typ *f);
void _Close(FILE2_RW_typ *f);
void _Create(FILE2_RW_typ *f);
void _Read(FILE2_RW_typ *f);
void _Write(FILE2_RW_typ *f);
void _Delete(FILE2_RW_typ *f);
void _ReadDir(FILE2_RW_typ *f);
void _ReadFileInfo(FILE2_RW_typ *f);
void _Rename(FILE2_RW_typ *f);
void _Copy(FILE2_RW_typ *f);

//void DeleteExtension(char *p);

#endif

