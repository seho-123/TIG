/********************************************************************
 * COPYRIGHT -- PAI
 ********************************************************************
 * Package: lib
 * File: Plot.c
 * Author: Christopher
 * Created: February 11, 2009
 *******************************************************************/

#ifndef _PLOTLIB_C
#define _PLOTLIB_C
 
#include <..\Lib\Plot.h>
#include <math.h>

plcstring Str[32];

/*PROTOTYPES for Internal Function*/


/**/
DINT PlotData(UDINT handle, PLOT_type *plot ) 
{	
	if(plot->pData == 0) 	return -100;
	
	
		if ( plot->DRAW && (handle > 0) ) {

			switch(plot->_step) {
				case 0:	
						if(plot->Type == PLOT_DOTS){
							if(DRAW_DOTS( handle, plot)){
								plot->DRAW = 0;
								plot->_step = 0;
							}
						}						
						else if(plot->Type > 0){ /* PLOT_WITH_GRID and PLOT_DISTRIBUTION*/
							CALCULATE (plot);
							if(CLEAR_GRAPH(handle, plot))
								plot->_step++;
						}						
						else
							plot->_step++;
				break;
	 			case 1:
					if(plot->Type > 0) /* PLOT_WITH_GRID and PLOT_DISTRIBUTION*/
					{	
						if(DRAW_GRID( handle, plot))
							plot->_step++;		
					}	
					else
						plot->_step++;
					
					break;
				case 2:
					if (DRAW_GRAPH( handle, plot))	{
						plot->DRAW 	= 0;
						plot->_step 	= 0;
					}
				break;

				case 3:

				break;	

				default:
				break;
			}
	}
		
	if ( plot->CLEAN && (handle > 0) ) {
		CLEAR_GRAPH( handle, plot);
		plot->CLEAN 		= 0;
		plot->_step		= 0;
	}
	
	return 1;
}


/**/
BOOL DRAW_GRAPH(UDINT handle, PLOT_type *plot) {
DINT i = 0;
DINT m = 0;
DINT X, Y, W, H, X1, X2,Y1,Y2;
DINT Y_OFFSET;		
DINT POS_Y_MIN, POS_Y_MAX;
REAL x_scaled,PIXEL_INC_X,PIXEL_INC_Y_POS;
REAL y_scaled_pos;
REAL prev_value_pos;
USINT POS_COLOR;
USINT k;

	/*Map variables for easier reading of code*/
	X 			= plot->GRAPH_X;
	Y 			= plot->GRAPH_Y;
	W 			= plot->GRAPH_WIDTH;
	H 			= plot->GRAPH_HEIGHT;

	/*Calculate the X scaling factor*/
	PIXEL_INC_X = ((float) W ) / ( (float) plot->DataLen);	
	PIXEL_INC_Y_POS = plot->_Y_INCR;

	if(plot->DataLen==0) return 0;
	
	if ( !VA_Saccess(1,handle) ) {

		POS_COLOR	= plot->ColorGraph;						
		/* Line Maximum */
		prev_value_pos	= (plot->_Y_MAX - plot->_Y_MIN) * PIXEL_INC_Y_POS;	
		Y2 = (UINT)((UINT)Y + (UINT)H - (UINT)prev_value_pos);

		VA_Line(1, handle, X, Y2, X+W, Y2, 21);	/* Magenta */

		if(plot->Type == PLOT_WITH_GRID){
			ftoa(plot->_Y_MAX ,Str);
			VA_Textout (1,handle, plot->Text_Font , X + W - 80, Y2 , plot->ColorGraph ,  TRANSP , Str);
		}

		/* Line ZERO */
		prev_value_pos	= (0 - plot->_Y_MIN) * PIXEL_INC_Y_POS;
		Y2 = (UINT)((UINT)Y + (UINT)H - (UINT)prev_value_pos);
		VA_Line(1, handle, X, Y2, X+W, Y2, plot->ColorBorder);	

		
				
		y_scaled_pos  = POS_Y_MIN;	
		X2 = X;
		X1 = X2;

		if(plot->startAt >= plot->DataLen)	plot->startAt = plot->DataLen - 1;		
		if(plot->stopAt <= plot->startAt)plot->stopAt = plot->DataLen;
			
		for (i = 0; i < plot->DataLen; i++) {		
			/*DRAW THE LINE from previous point to the current point*/

			if(*(plot->pData + i )<= (-1000000000)) break;
					
			X1 = X2 ;
			X2 = (UINT)(((float)X) + ((float)i * PIXEL_INC_X));			
			y_scaled_pos  = ((*(plot->pData + i ))- plot->_Y_MIN );		
			

			//if(y_scaled_pos < 0) y_scaled_pos = 0;
			y_scaled_pos  = y_scaled_pos * PIXEL_INC_Y_POS ;							
			Y1 = (UINT)((UINT)Y + (UINT)H - (UINT)y_scaled_pos);
			

			/*Draw the line*/	
			if(Y1 > (Y + H )) 	Y1 = (Y + H );
			if(Y1 < Y)			Y1 = Y;
			if(X2 <= (X + W +1)){

				if(i>1){
					if(plot->Type == PLOT_DISTRIBUTION)
						VA_Rect(1,handle, X2 , Y1 , 1 , Y + H - Y1, POS_COLOR, POS_COLOR);				
					
					else						
						VA_Line(1, handle, X1, Y2, X2, Y1, POS_COLOR);
					}
				Y2 = Y1;
			}
			
											
		}

		if(plot->Type == PLOT_DISTRIBUTION){
			X1 = (UINT)(((float)X) + ((float)(plot->VERT_Indx1))* PIXEL_INC_X);					
			X2 = (UINT)(((float)X) + ((float)(plot->VERT_Indx2))* PIXEL_INC_X);
			VA_Rect(1,handle, X1 , Y+1 , X2-X1 , H - 2 , plot->VERT_Color , plot->VERT_Color);		
		}
		
		/*RELEASE THE SCREEN HANDLE*/
		VA_Srelease(1,handle);
		return 1;
	
	}
	else return 0;

return 0;
}

