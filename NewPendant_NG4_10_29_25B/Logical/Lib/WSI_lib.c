/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Package: Lib
 * File: WSI_lib.c
 * Author: ckaczkowski
 * Created: June 18, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif






#ifndef _WSITYPES_C
#define _WSITYPES_C

#define CALIB_ONE2ONE			0
#define CALIB_2POINTS 			1
#define CALIB_2POINTS_4OFF 		2
#define CALIB_2POINTS_INV 		3
#define CALIB_2POINTS_4OFF_INV	4

/*
//Bubble sort 
 for(outer=0;outer<SIZE-1;outer++)
 {
	 for(inner=outer+1;inner<SIZE;inner++)
	 {
		 if(bubble[outer] > bubble[inner])
		 {
			 temp=bubble[outer];
			 bubble[outer] = bubble[inner];
			 bubble[inner] = temp;
		 }
	 }
 }
*/



//#define NULL 0


/* Prototyping of functions and function blocks */


float Scal(float x, AzzScal_typ* scale);
float LimScal(float x, AzzLimScal_typ* scale);

DINT Errors(ERROR_Mgr_typ *mgr , char type , unsigned short indx , unsigned long val);

DINT AckAllErrors(ERROR_Mgr_typ *mgr);
DINT KK_AckAllErrors(ERROR_Mgr_typ *mgr);


DINT StateVectorSrv(StateVectorProxy_typ *vect , UINT thread);

UINT Bool2Uint(plcbit in);
plcbit Uint2Bool(UINT in);

char* UDINT2ASC(UDINT val);
UDINT ASC2UDINT(char *p);

INT ABS_INT(INT x);

float getFromCalib(float x , Calib_Param_typ* calib , UINT type);

static UDINT hash_of_binary_array(char *key, int len);

///////////////////////////////////////////////////////
//////   Functions from Paul
void AzzLinSpline(AzzLinSpline_typ *lcr);
void AzzSlimPID(AzzSlimPID_typ *lcr);
void AzzRamp(AzzRamp_typ *lcr);
void AzzMovAvgFlt(AzzMovAvgFlt_typ *lcr) ;
void AzzLimScal(AzzLimScal_typ *lcr);
void AzzScal(AzzScal_typ *lcr);
void AzzTON(AzzTON_typ *t); 


void AzzLinSpline(AzzLinSpline_typ *lcr){
UINT outer , inner;
INT i;
float tempX, tempY;
float slope , delta;

	if(!lcr->pNodeVec)
		return;

	if(lcr->NbNodes < 2)
		return;

	//////////////////////////////////////////////

	if(lcr->Invert){

		if(lcr->x < lcr->pNodeVec[0].Y)
			lcr->y = lcr->pNodeVec[0].X;
		else if(lcr->x > lcr->pNodeVec[lcr->NbNodes-1].Y)
			lcr->y = lcr->pNodeVec[lcr->NbNodes-1].X;

		else{	
		
			i = lcr->NbNodes-2;
			while((lcr->x < lcr->pNodeVec[i].Y) && (i > 0) ) { i--; }
			
			lcr->_i = i;
			
			delta = lcr->pNodeVec[i+1].Y - lcr->pNodeVec[i].Y;
			
			if(delta != 0.0)
				slope = (lcr->pNodeVec[i+1].X - lcr->pNodeVec[i].X)/delta;
			else
				slope =0;
			
			lcr->y = lcr->pNodeVec[i].X + slope * (lcr->x - lcr->pNodeVec[i].Y);

		}

	}
	else{	

		if(lcr->x < lcr->pNodeVec[0].X)
			lcr->y = lcr->pNodeVec[0].Y;
		else if(lcr->x > lcr->pNodeVec[lcr->NbNodes-1].X)
			lcr->y = lcr->pNodeVec[lcr->NbNodes-1].Y;

		else{			

			i = lcr->NbNodes-2;
			while((lcr->x < lcr->pNodeVec[i].X) && (i > 0) ) { i--; }

			lcr->_i = i;
			
			delta = lcr->pNodeVec[i+1].X - lcr->pNodeVec[i].X;
			
			if(delta != 0.0)
				slope = (lcr->pNodeVec[i+1].Y - lcr->pNodeVec[i].Y)/delta;
			else
				slope =0;
			
			lcr->y = lcr->pNodeVec[i].Y + slope * (lcr->x - lcr->pNodeVec[i].X);

		}

	}
	
	///////////////////////////////////////////////
	if(lcr->Update){
		lcr->Update = 0;


		//Bubble sort 
		 for(outer=0;outer<lcr->NbNodes-1;outer++)
		 {
			 for(inner=outer+1;inner<lcr->NbNodes;inner++)
			 {

			 	if(lcr->Invert){	
					
					 if(lcr->pNodeVec[outer].Y > lcr->pNodeVec[inner].Y)
					 {
						 tempX=lcr->pNodeVec[outer].X;
						 tempY=lcr->pNodeVec[outer].Y;
						 
						 lcr->pNodeVec[outer].X = lcr->pNodeVec[inner].X;
						 lcr->pNodeVec[outer].Y = lcr->pNodeVec[inner].Y;

						 
						 lcr->pNodeVec[inner].X = tempX;
						 lcr->pNodeVec[inner].Y = tempX;
						 
					 }	 

			 	}
				else{

					if(lcr->pNodeVec[outer].X > lcr->pNodeVec[inner].X)
					 {
						 tempX=lcr->pNodeVec[outer].X;
						 tempY=lcr->pNodeVec[outer].Y;
						 
						 lcr->pNodeVec[outer].X = lcr->pNodeVec[inner].X;
						 lcr->pNodeVec[outer].Y = lcr->pNodeVec[inner].Y;

						 
						 lcr->pNodeVec[inner].X = tempX;
						 lcr->pNodeVec[inner].Y = tempX;
						 
					 }	 


				}
			 }
		 }
		


	}


}


