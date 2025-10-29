/********************************************************************
 * COPYRIGHT -- belgor
 ********************************************************************
 * Program: appUDP
 * File: appUDP.c
 * Author: goran
 * Created: Jan 25, 2013
 ********************************************************************
 * Implementation of program appUDP
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
//#include <string.h>

#include <../../bglib/bgString.c>
#include <../../bglib/bgBitUtilities.c>

/*
###############################################################################
Utilities
###############################################################################
*/

/**
 Simple logging
 Log goes to an array of PV strings > see _LOCAL STRING log[][] variable below
 Log output is visible via Automation Studio watch window
 Example of a log statement used within the code is: 
 DBG(lgr, "This is log example. %d ", value1);
 To complety disable logging from this source file, change next line to: #if 0
IMPORTANT:
==========
DBGINIT() statement MUST BE PLACED as a first statement within the task _INIT function!
Example:
void _INIT myTaskINIT(void)
{
	//TODO: Add code here
	//init logger object
	DBGINIT(lgr, log, LOG_ENTRY_COUNT, LOG_ENTRY_LENGTH);
}
*/
#if 1
#include <../../bglib/bgPVLogger.c>
#define LOG_ENTRY_COUNT 20	//max number of log entries
#define LOG_ENTRY_LENGTH  32 //max length of individual log message. Longer msg are truncated
_LOCAL STRING log[LOG_ENTRY_COUNT][LOG_ENTRY_LENGTH]; //log output visible from Automation Studio!
static bgPVLogger_typ lgr;	//PV logger object
#else
#define DBG(loggerInstance, format, args...) ;
#define DBGINIT(loggerInstance, logoutput, logEntryCount, logEntrySize)  ;
#endif



//UDP Server Receive function
static void udpServerCallback(UDINT pRxBuffer, UDINT lenRxData)
{
	DBG(lgr, "udp rx, datalen=%d", lenRxData);
	if(lenRxData > sizeof(gJsontextRx)){
		DBG(lgr, "udp rx datalen(%d) TOO LONG!", lenRxData);
		return;
	}
	memcpy(gJsontextRx, pRxBuffer, lenRxData);
}


//UDP Server Receive function
static void udpServerCallbackAfterReceive(UDINT pRxBuffer, UDINT lenRxData)
{
	udpServerCallback(pRxBuffer, lenRxData);
}


//UDP Client Send Callback function
static void udpClientCallback(UDINT pTxBuffer, UDINT lenTxBuffer, UDINT* pTxDataLen)
{
int length;

	if(gTxRply){
		gTxRply = 0;
		strcpy( pTxBuffer, gJsontextTxRply );

		length = strlen(gJsontextTxRply);
		DBG(lgr, "udpClientCallback len1=%d", length);	
		
		//define a total lenght of the message
		length = strlen(pTxBuffer);
		DBG(lgr, "udpClientCallback len2=%d", length);	
		
		(*pTxDataLen) = length;
		
		DBG(lgr, "udpClientCallback[,20]: %s", &(((char*)pTxBuffer)[length -  20]));	
		
		return;	
	}else
	if(gTxEvent){
		gTxEvent = 0;
		strcpy( pTxBuffer, gJsontext );
		//define a total lenght of the message
		
		(*pTxDataLen) = strlen(pTxBuffer);
		
		DBG(lgr, "udp server rx timeout");	
		
		return;
	}	
	
	(*pTxDataLen) = 0;
}

//UDP Client Send Callback function
static void udpClientCallbackBeforeSend(UDINT pTxBuffer, UDINT lenTxBuffer, UDINT* pTxDataLen)
{
	udpClientCallback(pTxBuffer, lenTxBuffer, pTxDataLen);
}


static void	udpTimeOutHandler()
{
	//application logic - an example of handling a UDP receive timeout event 
	static BOOL oldRxTimeout;
	if( gUdpServer.serverIf.oRxIsTimeout && !oldRxTimeout){
		DBG(lgr, "udp server rx timeout");	
	}
	
	if( !gUdpServer.serverIf.oRxIsTimeout && oldRxTimeout){
		DBG(lgr, "udp server rx ok");	
	}	
	
	oldRxTimeout = gUdpServer.serverIf.oRxIsTimeout;
}

/*
###############################################################################
Task execution entry points: Init & Cyclic Functions
###############################################################################
*/
void _INIT udpAppINIT(void)
{
	/* TODO: Add code here */
	//init logger object
	DBGINIT(lgr, log, LOG_ENTRY_COUNT, LOG_ENTRY_LENGTH);

	//Define application specific UDP client tx callback function 
	gUdpClient.clientIf.iTxCallback = (UDINT)udpClientCallbackBeforeSend;

	//Define application specific UDP server rx callback function 
	gUdpServer.serverIf.iRxCallback = (UDINT)udpServerCallbackAfterReceive;
	gUdpServer.serverIf.oRxDataLenMax = (UDINT)sizeof(ctrlInterfaceCmd_typ);
	
	//Adjust UDP client/server settings according to the application needs (i.e. use HMI if needed)
	//For other defaults see:
	//paiUDPService.var (i.e. defined UDP related const)
	//udpserv.c/udpservINIT() and udpclient.c/udpclientINIT()
	gUdpSystemCfg.udpClientMode = 2;//user mode
	gUdpSystemCfg.udpClientUpdateRate = 20;//[Hz]
	//strcpy(gUdpSystemCfg.udpRemoteHostIpAddress.text, "192.168.100.31");
	//strcpy(gUdpSystemCfg.udpRemoteHostIpAddress.text, "192.168.100.171");
	strcpy(gUdpSystemCfg.udpRemoteHostIpAddress.text, "192.168.100.255");
	gUdpClient.clientIf.iTaskCycleTimeXms = 10;//udp client runs under 10ms cycle time
	gUdpServer.serverIf.iRxTimeoutXms = 4000;//udp server rx timeout time
	
}

void _CYCLIC udpAppCYCLIC(void)
{
	/* TODO: Add code here */

#if 0		
	//application logic - on send complete, copy status into last statusSent
	if(gUdpClient.clientIf.oSendDone){
		gUdpClient.clientIf.oSendDone = 0;		
		//save last transmitted data
		
	}

	//application logic - en example of handling a UDP rx event	
	if(gUdpServer.serverIf.oRxReady){
		gUdpServer.serverIf.oRxReady = 0;
		
	}
#endif	
	
	//handle a UDP rx timeout
	udpTimeOutHandler();
}
//EOF

