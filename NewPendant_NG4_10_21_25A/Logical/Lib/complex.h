#ifndef _COMPLEXLIB_H
#define _COMPLEXLIB_H


/*** DEPENDANCIES:
*********************************************************/
#include <bur/plc.h>
#include <bur/plctypes.h>
#include <math.h>

/* TYPES:
*********************************************************/

typedef struct Complex
{
	double re;
	double im;
} Complex;

typedef struct PolComplex
{
	double rad;
	double ang;
} PolComplex; 
/* PROTOTYPES:
*********************************************************/


Complex Compl(double re , double im);
PolComplex PolCompl(double r , double angle);

double Rad2Deg(double rad);
double Deg2Rad(double deg);



double C_rad (Complex a);
double C_ang (Complex a);

PolComplex C2P(Complex a);
Complex P2C (PolComplex a);
Complex C_conj ( Complex a);
Complex C_inv (Complex a);
Complex C_add (Complex a, Complex b);
Complex C_sub (Complex a, Complex b);
Complex C_mul (Complex a, Complex b);
Complex C_div (Complex a, Complex b);
Complex C_Hyp(Complex a, double offset);
Complex C_Leg(Complex a, double offset);
Complex C_para (Complex a, Complex b);
Complex C_Equl (Complex a);
PolComplex P_Equl (PolComplex a);
PolComplex GetElipPoint(double a, double b, double ang);
 PolComplex P_Scale (PolComplex a, double b);
 Complex C_Scale(Complex a, double b);
/*********************************************************/

#endif