void AzzTON(AzzTON_typ *t) {
  if (t->IN) {
    // Check for rising edge
    if (!t->M) {
      t->StartTime = clock_ms();
    }

    // Determine elapsed time
    t->ET = clock_ms() - t->StartTime;

    // Check if the elapsed time has reached delay time
    if (t->ET >= t->PT) {
      t->Q = 1;
      t->ET = t->PT;
    } else {
      t->Q = 0;
    }

  } else {
    t->Q = 0;
    t->ET = 0;
  }

  // Set M to IN to allow checking for rising edge
  t->M = t->IN;
}


void AzzRamp(AzzRamp_typ *lcr) {
float x = lcr->x, y = lcr->y_old;
float dt;
DINT time , deltaT;


	lcr->status = 0;


	if ( !lcr->enable ) {

		lcr->timestore1 = AsIOTimeStamp();
		lcr->status = 65534;
		return;

	}
		
	// Check parameters
	  // rate up and rate down have to be greater than zero
	if ( lcr->dy_up <= 0 || lcr->dy_down <= 0 ) {
	  lcr->status = 12034;
	  return;
	}


	if ( lcr->set ) { // Set default value

		lcr->set = 0;

		y = lcr->y_set;

		lcr->y_old = y;

		lcr->y = y;

		lcr->timestore1 = AsIOTimeStamp();

	}
	else {
		// Process Ramp

		time = AsIOTimeStamp();
		deltaT = (time- lcr->timestore1); 	
		lcr->timestore1 = time;

		if(deltaT > 0 && (deltaT < 0x4FFFFFFF))
			dt = (float)(deltaT) / 1000000.0;
		else
			dt = 0.0;
		
		
		lcr->dt = dt;

		/*
		// Calculate new value
		if ( x > y && y > 0 ) {
			y = lcr->y_old + ( dt * lcr->dy_up );
			if(y > x)
				y = x;
			
		} else if ( x < y && y < 0 ) {
			y = lcr->y_old - ( dt * lcr->dy_up );
			if(y < x)
				y = x;
			
		} else if ( x < y && y > 0 ) {
			y = lcr->y_old - ( dt * lcr->dy_down );
			if(y < x)
				y = x;
			
		} else if ( x > y && y < 0 ) {
			y = lcr->y_old + ( dt * lcr->dy_down );
			if(y > x)
				y = x;
		}
		*/

		if ( x > y ) {
			y = lcr->y_old + ( dt * lcr->dy_up );
			if(y > x)
				y = x;
			
		} else if ( x < y ) {
			y = lcr->y_old - ( dt * lcr->dy_down );
			if(y < x)
				y = x;
		}
				
		// Check limits
		if (y >= lcr->y_max ) {
		  y = lcr->y_max;
		  lcr->max_limit = 1;

		 
		}
		
		else if(y <= lcr->y_min ) {
		  y = lcr->y_min;
		  lcr->min_limit = 1;

		}

		lcr->y = y;

		lcr->x_reached = (lcr->y == lcr->x)? 1:0;
		lcr->y_old = y;

	} 	
 
}

