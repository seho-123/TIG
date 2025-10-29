/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: WeldServ
 * File: GlobDecl.h
 * Author: Christopher
 * Created: June 09, 2009
 *******************************************************************/
#ifndef _GLOBAL_DECL_H
#define _GLOBAL_DECL_H

#include <bur/plctypes.h>

//#include <..\GlobDef.h>
//#include <..\Lib\kinematicLib.h> 

#include <..\..\Lib\Statelib.c>

#include <..\..\Lib\WSI_types.h>


_GLOBAL MTR_obj motors[NUM_MOTORS];

_GLOBAL PS_obj powerSupplies[NUM_POWER_SUPPLIES];

_GLOBAL HARDWARE_obj hardware;

_GLOBAL MENU_obj menu[NUM_LCD];

_GLOBAL MACHINE_obj machine;

_GLOBAL SM_obj master_sm;

_GLOBAL_RETAIN USERVALUES_obj userValues;//  _VAR_RETAIN;

_GLOBAL USINT HotWireProjectCMD;





#endif
