	/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Package: Lib
 * File: KinematicLib.c
 * Author: wsi
 * Created: October 12, 2012
 *******************************************************************/

#ifndef _KINEMATICLIB_C
#define _KINEMATICLIB_C


#include <..\Lib\kinematiclib.h>

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <..\Lib\Complex.c> 
#include <..\lib\MatrixLib.c>
///#include <..\math.c>



DINT GetCircleTCP(CIRCLE_PATH_typ *path ){
Complex A;
PolComplex P;
double angle;


	if(!path)
		return -1;

	if(!path->pTCP)
		return -2;

	if(!path->Active)
		return -3;


	switch(path->xCmd){


		case 0:

			path->linMoveGEN.enable = 0;
			path->angMoveGEN.enable = 0;
			path->radMoveGEN.enable = 0;

		break;


		

		case 1:	// Start RUN angle and radius	


			
			A = C_sub(path->pTCP->TCP ,  path->Center);
			P = C2P(A);			
			path->ang = fmod(P.ang, M_TWOPI);		
			path->radius = P.rad;	


			/* homing */
			path->radMoveGEN.enable = 1;
			path->radMoveGEN.Init = 0;
			path->radMoveGEN.Brake = 0;
			path->radMoveGEN.Stop= 1; /* stop everything */
			path->radMoveGEN.s_home= path->radius; /* homing position */
			path->radMoveGEN.Set_home= 1;
			HCRSPF(&path->radMoveGEN);

	
			/* Start Move */
			path->radMoveGEN.Init= 1;
			path->radMoveGEN.s_end= path->RadiusTarget;
			HCRSPF(&path->radMoveGEN);	

			path->linMoveGEN.enable = 0;
			path->angMoveGEN.enable = 0;

			if(path->VelMode){
				
				path->linMoveGEN.enable = 1;
				path->linMoveGEN.Init = 0;
				path->linMoveGEN.Brake = 0;
				path->linMoveGEN.Stop= 1; /* stop everything */
				path->linMoveGEN.s_home= 0; /* homing position */
				path->linMoveGEN.Set_home= 1;
				HCRSPF(&path->linMoveGEN);

				path->linMoveGEN.Init= 1;
				path->linMoveGEN.s_end= 10.0e6 * path->RotDir;
				HCRSPF(&path->radMoveGEN);	

			}
			else {
				
				path->angMoveGEN.enable = 1;
				path->angMoveGEN.Init = 0;
				path->angMoveGEN.Brake = 0;
				path->angMoveGEN.Stop= 1; /* stop everything */
				path->angMoveGEN.s_home= path->ang; /* homing position */
				path->angMoveGEN.Set_home= 1;
				HCRSPF(&path->angMoveGEN);

				path->angMoveGEN.Init= 1;
				path->angMoveGEN.s_end= 10.0e6 *  path->RotDir;
				HCRSPF(&path->radMoveGEN);	

			}
			

			if(path->VelMode)
				path->ang += path->linMoveGEN.v_set/path->radMoveGEN.s_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 
			else
				path->ang += path->angMoveGEN.v_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 

			//path->ang = fmod(P.ang, M_TWOPI);	
			
			P = PolCompl(path->radMoveGEN.s_set ,path->ang);
			path->pTCP->TCP = C_add(path->Center , P2C(P));					
			path->radius = P.rad;	


			path->xCmd = 10;

	
	
		break;	

		case 2:	// Start New radius Target 

			/* Start Move */
			path->radMoveGEN.Init= 1;
			path->radMoveGEN.s_end= path->RadiusTarget;

			HCRSPF(&path->radMoveGEN);
			HCRSPF(&path->angMoveGEN);
			HCRSPF(&path->linMoveGEN);

			if(path->VelMode)
				path->ang += path->linMoveGEN.v_set/path->radMoveGEN.s_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 
			else
				path->ang += path->angMoveGEN.v_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 

			//path->ang = fmod(P.ang, M_TWOPI);	
			
			P = PolCompl(path->radMoveGEN.s_set ,path->ang);
			path->pTCP->TCP = C_add(path->Center , P2C(P));					
			path->radius = P.rad;	

			path->xCmd = 10;

	
	
		break;	


		case 3: // Start STOPPING
		
			path->radMoveGEN.Brake = 1;  /* stop everything */
			path->angMoveGEN.Brake = 1; /* stop everything */
			path->linMoveGEN.Brake = 1; /* stop everything */

			HCRSPF(&path->radMoveGEN);
			HCRSPF(&path->angMoveGEN);
			HCRSPF(&path->linMoveGEN);

			path->xCmd = 10;

		break;	

		case 9: // Refresh param

			path->angMoveGEN.Init= 1;
			path->radMoveGEN.Init= 1;
			path->linMoveGEN.Init= 1;
			
			path->xCmd = 10;
		
		
		case 10: // Move progress
			
			HCRSPF(&path->radMoveGEN);
			HCRSPF(&path->angMoveGEN);
			HCRSPF(&path->linMoveGEN);
			
			
			if(path->VelMode)
				path->ang += path->linMoveGEN.v_set/path->radMoveGEN.s_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 
			else
				path->ang += path->angMoveGEN.v_set * (float)path->linMoveGEN.Internal.Ts / 1.0e6; 

			//path->ang = fmod(P.ang, M_TWOPI);	
			
			P = PolCompl(path->radMoveGEN.s_set ,path->ang);
			path->pTCP->TCP = C_add(path->Center , P2C(P));					
			path->radius = P.rad;	

			if((!path->angMoveGEN.enable || path->angMoveGEN.Reached) 
				&& (!path->linMoveGEN.enable || path->linMoveGEN.Reached))
					path->xCmd = 0;
				
		break;	


		
		default:
			path->xCmd = 0;
		break;	
			


	}
	

	
return 1;	
	
}

