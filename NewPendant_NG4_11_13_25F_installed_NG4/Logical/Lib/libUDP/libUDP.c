#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


/*
B&R components:
===============================================================================
*/
#include <bur/plc.h>
#include <bur/plctypes.h>




//void _NewDataSend(UDP_typ* udp);

///////////////////////////////////////////////////////////////////////


 signed char Udp_cyclic(UDP_typ* udp){


 	switch(udp->internal.step){

		case 0:				

			udp->listener.oStatus.error_number = 0;
			udp->listener.oStatus.error = 0;
			udp->listener.oStatus.ready = 0;

			udp->send.oStatus.error_number = 0;
			udp->send.oStatus.error = 0;
			udp->send.oStatus.ready = 0;

			if(udp->listener_config.buffer && (udp->listener_config.bufferSize > 0)){

				if(udp->internal.UdpOpen_0.ident){ // CLose existing connection on start

					UdpClose(&udp->internal.UdpClose_0);  //Call the Function

					if(udp->internal.UdpClose_0.status == ERR_OK) //
						udp->internal.step = 1;							
					
					else if (udp->internal.UdpClose_0.status == ERR_FUB_BUSY){} //UdpSend not finished -> redo 					
					  
					else{  //Error  

						udp->listener.oStatus.error_number = udp->internal.UdpClose_0.status;						
						udp->listener.oStatus.error = 1;
						udp->listener.oStatus.ready = 0;
						udp->internal.step = 100;
						
					}	

				}
				else
					udp->internal.step = 1;	


			}
			else{ // No buffer declare

					udp->listener.oStatus.error_number = 0xFFFFF;						
					udp->listener.oStatus.error = 1;
					udp->listener.oStatus.ready = 0;
					udp->internal.step = 100;

			}
			

		break;	


		case 1:


			udp->internal.UdpOpen_0.enable = 1;
			
			if(udp->listener_config.ipAddress){

				if(strlen(udp->listener_config.ipAddress) >= 8 )
					udp->internal.UdpOpen_0.pIfAddr = udp->listener_config.ipAddress;
				else
					udp->internal.UdpOpen_0.pIfAddr = 0; // Listen to all addrresses


			}else
				udp->internal.UdpOpen_0.pIfAddr = 0; // Listen to all addrresses
				
			udp->internal.UdpOpen_0.port = udp->listener_config.port;	// Port to listen
			udp->internal.UdpOpen_0.options = 0;
			
			UdpOpen(&udp->internal.UdpOpen_0);	// Call the Function

			if(udp->internal.UdpOpen_0.status == ERR_OK 
				|| udp->internal.UdpOpen_0.status == udpERR_ALREADY_EXIST){ // UdpOpen successful

								
				udp->internal.UdpSend_0.enable = 1;
				udp->internal.UdpSend_0.ident = udp->internal.UdpOpen_0.ident;	//Connection Ident from AsUDP.UDP_Open 				
				udp->internal.UdpSend_0.flags = 0;
				//
				udp->internal.UdpRecv_0.enable = 1;
				udp->internal.UdpRecv_0.ident = udp->internal.UdpOpen_0.ident;  //Connection Ident from AsUDP.UDP_Open 
				udp->internal.UdpRecv_0.pData	= udp->listener_config.buffer;  // Where to store the incoming data 
				udp->internal.UdpRecv_0.datamax = udp->listener_config.bufferSize;  // Lenght of data buffer 
				udp->internal.UdpRecv_0.flags = 0;
				//	
				udp->internal.UdpClose_0.enable = 1;
				udp->internal.UdpClose_0.ident = udp->internal.UdpOpen_0.ident;  //Connection Ident from AsUDP.UDP_Open 
				//
				udp->listener.oStatus.ready = 1;
				udp->send.oStatus.ready = 1;
				udp->internal.step = 5;	


			}
			else if( udp->internal.UdpOpen_0.status == ERR_FUB_BUSY ){}// UdpOpen not finished -> redo 			
			  	
			else {

				udp->listener.oStatus.error_number = udp->internal.UdpOpen_0.status;						
				udp->listener.oStatus.error = 1;
				udp->listener.oStatus.ready = 0;
				udp->internal.step = 100;


			}
			
		break;
		

		case 5: // Receive Data from the PS and Trasmit to the PS	
		

			UdpRecv(&udp->internal.UdpRecv_0);  // Call the Function

			if(udp->internal.UdpRecv_0.status == ERR_OK){// new response received	

				udp->listener.oStatus.error = 0;
				udp->listener.oStatus.ready = 1;
				udp->listener.oDataLen = udp->internal.UdpRecv_0.recvlen;				
				udp->listener.oStatus.counts++;
				
				if(udp->listener.iRxCallback){	
					
					void (*rxCallback)(UDINT pBuf, UDINT LenData , UDINT ringBuf) =  ( void(*) (UDINT, UDINT , UDINT) ) udp->listener.iRxCallback;
					(*rxCallback)( (UDINT)udp->listener_config.buffer , udp->listener.oDataLen , udp->listener.iRxRingBuf);
				
				}			
				
			}
			else if (udp->internal.UdpRecv_0.status == ERR_FUB_BUSY){} //UdpSend not finished -> redo 	

			else if (udp->internal.UdpRecv_0.status == udpERR_NO_DATA){} //UdpSend not finished -> redo 					
			
			else{ 
			
				udp->listener.oStatus.error_number = udp->internal.UdpRecv_0.status;						
				udp->listener.oStatus.error = 1;
				udp->listener.oStatus.ready = 0;
				udp->internal.step = 100;

			}


		break;	


		case 100:  // Error State

			if(udp->ResetCmd){
				udp->ResetCmd = 0;
				
				udp->internal.step ++;

			}	

		break;

		case 200:  // Idle for test purpose

		break;

		default: 
			
			udp->internal.step = 0;

			
		break;	




	}
	
	_NewDataSend(udp);
		
	
 
	return 0;
 }


