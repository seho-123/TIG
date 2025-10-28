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



void _INIT SuperMstrINIT(void)
{

	Master[0].pNode[0] = (UDINT)&NODES[0];
	Master[0].pUnits = Units;

	Master[1].pNode[0] = (UDINT)&NODES[0];

}

void _CYCLIC SuperMstrCYCLIC(void)
{
	
}

