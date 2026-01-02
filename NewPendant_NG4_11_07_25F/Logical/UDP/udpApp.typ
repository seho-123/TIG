(********************************************************************
 * COPYRIGHT -- HP
 ********************************************************************
 * File: Global.typ
 * Author: goran
 * Created: January 23, 2013
 ********************************************************************
 * Global data types of project YieldPlus
 ********************************************************************)

TYPE
	applicationGlobal_enums : 
		(
		PAI_PVILIGHT_LIST_ITEM_INDEX_MAX := 127, (*???*)
		PAI_PVILIGHT_LIST_INDEX_MAX := 31, (*???*)
		PAI_PVILITE_ARRLIST_INDEX_MAX := 19, (*???*)
		PAI_PVILIGHT_PV_NAME_LEN := 255, (*???*)
		PAI_PVILIGHT_UDP_MSG_SIZE_MAX := 9000, (*???*)
		PAI_PVILIGHT_PV_VAL_TXT_LEN := 31 (*???*)
		);
END_TYPE

(*==========================*)
(*==== UDP Message Header ====*)
(*==========================*)

TYPE
	ctrlInterfaceMsgHeader_typ : 	STRUCT 
		msgVersion : STRING[31];
		msgCounter : UDINT;
		msgEchoRequest : UDINT;
		msgEchoResponse : UDINT;
	END_STRUCT;
END_TYPE

(*==========================*)
(*==== UDP Command Message ====*)
(*==========================*)

TYPE
	ctrlInterfaceCmd_typ : 	STRUCT 
		header : ctrlInterfaceMsgHeader_typ;
	END_STRUCT;
END_TYPE

(*==========================*)
(*==== UDP Status Message ====*)
(*==========================*)

TYPE
	ctrlInterfaceStatus_typ : 	STRUCT 
		header : ctrlInterfaceMsgHeader_typ;
	END_STRUCT;
END_TYPE

(*==========================*)
(*==========================*)
(*==========================*)
(*==========================*)
(*==========================*)
(*==========================*)
(*==== paiPVI List Item =========*)
(*==========================*)

TYPE
	paiPVIGetArrItem_typ : 	STRUCT 
		isUsed : UDINT;
		isError : UDINT;
		isChanged : UDINT;
		pvArrName : STRING[PAI_PVILIGHT_PV_NAME_LEN];
		pvArrDimension : UDINT;
		arrIdxNext : UDINT;
		arrItemsPerUpdate : UDINT;
		notificationTimer : TON;
		arrItemAproxSizeJSON : UDINT;
		updateState : UDINT;
		rpcId : UDINT;
		iRmtIPAddr : STRING[UDP_IP_ADDR_LEN]; (*IP address of the remote host*)
		iRmtUdpPort : UINT; (*UDP port number of the remote host*)
	END_STRUCT;
END_TYPE

(*==========================*)
(*==== paiPVI List Item =========*)
(*==========================*)

TYPE
	paiPVIListItem_typ : 	STRUCT 
		isUsed : UDINT;
		isError : UDINT;
		isChanged : UDINT;
		pvName : STRING[PAI_PVILIGHT_PV_NAME_LEN];
		pvType : UDINT;
		pvDimension : UDINT;
		pvLen : UDINT;
		pPvValue : UDINT;
		pvValueOld : UDINT;
		pvValueOldFloat : REAL;
		pvValueTxt : STRING[PAI_PVILIGHT_PV_VAL_TXT_LEN];
	END_STRUCT;
END_TYPE

(*==========================*)
(*==== paiPVI List  ============*)
(*==========================*)

TYPE
	paiPVIList_typ : 	STRUCT 
		isUsed : UDINT; (*0-list not in use, 1-list is in use*)
		isActive : UDINT; (*0-list not active (no notifiation send), 1-list is active*)
		isChanged : UDINT; (*0-no PV changed 1-one or more PV-s changed*)
		isNotification : UDINT; (*0-no need to send notification msg, 1-request to send notification message*)
		iRmtIPAddr : STRING[UDP_IP_ADDR_LEN]; (*IP address of the remote host*)
		iRmtUdpPort : UINT; (*UDP port number of the remote host*)
		listName : STRING[PAI_PVILIGHT_PV_NAME_LEN]; (*name of the PV list*)
		notificationMode : UDINT;
		notificationFormat : UDINT;
		notificationTimeMinPeriod_s : REAL; (*[s] min time (i.e. max frequencly) between two notification messages*)
		notificationTimeMaxPeriod_s : REAL; (*[s] max time (i.e. min frequencly) between two notification messages*)
		notificationTimerMin : TON;
		notificationTimerMax : TON;
		itemCount : UDINT; (*number of PV items in the list*)
		itemList : ARRAY[0..PAI_PVILIGHT_LIST_ITEM_INDEX_MAX]OF paiPVIListItem_typ;
		pJson : UDINT; (*ptr to *)
	END_STRUCT;
END_TYPE

(*==========================*)
(*==== paiPVI List Collection =====*)
(*==========================*)

TYPE
	paiPVIListCollection_typ : 	STRUCT 
		listCount : UDINT;
		listCollection : ARRAY[0..PAI_PVILIGHT_LIST_INDEX_MAX]OF paiPVIList_typ;
		arrayListDlyTimer : TON;
		arrayListCollection : ARRAY[0..PAI_PVILITE_ARRLIST_INDEX_MAX]OF paiPVIGetArrItem_typ; (*support for automatic updates of entire PV arrays following a getPvArray JSON request*)
	END_STRUCT;
END_TYPE
