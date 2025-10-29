/********************************************************************
 * COPYRIGHT -- HP
 ********************************************************************
 * Program: udpsimonly
 * File: udpsimonly.c
 * Author: goran
 * Created: June 25, 2013
 ********************************************************************
 * Implementation of program udpsimonly
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT udpsimonlyINIT(void)
{
	/* TODO: Add code here */
	//Overrid UDP Client operation settings (Use HMI if needed)
	//gUdpSystemCfg.udpClientMode = 1;//continues mode
	gUdpSystemCfg.udpClientUpdateRate = 10;//[Hz]
	//TODO: Make this easy to change from sim to real target
	strcpy(gUdpSystemCfg.udpRemoteHostIpAddress.text, "127.0.0.1");
	/*strcpy(gUdpSystemCfg.udpRemoteHostIpAddress.text, "192.168.10.123");*/
	
	gUdpClient.clientIf.iTaskCycleTimeXms = 10;//udp client runs under 10ms cycle time
}

void _CYCLIC udpsimonlyCYCLIC(void)
{
	/* TODO: Add code here */
}

