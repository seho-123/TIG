/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: lib
 * File: statelib.h
 * Author: Christopher
 * Created: February 10, 2009
 *******************************************************************/
 
/* Do not edit ! */

#ifndef _STATELIB_H
	#define _STATELIB_H
	
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

	
	/* Datatypes and datatypes of function blocks */
	typedef struct SM_obj
	{	UDINT Step;
		UDINT oldStep;
		plcbit new;
		unsigned short ErrorStep;
		TON_typ Tmr1;
		plcstring Text[52];
	} SM_obj;

	typedef struct HS_obj
	{	plcbit Go;
		plcbit Ready;
		signed long Data;
		plcbit Error;
	} HS_obj;

	
	/********************************************************
	* BLOCK POINTERS:
	*********************************************************/
	TON_typ *t1,*t2,*t3;
	SM_obj  *sm;

	/********************************************************
	* DEFINITIONS:
	*********************************************************/

	/* General */
	#define ON							1
	#define OFF							0

	#define YES							1
	#define NO							0 

	/* State Machine */
	#define BASE_SM						0
	#define BASE_SM2					6
	#define ERROR_SM					100

	#define BEGIN_SM 		            sm_serv( sm );TimerExecute(t1);TimerExecute(t2);TimerExecute(t3);switch( sm->Step ) {  
	#define END_SM   		            default:;sm_text("Invalid Step ", sm);break;}
	#define INC_SM 			            sm->Step++
	#define GO_BASE_SM		            sm->Step = BASE_SM
	#define GO_BASE_SM2	                sm->Step = BASE_SM2
	#define GOTO_SM			            sm->Step = 
	#define RESET_SM		            sm->Step = 0
	#define GO_ERROR_SM	                sm->ErrorStep = sm->Step; sm->Step = ERROR_SM

	/* Timer */
	#define IF_TMR1     if ( TimerIsTimeOut( t1 ) )
	#define IF_TMR2     if ( TimerIsTimeOut( t2 ) )
	#define IF_TMR3     if ( TimerIsTimeOut( t3 ) )

	#define TB          sm_text( 
	#define TE          , sm);

	#define SMTXT(x)    sm_text((x), sm)

	//#define TMR1S       TimerStart( t1, 
	//#define TMRE        );
	//#define T2S         TimerStart( t2, 

	#define TMR1_20s 	TimerStart( t1, 20.0 )
	#define TMR1_15s 	TimerStart( t1, 15.0 )
	#define TMR1_10s 	TimerStart( t1, 10.0 )
	#define TMR1_5s 	TimerStart( t1, 5.0 )
	#define TMR1_2s 	TimerStart( t1, 2.0 )
	#define TMR1_1s 	TimerStart( t1, 1.0 )
	#define TMR1_500ms	TimerStart( t1, 0.5 )
	#define TMR1_250ms	TimerStart( t1, 0.25 )
	#define TMR1_100ms	TimerStart( t1, 0.1 )

	#define TMR2_20s 	TimerStart( t2, 20.0 )
	#define TMR2_15s 	TimerStart( t2, 15.0 )
	#define TMR2_10s 	TimerStart( t2, 10.0 )
	#define TMR2_5s 	TimerStart( t2, 5.0 )
	#define TMR2_2s 	TimerStart( t2, 2.0 )
	#define TMR2_1s 	TimerStart( t2, 1.0 )
	#define TMR2_500ms	TimerStart( t2, 0.5 )
	#define TMR2_250ms	TimerStart( t2, 0.25 )
	#define TMR2_100ms	TimerStart( t2, 0.1 )

	#define TMR3_20s 	TimerStart( t3, 20.0 )
	#define TMR3_15s 	TimerStart( t3, 15.0 )
	#define TMR3_10s 	TimerStart( t3, 10.0 )
	#define TMR3_5s 	TimerStart( t3, 5.0 )
	#define TMR3_2s 	TimerStart( t3, 2.0 )
	#define TMR3_1s 	TimerStart( t3, 1.0 )
	#define TMR3_500ms	TimerStart( t3, 0.5 )
	#define TMR3_250ms	TimerStart( t3, 0.25 )
	#define TMR3_100ms	TimerStart( t3, 0.1 )	



	/* Prototyping of functions and function blocks */

	plcbit SmTimerDone(struct SM_obj* sm);
	void SmTimerStart(struct SM_obj* sm, float timeoutTime);
	
	signed long TimerStart(struct TON* timer, float timeoutTime);
	plcbit TimerExecute(struct TON* timer);
	plcbit TimerIsTimeOut(struct TON* timer);
	plcbit HSCompare(struct HS_obj* hs, signed long status, signed long compare);
	signed long sm_text(plcstring* text, struct SM_obj* sm);
	signed long sm_serv(struct SM_obj* sm);
	signed long sm_init(struct SM_obj* sm);
	signed char HSReset(struct HS_obj* hs);
	plcbit sm_reset(struct SM_obj* sm, plcbit Go);


	UINT SetBit( UINT *wordh, UINT Bit_Num );
	plcbit GetBit( UINT wordh, UINT Bit_Num );
	UINT ResetBit( UINT *wordh, UINT Bit_Num );
	UINT ToggleBit( UINT *wordh, UINT Bit_Num );

	UDINT SetBit32( UDINT *wordh, UINT Bit_Num );
	plcbit GetBit32( UDINT wordh, UINT Bit_Num );
	UDINT ResetBit32( UDINT *wordh, UINT Bit_Num );
	UDINT ToggleBit32( UDINT *wordh, UINT Bit_Num );


	__asm__(".section \".plc\"");

	__asm__(".previous");


#endif /* _STATELIB_ */
