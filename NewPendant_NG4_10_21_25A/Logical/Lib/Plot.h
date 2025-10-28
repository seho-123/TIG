
#ifndef _PLOTLIB_H
#define _PLOTLIB_H


#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
 #include <AsDefault.h>
#endif

#ifndef _IEC_CONST
#define _IEC_CONST _WEAK const
#endif


//*===COLORS===*/
#define TRANSP						0xFF
#define GREEN						10
#define DARK_GREEN					2
#define BLUE						1
#define BABY_BLUE					11
#define TEAL						168
#define RED							0x33
#define YELLOW						46
#define BLACK						0
#define WHITE						15
#define GRAY						7
#define DARK_NAVY					219
#define DARK_GRAY					8
#define LIGHT_GRAY					227
#define CYAN						196
#define MAGENTA						21
#define BROWN						86
#define LIGHT_BLUE					52

#define PLOT_ONLY_GRAPH				0
#define PLOT_WITH_GRID				1
#define PLOT_DISTRIBUTION			2
#define PLOT_DOTS					3



typedef struct PLOT_type
{
		float* pData;
		float* pDataX;
		unsigned short DataLen;
		unsigned short startAt;
		unsigned short stopAt;
		unsigned char Type;
		unsigned char _step;
		BOOL AUTO_SCALE;
		BOOL AUTO_GRID;
		BOOL DRAW;
		BOOL CLEAN;
		unsigned short ColorBorder;
		unsigned short ColorFill;
		unsigned short ColorGraph;
		signed long GRAPH_X;
		signed long GRAPH_Y;
		signed long GRAPH_WIDTH;
		signed long GRAPH_HEIGHT;
		signed long GRAPH_NUMBER;
		unsigned short X_GRID_Per;		
		unsigned short VERT_Indx1;
		unsigned short VERT_Indx2;
		unsigned char  VERT_Color;
		unsigned short VERT_Indx3;
		unsigned short VERT_Indx4;
		unsigned char  VERT_Color2;
		unsigned char* Txt1;
		unsigned char* Txt2;
		float _Y_INCR;
		float _X_INCR;
		float _Y_MAX;
		float _Y_MIN;
		float Y_MAX;
		float Y_MIN;
		float LIMIT_Y_MAX;
		float LIMIT_Y_MIN;
		float LIMIT_X_MAX;
		float LIMIT_X_MIN;
		unsigned short Text_Font;
		unsigned short Text_Font1;
		unsigned short Text_Font2;
		signed long	   Text_Font_base;	
		float _test;

}PLOT_type;
 

/* Prototyping of functions and function blocks */
signed long PlotData(unsigned long handle, PLOT_type* plot);
DINT CALCULATE(PLOT_type *plot ) ;
BOOL DRAW_GRAPH(UDINT handle, PLOT_type *plot);
BOOL CLEAR_GRAPH(UDINT handle, PLOT_type *plot);
BOOL DRAW_LINE(UDINT handle, PLOT_type *plot, USINT type);
BOOL DRAW_GRID(UDINT handle, PLOT_type *plot );
BOOL DRAW_DOTS( handle, plot);




__asm__(".section \".plc\"");

__asm__(".previous");


#endif /* _PLOTLIB_ */

