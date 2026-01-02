#include <bur\plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#ifndef _FILEUSER_
#define _FILEUSER_	

	#define fuDRIVE_FTP 1U
	#define fuDRIVE_FILEIO 0U
	#define fuFILE_LOAD_OK 4

	/*User Private Library Error Numbers (value assigned as suggested in the B&R help file)*/
	#define USER_ERROR_MIN 0xC350
	#define USER_ERROR_MAX 0xEA5F
	#define USER_ERROR_COB	USER_ERROR_MIN

	/* Datatypes and datatypes of function blocks */
	typedef struct FILE_EXT
	{	plcstring Name[5];
	} FILE_EXT;

	typedef struct FILEOUT_IF
	{	plcbit OpenDirectory;
		unsigned short OpenFile;
		plcbit CopyFile;
		plcbit DeleteFile;
		plcbit SaveAs;
		plcbit Save;
		plcbit Rename;
		plcbit PageUp;
		plcbit PageDown;
		plcbit Refresh;
		plcbit ClearStatus;
		unsigned short PageIndex;
		plcstring SelectedFile[35];
		plcstring SelectedDirectory[35];
		plcstring NewFileNameBase[31];
		plcstring NewFileName[35];
		unsigned long FileSize;
		unsigned char ExtensionIndex;
		struct FILE_EXT Extension[5];
		plcstring pDestDev[21];
	} FILEOUT_IF;

	typedef struct FILEIN_IF
	{	unsigned short TotalPageFileNum;
		unsigned short TotalDirectoryFileNum;
		unsigned short TotalPages;
		plcstring FileNames[20][35];
		plcdt FileDates[20];
		unsigned long FileSizes[20];
		plcstring OpenFileName[35];
		unsigned long OpenFileLength;
		unsigned char PageNum;
		unsigned char ExtensionIndex;
		signed char Status;
		plcstring Message[50];
	} FILEIN_IF;

#define	FILEUSERTYPE 1

#define FU_MAX_PAGE_FILES	20
#define FU_MAX_EXT			5
#define FU_DRIVE_FILEIO		0
#define FU_DRIVE_FTP		1 
	 
	typedef struct {
		/* Public */
		STRING				Message[50];
		USINT				Error;
		STRING				Device[20];
		SINT				Status;
		UDINT				FileLength;
		STRING				OpenFileName[35];
		USINT				OpenFileNum;
		DATE_AND_TIME		OpenFileDate;
		UINT				Type;
		UDINT				BufferSize;
		UDINT				MaxDirectoryFiles;
		
		/* Private */
		UDINT				_if_in_adr;
		UDINT				_if_out_adr;
		UDINT				_fname_adr;
		UDINT				_ftp_adr;
		UINT				_state;
		UINT				_error;
		UDINT				_fileptr;
		UDINT				_numfiles;
		UINT				_infostatus;
		UINT				_readstatus;
		UINT				_fileopenstatus;
		UINT				_filereadstatus;
		UINT				_filecreatestatus;
		UINT				_filewritestatus;
		UINT				_userfileptr;
		DirInfo_typ			_dirinfo;
		DirRead_typ 		_dirread;
		FileOpen_typ 		_fileopen;
		FileRead_typ		_fileread;
		FileDelete_typ 		_filedelete;
		FileCopy_typ		_filecopy;
		FileCreate_typ		_filecreate;
		FileWrite_typ		_filewrite;
		FileClose_typ		_fileclose;
		FileRename_typ		_filerename;
		
		/* Methods */
		BOOL 				OpenDirectory;
		USINT 				OpenFile;
		BOOL				DeleteFile;
		BOOL 				ResetError;

	} FILEMGR_obj;

	typedef struct {
		BOOL LoggedIn;					/* User Logged In */
		BOOL Done;						/* Command Processed */
		BOOL Error;						/* Command Error */
		DINT reqFtpClientConnect; 		/*request flag: connect FTP client to server*/
		DINT reqFtpClientDisconnect;	/*request flag: disconnect FTP client from server*/
		DINT reqFtpClientGetFile;		/*request flag: get file from server*/
		DINT reqFtpClientPutFile;		/*request flag: put file to server (TODO!)*/
		DINT reqFtpClientCmdList;		/*request flag: list current directory (i.e. "dir") command*/
		DINT reqFtpClientPwd;			/*request flag: print working directory*/
		DINT reqFtpClientCd;			/*request flag: change working directory (i.e. "cd") command)*/
		
		DINT stateFtpClient;			/*monitor only: FTP client user state*/
		DINT NumFiles;
		
		UDINT pBuffer;					/* File Buffer 		*/
		DINT  iBufferSize;				/* File Buffer Size */
		USINT fileName[128];
	} FTPC_IF;


	/* Prototyping of functions and function blocks */
	signed long FU_AddExt(unsigned long fm_adr, plcstring* s, plcstring* ext);
	signed long FindString(plcstring* s, plcstring* ext);
	signed long FU_init(unsigned long fm_adr, plcstring* sDevice, unsigned long data_adr, unsigned long size, unsigned long MaxDirectoryFiles, unsigned char Type, unsigned long _if_in_adr, unsigned long _if_out_adr, plcstring* sDevice2, unsigned long _fname_adr, unsigned long _ftp_adr);
	signed long FU_serv(unsigned long fm_adr);
	signed long MakeName(FILEOUT_IF* out, unsigned char option);
	plcbit FU_DeleteExt(plcstring* s, plcstring* d);


	__asm__(".section \".plc\"");

	__asm__(".previous");


#endif /* _FILEUSER_ */
