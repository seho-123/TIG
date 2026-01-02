/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: lib
 * File: matrixlib.c
 * Author: Christopher
 * Created: March 26, 2009
 *******************************************************************/


#ifndef _MATRIXLIB_C
#define _MATRIXLIB_C


#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

#include <..\lib\MatrixLib.h>

/********************/
double xABS(double a );

#define MIN_SPEED 0.00001

int createMatrix1dim( Matrix1dim_type *a ,int ord){
int i;	
UINT status;

	status = TMP_alloc(ord *sizeof(float),(void**) &a->ar);
	if(!a->ar)
		return 0;           
	     
	a->order = ord;
	return 1;

	
}

/********************/

int destroyMatrix1dim( Matrix1dim_type *a){

int i;	
UINT status;

	if((a->order == 0) || !a->ar)
		return 0;
	
	status = TMP_free(a->order *sizeof(float),(void**) a->ar);
	a->order = 0;
	a->ar = 0;
	return 1;
	
}

/********************/

int createMatrix2dim( Matrix2dim_type *a ,int ord){
int i;	
UINT status;
	status = TMP_alloc(ord *sizeof(float *),(void**) &a->ar);
	if(!a->ar || status)
		return 0;           
	     
	for(i=0; i<ord+1; i++)
	{ 
		status = TMP_alloc(ord *sizeof(float),(void**) &a->ar[i]);
		if(status)
			return 0;
	 }
	a->order = ord;
	return 1;	
}

/********************/

int destroyMatrix2dim( Matrix2dim_type *a){

int i;	
UINT status;

	if((a->order == 0) || !a->ar)
		return 0;

	for(i=0; i<a->order+1; i++)
	{ 
		status = TMP_free(a->order *sizeof(float),(void**) a->ar[i]);
		if(status)
			return 0;
	 }
	status = TMP_free(a->order *sizeof(float *),(void**) a->ar);
	if(status)
		return 0; 

	a->order = 0;
	a->ar = 0;
	return 1;
}

