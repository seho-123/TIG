#ifndef _COMPLEXLIB_C
#define _COMPLEXLIB_C


#include <..\Lib\complex.h>

Complex Compl(double re , double im)
{
Complex result;
	 result.re = re;
	 result.im = im;
	 return result;
}
PolComplex PolCompl(double r , double angle)
{
PolComplex result;
	 result.rad = r;
	 result.ang = angle;
	 return result;

}

Complex C_conj (Complex a)
{
	Complex result;
	result.re = a.re;
	result.im = -a.im;
	return(result);
}


Complex C_inv (Complex a)
{
	Complex result, num;
	num.re = 1.0;
	num.im = 0.0;
	result = C_div (num, a);
	return(result);
}

Complex C_add (Complex a, Complex b)
{
	Complex result;
	result.re = a.re + b.re;
	result.im = a.im + b.im;
	return(result);
}

Complex C_sub (Complex a, Complex b)
{
	static Complex result;
	result.re = a.re - b.re;
	result.im = a.im - b.im;
	return(result);
}

Complex C_mul (Complex a, Complex b)
{
	Complex result;
	   result.re = a.re*b.re - a.im*b.im;
	   result.im = a.re*b.im + b.re*a.im;
	   return(result);
}
Complex C_div (Complex a, Complex b)
{
	Complex c, result, num;
	double denom;

	c = C_conj(b);
	num = C_mul (a, c);
	denom = b.re*b.re + b.im*b.im + 1.2e-63;  /*to prevent division by zero*/

	result.re = num.re / denom;
	result.im = num.im / denom;
	return(result);
}

/** Hypotenuse **/
Complex C_Hyp(Complex a, double offset){
	PolComplex lp;
	
	lp.rad = fabs(offset);
	if(offset > 0)
		lp.ang = C_ang (a)+M_PI_2 ;
	else
		lp.ang = C_ang (a)-M_PI_2 ;
	
	return C_add(a,P2C(lp));
}

/*** Leg **/
Complex C_Leg(Complex a, double offset){
	PolComplex lp;
	double rad;

	if(offset > 0)
		lp.ang = C_ang(a) + asin((double) fabs(offset) / (C2P(a).rad + 1e-99));
	else
		lp.ang = C_ang(a) - asin((double) fabs(offset) / (C2P(a).rad +  1e-99));

	rad = C2P(a).rad;
	if(rad > offset)
		lp.rad = sqrt( (double)(C2P(a).rad * C2P(a).rad)  - (offset * offset)); 
	else
		lp.rad = 1.2e-63; /*to prevent division by zero*/

	return P2C(lp);
	
}

Complex C_para (Complex a,Complex b)
{
	Complex result, num, denom;

	num = C_mul (a, b);
	denom = C_add(a, b);

	result = C_div (num, denom);
	return(result);
}


 double C_ang (Complex a)
 {
	double result;
	result =  atan2 ( a.im,  a.re + 1e-99);
	return (result);
 }

 double C_rad (Complex a)
 {
double result;
	result = sqrt (  (a.re*a.re + a.im*a.im));
	return (result);
 }

 PolComplex C2P (Complex a){
	PolComplex result;

	result.rad = C_rad(a);
	result.ang = C_ang(a);	
	return (result);
 
 }
Complex P2C (PolComplex a){
	Complex result;
	result.im = a.rad * sin(a.ang);
	result.re	= a.rad *  cos( a.ang);	
	return (result);
}

Complex C_Equl (Complex a){
Complex result;
	result.re = a.re;
	result.im = a.im;
	return result;	
}
PolComplex P_Equl (PolComplex a){
PolComplex result;
	result.ang = a.ang;
	result.rad = a.rad;
	return result;	
}
PolComplex GetElipPoint(double a, double b, double ang){
PolComplex result;
double a2,b2,tg2, x,x2,y2;
	a2=a*a;
	b2=b*b;
	tg2=pow(tan(ang),2.0);
	x2 = (a2*b2)/(b2*tg2 +a2);
	y2 = x2 * tg2;
	result.ang = ang;
	result.rad =sqrt(x2+y2); 

return result;	
}
Complex C_Scale(Complex a, double b)
{
	Complex result;
	   result.re = a.re*b;
	   
	   result.im = a.im*b;
	   return(result);
}
 PolComplex P_Scale (PolComplex a, double b){
	PolComplex result;

	result.rad = a.rad * b ;
	result.ang = a.ang;	
	return (result);
 
 }

 double Rad2Deg(double rad)
{
	return rad * 57.2958;
}
double Deg2Rad(double deg)
{
	return deg * 0.01745329;
}

#endif
/* EOF */
