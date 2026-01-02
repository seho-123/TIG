

/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: lib
 * File: matrixlib.h
 * Author: Christopher
 * Created: March 26, 2009
 *******************************************************************/

#ifndef _MATRIXLIB_H
#define _MATRIXLIB_H


#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

//#include<math.h>
//#include<stdlib.h>

typedef struct 
{
       int order;
       float *ar;        //for a 1-d array

}Matrix1dim_type;


typedef struct 
{
       int order;
       float **ar;        //for a 2-d array

}Matrix2dim_type;

typedef struct 
{
       int order;
       double *ar;        //for a 1-d array

}Matrix1dim_double;


typedef struct 
{
       int order;
       double **ar;        //for a 2-d array

}Matrix2dim_double;

typedef struct 
{
       int order;
	   USINT type;
       double ax[10];	   

}POLINOM_obj;


typedef struct 
{
       int order;
	   int cnt[10];
       double calibWght[10];
	   double measWght[10];	  
}LinearInput_obj;

typedef struct SETPOINT{

	double   x, y, yprim;
	
}SETPOINT;

typedef struct SPLINE{	

	double   a, b, y, x;
	
}SPLINE;

typedef struct MPGEN_type{
	double in_dest_val;
	double in_start_val;
	double out_val;
	double cur_vel;
	double vel;
	double acc;
	double tasktime;
	BOOL Done;
	BOOL Stop;

	struct intrn{
		double _in_val;
		BOOL _comp;
		SINT _dir;
		double _start;
		double _dest;
		double _curvel;
		double _curpos;
		double _half;
		USINT _segm;
	}intrn;

}MPGEN_type;


int createMatrix1dim( Matrix1dim_type *a ,int order);
int destroyMatrix1dim( Matrix1dim_type *a);
int createMatrix2dim( Matrix2dim_type *a ,int order);
int destroyMatrix2dim( Matrix2dim_type *a);
int forwardSubstitution(Matrix2dim_type *a);
int reverseElimination(Matrix2dim_type *a);
int gauss(Matrix2dim_type *a);

int createMatrix1dim_double( Matrix1dim_double *a ,int order);
int destroyMatrix1dim_double( Matrix1dim_double *a);
int createMatrix2dim_double( Matrix2dim_double *a ,int order);
int destroyMatrix2dim_double( Matrix2dim_double *a);
int forwardSubstitution_double(Matrix2dim_double *a);
int reverseElimination_double(Matrix2dim_double *a);
int gauss_double(Matrix2dim_double *a);
void ChebyshevNode_double(double x , double *t);	
float PolinFunc(double x ,POLINOM_obj *p);
float ChebyshevPolinFunc(double x ,POLINOM_obj *p);
double spline_value (double x, UINT n, const SPLINE *spline);
int spline_index (double x, UINT n, const SPLINE *spline);
int calculate_spline (USINT type ,UINT n, SETPOINT *setpoints,  SPLINE *spline);
double MPGEN(MPGEN_type *mpgen);


#endif