/***************************************************/
double xABS(double a ){
	if(a >= 0.0)
		return a;
	else
		return a*(-1.0);
}
double MPGEN(MPGEN_type *mp){

	
	if(mp->in_dest_val != mp->in_start_val ){
		
  		mp->intrn._start   = (double)mp->in_start_val;
  		mp->intrn._curpos  = mp->intrn._start;
  		mp->in_start_val   = mp->in_dest_val;
  		
		mp->intrn._dest    = ((double)mp->in_dest_val - mp->intrn._start)/2.0 + mp->intrn._start ;		
		if( (double)mp->in_dest_val < mp->intrn._start){
			mp->intrn._dir = -1;
			
			if(mp->acc == 0.0){
					mp->intrn._curvel = (double)mp->vel * (-1.0);
					mp->intrn._segm    = 14;
				}
			else{
				mp->intrn._segm    = 11;	
			}
						
		}
		else{
			mp->intrn._dir = 1;	
			if(mp->acc == 0.0){
				mp->intrn._curvel = (double)mp->vel ;
				mp->intrn._segm    = 4;
			}
			else{
				mp->intrn._segm    	= 1;				
			}
		}	
		
	}
	
		

	if(mp->Stop){

		if(mp->intrn._curvel > 0 )
			mp->intrn._segm = 3;  /* period decelartion */
		else if(mp->intrn._curvel < 0 )
			mp->intrn._segm = 13; /* period decelartion */

		else
			mp->Stop = 0;
		
	}
		
	
	switch(mp->intrn._segm){


			case 0:

			break;

			/*******************************************************************/

			/* period acceleration */
			case 1: 

				mp->intrn._curvel = mp->intrn._curvel + (double)mp->acc * (double)mp->tasktime ;	
				
				if((mp->intrn._curvel )>= (double)mp->vel){
					mp->intrn._dest = (double)mp->in_dest_val - (mp->intrn._curpos - mp->intrn._start) ;
					mp->intrn._segm = 2;
				}
				else if((mp->intrn._curpos) >= mp->intrn._dest ){
					
					mp->intrn._segm = 3;
				}

			break;

			/* period constance speed */
			case 2:
			
				if((mp->intrn._curpos ) >= mp->intrn._dest ){
				
					mp->intrn._segm = 3;
				}
				

			break;			

			/* period decelartion */
			case 3:

				mp->intrn._curvel = mp->intrn._curvel - (double)mp->acc * (double)mp->tasktime;	

				if(mp->Stop){
					if(mp->intrn._curvel <= 1.0e-05){
						mp->intrn._segm = 0;
						mp->Stop = 0;

						// New
						mp->in_dest_val = mp->intrn._curpos;
						mp->in_start_val = mp->intrn._curpos;
						
						mp->intrn._start = mp->intrn._curpos ;  
						mp->intrn._dest = mp->intrn._curpos ; 
												
				 	
					}
				}
				else if(mp->intrn._curvel < MIN_SPEED)
					mp->intrn._curvel = MIN_SPEED;

			break;

			/* period constance set speed  */
			case 4:
				
			break;

			/*******************************************************************/
			/* period decelartion */
			case 11: 

				mp->intrn._curvel = mp->intrn._curvel - (double)mp->acc * (double)mp->tasktime ;	
				
				if( (mp->intrn._curvel*(-1.0)) >= (double)mp->vel){
					mp->intrn._dest = (double)mp->in_dest_val - (mp->intrn._curpos - mp->intrn._start) ;
					mp->intrn._segm = 12;
				}
				else if(mp->intrn._curpos<= mp->intrn._dest ){
					
					mp->intrn._segm = 13;
				}

			break;

			/* period negative constance speed */
			case 12:
			
				if(mp->intrn._curpos <= mp->intrn._dest ){
				
					mp->intrn._segm = 13;
				}
				

			break;			

			/* period acceleration */
			case 13:

				mp->intrn._curvel = mp->intrn._curvel + (double)mp->acc * (double)mp->tasktime;	

				if(mp->Stop){
					if(mp->intrn._curvel >= (-1.0e-05)){
						mp->intrn._segm = 0;
						mp->Stop 		= 0;

						// New
						mp->in_dest_val = mp->intrn._curpos;
						mp->in_start_val = mp->intrn._curpos;
						
						mp->intrn._start = mp->intrn._curpos ;  
						mp->intrn._dest = mp->intrn._curpos ; 
					}
				}
				else if(mp->intrn._curvel > (-MIN_SPEED))
					mp->intrn._curvel = (-MIN_SPEED);

			break;

			/* period constance set speed  */
			case 14:				

			break;


			/* ***** */
			default:
			
				mp->intrn._segm = 0;
								
			break;

			
		
		}

		

	if(mp->intrn._segm){
		mp->intrn._curpos = mp->intrn._curpos + mp->intrn._curvel * (double)mp->tasktime;
		
		
		if(mp->intrn._dir>0){
			if(mp->intrn._curpos >= mp->in_dest_val){
				
			 	mp->intrn._curpos = (double)mp->in_dest_val;				 	
			 	mp->intrn._curvel = 0;
			 	mp->in_start_val = (float)mp->intrn._curpos;
			 	mp->intrn._segm = 0;
			 }
		 }
		 else{
		 	if(mp->intrn._curpos <= mp->in_dest_val){
				
			 	mp->intrn._curpos = (double)mp->in_dest_val;
			 	mp->intrn._curvel = 0;
			 	mp->in_start_val = (float)mp->intrn._curpos;
			 	mp->intrn._segm = 0;
			 }
	 	}		 	 

	}
	else{
		mp->intrn._curvel = 0;
		mp->intrn._curpos = (double)mp->in_dest_val;

	}
	
		
	

	mp->Done = !(mp->intrn._segm);

	mp->out_val = (float) mp->intrn._curpos;
	mp->cur_vel	= (float) mp->intrn._curvel;	
		
	return  mp->intrn._curpos;

}
	


/********************/

int forwardSubstitution(Matrix2dim_type *a) {
	int i, j, k,n, max;	
	float t;
	n = a->order;
	for (i = 0; i < n; ++i) {
		max = i;
		for (j = i + 1; j < n; ++j){
			if (a->ar[j][i] > a->ar[max][i])
				max = j;
        }

		for (j = 0; j < n + 1; ++j) {
			t = a->ar[max][j];
			a->ar[max][j] = a->ar[i][j];
			a->ar[i][j] = t;
		}

		for (j = n; j >= i; --j){
			for (k = i + 1; k < n; ++k){
				a->ar[k][j] -= a->ar[k][i]/a->ar[i][i] * a->ar[i][j];
			}
				
        }

	}
	return 1;
}

