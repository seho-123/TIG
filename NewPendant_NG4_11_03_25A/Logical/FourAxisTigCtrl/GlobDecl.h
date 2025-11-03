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

#include <..\GlobDef.h>

#include <..\..\Universal\SuperDeclaration.h>

#include <..\..\Lib\WSI_types.h>
	
//   THIS KK MUST BE
// Wazne do manualnej kalibracjii
_GLOBAL UINT KK_ManualCalib;
_GLOBAL float KK_ManualCalib_set_value;
_GLOBAL UINT KK_DoNotHashCalculation , KK_No_TVL_Restart;
_GLOBAL UINT Hold_Estop_ON;
_GLOBAL DINT StartPos , FinalPos; // must be for android display

/////////////////////////////////////////////

#endif