DINT GetLineTCP(LINE_PATH_typ *path ){
Complex A;
PolComplex P;


	if(!path)
		return -1;

	if(!path->pTCP)
		return -2;
	

	switch (path->xCmd){


		case 0 :


		break;	

		case 1: // Start RUN


			
			memcpy(&path->START , &path->pTCP->TCP , sizeof(Complex));

			A = C_sub(path->DEST , path->START);
			P = C2P(A);

			path->angle = P.ang;
			
			/* homing */
			path->MoveGen.Stop= 1; /* stop everything */
			path->MoveGen.s_home= 0; /* homing position */
			path->MoveGen.Set_home= 1;
			HCRSPF(&path->MoveGen);

			/* Start Move */
			path->MoveGen.Init= 1;
			path->MoveGen.s_end= P.rad;
			HCRSPF(&path->MoveGen);

			path->pTCP->TCP = C_add(path->START , P2C(PolCompl(path->MoveGen.s_set ,path->angle)));

			path->xCmd = 10;


		break;	


		case 9:

			path->MoveGen.Init= 1;
			path->xCmd = 10;
			
	
		case 10: // Run Progress

			HCRSPF(&path->MoveGen);

			path->pTCP->TCP = C_add(path->START , P2C(PolCompl(path->MoveGen.s_set ,path->angle)));

			if(path->MoveGen.Reached){
				
				memcpy(&path->pTCP->TCP , &path->DEST , sizeof(Complex));				

				path->xCmd = 0;
			}
			

		break;	


		default:
			path->xCmd = 0;
		break;			


	}


	
	return (DINT)path->MoveGen.Reached ;
	
}