/********************/

int reverseElimination(Matrix2dim_type *a) {
	int i,n, j;
	n = a->order;
	for (i = n - 1; i >= 0; --i) {
		a->ar[i][n] = a->ar[i][n] / a->ar[i][i];
		a->ar[i][i] = 1;
		for (j = i - 1; j >= 0; --j) {
			a->ar[j][n] -= a->ar[j][i] * a->ar[i][n];
			a->ar[j][i] = 0;
		}
	}
	return 1;
}

int gauss(Matrix2dim_type *a) {	
    if(forwardSubstitution(a)==1)
		reverseElimination(a);
    
	else return 0;

	return 1;
}

/********************/

int createMatrix1dim_double( Matrix1dim_double *a ,int ord){
int i;	
UINT status;

	status = TMP_alloc(ord *sizeof(double),(void**) &a->ar);
	if(!a->ar)
		return 0;           
	     
	a->order = ord;
	return 1;	
}

/********************/

int destroyMatrix1dim_double( Matrix1dim_double *a){

int i;	
UINT status;

	if((a->order == 0) || !a->ar)
		return 0;
	
	status = TMP_free(a->order *sizeof(double),(void**) a->ar);
	a->order = 0;
	a->ar = 0;
	return 1;
}

/********************/

int createMatrix2dim_double( Matrix2dim_double *a ,int ord){
int i;	
UINT status;
	status = TMP_alloc(ord *sizeof(double *),(void**) &a->ar);
	if(!a->ar || status)
		return 0;           
	     
	for(i=0; i<ord+1; i++)
	{ 
		status = TMP_alloc(ord *sizeof(double),(void**) &a->ar[i]);
		if(status)
			return 0;
	 }
	a->order = ord;
	return 1;	
}

/********************/

int destroyMatrix2dim_double( Matrix2dim_double *a){

int i;	
UINT status;

	if((a->order == 0) || !a->ar)
		return 0;

	for(i=0; i<a->order+1; i++)
	{ 
		status = TMP_free(a->order *sizeof(double),(void**) a->ar[i]);
		if(status)
			return 0;
	 }
	status = TMP_free(a->order *sizeof(double *),(void**) a->ar);
	if(status)
		return 0; 

	a->order = 0;
	a->ar = 0;
	return 1;
}

/********************/

int forwardSubstitution_double(Matrix2dim_double *a) {
	int i, j, k,n, max;	
	double t;
	n = a->order;
	for (i = 0; i < n; ++i) {
		max = i;
		for (j = i + 1; j < n; ++j){
			if (a->ar[j][i] > a->ar[max][i])
				max = j;
        }

		for (j = 0; j < n + 1; ++j) {
			t = a->ar[max][j];
			a->ar[max][j] = a->ar[i][j];
			a->ar[i][j] = t;
		}

		for (j = n; j >= i; --j){
			for (k = i + 1; k < n; ++k){
				if(a->ar[i][i]!=0.0)
					a->ar[k][j] -= a->ar[k][i]/a->ar[i][i] * a->ar[i][j];
				else
					return 0;
			}
				
        }

	}
	return 1;
}

/********************/

int reverseElimination_double(Matrix2dim_double *a) {
	int i,n, j;
	n = a->order;
	for (i = n - 1; i >= 0; --i) {
		if(a->ar[i][i]!=0.0)
			a->ar[i][n] = a->ar[i][n] / a->ar[i][i];
		else
			return 0;
		a->ar[i][i] = 1;
		for (j = i - 1; j >= 0; --j) {
			a->ar[j][n] -= a->ar[j][i] * a->ar[i][n];
			a->ar[j][i] = 0;
		}
	}
	return 1;
}

/********************/

int gauss_double(Matrix2dim_double *a) {	
	
    if(forwardSubstitution_double(a)){
		if(reverseElimination_double(a))
			return 1;
    }
	return 0;
}


