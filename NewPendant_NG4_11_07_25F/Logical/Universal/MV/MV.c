/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: DigiUSB
 * File: DigiUSB.c
 * Author: ckaczkowski
 * Created: July 17, 2015
 ********************************************************************
 * Implementation of program DigiUSB
 ********************************************************************/


#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

//#include <stdio.h>
#include <bur/plc.h>
#include <bur/plctypes.h>

#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>

   

/* Datatypes and datatypes of function blocks */
typedef struct Client_typ
{	unsigned short sStep;
	struct TcpOpen TcpOpen_0;
	struct TcpClient TcpClient_0;
	struct TcpRecv TcpRecv_0;
	struct TcpSend TcpSend_0;
	struct TcpIoctl TcpIoctl_0;
	struct TcpClose TcpClose_0;
	struct tcpLINGER_typ linger_opt;
	unsigned long recv_timeout;
} Client_typ;

static const char* LOAD_CMD[] = {
"load |1.pt| \r",
"load |2.pt| \r",
"load |3.pt| \r",
"load |4.pt| \r",
"load |5.pt| \r",
};

/* Variables */

_LOCAL struct Client_typ Client[MV_NUM];
_LOCAL plcstring CMD_Buffer[MV_NUM][51];

void Load_CMD(USINT inst , USINT mode);



void _INIT MVINIT(void)
{


	Client[0].TcpOpen_0.enable = 1;	
	Client[0].TcpOpen_0.pIfAddr = (UDINT)"192.168.10.178";  
	Client[0].TcpClient_0.pServer = Client[0].TcpOpen_0.pIfAddr; 
	Client[0].TcpOpen_0.port = 0;
	Client[0].TcpClient_0.portserv = 101;
	Client[0].TcpOpen_0.options = 0;
	Client[0].TcpSend_0.pData = (UDINT)&CMD_Buffer[0];
	Client[0].sStep = 1;

	

}

void _CYCLIC MVCYCLIC(void)
{
	USINT i;
		
	for (i=0; i<MV_NUM; i++)
	{

		if(MV[i].xCMD == MV_RESTART_CMD){

			MV[i].xCMD = 0;
			Client[i].sStep = 1;
			
		}
			
	
		switch(Client[i].sStep)
		{
			case 1:
				
				TcpOpen(&Client[i].TcpOpen_0);
				
				if (Client[i].TcpOpen_0.status == 0)
					Client[i].sStep = 2;
			break;
				
			case 2:
				
				Client[i].TcpClient_0.ident = Client[i].TcpOpen_0.ident;
				Client[i].TcpClient_0.enable = 1;
				
				TcpClient(&Client[i].TcpClient_0);
				
				if (Client[i].TcpClient_0.status == 0)
					Client[i].sStep = 3;
				
				else if(Client[i].TcpClient_0.status != 0xFFFF)// error
					Client[i].sStep = 5;
				
				
			break;
				
			case 3:		
					
				
				switch(MV[i].xCMD)
				{
					case MV_LOAD_NEXT_CMD:
						
							MV[i]._intr._prevMode ++;
							if(MV[i]._intr._prevMode > 4)
								MV[i]._intr._prevMode = 0;
						
							Load_CMD(i , MV[i]._intr._prevMode);	
							
							Client[i].sStep = 4;
						
						
					break;
					
			    	default:	

						MV[i].xCMD = 0; // reset wrong command
						
					break;					
				}

				
		
				
			break;


			case 4:

				Client[i].TcpSend_0.ident = Client[i].TcpClient_0.ident;
				Client[i].TcpSend_0.enable = 1;	

				TcpSend(&Client[i].TcpSend_0);
				
				if(Client[i].TcpSend_0.status == 0){

					MV[i].xCMD = 0; // reset  command
					
					Client[i].sStep = 3; // next command
					
				}
				else if(Client[i].TcpSend_0.status != 0xFFFF)
					Client[i].sStep = 5;


			break;	
				
			case 5:
				
				Client[i].TcpClose_0.ident = Client[i].TcpSend_0.ident;
				Client[i].TcpClose_0.enable = 1;

				MV[i].xCMD = 0;  // reset command		
				
				TcpClose(&Client[i].TcpClose_0);
				if (Client[i].TcpClose_0.status !=  0xFFFF)
					Client[i].sStep = 1; // try to open port again
				
				
			break;
		}
	}	
}





void Load_CMD(USINT inst , USINT mode){

	strcpy(CMD_Buffer[inst] , LOAD_CMD[mode]);
	
	Client[inst].TcpSend_0.datalen = strlen(CMD_Buffer[inst]);
}