void AzzMovAvgFlt(AzzMovAvgFlt_typ *lcr) {


  float *ptr;
  double *sum = (double*)&lcr->sum_old;
  int i;

  //lcr->y = lcr->x ;
  //return;

  if(!lcr){
  	lcr->status = -1;
	return;
  }

  if(lcr->base == 0){
  	lcr->status = -2;
	return;
  }
  	

  lcr->status = 0;
  
  if ( lcr->enable ) {
    // Default to successful status
    lcr->status = 0;

    // If base has changed, re-allocate buffer
    if ( lcr->base != lcr->base_old || ( (void *)lcr->p_xold == 0 ) ) {
		
      // Free the old buffer
	 if((lcr->p_xold != 0 ) && lcr->base_old > 0){
		  if(TMP_free(lcr->base_old , (void**) lcr->p_xold)){
		  	return;
		  }
		  else
		  	lcr->base_old = 0;
		  

	 }
	 else{ // Memory leack

	 	 lcr->base_old = 0;
		 lcr->p_xold = 0;
		 

	 }
	  	
      //if ( (void *)lcr->p_xold != 0 )
        //free((void *)lcr->p_xold);

      // Allocate new buffer
      //lcr->p_xold = (unsigned long)malloc(lcr->base * sizeof(lcr->x));
      if(TMP_alloc(lcr->base * sizeof(float)  , (void**)&lcr->p_xold))
	  	return;
	  else
		lcr->base_old = lcr->base;
     
      ptr = (float*)lcr->p_xold;

      // Set buffer to initial values (current x)
      if (ptr != 0) {
        // Loop through whole buffer
        for( i = 0; i < lcr->base; i++ ) {
          *ptr = lcr->x;
          ptr++;
        }
      } else {
        lcr->status = 12018;
        return;
      }	 

      // Set sum
      *sum = lcr->x * lcr->base;

	  lcr->y = (*sum) / lcr->base;

      // Set index
      lcr->i_xold = 0;
    }

	 //return;
	if ( lcr->i_xold >= lcr->base )
      lcr->i_xold = 0;
	
	ptr = (float*)lcr->p_xold;
    // Calculate new sum
    *sum = *sum - *(ptr + lcr->i_xold) + lcr->x;

    // Replace x value in buffer
    *(ptr + lcr->i_xold) = lcr->x;

    // Increase index and ring around the buffer if necessary
    lcr->i_xold ++;    
    
    // Calculate new avg
    lcr->y = (*sum) / lcr->base;

  } 

  else {
  	
    lcr->status = 65534;
  }
 

}

void AzzSlimPID(AzzSlimPID_typ *lcr) {

  AzzPID_typ *pid = &lcr->PID_inst;
  azzslimpid_par_typ *para = lcr->pPar;
  float p, i, d, error, dt;
  DINT time , deltaT;



  // TODO - Ensure that the derivative term does not go crazy first scan
  // We are not using the derivative

  if (lcr->enable) {
	    // Get current time
	    time = AsIOTimeStamp();

	    // Determine error
	      // Includes feedback damping
	      // e_fbk = Kfbk / Kp * (Ytotal - Y)

	    //error = lcr->W - lcr->X - pid->e_fbk;
	    error = lcr->W - lcr->X ;

		deltaT = (time - pid->last_time);

		if(deltaT > 0 && (deltaT < 0x4FFFFFFF))
	    	dt = (float)(deltaT) / 1000000.0;
		else
			dt = 0.0;

		lcr->dt = dt;
		lcr->e = error;

	    // Proportional control
	    p = para->Kp * error;

	    // Integral control
	    //pid->i_sum += error * dt;
	    pid->i_sum += ((error - pid->e_fbk) * para->Kp * dt);

		if(para->Tn > 0)
	    	i = pid->i_sum / para->Tn;
		else
			i = 0;

	    // Derivative control
	    if(dt > 0)
	    	d = ((lcr->X - pid->d_old) / dt) * para->Tv * para->Kp ;
		else
			d = 0.0;
		
	    pid->d_old = lcr->X;

	    // Set output variable
	    lcr->Y = p + i + d;

	    // Limit output variable
	    if (lcr->Y > para->Y_max) {
	      pid->e_fbk = para->Kfbk * (lcr->Y - para->Y_max);
	      lcr->Y = para->Y_max;
	    } else if (lcr->Y < para->Y_min) {
	      pid->e_fbk = para->Kfbk * (lcr->Y - para->Y_min);
	      lcr->Y = para->Y_min;
	    } else {
	      pid->e_fbk = 0;
	    }

	

  } else {

  	i = pid->i_sum = 0;
    lcr->status = 65534;
  }

  pid->last_time = AsIOTimeStamp();

}