/**/
BOOL DRAW_DOTS(UDINT handle, PLOT_type *plot) {
DINT i = 0;
DINT m = 0;
DINT X, Y, W, H, X1, X2,Y1,Y2;
DINT Y_OFFSET;		
DINT POS_Y_MIN, POS_Y_MAX;
REAL x_scaled,PIXEL_INC_X,PIXEL_INC_Y_POS;
REAL y_scaled_pos;
REAL prev_value_pos;
USINT POS_COLOR;
USINT k;

	/*Map variables for easier reading of code*/
	X 			= plot->GRAPH_X;
	Y 			= plot->GRAPH_Y;
	W 			= plot->GRAPH_WIDTH;
	H 			= plot->GRAPH_HEIGHT;

	/*Calculate the X scaling factor*/
	PIXEL_INC_X = ((float) W ) / ( (float) plot->DataLen);	
	PIXEL_INC_Y_POS = plot->_Y_INCR;

	
	
	if ( !VA_Saccess(1,handle) ) {

		POS_COLOR	= plot->ColorGraph;						
		
		for (i = 0 ; i <  plot->DataLen; i++) {		
			/*DRAW THE LINE from previous point to the current point*/
			
			X1 = X + (UINT)((*(plot->pDataX + i )) * (((float)plot->GRAPH_WIDTH-1.0) / plot->LIMIT_X_MAX));

			y_scaled_pos  = ((*(plot->pData + i ))- plot->_Y_MIN );
			

			y_scaled_pos  = y_scaled_pos * PIXEL_INC_Y_POS ;							
			Y1 = (UINT)((UINT)Y + (UINT)H - (UINT)y_scaled_pos);
			
			/*Draw the line*/	
			if((Y1 <=(Y + H )) &&
				(Y1 > Y)&&
				(X1 <= (X + W +1))&&
				(X1 >= X)){
				
					VA_Ellipse(1,handle,X1, Y1,4,4,POS_COLOR,POS_COLOR);
					
					VA_Line(1, handle, X1, Y, X1, Y+H, 7); /* Grey */
					VA_Line(1, handle, X, Y1, X+W, Y1, 7);  /* Grey */

					ftoa(*(plot->pData + i ) ,Str);				
					VA_Textout (1,handle,plot->Text_Font, X + 5, Y1 ,POS_COLOR , TRANSP, Str);
					ftoa(*(plot->pDataX + i ) ,Str);				
					VA_Textout (1,handle,plot->Text_Font, X1 + 5, Y + H - plot->Text_Font_base, POS_COLOR , TRANSP, Str);
			}
		}
		/*RELEASE THE SCREEN HANDLE*/
		VA_Srelease(1,handle);
		return 1;
	}

	return 0;
}


