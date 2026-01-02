/***** Header files *****/

#include <..\lib\fileuser.h>

/* Prototypes */

DINT CreateNewFileName( FILEOUT_IF *out, USINT option );
DINT GetExtType( FILEIN_IF *in, FILEOUT_IF *out ); 
void ConvertToUpper( STRING s[35] );

/* LOCAL */
//
DINT FU_init( UDINT fm_adr, STRING sDevice[20], UDINT data_adr, UDINT size, UDINT MaxDirectoryFiles, USINT Type, UDINT _if_in_adr, UDINT _if_out_adr, STRING sDevice2[20], UDINT _fname_adr, UDINT _ftp_adr )
{
FILEMGR_obj *fm = ( FILEMGR_obj *) fm_adr;
FTPC_IF *ftp;

	/* Default: */
	fm->OpenFile 		= 0;
	fm->_state 			= 0;
	
	fm->_if_in_adr		= _if_in_adr;
	fm->_if_out_adr		= _if_out_adr;
	fm->_fname_adr		= _fname_adr;
	fm->_ftp_adr		= _ftp_adr;
	fm->Type			= Type;
	fm->BufferSize		= size;
	fm->MaxDirectoryFiles	= MaxDirectoryFiles;
	
	/* */
	strcpy( fm->Device, sDevice );

	/* FTP Only: Data Buffer */
	if ( fm->Type == FU_DRIVE_FTP ) {
		ftp = (FTPC_IF *) fm->_ftp_adr;
		ftp->pBuffer		= data_adr;
		ftp->iBufferSize	= fm->BufferSize;
	}

	/* Init: DirInfo */
	fm->_dirinfo.enable 	= 1;
	fm->_dirinfo.pDevice	= (UDINT) fm->Device;
	fm->_dirinfo.pPath 		= 0;
	
	/* Init: DirRead */
	fm->_dirread.enable 	= 1;
	fm->_dirread.pDevice	= (UDINT) fm->Device;
	fm->_dirread.pPath 		= 0;
	fm->_dirread.option 	= FILE_FILE;
	fm->_dirread.data_len 	= sizeof(fiDIR_READ_DATA);
	
	/* Init: FileOpen */
	fm->_fileopen.enable 	= 1;
	fm->_fileopen.pDevice 	= (UDINT) fm->Device;
	fm->_fileopen.mode 		= FILE_RW; 
		
	/* Init: FileRead */
	fm->_fileread.enable	= 1;
	fm->_fileread.offset 	= 0;
	fm->_fileread.pDest 	= data_adr;
	fm->_fileread.len 		= fm->BufferSize;
	
	/* Init: FileDelete */
	fm->_filedelete.enable 	= 1;
	fm->_filedelete.pDevice	= (UDINT) fm->Device;
	fm->_filedelete.pName	= 0;
	
	/* Init: FileCopy */
	fm->_filecopy.enable	= 1;
	fm->_filecopy.pSrcDev	= (UDINT) fm->Device;
	fm->_filecopy.option	= FILE_OW_DETMEM;
	fm->_filecopy.pDestDev	= (UDINT) sDevice2;
	
	/* Init: FileCreate */
	fm->_filecreate.enable 	= 1;
	fm->_filecreate.pDevice = (UDINT) fm->Device;

	/* Init: FileRename */
	fm->_filerename.enable 	= 1;
	fm->_filerename.pDevice 	= (UDINT) fm->Device;
	
	/* Init: FileWrite */
	fm->_filewrite.enable	= 1;
	fm->_filewrite.offset 	= 0;
	fm->_filewrite.pSrc 	= data_adr;
	fm->_filewrite.len 		= fm->BufferSize;

	/* Init: FileClose */
	fm->_fileclose.enable	= 1;
	fm->_fileclose.ident 	= 0;
	
	/* Issue Open Directory Command 
	out	= (FILEOUT_IF  *) fm->_if_out_adr;
	out->OpenDirectory	= 1;
	*/
   	return 0;
}

