/*********************************************************
* FILE    	: 		statelib.c
*
* OBJECT	: 	N/A
*
* OVERVIEW	: 	Common Prototypes and Dependancies
*           
*
* HISTORY : 		02.20.06 - Version 1
*				
*
*				
*
* DEPENDANCIES:
*********************************************************/

#ifndef _STATELIB_C
#define _STATELIB_C

#include <..\..\lib\WSI_types.h>

#include <..\..\lib\statelib.h>
//#include <string.h>
#include <..\..\lib\xstring.c>


/********************************************************
* CODE BASE:
*********************************************************/

plcbit SmTimerDone(struct SM_obj* sm){
	return sm->Tmr1.Q;
}
void SmTimerStart(struct SM_obj* sm, float timeoutTime){

	sm->Tmr1.IN = 0;
	TON(&sm->Tmr1);
	sm->Tmr1.IN = 1;
	sm->Tmr1.PT = (int) ((timeoutTime * 1000.0)+0.5);
	TON(&sm->Tmr1);
	
}

SINT HSReset( HS_obj *hs )
{
	memset( hs, 0, sizeof(HS_obj ));
	return 0;
}

BOOL HSCompare( HS_obj *hs, DINT status, DINT compare )
{
	/* Proper Execution: Upstream Process */
	if ( status == compare ) {
		hs->Ready	= 1;
		hs->Error	= 0;
		return 1;
	}
	else
		return 0;
}


DINT TimerStart( TON_typ *timer, REAL timeoutTime ) {
	timer->IN = 0;
	TON(timer);
	timer->IN = 1;
	timer->PT = (int) ((timeoutTime * 1000.0)+0.5);
	TON(timer);
	return 1;
}

BOOL TimerExecute( TON_typ *timer ) {
	TON(timer);
	return 1;
}

REAL TimerTimeRemaining( TON_typ *timer ) {
	return  ( (REAL) (timer->PT - timer->ET) /1000.0);
}

BOOL TimerIsTimeOut( TON_typ *timer ) {
	return timer->Q;
}

DINT sm_init( SM_obj *sm  ) 
{

	/* Text Field: */
	strcpy( sm->Text, "Yet to be Started" );

	/* Default */
	sm->oldStep			= 255;
	sm->Step			= 0;
	return 1;
}

DINT sm_serv( SM_obj *sm ) 
{
	/* Step Transition */	
	if (sm->Step != sm->oldStep) {
		sm->new		= ON;
	    	sm->oldStep 		= sm->Step;
	}	

	TimerExecute(&sm->Tmr1);
	return 1;
}


BOOL sm_reset( SM_obj *sm, BOOL Go ) 
{

	/* Bad Pointer or No New Data */ 
	if (sm == 0) 
		return OFF; 	
	
	/* Default */
	sm->oldStep 		= 255;
	sm->Step			= 0;
	
				
	/* Return */
	return ON;	
}


DINT sm_text( STRING text[32] , SM_obj *sm) 
{
USINT stringh[50];

	
	/* Bad Pointer or No New Data */ 
	if ((sm == 0) || (!sm->new) )
		return 0;

	/* Turn Off Signal */	
	sm->new = OFF;

	

	
	if(strlen(text)>31)
		xsprintf(sm->Text,"%d:%31s ", sm->Step , text );
	else
		xsprintf(sm->Text,"%d:%s", sm->Step , text );
	
		
	/* Check String Length */

	return 1;
	
}

UINT BitToWord( BOOL *b )
{
UINT i, wordh = 0;

	for (i=0;i<16;i++,b++) 
		wordh = wordh + ((UINT) *b << i);
		
	return wordh;	
}

/* Return: Quantity of High Bits */
USINT WordToBit( DINT x, BOOL *b)
{
UINT i;
USINT q = 0;
 
	/* Assign Masks */
	for (i=0;i<16;i++,b++) {
		*b = (BOOL) ( x >> i ) & 0x01;
		if (*b)
			q++;
	}		

	return q;
}


UINT ToggleBit( UINT *wordh, UINT Bit_Num )
{
	if(Bit_Num >= 16)
		return *wordh;
	
	UINT offset = 1;
	offset <<= Bit_Num;

	if(*wordh & offset)
		ResetBit(wordh,Bit_Num );
	else
		SetBit(wordh,Bit_Num );
	
	return *wordh;
}


UINT SetBit( UINT *wordh, UINT Bit_Num )
{
UINT offset = 1;

	if(Bit_Num >= 16)
		return *wordh;

	offset <<= Bit_Num;
	
	*wordh |= (UINT) offset;
	
	return *wordh;
}
plcbit GetBit( UINT wordh, UINT Bit_Num )
{
UINT offset = 1;
plcbit ret;

	if(Bit_Num >= 16)
		return 0;
	
	offset <<= Bit_Num;

	ret = ((wordh & offset) > 0)? 1: 0;
	
	//return (plcbit)(wordh & offset);
	return ret;
}


UINT ResetBit( UINT *wordh, UINT Bit_Num )
{
UINT inverse = 0xFFFF;
UINT offset = 1;

	if(Bit_Num >= 16)
		return *wordh;

	offset <<= Bit_Num;
	
	inverse ^= offset;
	
	*wordh &= inverse;
	return *wordh;
}
//////
UDINT ToggleBit32( UDINT *wordh, UINT Bit_Num )
{
	if(Bit_Num >= 32)
		return *wordh;

	UDINT offset = 1;
	offset <<= Bit_Num;

	if(*wordh & offset)
		ResetBit32(wordh,Bit_Num );
	else
		SetBit32(wordh,Bit_Num );
	
	return *wordh;
}

UDINT SetBit32( UDINT *wordh, UINT Bit_Num )
{
UDINT offset = 1;

	if(Bit_Num >= 32)
		return *wordh;

	offset <<= Bit_Num;
	
	*wordh |= (UDINT) offset;
	
	return *wordh;
}
plcbit GetBit32( UDINT wordh, UINT Bit_Num )
{
UDINT offset = 1;

	if(Bit_Num >= 32)
		return 0;

	offset <<= Bit_Num;
	
	return (wordh & offset);
}


UDINT ResetBit32( UDINT *wordh, UINT Bit_Num )
{
UDINT inverse = 0xFFFF;
UDINT offset = 1;

	if(Bit_Num >= 32)
		return *wordh;

	offset <<= Bit_Num;
	
	inverse ^= offset;
	
	*wordh &= inverse;
	return *wordh;
}
//////

USINT GetHighBit( UINT wordh )
{
UINT Answer;
USINT i;

	for (i=0;i<16;i++) {
		Answer = (wordh >> i) & 0x0001;
		if (Answer > 0)
			return i;
	}
	return 0;	
}	

USINT GetNibble( USINT x, BOOL ho )
{
	if (ho) 	return ((x>>4)	& 0x0F);
	else		return ((x>>0)	& 0x0F);		
}

USINT NibblesToByte(USINT o1, USINT o2)
{
	return (((o2<<4)  & 0xF0) | ((o1 & 0x0F)));
}
/* EOF */
#endif
