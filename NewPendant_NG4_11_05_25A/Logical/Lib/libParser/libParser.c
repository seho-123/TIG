#ifndef _LIBPARSER_C
#define _LIBPARSER_C

/*
static plcstring WarnTxt[128];

#define DBG_CK(format, args...) bgRingBuffPrintf(&gRingBuffer, format, ## args); //Telnet
//#define DBG_CK(format, args...)  xsprintf(WarnTxt , format, ## args);   ERRxwarning(0xC350, 0, WarnTxt); // Logger in AS
#define DBG_LogMgr(format, args...)  xsprintf(WarnTxt , format, ## args);   LogTxt(&LogMgr, WarnTxt); // LogBook on Pendant

//#define DBG_CK(format, args...) ;
#define DBG_LogMgr(format, args...)  ;
*/


DINT KeyWord_init( CFGKEY *kw0 )
{
CFGKEY *kw;

	kw = kw0 + kwINDX_IDX;
	strcpy( kw->sKey, kwINDX);	
	
	kw = kw0 + kwSERIALNUM_IDX;
	strcpy( kw->sKey, kwSERIALNUM);

	kw = kw0 + kwARCLINK_IP_IDX;
	strcpy( kw->sKey, kwARCLINK_IP);

	/// TRAVEL 
	kw = kw0 + kwTVL_DIR_IDX;
	strcpy( kw->sKey, kwTVL_DIR);

	kw = kw0 + kwTVL_ENC_TICKS_IDX;
	strcpy( kw->sKey, kwTVL_ENC_TICKS);

	kw = kw0 + kwTVL_UNIT_DIST_IDX;
	strcpy( kw->sKey, kwTVL_UNIT_DIST);

	kw = kw0 + kwTVL_BTN_JOG_GAIN_IDX;
	strcpy( kw->sKey, kwTVL_BTN_JOG_GAIN);

	kw = kw0 + kwTVL_JOG_MODE_IDX;
	strcpy( kw->sKey, kwTVL_JOG_MODE);	

	/// TIG 
	kw = kw0 + kwTIG_MODE_IDX;
	strcpy( kw->sKey, kwTIG_MODE);
	
	return 0;
}

DINT ParseMachCfgFile( MACHCFG_ParserMgr *mgr, UDINT FileSize )
{
STRING sBlock[MAX_BLOCK_LENGTH];
UINT i, start=0;
USINT *p = (USINT*) mgr->f;
UINT BlockLength;
BOOL flag;
BOOL _flag;


	/* Check Size */
	if ( FileSize >= MAX_MACHCFG_FILE_SIZE) { /*MAX_MACHCFG_FILE_SIZE*/
		mgr->Status = -99;		/* File Size too Big */
		return (mgr->Status);
	}

	/* Diag: Clear Statistics */
	mgr->Blocks 	= 0;
	mgr->Errors 	= 0;
	mgr->ErrorItem 	= 0;
	mgr->Ignored	= 0;

	mgr->curLine = 0;

	flag = 0;
	_flag = 0;

	DBG_CK("File Len: %d Ponter: %d  Char1: %d Char2: %d Char3: %d\r\n" , FileSize , mgr->f , (UDINT)(p[0]), (UDINT)(p[1]), (UDINT)(p[2]));
	
	/* Cycle through File: */	
	for (i=0;i<FileSize;i++,p++) {

		if(*p == 0)
			break;
	
		/* Found white characters */				
		flag = ((*p == 10) ||  (*p == 11) || (*p == 12) ||  (*p == 13) ||  (*p == 14));

		if(!flag && _flag) // Begining of line
			start = i;	

		
		if(flag && !_flag){ // end of line

			mgr->curLine ++;			
			BlockLength	= i - start;	

			DBG_CK("Line No: %d Strt: %d Len: %d\r\n" , mgr->curLine , start , BlockLength   );
			
			if ( BlockLength > MAX_BLOCK_LENGTH ) {
				mgr->Status = -100;

				DBG_CK("\r\nBlock Too Long !!!\r\n");
				ERRxwarning(0xC350, 0, "MachCfgFile: String Too Big");
				ERRxwarning(0xC350, (i+1), "MachCfgFile: Line #");
				return mgr->Status;
			}
			else if(BlockLength > 0){				
				memcpy(&sBlock[0], ((USINT *) mgr->f+(start)), 	BlockLength);
				sBlock[BlockLength] = 0;
				//DBG_CK("\r\nBlock: %s start: %d Len: %d\r\n" , &sBlock[0] , start ,  BlockLength);

				
				/* Error Processing */ 				
				if(ProcessMachCfgBlock( mgr, &sBlock[0] ) < 0 ) {
					//ERRxwarning(0xC350, mgr->ErrorItem, "MachCfgFile:Block Error");
					ERRxwarning(0xC350, mgr->curLine, "MachCfgFile:Error in Line");
					//ERRxwarning(0xC350, mgr->Blocks, "MachCfgFile: Processed");
					//ERRxwarning(0xC350, mgr->Ignored, "MachCfgFile: Ignored");
					//ERRxwarning(0xC350, mgr->Status, "MachCfgFile: Status");
					//return (mgr->Status);
				}				
					
			}
		}
		_flag = flag;
	}

	
	/* Set Return Values: */
	mgr->Status = 1;	
	ERRxwarning(0xC350, mgr->ErrorItem, "MachCfgFile:Block Error");	
	ERRxwarning(0xC350, mgr->Blocks, "MachCfgFile: Processed");
	ERRxwarning(0xC350, mgr->Ignored, "MachCfgFile: Ignored");
	ERRxwarning(0xC350, mgr->Status, "MachCfgFile: Status");
	ERRxwarning(0xC350, mgr->Blocks, "MachCfgFile: Lines Processed");
	DBG_CK("\r\nTotal Blocks: %d\r\n ******* \r\n" ,  mgr->Blocks);
	return (mgr->Blocks);
}

