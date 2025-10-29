/********************************************************************
 * COPYRIGHT -- wsi
 ********************************************************************
 * Package: Lib
 * File: kinematicLib.h
 * Author: wsi
 * Created: October 12, 2012
 *******************************************************************/

#ifndef _KINEMATICLIB_H

#define _KINEMATICLIB_H
#include <bur/plctypes.h>
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


#include <..\Lib\Complex.h>
#include <..\lib\MatrixLib.h>

/* PATH TYPE */
#define LINE_RELATIVE 0
#define ARC_LEN_ANGLE 1
#define ARC_RAD_ANGLE 2
#define ROT_ANGLE 	  3
#define JOG_LINE 	  4
#define JOG_ROT 	  5



/** Data in  Path **/
#define LEN 			0
#define ANGLE 			1
#define ENTRY_ANGLE 	2



/* Arm COMMANDS */
#define CMD_IDLE 			0
#define CMD_INIT			1
#define CMD_HALT			2

#define CMD_GO_2ABS 		40
#define CMD_GO2_CENTER 		41

#define CMD_RUN_CIRCLE		50

#define CMD_STOP 			60

#define CMD_JOG				70
#define CMD_JOG_X			71
#define CMD_JOG_Y			72
#define CMD_JOG_ANG			73
#define CMD_JOG_ANGLE		74
#define CMD_JOG_LINE 		75


#define CMD_JOG_TETA		80
#define CMD_JOG_RAD			81
#define CMD_STEP_RAD_PLUS	82
#define CMD_STEP_RAD_MINUS	83

#define CMD_SWITCH_DIRECT	84



#define CMD_JOG_INDIV 		100

/* Arm STATUS */
#define STS_IDLE 			0
#define STS_RUN 			1
#define STS_SIM 			2
#define STS_JOG 			1
#define STS_JOG 			2



typedef struct TEACH_POINTS_typ   
{
	UDINT Type;
	UDINT PointNum;
	UDINT Num;
	UDINT padding;
	Complex Point[10];

}TEACH_POINTS_typ;



typedef struct SEGMENT_typ   
{
	SINT Active;
	Complex StartPoint;
	Complex EndPoint;
	char Dir;
	double Vin;
	double Vout;
	UINT Type;
	double data[3];
	
	/*****************************
	Type=0xXXX0 - Line		

	Type=0xXXX1  - Arc
		data[1] = radius
	
	************************************

	Type = 0xXX0X
		Relative values - use data[0] as Length
		
	Type =0xXX1X
		Abslotue values - use StartPoint and EndPoint	
		
	*/
	
}SEGMENT_typ;

typedef struct POLYNOM_typ   
{
	double   a, b, y, x;
	
}POLYNOM_typ;

typedef struct TCP_typ   
{
	double ToolAngleSP;
	Complex TCP;
	double a;
	double avc;
	USINT OutSide;
	USINT Gibbous;
	
}TCP_typ;

 typedef struct MTR_ANGLE_typ	  
{
	double ya[4];
	
}MTR_ANGLE_typ;

 typedef struct PATH_MSC_typ   
 {
  	 UINT state;	
	 INT segmIndx;
	 //INT oldSegmIndx;
	 MPGEN_type mpg;
	 POLYNOM_typ  polynom[4];		
	 SEGMENT_typ curSegm;
	 SEGMENT_typ nextSegm;
	 double x; //current x on path
	 double v; //current velocity on path
	 double t; // current time on path 
	 double angle; // segment angle
	 double len; // Segment length
	 double rad; // circle radius
	 double cirDir;
	 double curAngle; //current angle of motion
	 //SINT oldDir;
	 double shorter;
	 double compAngle;
	 double startAngle; // // start angle for circle
	 Complex startPoint;
	 Complex endPoint;
	 Complex centrPoint;

 }PATH_MSC_typ;

 
 typedef struct PATH_typ   
 {
	 BOOL newSegment;
	 SEGMENT_typ *pSegm;
	 UINT numSegments;
	 TCP_typ *pTCPx; 
	 USINT ClearView;
	 double deltaT;
	 //SINT dir;
	 //double MinRad;
	 PATH_MSC_typ msc; 
 
 }PATH_typ;
 
 typedef struct CIRCLE_PATH_typ   
{
	USINT xCmd;
	USINT Active;
	USINT VelMode;
	SINT  RotDir;
	Complex Center;	
	double MeasRadius;
	TCP_typ *pTCP;
	double ang;
	double radius;
	double RadiusTarget;
	double deltaT;	
	HCRSPF_typ radMoveGEN;
	HCRSPF_typ angMoveGEN;
	HCRSPF_typ linMoveGEN;

}CIRCLE_PATH_typ;
 

 typedef struct LINE_PATH_typ   
{
	//USINT Type;
	USINT xCmd;
	//USINT Manual;
	TCP_typ *pTCP;
	Complex START;
	Complex DEST;
	double dist;
	double angle;
	double deltaT;
	//GEN_type distMPGEN;
	HCRSPF_typ MoveGen;


}LINE_PATH_typ; 
 


 typedef struct ASSEM_3R_typ
{
	double R1;
	double R2;
	double R3;
	double ToolAngle;
	double JogAngle;
	USINT Type;
	USINT DrawArm;
	Complex Test_Point;		
	TCP_typ TCPx;
	TCP_typ oldTCPx;
	SINT _Inverse;
	MTR_ANGLE_typ Mtrs;

}ASSEM_3R_typ;

  typedef struct ARM_MSC_typ
 {
 	UINT traj_step;
 	
 }ARM_MSC_typ;


  typedef struct MTR_CTRL_typ
 {
 	UINT xCMD;
	UINT Status;
	UINT Error;	
	double *pPos;
	double posFB;	
 	
 }MTR_CTRL_typ;

  typedef struct ARM_PARAM_typ
 {
 	char AutoRepeat;
	char RoundCorners;
	double MinCornerRadius;
	USINT SelectServo;
	TCP_typ AbsTCP;
	TCP_typ *pPermTCP;
	UINT indx;
	HCRMovePara_typ jogMoveParam;
	HCRMovePara_typ linMoveParam;
	HCRMovePara_typ angleMoveParam;
	HCRMovePara_typ radMoveParam;
	
 }ARM_PARAM_typ; 

 typedef struct ARM_typ
{
	UINT xCMD;
	UINT alarm;
	ARM_PARAM_typ Param;
	char ConfigReady;
	UDINT pCfg;
	UDINT pCurCfg;
	SM_obj sm;
	USINT Manual;
	MTR_CTRL_typ Mtr[4];
	ARM_MSC_typ msc;
	PATH_typ *pPath;
	CIRCLE_PATH_typ *pCirPath;
	char VelMode;
	LINE_PATH_typ *pLinePath;
	ASSEM_3R_typ *pAssem;
	TEACH_POINTS_typ *pTeachPoints;

}ARM_typ;

 


DINT GetATriangleAngle(double a , double b, double c , double *out);
int Invers3R(ASSEM_3R_typ *as);  
double Forward3R(ASSEM_3R_typ *as);

int CircleOverTriangle(Complex *pIn , Complex *cent , double *rad);  


DINT GetTCP(PATH_typ *path);  
DINT GetCircleTCP(CIRCLE_PATH_typ *path);
DINT GetLineTCP(LINE_PATH_typ *path);
void WatchForGibbous(ASSEM_3R_typ *as);





#endif
