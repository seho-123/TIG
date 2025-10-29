TYPE
	RS_Channel_typ : STRUCT	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\lib\WSI_types.h" #2"1854" #3"16" *)
		Status : USINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\lib\WSI_types.h" #2"1855" #3"15" *)
		Data : ARRAY[0..31] OF USINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\lib\WSI_types.h" #2"1856" #3"15" *)
		Len : UINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\lib\WSI_types.h" #2"1857" #3"14" *)
		Cntr : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\lib\WSI_types.h" #2"1858" #3"15" *)
	END_STRUCT;
	UDP_int_typ : STRUCT	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"29" #3"16" *)
		send : UdpSend;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"31" #3"21" *)
		receive : UdpRecv;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"32" #3"21" *)
		close : UdpClose;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"33" #3"22" *)
		open : UdpOpen;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"34" #3"25" *)
	END_STRUCT;
	UDP2_typ : STRUCT	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"38" #3"16" *)
		Step : UINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"41" #3"10" *)
		SentCntr : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"43" #3"15" *)
		RecevCntr : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"44" #3"15" *)
		Intr : UDP_int_typ;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"46" #3"21" *)
		pSendChan : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"47" #3"24" *)
		pRecevChan : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/UDP_Client.c" #2"48" #3"24" *)
	END_STRUCT;
	bgRingBuffer_typ : STRUCT	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"23" #3"16" *)
		bufSize : DINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"25" #3"13" *)
		init : DINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"26" #3"13" *)
		overrun : DINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"27" #3"13" *)
		dbgLevel : DINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"28" #3"13" *)
		dbgFormat : DINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"29" #3"13" *)
		ptrCircularBufferBegin : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"31" #3"13" *)
		ptrCircularBufferEnd : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"32" #3"13" *)
		ptrDataIn : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"34" #3"13" *)
		ptrDataOut : UDINT;	(* #1"C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Universal/UDP_Client/..\..\Lib\utilityLib\bgringbuffer.h" #2"35" #3"13" *)
	END_STRUCT;
END_TYPE