DINT ProcessMachCfgBlock( MACHCFG_ParserMgr *mgr, char *sBlock )
{
ITEM_PARAM ipDest;

	/* Extract: Item and Param */
	if (FindItemParam(sBlock, &ipDest)) {		

		/* Locate: Keyword Match */		
		if(FindKeyWord(mgr, &ipDest)) {
			/* Diag: */
			mgr->Status = 1;
			mgr->Blocks++;

			DBG_CK("\r\nItem:%s Param:%s Key: %d\n\r " , ipDest.sItem , ipDest.sParam , (UDINT)ipDest.Key);

		
			/* Primary Keywords: Range 0-9 */
			if ((ipDest.Key > 0) && (ipDest.Key < 10) ) {

				switch (ipDest.Key) {

					/* ACP: AXIS Keyword */
					case kwINDX_IDX:
						/* Get Index Number: Verify */
						if (GetIndx( &ipDest, mgr ) != 0)
							mgr->Status = -1;
					break;

					default:
						
						if(SetGenCfg(&ipDest, mgr ) != 0){
							mgr->Status = -1;
						}
						
					break;
				}
			}
			/*  Parameters TVL: Range 10 -19  */			
			else if ( (ipDest.Key > 0) && ipDest.Key < 20) {
				
				if(SetTVLCfg(&ipDest, mgr ) != 0){
					mgr->Status = -2;
				}
					
			}	

			/*  Parameters TVL: Range 10 -19  */			
			else if ( (ipDest.Key > 0) && ipDest.Key < 30) {
				
				if(SetTIGCfg(&ipDest, mgr ) != 0){
					mgr->Status = -3;
				}
					
			}		
			
	
		}
		/* Keyword: Mismatch */
		else {
			/* Diag: */
			mgr->Status = -255;
			mgr->Errors++;			
			if ( !mgr->ErrorItem )
				mgr->ErrorItem = mgr->Ignored + mgr->Blocks + 1;
		}

	}
	/* Item Param: Ignored */
	else {
		mgr->Status = 0;
		mgr->Ignored++;
	}

	return mgr->Status;
}

DINT GetIndx( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr )
{
	if (!mgr)
		return -255;

	mgr->_idx = (USINT) atoi( (UDINT) ip->sParam);
	DBG_CK("Index: %d \r\n" , mgr->_idx );
	
	/* Out of Range */
	if ( (mgr->_idx > 20) )
		return -1;
	else {
		return 0;
	}
}

UINT FindItemParam( USINT *s, ITEM_PARAM *ip )
{
UINT i,start=0;
USINT *s1;
char p[MAX_BLOCK_LENGTH];


	s1 = s;

	/* Clear */
	memset( ip, 0, sizeof(ITEM_PARAM));

	/* Ignore: '*', Null, LF, CR or Space */
	//if ( (s1[0] == '*') || (s1[0] == 0) || (s1[0] == 14) || (s1[0] == 12) || (s1[0] == ' '))
	if ( (s1[0] == '*') || (s1[0] == 0) || (s1[0] == 14) || (s1[0] == 12))
		return 0;

	/* Locate '=' */
	for (i=0;i<MAX_BLOCK_LENGTH;i++,s1++) {
		if ( *s1 == '=' ) {
			*s1 = 0;
			
			strcpy( p , s );
			strcpy( ip->sItem, BothTrim(p));
			/*ERRxwarning(0xC340, s, ip->sItem);*/
			*s1 = '=';
			start = i + 1;
			break;
		}
	}

	/* Good Block */
	if (start) {
		/* Locate Null */
		for (i=0;i<MAX_BLOCK_LENGTH-start;i++,s1++) {
			if ( *s1 == 0 ) {
				memcpy( ip->sParam, s+start, i-1 );
				/*ERRxwarning(0xC360, s+start, ip->sParam);*/
				memset( s, 0, MAX_BLOCK_LENGTH );
				return start;
			}
		}
	}

	/* '=' Not Found */
	return 0;
}