void AzzScal(AzzScal_typ *lcr) {
  REAL k, d;

  if (lcr->x1 != lcr->x2) {
    k = (lcr->y2 - lcr->y1) / (lcr->x2 - lcr->x1);
    lcr->y = lcr->y1 + (k * (lcr->x - lcr->x1));
    
  } else {
    lcr->y = 0;
  }

}

void AzzLimScal(AzzLimScal_typ *lcr) {
  REAL *high, *low;

  // Run LCRScal - this function is the same except for limits
  AzzScal((AzzScal_typ *)lcr);

  // Find high limit and low limit
  if (lcr->y1 < lcr->y2) {
    high = &lcr->y2;
    low = &lcr->y1;
  } else {
    low = &lcr->y2;
    high = &lcr->y1;
  }

  // Limit y to be between the high and low values
  if (lcr->y > *high)
    lcr->y = *high;
  if (lcr->y < *low)
    lcr->y = *low;

}

//////////    END of New Functions /////////////////////
/////////////////////////////////////////////////////

float LimScal(float x, AzzLimScal_typ* scale){

	scale->x = x;
	AzzLimScal(scale);

	return scale->y;
}

float Scal(float x, AzzScal_typ* scale){

	scale->x = x;
	AzzScal(scale);

	return scale->y;
}


DINT Errors(ERROR_Mgr_typ *mgr , char type , unsigned short indx , unsigned long val){


	if(!mgr->logs[indx].status){

		mgr->lastItemIndx++;
		if(mgr->lastItemIndx >= 64)
			mgr->lastItemIndx = 0;

		mgr->logs[indx].status = 1;
		mgr->logs[indx].value = val;
		mgr->history[mgr->lastItemIndx] = indx;
		
		if(type > 0){
			if(type >= mgr->Error) // set only higher priority error
				mgr->Error = type;
		}
		else 
			mgr->Error = type;

		return 1;

	}
	else	
		return 0;
}


DINT AckAllErrors(ERROR_Mgr_typ *mgr){
	int i;

	//return 1;

	// Never clear fatal error
	if (mgr->Error < 0)
		return -1;
	
	for( i = 0 ; i < 64 ; i++){
		 
		mgr->logs[i].status = 0;

		mgr->history[i] = 0;

	}

	mgr->Error = 0;
	mgr->lastItemIndx = 0;


	return 1;	

}

DINT KK_AckAllErrors(ERROR_Mgr_typ *mgr){
	int i;

	// Never clear fatal error
	if (mgr->Error < 0)
		return -1;
	
	for( i = 0 ; i < 64 ; i++){
		 
		mgr->logs[i].status = 0;

		mgr->history[i] = 0;

	}

	mgr->Error = 0;
	mgr->lastItemIndx = 0;


	return 1;	

}