/*  */
BOOL CLEAR_GRAPH(UDINT handle, PLOT_type *plot) {
	DINT X, Y, W, H;
	
	X 	= plot->GRAPH_X;
	Y 	= plot->GRAPH_Y;
	W 	= plot->GRAPH_WIDTH;
	H 	= plot->GRAPH_HEIGHT;

	if (!(VA_Saccess(1,handle))) {		
		VA_Redraw(1,handle);
		VA_Srelease(1,handle);
		return 1;
	}

return 0;	
}
/*  */
BOOL DRAW_LINE(UDINT handle, PLOT_type *plot, USINT type) {
DINT X, Y, W, H;
	
	X 	= plot->GRAPH_X;
	Y 	= plot->GRAPH_Y;
	W 	= plot->GRAPH_WIDTH;
	H 	= plot->GRAPH_HEIGHT;

	if (!(VA_Saccess(1,handle))) {
		VA_Rect(1, handle, 	X+2, 	Y + H/2-2, 	W-3, 	5, 			34, 		34);
		VA_Srelease(1,handle);
		return 1;
	}

return 0;	
}
/*  */
BOOL DRAW_GRID(UDINT handle, PLOT_type *plot ) {
DINT X, X1 ,X2 , Y, W, H , exp , T;
DINT X_grid, Y_grid,X_offset, Y_offset;
USINT j , i ;
float y, ygrid , PIXEL_INC_X , dif ;

	X 	= plot->GRAPH_X;
	Y 	= plot->GRAPH_Y;
	W 	= plot->GRAPH_WIDTH;
	H 	= plot->GRAPH_HEIGHT;

	

	if (!(VA_Saccess(1,handle))) {

		/* Background */
		VA_Rect(1, handle, X, Y, W, H,  plot->ColorFill , plot->ColorBorder );

		if(plot->AUTO_GRID && (plot->Type != PLOT_DISTRIBUTION)){
			PIXEL_INC_X = ((float) W  / (float) plot->DataLen);	
			
			if((plot->VERT_Indx1>0) && (plot->VERT_Indx2 > 0)){
				X1 = (UINT)(((float)X) + ((float)(plot->VERT_Indx1))* PIXEL_INC_X);					
				X2 = (UINT)(((float)X) + ((float)(plot->VERT_Indx2))* PIXEL_INC_X);
				VA_Rect(1,handle, X1 , Y+1 , X2-X1 , H - 2 , plot->VERT_Color , plot->VERT_Color);	
			}

			if(plot->VERT_Indx3 > 0){
				X1 = (UINT)(((float)X) + ((float)(plot->VERT_Indx3))* PIXEL_INC_X);
				VA_Line( 1, handle, X1, Y , X1, Y+H, plot->VERT_Color2); 
			}
			if(plot->VERT_Indx4 > 0){
				X1 = (UINT)(((float)X) + ((float)(plot->VERT_Indx4))* PIXEL_INC_X);
				VA_Line( 1, handle, X1, Y , X1, Y+H, plot->VERT_Color2); 
			}

			if(plot->Type != PLOT_DOTS){
			
				j = 1;
				X_grid = X+( plot->X_GRID_Per * j) ;
				while( X_grid < (X + W)){				
					VA_Line( 1, handle, X_grid , Y, X_grid, Y+H, 7 ); /* Grey */
					j++;
					X_grid = X+( plot->X_GRID_Per * j) ;	
					if(j> 100) break;
				}				
				
				/**************************************/
				dif = (plot->_Y_MAX - plot->_Y_MIN);
				exp = 1; y = 0; j = 0;
				while(y < dif) {
					y = fmod(dif , pow(10,(float)exp++));
					if(j++> 30) break;
				}
				if(exp>0) exp--;
				if(exp>0) exp--;
				ygrid = pow(10,(float)exp);
				if(dif /ygrid < 10) ygrid = ygrid / 10;
				if(dif/ygrid > 10) ygrid = ygrid * 5;
				if(dif/ygrid < 5) ygrid = ygrid / 5;
				/***************************************/
				
				j = 1;
				plot->_test = ygrid;
				y = (((float) H/plot->_Y_INCR) + plot->_Y_MIN );
				y = y - fmod(y , ygrid) ;	
				
				while(1){

					Y_grid = Y+ H - (DINT)((y - plot->_Y_MIN)* plot->_Y_INCR);
		
					if(Y_grid < (Y+H)){ 
						if(Y_grid > Y){
							VA_Line( 1, handle, X, Y_grid , X+W, Y_grid, 7 );  /* Grey */					
							ftoa(y ,Str);				
							VA_Textout (1,handle,plot->Text_Font, X + W - 30, Y_grid , plot->ColorBorder , TRANSP, Str);
						}
					}
					else
						break;
					
					
					j++;
					y  = y - ygrid ;
					if(j> 100) break;
				}	

				T = (DINT)(((float)H) * 0.2)+ Y; 
				VA_Textout(1,handle, plot->Text_Font1, X1, T , BLACK , TRANSP , plot->Txt1);			
				VA_Textout(1,handle, plot->Text_Font2, X1 + 10 , T + 20, BLACK , TRANSP , plot->Txt2);
			}
			else{
			}
				
			
		}
		else{
			/* Draw Grid */
			X_grid	= W / 5;
			Y_grid	= H / 5;
			X_offset	= X_grid / 2;
			Y_offset	= Y_grid / 2;
			for ( j=0;j<6;j++) {
				VA_Line( 1, handle, X+(X_grid*j), Y, X+(X_grid*j), Y+H, 7 ); /* Grey */
				VA_Line( 1, handle, X, Y+(Y_grid*j), X+W, Y+(Y_grid*j), 7 );  /* Grey */
				if ( j < 5 ) {
					VA_Line( 1, handle, X+(X_grid*j)+X_offset, Y, X+(X_grid*j)+X_offset, Y+H, 7 );	 /* Grey */
					VA_Line( 1, handle, X, Y+(Y_grid*j)+Y_offset, X+W, Y+(Y_grid*j)+Y_offset, 7 );   /* Grey */
				}
			}	
		}	
		VA_Srelease(1,handle);
		return 1;
	}


return 0;	
}