DINT FindKeyWord( MACHCFG_ParserMgr *mgr, ITEM_PARAM *ip )
{
UINT i;
CFGKEY *kw = (CFGKEY*) mgr->_keyword_adr;

	
	//DBG_CK("Cons. Item:%s \n\r " , ip->sItem );
	
	/* Cycle: Keyword Names and Compare */
	for (i=0;i<MAX_KEYWORDS;i++,kw++) {
		
		//DBG_CK("Item:%s Skey: %s \n\r " , ip->sItem , kw->sKey );
		if (strcmp(kw->sKey,ip->sItem) == 0) {
			ip->Key = i;
			/*ERRxwarning(0xC350, i, ip->sItem);*/
			/* Key = 0 is valid: Add 1 to return */
			//DBG_CK("key: %d \n\r " ,ip->Key);
			return i+1;
		}
	}

	ip->Key = 0;
	ERRxwarning(0xC350, 0, "Keyword Not Found");
	ERRxwarning(0xC350+i, i, ip->sItem);	
	return 1;
}

DINT SetGenCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr ){
MASTER_typ *m;
Cfg_typ  *cfg;

	if(!mgr)
		return -1;

	
	m = (MASTER_typ*)mgr->pMaster;

	if(!m)
		return -2;

	if(!m->pCfg)
		return -3;
	
	cfg = m->pCfg;
		
	switch (ip->Key) {
		
		case kwSERIALNUM_IDX:
			
			DBG_CK("SN: %s  \r\n" , ip->sParam);
		break;	

		case kwARCLINK_IP_IDX:
			
			DBG_CK("ArcLinkIP: %s  \r\n" , ip->sParam);
			if(m->Objects.pArcLink_IP_Address)
				strcpy(m->Objects.pArcLink_IP_Address , ip->sParam);
		break;

		default:
			
			DBG_CK("Unknown GEN Key: %d  \r\n" , ip->Key);
			
		break;

	}
	return 0;

}

DINT SetTVLCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr ){
MASTER_typ *m;
Cfg_typ  *cfg;

	return;

	if(!mgr)
		return -1;

	
	m = (MASTER_typ*)mgr->pMaster;

	if(!m)
		return -2;

	if(!m->pCfg)
		return -3;
	
	cfg = m->pCfg;
	
	switch (ip->Key) {
		
		case kwTVL_DIR_IDX:
			cfg->HeadCfg.TVL.Enc_Dir = (DINT) atoi( (DINT) ip->sParam );
			DBG_CK("kwTVL_DIR_IDX: %s \r\n" , ip->sParam);			
		break;

		case kwTVL_ENC_TICKS_IDX:
			cfg->HeadCfg.TVL.EncPerTick = (DINT) atoi( (DINT) ip->sParam );
			DBG_CK("kwTVL_ENC_TICKS_IDX: %s  \r\n" , ip->sParam);
		break;

		case kwTVL_UNIT_DIST_IDX:
			cfg->HeadCfg.TVL.DistPerTick = (float) atof( (DINT) ip->sParam );
			DBG_CK("kwTVL_UNIT_DIST_IDX: %s  \r\n" , ip->sParam);
		break;

		case kwTVL_BTN_JOG_GAIN_IDX:
			cfg->HeadCfg.TVL.BtnJog_Gain = (float) atof( (DINT) ip->sParam );
			DBG_CK("kwTVL_BTN_JOG_GAIN_IDX: %s  \r\n" , ip->sParam);
		break;

		case kwTVL_JOG_MODE_IDX:
			cfg->HeadCfg.TVL.BtnJog_Mode = (UINT) atoi( (DINT) ip->sParam );
			DBG_CK("kwTVL_JOG_MODE_IDX: %s  \r\n" , ip->sParam);
		break;

		default:

			DBG_CK("Unknown TVL Key: %d  \r\n" , ip->Key);

		break;

	}


	return 0;
}

DINT SetTIGCfg( ITEM_PARAM *ip, MACHCFG_ParserMgr *mgr ){
MASTER_typ *m;
Cfg_typ  *cfg;
PS_Param_typ *ps;


	if(!mgr)
		return -1;

	
	m = (MASTER_typ*)mgr->pMaster;

	if(!m)
		return -2;

	if(!m->pCfg)
		return -3;

	if(mgr->_idx >= TIG_PS_NUM)
		return -4 ;
	
	cfg = m->pCfg;
	
	switch (ip->Key) {
		
		case kwTIG_MODE_IDX:
		
			if(m->Objects.pPS_Param_Set[mgr->_idx]){
				ps = (PS_Param_typ*)m->Objects.pPS_Param_Set[mgr->_idx];
				ps->Mode = (USINT) atoi((DINT)ip->sParam );			
			}
			DBG_CK("kwTIG_MODE: %s \r\n" , ip->sParam);	
	
		break;


		default:

			DBG_CK("Unknown TIG Key: %d  \r\n" , ip->Key);

		break;

	}
	
	return 0;

}	


UDINT BothTrim(char* s){
UDINT len , i;
char* p;

	p = s;
	len = strlen(s);	

	// Find frist non space character
	for(i = 0 ; i < len ; i++){

		if(*(s+i) != ' '){
			p = s+i;
			len -= i;
			break;
		}			

	}

	for(i = 0 ; i < len ; i++){
		if(*(p +i) == ' '){

			*(p+i) = 0;
			return (UDINT)p;

		}
	}
	return (UDINT)p;
}

#endif
