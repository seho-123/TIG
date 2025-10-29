
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <math.h>

_LOCAL  LCRLimScal_typ  LimScal_AO;

_GLOBAL INT hrd_EncInp;
_GLOBAL INT hrd_AnalOut;
_GLOBAL  plcbit hrd_ResetBtn;
_GLOBAL  plcbit hrd_LightOut;
_GLOBAL float ratio;
_GLOBAL DINT actPos _VAR_RETAIN; //_VAR_RETAIN

_LOCAL float debounce;

INT _EncInp;

void _INIT ProgramInit(void)
{

	LimScal_AO.x1 = -1.0;
	LimScal_AO.y1 = -32767.0;
	LimScal_AO.x2 = 1.0;
	LimScal_AO.y2 = 32767.0;

	ratio = 1.0/(1196.0 * 6.0);
	
	_EncInp = hrd_EncInp;


}

void _CYCLIC ProgramCyclic(void)
{DINT delta;

	if(hrd_ResetBtn){

		if(debounce > 3.0 )
			actPos = 0;
		else
			debounce += 0.0008;


	}
	else
		debounce = 0.0;
		

	
	delta = (DINT)hrd_EncInp - (DINT)_EncInp;
	_EncInp = hrd_EncInp;

	if(delta > 16000)
		delta -= 0xFFFF;
	else if( delta < -16000)
		delta += 0xFFFF;

	actPos += delta;
	
	

	
	LimScal_AO.x = (float)actPos * ratio;	
	LCRLimScal(&LimScal_AO);

	hrd_AnalOut = (INT)LimScal_AO.y;

	
	hrd_LightOut = (plcbit)(fabsf(LimScal_AO.x)< 0.01);

		
}