/**/
DINT CALCULATE(PLOT_type *plot ) 
{
DINT i , k;
REAL Y;
REAL Y_MIN, Y_MAX , POS_Y_MAX;	

	Y_MIN	= 3.4e+32;
	Y_MAX	= -3.4e+32;		
	for(i = 1; i < plot->DataLen; i++ ) {
		Y = *(plot->pData + i );		
		if ( Y >= Y_MAX ) 	{	Y_MAX = Y;	}
		if ( Y <= Y_MIN ) 	{	Y_MIN = Y;	}		
	}	
	plot->Y_MIN 	= Y_MIN;
	plot->Y_MAX 	= Y_MAX;

	
	if (plot->AUTO_SCALE){
		plot->LIMIT_Y_MAX = Y_MAX + (Y_MAX - Y_MIN)*0.1;
		plot->LIMIT_Y_MIN = Y_MIN - (Y_MAX - Y_MIN)*0.1;

	}	

	POS_Y_MAX	= (plot->LIMIT_Y_MAX - plot->LIMIT_Y_MIN) ;
	plot->_Y_MIN =  plot->LIMIT_Y_MIN;
	plot->_Y_MAX =  plot->Y_MAX;
		
	if(POS_Y_MAX > 0){		
		 plot->_Y_INCR	= ( (((REAL)  plot->GRAPH_HEIGHT))  / (POS_Y_MAX ) );
	}
	else{
		 plot->_Y_INCR = 0;
	}

	
	return 1;

	
}

#endif





































