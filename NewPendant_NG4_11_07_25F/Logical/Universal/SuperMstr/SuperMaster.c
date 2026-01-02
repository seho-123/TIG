/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: SuperMstr
 * File: SuperMstr.c
 * Author: ckaczkowski
 * Created: June 18, 2014
 ********************************************************************
 * Implementation of program SuperMstr
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\..\Universal\SuperDeclaration.h>
//#include <..\..\HotWire\GlobDef.h>  // To use hardware mapping in other projects

#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\LCDLib.c> 
#include <..\..\Lib\WSI_lib.c> 


_LOCAL LCD_NODE_obj NODES[NODES_NUM_MAX];
_LOCAL Units_obj Units[unitMaxIndx];
_LOCAL DINT _NextHeadConfig_Indx;



_LOCAL SerialPortDriver_typ LCD_RS232_Main;
void LCD_Display();


void _INIT SuperMstrINIT(void)
{

	Master[0].pNode[0] = (UDINT)&NODES[0];
	Master[0].pUnits = Units;

	Master[1].pNode[0] = (UDINT)&NODES[0];

	#ifdef UNIV_SYSTEM

	LCD_RS232_Main.txData[0] = 0; 	

	SerialPortInit(&LCD_RS232_Main , "IF1" , "/PHY=RS232 /BD=9600 /DB=8 /PA=N /SB=1");

	#endif

	LCD_Init_Step = 0;

}

void _CYCLIC SuperMstrCYCLIC(void)
{

	#ifdef UNIV_SYSTEM
	LCD_Display();
	#endif
}

void LCD_Display(){
	char *p;
	static int _initDone=0;


	//if(_ESTOP != hardware.Inputs[5])
		//LCD_Init_Step = 50;
	
	
	switch(LCD_Init_Step){


		case 0:

			//_Counter = menu[0].Counter;
			
			strcpy(curHeadName , NewHeadName[NextHeadConfig_Indx]);
			_NextHeadConfig_Indx = NextHeadConfig_Indx;
			LCD_Init_Step = 10;			
			
		break;
		

		case 10:			
					
			p = LCD_RS232_Main.txData;						

			//xsprintf(p, "Act: %s" , "CMT -> BPV");
			xsprintf(p, PLC_Version_TXT);
			LinePad(p  , 40);

			//xsprintf(p+40, "Next: %s" , " ONLY BPV");
			//xsprintf(p+40, "Verion 2025");
			xsprintf(p+40, curHeadName);			
			LinePad(p+40  , 40);

					
			LCD_RS232_Main.txDataLen = 80; //send length
			LCD_RS232_Main.txRequest = 1; //send data flag

			LCD_Init_Step ++;

						
			
		break;	


		case 11:

			if(_NextHeadConfig_Indx != NextHeadConfig_Indx)
				LCD_Init_Step = 0;

		break;
		

	}	

	SerialPortCyclic(&LCD_RS232_Main);
	
	
}