DINT StateVectorSrv(StateVectorProxy_typ *vect , UINT thread){

	if(vect->testMode)
		return;
	
	/*****************************************/
	///    VECTOR  SERVER

	if(!vect->simul){
	
		vect->OutVect.Mode = vect->Mode ;
		
		switch(vect->Mode){

			case vectSt_Mode_Idle:
				
				vect->OutVect.volt = 0.0;
				vect->OutVect.vel = 0.0;

				break;	

			case vectSt_Mode_Hold:

				


				break;	

			case vectSt_Mode_Home:

				


				break;	

			case vectSt_Mode_Pos:

				vect->OutVect.volt = 0.0;
				vect->OutVect.amp  = 0.0;

				//if(thread == vect->Indx )
					vect->OutVect.x = vect->InVal;

				break;	

			case vectSt_Mode_Vel:

				vect->OutVect.volt = 0.0;
				vect->OutVect.amp  = 0.0;	

				//if(thread == vect->Indx )
					vect->OutVect.vel = vect->InVal;

				break;	

			case vectSt_Mode_Volt:

				vect->OutVect.amp  = 0.0;

				//if(thread == vect->Indx )
					vect->OutVect.volt = vect->InVal;
				
				vect->OutVect.vel = 0;

				break;	

			case vectSt_Mode_Torq:

				vect->OutVect.volt = 0.0;
				
				//if(thread == vect->Indx )
					vect->OutVect.amp = vect->InVal;
				
				vect->OutVect.vel = 0;

				break;	

			case vectSt_Mode_PT:

				vect->OutVect.volt = 0.0;
				vect->OutVect.amp  = 0.0;
				vect->OutVect.x = vect->InVal;

				break;	

		}	
	}
	else{

		vect->FbkVect.Mode = vect->Mode;

		switch(vect->Mode){

			case vectSt_Mode_Idle:

				
				vect->FbkVect.x = 0;
				vect->FbkVect.vel = 0;

				

				break;	

			case vectSt_Mode_Hold:

				


				break;	

			case vectSt_Mode_Home:

				

				break;	

			case vectSt_Mode_Pos:

				vect->FbkVect.x = vect->InVal;
			
				break;	

			case vectSt_Mode_Vel:
				
				vect->FbkVect.vel = vect->InVal;				
				
				break;	

			case vectSt_Mode_Volt:

				vect->FbkVect.volt = vect->InVal;				
			
				break;	

			case vectSt_Mode_Torq:

				vect->FbkVect.amp = vect->InVal;
				

				break;		

			
			case vectSt_Mode_PT:

				vect->FbkVect.x = vect->InVal;
				//vect->OutVect.x = vect->InVal;

				break;	

		}	


	}

		
	return 1;

}

UINT Bool2Uint(plcbit in){

	if(in)
		return 1;
	else
		return 0;


}

plcbit Uint2Bool(UINT in){

	if(in > 0)
		return 1;
	else
		return 0;


}


char* UDINT2ASC(UDINT val){
static char ret[5];
UDINT mask;
int i;

	mask = 0x000000FF;
	
	for(i=3 ; i >= 0 ; i--){

		ret[i] = (char)(val & mask);
		val = val >> 8;
		
	}
	ret[4] = 0;
	return ret;

}


UDINT ASC2UDINT(char *p){
UDINT val;
int i;

	val = 0;

	for(i=0 ; i < 4  ; i++){
		val = val<<8;
		val += p[i];		
	}

	
	return val;
	
}
INT ABS_INT(INT x){

	if( x >= 0)
		return x;
	else
		return x*(-1);

}


float getFromCalib(float x , Calib_Param_typ* calib , UINT type){
float ret; float _x;
AzzScal_typ scale;

	if(!calib)
		return 0.0;

	if(x == 0.0)
		return 0.0;

	if(x < 0.0)
		_x = x * (-1.0);
	else
		_x = x;

	switch(type){


		case CALIB_ONE2ONE:
			
			ret = _x;
			
		break;


		case CALIB_2POINTS:

			scale.x = _x;
			scale.x1 = calib->val[0];
			scale.y1 = calib->val[1];
			scale.x2 = calib->val[2];
			scale.y2 = calib->val[3];

			if( _x < scale.x1)
				ret = (_x/scale.x1) * scale.y1; 
			
			else {
				AzzScal(&scale);			
				ret = scale.y;
			}
			
		break;	

		case CALIB_2POINTS_4OFF:

			scale.x = _x;
			scale.x1 = calib->val[4];
			scale.y1 = calib->val[5];
			scale.x2 = calib->val[6];
			scale.y2 = calib->val[7];

			if( _x < scale.x1)
				ret = (_x/scale.x1) * scale.y1; 
			
			else {
				AzzScal(&scale);			
				ret = scale.y;
			}
			
		break;	


		
		case CALIB_2POINTS_INV:
		
			scale.x = _x;
			scale.x1 = calib->val[1];
			scale.y1 = calib->val[0];
			scale.x2 = calib->val[3];
			scale.y2 = calib->val[2];

			if( _x < scale.x1)
				ret = (_x/scale.x1) * scale.y1; 
			
			else {
				AzzScal(&scale);			
				ret = scale.y;
			}
			
		break;	
		

		case CALIB_2POINTS_4OFF_INV:
		
			scale.x = _x;
			scale.x1 = calib->val[5];
			scale.y1 = calib->val[4];
			scale.x2 = calib->val[7];
			scale.y2 = calib->val[6];

			if( _x < scale.x1)
				ret = (_x/scale.x1) * scale.y1; 
			
			else {
				
				AzzScal(&scale);			
				ret = scale.y;
			}
			
		break;	

	}

	

	// Symetric to zero
	if(x <  0.0)
		ret = ret * (-1.);

	
	return ret;

}