DINT GetTCP( PATH_typ *path)
{
DINT ret = 0;
double a;
Complex c;

	if(!path->pTCPx)
		return -100;

	if((path->msc.curSegm.Vin + path->msc.curSegm.Vout) == 0.0)	
			return -101;

	if(path->msc.curSegm.data[LEN] <= 0.0)
			return -102;

	if(path->msc.state > 0)
		path->msc.t += path->deltaT;

	
	if(path->newSegment){
		path->newSegment = 0;

		


		// ??????????????????????????
		//path->msc.curSegm.Vin = path->msc.v;


		path->msc.len = path->msc.curSegm.data[LEN];
		path->msc.angle = path->msc.curSegm.data[ANGLE]; 		


		switch (path->msc.curSegm.Type ){			

			// Straight line - len & angle
			case LINE_RELATIVE:	


				// time of running line
				path->msc.polynom[1].x = (double)((path->msc.len * 2)/ (path->msc.curSegm.Vin + path->msc.curSegm.Vout)) ;
				
				path->msc.polynom[1].y = path->msc.len; // Lendth of segment

				path->msc.polynom[1].b = path->msc.curSegm.Vin;
				path->msc.polynom[1].a = (path->msc.curSegm.Vout - path->msc.curSegm.Vin)/2 /path->msc.polynom[1].x;
			
				
			
					
				path->msc.startPoint.re = path->pTCPx->TCP.re;
				path->msc.startPoint.im = path->pTCPx->TCP.im;

				path->msc.curAngle += path->msc.angle;

				path->msc.rad = 0;

				path->msc.endPoint.re = path->pTCPx->TCP.re + path->msc.len * cosf(path->msc.curAngle) ;
				path->msc.endPoint.im = path->pTCPx->TCP.im + path->msc.len * sinf(path->msc.curAngle) ;
					
					
				

			break;

			//ARC - len & angle
			case ARC_LEN_ANGLE:
			case ARC_RAD_ANGLE:
			case ROT_ANGLE:

				if((path->msc.angle == 0.0)|| (path->msc.len == 0))
					return -5;


				if(path->msc.curSegm.Type == ARC_RAD_ANGLE){				
					path->msc.rad = fabs(path->msc.len); 
					path->msc.len = path->msc.rad * fabs(path->msc.angle);
				}
				else 
					path->msc.rad = path->msc.len / fabs(path->msc.angle);
				
				// time of running line
				path->msc.polynom[1].x = (double)((fabs(path->msc.angle) * 2)/ (path->msc.curSegm.Vin/path->msc.rad + path->msc.curSegm.Vout/path->msc.rad)) ;
				
				path->msc.polynom[1].y = path->msc.angle; // Lendth of segment in angle

				path->msc.polynom[1].b = path->msc.curSegm.Vin/path->msc.rad;
				path->msc.polynom[1].a = (path->msc.curSegm.Vout/path->msc.rad - path->msc.curSegm.Vin/path->msc.rad)/2 /path->msc.polynom[1].x;	
				

			
				
					
				path->msc.startPoint.re = path->pTCPx->TCP.re;
				path->msc.startPoint.im = path->pTCPx->TCP.im;


				if(path->msc.angle >= 0)
					path->msc.cirDir = 1.0;
				else
					path->msc.cirDir = -1.0;
				
				path->msc.centrPoint = C_add(P2C(PolCompl(path->msc.rad ,path->msc.curAngle + M_PI_2 * path->msc.cirDir)),	path->msc.startPoint ) ;
				path->msc.endPoint = C_add(P2C(PolCompl(path->msc.rad ,path->msc.curAngle + path->msc.angle  - M_PI_2 * path->msc.cirDir)),	path->msc.centrPoint ) ;

				path->msc.startAngle = path->msc.curAngle - M_PI_2 * path->msc.cirDir ;
			

								
					
				

			break;


			case JOG_LINE:
			case JOG_ROT:

				path->msc.state = 0;

			break;	
			

		
		}
		
		path->msc.t = 0;		
		
		path->msc.state = 1;	
		
		
		

	}

	switch(path->msc.state){

		case 0:

			ret = 0;
						
		break;	


		case 1:

			if( (path->msc.t >= 0) && (path->msc.t < path->msc.polynom[1].x)){ 
				path->msc.x = path->msc.polynom[1].a * path->msc.t * path->msc.t + path->msc.polynom[1].b * path->msc.t;
				path->msc.v = path->msc.polynom[1].a * 2 * path->msc.t + path->msc.polynom[1].b;
				
				ret = 0;
			}
			else if((path->msc.t >=  path->msc.polynom[1].x)){

				path->msc.x = path->msc.polynom[1].a * path->msc.polynom[1].x *path->msc.polynom[1].x + path->msc.polynom[1].b * path->msc.polynom[1].x;
				path->msc.v = path->msc.polynom[1].a * 2 * path->msc.polynom[1].x + path->msc.polynom[1].b;

				ret = 1;
				path->msc.state = 2;
				
			}
			else if(path->msc.t < 0){
				path->msc.x = 0;
				path->msc.v = path->msc.polynom[1].b;		
				
				ret = -1;
				path->msc.state = 3;
			}	

		break;	


		case 2:

			ret = 1;

		break;


		case 3:

			ret = -1;

		break;	


	}

	

	switch(path->msc.curSegm.Type){


		// Straight line
		case LINE_RELATIVE:		

			path->pTCPx->TCP.re = path->msc.startPoint.re + path->msc.x * cos(path->msc.curAngle) ;
			path->pTCPx->TCP.im = path->msc.startPoint.im + path->msc.x * sin(path->msc.curAngle);	

			//path->pTCPx->a =  path->msc.curAngle  - path->pTCPx->ToolAngleSP * path->msc.curSegm.Active;	
			path->pTCPx->a =  path->msc.curAngle  + path->pTCPx->ToolAngleSP;


		break;	

		// Arc
		case ARC_LEN_ANGLE:
		case ARC_RAD_ANGLE:

	
			path->msc.curAngle = path->msc.startAngle + path->msc.x * path->msc.cirDir ;
			
			c = C_add (path->msc.centrPoint , P2C(PolCompl(path->msc.rad,path->msc.curAngle )));
			
			path->msc.curAngle += M_PI_2 * path->msc.cirDir;
			
			//path->pTCPx->a =  path->msc.curAngle  - path->pTCPx->ToolAngleSP * path->msc.curSegm.Active;	
			path->pTCPx->a =  path->msc.curAngle  + path->pTCPx->ToolAngleSP ;

			path->pTCPx->TCP = c; 

			
			path->msc.v = path->msc.v * path->msc.rad;
			

		break;	


		// Rotation
		case ROT_ANGLE:

			path->msc.curAngle = path->msc.startAngle + path->msc.x * path->msc.cirDir ;			
			
			path->msc.curAngle += M_PI_2 * path->msc.cirDir;

			path->pTCPx->a =  path->msc.curAngle  + path->pTCPx->ToolAngleSP ;
			
			//path->pTCPx->a =  path->msc.curAngle  - path->pTCPx->ToolAngleSP * path->msc.curSegm.Active;
		

		break;	


		// Jog Line //
		case JOG_LINE:

			path->msc.x = path->msc.curSegm.Vin *  path->deltaT; 

			path->pTCPx->TCP.re +=  path->msc.x * cos(path->msc.curSegm.data[ANGLE]) ;
			path->pTCPx->TCP.im +=  path->msc.x * sin(path->msc.curSegm.data[ANGLE]); 
			
			//path->pTCPx->a =  path->msc.curAngle  + path->pTCPx->ToolAngleSP;

	
		break;	

		// Jog Rotation //
		case JOG_ROT:

			path->msc.x = path->msc.curSegm.Vin *  path->deltaT;
			
			path->pTCPx->a += path->msc.x;

	
		break;		


 
	}	

	path->pTCPx->a = fmod(path->pTCPx->a +  M_TWOPI  , M_TWOPI);
	
	return ret;
}



