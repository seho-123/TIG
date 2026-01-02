(********************************************************************
 * COPYRIGHT (C) BERNECKER + RAINER, AUSTRIA, A-5142 EGGELSBERG
 ********************************************************************
 * Library: AsRfbExt
 * File: AsRfbExt.typ
 * Created: 10.04.2008
 ********************************************************************
 * Global data types of library AsRfbExt
 ********************************************************************)
TYPE 
	AsRfbExtType : STRUCT
		pClient : UDINT;
		pHeader : UDINT;
		uiTimeOut : UDINT;
		uiTimeOutValue : UDINT;
		status : UINT;
	END_STRUCT;

	AsRfbExt_typ : STRUCT
		pClient : UDINT;
		pHeader : UDINT;
		uiTimeOut : UDINT;
		uiTimeOutValue : UDINT;
		status : UINT;
	END_STRUCT;
END_TYPE