DINT FU_serv( UDINT fm_adr )
{
FILEMGR_obj *fm			= (FILEMGR_obj *) fm_adr;
FILEIN_IF *in 			= (FILEIN_IF   *) fm->_if_in_adr;
FILEOUT_IF *out			= (FILEOUT_IF  *) fm->_if_out_adr;
fiDIR_READ_DATA *fname;
UINT i,j,StartIndex;
BOOL FilterExt = 0, FileFound = 0;
FTPC_IF *ftp;

	/* FTP Only: Pointer */
	if ( fm->Type == FU_DRIVE_FTP )
		ftp = (FTPC_IF *) fm->_ftp_adr;

	/* 
	if ( strcmp( fm->Device, out->SelectedDirectory) != 0) {
		strcpy( fm->Device, out->SelectedDirectory );
		fm->_dirinfo.pDevice		= (UDINT) fm->Device;
		fm->_dirread.pDevice		= (UDINT) fm->Device;
		fm->_fileopen.pDevice 		= (UDINT) fm->Device;
		fm->_filedelete.pDevice	= (UDINT) fm->Device;
		fm->_filecopy.pSrcDev		= (UDINT) fm->Device;
		fm->_filecreate.pDevice 	= (UDINT) fm->Device;
	}
	*/
	
	/* Method: Open Directory */
	switch ( fm->_state )
	{
			
		/**** Return Directory Information ****/
		case 0: 
		
			if ( out->OpenDirectory ) {

				/* Pointer */
				out->PageIndex	= 0;
			
				/* FTP */
				if ( fm->Type == FU_DRIVE_FTP ) {
					
					/* Reset */
					ftp->Error = 0;
					
					/* Reset Status */
					fm->Status = 0;	
					
					/* Init File Name Base Pointer */
					fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
					
					/* Clear Array */
					for (i=0;i<fm->MaxDirectoryFiles;i++,fname++)
						memset( fname, 0, sizeof(fiDIR_READ_DATA) );
						
					if (ftp->LoggedIn) {
						ftp->reqFtpClientCmdList	= 1;
						ftp->Done					= 0;
						fm->_state					= 52;
						strcpy( fm->Message, "FTP: Reading...Please Wait");
					}
					else { 
						ftp->reqFtpClientConnect 	= 1;
						ftp->Done					= 0;	
						fm->_state 				 	= 50;
						strcpy( fm->Message, "FTP: Reading...Please Wait");
					}		
				}
				
				/* File IO */
				else {
					/* Reset Status */
					fm->Status = 0;	
				
					ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, Cmd.OpenDirectory");	
					/* Call FBK */
					DirInfo(&fm->_dirinfo);
					
					/* Get FBK output information */
					fm->_numfiles	= fm->_dirinfo.filenum;
					fm->_infostatus	= fm->_dirinfo.status;
					
					ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDirectory");
					
					/* Directory Info: Returned */
					if ( fm->_infostatus == 0 )	{
						fm->_fileptr	= 0;
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir1");
						/* File Name Limit */
						if ( fm->_numfiles > fm->MaxDirectoryFiles )
							fm->_numfiles = fm->MaxDirectoryFiles;
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir2");
						/* Init File Name Base Pointer */
						fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir3");
						/* Clear Array 
						for (i=0;i<fm->MaxDirectoryFiles;i++,fname++)
							memset( fname, 0, sizeof(fiDIR_READ_DATA) );*/
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir4");
						/* Return File Names: */	
						fm->_state++;
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir5");
						/* Message */
						strcpy( fm->Message, "Reading Directory...Please Wait");
						ERRxwarning(USER_ERROR_COB, 1 , "Type=FILEIO, OpenDir6");

					}
					
					/* Directory Info: Unsuccessful */
					else if ( fm->_infostatus != 65535) {
						strcpy( fm->Message, "Directory Info: Error!");
						if ( fm->_infostatus == 20799)
							fm->Error = FileIoGetSysError();
						fm->_state 			= 100;						
					}
				}	
			}

			#if 0
			else if ( out->Save ) {

				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );

				/* Initialize file open structrue */
				fm->_fileopen.pFile		= (UDINT) out->SelectedFile;
	
				/* Call FBK */
				FileOpen(&fm->_fileopen);
	
				/* Get FBK output information */
				fm->_fileopenstatus 		= fm->_fileopen.status;

				/* File Open: Success */		
				if (fm->_fileopenstatus == 0) {
					
					fm->_state	= 10;
				}
				
				/* File Open: Error */
				else if (fm->_fileopenstatus != 65535) {	
					fm->_state			= 100;
					if (fm->_fileopenstatus == 20799)
						fm->Error = FileIoGetSysError();
				}
			}
			#endif
			
			/* Method: Select Pattern: Process File */
			else if ( out->OpenFile | out->Save ) {
				
				/* Selected File has file name */
				if ( strcmp(out->SelectedFile, "") != 0) {
				
					/* Init File Name Base Pointer */
					fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
					
					/* To Upper Case */
					ConvertToUpper( out->SelectedFile );
					
					/* Cycle Array: Directory */
					for (i=0;i<fm->MaxDirectoryFiles;i++,fname++) {

						/* Located File Index: */
						if ( strcmp( out->SelectedFile, fname->Filename) == 0 ) {
							FileFound	= 1;
							break;
						}		
					}
				}
				
				/* Null Selected File: Error */
				else {
					strcpy( fm->Message, "FTP: Null File Name");
					out->OpenFile = 0;
				}	
			
				if ( FileFound ) {
			
					/* Reset Status */
					fm->Status 			= 0;
					
					/* File Size: Error */
					if (fname->Filelength >= fm->BufferSize) {
						strcpy( fm->Message, "File Open: Too Big");
						fm->_state			= 100;
					}
					
					/* File Size: Okay */
					else {
							
						/* Initialize file open structrue */
						fm->_fileopen.pFile		= (UDINT) fname->Filename;
			
						/* Call FBK */
						FileOpen(&fm->_fileopen);
			
						/* Get FBK output information */
						fm->_fileopenstatus = fm->_fileopen.status;
		
						/* File Open: Doesn't Exist */
						if (fm->_fileopenstatus == 20708) {
							strcpy( fm->Message, "File Open: Does Not Exist");
							if (out->Save) {
								out->Save 		= 0;
								out->SaveAs		= 1;
							}
							else {
								fm->_state			= 100;
							}	
						}
						
						/* File Open: Success */		
						else if (fm->_fileopenstatus == 0) {
							/* Get File Number */
							fm->OpenFileNum	= fm->_userfileptr + 1;
							/* Get Name */
							strcpy( fm->OpenFileName, fname->Filename  );
							fm->OpenFileDate	= fname->Date;
							strcpy( in->OpenFileName, fm->OpenFileName );
							GetExtType( in, out );	
							/* Get Size */
							fm->FileLength 		= fname->Filelength;
							in->OpenFileLength	= fname->Filelength;
							/* Reset Status */
							fm->Status 			= 2;
							strcpy( fm->Message, "File Open: Success");
							
							/* Next Step */
							if ( out->OpenFile ) {
								memset( (void *) fm->_fileread.pDest, 0, fm->BufferSize );	/* Clear Memory Location */
								fm->_state		= 2;	/* File Read */
							}	
							else if ( out->Save )
								fm->_state		= 3;	/* File Write */
						}
						
						/* File Open: Error */
						else if (fm->_fileopenstatus != 65535) {	
							strcpy( fm->Message, "File Open: Error");
							fm->_state			= 100;
							if (fm->_fileopenstatus == 20799)
								fm->Error = FileIoGetSysError();
						}
					}	
				}
				
				/* File Not Found: */
				else {
					strcpy( fm->Message, "File Not Found");
					out->OpenFile 	= 0;
					if (out->Save) {
						out->Save 		= 0;
						out->SaveAs		= 1;
					}
				}
			}
			
			/* Delete File */
			else if ( out->DeleteFile ) {
				
				/* Init File Name Base Pointer */
				fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
				
				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );
				
				/* Cycle Array: Directory */
				for (i=0;i<fm->MaxDirectoryFiles;i++,fname++) {

					/* Located File Index: */
					if ( strcmp( out->SelectedFile, fname->Filename) == 0 ) {
						FileFound	= 1;
						break;
					}		
				}
				
				/* File Located */
				if ( FileFound ) {							
			
					/* Initialize file Delete structrue */
					fm->_filedelete.pName 	= (UDINT) out->SelectedFile;
					
					/* Call FBK */
	            			FileDelete(&fm->_filedelete);
	            	
					/* Get FBK output information */
	            			fm->_infostatus	= fm->_filedelete.status;

					/* Directory Info: Returned */
					if ( fm->_infostatus == 0 ) {
						strcpy( fm->Message, "Delete File: Success");
						/* Reset Command */
						out->DeleteFile = 0;
						/* Issue Read Directory */
						out->OpenDirectory		= 1;	
					}	
						
					/* Directory Info: Unsuccessful */
					else if ( fm->_infostatus != 65535) {
						strcpy( fm->Message, "Delete File: Error!");
						/* Reset Command */
						out->DeleteFile = 0;
							
					}
				}
				/* File Not Found: Ignore error and Open Directory Again */
				else {
					out->DeleteFile 		= 0;
					out->OpenDirectory	= 1;	
				}
			}
			
			/* NEW: Use out->SelectedFile */
			/* Method: */
			else if ( out->CopyFile ) {
						
				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );	
					
				/* Initialize file Copy structrue */
				fm->_filecopy.pSrc		= (UDINT) out->SelectedFile;
				fm->_filecopy.pDest		= 0;		/* Same Name */
	
				/* Call FBK */
				FileCopy(&fm->_filecopy);
		
				/* Copy File: Doesn't Exist */
				if (fm->_filecopy.status == 20708) {
					strcpy( fm->Message, "Copy File: Doesn't Exist");
					out->CopyFile = 0;	
				}
				
				/* Copy File: Success */		
				else if (fm->_filecopy.status == 0) {
					strcpy( fm->Message, "Copy File: Success");
					out->CopyFile = 0;
				}
				
				/* Copy File: Error */
				else if (fm->_filecopy.status != 65535) {	
					strcpy( fm->Message, "Copy File: Error");
					out->CopyFile = 0;
				}
			}

			/* Method: */
			else if ( out->Rename ) {
						
				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );	
				ConvertToUpper( out->NewFileName );	
				
				/* Initialize file Rename structrue */
				fm->_filerename.pName		= (UDINT) out->SelectedFile;
				fm->_filerename.pNewName	= (UDINT) out->NewFileName;
	
				/* Call FBK */
				FileRename(&fm->_filerename);
		
				/* Rename File: Doesn't Exist */
				if (fm->_filerename.status == 20708) {
					strcpy( fm->Message, "Rename File: Doesn't Exist");
					out->Rename = 0;	
				}
				
				/* Rename File: Success */		
				else if (fm->_filerename.status == 0) {
					strcpy( fm->Message, "Rename File: Success");
					out->Rename 			= 0;
					out->OpenDirectory	= 1;	
				}
				
				/* Rename File: Error */
				else if (fm->_filerename.status != 65535) {	
					strcpy( fm->Message, "Rename File: Error");
					out->Rename = 0;
				}
			}
			
			/* Method */
			else if ( out->SaveAs ) {
			
				/* Reset Status */
				fm->Status 			= 0;

				/* Create with Extension */
				if ( CreateNewFileName(out, 0) == 0 ) {

					/* Init File Name Base Pointer */
					fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
				
					/* To Upper Case */
					ConvertToUpper( out->SelectedFile );

					/* Cycle Array: Directory */
					for (i=0;i<fm->MaxDirectoryFiles;i++,fname++) {

						/* Located File Index: */
						if ( strcmp( out->SelectedFile, fname->Filename) == 0 ) {
							FileFound	= 1;
							break;
						}		
					}

					/* Not Found: Create File as Planned */
					if ( !FileFound ) {

						/* Initialize file create structrue */
						fm->_filecreate.pFile	= (UDINT) out->SelectedFile;
							
						/* Call FBK */
						FileCreate(&fm->_filecreate);
						
						/* Get FBK output information */
						fm->_filecreatestatus = fm->_filecreate.status;
			
						/* No Need To Go to Error State! */
						/* File Already Exists: 20705 */
						if (fm->_filecreatestatus == fiERR_EXIST ) {
							strcpy( fm->Message, "File Exists: Updating..");
							out->SaveAs 	= 0;
							out->Save		= 1;
							fm->_state		= 0;
						}
						
						/* File Create: Success */		
						else if (fm->_filecreatestatus == 0) {
							strcpy( fm->Message, "File Create: Success");
							fm->Status		= 3;
							fm->_state		= 3;	/* File Create Success: Perform File Write */
						}
							 
						/* File Create: Error */
						else if (fm->_filecreatestatus != 65535) {	
							strcpy( fm->Message, "File Create: Error");
							fm->_state		= 100;
							if (fm->_filecreatestatus == fiERR_SYSTEM)
								fm->Error = FileIoGetSysError();
						}
					}
					/* File Located: */
					else {
						out->SaveAs 	= 0;
						out->Save	= 1;
					}
				}
				else {
					strcpy( fm->Message, "File Create: Bad Extension");
					fm->_state		= 100;
				}	
			}	
		
		break;
		
		/**** Return File Names ****/	
		case 1:
		
			/* Read File Names */
			if (fm->_fileptr < fm->_numfiles) {
				ERRxwarning(USER_ERROR_COB, 2 , "Type=FILEIO, ReturnFiles");
				/* Init: Dynamic Read Info */
				fm->_dirread.entry 		= fm->_fileptr;
				
				/* Init File Name Base Pointer: Then Index */
				fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
				fname 	+= fm->_fileptr;	
					
				/* Initialize Directory Read structrue */
				fm->_dirread.pData 		= (UDINT) fname->Filename;

				/* Call FBK */
				DirRead(&fm->_dirread);
				ERRxwarning(USER_ERROR_COB, 3 , "Type=FILEIO, DirRead");
				/* Update Read Status */
				fm->_readstatus		= fm->_dirread.status;
				
				/* Directory Read: Returned */
				if ( fm->_readstatus == 0) {
					ConvertToUpper( fname->Filename );
					fm->_fileptr++;
				}
	
				/* Directory Read: Unsuccessful */
				else if (fm->_readstatus != 65535)	{
					fm->_state 		= 100;
					strcpy( fm->Message, "Directory Read: Error");
					if ( fm->_readstatus == 20799)
						fm->Error = FileIoGetSysError();
				}
			}
			
			/* File Names Read */
			else {
				strcpy( fm->Message, "Directory Read: Completed");
				fm->_state			= 0;
				fm->Status 			= 1;
				out->OpenDirectory	= 0;
				FilterExt			= 1;
			}	
			
		break;
	
		/**** Read data from file ****/
		case 2: 
			
			/* Initialize file read structure */
			fm->_fileread.ident 	= fm->_fileopen.ident;

			/* Call FBK */
			FileRead(&fm->_fileread);

			/* Get status */
			fm->_filereadstatus = fm->_fileread.status;

			/* File Read: Success */
			if (fm->_filereadstatus == 0) {
				fm->_state 			= 4;
				out->OpenFile 		= 0;
				fm->_fileclose.ident = fm->_fileread.ident;
				strcpy( fm->Message, "File Read: Success");				
			}	
			/* File Read: Error */
			else if (fm->_filereadstatus != 65535)	{
				strcpy( fm->Message, "File Read: Error");
				out->OpenFile 		= 0;
				fm->_state 			= 101;
				fm->_fileclose.ident = fm->_fileread.ident;
				if (fm->_filereadstatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
		
		/**** File Write: Part 2 of out->SaveAs Method ****/
		case 3: 

			/* Initialize file write structure */
			if ( out->SaveAs )
				fm->_filewrite.ident 	= fm->_filecreate.ident;
			else if ( out->Save )
				fm->_filewrite.ident	= fm->_fileopen.ident;	
			
			/* Size */
			fm->_filewrite.len 			= out->FileSize;
			
			/* Clear Commands */
			out->SaveAs	= 0;
			out->Save   	= 0;
			
			/* Call FBK */
			FileWrite(&fm->_filewrite);

			/* Get status */
			fm->_filewritestatus = fm->_filewrite.status;

			/* File Write: Success */
			if (fm->_filewritestatus == 0) {
				fm->_state 	= 4;
				fm->_fileclose.ident = fm->_filewrite.ident;
				strcpy( fm->Message, "File Write: Success");
			}	
			/* File Write: Error */
			else if (fm->_filewritestatus != 65535)	{
				strcpy( fm->Message, "File Write: Error");
				fm->_state 	= 101;
				fm->_fileclose.ident = fm->_filewrite.ident;
				if (fm->_filewritestatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
				
		/**** FileClose: called after each fileRead or fileWrite ****/
		case 4:
		
			/*R or W completed*/ 
			fm->Status		= 4;

			/* Call FBK */
			FileClose(&fm->_fileclose);
			
			fm->_state 	= 0;

		break;

		/**** File Write: Part 2 of out->SaveAs Method ****/
		case 10: 

			fm->_filewrite.ident	= fm->_fileopen.ident;	
			
			/* Size */
			fm->_filewrite.len 		= out->FileSize;
			
			/* Clear Commands */
			out->Save   	= 0;
			
			/* Call FBK */
			FileWrite(&fm->_filewrite);

			/* Get status */
			fm->_filewritestatus = fm->_filewrite.status;

			/* File Write: Success */
			if (fm->_filewritestatus == 0) {
				fm->_state 	= 4;
				fm->_fileclose.ident = fm->_filewrite.ident;
				strcpy( fm->Message, "File Write: Success");
			}	
			/* File Write: Error */
			else if (fm->_filewritestatus != 65535)	{
				strcpy( fm->Message, "File Write: Error");
				fm->_state 	= 101;
				fm->_fileclose.ident = fm->_filewrite.ident;
				if (fm->_filewritestatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
		
		/* FTP */
		case 50:
			if ( (ftp->stateFtpClient == 2) && (ftp->LoggedIn) ) {
				strcpy( ftp->fileName, out->SelectedDirectory );
				ftp->reqFtpClientCd		= 1;
				ftp->Done				= 0;	
				fm->_state 				= 51;
			}
			else if (ftp->Error) {
				fm->_state 				= 0;
				out->OpenDirectory		= 0;
				strcpy( fm->Message, "FTP: Directory Read Error");
			} 	
		break;
		
		case 51:
			if ( (ftp->stateFtpClient == 2) && (ftp->Done) ) {
				ftp->reqFtpClientCmdList	= 1;
				ftp->Done					= 0;
				fm->_state 					= 52;
			}
			else if (ftp->Error) {
				fm->_state 					= 0;
				out->OpenDirectory			= 0;
				strcpy( fm->Message, "FTP: Directory Read Error");
			} 
		break;

		case 52:
			if ( (ftp->stateFtpClient == 2) && (ftp->Done) ) {
				fm->_numfiles 			= ftp->NumFiles;
				fm->_state				= 0;
				out->OpenDirectory	 	= 0;
				fm->Status 				= 1;
				/* Init File Name Base Pointer */
				fname		= (fiDIR_READ_DATA *) fm->_fname_adr;
					
				/* Directory Read: Returned */
				for (i=0;i<fm->_numfiles;i++,fname++) {
					if ( strlen(fname->Filename) < 35 ) 
						ConvertToUpper( fname->Filename );
				}
				strcpy( fm->Message, "FTP: Directory Read Completed");
				FilterExt				= 1;
			}
			else if (ftp->Error) {
				fm->_state 				= 0;
				out->OpenDirectory		= 0;
				strcpy( fm->Message, "FTP: Directory Read Error");
			} 
		break;
		
		/* FTP Open and Read */
		case 60:
			if ( (ftp->stateFtpClient == 2) && (ftp->Done) ) {
				
				/* Get Data: */
				strcpy( fm->OpenFileName, fname->Filename  );
				fm->OpenFileDate		= fname->Date;
				fm->FileLength 			= fname->Filelength;
				strcpy( in->OpenFileName, fm->OpenFileName );
				GetExtType( in, out );	
				
				/* Success: Status */
				fm->Status 				= 4;
				strcpy( fm->Message, "FTP Read: Success");
				fm->_state				= 0;
				out->OpenFile	 		= 0;
				
				/* Next Step */
				if ( out->OpenFile ) {
					memset( (void *) fm->_fileread.pDest, 0, fm->BufferSize );	/* Clear Memory Location */
					fm->_state		= 2;	/* File Read */
				}	
				else if ( out->Save )
					fm->_state		= 3;	/* File Write */
			}
			else if (ftp->Error) {
				strcpy( fm->Message, "FTP Read: Error");
				fm->_state				= 100;
				out->OpenFile			= 0;
			} 
		break;
		

				
		/**** Error State ****/
		case 100:
		
			/* Clear Commands */
			out->OpenDirectory 	= 0;
			out->OpenFile 		= 0;
			out->SaveAs 			= 0;
			out->Save			= 0;
			
			/* Error Status */
			fm->Status 			= -1;
		
			/* Command: Reset Error */
			fm->ResetError 	= 0;
			fm->_state		= 0;	
				
		break;
		
		/**** Error State w. FileClose ****/
		case 101:
		
			/* Call FBK */
			FileClose(&fm->_fileclose);
			fm->_state 	= 100;
			
		break;

		default:
		break;
		
	}

	/* Input Interface: Files in Directory */
	in->TotalDirectoryFileNum	= fm->_numfiles;
	/* Input Interface: Message */
	strcpy( in->Message, fm->Message );
	
	/* Process Page Scroll: Up */
	if ( out->PageUp ) {
		FilterExt 	= 1;
		if (in->PageNum > 1) 
			in->PageNum--;
		
	}
	/* Process Page Scroll: Down */
	if ( out->PageDown ) {
		FilterExt			= 1;
		if ( (in->PageNum < FU_MAX_PAGE_FILES) && (in->TotalPageFileNum == FU_MAX_PAGE_FILES) ) 
			in->PageNum++;
	}
	
	/* Perform Extension Filter */
	if ( FilterExt | out->Refresh ) {
		
		/* Reset */
		out->Refresh	= 0;
		
		/* Locate Total Files per Extension */
		j = 0;
		
		/* Init File Name Base Pointer */
		fname	= (fiDIR_READ_DATA *) fm->_fname_adr;

		/* Cycle Array */
		for (i=0;i<fm->_numfiles;i++,fname++) {
		
			/* All Extensions */
			if ( out->ExtensionIndex == 0 )	
				j++;
				
			/* Specific Extension */
			else if ( out->ExtensionIndex <= FU_MAX_EXT ) {
				if (FindString( fname->Filename, out->Extension[out->ExtensionIndex-1].Name) == 0 )
					j++;
			}		
		}

		/* Assign */
		in->TotalDirectoryFileNum	= j;
		in->TotalPages			= in->TotalDirectoryFileNum / FU_MAX_PAGE_FILES;
		if ( (in->TotalDirectoryFileNum % FU_MAX_PAGE_FILES) > 0 )
			in->TotalPages++;

		/* Locate Total Page Files per Extension */		
		j = 0;
		
		/* Page Scroll Index */
	    	StartIndex = (in->PageNum-1) * FU_MAX_PAGE_FILES;
	    
		/* Clear */
		for (i=0;i<FU_MAX_PAGE_FILES;i++) {
			memset( (UDINT *) &in->FileNames[i][0], 0, sizeof(in->FileNames[0]));
			in->FileDates[i] = 0;
			in->FileSizes[i] = 0;
		}
				
		/* Init File Name Base Pointer */
		fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
		
		/* Cycle Array */
		for (i=0;i<fm->_numfiles;i++,fname++) {		
			
			/* View All Extensions */
			if ( out->ExtensionIndex == 0 ) {
				if ( j >= StartIndex ) {
					strcpy( &in->FileNames[j-StartIndex][0], fname->Filename );
					in->FileDates[j-StartIndex]	= fname->Date;
					in->FileSizes[j-StartIndex]	= fname->Filelength;
				}	
				j++;
			}
			
			/* Locate File with Proper Extension */
			else if ( out->ExtensionIndex <= FU_MAX_EXT ) {
				if (FindString( fname->Filename, out->Extension[out->ExtensionIndex-1].Name) == 0 ) {
					if ( j >= StartIndex ) {
						strcpy( &in->FileNames[j-StartIndex][0],fname->Filename);
						in->FileDates[j-StartIndex]	= fname->Date;
						in->FileSizes[j-StartIndex]	= fname->Filelength;
					}	
					j++;
				}
			}
			
			/* Check Page Disply Limit */
			if ( j >= (StartIndex + FU_MAX_PAGE_FILES) ) {
				break;	
			}		
			
		}
		
		/* Files in Page View */
		in->TotalPageFileNum 		= j - StartIndex;
	}
	
	/* Method: Clear Status */
	if ( out->ClearStatus )  {
		out->ClearStatus 	= 0;
		fm->Status			= 0;
	}

	/* New: Process Page Scroll: Up */
	if ( out->PageUp ) {
		out->PageUp = 0;
		/* New */
		out->PageIndex	= FU_MAX_PAGE_FILES -1;
		strcpy( out->SelectedFile, in->FileNames[out->PageIndex] );
	}
	/* New: Process Page Scroll: Down */
	if ( out->PageDown ) {
		out->PageDown 	= 0;
		/* New */
		out->PageIndex	= 0;
		strcpy( out->SelectedFile, in->FileNames[out->PageIndex] );
	}
	
	/* Status */
	in->Status				= fm->Status;	
	
	return 0;
}

DINT FU_AddExt( UDINT fm_adr, STRING s[35], STRING ext[4] )
{
FILEMGR_obj *fm	= (FILEMGR_obj *) fm_adr;
FILEOUT_IF *out	= (FILEOUT_IF  *) fm->_if_out_adr;
STRING _s[35];
USINT i;

	/* Source String: Local Copy */
	strcpy( _s, s );

	/* Locate '.' */
	for (i=0;i<strlen(_s);i++) {
		/* Found Extension */
		if ( _s[i] == '.' ) {
			memcpy( &_s[i+1], ext, 3); 
			/* Add Null */
			_s[i+4] = 0;
			/* To Upper */
			ConvertToUpper( _s );
			/* Copy */
			strcpy( out->SelectedFile, _s );	
			/* Return */
			return (i);
		}
	}
	
	return 0;
} 

void ConvertToUpper( STRING s[35] )
{
UINT i;

	/* Verify Letters Only */
	for (i=0;(i<strlen(s)) && (i<35);i++) {
		if ( ((s[i] >= 'A') && (s[i] <= 'Z')) || 
			 ((s[i] >= 'a') && (s[i] <= 'z'))    ) {
			s[i] = s[i] & 0xDF;
		}
	}	
}

/* */
DINT FindString( STRING s[35], STRING ext[4] ) 
{
USINT i, j, s_ext[5];

	/* Locate '.' */
	for (i=0;i<strlen(s);i++) {
		if ( s[i] == '.' ) {
			/* Convert to Next 3 Chars to Upper */
			for (j=1;j<4;j++)
				s[i+j] = s[i+j] & 0xDF;
			memcpy( s_ext,&s[i], 4);
			/* Add Null */
			s_ext[4] = 0;
			return strcmp(s_ext, ext);
			break;
		}
	}
	
	/* Not Found */
	return 255;
}
//

BOOL FU_DeleteExt( STRING s[35], STRING d[35]) 
{
USINT i;

	/* Locate '.' */
	for (i=0;i<strlen(s);i++) {
		d[i]	= s[i];
		if ( s[i] == '.' ) {
			d[i]	= 0;
			return 1;
		}
	}
	
	/* Not Found */
	return 0;
}
/* */
DINT GetExtType( FILEIN_IF *in, FILEOUT_IF *out ) 
{
USINT i,j, s_ext[5];

	/* Locate '.' */
	for (i=0;i<strlen(in->OpenFileName);i++) {
		if ( in->OpenFileName[i] == '.' ) {
			memcpy( s_ext,&in->OpenFileName[i], 4);
			/* Add Null */
			s_ext[4] = 0;
			for (j=0;j<5;j++) {
				if (strcmp(s_ext, out->Extension[j].Name) == 0) {
					in->ExtensionIndex = j;
					return 0;
				}	
			}	
		}
	}
	
	/* */
	return -1;
}
/*********************************************************
* NAME: 			MakeName
*
* OBJECT: 		FILEOUT_IF
*
* FUNCTION: 		Create string with extension 
*          
*
* PARAMETERS: 	.ExtensionIndex
				.NewFileNameBase

				Destination option: 	0	= SelectedFile
									1	= NewFileName
*
* RETURN: 		        
*
* HISTORY: 		11.10.05 		- 
* 	  			
*             
*********************************************************/
//
DINT MakeName( FILEOUT_IF *out, USINT option )
{
	return CreateNewFileName( out, option );
}
DINT CreateNewFileName( FILEOUT_IF *out, USINT option )
{	
STRING base[34];

	if ( (FU_MAX_EXT + 1) > out->ExtensionIndex > 0 ) {
		/* Process */	
		memset( base, 0, sizeof(base));
		strcpy( base, out->NewFileNameBase );
		strcat( base, out->Extension[out->ExtensionIndex-1].Name );
		if ( option == 0 )
			strcpy( out->SelectedFile, base);
		else if (option == 1)
			strcpy( out->NewFileName, base );
		return 0;
	}
	else
		return 1;		
}

		