DINT GetATriangleAngle(double a , double b, double c , double *out)
{
	double x ;

	double aa = (double)a*(double)a;
	double bb = (double)b*(double)b;
	double cc = (double)c*(double)c;

	x = aa + bb  - cc;

	//if(a <= (b +c)){	
	//if(c <= (a + b)){	
	if(b <= (a + c)){	
		*out = acos( x / (2*a*b));
		return 1;
	}
	else{
		*out = 0;
		return -1;
	}
		
}




int Invers3R(ASSEM_3R_typ *as){
double inv = 1.0;
Complex B , AB;
double a , c;
USINT i;
double delta;
double old_ya;
BOOL dir;


	old_ya = as->Mtrs.ya[0];

	if(!as->TCPx.OutSide)
		inv = -1.0;	
	
	B = C_add(as->TCPx.TCP , C_Scale(P2C(PolCompl(as->R3 + as->TCPx.avc , as->TCPx.a )), inv));	
	

	as->Test_Point = B;

	c = C2P(B).rad;


	
	if(GetATriangleAngle(as->R1 , c , as->R2 , &a) ==1){

		if(as->TCPx.Gibbous)
			as->Mtrs.ya[0] =  C2P(B).ang + a ;
		else
			as->Mtrs.ya[0] =  C2P(B).ang - a;
	}
	else {

		memcpy(&as->TCPx , &as->oldTCPx , sizeof(TCP_typ));
		return -1;

	}	

		
	if(GetATriangleAngle(as->R1 , as->R2 , c , &a) == 1){

		if(fabs(a) < (M_PI))
			as->Mtrs.ya[1] = a;
		
		else{

			as->Mtrs.ya[0] = old_ya;	
			memcpy(&as->TCPx , &as->oldTCPx , sizeof(TCP_typ));
			return -2;

		}	
			

	}	
	else {

		as->Mtrs.ya[0] = old_ya;	
		memcpy(&as->TCPx , &as->oldTCPx , sizeof(TCP_typ));
		return -2;

	}	

	if(!as->TCPx.Gibbous)
		as->Mtrs.ya[1] = as->Mtrs.ya[1] * (-1); 
		
	
	AB = C_sub(B , P2C(PolCompl(as->R1,as->Mtrs.ya[0]))); 



	if(as->TCPx.OutSide)	
		as->Mtrs.ya[2] = as->TCPx.a - C2P(AB).ang;	
	else
		as->Mtrs.ya[2] = as->TCPx.a - M_PI - C2P(AB).ang;


		
	memcpy(&as->oldTCPx , &as->TCPx , sizeof(TCP_typ));

	return 1;	
	
}