static UDINT hash_of_binary_array(char *key, int len)
{
	unsigned int hash, i;
	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

///   New Path Generator created March 2019 by CK
void HCRSPFazz(HCRSPFazz_type* inst){
float dist, a , d , v , v02 , ad2 , x , s ;
float S_end , S_in , V_in , V0 , V_max;
float dir;
UINT t;
float deltaT;

	if(!inst)
		return;
/*****************************************/

	if(!inst->Intr._Set_home && inst->Set_home){
		// This is new for conveniance
		inst->Set_home = 0;

		inst->s_set = inst->s_home;
		inst->v_set = 0;
		inst->a_set = 0;
		
		inst->Intr.step = 0;

	}

	if(!inst->Intr._Stop && inst->Stop){

		// This is new for conveniance
		inst->Stop = 0;

		inst->v_set = 0;
		inst->a_set = 0;
		
		inst->Intr.step = 0;

	}	

	if(!inst->Intr._Brake && inst->Brake){
	

		d = (inst->v_set > 0)? inst->pMove_Para->aDec_pos : inst->pMove_Para->aDec_neg;			
		dir = (inst->v_set > 0.0)? 1.0:-1.0;

		/*
		inst->s_end = ((inst->v_set * inst->v_set )/d/2.0)*dir + inst->s_set;

		inst->Intr.step = 1;
		*/
		

		inst->Intr.S0 = ((inst->v_set * inst->v_set )/d/2.0)*dir;			
		inst->Intr.segm[0].s = inst->Intr.S0  + inst->s_set ;
		inst->Intr.segm[0].v = 0;
		inst->Intr.segm[0].a = -d * dir;	

		inst->Intr.S1 = 0.0;
		inst->Intr.segm[1].s = inst->Intr.segm[0].s;
		inst->Intr.segm[1].v = 0;
		inst->Intr.segm[1].a = 0;	

		inst->Intr.S2 = 0.0;
		inst->Intr.segm[2].s = inst->Intr.segm[0].s;
		inst->Intr.segm[2].v = 0;
		inst->Intr.segm[2].a = 0;	

		inst->Intr.S3 = 0.0;
		inst->Intr.segm[3].s = inst->Intr.segm[0].s;
		inst->Intr.segm[3].v = 0;
		inst->Intr.segm[3].a = 0;	
		
		inst->Intr.indx = 0;
		inst->elapseTime = 0.0;
		inst->Reached = 0;
		inst->Intr.step = 3;

		

	}	
		

	if(!inst->Intr._Init && inst->Init )
		inst->Intr.step = 1;
	

	switch(inst->Intr.step){




		//// Verify if current velocity in bigger then maximum velocity
		case 1:

			strcpy(inst->Intr.statusTxt , "");

			inst->Reached = 0;

			inst->Intr.segm[0].s = inst->s_set;
			inst->Intr.segm[0].v = inst->v_set;
			inst->Intr.segm[0].a = 0.0;	

			inst->Intr.S0 = 0;
			////////////////////////////////////////////////////////////////////////////////////////
			// Check if movement direction change is necessary
			
			dist = inst->s_end - inst->Intr.segm[0].s;
			d = (inst->Intr.segm[0].v > 0)? inst->pMove_Para->aDec_pos : inst->pMove_Para->aDec_neg;			
			
			s = ((inst->v_set * inst->v_set)/d)/2.0; 

			if((fabsf(dist) < s) 					// Distance is smaller then coming from deceleration
				|| ((dist *  inst->v_set)<0.0)){ 	// Running in oposite direction
				
				strcpy(inst->Intr.statusTxt , "Change move direction");


				if(inst->Intr.segm[0].v > 0){

					inst->Intr.S0 = s;
					inst->Intr.segm[0].s += s;
					inst->Intr.segm[0].v = 0.0;
					inst->Intr.segm[0].a = -d;

					

				}
				if(inst->Intr.segm[0].v < 0){

					inst->Intr.S0 = -s;
					inst->Intr.segm[0].s -= s;
					inst->Intr.segm[0].v = 0.0;
					inst->Intr.segm[0].a = d;		

					
				}			

				goto CALCULATE_TRAPEZOID;
				
			}
			
			///////////////////////////////////////////////////////////////////////////////////////////
			///  Check Speed above maximum

			if(inst->v_set > 0.0)
				V_max = inst->pMove_Para->vC_pos;
			else if(inst->v_set < 0.0) 
				V_max = inst->pMove_Para->vC_neg;
			
			if(fabsf(inst->v_set) > V_max){ // reduce speed first{

				
				if(inst->v_set > 0.0){

					inst->Intr.segm[0].v = V_max;
					inst->Intr.segm[0].a = -inst->pMove_Para->aDec_pos;
				
					x = (fabsf(inst->v_set) - V_max );
					inst->Intr.S0 = (x * x)/ inst->pMove_Para->aDec_pos /2.0 ;
					inst->Intr.S0 += inst->Intr.segm[0].v * x/inst->pMove_Para->aDec_pos;
			
					inst->Intr.segm[0].s += inst->Intr.S0;

					strcpy(inst->Intr.statusTxt , "Max Pos Vel");
				}
				else if(inst->v_set < 0.0){

					
					inst->Intr.segm[0].v = -V_max;
					inst->Intr.segm[0].a = inst->pMove_Para->aDec_neg;
				
					x = (fabsf(inst->v_set) - V_max );
					inst->Intr.S0 = (x * x)/ inst->pMove_Para->aDec_neg /(-2.0) ;
					inst->Intr.S0 += inst->Intr.segm[0].v * x/inst->pMove_Para->aDec_neg;
			
					inst->Intr.segm[0].s += inst->Intr.S0;

					strcpy(inst->Intr.statusTxt , "Max Neg Vel");
				}	

				
 
			}
			

			CALCULATE_TRAPEZOID:
			//////////////////////////////////////////////////////////
			///  Calculate Segments 1 , 2 , 3
			S_end = inst->s_end;
			S_in  = inst->Intr.segm[0].s ;
			V_in  = inst->Intr.segm[0].v ;

			dir = (S_end > S_in)? 1.0:-1.0;

			if(dir > 0){
				a = inst->pMove_Para->aAcc_pos;
				d = inst->pMove_Para->aDec_pos;
			}
			else if(dir < 0){

				a = inst->pMove_Para->aAcc_neg;
				d = inst->pMove_Para->aDec_neg;
			}
			else{

				strcpy(inst->Intr.statusTxt , "No Trapezoid");

			}

			inst->Intr.segm[1].a = a ; 
			inst->Intr.segm[2].a = 0; 
			inst->Intr.segm[3].a = -d  ;

			////////////////////////////////////////////
			// Calculate maximum velocity 	
			
			dist = fabsf(S_end - S_in);
			V0 = fabsf(V_in);
			

			v02 = (V0 * V0)/ (2.0 * a);
			ad2 = (1.0/(2*a)) + (1.0/(2*d));

			 v = (dist + v02)/ad2;

			 v = sqrtf(v);			

			////////////////////////			
			
			if(dir > 0.0){	 // positive move direction		 	

				if(v > inst->pMove_Para->vC_pos){

					inst->Intr.segm[1].v = inst->pMove_Para->vC_pos;					
				}
				else{

					inst->Intr.segm[1].v = v;		

				} 
			}	

			else if(dir < 0.0){	 // negative move direction 	

				if( v > inst->pMove_Para->vC_neg){

					inst->Intr.segm[1].v = inst->pMove_Para->vC_neg ;					
				}
				else{

					inst->Intr.segm[1].v = v ;		

				} 					
			}

			inst->Intr.segm[2].v = inst->Intr.segm[1].v;
			inst->Intr.segm[3].v = 0.0;

			////////////////////////////////////	
			
			inst->Intr.S1 = 0.0; // Init with 0
			
			x = inst->Intr.segm[1].v - V0;
			x = x*x;
			x = x/(2.0 * a);			
			
			inst->Intr.S1 += x; // + Triangle area

			///

			x = V0 * (inst->Intr.segm[1].v - V0);
			x = x /(2.0 * a) ;

			inst->Intr.S1 += x; // + Square area
			inst->Intr.S1 = inst->Intr.S1 * dir;
			inst->Intr.segm[1].s = inst->Intr.S1 + inst->Intr.segm[0].s ;

			//////////////////////////////			

			inst->Intr.S3 = (inst->Intr.segm[2].v *  inst->Intr.segm[2].v)/( 2*d)* dir ; // Area under deceleration				
			inst->Intr.S2 = (S_end - S_in) - inst->Intr.S1 - inst->Intr.S3;
			
			inst->Intr.segm[2].s = S_end - inst->Intr.S3;			
			inst->Intr.segm[3].s = S_end;	



			inst->Intr.segm[1].v = inst->Intr.segm[1].v * dir ; 
			inst->Intr.segm[2].v = inst->Intr.segm[2].v * dir ; 
			inst->Intr.segm[3].v = inst->Intr.segm[3].v * dir ;

			inst->Intr.segm[1].a = inst->Intr.segm[1].a * dir ; 
			inst->Intr.segm[2].a = 0; 
			inst->Intr.segm[3].a = inst->Intr.segm[3].a * dir ;


			inst->ApproachDir =(inst->Intr.segm[2].v > 0)? 1:-1;
			if(inst->Intr.segm[2].v == 0)
				inst->ApproachDir = 0;


			/////////////////////////////
			inst->Intr.indx = 0;
			inst->elapseTime = 0.0;
			
			
			// This is new for conveniance
			inst->Init = 0;
			
			//inst->Intr.step++;
			//inst->Intr.step++;
			inst->Intr.step = 3;
			 
		break;



		case 3:

			inst->Reached = 0;

			if(inst->Intr.TaskPeriod <= 0.0)
				inst->Intr.TaskPeriod = 0.0024; // standard time
			
			inst->elapseTime += inst->Intr.TaskPeriod;

			inst->a_set = inst->Intr.segm[inst->Intr.indx].a;
			inst->v_set = inst->Intr._v_set +  inst->Intr.segm[inst->Intr.indx].a * inst->Intr.TaskPeriod;
			inst->s_set = inst->Intr._s_set +  (inst->v_set + inst->Intr._v_set)/2.0 * inst->Intr.TaskPeriod;
			

			if((inst->s_set > inst->Intr._s_set)){

				if(inst->s_set >= inst->Intr.segm[inst->Intr.indx].s){

					inst->v_set = inst->Intr.segm[inst->Intr.indx].v;
					inst->Intr.indx++;				

				}
			}

			else if((inst->s_set < inst->Intr._s_set)){

				if(inst->s_set <= inst->Intr.segm[inst->Intr.indx].s){

					inst->v_set = inst->Intr.segm[inst->Intr.indx].v;
					inst->Intr.indx++;				

				}
			}
				
			else if (fabsf(inst->s_set - inst->Intr.segm[inst->Intr.indx].s) < 0.01) {

				strcpy(inst->Intr.statusTxt , "END END");

				inst->s_set = inst->Intr.segm[inst->Intr.indx].s;
				inst->v_set = inst->Intr.segm[inst->Intr.indx].v;
				inst->Intr.indx++;	


			}
	
			
			if(inst->Intr.indx > 3){

				inst->s_set = inst->Intr.segm[3].s;
				inst->v_set = inst->Intr.segm[3].v;
				inst->a_set = 0.0;

				inst->Reached = 1;

				// This is new for conveniance
				inst->Brake = 0;
					
				inst->Intr.step = 0;

			}

		break;

		default:

			strcpy(inst->Intr.statusTxt , "Idle");

			inst->Intr.step = 0;

		break;

		



	}

	
	inst->MoveDir =(inst->v_set > 0)? 1:-1;
	if(inst->v_set == 0.0)
		inst->MoveDir = 0;
	

/****************************************/
	inst->Intr._s_set = inst->s_set ;
	inst->Intr._v_set = inst->v_set ;
	inst->Intr._a_set = inst->a_set ;

	inst->Intr._enable = inst->enable ;
	inst->Intr._Stop = inst->Stop ;
	inst->Intr._Set_home = inst->Set_home ;
	inst->Intr._Init = inst->Init ;
	inst->Intr._Brake = inst->Brake ;
	
}





#endif