void ChebyshevNode_double(double x , double *t) {	
double x1,x2,x3,x4,x5,x6,x7,x8,x9;	

	x1 = x;
	x2 = x1*x1;
	x3 = x2*x1;
	x4 = x2*x2;
	x5 = x3*x2;
	x6 = x3*x3;
	x7 = x4*x3;
	x8 = x4*x4;
	x9 = x5*x4;
	
	t[0]= 1;
	t[1]= x1;
	t[2]= 2*x2- 1;
	t[3]= 4*x3 - 3*x1; 
	t[4]= 8*x4 - 8*x2 + 1; 
	t[5]= 16*x5 - 20*x3 + 5*x1; 
	t[6]= 32*x6 - 48*x4 + 18*x2 - 1; 
	t[7]= 64*x7 - 112*x5 + 56*x3 - 7*x; 
	t[8]= 128*x8 - 256*x6 + 160*x4 - 32*x2 + 1; 
	t[9]= 256*x9 - 576*x7 + 432*x5 - 120*x3 + 9*x; 


	return 1;
}


float PolinFunc(double x ,POLINOM_obj *p){
int i;
double y = 0.0;

	y = p->ax[0];
	
	for(i=1 ; i < p->order ; i++){
		y = y + x * p->ax[i];
		x = x * x ;
	}
	return (float)y;
}

float ChebyshevPolinFunc(double x ,POLINOM_obj *p){
int i;
double y = 0.0;
double cheb[10];

	y = p->ax[0];

	ChebyshevNode_double(x ,cheb );
	
	for(i=1 ; i < p->order ; i++){
		y = y + cheb[i] * p->ax[i];
	}
	return (float)y;
}



int calculate_spline (USINT type ,UINT n,  SETPOINT *setpoints,  SPLINE *spline)
{	
	/* IMPORTANT - n means number of points in path */
	/* SETPOINTS must be the same number but started from index 0 */
	
	double   cprev, deltax, deltay, deltayprim;
	int      i;
	

	if (n < 3 || !setpoints || !spline)
		return -1;	/* invalid input data */

	
	deltax = setpoints[n-1].x - setpoints[n-2].x;
	
	setpoints[n-2].yprim = (setpoints[n-1].y - setpoints[n-2].y)/deltax;
	
	for (i = n-2 ; i>= 0; i--)
	{		
		deltax = setpoints[ i+1 ].x - setpoints[ i].x;
		if(deltax ==0) return 0;
		
		deltay = setpoints[ i+1 ].y - setpoints[ i].y;

		if(type == 3){ /* Simple poin to po int spline */
			spline[i].b = deltay/deltax;
			spline[i].a = 0;
		}
		else{
			spline[i].a = (setpoints[i].yprim * deltax - deltay)/(deltax * deltax);
			spline[i].b = setpoints[i].yprim - (2.0 * spline[i].a * deltax);
			setpoints[i-1].yprim = spline[i].b;
		}
	
		spline[i].x = setpoints[i].x;
		spline[i].y = setpoints[i].y;

		
	}

	
	return 1; 
}



double spline_value (double x, UINT n, const SPLINE *spline)
{
	double   deltax;
	int      i,k;

	if (n < 3 || !spline)
		return 0.0;		/* invalid data */

	
	/* first find the right segment - binary search */
	
	for(i=1 ; i < (n-1) ; i++)
	{
		if (x < spline[i].x)
		{
			/* found */			
			break;
		}
	}
	k = i-1;
	
	/* now i points a spline segment that coefficients */
	/* will be used to calculate y */
	
	deltax = x - spline[k].x;
	return ((spline[k].a * deltax + spline[k].b) * deltax + spline[k].y);
}

int spline_index (double x, UINT n, const SPLINE *spline)
{	
	int      i , k;

	if (n < 3 || !spline)
		return 0;		/* invalid data */

	
	/* first find the right segment - binary search */	
	for(i=1 ; i < (n-1) ; i++)
	{
		if (x < spline[i].x)
		{
			/* found */
			
			break;
		}
		
	}
	k=i-1;
	return k;
/*	
	if(i== n-2) return n-3;
	else 		return i-1;
	*/
}



#endif