void _NewDataSend(UDP_typ* udp){

	//////////////////////////////////////////////
	if(udp->send.iSendCMD){

		udp->listener.oStatus.error = 0;		

		if(udp->internal.step == 5){ // UDP Opened

			udp->internal.UdpSend_0.pHost = udp->send.iIPaddress;	
			udp->internal.UdpSend_0.port = udp->send.iPort; 	
			udp->internal.UdpSend_0.pData = udp->send.iBuffer;	// Which data to send				
			udp->internal.UdpSend_0.datalen = udp->send.iBufferSize;	// data length

			UdpSend(&udp->internal.UdpSend_0);

			if(udp->internal.UdpSend_0.status == ERR_OK){// new data sent				

				udp->send.iSendCMD = 0;
				udp->send.oStatus.counts++;

				if(udp->send.iTxCallback){	
					
					void (*txCallback)(UDINT pBuf, UDINT LenData) =  ( void(*) (UDINT, UDINT) ) udp->send.iTxCallback;
					//(*txCallback)( (UDINT)udp->send.iBuffer , udp->send.iBufferSize );
					(*txCallback)( (UDINT)udp->internal.UdpSend_0.pData , udp->internal.UdpSend_0.sentlen);
					
				}


			}
			else if (udp->internal.UdpSend_0.status == ERR_FUB_BUSY){} //UdpSend not finished -> redo
					  
			else{  //Error  

				udp->send.iSendCMD = 0;		

				udp->listener.oStatus.error_number = udp->internal.UdpRecv_0.status;						
				udp->listener.oStatus.error = 1;
							

			}
			
		}
		else{ // Command error

			udp->send.iSendCMD = 0;

			udp->listener.oStatus.error_number = 0xFFFF;						
			udp->listener.oStatus.error = 1;
			
		}

	}	


}