double  Forward3R(ASSEM_3R_typ *as){

Complex  AC , AB , BC;
double angleC;

double ret;

	WatchForGibbous(as);

	AB = P2C(PolCompl(as->R1 , as->Mtrs.ya[0]));

	BC = P2C(PolCompl(as->R2 , M_PI + as->Mtrs.ya[0] +  as->Mtrs.ya[1]));

	AC =C_add(AB , BC);	

	angleC = as->Mtrs.ya[0] + as->Mtrs.ya[1] + as->Mtrs.ya[2] ;	

	as->TCPx.TCP = C_add(AC ,P2C(PolCompl(as->R3 + as->TCPx.avc , angleC )));

	as->TCPx.a = fmod(angleC + M_TWOPI , M_TWOPI);

	//as->TCPx.a = angleC;
	
	if(as->TCPx.OutSide)
		ret = angleC + as->TCPx.ToolAngleSP  ;			
	else
		ret = angleC - as->TCPx.ToolAngleSP ;

	//return fmod(ret , M_TWOPI);


	return ret;

}
void WatchForGibbous(ASSEM_3R_typ *as){

	if(!as)
		return;

	if(	as->Mtrs.ya[1] > M_PI){
		as->Mtrs.ya[1] = (-M_PI + fmod(as->Mtrs.ya[1] , M_PI));
		as->TCPx.Gibbous = 0;
	}
	else if(as->Mtrs.ya[1] < (-M_PI)){
		as->Mtrs.ya[1] = (M_PI + fmod(as->Mtrs.ya[1] , M_PI));
		as->TCPx.Gibbous = 1;
	}	

}


int CircleOverTriangle(Complex *pIn , Complex *cent , double *rad)
{
double a1, a2 , b1, b2 ,c1 , c2 , d , e;

	a1 = pIn[0].re - pIn[1].re;
	b1 = pIn[0].im - pIn[1].im;
	c1 = pIn[0].re * pIn[0].re + pIn[0].im * pIn[0].im - pIn[1].re * pIn[1].re - pIn[1].im * pIn[1].im;
	c1 = c1/2;

	a2 = pIn[1].re - pIn[2].re;
	b2 = pIn[1].im - pIn[2].im;
	c2 = pIn[1].re * pIn[1].re + pIn[1].im * pIn[1].im - pIn[2].re * pIn[2].re - pIn[2].im * pIn[2].im;
	c2 = c2/2;

	if( (a1 == 0 && b1 == 0) ||
		(a2 == 0 && b2 == 0))

		return -1;  // two points are the same


	if( a1 != 0){	

	
		/////////////////////////

		d = c2- (c1*a2/a1);
		e = b2 - (b1*a2/a1);


		if(e != 0){		

			cent->im = d/e;

			cent->re = (c1 - cent->im*b1)/a1;
		}
		else
			return -2; // Points are on line

		/////////////////////////

	}
	else if( b1 != 0){


		/////////////////////////

		d = c2- (c1*b2/b1);
		e = a2 - (a1*b2/b1);

		
		if(e!= 0){
			
			cent->re = d/e;

			cent->im = (c1 - cent->re*a1)/b1;
		}
		else
			return -2; // Points are on line

		/////////////////////////


	}
	
	

	a1 = pIn[0].re - cent->re;
	b1 = pIn[0].im - cent->im;
	
	*rad = sqrt(a1*a1 + b1*b1); 
	
	return 1;
}



#endif
