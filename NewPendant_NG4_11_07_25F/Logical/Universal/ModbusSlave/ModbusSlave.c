/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Program: ModbusSlave
 * File: ModbusSlave.c
 * Author: wsi
 * Created: April 17, 2013
 ********************************************************************
 * Implementation of program ModbusSlave
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


_LOCAL MBslave_typ MBSlaveTCP;
_LOCAL modbus_slave_cfg_typ MBSlaveCfg;


_LOCAL plcbit DO_bool[100];
_LOCAL plcbit DI_bool[100];

_LOCAL UINT AO_Word[100];
_LOCAL UINT AI_Word[100];


_LOCAL plcstring logger[20][50+1];
_LOCAL plcbit StartStop;


/////////

_LOCAL DINT  DINT_Data;
_LOCAL float FLOAT_Data; 
_LOCAL char  FLOAT_char_Data[4];
_LOCAL char  DINT_char_Data[4];




void _INIT ModbusSlaveInit(void)
{int i;

	/*

	for(i = 0 ; i < 100 ; i ++){

		MBSlaveCfg.p_coils[i] = (UDINT)&DO_bool[i];
		MBSlaveCfg.p_discrete_inputs[i] = (UDINT)&DI_bool[i];
		MBSlaveCfg.p_holding_registers[i] = (UDINT)&AO_Word[i];
		MBSlaveCfg.p_input_registers[i] = (UDINT)&AI_Word[i];
	}
	*/
	

	MBSlaveCfg.p_input_registers[2] = (UDINT)&DINT_Data;
	MBSlaveCfg.p_input_registers[3] = MBSlaveCfg.p_input_registers[2]+2;
	
	MBSlaveCfg.p_input_registers[0] = (UDINT)&FLOAT_Data;
	MBSlaveCfg.p_input_registers[1] = MBSlaveCfg.p_input_registers[0]+2;


	
	MBSlaveTCP.enable = 1;
	MBSlaveTCP.device = "IF2";
	MBSlaveTCP.p_cfg = &MBSlaveCfg;
	//MBSlaveTCP.port = 502;

	MBSlaveTCP.master_timeout = 5000;

	MBSlaveTCP.log.data_lines = 20;
	MBSlaveTCP.log.pData = &logger;
	
	//StartStop = 1;

	
}

void _CYCLIC ModbusSlave(void)
{
	
	
	if(StartStop){
		
		DINT_Data++;
		FLOAT_Data+= 0.0001; 

	}

	memcpy(FLOAT_char_Data , &FLOAT_Data , 4);
	memcpy(DINT_char_Data , &DINT_Data , 4);
	
	MBslave(&MBSlaveTCP);

}

void _EXIT ModbusSlaveExit(void)
{



	
}

