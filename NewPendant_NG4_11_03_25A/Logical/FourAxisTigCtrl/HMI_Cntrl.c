/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: Main2
 * File: HMI_Cntrl.c
 * Author: ckaczkowski
 * Created: November 06, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include <math.h>


#ifndef _HMICNTRL_C
#define _HMICNTRL_C

#include <..\GlobDef.h>
#include <..\GlobDecl.h>
#include <..\..\Lib\LCDLib.c>

//#include <..\..\Lib\utilityLib\bgdebug.h>



void ScrollIndx(LCD_NODE_obj *pNodes , HMI_typ *hmi ,  HMI_INTRF_typ *intrf , UINT LineID , UINT limit);
void PickTMLServo(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi, int diagnPage);
void PickPROCESS(LCD_NODE_obj *pNodes ,HMI_INTRF_typ *intrf , HMI_typ *hmi, int diagnPage);
void PickAXIS(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi, int diagnPage);
void PickDCMtr(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi, int diagnPage);
void PickIO(HMI_typ *hmi, int diagnPage);
void F_ERROR(FILE_RW_typ *f , HMI_typ *hmi);

void Set_Message(HMI_typ *hmi , float time);

void EvaluateHMI_1(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);
void EvaluateHMI_Android(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);


void CalculateTiming(MASTER_typ *m);

void AssignHeadText(char* txt ,   UINT HeadType);

void AssignMtrErrorText(char* txt , UDINT i , UDINT val );

void EncodeMappingIndx(CurCalib_typ *cal , UINT indx);
UINT DecodeMappingIndx(CurCalib_typ *cal);
void WatchSegmented_Yes_No(MASTER_typ *m);


void Param_Global_Standard(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);
void Param_Global_BPV_v2_MPBO_STANDARD(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);
void Param_Global_BPV_v2_MPBO_ONEWAY(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);
void Param_Global_BPV_v2_MPBO_CONTIN(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf);




DINT _oldHeadSetup;
//UINT _curHeadID;
DINT _CalibrActDistTicks ;
float calibTmr;

float fbkVect_x;

_GLOBAL SINT _segmYes[PARAM_NUM_MAX]; 

_GLOBAL float v1_0 , v2_0 ,  v1_1 , v2_1 , K1 , K2 , b0 , b1 , b2 , m1 , m2;
_GLOBAL plcbit KK_Functionality;
_GLOBAL UINT 	curTML_indx ,TML_Setup_From_Diagn, curMappingSetup_Indx;



void EvaluateHMI_Android(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf){

	Scaled_Var_obj *var , *var1;
	Units_obj *unit , *unit1;
	CurCfg_typ *curCfg;
	Cfg_typ *cfg;
	float incr , min , max , gain , offset; 
	double angle , dist;
	char *pUnitName;
	Unit_Scale_obj *scale;
	int prec;
	int i;
	UDINT k;
	static char lines[3][32];
	static char blankField[32];
	char *p;
	LCD_NODE_obj *head;
	CurCalib_typ *curCalib , *defaultCalib;
	SegmentsTable_obj *segmTable;
	StateVectorProxy_typ* vect;
	DINT d;
	float x , y;
	UINT status;

	UINT n,l;

	//float v1_0 , v2_0 ,  v1_1 , v2_1 , K1 , K2 , b0 , b1 , b2 , m1 , m2;



	///////////////////////
	char* s;
	s = "0123456789ABCDEF";

	if(!m || !hmi || !nodes || !intrf)
		return;

	curCfg = (CurCfg_typ*)m->pCurCfg;
	cfg = (Cfg_typ*)m->pCfg;

	var = (Scaled_Var_obj*) m->pScaledVar;

	curCalib = m->pCalib;
	defaultCalib = m->pDefaultCalib;

	segmTable = (SegmentsTable_obj*) m->pSegmentTable;

	
	if(!curCfg || !cfg || !var || !nodes || !segmTable)

		return;


			
	/////////////////////////////////
	// GLOBAL EVENTS

	if(hmi->FiredEvent == EVNT_GO_TO_DEFAULT){

			hmi->DynPageActive = 1;
			hmi->oldDynPageNodeIndx = 0xFFFF;
			
	}	

	// All pages below menu are not dynamic pages
	else if(hmi->curHeadID >= P_MENU){
		hmi->DynPageActive = 0;

	}
	else
		hmi->DynPageActive = 1;


	

	//	Global Page Navigation	
	if(hmi->DynPageActive){
		if(intrf->LCDEntr_PosEdge){
			//Check is current line has Enter Btn active
					
			if(!(nodes[hmi->curNodeID].Type & N_EDIT)
				&& !(nodes[hmi->curNodeID].Type	& N_JUMP)
				&& !(nodes[hmi->curNodeID].Type	& N_EVENT) ){

				hmi->Node2JumpID = P_MENU;
				hmi->JumpLineID = 0;				
				hmi->xCMD = JUMP2ID_CMD;

				

			}
				
				
			
		}
	}
			
	// Dispaly Dynamic Page
	if(hmi->DynPageActive){
		
		// Make a jump when active page changed
		if(hmi->DynPageNodeIndx != hmi->oldDynPageNodeIndx){

			if(hmi->DynPageNodeIndx < NODES_NUM_MAX){
				hmi->Node2JumpID =  hmi->DynPageNodeIndx;
				hmi->JumpLineID = 0;
				hmi->xCMD = JUMP2ID_CMD;

			}
		}	
			
	}
	hmi->oldDynPageNodeIndx = hmi->DynPageNodeIndx;


	
	///////////////////////////////////////////////////////
	
	memset(hmi->ActiveBanner , 0 , sizeof(hmi->ActiveBanner));
	
	
	/*************************************************************/
	if(hmi->curHeadID != 0xFFFF){
		
		switch(hmi->curHeadID){

			/*************************************************************/
			case P_INIT:

					
				

			break;	


			
			/*************************************************************/
			case 0xFFFE:  // RUN and Alternate RUN sccreen


				

				if(curCfg->Param_ENUM[parENUM_WeldProgramLinkMode].val[0] > 0)
					strcpy(hmi->pHMI_Lines[0] , " YES");
				else
					strcpy(hmi->pHMI_Lines[0] , " NO");

				if(curCfg->Param_ENUM[parENUM_PathMode].val[0] > 0)
					strcpy(hmi->pHMI_Lines[1] , " NO");
				else
					strcpy(hmi->pHMI_Lines[1] , " YES");
				

			break;	


			
			/*************************************************************/
			case P_MENU: strcpy(hmi->ActiveBanner , "	<<<  MENU >>>>");


			
				///   COOLANT //

				if(COOL_Param_Set[0].pOutput){
					
					if(hmi->FiredEvent == EVNT_TOGGLE){		

						//if(Process[PROC_COOL].Status == prcsSTATUS_Run)
						if(*COOL_Param_Set[0].pOutput)
							
							Process[PROC_COOL].xCMD = prcsHalt;
						else
							Process[PROC_COOL].xCMD = prcsForceON;		

						
					}	

					
					if(*COOL_Param_Set[0].pOutput)

					   strcpy(hmi->pHMI_Lines[0] , ON_OFF_TXT[1]);
											
					else
						strcpy(hmi->pHMI_Lines[0] , ON_OFF_TXT[0]);
					
				}
				else
					strcpy(hmi->pHMI_Lines[0] , "");

				
				

				if(COOL_Param_Set[0].pSensInput){ 
						
					if(*COOL_Param_Set[0].pSensInput)					
				
						strcpy(hmi->pHMI_Lines[1] , ON_OFF_TXT[1]);
					else
						strcpy(hmi->pHMI_Lines[1] , ON_OFF_TXT[0]);		
				}
				else
					strcpy(hmi->pHMI_Lines[1] , "");
					

				
				if(GAS_Param_Set[0].pSensInput){ 
									
					if(*GAS_Param_Set[0].pSensInput)
						strcpy(hmi->pHMI_Lines[2] , ON_OFF_TXT[1]);
					else
						strcpy(hmi->pHMI_Lines[2] , ON_OFF_TXT[0]); 
				}
				else
					strcpy(hmi->pHMI_Lines[2] , "");
	

				

			break;	

			
			/*********************************************************/
			case P_PARAM_CONTIN_WELD: strcpy(hmi->ActiveBanner , "< CONTIN WELD PARAMS >");

			
				
				switch(hmi->curNodeID){					

					case P_PARAM_CONTIN_WELD +2:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[0][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenDintValue[0]++;				
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenDintValue[0]--;
								
																							
							}	

							
							if(curCalib->GenDintValue[0] < -30)
								curCalib->GenDintValue[0] = -30;
							
							else if(curCalib->GenDintValue[0] > 30)
								curCalib->GenDintValue[0] = 30;
								

						}
						

					break;

					

					case P_PARAM_CONTIN_WELD +3:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[0][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenDintValue[1]++;				
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenDintValue[1]--;
								
																							
							}	

							
							if(curCalib->GenDintValue[1] < -30)
								curCalib->GenDintValue[1] = -30;
							
							else if(curCalib->GenDintValue[1] > 30)
								curCalib->GenDintValue[1] = 30;
							

						}
						

					break;
				}

				/*
				switch(curCalib->GenDintValue[0]){

					default: strcpy(hmi->pHMI_Lines[0], "Option A"); break; 
					case 1: strcpy(hmi->pHMI_Lines[0], "Option B"); break;
				}
				*/

				xsprintf(hmi->pHMI_Lines[0], "Path A Correct.: %d" ,curCalib->GenDintValue[0]);
				
				xsprintf(hmi->pHMI_Lines[1], "Path B Correct.: %d" ,curCalib->GenDintValue[1]);

			break;		

			/*********************************************************/
			case P_PARAM_GLOBAL: strcpy(hmi->ActiveBanner , "< GLOBAL PARAMS >");

				curMappingSetup_Indx = DecodeMappingIndx(curCalib);
				
				switch(hmi->curNodeID){

					

					case P_PARAM_GLOBAL:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[0][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_AUTO_FILL]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_AUTO_FILL]--;
								
																							
							}	

							if(curCalib->GenSetup[SETUP_AUTO_FILL] < 0)
								curCalib->GenSetup[SETUP_AUTO_FILL] = 0;
							
							else if(curCalib->GenSetup[SETUP_AUTO_FILL] > 2)
								curCalib->GenSetup[SETUP_AUTO_FILL] = 2;

						}
						

					break;

					case P_PARAM_GLOBAL +1:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[1][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_WHERE_FILL]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_WHERE_FILL]--;
								
																							
							}	

							if(curCalib->GenSetup[SETUP_WHERE_FILL] < 0)
								curCalib->GenSetup[SETUP_WHERE_FILL] = 0;
							
							else if(curCalib->GenSetup[SETUP_WHERE_FILL] > 2)
								curCalib->GenSetup[SETUP_WHERE_FILL] = 2;

						}
						

					break;
					
					

					case P_PARAM_GLOBAL+2:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[2][0]!='*')
							&& !m->pCfg->HeadCfg.TVL.PinsPerRot){ // Pins Per Rot Zero - means not predifne track					

							if(intrf->TickPlus){	
								intrf->TickPlus = 0;
								curCalib->GenSetup[SETUP_PIN_PER_ROT]++;
								curCalib->GenSetup[SETUP_SEGM_NO] = 0;
								//curCalib->GenSetup[SETUP_ZONE_NO] = 0;
								//curCalib->GenSetup[SETUP_TVL_DISTANCE] = 0;

								PleaseUpdateHeadSetup(m);

																					
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;
								curCalib->GenSetup[SETUP_PIN_PER_ROT]--;	
								curCalib->GenSetup[SETUP_SEGM_NO] = 0;
								//curCalib->GenSetup[SETUP_ZONE_NO] = 0;							
								//curCalib->GenSetup[SETUP_TVL_DISTANCE] = 0;

								PleaseUpdateHeadSetup(m);

														
							}
							

							
						}
						if(curCalib->GenSetup[SETUP_PIN_PER_ROT]< 1)
							curCalib->GenSetup[SETUP_PIN_PER_ROT] = 1;

						

					break;	

					/////////////////////////////////
					case P_PARAM_GLOBAL +3:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[3][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curMappingSetup_Indx ++;
								EncodeMappingIndx(curCalib , curMappingSetup_Indx);
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curMappingSetup_Indx --;
								EncodeMappingIndx(curCalib , curMappingSetup_Indx);
																							
							}	

							

						}
						

					break;

					///////////////////////////

					case P_PARAM_GLOBAL +4:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[4][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_AUTO_START]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_AUTO_START]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_AUTO_START] < 0)
								curCalib->GenSetup[SETUP_AUTO_START] = 0;
							
							else if(curCalib->GenSetup[SETUP_AUTO_START] > 1)
								curCalib->GenSetup[SETUP_AUTO_START] = 1;

						}
						

					break;

					////
					case P_PARAM_GLOBAL +5:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[5][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] += 0.5;
							
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] -= 0.5;
																													
							}	

							if(curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] < 0)
								curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] = 0;
							
							else if(curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] > 10.0)
								curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY] = 10.0;

						}
						

					break;

					case P_PARAM_GLOBAL +6:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[6][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_SOUND_DELAY]++;
						
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_SOUND_DELAY]--;
																															
							}	

							if(curCalib->GenSetup[SETUP_SOUND_DELAY] < 0)
								curCalib->GenSetup[SETUP_SOUND_DELAY] = 0;
							
							else if(curCalib->GenSetup[SETUP_SOUND_DELAY] > 1)
								curCalib->GenSetup[SETUP_SOUND_DELAY] = 1;

						}
						

					break;


					case P_PARAM_GLOBAL +7:	
										
										
						if(intrf->LCDEntr && (hmi->pHMI_Lines[7][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] += 0.5;
															
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] -= 0.5;
								
																							
							}	

							if(curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] < 0)
								curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] = 0;
							
							else if(curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] > 10.0)
								curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM] = 10.0;

						}
						

					break;



					/////////////////////////////////////
					case P_PARAM_GLOBAL+8:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[8][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_INDX_DIR]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_INDX_DIR]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_INDX_DIR] < 0)
								curCalib->GenSetup[SETUP_INDX_DIR] = 0;
							
							else if(curCalib->GenSetup[SETUP_INDX_DIR] > 1)
								curCalib->GenSetup[SETUP_INDX_DIR] = 1;

						}

					break;


					
					/////////////////////////////////////
					case P_PARAM_GLOBAL+9:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[9][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_INDX_ENBL]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_INDX_ENBL]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_INDX_ENBL] < 0)
								curCalib->GenSetup[SETUP_INDX_ENBL] = 0;
							
							else if(curCalib->GenSetup[SETUP_INDX_ENBL] > 3)
								curCalib->GenSetup[SETUP_INDX_ENBL] = 2;

						}

					break;

					/////////////////////////////////////
					case P_PARAM_GLOBAL+10:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[10][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_ZONE_CENTR]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_ZONE_CENTR]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_ZONE_CENTR] < 0)
								curCalib->GenSetup[SETUP_ZONE_CENTR] = 0;
							
							else if(curCalib->GenSetup[SETUP_ZONE_CENTR] > 1)
								curCalib->GenSetup[SETUP_ZONE_CENTR] = 1;

						}

					break;

					/////////////////////////////////////
					case P_PARAM_GLOBAL+11:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[11][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_TWO_TORCH]++;
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_TWO_TORCH]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_TWO_TORCH] < 0)
								curCalib->GenSetup[SETUP_TWO_TORCH] = 0;
							
							else if(curCalib->GenSetup[SETUP_TWO_TORCH] > 1)
								curCalib->GenSetup[SETUP_TWO_TORCH] = 1;

						}

					break;

					/////////////////////////////////////
					case P_PARAM_GLOBAL+12:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[12][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_TWO_UP]++;							
								
								
								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_TWO_UP]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_TWO_UP] < 0)
								curCalib->GenSetup[SETUP_TWO_UP] = 0;
							
							else if(curCalib->GenSetup[SETUP_TWO_UP] > 1)
								curCalib->GenSetup[SETUP_TWO_UP] = 1;


					}

					break;

					/////////////////////////////////////
					case P_PARAM_GLOBAL+13:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[13][0]!='*')){
							
							if(hmi->UserLevel >= 15){ // Supervisor

								if(m->ESTOP){												
									
									if(intrf->TickPlus){	
										intrf->TickPlus = 0;

										curCalib->GenSetup[SETUP_CONTIN_WELD]++;							
										
										
										
									}
									else if(intrf->TickMinus){					
										intrf->TickMinus = 0;

										curCalib->GenSetup[SETUP_CONTIN_WELD]--;
																									
									}	

									if(curCalib->GenSetup[SETUP_CONTIN_WELD] < 0)
										curCalib->GenSetup[SETUP_CONTIN_WELD] = 0;
									
									//else if(curCalib->GenSetup[SETUP_CONTIN_WELD] > 2)
										//curCalib->GenSetup[SETUP_CONTIN_WELD] = 2;	

									else if(curCalib->GenSetup[SETUP_CONTIN_WELD] > 1)
										curCalib->GenSetup[SETUP_CONTIN_WELD] = 1;


								}
								else{

									strcpy(hmi->pMessage[0], " ESTOP");
									strcpy(hmi->pMessage[1], "Must Be Pressed ");
									strcpy(hmi->pMessage[2], "  to change");
									strcpy(hmi->pMessage[3], " HEAD setup");
									TimerStart(&hmi->MsgTmr, 2.0);


								}				

							}
							else{

								strcpy(hmi->pMessage[0], " Only ");
								strcpy(hmi->pMessage[1], "	SUPERVISOR ");
								strcpy(hmi->pMessage[2], " can performe");
								strcpy(hmi->pMessage[3], " This operation");
								TimerStart(&hmi->MsgTmr, 2.0);


							}

						}

					break;

					

					/////////////////////////////////////
					case P_PARAM_GLOBAL+14:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[14][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_TVL_ENC_SET]++;							
								
								
								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_TVL_ENC_SET]--;
																							
							}	

							if(curCalib->GenSetup[SETUP_TVL_ENC_SET] < 0)
								curCalib->GenSetup[SETUP_TVL_ENC_SET] = 0;
							
							else if(curCalib->GenSetup[SETUP_TVL_ENC_SET] > 1)
								curCalib->GenSetup[SETUP_TVL_ENC_SET] = 1;


					}

					break;

					/////////////////////////////////////

					/////////////////////////////////////
					case P_PARAM_GLOBAL+15:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[15][0]!='*')){ 	


							if(m->ESTOP){
						
								if(intrf->TickPlus){	
									intrf->TickPlus = 0;

									curCalib->GenSetup[SETUP_TVL_SEC_ENC]++;							
									
									
									
								}
								else if(intrf->TickMinus){					
									intrf->TickMinus = 0;

									curCalib->GenSetup[SETUP_TVL_SEC_ENC]--;
																								
								}	

								if(curCalib->GenSetup[SETUP_TVL_SEC_ENC] < 0)
									curCalib->GenSetup[SETUP_TVL_SEC_ENC] = 0;
								
								else if(curCalib->GenSetup[SETUP_TVL_SEC_ENC] > 1)
									curCalib->GenSetup[SETUP_TVL_SEC_ENC] = 1;

							}
							else{

								strcpy(hmi->pMessage[0], " ESTOP");
								strcpy(hmi->pMessage[1], "Must Be Pressed ");
								strcpy(hmi->pMessage[2], "  to change");
								strcpy(hmi->pMessage[3], " TVL Feedback");
								TimerStart(&hmi->MsgTmr, 2.0);


							}


					}

					break;

					/////////////////////////////////////

					/////////////////////////////////////


					case P_PARAM_GLOBAL+17:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[16][0]!='*')){	

							if(hmi->UserLevel >= 15){ // Supervisor

								if(m->ESTOP){

									if(intrf->TickPlus){	
										intrf->TickPlus = 0;
										
										//curCalib->GenSetup[SETUP_HEAD_SETUP]++;
										NextHeadConfig_Indx++;
										
										curCalib->GenSetup[SETUP_SEGM_NO] = 0;
										

										PleaseUpdateHeadSetup(m);

										//ReadPermMemory_HS.Go = 1;

										hmi->PLC_Changes |= 0x0008; // new Segment Max

									
																	
									}
									else if(intrf->TickMinus){					
										intrf->TickMinus = 0;
										
										//curCalib->GenSetup[SETUP_HEAD_SETUP]--;	
										NextHeadConfig_Indx--;
									
										curCalib->GenSetup[SETUP_SEGM_NO] = 0;
										
										
										PleaseUpdateHeadSetup(m);

										//ReadPermMemory_HS.Go = 1;

										hmi->PLC_Changes |= 0x0008; // new Segment Max
								
										
									}

									#ifdef CHERRYPOINT_LIST

									switch(NextHeadConfig_Indx){
								
								
										case U_BEND_HP_PINS: NextHeadConfig_Indx++;											
									
										case U_BEND_HP_ENC: NextHeadConfig_Indx++;											
										
										case F_HEAD_HP_ENCODER:  NextHeadConfig_Indx++;
										
										case BPV_F_HEAD:  break;
										
										case OMEGA_16INCH_HP_ENCODER: NextHeadConfig_Indx++;
																						
										case OMEGA_10INCH_HP_ENCODER: NextHeadConfig_Indx++;
																						
										case OMEGA_8INCH_HP_ENCODER: NextHeadConfig_Indx++;											
											
										case F_HEAD_HP_MINI_ENC: break;

										case SUPER_MINI: NextHeadConfig_Indx++;

										case BPV_FACE_OSC: NextHeadConfig_Indx++;

										case U_BEND_HP_ENC_BEMF: NextHeadConfig_Indx++;

										case F_HEAD_HP_ENC_BEMF: NextHeadConfig_Indx++;

										case BPV_FACE_OSC_ENC: NextHeadConfig_Indx++;

										case BPV_FACE_OSC_EXP: break;

										case M_94: NextHeadConfig_Indx++;	
										

										case BPV_v2: break;

										case BPV_v2_MPBO: break;

										case BPV_v2_MPGH: NextHeadConfig_Indx++;

										case BPV_v2_MPGV: NextHeadConfig_Indx++;
										

										case BPV_v2_TT_BO: NextHeadConfig_Indx++;

										case BPV_v2_MT_BO: NextHeadConfig_Indx++;	
										
										case BPV_v2_TT_BO_TOS: NextHeadConfig_Indx++;
										
										case BPV_v2_MT_BO_TOS: NextHeadConfig_Indx++;

										
										case RUBIX: NextHeadConfig_Indx++;

										case F_HEAD_MINI_STND: NextHeadConfig_Indx++;

										case F_HEAD_MINI_OMEGA: NextHeadConfig_Indx++;

										case D_HEAD_HP:	NextHeadConfig_Indx++;

										case BPV2_FACE_OSC_EXP: break;

										case BPV_v2_MT: NextHeadConfig_Indx++;

										case BPV_v1_MT: NextHeadConfig_Indx++;

										case BPV_v2_Video: NextHeadConfig_Indx++;

										case CLAMSHELL_HEAD: NextHeadConfig_Indx++;
										
										case CHERRYPOINT_BP1_HEAD: break;

										case CHERRYPOINT_BP2_HEAD: break;

										case CHERRYPOINT_MT_BP2_HEAD: break;

										case MIG_TIG_HEAD: NextHeadConfig_Indx++;

										
											
										default:
								
											NextHeadConfig_Indx = U_BEND_HP_ENC;
								
										break;
									
								
									}
									#endif
									
									#ifdef FULL_HEAD_LIST
									
									switch(NextHeadConfig_Indx){
								
								
										case U_BEND_HP_PINS: NextHeadConfig_Indx++;											
									
										case U_BEND_HP_ENC: break;											
										
										case F_HEAD_HP_ENCODER:  break;
										
										case BPV_F_HEAD:  break;
										
										case OMEGA_16INCH_HP_ENCODER: NextHeadConfig_Indx++;
																						
										case OMEGA_10INCH_HP_ENCODER: NextHeadConfig_Indx++;
																						
										case OMEGA_8INCH_HP_ENCODER: NextHeadConfig_Indx++;											
											
										case F_HEAD_HP_MINI_ENC: break;

										case SUPER_MINI: break;

										case BPV_FACE_OSC: break;

										case U_BEND_HP_ENC_BEMF: break;

										case F_HEAD_HP_ENC_BEMF: break;

										case BPV_FACE_OSC_ENC: break;

										case BPV_FACE_OSC_EXP: break;

										case M_94: break;	
										

										case BPV_v2: break;

										case BPV_v2_MPBO: break;

										case BPV_v2_MPGH: break;

										case BPV_v2_MPGV: break;
										

										case BPV_v2_TT_BO: break;

										case BPV_v2_MT_BO: break;	
										
										case BPV_v2_TT_BO_TOS: break;
										
										case BPV_v2_MT_BO_TOS: break;

										
										case RUBIX: break;

										case F_HEAD_MINI_STND: break;

										case F_HEAD_MINI_OMEGA: break;

										case D_HEAD_HP:	break;

										case BPV2_FACE_OSC_EXP: break;

										case BPV_v2_MT: break;

										case BPV_v1_MT: break;

										case BPV_v2_Video: break;

										case CLAMSHELL_HEAD: break;
										
										case CHERRYPOINT_BP1_HEAD: break;

										case CHERRYPOINT_BP2_HEAD: break;

										case CHERRYPOINT_MT_BP2_HEAD: break;

										case MIG_TIG_HEAD: break;

										
											
										default:
								
											NextHeadConfig_Indx = U_BEND_HP_ENC;
								
										break;
									
								
									}
									#endif
										
										///////////////////////////////////
									/*
									if(NextHeadConfig_Indx < 0)
										NextHeadConfig_Indx = 0;

									else if(NextHeadConfig_Indx >= HEAD_INVALID)
										NextHeadConfig_Indx = HEAD_INVALID-1;

									*/	
									
								}
								else{

									strcpy(hmi->pMessage[0], " ESTOP");
									strcpy(hmi->pMessage[1], "Must Be Pressed ");
									strcpy(hmi->pMessage[2], "  to change");
									strcpy(hmi->pMessage[3], " HEAD setup");
									TimerStart(&hmi->MsgTmr, 2.0);


								}
								
							}
							else{

							strcpy(hmi->pMessage[0], " Only ");
							strcpy(hmi->pMessage[1], "	SUPERVISOR ");
							strcpy(hmi->pMessage[2], " can performe");
							strcpy(hmi->pMessage[3], " This operation");
							TimerStart(&hmi->MsgTmr, 2.0);


							}
					
						}

					break;


					/////////////////////////////////////
					case P_PARAM_GLOBAL+18:


						
						switch(curCalib->GenSetup[SETUP_HEAD_SETUP] ){
						
							default:

								if(intrf->LCDEntr && (hmi->pHMI_Lines[17][0]!='*')){ 	


									if(m->ESTOP){
								
										if(intrf->TickPlus){	
											intrf->TickPlus = 0;

											curCalib->GenSetup[SETUP_AVC_LEGACY]++;							
											
											
											
										}
										else if(intrf->TickMinus){					
											intrf->TickMinus = 0;

											curCalib->GenSetup[SETUP_AVC_LEGACY]--;
																										
										}	

										if(curCalib->GenSetup[SETUP_AVC_LEGACY] < 0)
											curCalib->GenSetup[SETUP_AVC_LEGACY] = 0;
										
										else if(curCalib->GenSetup[SETUP_AVC_LEGACY] > 1)
											curCalib->GenSetup[SETUP_AVC_LEGACY] = 1;

									}
									else{

										strcpy(hmi->pMessage[0], " ESTOP");
										strcpy(hmi->pMessage[1], "Must Be Pressed ");
										strcpy(hmi->pMessage[2], "  to change");
										strcpy(hmi->pMessage[3], " Legacy AVC");
										TimerStart(&hmi->MsgTmr, 2.0);


									}


								}

							break;

							case BPV_F_HEAD:
							case BPV_v1_MT:
							case CLAMSHELL_HEAD:	
							case CHERRYPOINT_BP1_HEAD:
							case CHERRYPOINT_BP2_HEAD:
							case CHERRYPOINT_MT_BP2_HEAD:
							case MIG_TIG_HEAD: 	

							break;

						}

					break;

					/////////////////////////////////////
					case P_PARAM_GLOBAL+19:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[18][0]!='*')){ 	


							if(m->ESTOP){
						
								if(intrf->TickPlus){	
									intrf->TickPlus = 0;

									curCalib->GenSetup[SETUP_WF_WP_SWAP]++;							
									
									
									
								}
								else if(intrf->TickMinus){					
									intrf->TickMinus = 0;

									curCalib->GenSetup[SETUP_WF_WP_SWAP]--;
																								
								}	

								if(curCalib->GenSetup[SETUP_WF_WP_SWAP] < 0)
									curCalib->GenSetup[SETUP_WF_WP_SWAP] = 0;
								
								else if(curCalib->GenSetup[SETUP_WF_WP_SWAP] > 1)
									curCalib->GenSetup[SETUP_WF_WP_SWAP] = 1;

							}
							else{

								strcpy(hmi->pMessage[0], " ESTOP");
								strcpy(hmi->pMessage[1], "Must Be Pressed ");
								strcpy(hmi->pMessage[2], "  to swap");
								strcpy(hmi->pMessage[3], " WF and WP");
								TimerStart(&hmi->MsgTmr, 2.0);


							}


					}

					break;				


					/////////////////////////////////////
					case P_PARAM_GLOBAL+20: //  WF Stop Ramp

						if(intrf->LCDEntr && (hmi->pHMI_Lines[19][0]!='*')){ 	

			
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								if((curCalib->GenSetup[SETUP_WF_RAMP] & 0x3) < 0x3)
									curCalib->GenSetup[SETUP_WF_RAMP]++;							
								
								
								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								if((curCalib->GenSetup[SETUP_WF_RAMP] & 0x3) > 0)
									curCalib->GenSetup[SETUP_WF_RAMP]--;
																							
							}									


					}

					break;				

					/////////////////////////////////////

					case P_PARAM_GLOBAL+21: //  WF Stop Ramp

						if(intrf->LCDEntr && (hmi->pHMI_Lines[20][0]!='*')){ 	

			
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								
								curCalib->GenSetup[SETUP_WF_RAMP] |= 0x4;							
								
								
								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenSetup[SETUP_WF_RAMP] &= 0xFFFFFFFB;	
																							
							}									


					}

					break;				

					/////////////////////////////////////

				}

				switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){


					case BPV_v2:
					case BPV_v2_Video:	
					case BPV_v2_MPBO:
					case BPV_v2_MPGH:
					case BPV_v2_MPGV:
						
					case BPV_v2_TT_BO:
					case BPV_v2_MT_BO:
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO_TOS:	
					//dodane 6Dec23
					case BPV_v2_MT:
					case BPV_v1_MT:	
					case MIG_TIG_HEAD:	

					case CHERRYPOINT_MT_BP2_HEAD:
				
						if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 2) // Continuse
							Param_Global_BPV_v2_MPBO_CONTIN(m,nodes,hmi,intrf);
						else if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 1) // One Way
							Param_Global_BPV_v2_MPBO_ONEWAY(m,nodes,hmi,intrf);	
						else
							Param_Global_BPV_v2_MPBO_STANDARD(m,nodes,hmi,intrf);

					break;

					/*
					case BPV_v2_MT_BO_TOS:
					case BPV_v2_TT_BO_TOS:		

						if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 1) // Continuse
							Param_Global_BPV_v2_MPBO_CONTIN(m,nodes,hmi,intrf);
						else if(curCalib->GenSetup[SETUP_CONTIN_WELD] == 2) // One Way
							Param_Global_BPV_v2_MPBO_ONEWAY(m,nodes,hmi,intrf);	
						else
							curCalib->GenSetup[SETUP_CONTIN_WELD] = 1;

					break;
					*/

					default:

						Param_Global_Standard(m,nodes,hmi,intrf);
						curCalib->GenSetup[SETUP_CONTIN_WELD] = 0;

					break;


				}
				

			
			break;	

			/*********************************************************/
			case P_PARAM_OSC: strcpy(hmi->ActiveBanner , "< OSC PARAMS >");	
			

			break;

			/*********************************************************/
			case P_PARAM_TVL_ROT: strcpy(hmi->ActiveBanner , "< TVL PARAMS >");	


			break;

			/*********************************************************/
			case P_PARAM_WP: strcpy(hmi->ActiveBanner , "< WP PARAMS >");	


			break;

			/*********************************************************/
			case P_PARAM_WF: strcpy(hmi->ActiveBanner , "< WF PARAMS >");	


			break;

			/*********************************************************/
			case P_PARAM_AVC: strcpy(hmi->ActiveBanner , "< AVC PARAMS >");	


			break;

			/*********************************************************/
			case P_PARAM_HW: strcpy(hmi->ActiveBanner , "< HW PARAMS >");	


			break;

			/*********************************************************/
			case P_PARAM_GAS: strcpy(hmi->ActiveBanner , "< GAS PARAMS >");	


			break;
		

			/*********************************************************/
			case P_PARAM_WELD: strcpy(hmi->ActiveBanner , "< WELD PARAMS >");	

				hmi->EditMode = EDIT_MODE_VALUE;

				
			
				
			break;

			/*********************************************************/
			case P_PARAM_APPL:

			
				switch(hmi->curNodeID){

					case P_PARAM_APPL + 1:
						
						
						if(intrf->LCDEntr){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;							

								curCfg->Param_INT[parINT_NumZone].val[0]++;

								PleaseUpdateHeadSetup(m);

								hmi->PLC_Changes |= 0x0008; // new Segment Max

							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCfg->Param_INT[parINT_NumZone].val[0]--;

								PleaseUpdateHeadSetup(m);

								hmi->PLC_Changes |= 0x0008; // new Segment Max
								
							}					
							

						}
											

					break;


					case P_PARAM_APPL + 2:
						
						
						if(intrf->LCDEntr){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

							 	curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] += (0.05 * 25.4);							

							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] -= (0.05 * 25.4);	

								if(curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] < 0.0 )	
									curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] < 0.0;
								
							}					
							

						}
											

					break;


					case P_PARAM_APPL + 7:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[2][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_EXTRAS] |=0x1; 
								
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;
								
								curCalib->GenSetup[SETUP_EXTRAS] &=0xFFFFFFFE; 
																							
							}	

							

						}

					break;

					case P_PARAM_APPL + 8:

						if(intrf->LCDEntr && (hmi->pHMI_Lines[3][0]!='*')){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								if((curCalib->GenSetup[SETUP_EXTRAS]&0x6) < 0x4)
									curCalib->GenSetup[SETUP_EXTRAS] += 0x2;						
								

								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								if((curCalib->GenSetup[SETUP_EXTRAS]&0x6) > 0x0)
									curCalib->GenSetup[SETUP_EXTRAS] -= 0x2;
																							
							}	

							

						}

					break;

					/*
					case P_PARAM_APPL + 9:

						if(hmi->FiredEvent == EVNT_OK){					
						
							if(hmi->UserLevel >= 15){// Supervisor


								if(m->ESTOP){

									if(TML_MTR_INST[intrf->tempIndx]._cfg.pAbsLastPos || 1){

										//(*TML_MTR_INST[intrf->tempIndx]._cfg.pAbsLastPos) = 0;
										//AxisCh[intrf->tempIndx].curPos = 0;
										//TML_MTR_INST[intrf->tempIndx].xCMD = TML_RESTART;
										

										 curCalib->GenFloatValue[SETUP_FLOAT_TILT_OFFSET] = LimScal_Tilt.y;

										strcpy(hmi->pMessage[0], " Zero TILT ");
										strcpy(hmi->pMessage[1], " is Set ");
										strcpy(hmi->pMessage[2], "");
										strcpy(hmi->pMessage[3], "");
										TimerStart(&hmi->MsgTmr, 2.0);

									}else{

										strcpy(hmi->pMessage[0], "TILT");
										strcpy(hmi->pMessage[1], " does not");
										strcpy(hmi->pMessage[2], " have ");
										strcpy(hmi->pMessage[3], "Absolute Pos.");
										TimerStart(&hmi->MsgTmr, 2.0);

									}
									

								}else{
									
									strcpy(hmi->pMessage[0], " ESTOP");
									strcpy(hmi->pMessage[1], "Must Be Pressed ");
									strcpy(hmi->pMessage[2], "  to Set");
									strcpy(hmi->pMessage[3], "Absolute Zero");
									TimerStart(&hmi->MsgTmr, 2.0);
								}
								

							}
							else {

								strcpy(hmi->pMessage[0], " Only ");
								strcpy(hmi->pMessage[1], "	SUPERVISOR ");
								strcpy(hmi->pMessage[2], " can performe");
								strcpy(hmi->pMessage[3], " This operation");
								TimerStart(&hmi->MsgTmr, 2.0);

							}
						}
						
					break;
					*/
						
				}
				
				
				xsprintf(hmi->pHMI_Lines[0] , "Segments No: %d",m->numZone);

				xsprintf(hmi->pHMI_Lines[1] , "Track Dia: %5.2f in", curCalib->GenFloatValue[SETUP_FLOAT_TRACK_DIA] * mm2inch);

				if(curCalib->GenSetup[SETUP_EXTRAS]&0x1)
					xsprintf(hmi->pHMI_Lines[2] , "Weld Report: ON");
				else
					xsprintf(hmi->pHMI_Lines[2] , "Weld Report: OFF");

				if(curCalib->GenSetup[SETUP_EXTRAS]&0x2)
					xsprintf(hmi->pHMI_Lines[3] , "TVL Rev DownSlop");

				else if(curCalib->GenSetup[SETUP_EXTRAS]&0x4)
					xsprintf(hmi->pHMI_Lines[3] , "Weld Index");
				else
					xsprintf(hmi->pHMI_Lines[3] , "Normal Weld");

				//xsprintf(hmi->pHMI_Lines[4] , "Set Tilt Zero");
		
		
			break;	
			
			/*********************************************************/
			case P_PARAM:

				switch(hmi->curNodeID){

					case P_PARAM+14:
						
						
						if(intrf->LCDEntr){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;							

								curCfg->Param_INT[parINT_NumZone].val[0]++;

								PleaseUpdateHeadSetup(m);
								hmi->PLC_Changes |= 0x0008; // new Segment Max

							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;

								curCfg->Param_INT[parINT_NumZone].val[0]--;

								PleaseUpdateHeadSetup(m);
								hmi->PLC_Changes |= 0x0008; // new Segment Max
								
							}					
							

						}
											

					break;

				}


				xsprintf(hmi->pHMI_Lines[0] , "Segments No: %d",m->numZone);
		
			break;	

			/*********************************************************/
			//     SPECIAL OPERATION	
			case P_SPEC_OPERATION:	strcpy(hmi->ActiveBanner , "<< SPECIAL OPER. >>");

				switch(hmi->FiredEvent){
					
					case EVNT_LOAD_DEFAULT:
						
						LoadDefaultParam(m); // load Defaults

						/*
						if(hmi->UserLevel < 10)
							ApplyGenRulesToCurCfg(m);
							*/

						File_RW.NewFileLoaded = 1; // To process android refresh data
						
						CheckCurCfg(m);

						strcpy(hmi->pMessage[0], "  Default");
						strcpy(hmi->pMessage[1], "  Parameters ");
						strcpy(hmi->pMessage[2], "  LOADED");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 2.0);

					break;	

					case EVNT_ALL_VECTORS_SIMUL:

						for(i = 0 ; i < VECTORS_NUM ; i++)
							SetBit(&VectorStateMach[0].simul,2); // simulation ON

						strcpy(hmi->pMessage[0], " ALL Motors ");
						strcpy(hmi->pMessage[1], "  switched to ");
						strcpy(hmi->pMessage[2], " SIMULATION ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 2.0);

						m->sm.Step = 0;
						

					break;
								
					case EVNT_ALL_VECTORS_NO_SIMUL:

						for(i = 0 ; i < VECTORS_NUM ; i++)
							ResetBit(&VectorStateMach[0].simul,2); // simulation OFF

						strcpy(hmi->pMessage[0], " ALL Motors ");
						strcpy(hmi->pMessage[1], "  switched to ");
						strcpy(hmi->pMessage[2], " REAL Mode ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 2.0);

						m->sm.Step = 0;

					break;	

					case EVNT_ALL_PARAM_SEGMENT:

						for(i = 0 ; i < parLAST_PARAM; i++){

							if(cfg->ScaledVar[i].segm)
								curCfg->Param[i].segmYes = 1;
							
							var1 = var+i;
							var1->segm = 1;
							

						}

						hmi->PLC_Changes |= 0x0004; // New Segment List	

						strcpy(hmi->pMessage[0], " ALL PARAM ");
						strcpy(hmi->pMessage[1], "  use ");
						strcpy(hmi->pMessage[2], " segments ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 2.0);

					
					break;	

					case EVNT_ALL_PARAM_NO_SEGMENT:

						for(i = 0 ; i < parLAST_PARAM; i++){

							curCfg->Param[i].segmYes = 0;
							var1 = var+i;
							var1->segm = 0;

						}

						hmi->PLC_Changes |= 0x0004; // New Segment List
							

						strcpy(hmi->pMessage[0], " ALL PARAM ");
						strcpy(hmi->pMessage[1], "  use NON ");
						strcpy(hmi->pMessage[2], " segments ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 2.0);

					
					break;				
					
					case EVNT_ENT_BTN_REQ:

						if(curCfg->Param[parLCD_BtnMode].val[0] > 0.0)
							curCfg->Param[parLCD_BtnMode].val[0]  = 0.0;

						else
							curCfg->Param[parLCD_BtnMode].val[0]  = 1.0;


					break;


					case EVNT_SET_AMERICAN_UNIT:

						for(i = 0 ; i < varLAST_VARIABLE; i++){
							
							var1 = var+i;						
							m->pScaledUnitIndx[i] =	var1->DefaultUnit;
							
						}

						hmi->PLC_Changes |= 0x0002; // New Units
						
						strcpy(hmi->pMessage[0], " Default Units ");
						strcpy(hmi->pMessage[1], "  are Set");
						strcpy(hmi->pMessage[2], "  ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 1.0);

					
					break;	

					case EVNT_SET_NORMAL_UNIT:

						for(i = 0 ; i < varLAST_VARIABLE; i++){
							
							var1 = var+i;						
							m->pScaledUnitIndx[i] =	1;
							
						}

						hmi->PLC_Changes |= 0x0002; // New Units
						
						strcpy(hmi->pMessage[0], " SI Units ");
						strcpy(hmi->pMessage[1], "  are Set");
						strcpy(hmi->pMessage[2], "  ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 1.0);

					
					break;	

					case EVNT_LOAD_DEFAULT_CALIB:

						LoadDefaultCalib(m);

						Process[PROC_AVC].xCMD = prcsRefrCalib;
						Process[PROC_OSC].xCMD = prcsRefrCalib;
						
						strcpy(hmi->pMessage[0], " Default Calibration ");
						strcpy(hmi->pMessage[1], "  is Loaded");
						strcpy(hmi->pMessage[2], "  ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 1.0);

					
					break;	


					

				}

				
				/*
				if(AdjustmentMODE)
					strcpy(nodes[P_SPEC_OPERATION + 3].var.Format , " Normal Mode");
				else
					strcpy(nodes[P_SPEC_OPERATION + 3].var.Format , "Mtr Adjustment Mode");
				*/

				if(curCfg->Param[parLCD_BtnMode].val[0] > 0.0)
					strcpy(nodes[P_SPEC_OPERATION + 5].var.Format , " Hold Btn to edit");
				else
					strcpy(nodes[P_SPEC_OPERATION + 5].var.Format , "NO Hold Btn 2 edit");
					

				
			break;

			/*********************************************************/
			//    P_LOGS
			case P_LOGS:	strcpy(hmi->ActiveBanner , "<<< LOG BOOK >>>");


						
					if(intrf->LCDEntr){

						if(intrf->TickPlus)					
							LogMgr.Cmd.ScrollDwn = 1;
							
						else if(intrf->TickMinus)
							
							LogMgr.Cmd.ScrollUp = 1;
							
						
					}
					

					switch(hmi->curNodeID){


						case P_LOGS:

							 if(intrf->TickMinus)						
								LogMgr.Cmd.ScrollUp = 1;					

							

						break;



						case P_LOGS + 5:

							
							if(intrf->TickPlus)						
								LogMgr.Cmd.ScrollDwn = 1;
													

						break;

							


					}


					for(k=0 ; k < 6 ; k++)
						strcpy(hmi->pHMI_Lines[k] , LogMgr.LogView[k]);

					/*

					p = strchr(LogMgr.LogView[0] , '>');
					
					k = 0;	
					
					memset(hmi->pHMI_Lines[k], 0 , 21);
					memcpy(hmi->pHMI_Lines[k++] ,  LogMgr.LogView[0]+1, p - LogMgr.LogView[0]-1);
					memset(hmi->pHMI_Lines[k], 0 , 19);
					memcpy(hmi->pHMI_Lines[k++] , p+1 , 18);
					memset(hmi->pHMI_Lines[k], 0 , 19);
					memcpy(hmi->pHMI_Lines[k++] , p+19, 18);
					*/
				
					
			break;	

			/*********************************************************/
			//    ERRORS
			case P_ERRORS: strcpy(hmi->ActiveBanner , "<<<  ERRORS >>>");

				if(ErrorMgr.Error == 0)
					strcpy(hmi->ActiveBanner , "<<< No ERRORS >>>");

				/*			
				if(intrf->LCDEntr){
				
					if(intrf->TickPlus) 				
						hmi->Indx++;
						
					else if(intrf->TickMinus)					
						hmi->Indx--;	
					
				}
				*/


				switch(hmi->curNodeID){


					case P_ERRORS:
						
						 if(intrf->TickMinus)						
							hmi->Indx--;	
					break;



					case P_ERRORS + 5:
						
						if(intrf->TickPlus) 					
							hmi->Indx++;
												

					break;

						


				}

				if(hmi->Indx < 1)
					hmi->Indx = 1;
				else if(hmi->Indx > 1){ 
					if(hmi->Indx > (ErrorMgr.lastItemIndx -1))
						hmi->Indx = (ErrorMgr.lastItemIndx -1);
				}	
			
				
				for(i = 0 ; i < 5 ; i++){
				

					if((hmi->Indx + i)<= ErrorMgr.lastItemIndx){

						if((ErrorMgr.history[hmi->Indx + i]>=ERROR_DRV_01)
							&& (ErrorMgr.history[hmi->Indx + i] < ERROR_DRV_41)){

							AssignMtrErrorText(hmi->pHMI_Lines[i] , ErrorMgr.history[hmi->Indx + i] , ErrorMgr.logs[ErrorMgr.history[hmi->Indx + i]].value);

							//xsprintf(hmi->pHMI_Lines[i], "Drv. %s: %X" , TML_MTR_INST[ErrorMgr.history[hmi->Indx + i]-ERROR_TML_1].Name ,\
								//ErrorMgr.logs[ErrorMgr.history[hmi->Indx + i]].value);
						}
						else					
							xsprintf(hmi->pHMI_Lines[i], "%s: %X" , ErrorMgr.logs[ErrorMgr.history[hmi->Indx + i]].txt , ErrorMgr.logs[ErrorMgr.history[hmi->Indx + i]].value);
					}
					else
						strcpy(hmi->pHMI_Lines[i] , " -------------- ");

				}

				if(ErrorMgr.lastItemIndx == 0)
					strcpy(hmi->pHMI_Lines[0] , " No Errors - Ready to GO");
					//strcpy(hmi->pHMI_Lines[0] , ErrorMgr.logs[ERROR_NONE].txt);
				

				if(hmi->FiredEvent == EVNT_ACKN_ALL){

					//if(intrf->GlblBtnCmd[0] && (0xFFFFFFFF - ANDR_BTN_ENTER )){
					if(0){

						strcpy(hmi->pMessage[0], " DO NOT HOLD");
						strcpy(hmi->pMessage[1], " operation button ");
						strcpy(hmi->pMessage[2], " when reset");
						strcpy(hmi->pMessage[3], " errors");
						TimerStart(&hmi->MsgTmr, 1.0);	


					}else{

								
						
						for(i = 0 ; i < TML_SERVO_NUM ; i++){

						
							TML_MTR_INST[i].Error = 0;
							TML_MTR_INST[i].xCMD = 0;

						}						
						
						AckAllErrors(&ErrorMgr);

						//m->sm.Step = 0;


					}

				}	

			break;
			
			/*********************************************************/
			//	  P_VAR_SETUP
			case P_VAR_SETUP:	strcpy(hmi->ActiveBanner , "<< VAR SETUP >>");


			break;	

			/*********************************************************/
			//   P_SEGMENTS
			case P_SEGMENTS:	strcpy(hmi->ActiveBanner , "< Segments >");

				if(segmTable->segm_num == 0){

					strcpy(hmi->pHMI_Lines[0] , " No Parameters");
					strcpy(hmi->pHMI_Lines[1] , " To Be Segmented");
				
					break;
				}	
							
				ScrollIndx(nodes ,hmi,intrf,P_SEGMENTS+1, segmTable->segm_num -1);

				if(intrf->tempIndx >= parLAST_PARAM)
					break;

				k = segmTable->ParamIndx[intrf->tempIndx];
				
				if(k >= parLAST_PARAM)
					break;
					
				var1 = var+ k;

				
				if(hmi->FiredEvent == EVNT_TOGGLE){

					curCfg->Param[k].segmYes = !curCfg->Param[k].segmYes;

					if(curCfg->Param[k].segmYes)
						var1->segm = 1;
					else
						var1->segm = 0;

				}
					
				if(m->Intrf[LOWER_LEVEL].LCDEntr
					&& (hmi->curNodeID == (P_SEGMENTS+1))
					&& hmi->FastBlink)
					xsprintf(hmi->pHMI_Lines[0] , " " );
				else
					xsprintf(hmi->pHMI_Lines[0] , " %s"  ,var1->name);

				if(curCfg->Param[k].segmYes)				
					strcpy(hmi->pHMI_Lines[1] , " Segmented");
				else
					strcpy(hmi->pHMI_Lines[1]  , " Single");
				
								
			break;
			

			/*********************************************************/
			//	  DIAGNOSTIC 
			case P_DIAGNOSTIC:	strcpy(hmi->ActiveBanner , " <<< DIAGNOSTIC >>>");

			
				if(hmi->FiredEvent == EVNT_RESTART)
					m->sm.Step = 0;
				
							
			break;		
			
			/*********************************************************/
			//   P_DIAGN_TML
			case P_DIAGN_TML:	strcpy(hmi->ActiveBanner , "<   TML DRIVES  >");

				//TML_MTR_INST[intrf->tempIndx].pState->testMode = 0;	
				ScrollIndx(nodes ,hmi, intrf  ,P_DIAGN_TML+1  , TML_SERVO_NUM-1);
				//TML_MTR_INST[intrf->tempIndx].pState->testMode = 0;	

				if(hmi->FiredEvent == EVNT_TOGGLE){

					if(TML_MTR_INST[intrf->tempIndx].Disable){
						TML_MTR_INST[intrf->tempIndx].Disable = 0;
						m->sm.Step = 0;
					}
					else
						TML_MTR_INST[intrf->tempIndx].Disable = 1;
				}
				else if(hmi->FiredEvent == EVNT_OK){

				
				
					if(hmi->UserLevel >= 15){// Supervisor


						if(m->ESTOP){

							if(TML_MTR_INST[intrf->tempIndx]._cfg.pAbsLastPos){
								
								(*TML_MTR_INST[intrf->tempIndx]._cfg.pAbsLastPos) = 0;
								AxisCh[intrf->tempIndx].curPos = 0;
								TML_MTR_INST[intrf->tempIndx].xCMD = TML_RESTART;

								strcpy(hmi->pMessage[0], " Zero Home ");
								strcpy(hmi->pMessage[1], " is Set ");
								strcpy(hmi->pMessage[2], "");
								strcpy(hmi->pMessage[3], "");
								TimerStart(&hmi->MsgTmr, 2.0);

							}else{

								strcpy(hmi->pMessage[0], "This motor");
								strcpy(hmi->pMessage[1], " does not");
								strcpy(hmi->pMessage[2], " have ");
								strcpy(hmi->pMessage[3], "Absolute Pos.");
								TimerStart(&hmi->MsgTmr, 2.0);

							}
							

						}else{
							
							strcpy(hmi->pMessage[0], " ESTOP");
							strcpy(hmi->pMessage[1], "Must Be Pressed ");
							strcpy(hmi->pMessage[2], "  to Set");
							strcpy(hmi->pMessage[3], "Absolute Zero");
							TimerStart(&hmi->MsgTmr, 2.0);
						}
						

					}
					else {

						strcpy(hmi->pMessage[0], " Only ");
						strcpy(hmi->pMessage[1], "	SUPERVISOR ");
						strcpy(hmi->pMessage[2], " can performe");
						strcpy(hmi->pMessage[3], " This operation");
						TimerStart(&hmi->MsgTmr, 2.0);

					}
				}

				else if(hmi->FiredEvent == EVNT_LOAD){

					if(hmi->UserLevel >= 15){// Supervisor	


						strcpy(hmi->pMessage[0], " Please ");
						strcpy(hmi->pMessage[1], " Follow");
						strcpy(hmi->pMessage[2], " Banner");
						strcpy(hmi->pMessage[3], " Instructions");

						
						TimerStart(&hmi->MsgTmr, 2.0);
					
						//TML_MTR_INST[intrf->tempIndx].xCMD = TML_DOWNL;
						curTML_indx = intrf->tempIndx;
						TML_Setup_From_Diagn = 1;
						m->sm.Step = 233;

						/*
						if((!m->ESTOP &&  (ProjectConfig == project_US_Orbital))
							|| (m->ESTOP && (ProjectConfig != project_US_Orbital))){

							strcpy(hmi->pMessage[0], " Please ");
							strcpy(hmi->pMessage[1], "	WAIT !!!");
							strcpy(hmi->pMessage[2], "");
							strcpy(hmi->pMessage[3], " Do Not Interrupt");
							TimerStart(&hmi->MsgTmr, 2.0);
						
							//TML_MTR_INST[intrf->tempIndx].xCMD = TML_DOWNL;
							curTML_indx = intrf->tempIndx;
							TML_Setup_From_Diagn = 1;
							//m->sm.Step = 222;
							m->sm.Step = 233;

						}else{

							
							if(ProjectConfig == project_US_Orbital){
								
								strcpy(hmi->pMessage[0], " ESTOP");
								strcpy(hmi->pMessage[1], "Must Be Healthy ");
								strcpy(hmi->pMessage[2], "	to change");
								strcpy(hmi->pMessage[3], " HEAD setup");
								TimerStart(&hmi->MsgTmr, 2.0);

							}else{
								strcpy(hmi->pMessage[0], " ESTOP");
								strcpy(hmi->pMessage[1], "Must Be Pressed ");
								strcpy(hmi->pMessage[2], "  to change");
								strcpy(hmi->pMessage[3], " HEAD setup");
								TimerStart(&hmi->MsgTmr, 2.0);
							}

						}
						*/

					}
					else{

						strcpy(hmi->pMessage[0], " Only ");
						strcpy(hmi->pMessage[1], "	SUPERVISOR ");
						strcpy(hmi->pMessage[2], " can performe");
						strcpy(hmi->pMessage[3], " This operation");
						TimerStart(&hmi->MsgTmr, 2.0);


					}
					
				}
				
				else if(hmi->FiredEvent == EVNT_RESTART){
					
					TML_MTR_INST[intrf->tempIndx].xCMD = TML_RESTART;
				}


				if(TML_MTR_INST[intrf->tempIndx].pState){	
				
					if(intrf->GlblBtnCmd[0] == ANDR_BTN_ENTER){						
									
						TML_MTR_INST[intrf->tempIndx].pState->testMode = 1;							
						
						if(hmi->curNodeID == (P_DIAGN_TML +15)){

							if(TML_MTR_INST[intrf->tempIndx]._cfg.MtrType == 1) // brushless
								TML_MTR_INST[intrf->tempIndx].pState->OutVect.vel = TML_MTR_INST[intrf->tempIndx]._cfg.maxVel * 0.25;

							else
								TML_MTR_INST[intrf->tempIndx].pState->OutVect.volt = 5.0;							

						}						
						else if(hmi->curNodeID == (P_DIAGN_TML +16)){

							if(TML_MTR_INST[intrf->tempIndx]._cfg.MtrType == 1) // brushless
								TML_MTR_INST[intrf->tempIndx].pState->OutVect.vel = TML_MTR_INST[intrf->tempIndx]._cfg.maxVel * (-0.25);

							else
								TML_MTR_INST[intrf->tempIndx].pState->OutVect.volt = -5.0;		
							
						}
						
						else {
							
							TML_MTR_INST[intrf->tempIndx].pState->OutVect.volt = 0.0;
							TML_MTR_INST[intrf->tempIndx].pState->OutVect.vel = 0.0;
						}
							
						if(TML_MTR_INST[intrf->tempIndx]._cfg.MtrType == 1) // brushless
							TML_MTR_INST[intrf->tempIndx].pState->OutVect.Mode = vectSt_Mode_Vel;
						else
							TML_MTR_INST[intrf->tempIndx].pState->OutVect.Mode = vectSt_Mode_Volt;
					}
					else {
						TML_MTR_INST[intrf->tempIndx].pState->testMode = 0;	

					}
								
				}
				
				PickTMLServo(nodes , intrf , hmi, P_DIAGN_TML+1);
				
			break;

			/*********************************************************/
			//   P_DIAGN_PROC
			case P_DIAGN_PROC:	strcpy(hmi->ActiveBanner , "<   PROCESS >");
				
				ScrollIndx(nodes ,hmi, intrf  ,P_DIAGN_PROC+1  , PROCESS_NUM-1);
				
				if(hmi->FiredEvent == EVNT_TOGGLE){

					if(Process[intrf->tempIndx].Disable & 0x01)
						Process[intrf->tempIndx].Disable &= 0xFE; 
					else
						Process[intrf->tempIndx].Disable |= 0x01; 
				}
				

				else if(hmi->FiredEvent == EVNT_RESTART)
					Process[intrf->tempIndx]._step = 0;
			
				PickPROCESS(nodes , intrf , hmi ,  P_DIAGN_PROC+1);
				
			break;
			
			/*********************************************************/
			//   P_DIAGN_AXIS
			case P_DIAGN_AXIS:	strcpy(hmi->ActiveBanner , "<   AXISES >");

				ScrollIndx(nodes ,hmi, intrf  ,P_DIAGN_AXIS+1   , VECTORS_NUM -1);
				
				PickAXIS(nodes , intrf , hmi  , P_DIAGN_AXIS+1);
				
			break;
			

			/*********************************************************/
			//   P_DIAGN_DC_MTR
			case P_DIAGN_DC_MTR:	strcpy(hmi->ActiveBanner , "<  DC Motors >");

				ScrollIndx(nodes ,hmi, intrf  , P_DIAGN_DC_MTR+1   , GEN_MTR_NUM -1);
				
				if(hmi->FiredEvent == EVNT_TOGGLE){

					DC_Mtr[intrf->tempIndx].Disable = !DC_Mtr[intrf->tempIndx].Disable;
					
				}
				
				else if(hmi->FiredEvent == EVNT_RESTART)
					DC_Mtr[intrf->tempIndx].xCmd = 0;


				vect = (StateVectorProxy_typ*) DC_Mtr[intrf->tempIndx].pState;
				
				if(vect && intrf){
					if(intrf->GlblBtnCmd[0] == ANDR_BTN_ENTER){					
									
						vect->testMode = 1;	
						
						if(hmi->curNodeID == (P_DIAGN_DC_MTR + 5)){
							vect->OutVect.volt = 12.0 ;							
							//vect->OutVect.volt = 12.0 * DC_Mtr[intrf->tempIndx].Cfg.mtr_dir;

						}						
						else if(hmi->curNodeID == (P_DIAGN_DC_MTR + 6)){
							vect->OutVect.volt = -12.0 ;
							//vect->OutVect.volt = -12.0 * DC_Mtr[intrf->tempIndx].Cfg.mtr_dir;
						}
						else 
							vect->OutVect.volt = 0.0;
							
						
						vect->OutVect.Mode = vectSt_Mode_Volt;
					}
					else
						vect->testMode = 0;	

				}
			
				PickDCMtr(nodes , intrf , hmi ,P_DIAGN_DC_MTR+1);

			break;

			/*********************************************************/
			//   P_DIAGN_IO
			case P_DIAGN_IO:	strcpy(hmi->ActiveBanner , "<  IO diagn  >");
				
				PickIO( hmi , P_DIAGN_IO + 1);
			break;

			/*********************************************************/
			//   P_MODE_CALIB
			case P_MODE_CALIB:	strcpy(hmi->ActiveBanner , "<  CALIBRATION  >");
				
				intrf->tempIndx = 0;

				m->CalibType = CALIB_NONE;

				/* Nie potrzebne
				curCfg->Param[parGEN_CalibDist].val[0] = 0;
				curCfg->Param[parGEN_CalibTime].val[0] = 0;
				curCfg->Param[parGEN_AmpRead].val[0] = 0;
				*/

				hmi->EditMode = EDIT_MODE_VALUE;

				hmi->CalibStep = 1;
				m->CalibType = 0;
			
			break;

			
			/*******************************************************************/
			case P_MODE_HW_PS_CALIB_PAR:	strcpy(hmi->ActiveBanner , "<HW AMP Calib Par>");

				K1 = curCalib->Points[CALIB_HW_AMPS_CMD].val[3] - curCalib->Points[CALIB_HW_AMPS_CMD].val[1];
				if(curCalib->Points[CALIB_HW_AMPS_CMD].val[2] != curCalib->Points[CALIB_HW_AMPS_CMD].val[0])
					K1 = K1/(curCalib->Points[CALIB_HW_AMPS_CMD].val[2] - curCalib->Points[CALIB_HW_AMPS_CMD].val[0]);		
				
				
				xsprintf(hmi->pHMI_Lines[1] ,"m: %4.1f %s " , K1 * 100.0  , "%"  );
				
				strcpy(hmi->pHMI_Lines[2] , "Load Default Calibr. !!!");
							
				if( (curCfg->Param[parHW_CalibAmp1].val[0] <= 0.0) ||  (curCfg->Param[parHW_CalibAmp2].val[0] <= 0.0)
					||(fabsf(curCfg->Param[parHW_CalibAmp1].val[0] - curCfg->Param[parHW_CalibAmp2].val[0]) < 10.0)){
								
					strcpy(hmi->pHMI_Lines[0] , "Wrong values");						
					status = 0;
					
				}else {
				
					strcpy(hmi->pHMI_Lines[0] , "Start Calibrate");						
					status = 1;
				}				
				
				
				if(status){
				
					if(hmi->FiredEvent == EVNT_OK){
				
						hmi->CalibStepDone = 0;
						
						hmi->Node2JumpID = P_MODE_HW_PS_CALIB;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;
				
						hmi->_ProcStatus = Process[PROC_HW_PS].Status;
					
				
						if(!KK_Functionality){
							
							curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
							
						}else{

							curCfg->Param[parGEN_CalibAmp1].val[0] = curCfg->Param[parHW_CalibAmp1].val[0];
							curCfg->Param[parGEN_CalibAmp2].val[0] = curCfg->Param[parHW_CalibAmp2].val[0];

						}
						
						for(i = 0 ; i < 8; i++){
							hmi->tempCalib.val[i] = 0.0;
							hmi->CalibPoint[i] = 999;
							hmi->CalibPointCmd[i] = 0.0;
						}
				
								
					}
				
				}		
							
				
				 if(hmi->FiredEvent == EVNT_CANCEL){
				
					strcpy(hmi->pMessage[0], " Default");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Loaded ");
					TimerStart(&hmi->MsgTmr, 1.0);								
					
					memcpy(&curCalib->Points[CALIB_HW_AMPS_CMD]  , &defaultCalib->Points[CALIB_HW_AMPS_CMD] , sizeof(hmi->tempCalib));	
				
				}	
				 
			break;


			/*******************************************************************/

			case P_MODE_HW_PS_CALIB:	strcpy(hmi->ActiveBanner , "< HW PS AMP Calib >");

				m->CalibType = CALIB_HW_PS;

				if(hmi->CalibStepDone)
					break;	

				status = 1;
				if((curCfg->Param[parGEN_CalibAmp1].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibAmp2].val[0] <= 0.0))
					status = 0; 			
					
				for(i = 1 ; i < 3; i++){

					if(hmi->CalibPointCmd[i] == 0.0)						
						status = 0;
											
					if(hmi->CalibPoint[i] == 999)						
						status = 0;
						
				}

				i = (hmi->curNodeID -  hmi->curHeadID);
				
				if(status){

					if(!KK_Functionality)
						hmi->CalibStep = 4;
					else{

						if(i < 0)
							i = 0;
						else if(i >= 3)
							i = 4;
						
						hmi->CalibStep = i;


					}
					
				}
				else {

					if(i < 0)
						i = 0;
					else if(i> 3)
						i = 0;
					
					hmi->CalibStep = i;

				}	

											
				switch(hmi->CalibStep){


					case 0:

				
						strcpy(hmi->pHMI_Lines[0] , "Please - Select Amp ");
						strcpy(hmi->pHMI_Lines[1] , "");


					break;

					case 1:		

						xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Amp" , hmi->CalibStep);
						
						if(Process[PROC_HW_PS].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");							
						
					break;	


					case 2: 

						xsprintf(hmi->pHMI_Lines[0] , "(%d) High Amp " , hmi->CalibStep);

						if(Process[PROC_HW_PS].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");	
						
						
					break;

					
					case 3: 

						strcpy(hmi->pHMI_Lines[0] , "");
						strcpy(hmi->pHMI_Lines[1] , "Please - Amp ");
						
					
					break;

					case 4:
							
						strcpy(hmi->pHMI_Lines[0] , "You made all steps. Press To repeat !!!");
						strcpy(hmi->pHMI_Lines[1] , "Calculate New Calibr.");

					break;

				}

				//// Calculation //		

				if(hmi->CalibStep == 4){

					if(hmi->FiredEvent == EVNT_CANCEL){	

						if(!KK_Functionality){
							
							curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
							
						}

						hmi->CalibStepDone = 0;
						
					}
					else if(hmi->FiredEvent == EVNT_OK){					
						

						if(!status){

							strcpy(hmi->pMessage[0], " Some ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], " is not taken ");
							strcpy(hmi->pMessage[3], " or ZERO");
							TimerStart(&hmi->MsgTmr, 1.0);

							hmi->CalibStepDone = 0;

							break;

						}	

						

						// 	
						v1_0 = curCfg->Param[parGEN_CalibAmp1].val[0]; // Low
						v2_0 = curCfg->Param[parGEN_CalibAmp2].val[0]; // High
						
						if((v1_0 >= v2_0 )
							|| (v1_0 <= 0.0)
							|| (v2_0 <= 0.0)){
							
							strcpy(hmi->pMessage[0], " Wrong  ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], "");
							strcpy(hmi->pMessage[3], " ");
							TimerStart(&hmi->MsgTmr, 1.0);

							break;

						}	

						///////////////////////////
						//  Calculation 
						// Low Amp								
						hmi->tempCalib.val[hmi->CalibPoint[1]] = v1_0; // x1
						hmi->tempCalib.val[hmi->CalibPoint[1]+1] = hmi->CalibPointCmd[1]; // y1

						// High Amp	
						hmi->tempCalib.val[hmi->CalibPoint[2]] = v2_0; // x2
						hmi->tempCalib.val[hmi->CalibPoint[2]+1] = hmi->CalibPointCmd[2]; // y2
						
						x = curCfg->Param[parHW_CalibAmp2].val[0] - curCfg->Param[parHW_CalibAmp1].val[0]; 
						y = v2_0 - v1_0;
						K1 = x/y;
						m1= fabsf(K1-hmi->oldK1)/K1;
						
						// Must be 0.01 because when zero getcalib also zero
						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_HW_AMPS_CMD]  ,CALIB_2POINTS );

						b1 = fabsf((x-y)/x);
						//b1 = x;

						xsprintf(hmi->pHMI_Lines[0] ,"error/ m: %4.1f %s / b: %4.1f %s" , fabsf(K1-1.0) * 100.0  , "%" , b1 , "%" );
											
					
						if(hmi->oldK1 == 0.0)
							strcpy(hmi->pHMI_Lines[1] , "First Calibration ");
						else
							xsprintf(hmi->pHMI_Lines[1] ,"Repeat: m: %4.1f %s " , (m1 * 100.0) , "%" ); 
						
										
						hmi->oldK1 = K1;

						/////////////////////////					
						strcpy(hmi->pHMI_Lines[3] , "");
						strcpy(hmi->pHMI_Lines[4] , "");

						/*
						strcpy(hmi->pMessage[0], " Wait ");
						strcpy(hmi->pMessage[1], " for");
						strcpy(hmi->pMessage[2], " New Calibration");
						strcpy(hmi->pMessage[3], "");
						TimerStart(&hmi->MsgTmr, 1.0);
						*/
						hmi->CalibStepDone = 1;
						
						hmi->Node2JumpID = P_MODE_HW_PS_CALIB_VER;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;
						
					}
				}
				

					/////////////////////////////

				


					
			break;
				
			/*************************************************************/
			case P_MODE_HW_PS_CALIB_VER:	strcpy(hmi->ActiveBanner , "<HW AMP Verif >");

				m->CalibType = CALIB_HW_PS;
					
				if(hmi->FiredEvent == EVNT_CANCEL){					

					hmi->_ProcStatus = Process[PROC_HW_PS].Status;					

					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
						
					}					
			
					hmi->CalibStepDone = 0;

					for(i = 0 ; i < 8; i++){
						hmi->tempCalib.val[i] = 0.0;
						hmi->CalibPoint[i] = 999;
						hmi->CalibPointCmd[i] = 0.0;
					}
					
					hmi->Node2JumpID = P_MODE_HW_PS_CALIB;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					//m->ToolHeadID = 0; // REV


				}				
				else if(hmi->FiredEvent == EVNT_OK){						

					memcpy( &curCalib->Points[CALIB_HW_AMPS_CMD] , &hmi->tempCalib , sizeof(hmi->tempCalib));						

					hmi->_ProcStatus = Process[PROC_HW_PS].Status;
					
					hmi->CalibStepDone = 0;

					hmi->Node2JumpID = P_MODE_HW_PS_CALIB_PAR;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
						
					}	

					strcpy(hmi->pMessage[0], " ");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Saved ");
					TimerStart(&hmi->MsgTmr, 1.0);


				}			

				
							
			break;

			/*******************************************************************/
			case P_MODE_TIG_PS_CALIB_PAR:	strcpy(hmi->ActiveBanner , "<TIG AMP Calib Par>");		
				
			
			K1 = curCalib->Points[CALIB_TIG_AMPS_CMD].val[3] - curCalib->Points[CALIB_TIG_AMPS_CMD].val[1];
			if(curCalib->Points[CALIB_TIG_AMPS_CMD].val[2] != curCalib->Points[CALIB_TIG_AMPS_CMD].val[0])
				K1 = K1/(curCalib->Points[CALIB_TIG_AMPS_CMD].val[2] - curCalib->Points[CALIB_TIG_AMPS_CMD].val[0]);		
			
			
			xsprintf(hmi->pHMI_Lines[1] ,"m: %4.1f %s " , K1 * 100.0  , "%"  );
			
			strcpy(hmi->pHMI_Lines[2] , "Load Default Calibr. !!!");
						
			if( (curCfg->Param[parTIG_CalibAmp1].val[0] <= 0.0) ||  (curCfg->Param[parTIG_CalibAmp2].val[0] <= 0.0)
				||(fabsf(curCfg->Param[parTIG_CalibAmp1].val[0] - curCfg->Param[parTIG_CalibAmp2].val[0]) < 10.0)){
							
				strcpy(hmi->pHMI_Lines[0] , "Wrong values ");						
				status = 0;
				
			}else {
			
				strcpy(hmi->pHMI_Lines[0] , " Start Calibrate ");						
				status = 1;
			}				
			
			
			if(status){
			
				if(hmi->FiredEvent == EVNT_OK){
			
					hmi->CalibStepDone = 0;
					
					hmi->Node2JumpID = P_MODE_TIG_PS_CALIB;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;
			
					hmi->_ProcStatus = Process[PROC_TIG_PS].Status;
				
			
					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
						
					}	
					else{

						curCfg->Param[parGEN_CalibAmp1].val[0] = curCfg->Param[parTIG_CalibAmp1].val[0];
						curCfg->Param[parGEN_CalibAmp2].val[0] = curCfg->Param[parTIG_CalibAmp2].val[0];

					}						
					
					for(i = 0 ; i < 8; i++){
						hmi->tempCalib.val[i] = 0.0;
						hmi->CalibPoint[i] = 999;
						hmi->CalibPointCmd[i] = 0.0;
					}
			
							
				}
			
			}		
						
			
			 if(hmi->FiredEvent == EVNT_CANCEL){
			
				strcpy(hmi->pMessage[0], " Default");
				strcpy(hmi->pMessage[1], " Calib ");
				strcpy(hmi->pMessage[2], " is");
				strcpy(hmi->pMessage[3], " Loaded ");
				TimerStart(&hmi->MsgTmr, 1.0);								
				
				memcpy(&curCalib->Points[CALIB_TIG_AMPS_CMD]  , &defaultCalib->Points[CALIB_TIG_AMPS_CMD] , sizeof(hmi->tempCalib));	
			
			}							
			
			
							
			break;

			
			/*******************************************************************/

			case P_MODE_TIG_PS_CALIB:	strcpy(hmi->ActiveBanner , "< TIG PS AMP Calib >");

				m->CalibType = CALIB_TIG_PS;

				if(hmi->CalibStepDone)
					break;		


				status = 1;
				if((curCfg->Param[parGEN_CalibAmp1].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibAmp2].val[0] <= 0.0))
					status = 0; 			
					
				for(i = 1 ; i < 3; i++){

					if(hmi->CalibPointCmd[i] == 0.0)						
						status = 0;
											
					if(hmi->CalibPoint[i] == 999)						
						status = 0;
						
				}

				i = (hmi->curNodeID -  hmi->curHeadID);
				
				if(status){

					if(!KK_Functionality)
						hmi->CalibStep = 4;
					else{

						if(i < 0)
							i = 0;
						else if(i >= 3)
							i = 4;
						
						hmi->CalibStep = i;


					}
					
				}
				else {

					if(i < 0)
						i = 0;
					else if(i> 3)
						i = 0;
					
					hmi->CalibStep = i;

				}	

				xsprintf(hmi->pHMI_Lines[1] , "Act Volt: %4.2f " , Process[m->pCfg->HeadCfg.AVC.PROC_Indx].Fbk[0]);
											
				switch(hmi->CalibStep){


					case 0:

				
						strcpy(hmi->pHMI_Lines[0] , "Please - Select Amp ");
						strcpy(hmi->pHMI_Lines[1] , "");


					break;

					case 1:		

						xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Amp" , hmi->CalibStep);
						
						if(Process[PROC_TIG_PS].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");							
						
					break;	


					case 2: 

						xsprintf(hmi->pHMI_Lines[0] , "(%d) High Amp " , hmi->CalibStep);

						if(Process[PROC_TIG_PS].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");	
						
						
					break;

					
					case 3: 

						strcpy(hmi->pHMI_Lines[0] , "");
						strcpy(hmi->pHMI_Lines[1] , "Please - Amp ");
						
					
					break;

					case 4:
							
						strcpy(hmi->pHMI_Lines[0] , "You made all steps. Press To repeat !!!");
						strcpy(hmi->pHMI_Lines[1] , "Calculate New Calibr.");

					break;

				}

				//// Calculation //		

				if(hmi->CalibStep == 4){

					if(hmi->FiredEvent == EVNT_CANCEL){	

						if(!KK_Functionality){
							
							curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
							
						}

						hmi->CalibStepDone = 0;
						
					}
					else if(hmi->FiredEvent == EVNT_OK){									
					
						
						if(!status){

							strcpy(hmi->pMessage[0], " Some ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], " is not taken ");
							strcpy(hmi->pMessage[3], " or ZERO");
							TimerStart(&hmi->MsgTmr, 1.0);

							hmi->CalibStepDone = 0;

							break;

						}	
						

						// 	
						v1_0 = curCfg->Param[parGEN_CalibAmp1].val[0]; // Low
						v2_0 = curCfg->Param[parGEN_CalibAmp2].val[0]; // High
						
						if((v1_0 >= v2_0 )
							|| (v1_0 <= 0.0)
							|| (v2_0 <= 0.0)){
							
							strcpy(hmi->pMessage[0], " Wrong  ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], "");
							strcpy(hmi->pMessage[3], " ");
							TimerStart(&hmi->MsgTmr, 1.0);

							break;

						}	

						///////////////////////////
						//  Calculation 
						// Low Amp								
						hmi->tempCalib.val[hmi->CalibPoint[1]] = v1_0; // x1
						hmi->tempCalib.val[hmi->CalibPoint[1]+1] = hmi->CalibPointCmd[1]; // y1

						// High Amp	
						hmi->tempCalib.val[hmi->CalibPoint[2]] = v2_0; // x2
						hmi->tempCalib.val[hmi->CalibPoint[2]+1] = hmi->CalibPointCmd[2]; // y2
						
						x = curCfg->Param[parTIG_CalibAmp2].val[0] - curCfg->Param[parTIG_CalibAmp1].val[0]; 
						y = v2_0 - v1_0;
						K1 = x/y;
						m1= fabsf(K1-hmi->oldK1)/K1;
						
						// Must be 0.01 because when zero getcalib also zero
						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_TIG_AMPS_CMD]  ,CALIB_2POINTS );

						b1 = fabsf((x-y)/x);
						//b1 = x;

						xsprintf(hmi->pHMI_Lines[0] ,"error/ m: %4.1f %s / b: %4.1f %s" , fabsf(K1-1.0) * 100.0  , "%" , b1 , "%" );

						
						if(hmi->oldK1 == 0.0)
							strcpy(hmi->pHMI_Lines[1] , "First Calibration ");
						else
							xsprintf(hmi->pHMI_Lines[1] ,"Repeat: m: %4.1f %s " , (m1 * 100.0) , "%" ); 
						
										
						hmi->oldK1 = K1;
						
						/////////////////////////					
						strcpy(hmi->pHMI_Lines[3] , "");
						strcpy(hmi->pHMI_Lines[4] , "");

						/*
						strcpy(hmi->pMessage[0], " Wait ");
						strcpy(hmi->pMessage[1], " for");
						strcpy(hmi->pMessage[2], " New Calibration");
						strcpy(hmi->pMessage[3], "");
						TimerStart(&hmi->MsgTmr, 1.0);
						*/
						hmi->CalibStepDone = 1;
						
						hmi->Node2JumpID = P_MODE_TIG_PS_CALIB_VER;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;
						
					}
				}
				

					/////////////////////////////

				

				
			break;
			
			/*************************************************************/
			case P_MODE_TIG_PS_CALIB_VER:	strcpy(hmi->ActiveBanner , "< TIG PS AMP Verif >");

				m->CalibType = CALIB_TIG_PS;


				if(hmi->FiredEvent == EVNT_CANCEL){					

					hmi->_ProcStatus = Process[PROC_TIG_PS].Status;					

					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
						
					}					
			
					hmi->CalibStepDone = 0;

					for(i = 0 ; i < 8; i++){
						hmi->tempCalib.val[i] = 0.0;
						hmi->CalibPoint[i] = 999;
						hmi->CalibPointCmd[i] = 0.0;
					}
					
					hmi->Node2JumpID = P_MODE_TIG_PS_CALIB;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					//m->ToolHeadID = 0; // REV


				}				
				else if(hmi->FiredEvent == EVNT_OK){						

					memcpy( &curCalib->Points[CALIB_TIG_AMPS_CMD] , &hmi->tempCalib , sizeof(hmi->tempCalib));						

					hmi->_ProcStatus = Process[PROC_TIG_PS].Status;
					
					hmi->CalibStepDone = 0;

					hmi->Node2JumpID = P_MODE_TIG_PS_CALIB_PAR;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibAmp1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibAmp2].val[0] = 0.0;
						
					}	

					strcpy(hmi->pMessage[0], " ");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Saved ");
					TimerStart(&hmi->MsgTmr, 1.0);


				}			

				
				

						
			break;

			/*************************************************************/
			case P_MODE_TIG_FBK_CALIB:	strcpy(hmi->ActiveBanner , "TIG Volt FBK Calib");

				m->CalibType = CALIB_TIG_VOLT_FBK;
				
				ScrollIndx(nodes ,hmi, intrf ,P_MODE_TIG_FBK_CALIB+2, 10);

				if(intrf->tempIndx == 0){
					
					xsprintf(hmi->pHMI_Lines[0] , " ");
					strcpy(hmi->pHMI_Lines[1] , "Start Calibr");
				}
				
				else {		
					
					xsprintf(hmi->pHMI_Lines[0] , "Set %d.0 VOLT"  , (intrf->tempIndx - 1) * 6 );
					strcpy(hmi->pHMI_Lines[1] , " STORE Point !!!");

				}
						
				if(hmi->FiredEvent == EVNT_SET_TIG_FBK_CALIB_POINT){

					if(intrf->tempIndx == 0){

						
						memcpy(&hmi->tempCalib , &defaultCalib->Points[CALIB_TIG_VOLT_FBK] , sizeof( Calib_Param_typ)); 
											
						intrf->tempIndx++;
						
						strcpy(hmi->pMessage[0], " New Calibr ");
						strcpy(hmi->pMessage[1], "  started");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");				
						TimerStart(&hmi->MsgTmr, 1.0);
						
					}
					else{

						// This was change July 25 2024
						// All legacy heads use mode 0 and read voltage sense from analog input B&R card
						if(AVC_Param_Set[0].Mode == 0){

							if(ProjectConfig == project_US_Orbital)
									fbkVect_x = ((float)Hardware[1].Anal_Inp[1] / 32767) * 10.0; 
								else
									fbkVect_x = ((float)TML_MTR_INST[2].AnInp[1] / 4095.0) * 5.0; 


						}
						else							
							fbkVect_x = VectorStateMach[VECT_AVC].FbkVect.x;
						
						/*
						switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

							default:

								fbkVect_x = VectorStateMach[VECT_AVC].FbkVect.x;

							break;


							case F_HEAD_MINI_STND:
							case RUBIX:

								if(ProjectConfig == project_US_Orbital)
									fbkVect_x = ((float)Hardware[1].Anal_Inp[1] / 32767) * 10.0; 
								else
									fbkVect_x = ((float)TML_MTR_INST[2].AnInp[1] / 4095.0) * 5.0; 

								 
							break;
						}
						*/

						
						
						if((fabsf(fbkVect_x - defaultCalib->Points[CALIB_TIG_VOLT_FBK].val[intrf->tempIndx - 1]) < 6.0)
								&& ((hmi->tempCalib.val[intrf->tempIndx - 2] < fbkVect_x) ||( intrf->tempIndx < 2) )){ // Every valuse must be greater
						
							hmi->tempCalib.val[intrf->tempIndx - 1] = fbkVect_x;

							//hmi->tempCalib.val[0] = 0.0 ;  // Always start from zero 						 

							if(intrf->tempIndx+1 <= 4){

							
								
								intrf->tempIndx++;

								strcpy(hmi->pMessage[0], " Calibration ");
								strcpy(hmi->pMessage[1], " ");
								strcpy(hmi->pMessage[2], "   Point SET ");
								strcpy(hmi->pMessage[3], " Take next one");				
								TimerStart(&hmi->MsgTmr, 1.0);

							}
							else{

								strcpy(hmi->pMessage[0], " Calibration ");
								strcpy(hmi->pMessage[1], "    DONE ");
								strcpy(hmi->pMessage[2], "   Thanks");
								strcpy(hmi->pMessage[3], " ");				
								TimerStart(&hmi->MsgTmr, 2.0);
								
								hmi->tempCalib.num = 4;
								memcpy(&curCalib->Points[CALIB_TIG_VOLT_FBK] , &hmi->tempCalib  , sizeof( Calib_Param_typ)); 
								
								Process[PROC_AVC].xCMD = prcsRefrCalib;
								intrf->tempIndx = 0;

								/*
								intrf->TickPlus = 0;				
								hmi->Node2JumpID = P_MODE_CALIB;
								hmi->JumpLineID = 0;
								hmi->xCMD = JUMP2ID_CMD;
								*/


							}
							
						}
						else{

							
							strcpy(hmi->pMessage[0], " Calibration ");
							strcpy(hmi->pMessage[1], "   Value ");
							strcpy(hmi->pMessage[2], " is out of sense");
							strcpy(hmi->pMessage[3], "   TRY AGAIN ");				
							TimerStart(&hmi->MsgTmr, 2.0);



						}
					}
					

			
				}
				else if(hmi->FiredEvent == EVNT_LOAD_DEFAULT_CALIB){


					memcpy(&curCalib->Points[CALIB_TIG_VOLT_FBK] , &defaultCalib->Points[CALIB_TIG_VOLT_FBK]  , sizeof( Calib_Param_typ)); 

					strcpy(hmi->pMessage[0], "   Default ");
					strcpy(hmi->pMessage[1], " Calibration ");
					strcpy(hmi->pMessage[2], "    was ");
					strcpy(hmi->pMessage[3], "   Loaded ");				
					TimerStart(&hmi->MsgTmr, 2.0);

					Process[PROC_AVC].xCMD = prcsRefrCalib;
					intrf->tempIndx = 0;
					

				}
			
			

			break;

			/*************************************************************/
			case P_MODE_OSC_LIMIT_CALIB:	strcpy(hmi->ActiveBanner , "OSC Limit Calib");	


				if((curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_TT_BO_TOS)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGH)
				|| (curCalib->GenSetup[SETUP_HEAD_SETUP] == BPV_v2_MPGV)){


					strcpy(hmi->pHMI_Lines[0] , "TVL (OSC) Limits ");

				}
				else{


					if(m->pCfg->HeadCfg.OSC.TML_A_Indx == m->pCfg->HeadCfg.OSC.TML_B_Indx)
						strcpy(hmi->pHMI_Lines[0] , " Head OSC Limits ");

					else {
				
						if(m->ToolHeadID)
							strcpy(hmi->pHMI_Lines[0] , "Head: BB");
						else
							strcpy(hmi->pHMI_Lines[0] , "Head: AA");

					}

				}
					
				

			break;	


			/*************************************************************/
			case P_PARAM_INDX :	

				strcpy(hmi->ActiveBanner , "INDEX");	
				
				switch(hmi->curNodeID){

					default:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 0;
						
					break;

					case P_PARAM_INDX + 5:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 1;

					break;

					case P_PARAM_INDX + 6:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 2;

					break;		


				}
					
				

			break;	

			/*************************************************************/
			case P_MODE_INDX_CALIB :	

				strcpy(hmi->ActiveBanner , "INDX Calib");	
				
				switch(hmi->curNodeID){

					default:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 0;
						
					break;

					case P_MODE_INDX_CALIB + 1:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 1;

					break;

					case P_MODE_INDX_CALIB + 2:

						Process[m->pCfg->HeadCfg.INDX.PROC_Indx].Param[3] = 2;

					break;		


				}
					
				

			break;	
					
			/*************************************************************/

			
			case P_MODE_WF_CALIB_PAR:	strcpy(hmi->ActiveBanner , "<WF Calib Par >");

				m->CalibType = CALIB_NONE;

			
				K1 = curCalib->Points[CALIB_WF_SPEED_CMD].val[3] - curCalib->Points[CALIB_WF_SPEED_CMD].val[1];
				if(curCalib->Points[CALIB_WF_SPEED_CMD].val[2] != curCalib->Points[CALIB_WF_SPEED_CMD].val[0])
					K1 = K1/(curCalib->Points[CALIB_WF_SPEED_CMD].val[2] - curCalib->Points[CALIB_WF_SPEED_CMD].val[0]);

				K2 = curCalib->Points[CALIB_WF_SPEED_CMD].val[3+4] - curCalib->Points[CALIB_WF_SPEED_CMD].val[1+4];
				if(curCalib->Points[CALIB_WF_SPEED_CMD].val[2+4] != curCalib->Points[CALIB_WF_SPEED_CMD].val[0+4])
					K2 = K2/(curCalib->Points[CALIB_WF_SPEED_CMD].val[2+4] - curCalib->Points[CALIB_WF_SPEED_CMD].val[0+4]);

				xsprintf(hmi->pHMI_Lines[1] ,"A: %4.1f %s / B: %4.1f %s" , K2 * 100.0  , "%" , K1 * 100.0 , "%" );

				strcpy(hmi->pHMI_Lines[2] , "Load Default Calibr. !!!");
							
				if( (curCfg->Param[parWF_CalibVel1].val[0] <= 0.0) ||  (curCfg->Param[parWF_CalibVel2].val[0] <= 0.0)
					||(fabsf(curCfg->Param[parWF_CalibVel1].val[0] - curCfg->Param[parWF_CalibVel2].val[0]) < 10.0)
					|| (curCfg->Param[parGEN_CalibDist].val[0] <= 0.0)){

					strcpy(hmi->pHMI_Lines[0] , "Wrong values ");						
					status = 0;
					
				}else {
				
					strcpy(hmi->pHMI_Lines[0] , " Start Calibrate ");						
					status = 1;
				}				
				

				if(status){

					if(hmi->FiredEvent == EVNT_OK){

						hmi->CalibStepDone = 0;
						
						hmi->Node2JumpID = P_MODE_WF_CALIB;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;

						hmi->_ProcStatus = Process[PROC_WIRE_FEED].Status;
					

						if(!KK_Functionality){
							
							curCfg->Param[parGEN_CalibDist1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist2].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist3].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist4].val[0] = 0.0;
							
						}else{

							curCfg->Param[parGEN_CalibDist1].val[0] = curCfg->Param[parGEN_CalibDist].val[0];
							curCfg->Param[parGEN_CalibDist2].val[0] = curCfg->Param[parGEN_CalibDist_2].val[0];
							curCfg->Param[parGEN_CalibDist3].val[0] = curCfg->Param[parGEN_CalibDist].val[0];
							curCfg->Param[parGEN_CalibDist4].val[0] = curCfg->Param[parGEN_CalibDist_2].val[0];

						}
							

						curCfg->Param[parGEN_CalibTime1].val[0] =  curCfg->Param[parGEN_CalibDist].val[0] / curCfg->Param[parWF_CalibVel1].val[0];
						curCfg->Param[parGEN_CalibTime2].val[0] =  curCfg->Param[parGEN_CalibDist_2].val[0] / curCfg->Param[parWF_CalibVel2].val[0];
					
						for(i = 0 ; i < 8; i++){
							hmi->tempCalib.val[i] = 0.0;
							hmi->CalibPoint[i] = 999;
							hmi->CalibPointCmd[i] = 0.0;
						}

						//m->ToolHeadID = 0; // REV


					}

				}		
							

				 if(hmi->FiredEvent == EVNT_CANCEL){

					strcpy(hmi->pMessage[0], " Default");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Loaded ");
					TimerStart(&hmi->MsgTmr, 1.0);								
					
					memcpy(&curCalib->Points[CALIB_WF_SPEED_CMD]  , &defaultCalib->Points[CALIB_WF_SPEED_CMD] , sizeof(hmi->tempCalib));	

				}							
				

			break;
			
			/*************************************************************/
			case P_MODE_WF_CALIB:	strcpy(hmi->ActiveBanner , "< WF AMP Calib >");

				m->CalibType = CALIB_WF;

				if(hmi->CalibStepDone)
					break;		


				
				status = 1;
				if((curCfg->Param[parGEN_CalibDist1].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibDist2].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibDist3].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibDist4].val[0] <= 0.0))
					status = 0; 			
					
				for(i = 1 ; i < 5; i++){

					if(hmi->CalibPointCmd[i] == 0.0)						
						status = 0;
											
					if(hmi->CalibPoint[i] == 999)						
						status = 0;
						
				}

				i = (hmi->curNodeID -  hmi->curHeadID);
				
				if(status){

					if(!KK_Functionality)
						hmi->CalibStep = 6;
					else{

						if(i < 0)
							i = 0;
						else if(i >= 5)
							i = 6;
						
						hmi->CalibStep = i;


					}
					
				}
				else {

					if(i < 0)
						i = 0;
					else if(i> 5)
						i = 0;
					
					hmi->CalibStep = i;

				}	

											
				switch(hmi->CalibStep){


					case 0:

				
						strcpy(hmi->pHMI_Lines[0] , "Please - Select Speed and Direction ");
						strcpy(hmi->pHMI_Lines[1] , "");


					break;

					case 1:		

						xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Vel. Head A" , hmi->CalibStep);
						
						if(Process[PROC_WIRE_FEED].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");
							
						m->ToolHeadID = 0;	
										
					break;	


					case 2: 

						xsprintf(hmi->pHMI_Lines[0] , "(%d) High Vel. Head A " , hmi->CalibStep);

						if(Process[PROC_WIRE_FEED].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");
						
						m->ToolHeadID = 0;
						
					break;

					case 3: 

						xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Vel. Head B" , hmi->CalibStep);

						if(Process[PROC_WIRE_FEED].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");
						
						m->ToolHeadID = 1;
						
					break;


					case 4: 

						xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Vel. Head B" , hmi->CalibStep);

						if(Process[PROC_WIRE_FEED].Status == prcsSTATUS_Stopped)
							strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
						else
							strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");
						
						m->ToolHeadID = 1;
						
					break;

					case 5: 

						strcpy(hmi->pHMI_Lines[0] , "");
						strcpy(hmi->pHMI_Lines[1] , "Please - Select Speed and Direction ");
						
					
					break;

					case 6:
							
						strcpy(hmi->pHMI_Lines[0] , "You made all steps. Press To repeat !!!");
						strcpy(hmi->pHMI_Lines[1] , "Calculate New Calibr.");

					break;

				}

				//// Calculation //		

				if(hmi->CalibStep == 6){

					if(hmi->FiredEvent == EVNT_CANCEL){	

						if(!KK_Functionality){
							
							curCfg->Param[parGEN_CalibDist1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist2].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist3].val[0] = 0.0;
							curCfg->Param[parGEN_CalibDist4].val[0] = 0.0;
						}

						hmi->CalibStepDone = 0;
						
					}
					else if(hmi->FiredEvent == EVNT_OK){									
					
						
						if(!status){

							strcpy(hmi->pMessage[0], " Some ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], " is not taken ");
							strcpy(hmi->pMessage[3], " or ZERO");
							TimerStart(&hmi->MsgTmr, 1.0);

							hmi->CalibStepDone = 0;

							break;

						}							

						// Head A	
						v1_0 = curCfg->Param[parGEN_CalibDist1].val[0]/curCfg->Param[parGEN_CalibTime1].val[0]; // Low
						v2_0 = curCfg->Param[parGEN_CalibDist2].val[0]/curCfg->Param[parGEN_CalibTime2].val[0]; // High
						// Head B
						v1_1 = curCfg->Param[parGEN_CalibDist3].val[0]/curCfg->Param[parGEN_CalibTime1].val[0]; // Low
						v2_1 = curCfg->Param[parGEN_CalibDist4].val[0]/curCfg->Param[parGEN_CalibTime2].val[0]; // High

						if((v1_0 >= v2_0 )
							|| (v1_0 >= v2_0 )){
							
							strcpy(hmi->pMessage[0], " Speed  ");
							strcpy(hmi->pMessage[1], " Low");
							strcpy(hmi->pMessage[2], " is Higher ");
							strcpy(hmi->pMessage[3], " then Low");
							TimerStart(&hmi->MsgTmr, 1.0);

							break;

						}	

						///////////////////////////
						//  Calculation 
						// Low Speed								
						hmi->tempCalib.val[hmi->CalibPoint[1]] = v1_0; // x1
						hmi->tempCalib.val[hmi->CalibPoint[1]+1] = hmi->CalibPointCmd[1]; // y1
						hmi->tempCalib.val[hmi->CalibPoint[3]] = v1_1; // x2
						hmi->tempCalib.val[hmi->CalibPoint[3]+1] = hmi->CalibPointCmd[3]; // y2

						//High Speed
						hmi->tempCalib.val[hmi->CalibPoint[2]] = v2_0;
						hmi->tempCalib.val[hmi->CalibPoint[2]+1] = hmi->CalibPointCmd[2];
						hmi->tempCalib.val[hmi->CalibPoint[4]] = v2_1;
						hmi->tempCalib.val[hmi->CalibPoint[4]+1]= hmi->CalibPointCmd[4];

						
						x = curCfg->Param[parWF_CalibVel2].val[0] - curCfg->Param[parWF_CalibVel1].val[0]; 
						y = v2_0 - v1_0;
						K1 = x/y;
						m1 = fabs(K1 - hmi->oldK1)/K1;	

						
						y = v2_1 - v1_1;
						K2 = x/y;
						m2 = fabs(K1 - hmi->oldK1)/K2;	

						// Must be 0.01 because when zero getcalib also zero
						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_WF_SPEED_CMD]  ,CALIB_2POINTS );

						b1 = fabsf((x-y)/x);
						//b1 = x;

						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS_4OFF );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_WF_SPEED_CMD]  ,CALIB_2POINTS_4OFF );

						b2 = fabsf((x-y)/x);
						//b2 = x;

						xsprintf(hmi->pHMI_Lines[0] ,"error A/ m: %4.1f %s / b: %4.1f %s" , fabsf(K1-1.0) * 100.0  , "%" , b1 , "%" );
						xsprintf(hmi->pHMI_Lines[1] ,"error B/ m: %4.1f %s / b: %4.1f %s" , fabsf(K2-1.0) * 100.0 , "%" , b2 ,  "%" ); 

						if(hmi->oldK1 == 0.0 
							|| hmi->oldK2 == 0.0 )
							strcpy(hmi->pHMI_Lines[2] , "First Calibration ");
						else
							xsprintf(hmi->pHMI_Lines[2] ,"Repeat: A: %4.1f %s / B: %4.1f %s" , (m1 * 100.0) , "%" , (m2*100.0) ,  "%" ); 
						
										
						hmi->oldK1 = K1;
						hmi->oldK2 = K2;						

						//xsprintf(hmi->pHMI_Lines[3] ,"t1: %4.1f t2: %4.1f" , curCfg->Param[parGEN_CalibTime1].val[0] ,curCfg->Param[parGEN_CalibTime2].val[0]);

						/////////////////////////					
						//strcpy(hmi->pHMI_Lines[3] , "");
						strcpy(hmi->pHMI_Lines[4] , "");

						/*
						strcpy(hmi->pMessage[0], " Wait ");
						strcpy(hmi->pMessage[1], " for");
						strcpy(hmi->pMessage[2], " New Calibration");
						strcpy(hmi->pMessage[3], "");
						TimerStart(&hmi->MsgTmr, 1.0);
						*/
						hmi->CalibStepDone = 1;
						
						hmi->Node2JumpID = P_MODE_WF_CALIB_VER;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;
						
					}
				}
				

					/////////////////////////////

			break;
			
			/*************************************************************/
			case P_MODE_WF_CALIB_VER:	strcpy(hmi->ActiveBanner , "< WF Speed Verif >");

				m->CalibType = CALIB_WF;
				
				if(hmi->FiredEvent == EVNT_CANCEL){					

					hmi->_ProcStatus = Process[PROC_WIRE_FEED].Status;					

					if(!KK_Functionality){
							
						curCfg->Param[parGEN_CalibDist1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist2].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist3].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist4].val[0] = 0.0;
					}
					
					hmi->_ProcStatus = Process[PROC_TVL].Status; 

					hmi->CalibStepDone = 0;

					for(i = 0 ; i < 8; i++){
						hmi->tempCalib.val[i] = 0.0;
						hmi->CalibPoint[i] = 999;
						hmi->CalibPointCmd[i] = 0.0;
					}
					
					hmi->Node2JumpID = P_MODE_WF_CALIB;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					//m->ToolHeadID = 0; // REV


				}				
				else if(hmi->FiredEvent == EVNT_OK){						

					memcpy( &curCalib->Points[CALIB_WF_SPEED_CMD] , &hmi->tempCalib , sizeof(hmi->tempCalib));						

					hmi->_ProcStatus = Process[PROC_WIRE_FEED].Status;
					
					hmi->CalibStepDone = 0;

					hmi->Node2JumpID = P_MODE_WF_CALIB_PAR;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					if(!KK_Functionality){
							
						curCfg->Param[parGEN_CalibDist1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist2].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist3].val[0] = 0.0;
						curCfg->Param[parGEN_CalibDist4].val[0] = 0.0;
					}

					strcpy(hmi->pMessage[0], " ");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Saved ");
					TimerStart(&hmi->MsgTmr, 1.0);


				}			

				

						
			break;
			
			/*************************************************************/			
			case P_MODE_TVL_CALIB_PAR:	strcpy(hmi->ActiveBanner , "< TVL Calib Par >");

				m->CalibType = CALIB_NONE;

				K1 = curCalib->Points[CALIB_TVL_SPEED_CMD].val[3] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[1];
				if(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2] != curCalib->Points[CALIB_TVL_SPEED_CMD].val[0])
					K1 = K1/(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[0]);

				/*
				K2 = curCalib->Points[CALIB_TVL_SPEED_CMD].val[3] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[1];
				if(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2] != curCalib->Points[CALIB_TVL_SPEED_CMD].val[0])
					K2 = K2/(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[0]);
				*/
				
				K2 = curCalib->Points[CALIB_TVL_SPEED_CMD].val[3+4] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[1+4];
				if(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2+4] != curCalib->Points[CALIB_TVL_SPEED_CMD].val[0+4])
					K2 = K2/(curCalib->Points[CALIB_TVL_SPEED_CMD].val[2+4] - curCalib->Points[CALIB_TVL_SPEED_CMD].val[0+4]);

				//xsprintf(hmi->pHMI_Lines[1] ,"m1: %d %s / m2: %d %s" , (DINT)(K1 * 100.0)  , "%" , (DINT)(K2 * 100.0) , "%" );
				xsprintf(hmi->pHMI_Lines[1] ,"A: %4.1f %s / B: %4.1f %s" , K2 * 100.0  , "%" , K1 * 100.0 , "%" );

				strcpy(hmi->pHMI_Lines[2] , "Load Default Calibr. !!!");


				if( (curCfg->Param[parTVL_CalibVel1].val[0] <= 0.0) || (curCfg->Param[parTVL_CalibVel2].val[0] <= 0.0)
					||(fabsf(curCfg->Param[parTVL_CalibVel1].val[0] - curCfg->Param[parTVL_CalibVel2].val[0]) < 0.1)
					|| (curCfg->Param[parGEN_CalibDist].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibDist_2].val[0] <= 0.0)){

					strcpy(hmi->pHMI_Lines[0] , "Wrong values ");						
					status = 0;
					
				}else {
				
					strcpy(hmi->pHMI_Lines[0] , " Start Calibrate ");	
					
					status = 1;
				}				
				

				if(status){

					if(hmi->FiredEvent == EVNT_OK){


						hmi->Node2JumpID = P_MODE_TVL_CALIB;
						hmi->xCMD = JUMP2ID_CMD;
						

						hmi->_ProcStatus = Process[PROC_TVL].Status;
						hmi->CalibStepDone = 0;

						if(!KK_Functionality){
							curCfg->Param[parGEN_CalibTime1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime2].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime3].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime4].val[0] = 0.0;
						}
						else{

							curCfg->Param[parGEN_CalibTime1].val[0] = curCfg->Param[parGEN_CalibDist].val[0]/ curCfg->Param[parTVL_CalibVel1].val[0];
							curCfg->Param[parGEN_CalibTime2].val[0] = curCfg->Param[parGEN_CalibDist].val[0]/ curCfg->Param[parTVL_CalibVel1].val[0];
							curCfg->Param[parGEN_CalibTime3].val[0] = curCfg->Param[parGEN_CalibDist_2].val[0]/ curCfg->Param[parTVL_CalibVel2].val[0];
							curCfg->Param[parGEN_CalibTime4].val[0] = curCfg->Param[parGEN_CalibDist_2].val[0]/ curCfg->Param[parTVL_CalibVel2].val[0];


						}

						hmi->_ProcStatus = Process[PROC_TVL].Status; 

						for(i = 0 ; i < 8; i++){
							hmi->tempCalib.val[i] = 0.0;
							hmi->CalibPoint[i] = 999;
							hmi->CalibPointCmd[i] = 0.0;
						}					
						
					}

				}		
							

				 if(hmi->FiredEvent == EVNT_CANCEL){

					strcpy(hmi->pMessage[0], " Default");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Loaded ");
					TimerStart(&hmi->MsgTmr, 1.0);								
					
					memcpy(&curCalib->Points[CALIB_TVL_SPEED_CMD]  , &defaultCalib->Points[CALIB_TVL_SPEED_CMD] , sizeof(hmi->tempCalib));	

				}							

			break;
			
			/*************************************************************/
			case P_MODE_TVL_CALIB:	strcpy(hmi->ActiveBanner , "< TVL Calib >");

				m->CalibType = CALIB_TVL;	

				if(hmi->CalibStepDone)
					break;				


				status = 1;
				if((curCfg->Param[parGEN_CalibTime1].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibTime2].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibTime3].val[0] <= 0.0)
					|| (curCfg->Param[parGEN_CalibTime4].val[0] <= 0.0))
					status = 0;				
					
				for(i = 1 ; i < 5; i++){

					if(hmi->CalibPointCmd[i] == 0.0)						
						status = 0;
											
					if(hmi->CalibPoint[i] == 999)						
						status = 0;
						
				}


				i = (hmi->curNodeID -  hmi->curHeadID);
				if(status){


					/*

					if(!KK_Functionality)
						hmi->CalibStep = 6;
					else{

						if(i < 0)
							i = 0;
						else if(i >= 5)
							i = 6;
						
						hmi->CalibStep = i;


					}
					*/
					
					if(i < 0)
						i = 0;
					else if(i >= 5)
						i = 6;

					hmi->CalibStep = i;
					
				}else {

					if(i < 0)
						i = 0;
					else if(i> 5)
						i = 0;
					
					hmi->CalibStep = i;

				}	
					
												
				switch(hmi->CalibStep){

					case 1: 

						m->ToolHeadID = 0;	

						if(CalibrTMR.IN)
							curCfg->Param[parGEN_CalibTime1].val[0] = (int)CalibrTMR.ET/1000.0;
							
										
					break;	


					case 2: 						

						m->ToolHeadID = 1;		

						if(CalibrTMR.IN)
							curCfg->Param[parGEN_CalibTime2].val[0] = (int)CalibrTMR.ET/1000.0;
			
					break;	


					case 3: 						

						m->ToolHeadID = 0;	

						if(CalibrTMR.IN)
							curCfg->Param[parGEN_CalibTime3].val[0] = (int)CalibrTMR.ET/1000.0;
						
					break;	

					case 4:  
						
						m->ToolHeadID = 1;		

						if(CalibrTMR.IN)
							curCfg->Param[parGEN_CalibTime4].val[0] = (int)CalibrTMR.ET/1000.0;
						
					break;							

			    }							
				
				switch(hmi->CalibStep){
				
						case 0:
							
							strcpy(hmi->pHMI_Lines[0] , "Please - Select Speed and Direction ");
							strcpy(hmi->pHMI_Lines[1] , "");

							
						break;
				
						case 1:							
						case 2:
							
							if(Process[PROC_TVL].Param[1] & 0x1)
							//if(m->ToolHeadID)
								xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Vel. REV" , hmi->CalibStep);
							else
								xsprintf(hmi->pHMI_Lines[0] , "(%d) Low Vel. FWD" , hmi->CalibStep);

							if(Process[PROC_TVL].Status == prcsSTATUS_Stopped)
								strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
							else
								strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");

							strcpy(hmi->pHMI_Lines[1] , "");	
							
						break;						
	
						case 3:
						case 4:
	
							if(Process[PROC_TVL].Param[1] & 0x1)
							//if(m->ToolHeadID)
								xsprintf(hmi->pHMI_Lines[0] , "(%d) High Vel. REV" , hmi->CalibStep);
							else
								xsprintf(hmi->pHMI_Lines[0] , "(%d) High Vel. FWD" , hmi->CalibStep);

							if(Process[PROC_TVL].Status == prcsSTATUS_Stopped)
								strcat(hmi->pHMI_Lines[0] ," - Press START Btn");
							else
								strcat(hmi->pHMI_Lines[0] ," - Press STOP Btn");
							
							strcpy(hmi->pHMI_Lines[1] , "");
							
						break;	

						case 5:
							
							strcpy(hmi->pHMI_Lines[0] , "");
							strcpy(hmi->pHMI_Lines[1] , "Please - Select Speed and Direction ");							
						
						break;
						

						case 6:
							
							strcpy(hmi->pHMI_Lines[0] , "You made all steps. Press To repeat !!!");
							strcpy(hmi->pHMI_Lines[1] , "Calculate New Calibr.");

						break;
						
		
					}

					//// Calculation or Repeat //		

				if(hmi->CalibStep == 6){

					if(hmi->FiredEvent == EVNT_CANCEL){	

						hmi->CalibStepDone = 0;

						if(!KK_Functionality){
							curCfg->Param[parGEN_CalibTime1].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime2].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime3].val[0] = 0.0;
							curCfg->Param[parGEN_CalibTime4].val[0] = 0.0;
						}
						
					}
					else if(hmi->FiredEvent == EVNT_OK){									
					
						
						if(!status){

							strcpy(hmi->pMessage[0], " Some ");
							strcpy(hmi->pMessage[1], " measurement");
							strcpy(hmi->pMessage[2], " is not taken ");
							strcpy(hmi->pMessage[3], " or ZERO");
							TimerStart(&hmi->MsgTmr, 1.0);

							hmi->CalibStepDone = 0;

							break;

						}							
							
							
						v1_0 = curCfg->Param[parGEN_CalibDist].val[0]/curCfg->Param[parGEN_CalibTime1].val[0];
						v1_1 = curCfg->Param[parGEN_CalibDist].val[0]/curCfg->Param[parGEN_CalibTime2].val[0];
						v2_0 = curCfg->Param[parGEN_CalibDist_2].val[0]/curCfg->Param[parGEN_CalibTime3].val[0];
						v2_1 = curCfg->Param[parGEN_CalibDist_2].val[0]/curCfg->Param[parGEN_CalibTime4].val[0];

						if((v1_0 >= v2_0 )
							|| (v1_0 >= v2_0 )){
							
							strcpy(hmi->pMessage[0], " Speed  ");
							strcpy(hmi->pMessage[1], " Low");
							strcpy(hmi->pMessage[2], " is Higher ");
							strcpy(hmi->pMessage[3], " then Low");
							TimerStart(&hmi->MsgTmr, 1.0);

							break;

						}	

						///////////////////////////
						//  Calculation 
						// Low Speed								
						hmi->tempCalib.val[hmi->CalibPoint[1]] = v1_0; // x1
						hmi->tempCalib.val[hmi->CalibPoint[1]+1] = hmi->CalibPointCmd[1]; // y1
						hmi->tempCalib.val[hmi->CalibPoint[2]] = v1_1; // x2
						hmi->tempCalib.val[hmi->CalibPoint[2]+1] = hmi->CalibPointCmd[2]; // y2

						//High Speed
						hmi->tempCalib.val[hmi->CalibPoint[3]] = v2_0;
						hmi->tempCalib.val[hmi->CalibPoint[3]+1] = hmi->CalibPointCmd[3];
						hmi->tempCalib.val[hmi->CalibPoint[4]] = v2_1;
						hmi->tempCalib.val[hmi->CalibPoint[4]+1]= hmi->CalibPointCmd[4];

						
						x = curCfg->Param[parTVL_CalibVel2].val[0] - curCfg->Param[parTVL_CalibVel1].val[0]; 
						y = v2_0 - v1_0;						
						K1 = x/y;
						m1 = fabs(K1 - hmi->oldK1)/K1;						

						//x = curCfg->Param[parTVL_CalibVel2].val[0] - curCfg->Param[parTVL_CalibVel1].val[0]; 
						y = v2_1 - v1_1;
						K2 = x/y;
						m2 = fabs(K2 - hmi->oldK2)/K2;						

						// Must be 0.01 because when zero getcalib also zero
						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_TVL_SPEED_CMD]  ,CALIB_2POINTS );

						b1 = fabsf((x-y)/x);
						//b1 = x;

						x = getFromCalib(0.01, &hmi->tempCalib ,CALIB_2POINTS_4OFF );
						y = getFromCalib(0.01, &curCalib->Points[CALIB_TVL_SPEED_CMD]  ,CALIB_2POINTS_4OFF );

						b2 = fabsf((x-y)/x);
						//b2 = x;	


						xsprintf(hmi->pHMI_Lines[0] ,"error A/ m: %4.1f %s / b: %4.1f %s" , (fabsf(K1-1.0) * 100.0)  , "%" , b1 , "%" );
						xsprintf(hmi->pHMI_Lines[1] ,"error B/ m: %4.1f %s / b: %4.1f %s" , (fabsf(K2-1.0) * 100.0) , "%" , b2 ,  "%" ); 

						if(hmi->oldK1 == 0.0 
							|| hmi->oldK2 == 0.0 )
							strcpy(hmi->pHMI_Lines[2] , "First Calibration ");
						else
							xsprintf(hmi->pHMI_Lines[2] ,"Repeat: A: %4.1f %s / B: %4.1f %s" , (m1 * 100.0) , "%" , (m2*100.0) ,  "%" ); 
						
										
						hmi->oldK1 = K1;
						hmi->oldK2 = K2;
						/////////////////////////					
						strcpy(hmi->pHMI_Lines[4] , "");

						/*
						strcpy(hmi->pMessage[0], " Wait ");
						strcpy(hmi->pMessage[1], " for");
						strcpy(hmi->pMessage[2], " New Calibration");
						strcpy(hmi->pMessage[3], "");
						TimerStart(&hmi->MsgTmr, 1.0);
						*/

						hmi->CalibStepDone = 1;
						
						hmi->Node2JumpID = P_MODE_TVL_CALIB_VER;
						hmi->JumpLineID = 1;
						hmi->xCMD = JUMP2ID_CMD;

						//hmi->CalibStep++;
					}
				}
				

					/////////////////////////////
		
			break;
			
			/*************************************************************/
			case P_MODE_TVL_CALIB_VER:	strcpy(hmi->ActiveBanner , "< TVL Calib Verif >");

				m->CalibType = CALIB_TVL;				

				if(hmi->FiredEvent == EVNT_CANCEL){					

					hmi->_ProcStatus = Process[PROC_TVL].Status;
					hmi->CalibStepDone = 0;

					if(!KK_Functionality){
						curCfg->Param[parGEN_CalibTime1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime2].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime3].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime4].val[0] = 0.0;
					}
					
					hmi->_ProcStatus = Process[PROC_TVL].Status; 

					for(i = 0 ; i < 8; i++){
						hmi->tempCalib.val[i] = 0.0;
						hmi->CalibPoint[i] = 999;
						hmi->CalibPointCmd[i] = 0.0;
					}

					hmi->CalibStepDone = 0;
					hmi->Node2JumpID = P_MODE_TVL_CALIB;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;

					//m->ToolHeadID = 0; // REV


				}				
				else if(hmi->FiredEvent == EVNT_OK){						

					memcpy( &curCalib->Points[CALIB_TVL_SPEED_CMD] , &hmi->tempCalib , sizeof(hmi->tempCalib));						

					hmi->_ProcStatus = Process[PROC_TVL].Status;
					hmi->CalibStepDone = 0;

					if(!KK_Functionality){
						
						curCfg->Param[parGEN_CalibTime1].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime2].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime3].val[0] = 0.0;
						curCfg->Param[parGEN_CalibTime4].val[0] = 0.0;
					}

					hmi->CalibStepDone = 0;
					hmi->Node2JumpID = P_MODE_TVL_CALIB_PAR;
					hmi->JumpLineID = 1;
					hmi->xCMD = JUMP2ID_CMD;


					strcpy(hmi->pMessage[0], " ");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Saved ");
					TimerStart(&hmi->MsgTmr, 1.0);


				}			

			
			break;

			////////////////    WRIST ///////////////////

			case P_PARAM_WRST:	strcpy(hmi->ActiveBanner , "< TRACK Mapping Start >");	

				
			   switch(hmi->curNodeID){
					

					case P_PARAM_WRST + 2:	

			   		break;
			   	}

			    if(hmi->FiredEvent == EVNT_OK){

					TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absLastPos = 0; //OLD_MTR_POS[INDX_B_LAST_POS];
					TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx]._cfg.absFbk = 2;
					TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx].xCMD = 1;

					strcpy(hmi->pMessage[0], "WRIST ");
					strcpy(hmi->pMessage[1], "	Zero Pos. ");
					xsprintf(hmi->pMessage[2], "is Set");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 0.1);

					
			    }
			    xsprintf(hmi->pHMI_Lines[0] ,"Act Wrst Angle: %2.1f" , TML_MTR_INST[m->pCfg->HeadCfg.WRST.TML_A_Indx].PosFBK);
				strcpy(hmi->pHMI_Lines[1] ,"Set Zero Wrst. Pos.!!!");

			break;	
			
			/*************************************************************/
			
			//    P_MODE_TRACK_CALIB_PAR

			////////////////// ***************************** ///////////////////////////

			


			case P_MODE_TRACK_CALIB_PAR:	strcpy(hmi->ActiveBanner , "< TRACK Mapping Start >");	

				
			   switch(hmi->curNodeID){
					

					case P_MODE_TRACK_CALIB_PAR + 1:	
										
										
						if(intrf->LCDEntr){ 				
						
							if(intrf->TickPlus){	
								intrf->TickPlus = 0;

								curCalib->GenSetup[SETUP_POLE_ELEMENTS]++;								
							}
							else if(intrf->TickMinus){					
								intrf->TickMinus = 0;
 
								curCalib->GenSetup[SETUP_POLE_ELEMENTS]--;						
																							
							}								

							if(curCalib->GenSetup[SETUP_POLE_ELEMENTS] < 2)
								curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 2;
							
							else if(curCalib->GenSetup[SETUP_POLE_ELEMENTS] > 32)
								curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 32;
														

						}
						

					break;

					
			   	}

			   	xsprintf(hmi->pHMI_Lines[0] ,"Load Clean Mapping  Values"); 				
				xsprintf(hmi->pHMI_Lines[1] ,"Pole Elem.: %d" ,curCalib->GenSetup[SETUP_POLE_ELEMENTS] );
				xsprintf(hmi->pHMI_Lines[2] ,"Go to Mapping");	
				xsprintf(hmi->pHMI_Lines[3] ,"Go To Verification");
				strcpy(hmi->pHMI_Lines[4] , "");
				strcpy(hmi->pHMI_Lines[5] , "");
				strcpy(hmi->pHMI_Lines[6] , "");
				
				if(hmi->FiredEvent == EVNT_OK){

					intrf->tempIndx = 0;

					/*
					for(i = 0 ; i < 64 ; i++){

						tmpMap[i].x = ABS_TVL_Pos; 
						tmpMap[i].angle = WRST_TML_Param_Set[0].actAngle;

					}
					*/

					memcpy(tmpMap , curCalib->Map , sizeof(tmpMap));

					hmi->Node2JumpID = P_MODE_TRACK_CALIB;
					hmi->xCMD = JUMP2ID_CMD;

				}
				else if(hmi->FiredEvent == EVNT_ACKN){

					intrf->tempIndx = 0;

					memcpy(tmpMap , curCalib->Map , sizeof(tmpMap));

					hmi->Node2JumpID = P_MODE_TRACK_CALIB_VER;
					hmi->xCMD = JUMP2ID_CMD;

				}
			
				else if(hmi->FiredEvent == EVNT_CANCEL){

					strcpy(hmi->pMessage[0], " Default");
					strcpy(hmi->pMessage[1], " Calib ");
					strcpy(hmi->pMessage[2], " is");
					strcpy(hmi->pMessage[3], " Loaded ");
					TimerStart(&hmi->MsgTmr, 1.0);	

					
					memset(tmpMap , 0 , sizeof(tmpMap));

					
					for(i = 0 ; i < 64 ; i++){

						tmpMap[i].x = ABS_TVL_Pos; 
						tmpMap[i].angle = WRST_TML_Param_Set[0].actAngle;

					}
					

					/*//
					tmpMap[0].x = ABS_TVL_Pos; 
					tmpMap[0].angle = WRST_TML_Param_Set[0].actAngle;
					//tmpMap[0].angle = 15.0;
					
					// Start
					tmpMap[1].x = tmpMap[0].x + 50.0; 
					tmpMap[1].angle = tmpMap[0].angle - 15.0;
					
					tmpMap[2].x = tmpMap[0].x + 50.0 +30.0; 
					tmpMap[2].angle = tmpMap[0].angle;
				
					// Corner
					tmpMap[3].x = tmpMap[0].x + 100.0; 
					tmpMap[3].angle = tmpMap[0].angle - 15.0;

					tmpMap[4].x = tmpMap[0].x + 100.0 + 30.0; 
					tmpMap[4].angle = tmpMap[0].angle;

					// Corner
					tmpMap[5].x = tmpMap[0].x + 150.0; 
					tmpMap[5].angle = tmpMap[0].angle - 15.0;

					tmpMap[6].x = tmpMap[0].x + 150.0 + 30.0; 
					tmpMap[6].angle = tmpMap[0].angle;

					
					// Corner
					tmpMap[7].x = tmpMap[0].x + 200.0; 
					tmpMap[7].angle = tmpMap[0].angle - 15.0;

					tmpMap[8].x = tmpMap[0].x + 200.0 + 30.0; 
					tmpMap[8].angle = tmpMap[0].angle;				
				

					curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 4;	

					memcpy(curCalib->Map , tmpMap , sizeof(tmpMap));

					/////*/

					/*//
					tmpMap[0].x = ABS_TVL_Pos; 
					tmpMap[0].angle = WRST_TML_Param_Set[0].actAngle;
					//tmpMap[0].angle = 15.0;
					
					// Start
					tmpMap[1].x = tmpMap[0].x + 25.0; 
					tmpMap[1].angle = tmpMap[0].angle - 15.0;
					
					tmpMap[2].x = tmpMap[0].x + 25.0 +10.0; 
					tmpMap[2].angle = tmpMap[0].angle;
				
					// Corner
					tmpMap[3].x = tmpMap[0].x + 50.0; 
					tmpMap[3].angle = tmpMap[0].angle - 15.0;

					tmpMap[4].x = tmpMap[0].x + 50.0 + 10.0; 
					tmpMap[4].angle = tmpMap[0].angle;

					// Corner
					tmpMap[5].x = tmpMap[0].x + 75.0; 
					tmpMap[5].angle = tmpMap[0].angle - 15.0;

					tmpMap[6].x = tmpMap[0].x + 75.0 + 10.0; 
					tmpMap[6].angle = tmpMap[0].angle;

					
					// Corner
					tmpMap[7].x = tmpMap[0].x + 100.0; 
					tmpMap[7].angle = tmpMap[0].angle - 15.0;

					tmpMap[8].x = tmpMap[0].x + 100.0 + 10.0; 
					tmpMap[8].angle = tmpMap[0].angle;				
				

					curCalib->GenSetup[SETUP_POLE_ELEMENTS] = 4;	

					memcpy(curCalib->Map , tmpMap , sizeof(tmpMap));

					//*/
					
					
					//hmi->Node2JumpID = P_MODE_TRACK_CALIB_VER;
					//hmi->xCMD = JUMP2ID_CMD;

				}							
			
			break;
						
			/*************************************************************/
			case P_MODE_TRACK_CALIB:	strcpy(hmi->ActiveBanner , "< TRACK Mapping>");

				n = intrf->tempIndx/2 + 1;
				l = intrf->tempIndx%2 + 1;

				xsprintf(hmi->pHMI_Lines[0] ,"Prev Point");		
				xsprintf(hmi->pHMI_Lines[1] ,"Return to Start");	
				xsprintf(hmi->pHMI_Lines[2] ,"Cur Segm %d / Point %d / TVL fact.:  %2.1f" , n , l , 1.0/fabsf(cosf(WRST_TML_Param_Set[0].actAngle/180 * M_PI)));	

				if(intrf->tempIndx>0){
					xsprintf(hmi->pHMI_Lines[3] ,"TVL Pos. %3.1f / %3.1f" ,  ABS_TVL_Pos , tmpMap[intrf->tempIndx-1].x);	
					xsprintf(hmi->pHMI_Lines[4] ,"Angle: %2.1f / %2.1f " , WRST_TML_Param_Set[0].actAngle , tmpMap[intrf->tempIndx-1].angle);	
				}else{
					xsprintf(hmi->pHMI_Lines[3] ,"TVL Pos. %3.1f / %3.1f" ,  ABS_TVL_Pos , tmpMap[intrf->tempIndx].x);	
					xsprintf(hmi->pHMI_Lines[4] ,"Angle: %2.1f / %2.1f " , WRST_TML_Param_Set[0].actAngle , tmpMap[intrf->tempIndx].angle);	
				}
				
				xsprintf(hmi->pHMI_Lines[5] ,"Store & Next Point"); 
				xsprintf(hmi->pHMI_Lines[6] ,"Next Point");	
				
				// curCalib->Map[]
				
				 if(hmi->FiredEvent == EVNT_CANCEL){

				 	if (intrf->tempIndx > 0){
						intrf->tempIndx --;

						/*
						strcpy(hmi->pMessage[0], "Prev Point");
						strcpy(hmi->pMessage[1], " ");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 0.1);
						*/

				 	}else{

						strcpy(hmi->pMessage[0], "First Point");
						strcpy(hmi->pMessage[1], " ");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 0.1);	

				 	}
					
				}	
				 else if(hmi->FiredEvent == EVNT_ACKN){

					hmi->Node2JumpID = P_MODE_TRACK_CALIB_PAR;
					hmi->xCMD = JUMP2ID_CMD;

					strcpy(hmi->pMessage[0], "Start Again");
					strcpy(hmi->pMessage[1], " ");
					strcpy(hmi->pMessage[2], " ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.0);	

				}	

				 
				else if(hmi->FiredEvent == EVNT_OK){
				
				   intrf->tempIndx ++;

				   if(intrf->tempIndx >= 64)
					   intrf->tempIndx = 63;

				   if (intrf->tempIndx >= curCalib->GenSetup[SETUP_POLE_ELEMENTS] * 2 ){

					   
						   
					   hmi->Node2JumpID = P_MODE_TRACK_CALIB_VER;
					   hmi->xCMD = JUMP2ID_CMD;

					   strcpy(hmi->pMessage[0], "Last");
					   strcpy(hmi->pMessage[1], "Point");
					   strcpy(hmi->pMessage[2], " ");
					   strcpy(hmi->pMessage[3], " ");
					   TimerStart(&hmi->MsgTmr, 1.0);  

				   }   				   

			   }
				else if(hmi->FiredEvent == EVNT_ACKN_ALL){

					if(intrf->tempIndx > 0){
						if(ABS_TVL_Pos <= tmpMap[intrf->tempIndx-1].x){

							strcpy(hmi->pMessage[0], "Wrong TVL Pos.");
							strcpy(hmi->pMessage[1], " must be bigger  ");
							strcpy(hmi->pMessage[2], " then previous");
							strcpy(hmi->pMessage[3], " ");
							TimerStart(&hmi->MsgTmr, 1.0);	
							break;

						}
					}

					intrf->tempIndx ++;					

					if(intrf->tempIndx >= 64)
						intrf->tempIndx = 63;

					

					if (intrf->tempIndx >= curCalib->GenSetup[SETUP_POLE_ELEMENTS] * 2 ){						

						tmpMap[intrf->tempIndx-1].x = 	ABS_TVL_Pos; 
						tmpMap[intrf->tempIndx-1].angle = 	WRST_TML_Param_Set[0].actAngle;

						intrf->tempIndx = curCalib->GenSetup[SETUP_POLE_ELEMENTS] * 2 - 1;
						
						hmi->Node2JumpID = P_MODE_TRACK_CALIB_VER;
						hmi->xCMD = JUMP2ID_CMD;

						strcpy(hmi->pMessage[0], "Finished");
						strcpy(hmi->pMessage[1], " Confirm ");
						strcpy(hmi->pMessage[2], " Mapping");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 1.0);	

					}
					else{					

						tmpMap[intrf->tempIndx-1].x = 	ABS_TVL_Pos; 
						tmpMap[intrf->tempIndx-1].angle = 	WRST_TML_Param_Set[0].actAngle;

									
						strcpy(hmi->pMessage[0], "Stored");
						strcpy(hmi->pMessage[1], "Next Point");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 0.1);
						

					}

				}	

				/////////////////////////////
		
			break;
			
			/*************************************************************/
			case P_MODE_TRACK_CALIB_VER:	strcpy(hmi->ActiveBanner , "< TRACK Mapping End >");

				
				n = intrf->tempIndx/2 + 1;
				l = intrf->tempIndx%2 + 1;
				
				xsprintf(hmi->pHMI_Lines[0] ,"Go To Prev Point"); 	
				xsprintf(hmi->pHMI_Lines[1] ,"Accept/Store Mapping");	
				xsprintf(hmi->pHMI_Lines[2] ,"Cur Segm %d / Point %d / TVL fact.:  %2.1f" , n , l , 1.0/fabsf(cosf(curCalib->Map[intrf->tempIndx].angle/180 * M_PI)));			
				xsprintf(hmi->pHMI_Lines[3] ,"TVL Pos.:  %f" , tmpMap[intrf->tempIndx].x);	
				xsprintf(hmi->pHMI_Lines[4] ,"WRST Angle: %2.1f" ,  tmpMap[intrf->tempIndx].angle);		
				xsprintf(hmi->pHMI_Lines[5] ,"Go to Next Point");
				strcpy(hmi->pHMI_Lines[6] , "");
				
				if(hmi->FiredEvent == EVNT_CANCEL){

				 	if (intrf->tempIndx > 0){
						intrf->tempIndx --;

						/*
						strcpy(hmi->pMessage[0], "Prev Point");
						strcpy(hmi->pMessage[1], " ");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 0.1);
						*/

				 	}else{

						strcpy(hmi->pMessage[0], "First Point");
						strcpy(hmi->pMessage[1], " ");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 0.1);	

				 	}
					
				}	
				 else if(hmi->FiredEvent == EVNT_OK){

					intrf->tempIndx ++;

					if(intrf->tempIndx >= 64)
						intrf->tempIndx = 63;

					if (intrf->tempIndx >= curCalib->GenSetup[SETUP_POLE_ELEMENTS] * 2 ){

						intrf->tempIndx --;		

						strcpy(hmi->pMessage[0], "Last");
						strcpy(hmi->pMessage[1], "Point");
						strcpy(hmi->pMessage[2], " ");
						strcpy(hmi->pMessage[3], " ");
						TimerStart(&hmi->MsgTmr, 1.0);	

					}	
					

				}			

				
				else if(hmi->FiredEvent == EVNT_ACKN){

					memcpy(curCalib->Map , tmpMap , sizeof(tmpMap));
				
					hmi->Node2JumpID = P_MODE_TRACK_CALIB_PAR;
					hmi->xCMD = JUMP2ID_CMD;

					strcpy(hmi->pMessage[0], "Accepted");
					strcpy(hmi->pMessage[1], " ");
					strcpy(hmi->pMessage[2], " ");
					strcpy(hmi->pMessage[3], " ");
					TimerStart(&hmi->MsgTmr, 1.0);	

				}
			
			break;
				
			/*************************************************************/
			
			//	  P_JOBS

			/////////////////// ************************** /////////////////////////////
			case P_JOBS:	strcpy(hmi->ActiveBanner , "<< FILE OPER. >>");

				switch(hmi->FiredEvent){


					case EVNT_TOGGLE:

						hmi->ROM_USB = !hmi->ROM_USB;					
				

					break;	


					case EVNT_LOAD_JOB:

						if(File_RW.CMD == 0){
							
							if(hmi->ROM_USB)
								strcpy(File_RW.deviceName, "USB_WeldPrograms");
							else
								strcpy(File_RW.deviceName, "WeldPrograms");
							
							File_RW.CMD = F_READ_DIR;

							hmi->Node2JumpID = P_JOB_LOAD;
							hmi->JumpLineID = 0;
							hmi->xCMD = JUMP2ID_CMD;
						}
					
					break;	

					case EVNT_DELETE_JOB:

						if(File_RW.CMD == 0){
							
							if(hmi->ROM_USB)
								strcpy(File_RW.deviceName, "USB_WeldPrograms");
							else
								strcpy(File_RW.deviceName, "WeldPrograms");
							
							File_RW.CMD = F_READ_DIR;

							hmi->Node2JumpID = P_JOB_DELETE;
							hmi->JumpLineID = 0;
							hmi->xCMD = JUMP2ID_CMD;
						}
						

					break;	

					case EVNT_SAVE_JOB:

						if(File_RW.CMD == 0){

							if(hmi->ROM_USB)
								strcpy(File_RW.deviceName, "USB_WeldPrograms");
							else
								strcpy(File_RW.deviceName, "WeldPrograms");
							
							File_RW.CMD = F_SAVE_CURCFG;

							strcpy(hmi->pMessage[0], " ");
							xsprintf(hmi->pMessage[1],"Program: %s", curCfg->CurProgramName);
							strcpy(hmi->pMessage[2], " is");
							strcpy(hmi->pMessage[3], " Saved ");
							TimerStart(&hmi->MsgTmr, 1.0);

							

						}
					
					break;	

				}

				F_ERROR(&File_RW , hmi);

				if(	hmi->ROM_USB)
					strcpy(nodes[P_JOBS + 2].var.Format , "  USB MEM");
				else
					strcpy(nodes[P_JOBS + 2].var.Format , "INTERNAL MEM");

					
			break;


			/*********************************************************/
			//    P_JOB_LOAD
			case P_JOB_LOAD:	strcpy(hmi->ActiveBanner , "<<Load Program>>");


				if(hmi->FiredEvent == EVNT_LOAD_JOB){

					if(hmi->ROM_USB)
						strcpy(File_RW.deviceName, "USB_WeldPrograms");
					else
						strcpy(File_RW.deviceName, "WeldPrograms");


					strcpy(File_RW.fileName , File_RW.FileInfo[0].Filename);
					File_RW.CMD = F_LOAD_CURCFG;

					strcpy(hmi->pMessage[0], " ");
					xsprintf(hmi->pMessage[1],"Program: %s", File_RW.FileInfo[0].Filename);
					strcpy(hmi->pMessage[2], "  is");
					strcpy(hmi->pMessage[3], " Loaded ");
					TimerStart(&hmi->MsgTmr, 1.0);


				}

				// Load Weld Program
				if((File_RW.CMD == 0) && (File_RW._int.FDirInfo.filenum > 0) ){

					if(intrf->LCDEntr && (hmi->curNodeID == P_JOB_LOAD+1 )){

						if(intrf->TickPlus){	
							intrf->TickPlus = 0;

							File_RW._int.FFileInfo.entry++;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum)
								File_RW._int.FFileInfo.entry = File_RW._int.FDirInfo.filenum - 1;
							
							File_RW.CMD = F_READ_FILE_INF;
							
								
						}

						else if(intrf->TickMinus){					
							intrf->TickMinus = 0;
							
							File_RW._int.FFileInfo.entry--;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum)
								File_RW._int.FFileInfo.entry = 0;

							File_RW.CMD = F_READ_FILE_INF;
						}	


					}	


				}


				// Select Weld Program Param
				
				if((File_RW.CMD == 0) && (File_RW._int.FDirInfo.filenum > 0) ){

					if(intrf->LCDEntr && (hmi->curNodeID == P_JOB_LOAD+8 )){
						

						if(intrf->TickPlus){	
							intrf->TickPlus = 0;

							File_RW._int.FFileInfo.entry++;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum){
								
								File_RW._int.FFileInfo.entry = File_RW._int.FDirInfo.filenum;
								strcpy(curCfg->NextProgramName , "");							
							}
							else{
							
								File_RW.CMD = F_READ_FILE_INF;
								hmi->selectNextWeldProgram_step = 1;
							}
							
								
						}

						else if(intrf->TickMinus){					
							intrf->TickMinus = 0;
							
							File_RW._int.FFileInfo.entry--;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum){
								
								File_RW._int.FFileInfo.entry = File_RW._int.FDirInfo.filenum;							
								strcpy(curCfg->NextProgramName , "");
							}
							else{

								File_RW.CMD = F_READ_FILE_INF;
								hmi->selectNextWeldProgram_step = 1;
								
							}
						}	


					}	


				}
				switch(hmi->selectNextWeldProgram_step){

					case 1:


						if(File_RW.CMD == 0){
							
							if(!File_RW.Error){ 					
								strcpy(curCfg->NextProgramName , File_RW.FileInfo[0].Filename);
								
							}
							hmi->selectNextWeldProgram_step = 0;
						
						}
						


					break;

				}

				F_ERROR(&File_RW , hmi);
				
			break;


			/*********************************************************/
			//    P_JOB_DELETE
			case P_JOB_DELETE:	strcpy(hmi->ActiveBanner , "<<Delete Program>>");

				
				if((File_RW.CMD == 0) && (File_RW._int.FDirInfo.filenum > 0) ){
				
					if(intrf->LCDEntr && (hmi->curNodeID == P_JOB_DELETE+1 )){

						if(intrf->TickPlus){	
							intrf->TickPlus = 0;

							File_RW._int.FFileInfo.entry++;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum)
								File_RW._int.FFileInfo.entry = File_RW._int.FDirInfo.filenum - 1;
							
							File_RW.CMD = F_READ_FILE_INF;
							
								
						}

						else if(intrf->TickMinus){					
							intrf->TickMinus = 0;
							
							File_RW._int.FFileInfo.entry--;

							if(File_RW._int.FFileInfo.entry >= File_RW._int.FDirInfo.filenum)
								File_RW._int.FFileInfo.entry = 0;

							File_RW.CMD = F_READ_FILE_INF;
						}	


					}	


				}

				F_ERROR(&File_RW , hmi);

			break;

			/*********************************************************/
			//    P_JOB_DELETE
			case P_JOB_DELETE_CONF:	strcpy(hmi->ActiveBanner , "<< You will Delete: >>");


				
				if(hmi->FiredEvent == EVNT_DELETE_JOB){

					if(hmi->ROM_USB)
						strcpy(File_RW.deviceName, "USB_WeldPrograms");
					else
						strcpy(File_RW.deviceName, "WeldPrograms");


					strcpy(File_RW.fileName , File_RW.FileInfo[0].Filename);
					File_RW.CMD = F_DELETE;

					strcpy(hmi->pMessage[0], " ");
					xsprintf(hmi->pMessage[1],"Program: %s", File_RW.FileInfo[0].Filename);
					strcpy(hmi->pMessage[2], "  is");
					strcpy(hmi->pMessage[3], " Deleted ");
					TimerStart(&hmi->MsgTmr, 1.0);

					hmi->Node2JumpID = P_JOB_DELETE;
					hmi->JumpLineID = 0;
					hmi->xCMD = JUMP2ID_CMD;

					


				}

			break;
			
			/*********************************************************/
			//    P_JOB_SAVE_AS
			case P_JOB_SAVE_AS:	strcpy(hmi->ActiveBanner , "<<Save Program As>>");

				if(hmi->FiredEvent == EVNT_SAVE_JOB){

					if(hmi->ROM_USB)
						strcpy(File_RW.deviceName, "USB_WeldPrograms");
					else
						strcpy(File_RW.deviceName, "WeldPrograms");


					strcpy(File_RW.fileNewName , cfg->TXT_Resources[resWeldProgramName]);
					File_RW.CMD = F_SAVE_AS_CURCFG;

					strcpy(hmi->pMessage[0], " ");
					xsprintf(hmi->pMessage[1],"Program: %s", cfg->TXT_Resources[resWeldProgramName]);
					strcpy(hmi->pMessage[2], "  is");
					strcpy(hmi->pMessage[3], " saved ");
					TimerStart(&hmi->MsgTmr, 1.0);

					

				}


				F_ERROR(&File_RW , hmi);

			break;

			/*********************************************************/


			default:

				//strcpy(hmi->ActiveBanner , m->sm.Text);

				//hmi->Node2JumpID = 0;
				//hmi->JumpLineID = 0;
				//hmi->xCMD = 0;
						

			break;
				
			

		}

	}
	
	/*************************************************************/

	/*
	if(hmi->pBanner){
		
		// ERROR Banner
		if(m->sm.Step == 200){ // error state

			switch (ErrorMgr.Error){
				
				case ERROR_FATAL:
					strcpy (hmi->pBanner , "FATAL ERR MUST RBT !!!");
				break;	

				default:
					strcpy (hmi->pBanner , "  ERROR  !!!");	
				break;	
			
			}
					
		}		
		else if(hmi->curHeadID == 0xFFFF){
		
				strcpy (hmi->pBanner , hmi->ActiveBanner);
				
						
		}
	}
	*/
	
	if(hmi->pMessage[0] && hmi->pMessage[1] && hmi->pMessage[2] && hmi->pMessage[3]){
		
		if(PleaseShowFileMessage){
			PleaseShowFileMessage = 0;

			if(File_RW.Error){

				if(File_RW._int.wError == 1987){ // Machine Run - no file load allow

					strcpy(hmi->pMessage[0], " FILE OPER. ERROR\n ");
					strcpy(hmi->pMessage[1], File_RW.fileName);
					strcpy(hmi->pMessage[2], File_RW.MsgText);
					xsprintf(hmi->pMessage[3] ,"Code: %d" , File_RW._int.wError);
					
					Set_Message(hmi , 5.0);
					
				}else{

					strcpy(hmi->pMessage[0], " FILE Loading\n ");
					strcpy(hmi->pMessage[1], " Is prohibited");
					strcpy(hmi->pMessage[2], File_RW.MsgText);
					xsprintf(hmi->pMessage[3] ,"Code: %d" , File_RW._int.wError);
					
					Set_Message(hmi , 1.0);
				}
				
				LogTxt(&LogMgr , File_RW.MsgText );
			}
			else{

				strcpy(hmi->pMessage[0], " FILE OPER. DONE\n");
				strcpy(hmi->pMessage[1], "");
				strcpy(hmi->pMessage[2], File_RW.MsgText);
				xsprintf(hmi->pMessage[3] ,"");
				
				Set_Message(hmi , 2.0);
				
				LogTxt(&LogMgr , File_RW.MsgText );



			}
		}

	}
	///////////////////////////////////////
	//  Watch to switch to calibration mode

	 if(hmi->curHeadID == 0xFFFF){ 
		hmi->oldK1 = 0.0; // Reset Previous calibration slope
		hmi->oldK2 = 0.0;
	}
	else if((hmi->curHeadID >= P_MODE_OSC_LIMIT_CALIB) && (hmi->curHeadID <= P_MODE_INDX_CALIB)){
	
			if((m->SeqStep == 0) && (m->sm.Step == 50)){
	
				//strcpy(hmi->pMessage[0], "Going ");
				//strcpy(hmi->pMessage[1], "to");
				//strcpy(hmi->pMessage[2], "Calibration");
	
				//Set_Message(hmi , 3.0);

				intrf->tempIndx= 0;
				hmi->CalibStep = 0;			

				switch(hmi->curHeadID){


					case P_MODE_TIG_PS_CALIB_PAR:
					case P_MODE_HW_PS_CALIB_PAR:
					case P_MODE_WF_CALIB_PAR:
					case P_MODE_TVL_CALIB_PAR:

						hmi->CalibStepDone = 0;
						hmi->CalibStep = 1;

					break;


					case P_MODE_TIG_PS_CALIB_VER:
					case P_MODE_HW_PS_CALIB_VER:

						hmi->CalibStep = 4;
						hmi->CalibStepDone = 1;

					break;	
					
					case P_MODE_WF_CALIB_VER:
					case P_MODE_TVL_CALIB_VER:

						hmi->CalibStep = 6;
						hmi->CalibStepDone = 1;

					break;	

						
					


				}

				
				
				if((hmi->curHeadID == P_MODE_TIG_PS_CALIB) || (hmi->curHeadID == P_MODE_HW_PS_CALIB)
					|| (hmi->curHeadID == P_MODE_WF_CALIB) || (hmi->curHeadID == P_MODE_TVL_CALIB))
					hmi->CalibStep = 1;
				else
					hmi->CalibStep = 0;				
				
				CalibrTweakGain = 1.0;
				m->sm.Step = 100;
	
			}	
	
			else if((m->sm.Step != 100) || (m->sm.Step != 101)){
				//strcpy(hmi->pMessage[0], "Calibration ");
				//strcpy(hmi->pMessage[1], "is Not Allowed");
				//strcpy(hmi->pMessage[2], " Exit from Calibr.");
	
				//Set_Message(hmi , 5.0);
	
			}
	}	
	else if((m->sm.Step == 100) || (m->sm.Step == 101)){

		//strcpy(hmi->pMessage[0], "Return ");
		//strcpy(hmi->pMessage[1], "from");
		//strcpy(hmi->pMessage[2], "Calibration");

		//Set_Message(hmi , 3.0);

		m->sm.Step = 50;

	}	
	////  Mapping Pages ///////
	switch(hmi->curHeadID){

		case  P_MODE_TRACK_CALIB_PAR:
		case  P_MODE_TRACK_CALIB:
			//case  P_MODE_TRACK_CALIB_VER:
			
			WRST_TML_Param_Set[0].StopEvaluateAngle = 1;
		
			break;

		default:

			WRST_TML_Param_Set[0].StopEvaluateAngle = 0;

			break;
	}
		

	////////////////////////////
	TimerExecute(&hmi->MsgTmr);

	////
	WatchSegmented_Yes_No(m);
	
}



void ScrollIndx(LCD_NODE_obj *pNodes ,HMI_typ *hmi ,  HMI_INTRF_typ *intrf , UINT LineID , UINT limit){


	if(hmi->UserLevel < pNodes[hmi->curNodeID].MinUserLevel)
		return;
	
	if(intrf->LCDEntr && (hmi->curNodeID == LineID )){

		if(intrf->TickPlus){	
			intrf->TickPlus = 0;
			intrf->tempIndx ++;
				
		}

		else if(intrf->TickMinus){					
			intrf->TickMinus = 0;
			intrf->tempIndx --;
		}	


	}	

	

	if(intrf->tempIndx == 0xFFFF)
		intrf->tempIndx = 0;

	else if(intrf->tempIndx > limit)
		intrf->tempIndx = limit;

	return;
}

void PickTMLServo(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi ,  int diagnPage){
int i , k;

	if(intrf->tempIndx >= TML_SERVO_NUM)
		intrf->tempIndx = 0;

	i = diagnPage;	
	k = 0;
	
	if(intrf->LCDEntr
		&& (hmi->curNodeID == i)
		&& hmi->FastBlink)
		xsprintf(hmi->pHMI_Lines[k++] , "  " );
	
	else
		xsprintf(hmi->pHMI_Lines[k++] , "%u: %s"  , intrf->tempIndx ,TML_MTR_INST[intrf->tempIndx].Name);

	
	strcpy(hmi->pHMI_Lines[k++] , TML_STATUS_TXT[TML_MTR_INST[intrf->tempIndx].Status - TML_STAT_DRIVE_ERR]);
	
	xsprintf(hmi->pHMI_Lines[k++] , "Step: %u"  ,TML_MTR_INST[intrf->tempIndx].xCMD);
	
	if(TML_MTR_INST[intrf->tempIndx].Disable)
		xsprintf(hmi->pHMI_Lines[k++] , "Disabled" );
	else
		xsprintf(hmi->pHMI_Lines[k++] , "Enable" );


	if(TML_MTR_INST[intrf->tempIndx].ErrorReg)
		xsprintf(hmi->pHMI_Lines[k++] , "Err. Reg.: %X"  , TML_MTR_INST[intrf->tempIndx].ErrorReg);
		
	else{
		
		if(TML_MTR_INST[intrf->tempIndx]._int.Init)
			xsprintf(hmi->pHMI_Lines[k++] , "Init OK" );
		else
			xsprintf(hmi->pHMI_Lines[k++] , "Init False" );

	}

	
	xsprintf(hmi->pHMI_Lines[k++] , "Analog 1: %u"  , TML_MTR_INST[intrf->tempIndx].AnInp[0]);
	xsprintf(hmi->pHMI_Lines[k++] , "Analog 2: %u"  , TML_MTR_INST[intrf->tempIndx].AnInp[1]);
	
	
	strcpy(hmi->pHMI_Lines[k++] , ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].EnableInp)]);

	xsprintf(hmi->pHMI_Lines[k++] , "Inp0: %s , Inp1: %s"  , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[0])] , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[1])]);

	xsprintf(hmi->pHMI_Lines[k++] , "Inp2: %s , Inp3: %s"  , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[2])] , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[3])]);

	xsprintf(hmi->pHMI_Lines[k++] , "Out0: %s , Out1: %s"  , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigOut_stat[0])] , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigOut_stat[1])]);

	xsprintf(hmi->pHMI_Lines[k++] , "Out4: %s "  , \
									ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigOut_stat[2])]);

	/*
	strcpy(hmi->pHMI_Lines[k++] , ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[0])]);
	strcpy(hmi->pHMI_Lines[k++] , ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[1])]);
	strcpy(hmi->pHMI_Lines[k++] , ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[2])]);
	strcpy(hmi->pHMI_Lines[k++] , ON_OFF_TXT[Bool2Uint(TML_MTR_INST[intrf->tempIndx].DigInp[3])]);
	*/

	if(TML_MTR_INST[intrf->tempIndx].xCMD != TML_DOWNL_DONE)
		xsprintf(hmi->pHMI_Lines[k++] , "%u"  , TML_MTR_INST[intrf->tempIndx].xCMD);
	else
		strcpy(hmi->pHMI_Lines[k++] , "Firmware Done");

	if(TML_MTR_INST[intrf->tempIndx].pState)		
		xsprintf(hmi->pHMI_Lines[k++] ,"Fbk: %9.3f" , TML_MTR_INST[intrf->tempIndx].PosFBK);
		//xsprintf(hmi->pHMI_Lines[k++] ,"%9.3f" , TML_MTR_INST[intrf->tempIndx].pState->FbkVect.x);

	

}
void PickPROCESS(LCD_NODE_obj *pNodes ,HMI_INTRF_typ *intrf , HMI_typ *hmi , int diagnPage){
int i, k;

	if(intrf->tempIndx >= PROCESS_NUM)
		intrf->tempIndx = 0;

	i = diagnPage;
	k = 0;

	if(intrf->LCDEntr
			&& (hmi->curNodeID == i)
			&& hmi->FastBlink)
		xsprintf(hmi->pHMI_Lines[k++] , "  " );
		
	else
		xsprintf(hmi->pHMI_Lines[k++] , "%u: %s"  , intrf->tempIndx ,Process[intrf->tempIndx].Name);

	
	strcpy(hmi->pHMI_Lines[k++] , PROCESS_STATUS_TXT[Process[intrf->tempIndx].Status - prcsSTATUS_StartDly]);
	
	xsprintf(hmi->pHMI_Lines[k++] , "Step: %u"  , Process[intrf->tempIndx]._step);

	if(Process[intrf->tempIndx].Disable)
		xsprintf(hmi->pHMI_Lines[k++] , "Disabled" );
	else
		xsprintf(hmi->pHMI_Lines[k++] , "Enable" );

		
}

void PickAXIS(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi , int diagnPage){
int i, k;


	if(intrf->tempIndx >= VECTORS_NUM)
		intrf->tempIndx = 0;

	i = diagnPage;
	k = 0;
	
	if(intrf->LCDEntr
			&& (hmi->curNodeID == i)
			&& hmi->FastBlink)
			xsprintf(hmi->pHMI_Lines[k++] , "  " );
	else	
		xsprintf(hmi->pHMI_Lines[k++] , "%u: %s"  , intrf->tempIndx ,VectorStateMach[intrf->tempIndx].Name);

	
	xsprintf(hmi->pHMI_Lines[k++] , "Mode:     %s"  , AXIS_STATUS_TXT[VectorStateMach[intrf->tempIndx].Mode]);
	xsprintf(hmi->pHMI_Lines[k++] , "Mode FBK: %s"  , AXIS_STATUS_TXT[VectorStateMach[intrf->tempIndx].FbkVect.Mode]);
	xsprintf(hmi->pHMI_Lines[k++] , "ValIn:  %4.1f"  , VectorStateMach[intrf->tempIndx].InVal);
	xsprintf(hmi->pHMI_Lines[k++] , "ValFBK: %4.1f"  , VectorStateMach[intrf->tempIndx].FbkVect.x);	

		
}



void PickDCMtr(LCD_NODE_obj *pNodes , HMI_INTRF_typ *intrf , HMI_typ *hmi , int diagnPage){
int i, k;


	if(intrf->tempIndx >= GEN_MTR_NUM)
		intrf->tempIndx = 0;

	i = diagnPage;
	k = 0;

	if(intrf->LCDEntr
		&& (hmi->curNodeID == i)
		&& hmi->FastBlink)
		xsprintf(hmi->pHMI_Lines[k++] , "   " );
	
	else
		xsprintf(hmi->pHMI_Lines[k++] , "%u: %s"  , intrf->tempIndx ,DC_Mtr[intrf->tempIndx].Name);


	xsprintf(hmi->pHMI_Lines[k++] , "Step: %u"  , DC_Mtr[intrf->tempIndx].xCmd);

	
	if(DC_Mtr[intrf->tempIndx].Disable)
		xsprintf(hmi->pHMI_Lines[k++] , "Disabled" );
	else
		xsprintf(hmi->pHMI_Lines[k++] , "Enable" );

	xsprintf(hmi->pHMI_Lines[k++] ,"%f10.3" , DC_Mtr[intrf->tempIndx].actPos  * DC_Mtr[intrf->tempIndx].Cfg.mtr_dir);
	
	
}
void PickIO(HMI_typ *hmi, int diagnPage){
int i, k;

	i = diagnPage;
	k = 0;

	if(GAS_Param_Set[0].pSensInput)
		xsprintf(hmi->pHMI_Lines[k++] , "GAS Sw: %s"  , ON_OFF_TXT[(UINT)(*GAS_Param_Set[0].pSensInput)]);
	else
		xsprintf(hmi->pHMI_Lines[k++] , "GAS Sw: NA" ); 

	if(COOL_Param_Set[0].pSensInput)
		xsprintf(hmi->pHMI_Lines[k++] , "COOL Sw: %s"  , ON_OFF_TXT[(UINT)(*COOL_Param_Set[0].pSensInput)]);
	else
		xsprintf(hmi->pHMI_Lines[k++] , "COOL Sw: NA"); 

	xsprintf(hmi->pHMI_Lines[k++] , "Tick Sw: %s"  , ON_OFF_TXT[(UINT)Hardware[1].Inp[23]]);

	xsprintf(hmi->pHMI_Lines[k++] , "TVL Enc: %d"  , AxisCh[TML_AX_TVL].curPos);
	
	if(DC_Mtr[DC_AX_WF].Hrdw.pEncFbk)
		xsprintf(hmi->pHMI_Lines[k++] , "WF #1: %d"  , ((*DC_Mtr[DC_AX_WF].Hrdw.pEncFbk)* (INT)DC_Mtr[DC_AX_WF].Cfg.mtr_dir) * Process[PROC_WIRE_FEED].Param[2]);
	
	if(DC_Mtr[DC_AX_WP].Hrdw.pEncFbk)
		xsprintf(hmi->pHMI_Lines[k++] , "WP #1: %d"  , ((*DC_Mtr[DC_AX_WP].Hrdw.pEncFbk)* (INT)DC_Mtr[DC_AX_WP].Cfg.mtr_dir)* Process[PROC_WIRE_FEED].Param[2]);

	if(DC_Mtr[DC_AX_WF_1].Hrdw.pEncFbk)
		xsprintf(hmi->pHMI_Lines[k++] , "WF #2: %d"  , ((*DC_Mtr[DC_AX_WF_1].Hrdw.pEncFbk)* (INT)DC_Mtr[DC_AX_WF_1].Cfg.mtr_dir)* Process[PROC_WIRE_FEED].Param[2]);
	
	if(DC_Mtr[DC_AX_WP_1].Hrdw.pEncFbk)
		xsprintf(hmi->pHMI_Lines[k++] , "WP #2: %d"  , ((*DC_Mtr[DC_AX_WP_1].Hrdw.pEncFbk)* (INT)DC_Mtr[DC_AX_WP_1].Cfg.mtr_dir)* Process[PROC_WIRE_FEED].Param[2]);

	xsprintf(hmi->pHMI_Lines[k++] , "AVC: %d"  , TML_MTR_INST[TML_AX_AVC].AnInp[1]);
	xsprintf(hmi->pHMI_Lines[k++] , "OSC: %d"  , TML_MTR_INST[TML_AX_OSC].AnInp[1]);

	xsprintf(hmi->pHMI_Lines[k++] , "And Btn: %x"  , Master[0].AndIntrf[0].GlblBtnCmd[0] & 0xFFFFFFFE);
		

}


void F_ERROR(FILE_RW_typ *f , HMI_typ *hmi){

	if(f->Error){
		
		strcpy(hmi->pMessage[0], " File Operation ");
		strcpy(hmi->pMessage[1], "    ERROR  ");
		xsprintf(hmi->pMessage[2]," No: %d " , f->_int.wError);		
		strcpy(hmi->pMessage[3], "  ");
		TimerStart(&hmi->MsgTmr, 2.0);

		f->Error = 0;
		f->CMD = 0;

	}

return;
}

void Set_Message(HMI_typ *hmi , float time){

	if(hmi->MsgTmr.Q)
		TimerStart(&hmi->MsgTmr, time);	
}

#if 0
void CalculateTiming(MASTER_typ *m){
	CurCfg_typ *curCfg;
	Cfg_typ *cfg;

	

	//////////////////////
	// Input Vector
	//
	UINT WeldProgram_Case;
	// Delays
	float T1_WF_STRT_dly , T3_DS_START_dly , T4_TVL_STOP_dly , GAS_POSTPURGE;
	// Slopes
	float UPSLOPE , RAMP_UP , RAMP_DWN , DOWNSLOPE ;
	// Others
	float AVC_US_dly , AVC_Retract_TM ,  TVL_US ; // TVL_Vel , TVL_Fin_Vel , ;

	/// Output Vector
	// Leaders
	float  AVC_US , AVC_DS_dly , AVC_DS , AVC_HoldTM;
	float TVL_US_dly , TVL_DS_dly , TVL_DS ,  TVL_HoldTM ; // TVL_Retract_Dist ;
	float WF_US_dly , WF_US , WF_DS_dly , WF_DS , WF_HoldTM;
	float GAS_POSTPURGE_Native;
	// Followers
	float TIG_US_dly , TIG_US , TIG_DS_dly , TIG_DS;
	float OSC_US_dly , OSC_US , OSC_DS_dly , OSC_DS;	
	float WP_US_dly , WP_US , WP_DS_dly , WP_DS;
	float HW_US_dly , HW_US , HW_DS_dly , HW_DS;
	/////////////////////

	float T0_TVL_STRT_dly;


	if(!m)
		return;
	
	curCfg = (CurCfg_typ*)m->pCurCfg;
	cfg = (Cfg_typ*)m->pCfg;

	if(!curCfg || !cfg)
		return;



	////////////////////////////////////////////////////////////////////

	//WeldProgram_Case 		= (UINT)curCfg->Param_ENUM[parENUM_WeldProgramCase].val[0];
	
	T0_TVL_STRT_dly = curCfg->Param[parGEN_T0_TVL_StartDly].val[0];
	T1_WF_STRT_dly	= curCfg->Param[parGEN_T1_WF_StartDly].val[0];
	T3_DS_START_dly	= curCfg->Param[parGEN_T3_DS_StartDly].val[0];
	T4_TVL_STOP_dly	= curCfg->Param[parGEN_T4_TVL_StoptDly].val[0];
	GAS_POSTPURGE 	= curCfg->Param[parGEN_GAS_PostPurgeTm].val[0];
	
	UPSLOPE			= curCfg->Param[parGEN_UpSlope].val[0];
	RAMP_UP			= curCfg->Param[parGEN_RampUp].val[0];
	DOWNSLOPE		= curCfg->Param[parGEN_DownSlope].val[0];
	RAMP_DWN		= curCfg->Param[parGEN_RampDown].val[0];

	AVC_US_dly		= curCfg->Param[parAVC_StartDly].val[0];
	AVC_Retract_TM	= curCfg->Param[parAVC_GoBackTm].val[0];
	TVL_US			= curCfg->Param[parTVL_RampUp].val[0];

	////////////////////////////////////////////////////////////////////



	switch (WeldProgram_Case){


		case 1: // Generic 

			// Using :  	UPSLOPE , DOWNSLOPE ,  RAMP_UP , RAMP_DWN , 
			// 			T1_WF_START_dly , T3_DS_START_dly , T4_TVL_STOP_dly 
			// 			AVC_US_dly , AVC_Retract_TM ,  TVL_US


			//Inputs Validation

			if(UPSLOPE < RAMP_UP)
				UPSLOPE = RAMP_UP; 		

			if(DOWNSLOPE < RAMP_DWN)
				DOWNSLOPE = RAMP_DWN; 			
							
			if((T1_WF_STRT_dly + RAMP_UP) > UPSLOPE)
				T1_WF_STRT_dly = UPSLOPE - RAMP_UP;	
			

			// Leaders	
			
			T0_TVL_STRT_dly = UPSLOPE - RAMP_UP - T1_WF_STRT_dly;
			T1_WF_STRT_dly = UPSLOPE - RAMP_UP - T0_TVL_STRT_dly;

			
			GAS_POSTPURGE_Native = T1_WF_STRT_dly + T3_DS_START_dly + DOWNSLOPE + GAS_POSTPURGE;
			
			AVC_US = UPSLOPE;
			AVC_DS_dly = T1_WF_STRT_dly + T3_DS_START_dly;
			AVC_DS = DOWNSLOPE;
			AVC_HoldTM  = GAS_POSTPURGE;

			TVL_US_dly = T0_TVL_STRT_dly + AVC_US_dly;
			TVL_US = TVL_US;
			TVL_DS_dly = T1_WF_STRT_dly + RAMP_DWN + T4_TVL_STOP_dly;
			TVL_DS = TVL_US;
			TVL_HoldTM = GAS_POSTPURGE_Native - (T4_TVL_STOP_dly + TVL_DS) + AVC_Retract_TM;
			if(TVL_HoldTM < 0)
				TVL_HoldTM = 0;
			

			WF_US_dly = T0_TVL_STRT_dly + T1_WF_STRT_dly + AVC_US_dly;
			WF_US = RAMP_UP;
			WF_DS_dly = T1_WF_STRT_dly;
			WF_DS = RAMP_DWN;	
			WF_HoldTM = GAS_POSTPURGE_Native - (T1_WF_STRT_dly +RAMP_DWN); 
			if(WF_HoldTM < 0)
				WF_HoldTM = 0;

			

			// Followers
			TIG_US_dly 	= AVC_US_dly;
			TIG_US 		= AVC_US;
			TIG_DS_dly 	= AVC_DS_dly;
			TIG_DS 		= AVC_DS;

			OSC_US_dly 	= AVC_US_dly;
			OSC_US 		= AVC_US;
			OSC_DS_dly 	= AVC_DS_dly;
			OSC_DS 		= AVC_DS;

			WP_US_dly 	= WF_US_dly;
			WP_US 		= WF_US;
			WP_DS_dly 	= WF_DS_dly;
			WP_DS 		= WF_DS;
			
			HW_US_dly 	= WF_US_dly;
			HW_US 		= WF_US;
			HW_DS_dly 	= WF_DS_dly;
			HW_DS 		= WF_DS;
			


		break;



		case 2: // Case #1

			// Using :  	UPSLOPE , DOWNSLOPE ,  RAMP_UP  , 
			// 			 AVC_US_dly , AVC_Retract_TM ,  TVL_US

			// Inputs Validation


			// Assumption			
			RAMP_DWN = RAMP_UP;
			
			// Leaders				
			GAS_POSTPURGE_Native = RAMP_DWN + DOWNSLOPE + GAS_POSTPURGE;

			
			AVC_US = UPSLOPE;		
			AVC_DS_dly = RAMP_DWN;
			AVC_DS = DOWNSLOPE;
			AVC_HoldTM  = GAS_POSTPURGE;
			

			TVL_US_dly = UPSLOPE + AVC_US_dly;
			TVL_US = TVL_US;
			TVL_DS_dly = RAMP_DWN;
			TVL_DS = TVL_US;
			TVL_HoldTM = GAS_POSTPURGE_Native - (RAMP_DWN + TVL_DS) ;
			if(TVL_HoldTM < 0)
				TVL_HoldTM = 0;

			WF_US_dly = UPSLOPE + AVC_US_dly;
			WF_US = RAMP_UP;
			WF_DS_dly = 0.0;
			WF_DS = RAMP_DWN;	
			WF_HoldTM = DOWNSLOPE + GAS_POSTPURGE; 

			
			// Followers
			TIG_US_dly 	= AVC_US_dly;
			TIG_US 		= AVC_US;
			TIG_DS_dly 	= AVC_DS_dly;
			TIG_DS 		= AVC_DS;

			OSC_US_dly 	= AVC_US_dly;
			OSC_US 		= AVC_US;
			OSC_DS_dly 	= AVC_DS_dly;
			OSC_DS 		= AVC_DS;

			WP_US_dly 	= WF_US_dly;
			WP_US 		= WF_US;
			WP_DS_dly 	= WF_DS_dly;
			WP_DS 		= WF_DS;
			
			HW_US_dly 	= WF_US_dly;
			HW_US 		= WF_US;
			HW_DS_dly 	= WF_DS_dly;
			HW_DS 		= WF_DS;		

			// Special from CK
			AVC_DS_dly = 0.0;
			AVC_DS = 0.0;
			AVC_HoldTM  = GAS_POSTPURGE + RAMP_DWN + DOWNSLOPE;


		break;			


			
		case 3:  // Case #2
				
			// Using :		UPSLOPE , DOWNSLOPE ,  RAMP_UP , 
			//			 AVC_US_dly , AVC_Retract_TM ,	TVL_US
			
			
			// Inputs Validation
			
			
			// Assumption			
			RAMP_DWN = RAMP_UP;
			
			// Leaders				
			GAS_POSTPURGE_Native = RAMP_DWN + DOWNSLOPE + GAS_POSTPURGE;

			
			AVC_US = UPSLOPE;
			AVC_DS_dly = RAMP_DWN;
			AVC_DS = DOWNSLOPE;
			AVC_HoldTM  = GAS_POSTPURGE;
			

			TVL_US_dly = UPSLOPE + AVC_US_dly;
			TVL_US = TVL_US;
			TVL_DS_dly = RAMP_DWN + DOWNSLOPE;
			TVL_DS = TVL_US;
			TVL_HoldTM = GAS_POSTPURGE - TVL_DS  ;
			if(TVL_HoldTM < 0)
				TVL_HoldTM = 0;

			WF_US_dly = UPSLOPE + AVC_US_dly;
			WF_US = RAMP_UP;
			WF_DS_dly = 0.0;
			WF_DS = RAMP_DWN;	
			WF_HoldTM = DOWNSLOPE + GAS_POSTPURGE; 
			
			
			// Followers
			TIG_US_dly	= AVC_US_dly;
			TIG_US		= AVC_US;
			TIG_DS_dly	= AVC_DS_dly;
			TIG_DS		= AVC_DS;

			OSC_US_dly	= AVC_US_dly;
			OSC_US		= AVC_US;
			OSC_DS_dly	= AVC_DS_dly;
			OSC_DS		= AVC_DS;

			WP_US_dly	= WF_US_dly;
			WP_US		= WF_US;
			WP_DS_dly	= WF_DS_dly;
			WP_DS		= WF_DS;
			
			HW_US_dly	= WF_US_dly;
			HW_US		= WF_US;
			HW_DS_dly	= WF_DS_dly;
			HW_DS		= WF_DS;	

			// Special from CK
			AVC_DS_dly = 0.0;
			AVC_DS = 0.0;
			AVC_HoldTM  = GAS_POSTPURGE + RAMP_DWN + DOWNSLOPE;

	
		break;	

		
		case 4:  // Case #3
				
			// Using :		UPSLOPE , DOWNSLOPE ,  RAMP_UP , 
			//			 AVC_US_dly , AVC_Retract_TM ,	TVL_US


			// Assumption
			RAMP_DWN = RAMP_UP;
			
			//Inputs Validation
			
			if(UPSLOPE < RAMP_UP)
				UPSLOPE = RAMP_UP; 		

			if(DOWNSLOPE < RAMP_DWN)
				DOWNSLOPE = RAMP_DWN; 			
			
			// Leaders	
		
			AVC_US = UPSLOPE;
			AVC_DS_dly = 0.0;
			AVC_DS = DOWNSLOPE;
			AVC_HoldTM  = GAS_POSTPURGE;
		
			TVL_US_dly = UPSLOPE - RAMP_UP + AVC_US_dly;		
			
			TVL_US = TVL_US;
			TVL_DS_dly = DOWNSLOPE;
			TVL_DS = TVL_US;
			TVL_HoldTM = GAS_POSTPURGE - TVL_DS + AVC_Retract_TM ;
			if(TVL_HoldTM < 0)
				TVL_HoldTM = 0;

			WF_US_dly = TVL_US_dly;
			WF_US = RAMP_UP;
			WF_DS_dly = 0.0;
			WF_DS = RAMP_DWN;	
			WF_HoldTM = DOWNSLOPE + GAS_POSTPURGE - DOWNSLOPE;
			if(WF_HoldTM < 0)
				WF_HoldTM = 0;
			

			GAS_POSTPURGE_Native = DOWNSLOPE + GAS_POSTPURGE;

			// Followers
			TIG_US_dly	= AVC_US_dly;
			TIG_US		= AVC_US;
			TIG_DS_dly	= AVC_DS_dly;
			TIG_DS		= AVC_DS;

			OSC_US_dly	= AVC_US_dly;
			OSC_US		= AVC_US;
			OSC_DS_dly	= AVC_DS_dly;
			OSC_DS		= AVC_DS;

			WP_US_dly	= WF_US_dly;
			WP_US		= WF_US;
			WP_DS_dly	= WF_DS_dly;
			WP_DS		= WF_DS;
			
			HW_US_dly	= WF_US_dly;
			HW_US		= WF_US;
			HW_DS_dly	= WF_DS_dly;
			HW_DS		= WF_DS;

			// Special from CK
			AVC_DS_dly = 0.0;
			AVC_DS = 0.0;
			AVC_HoldTM  = GAS_POSTPURGE + RAMP_DWN + DOWNSLOPE;

	
		break;	


		case 5:   // Case #4
				
			// Using :		UPSLOPE , DOWNSLOPE ,  RAMP_UP , 
			//				T1_WF_STRT_dly 
			//			 AVC_US_dly , AVC_Retract_TM ,	TVL_US
			

			// Assumption	
			RAMP_DWN = RAMP_UP;
					
			
			// Inputs Validation

			if(UPSLOPE < RAMP_UP)
				UPSLOPE = RAMP_UP; 		

			if(DOWNSLOPE < RAMP_DWN)
				DOWNSLOPE = RAMP_DWN; 			
							
			if((T1_WF_STRT_dly + RAMP_UP) > UPSLOPE)
				T1_WF_STRT_dly = UPSLOPE - RAMP_UP;	

						
			
			// Leaders				
			GAS_POSTPURGE_Native = T1_WF_STRT_dly + DOWNSLOPE + GAS_POSTPURGE;

			T0_TVL_STRT_dly = UPSLOPE - RAMP_UP - T1_WF_STRT_dly;
			
			AVC_US = UPSLOPE;			
			AVC_DS_dly = T1_WF_STRT_dly;
			AVC_DS = DOWNSLOPE;
			AVC_HoldTM  = GAS_POSTPURGE;
		
			TVL_US_dly = T0_TVL_STRT_dly + AVC_US_dly;
			TVL_US = TVL_US;
			TVL_DS_dly = T1_WF_STRT_dly + DOWNSLOPE;
			TVL_DS = TVL_US;
			TVL_HoldTM = GAS_POSTPURGE  - TVL_DS + AVC_Retract_TM;
			if(TVL_HoldTM < 0)
				TVL_HoldTM = 0;

			WF_US_dly = T0_TVL_STRT_dly + T1_WF_STRT_dly;
			WF_US = RAMP_UP;
			WF_DS_dly = T1_WF_STRT_dly;
			WF_DS = RAMP_DWN;
			WF_HoldTM = DOWNSLOPE - RAMP_DWN  + GAS_POSTPURGE; 
			
			GAS_POSTPURGE_Native = T1_WF_STRT_dly + DOWNSLOPE + GAS_POSTPURGE;

			// Followers
			TIG_US_dly	= AVC_US_dly;
			TIG_US		= AVC_US;
			TIG_DS_dly	= AVC_DS_dly;
			TIG_DS		= AVC_DS;

			OSC_US_dly	= AVC_US_dly;
			OSC_US		= AVC_US;
			OSC_DS_dly	= AVC_DS_dly;
			OSC_DS		= AVC_DS;

			WP_US_dly	= WF_US_dly;
			WP_US		= WF_US;
			WP_DS_dly	= WF_DS_dly;
			WP_DS		= WF_DS;
			
			HW_US_dly	= WF_US_dly;
			HW_US		= WF_US;
			HW_DS_dly	= WF_DS_dly;
			HW_DS		= WF_DS;	

			// Special from CK
			AVC_DS_dly = 0.0;
			AVC_DS = 0.0;
			AVC_HoldTM  = GAS_POSTPURGE + RAMP_DWN + DOWNSLOPE;

	
		break;	




	}	


	////////////////////////////////////////////////////////////////////////


	curCfg->Param[parGEN_UpSlope].val[0] 		= UPSLOPE;
	curCfg->Param[parGEN_RampUp].val[0] 		= RAMP_UP;
	curCfg->Param[parGEN_DownSlope].val[0] 		= DOWNSLOPE;
	curCfg->Param[parGEN_RampDown].val[0] 		= RAMP_DWN;

	curCfg->Param[parGEN_T1_WF_StartDly].val[0] = T1_WF_STRT_dly;
	 
	

	curCfg->Param[parAVC_Upslope].val[0] 		= AVC_US;
	curCfg->Param[parAVC_StopDly].val[0] 		= AVC_DS_dly;
	curCfg->Param[parAVC_DownSlope].val[0] 		= AVC_DS;	

	
	curCfg->Param[parTVL_StartDly].val[0]		= TVL_US_dly;	
	curCfg->Param[parTVL_StopDly].val[0]		= TVL_DS_dly;	
	curCfg->Param[parTVL_RampDwn].val[0]		= TVL_DS;	
	curCfg->Param[parTVL_RewrapDly].val[0]		= TVL_HoldTM;	

	curCfg->Param[parWF_StartDly].val[0]		= WF_US_dly;	
	curCfg->Param[parWF_RampUp].val[0]			= WF_US;	
	curCfg->Param[parWF_StopDly].val[0]			= WF_DS_dly;	
	curCfg->Param[parWF_RampDwn].val[0]			= WF_DS;
	curCfg->Param[parWF_RetractDly].val[0] 		= WF_HoldTM;	

	
 	curCfg->Param[parTIG_StartDly].val[0]		= TIG_US_dly;	
 	curCfg->Param[parTIG_RampUp].val[0]			= TIG_US;	
	curCfg->Param[parTIG_StopDly].val[0]		= TIG_DS_dly;	
	curCfg->Param[parTIG_RampDwn].val[0]		= TIG_DS;	

	
	curCfg->Param[parOSC_StartDly].val[0]		= OSC_US_dly;	
	curCfg->Param[parOSC_RampUp].val[0] 		= OSC_US;	
	curCfg->Param[parOSC_StopDly].val[0]		= OSC_DS_dly;	
	curCfg->Param[parOSC_RampDwn].val[0]		= OSC_DS;	

	
	curCfg->Param[parWP_StartDly].val[0]		= WP_US_dly;	
	curCfg->Param[parWP_RampUp].val[0]			= WP_US;	
	curCfg->Param[parWP_StopDly].val[0] 		= WP_DS_dly;	
	curCfg->Param[parWP_RampDwn].val[0] 		= WP_DS;	

	curCfg->Param[parHW_RampDwnDly].val[0]		= HW_US_dly;	
	curCfg->Param[parHW_RampUp].val[0]			= HW_US;	
	curCfg->Param[parHW_RampDwnDly].val[0] 		= HW_DS_dly;	
	curCfg->Param[parHW_RampDwn].val[0] 		= HW_DS;	

	
	curCfg->Param[parGAS_PostPurgeTm].val[0]	= GAS_POSTPURGE_Native;
	curCfg->Param[parTVL_RunDist].val[0] 		= m->numSegm/2;


	////////////////////////////////////////////////////////////////////////
	

}

#endif 
void AssignHeadText(char* txt ,   UINT HeadType){


	
	switch(HeadType){

		
		case U_BEND_HP_PINS:
			
			xsprintf(txt, "U BEND HP PINS" ); 			
			
		break;			

		case U_BEND_HP_ENC:
			
			xsprintf(txt, "U BEND HP ENC" ); 
			
		break;	

		case F_HEAD_HP_ENCODER:
			
			xsprintf(txt, "F HEAD HP ENCODER" ); 
			
		break;

		case F_HEAD_HP_MINI_ENC:
			
			xsprintf(txt, "MINI HEAD HP ENCODER" ); 
			
		break;

		case BPV_F_HEAD:
			
			xsprintf(txt, "BPV F HEAD" ); 
			
		break;

		case OMEGA_16INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 16INCH HP ENCODER" ); 
			
		break;

		case OMEGA_10INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 10INCH HP ENCODER" ); 
			
		break;

		case OMEGA_8INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 8INCH HP ENCODER" ); 
			
		break;	

		case SUPER_MINI:
			
			xsprintf(txt, "SUPER MINI" ); 
			
		break;	

		case BPV_FACE_OSC:
			
			xsprintf(txt, "BPV FLANGE OSC POT" ); 
			
		break;	

		case U_BEND_HP_ENC_BEMF:
			
			xsprintf(txt, "U BEND HP ENC BEMF" ); 
			
		break;

		case F_HEAD_HP_ENC_BEMF:
			
			xsprintf(txt, "F HEAD HP ENC BEMF" ); 
			
		break;

		case BPV_FACE_OSC_ENC:
			
			xsprintf(txt, "BPV FLANGE OSC ENCODER" ); 
			
		break;

		case BPV_FACE_OSC_EXP:
			
			xsprintf(txt, "BPV Ext.OSC ENC+POT" ); 
			
		break;

		case M_94:
			
			xsprintf(txt, "M_94" ); 
			
		break;

		case BPV_v2:
			
			xsprintf(txt, "BPV 2.0 TT-BO IND" ); 
			
		break;	

		case BPV_v2_MPBO:
			
			xsprintf(txt, "BPV 2.0 MT-BO IND" ); 
			
		break;

		case BPV_v2_MPGH:
			
			xsprintf(txt, "BPV 2.0 TT-BO TOS IND" ); 
			
		break;

		case BPV_v2_MPGV:
			
			xsprintf(txt, "BPV 2.0 MT-BO TOS IND" ); 
			
		break;

		
		case BPV_v2_TT_BO:
			
			xsprintf(txt, "BPV 2.0 TT-BO AWF" ); 
			
		break;	

		case BPV_v2_MT_BO:
			
			xsprintf(txt, "BPV 2.0 MT-BO AWF" ); 
			
		break;

		case BPV_v2_TT_BO_TOS:
			
			xsprintf(txt, "BPV 2.0 TT-BO TOS AWF" ); 
			
		break;

		case BPV_v2_MT_BO_TOS:
			
			xsprintf(txt, "BPV 2.0 MT-BO TOS AWF" ); 
			
		break;

		case RUBIX:
			
			xsprintf(txt, "RUBIX" ); 
			
		break;

		case F_HEAD_MINI_STND:
			
			xsprintf(txt, "MINI HEAD STND" ); // similar to "MINI HEAD HP ENCODER"
			
		break;		

		case F_HEAD_MINI_OMEGA:
			
			xsprintf(txt, "MINI HEAD OMEGA" ); // similar to "MINI HEAD HP ENCODER"
			
		break;

		case D_HEAD_HP:

			xsprintf(txt, "D - HEAD HP" ); // similar to "F_HEAD_MINI_OMEGA" ???

		break;		


		case BPV2_FACE_OSC_EXP:
			
			xsprintf(txt, "BPV2 Ext.OSC ENC+POT" ); 
			
		break;

		case BPV_v2_MT:
			
			xsprintf(txt, "BPV 2.0 MT" ); 
			
		break;

		case BPV_v1_MT:
			
			xsprintf(txt, "BPV 1.0 MT" ); 
			
		break;

		case BPV_v2_Video:
			
			xsprintf(txt, "BPV 2.0 Video" ); 
			
		break;

		case CLAMSHELL_HEAD:
			
			xsprintf(txt, "ClamShell KKL" ); 
			
		break;	
		
		case CHERRYPOINT_BP1_HEAD:
			
			xsprintf(txt, "BPV Ext.OSC ENC Not POT" ); 
			
		break;

		case CHERRYPOINT_BP2_HEAD:
			
			xsprintf(txt, "BPV2 Ext.OSC ENC Not POT" ); 
			
		break;


		case CHERRYPOINT_MT_BP2_HEAD:

			xsprintf(txt, "BPV2 MT Ext.OSC ENC+POT" );

		break;

		case MIG_TIG_HEAD: 

			xsprintf(txt, "MIG NG4 - TIG " ); 
			
		break;


		default:

			xsprintf(txt, "HEAD: WRONG" ); 

		break;	

	}	

	return;

}

void AssignHeadText_old(char* txt ,   UINT HeadType){


	
	switch(HeadType){

		
		case U_BEND_HP_PINS:
			
			xsprintf(txt, "U BEND HP PINS" ); 
			
		break;			

		case U_BEND_HP_ENC:
			
			xsprintf(txt, "U BEND HP ENC" ); 
			
		break;	

		case F_HEAD_HP_ENCODER:
			
			xsprintf(txt, "F HEAD HP ENCODER" ); 
			
		break;

		case F_HEAD_HP_MINI_ENC:
			
			xsprintf(txt, "MINI HEAD HP ENCODER" ); 
			
		break;

		case BPV_F_HEAD:
			
			xsprintf(txt, "BPV F HEAD" ); 
			
		break;

		case OMEGA_16INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 16INCH HP ENCODER" ); 
			
		break;

		case OMEGA_10INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 10INCH HP ENCODER" ); 
			
		break;

		case OMEGA_8INCH_HP_ENCODER:
			
			xsprintf(txt, "OMEGA 8INCH HP ENCODER" ); 
			
		break;	

		case SUPER_MINI:
			
			xsprintf(txt, "SUPER MINI" ); 
			
		break;	

		case BPV_FACE_OSC:
			
			xsprintf(txt, "BPV FLANGE OSC POT" ); 
			
		break;	

		case U_BEND_HP_ENC_BEMF:
			
			xsprintf(txt, "U BEND HP ENC BEMF" ); 
			
		break;

		case F_HEAD_HP_ENC_BEMF:
			
			xsprintf(txt, "F HEAD HP ENC BEMF" ); 
			
		break;

		case BPV_FACE_OSC_ENC:
			
			xsprintf(txt, "BPV FLANGE OSC ENCODER" ); 
			
		break;

		case BPV_FACE_OSC_EXP:
			
			xsprintf(txt, "BPV FLANGE OSC ENC+POT" ); 
			
		break;

		case M_94:
			
			xsprintf(txt, "M_94" ); 
			
		break;

		case BPV_v2:
			
			xsprintf(txt, "BPV 2.0 TT-BO IND" ); 
			
		break;	

		case BPV_v2_MPBO:
			
			xsprintf(txt, "BPV 2.0 MT-BO IND" ); 
			
		break;

		case BPV_v2_MPGH:
			
			xsprintf(txt, "BPV 2.0 TT-BO TOS IND" ); 
			
		break;

		case BPV_v2_MPGV:
			
			xsprintf(txt, "BPV 2.0 MT-BO TOS IND" ); 
			
		break;

		
		case BPV_v2_TT_BO:
			
			xsprintf(txt, "BPV 2.0 TT-BO AWF" ); 
			
		break;	

		case BPV_v2_MT_BO:
			
			xsprintf(txt, "BPV 2.0 MT-BO AWF" ); 
			
		break;

		case BPV_v2_TT_BO_TOS:
			
			xsprintf(txt, "BPV 2.0 TT-BO TOS AWF" ); 
			
		break;

		case BPV_v2_MT_BO_TOS:
			
			xsprintf(txt, "BPV 2.0 MT-BO TOS AWF" ); 
			
		break;

		case RUBIX:
			
			xsprintf(txt, "RUBIX" ); 
			
		break;

		case F_HEAD_MINI_STND:
			
			xsprintf(txt, "MINI HEAD STND" ); // similar to "MINI HEAD HP ENCODER"
			
		break;		

		case F_HEAD_MINI_OMEGA:
			
			xsprintf(txt, "MINI HEAD OMEGA" ); // similar to "MINI HEAD HP ENCODER"
			
		break;

		case D_HEAD_HP:

			xsprintf(txt, "D - HEAD HP" ); // similar to "F_HEAD_MINI_OMEGA" ???

		break;		


		case BPV2_FACE_OSC_EXP:
			
			xsprintf(txt, "BPV2 FLANGE OSC ENC+POT" ); 
			
		break;

		case BPV_v2_MT:
			
			xsprintf(txt, "BPV 2.0 MT" ); 
			
		break;

		case BPV_v1_MT:
			
			xsprintf(txt, "BPV 1.0 MT" ); 
			
		break;

		case BPV_v2_Video:
			
			xsprintf(txt, "BPV 2.0 Video" ); 
			
		break;

		case CLAMSHELL_HEAD:
			
			xsprintf(txt, "ClamShell KKL" ); 
			
		break;	
		
		case CHERRYPOINT_BP1_HEAD:
			
			xsprintf(txt, "CP BPV ???" ); 
			
		break;

		case CHERRYPOINT_BP2_HEAD:
			
			xsprintf(txt, "CherryP BPV2" ); 
			
		break;


		case CHERRYPOINT_MT_BP2_HEAD:

			xsprintf(txt, "CherryP MT BPV2" );

		break;

		case MIG_TIG_HEAD: 

			xsprintf(txt, "MIG TIG not ready" ); 
			
		break;


		default:

			xsprintf(txt, "HEAD: WRONG" ); 

		break;	

	}	

	return;

}

void AssignMtrErrorText(char* txt , UDINT i , UDINT val ){

plcstring errortxt[64];


	/*
	if(i < ERROR_DRV_11)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i-1].Name);
	else if(i < ERROR_DRV_21)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i - ERROR_DRV_11].Name);
	else if(i < ERROR_DRV_31)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- ERROR_DRV_21].Name);
	else if(i < ERROR_DRV_41)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- ERROR_DRV_31].Name);
	*/

	if(i < 16)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i-1].Name);
	else if(i < 32)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i - 16].Name);
	else if(i < 48)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- 32].Name);
	else if(i < 64)
		xsprintf(txt, "Drive: %s: " , TML_MTR_INST[i- 48].Name);

	switch(val){


		case 1: strcat(txt , "CANbus Error"); 			break;

		case 2: strcat(txt , "Short-Circuit Error"); 		break;

		case 3: strcat(txt , "Invalid Setup Error"); 		break;

		case 4: strcat(txt , "Control (Position) Error "); 		break;

		case 5: strcat(txt , "Serial Comm Error"); 		break;

		case 6: strcat(txt , "Feedback Error"); 		break;

		case 7: strcat(txt , "POT Limit Error"); 			break;

		case 8: strcat(txt , "NOT Limit Error"); 			break;

		case 9: strcat(txt , "Instantanious Over-Current Error"); 			break;

		case 10: strcat(txt , "I2t Over-Current Error"); 					break;

		case 11: strcat(txt , "Over-Temperature Error - Motor"); 	break;

		case 12: strcat(txt , "Over-Temperature Error - Drive"); 	break;

		case 13: strcat(txt , "Over-Voltage Error"); 		break;

		case 14: strcat(txt , "Under-Voltage Error"); 		break;

		case 15: strcat(txt , "Command Error"); 		break;

		case 16: strcat(txt , "E-Stop Enable Inactive Error"); 	break;

		case 17: strcat(txt , "Init Error"); 	break;

		case 18: strcat(txt , "No Comunnication"); 	break;

		default: xsprintf(txt+strlen(txt), "Unknown Error %X " , val);

	}


}

UINT DecodeMappingIndx(CurCalib_typ *cal ){
UINT indx;

	if(!cal)
		return 0;

	switch(cal->GenSetup[SETUP_MAPPING_ON]){
		

		case MAPPING_OFF: 									indx = 0; break; // OFF

		case MAPPING_ON: 									indx = 1; break; // ON

		case MAPPING_ON + MAPPING_SEGM: 					indx = 2; break; // ON + Segments

		case MAPPING_ON + MAPPING_AVC_FF: 					indx = 3; break; // ON + AVC FF

		case MAPPING_ON + MAPPING_SEGM + MAPPING_AVC_FF: 	indx = 4; break; // ON + Segments + AVC FF

		default: indx = 5; break; // wrong setup	


	}

	
return indx;

}

void EncodeMappingIndx(CurCalib_typ *cal , UINT indx){

	if(!cal)
		return ;


	switch(indx){

		case 0: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_OFF; break; // OFF 

		case 1: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_ON; break; // ON

		case 2: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_ON + MAPPING_SEGM; break; // ON + Segments

		case 3: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_ON + MAPPING_AVC_FF; break;// ON + AVC FF 

		case 4: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_ON + MAPPING_SEGM + MAPPING_AVC_FF ; break; // ON + Segments + AVC FF

		default: cal->GenSetup[SETUP_MAPPING_ON] = MAPPING_OFF; break; // wrong setup	
	}

	return;
}

void WatchSegmented_Yes_No(MASTER_typ *m){
int i, k;
char status;
//Cfg_typ *cfg;
CurCfg_typ *curCfg;

	if(!m)
		return;

	//cfg = (Cfg_typ*) m->pCfg;
	curCfg = (Cfg_typ*) m->pCurCfg;

	
	if(!curCfg)
		return;

	status = 0;
	for (i = 0 ; i < m->paramLastNum; i++){

		if(curCfg->Param[i].segmYes != _segmYes[i])
			status = 1;
		
		
	}	

	if(status){	
			
		ParametrAssignToPointers(m, m->curSegm[ZONE]);

		for (i = 0 ; i < m->paramLastNum; i++)
			_segmYes[i] = curCfg->Param[i].segmYes;
	}
	
return;

}

///////////   GLOBAL   Setting Display

void Param_Global_Standard(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf){
CurCalib_typ *curCalib , *defaultCalib;

	

	if(!hmi->pHMI_Lines[0] || !hmi->pHMI_Lines[1])
		return;

	
		curCalib = m->pCalib;
		defaultCalib = m->pDefaultCalib;

		//xsprintf(hmi->pHMI_Lines[0], "TVL Distance: %d" , curCalib->GenSetup[SETUP_TVL_DISTANCE]); 
		//xsprintf(hmi->pHMI_Lines[0], "Pos Sgm No: %d" , m->numSegm); 

		
		switch(curCalib->GenSetup[SETUP_AUTO_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 
			case 1: strcpy(hmi->pHMI_Lines[0], "Segm Fill: ON"); break; 
			case 2: strcpy(hmi->pHMI_Lines[0], "Segm Fill: AUTO"); break; 
			default: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 

		}
		
		switch(curCalib->GenSetup[SETUP_WHERE_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 
			case 1: strcpy(hmi->pHMI_Lines[1], "Fill: only Kbd"); break; 
			case 2: strcpy(hmi->pHMI_Lines[1], "Fill: only Editor"); break; 
			default: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 

		}
		
		xsprintf(hmi->pHMI_Lines[2], "Ticks Per Rot: %d" , curCalib->GenSetup[SETUP_PIN_PER_ROT]); 

		switch(curCalib->GenSetup[SETUP_MAPPING_ON]) {

			case MAPPING_OFF: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // OFF 

			case MAPPING_ON: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: ON"); break; // ON

			case MAPPING_ON+MAPPING_SEGM: 				xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments"); break; // ON + Segments

			case MAPPING_ON+MAPPING_AVC_FF: 		xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + AVC FF"); break;// ON + AVC FF 

			case MAPPING_ON+MAPPING_SEGM+MAPPING_AVC_FF:xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments + AVC FF"); break; // ON + Segments + AVC FF

			default: curCalib->GenSetup[SETUP_MAPPING_ON] = 0 ; xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // wrong setup	
			
			/*
			case 1: xsprintf(hmi->pHMI_Lines[3], "Mapping: ON");	break;

			default: xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF");	break;	
			*/

		}
		
		switch(curCalib->GenSetup[SETUP_AUTO_START]) {

			case 1: xsprintf(hmi->pHMI_Lines[4], "Auto Start: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[4], "Auto Start: OFF");	break;						

		}
		
		xsprintf(hmi->pHMI_Lines[5], "Auto Start Delay: %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]); 

		

		switch(curCalib->GenSetup[SETUP_SOUND_DELAY]) {

			case 1: xsprintf(hmi->pHMI_Lines[6], "Sound: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[6], "Sound: OFF");	break;						

		}					

		xsprintf(hmi->pHMI_Lines[7], "Sound Time:  %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);


		if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // On OSC


			
			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: OUT");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: IN"); break;						

			}


		}
		else{

			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: UP");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: DWN");	break;						

			}

		}

		switch(curCalib->GenSetup[SETUP_INDX_ENBL]) {

			case 1: xsprintf(hmi->pHMI_Lines[9], "INDX: ENABLED");		break;

			case 2: xsprintf(hmi->pHMI_Lines[9], "INDX: DISABLED");		break;

			case 3: xsprintf(hmi->pHMI_Lines[9], "INDX: on OSC");		break;

			default: xsprintf(hmi->pHMI_Lines[9], "INDX: Not Installed");	break;						

		}

		
		switch(curCalib->GenSetup[SETUP_ZONE_CENTR]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[10], "SEGM: Gravity Centric");		break;

			default: xsprintf(hmi->pHMI_Lines[10], "SEGM: Start Centric");	break;						

		}

		switch(curCalib->GenSetup[SETUP_TWO_TORCH]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[11], "TORCH: Dual");		break;

			default: xsprintf(hmi->pHMI_Lines[11], "TORCH: Single");	break;						

		}

		switch(curCalib->GenSetup[SETUP_TWO_UP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[12], "WELD: DOUBLE UP");		break;

			default: xsprintf(hmi->pHMI_Lines[12], "WELD: 360 deg.");	break;						

		}	
		
		/*
		switch(curCalib->GenSetup[SETUP_CONTIN_WELD]) {
		
			case 2: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN.");		break;

			case 1: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN. ONE WAY");		break;

			default: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: STANDARD");	break;						

		}
		*/
		strcpy(hmi->pHMI_Lines[13], "********");

		switch(curCalib->GenSetup[SETUP_TVL_ENC_SET]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: YES");		break;

			default: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: NO");	break;						

		}	

		switch(curCalib->GenSetup[SETUP_TVL_SEC_ENC]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Sec. Enc.");		break;

			default: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Mtr. Enc.");	break;						

		}	
		
		
		AssignHeadText(hmi->pHMI_Lines[16] ,   NextHeadConfig_Indx);


		switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

			default:

				switch(curCalib->GenSetup[SETUP_AVC_LEGACY]) {
				
					case 1: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Bang Bang");		break;

					default: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Normal");	break;						

				}

			break;

			case F_HEAD_HP_ENC_BEMF:
			case U_BEND_HP_ENC_BEMF:
			case SUPER_MINI:
			case BPV_F_HEAD:
			case BPV_FACE_OSC:	
			case BPV_FACE_OSC_ENC:
			case BPV_FACE_OSC_EXP:
			case BPV2_FACE_OSC_EXP:	
				
			case BPV_v2:
			case BPV_v2_MPBO:	
			case BPV_v2_MPGH:
			case BPV_v2_MPGV:	

			case BPV_v2_MT_BO:
			case BPV_v2_MT_BO_TOS:
			case BPV_v2_TT_BO:
			case BPV_v2_TT_BO_TOS:
			//dodane 6Dec23
			case BPV_v2_MT:
			case BPV_v1_MT:
			//dodane 8July24
			case BPV_v2_Video:
			case CLAMSHELL_HEAD:
			case CHERRYPOINT_BP1_HEAD:
			case CHERRYPOINT_BP2_HEAD:
			case CHERRYPOINT_MT_BP2_HEAD:	
			case MIG_TIG_HEAD: 	
				
				strcpy(hmi->pHMI_Lines[17], "********");	
				
						
			break;

			

		}

		switch(curCalib->GenSetup[SETUP_WF_WP_SWAP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[18], "WF Normal");		break;

			default: xsprintf(hmi->pHMI_Lines[18], "WF Swapped");	break;						

		}	

		

				
	return;
}

void Param_Global_BPV_v2_MPBO_STANDARD(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf){
CurCalib_typ *curCalib , *defaultCalib;

	

	if(!hmi->pHMI_Lines[0] || !hmi->pHMI_Lines[1])
		return;

	
		curCalib = m->pCalib;
		defaultCalib = m->pDefaultCalib;

		//xsprintf(hmi->pHMI_Lines[0], "TVL Distance: %d" , curCalib->GenSetup[SETUP_TVL_DISTANCE]); 
		//xsprintf(hmi->pHMI_Lines[0], "Pos Sgm No: %d" , m->numSegm); 

		
		switch(curCalib->GenSetup[SETUP_AUTO_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 
			case 1: strcpy(hmi->pHMI_Lines[0], "Segm Fill: ON"); break; 
			case 2: strcpy(hmi->pHMI_Lines[0], "Segm Fill: AUTO"); break; 
			default: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 

		}
		
		switch(curCalib->GenSetup[SETUP_WHERE_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 
			case 1: strcpy(hmi->pHMI_Lines[1], "Fill: only Kbd"); break; 
			case 2: strcpy(hmi->pHMI_Lines[1], "Fill: only Editor"); break; 
			default: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 

		}
		
		xsprintf(hmi->pHMI_Lines[2], "Ticks Per Rot: %d" , curCalib->GenSetup[SETUP_PIN_PER_ROT]); 

		switch(curCalib->GenSetup[SETUP_MAPPING_ON]) {

			case MAPPING_OFF: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // OFF 

			case MAPPING_ON: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: ON"); break; // ON

			case MAPPING_ON+MAPPING_SEGM: 				xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments"); break; // ON + Segments

			case MAPPING_ON+MAPPING_AVC_FF: 		xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + AVC FF"); break;// ON + AVC FF 

			case MAPPING_ON+MAPPING_SEGM+MAPPING_AVC_FF:xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments + AVC FF"); break; // ON + Segments + AVC FF

			default: curCalib->GenSetup[SETUP_MAPPING_ON] = 0 ; xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // wrong setup	
			
			/*
			case 1: xsprintf(hmi->pHMI_Lines[3], "Mapping: ON");	break;

			default: xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF");	break;	
			*/

		}
		
		switch(curCalib->GenSetup[SETUP_AUTO_START]) {

			case 1: xsprintf(hmi->pHMI_Lines[4], "Auto Start: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[4], "Auto Start: OFF");	break;						

		}
		
		xsprintf(hmi->pHMI_Lines[5], "Auto Start Delay: %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]); 

		

		switch(curCalib->GenSetup[SETUP_SOUND_DELAY]) {

			case 1: xsprintf(hmi->pHMI_Lines[6], "Sound: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[6], "Sound: OFF");	break;						

		}					

		xsprintf(hmi->pHMI_Lines[7], "Sound Time:  %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);


		if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // On OSC


			
			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: OUT");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: IN"); break;						

			}


		}
		else{

			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: UP");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: DWN");	break;						

			}

		}

		switch(curCalib->GenSetup[SETUP_INDX_ENBL]) {

			case 1: xsprintf(hmi->pHMI_Lines[9], "INDX: ENABLED");		break;

			case 2: xsprintf(hmi->pHMI_Lines[9], "INDX: DISABLED");		break;

			case 3: xsprintf(hmi->pHMI_Lines[9], "INDX: on OSC");		break;

			default: xsprintf(hmi->pHMI_Lines[9], "INDX: Not Installed");	break;						

		}

		
		switch(curCalib->GenSetup[SETUP_ZONE_CENTR]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[10], "SEGM: Gravity Centric");		break;

			default: xsprintf(hmi->pHMI_Lines[10], "SEGM: Start Centric");	break;						

		}

		switch(curCalib->GenSetup[SETUP_TWO_TORCH]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[11], "TORCH: Dual");		break;

			default: xsprintf(hmi->pHMI_Lines[11], "TORCH: Single");	break;						

		}

		switch(curCalib->GenSetup[SETUP_TWO_UP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[12], "WELD: DOUBLE UP");		break;

			default: xsprintf(hmi->pHMI_Lines[12], "WELD: 360 deg.");	break;						

		}	
		//strcpy(hmi->pHMI_Lines[13], "********");
		
		switch(curCalib->GenSetup[SETUP_CONTIN_WELD]) {

			
			case 1: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN. ONE WAY");		break;
		
			case 2: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN.");		break;

	
			default: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: STANDARD");	break;						

		}		

		switch(curCalib->GenSetup[SETUP_TVL_ENC_SET]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: YES");		break;

			default: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: NO");	break;						

		}	

		switch(curCalib->GenSetup[SETUP_TVL_SEC_ENC]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Sec. Enc.");		break;

			default: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Mtr. Enc.");	break;						

		}	
		
		
		AssignHeadText(hmi->pHMI_Lines[16] ,   NextHeadConfig_Indx);


		switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

			default:

				switch(curCalib->GenSetup[SETUP_AVC_LEGACY]) {
				
					case 1: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Bang Bang");		break;

					default: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Normal");	break;						

				}

			break;

			case F_HEAD_HP_ENC_BEMF:
			case U_BEND_HP_ENC_BEMF:
			case SUPER_MINI:
			case BPV_F_HEAD:
			case BPV_FACE_OSC:	
			case BPV_FACE_OSC_ENC:
			case BPV_FACE_OSC_EXP:	
			case BPV2_FACE_OSC_EXP:
				
			case BPV_v2:
			case BPV_v2_MPBO:	
			case BPV_v2_MPGH:
			case BPV_v2_MPGV:	

			case BPV_v2_MT_BO:
			case BPV_v2_MT_BO_TOS:
			case BPV_v2_TT_BO:
			case BPV_v2_TT_BO_TOS:
			//dodane 6Dec23
			case BPV_v2_MT:
			case BPV_v1_MT:	
			// dodane 8July24
			case BPV_v2_Video:
			case CLAMSHELL_HEAD:
			case CHERRYPOINT_BP1_HEAD:
			case CHERRYPOINT_BP2_HEAD:	
			case CHERRYPOINT_MT_BP2_HEAD:	
			case MIG_TIG_HEAD: 	

				strcpy(hmi->pHMI_Lines[17], "********");	
				
			break;

			

		}

		switch(curCalib->GenSetup[SETUP_WF_WP_SWAP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[18], "WF Normal");		break;

			default: xsprintf(hmi->pHMI_Lines[18], "WF Swapped");	break;						

		}	


		
		switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x3) {


			case 0x0: // No ramp , no retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , No Retract");

			break;

			case 0x1: // No Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , Retract");
				
			break;

			case 0x2: // Ramp , No Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , No Retract");

			break;

			case 0x3: // Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , Retract");

			break;
					

		}


		if(hmi->curNodeID < P_PARAM_GLOBAL+10)
			return;
			
					
		if(curCalib->GenSetup[SETUP_WF_RAMP] > 0x3)
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp ON");
		else
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp OFF");
			
						
	return;
}

void Param_Global_BPV_v2_MPBO_ONEWAY(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf){
CurCalib_typ *curCalib , *defaultCalib;

	

	if(!hmi->pHMI_Lines[0] || !hmi->pHMI_Lines[1])
		return;

	
		curCalib = m->pCalib;
		defaultCalib = m->pDefaultCalib;

				
		switch(curCalib->GenSetup[SETUP_AUTO_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 
			case 1: strcpy(hmi->pHMI_Lines[0], "Segm Fill: ON"); break; 
			case 2: strcpy(hmi->pHMI_Lines[0], "Segm Fill: AUTO"); break; 
			default: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 

		}
		
		switch(curCalib->GenSetup[SETUP_WHERE_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 
			case 1: strcpy(hmi->pHMI_Lines[1], "Fill: only Kbd"); break; 
			case 2: strcpy(hmi->pHMI_Lines[1], "Fill: only Editor"); break; 
			default: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 

		}
		
		xsprintf(hmi->pHMI_Lines[2], "Ticks Per Rot: %d" , curCalib->GenSetup[SETUP_PIN_PER_ROT]); 

		switch(curCalib->GenSetup[SETUP_MAPPING_ON]) {

			case MAPPING_OFF: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // OFF 

			case MAPPING_ON: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: ON"); break; // ON

			case MAPPING_ON+MAPPING_SEGM: 				xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments"); break; // ON + Segments

			case MAPPING_ON+MAPPING_AVC_FF: 		xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + AVC FF"); break;// ON + AVC FF 

			case MAPPING_ON+MAPPING_SEGM+MAPPING_AVC_FF:xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments + AVC FF"); break; // ON + Segments + AVC FF

			default: curCalib->GenSetup[SETUP_MAPPING_ON] = 0 ; xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // wrong setup	
			
			
		}

		
		switch(curCalib->GenSetup[SETUP_AUTO_START]) {

			case 1: xsprintf(hmi->pHMI_Lines[4], "Auto Start: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[4], "Auto Start: OFF");	break;						

		}
		
		
		xsprintf(hmi->pHMI_Lines[5], "Auto Start Delay: %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]); 
			

		switch(curCalib->GenSetup[SETUP_SOUND_DELAY]) {

			case 1: xsprintf(hmi->pHMI_Lines[6], "Sound: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[6], "Sound: OFF");	break;						

		}					

		xsprintf(hmi->pHMI_Lines[7], "Sound Time:  %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);


		
		if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // On OSC


			
			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: OUT");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: IN"); break;						

			}


		}
		else{

			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: UP");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: DWN");	break;						

			}

		}
		
	
		
		switch(curCalib->GenSetup[SETUP_INDX_ENBL]) {

			case 1: xsprintf(hmi->pHMI_Lines[9], "INDX: ENABLED");		break;

			case 2: xsprintf(hmi->pHMI_Lines[9], "INDX: DISABLED");		break;

			case 3: xsprintf(hmi->pHMI_Lines[9], "INDX: on OSC");		break;

			default: xsprintf(hmi->pHMI_Lines[9], "INDX: Not Installed");	break;						

		}		

		/*
		switch(curCalib->GenSetup[SETUP_ZONE_CENTR]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[10], "SEGM: Gravity Centric");		break;

			default: xsprintf(hmi->pHMI_Lines[10], "SEGM: Start Centric");	break;						

		}
		*/
		
		//curCalib->GenSetup[SETUP_ZONE_CENTR] = 0;
		xsprintf(hmi->pHMI_Lines[10], "*************");	

		/*
		switch(curCalib->GenSetup[SETUP_TWO_TORCH]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[11], "TORCH: Dual");		break;

			default: xsprintf(hmi->pHMI_Lines[11], "TORCH: Single");	break;						

		}
		*/
		
		//curCalib->GenSetup[SETUP_TWO_TORCH] = 0;
		xsprintf(hmi->pHMI_Lines[11], "*************");	

		/*
		switch(curCalib->GenSetup[SETUP_TWO_UP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[12], "WELD: DOUBLE UP");		break;

			default: xsprintf(hmi->pHMI_Lines[12], "WELD: 360 deg.");	break;						

		}
		*/
		
		//curCalib->GenSetup[SETUP_TWO_UP] = 0;
		xsprintf(hmi->pHMI_Lines[12], "*************");	

	
		switch(curCalib->GenSetup[SETUP_CONTIN_WELD]) {		
			
			case 1: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN. ONE WAY");		break;
		
			case 2: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN.");		break;
			
			default: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: STANDARD");	break;						

		}	

		switch(curCalib->GenSetup[SETUP_TVL_ENC_SET]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: YES");		break;

			default: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: NO");	break;						

		}	

		switch(curCalib->GenSetup[SETUP_TVL_SEC_ENC]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Sec. Enc.");		break;

			default: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Mtr. Enc.");	break;						

		}	
		
		
		AssignHeadText(hmi->pHMI_Lines[16] ,   NextHeadConfig_Indx);


		switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

			default:

				switch(curCalib->GenSetup[SETUP_AVC_LEGACY]) {
				
					case 1: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Bang Bang");		break;

					default: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Normal");	break;						

				}

			break;

			case F_HEAD_HP_ENC_BEMF:
			case U_BEND_HP_ENC_BEMF:
			case SUPER_MINI:
			case BPV_F_HEAD:
			case BPV_FACE_OSC:	
			case BPV_FACE_OSC_ENC:
			case BPV_FACE_OSC_EXP:	
			case BPV2_FACE_OSC_EXP:	

			case BPV_v2:
			case BPV_v2_MPBO:	
			case BPV_v2_MPGH:
			case BPV_v2_MPGV:	

			case BPV_v2_MT_BO:
			case BPV_v2_MT_BO_TOS:
			case BPV_v2_TT_BO:
			case BPV_v2_TT_BO_TOS:
			//dodane 6Dec23
			case BPV_v2_MT:
			case BPV_v1_MT:	
			// dodane 8July24
			case BPV_v2_Video:
			case CLAMSHELL_HEAD:
			case CHERRYPOINT_BP1_HEAD:
			case CHERRYPOINT_BP2_HEAD:
			case CHERRYPOINT_MT_BP2_HEAD:	
			case MIG_TIG_HEAD: 	
			
				strcpy(hmi->pHMI_Lines[17], "********");
				
			break;

			

		}

		
		switch(curCalib->GenSetup[SETUP_WF_WP_SWAP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[18], "WF Swapped");		break;

			default: xsprintf(hmi->pHMI_Lines[18], "WF Normal");	break;						

		}

		
		switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x3) {


			case 0x0: // No ramp , no retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , No Retract");

			break;

			case 0x1: // No Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , Retract");
				
			break;

			case 0x2: // Ramp , No Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , No Retract");

			break;

			case 0x3: // Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , Retract");

			break;
					

		}

		if(hmi->curNodeID < P_PARAM_GLOBAL+10)
			return;

		if(curCalib->GenSetup[SETUP_WF_RAMP] > 0x3)
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp ON");
		else
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp OFF");
		

				
	return;
}

void Param_Global_BPV_v2_MPBO_CONTIN(MASTER_typ *m , LCD_NODE_obj *nodes ,HMI_typ *hmi  , HMI_INTRF_typ *intrf){
CurCalib_typ *curCalib , *defaultCalib;

	

	if(!hmi->pHMI_Lines[0] || !hmi->pHMI_Lines[1])
		return;

	
		curCalib = m->pCalib;
		defaultCalib = m->pDefaultCalib;

				
		switch(curCalib->GenSetup[SETUP_AUTO_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 
			case 1: strcpy(hmi->pHMI_Lines[0], "Segm Fill: ON"); break; 
			case 2: strcpy(hmi->pHMI_Lines[0], "Segm Fill: AUTO"); break; 
			default: strcpy(hmi->pHMI_Lines[0], "Segm Fill: OFF"); break; 

		}
		
		switch(curCalib->GenSetup[SETUP_WHERE_FILL]){

			case 0: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 
			case 1: strcpy(hmi->pHMI_Lines[1], "Fill: only Kbd"); break; 
			case 2: strcpy(hmi->pHMI_Lines[1], "Fill: only Editor"); break; 
			default: strcpy(hmi->pHMI_Lines[1], "Fill: Kbd & Editor"); break; 

		}
		
		xsprintf(hmi->pHMI_Lines[2], "Ticks Per Rot: %d" , curCalib->GenSetup[SETUP_PIN_PER_ROT]); 

		switch(curCalib->GenSetup[SETUP_MAPPING_ON]) {

			case MAPPING_OFF: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // OFF 

			case MAPPING_ON: 							xsprintf(hmi->pHMI_Lines[3], "Mapping: ON"); break; // ON

			case MAPPING_ON+MAPPING_SEGM: 				xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments"); break; // ON + Segments

			case MAPPING_ON+MAPPING_AVC_FF: 		xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + AVC FF"); break;// ON + AVC FF 

			case MAPPING_ON+MAPPING_SEGM+MAPPING_AVC_FF:xsprintf(hmi->pHMI_Lines[3], "Mapping: ON + Segments + AVC FF"); break; // ON + Segments + AVC FF

			default: curCalib->GenSetup[SETUP_MAPPING_ON] = 0 ; xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF"); break; // wrong setup	
			
			/*
			case 1: xsprintf(hmi->pHMI_Lines[3], "Mapping: ON");	break;

			default: xsprintf(hmi->pHMI_Lines[3], "Mapping: OFF");	break;	
			*/

		}

		/*
		switch(curCalib->GenSetup[SETUP_AUTO_START]) {

			case 1: xsprintf(hmi->pHMI_Lines[4], "Auto Start: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[4], "Auto Start: OFF");	break;						

		}
		*/
		//curCalib->GenSetup[SETUP_AUTO_START] = 0;
		 xsprintf(hmi->pHMI_Lines[4], "**********");
		
		//xsprintf(hmi->pHMI_Lines[5], "Auto Start Delay: %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_AUTOSTART_DLY]); 
		xsprintf(hmi->pHMI_Lines[5], "**********");		

		

		switch(curCalib->GenSetup[SETUP_SOUND_DELAY]) {

			case 1: xsprintf(hmi->pHMI_Lines[6], "Sound: ON");		break;

			default: xsprintf(hmi->pHMI_Lines[6], "Sound: OFF");	break;						

		}					

		xsprintf(hmi->pHMI_Lines[7], "Sound Time:  %3.1f s" , curCalib->GenFloatValue[SETUP_FLOAT_SOUND_TM]);

	
		/*
		if(curCalib->GenSetup[SETUP_INDX_ENBL] == 3){ // On OSC


			
			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: OUT");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: IN"); break;						

			}


		}
		else{

			switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

				case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: UP");	break;
					

				default: xsprintf(hmi->pHMI_Lines[8], "INDX: DWN");	break;						

			}

		}
		*/
		switch(curCalib->GenSetup[SETUP_INDX_DIR]) {

			case 1: xsprintf(hmi->pHMI_Lines[8], "INDX: UP");	break;
				

			default: xsprintf(hmi->pHMI_Lines[8], "INDX: DWN");	break;						

		}
	
		
		switch(curCalib->GenSetup[SETUP_INDX_ENBL]) {

			case 1: xsprintf(hmi->pHMI_Lines[9], "INDX: ENABLED");		break;

			case 2: xsprintf(hmi->pHMI_Lines[9], "INDX: DISABLED");		break;

			case 3: xsprintf(hmi->pHMI_Lines[9], "INDX: on OSC");		break;

			default: xsprintf(hmi->pHMI_Lines[9], "INDX: Not Installed");	break;						

		}
		
		//curCalib->GenSetup[SETUP_INDX_ENBL] = 0;
		//xsprintf(hmi->pHMI_Lines[9], "*************");	


		/*
		
		switch(curCalib->GenSetup[SETUP_ZONE_CENTR]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[10], "SEGM: Gravity Centric");		break;

			default: xsprintf(hmi->pHMI_Lines[10], "SEGM: Start Centric");	break;						

		}
		*/
		//curCalib->GenSetup[SETUP_ZONE_CENTR] = 0;
		xsprintf(hmi->pHMI_Lines[10], "*************");	

		/*
		switch(curCalib->GenSetup[SETUP_TWO_TORCH]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[11], "TORCH: Dual");		break;

			default: xsprintf(hmi->pHMI_Lines[11], "TORCH: Single");	break;						

		}
		*/
		//curCalib->GenSetup[SETUP_TWO_TORCH] = 0;
		xsprintf(hmi->pHMI_Lines[11], "*************");	

		/*
		switch(curCalib->GenSetup[SETUP_TWO_UP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[12], "WELD: DOUBLE UP");		break;

			default: xsprintf(hmi->pHMI_Lines[12], "WELD: 360 deg.");	break;						

		}
		*/
		//curCalib->GenSetup[SETUP_TWO_UP] = 0;
		xsprintf(hmi->pHMI_Lines[12], "*************");	

	
		switch(curCalib->GenSetup[SETUP_CONTIN_WELD]) {		
			
			case 1: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN. ONE WAY");		break;
		
			case 2: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: CONTIN.");		break;
			
			default: xsprintf(hmi->pHMI_Lines[13], "WELD MODE: STANDARD");	break;						
		}	

		switch(curCalib->GenSetup[SETUP_TVL_ENC_SET]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: YES");		break;

			default: xsprintf(hmi->pHMI_Lines[14], "TVL ZERO after Head Swap: NO");	break;						

		}	

		switch(curCalib->GenSetup[SETUP_TVL_SEC_ENC]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Sec. Enc.");		break;

			default: xsprintf(hmi->pHMI_Lines[15], "TVL Feedback: Mtr. Enc.");	break;						

		}	
		
		
		AssignHeadText(hmi->pHMI_Lines[16] ,   NextHeadConfig_Indx);


		switch(curCalib->GenSetup[SETUP_HEAD_SETUP]){

			default:

				switch(curCalib->GenSetup[SETUP_AVC_LEGACY]) {
				
					case 1: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Bang Bang");		break;

					default: xsprintf(hmi->pHMI_Lines[17], "Legacy AVC: Normal");	break;						

				}

			break;

			case F_HEAD_HP_ENC_BEMF:
			case U_BEND_HP_ENC_BEMF:
			case SUPER_MINI:
			case BPV_F_HEAD:
			case BPV_FACE_OSC:	
			case BPV_FACE_OSC_ENC:
			case BPV_FACE_OSC_EXP:	
			case BPV2_FACE_OSC_EXP:	

			case BPV_v2:
			case BPV_v2_MPBO:	
			case BPV_v2_MPGH:
			case BPV_v2_MPGV:	

			case BPV_v2_MT_BO:
			case BPV_v2_MT_BO_TOS:
			case BPV_v2_TT_BO:
			case BPV_v2_TT_BO_TOS:

			//dodane 6Dec23
			case BPV_v2_MT:
			case BPV_v1_MT:	
			// dodane 8July24
			case BPV_v2_Video:
			case CLAMSHELL_HEAD:
			case CHERRYPOINT_BP1_HEAD:
			case CHERRYPOINT_BP2_HEAD:
			case CHERRYPOINT_MT_BP2_HEAD:
			case MIG_TIG_HEAD: 	
				
				strcpy(hmi->pHMI_Lines[17], "********");
			
			break;

			

		}

		
		switch(curCalib->GenSetup[SETUP_WF_WP_SWAP]) {
		
			case 1: xsprintf(hmi->pHMI_Lines[18], "WF Swapped");		break;

			default: xsprintf(hmi->pHMI_Lines[18], "WF Normal");	break;						

		}

		switch(curCalib->GenSetup[SETUP_WF_RAMP] & 0x3) {


			case 0x0: // No ramp , no retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , No Retract");

			break;

			case 0x1: // No Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp OFF , Retract");
				
			break;

			case 0x2: // Ramp , No Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , No Retract");

			break;

			case 0x3: // Ramp , Retract

				xsprintf(hmi->pHMI_Lines[19], "EN/DIS WF -Stop Ramp ON , Retract");

			break;
					

		}

		if(hmi->curNodeID < P_PARAM_GLOBAL+10)
			return;

		if(curCalib->GenSetup[SETUP_WF_RAMP] > 0x3)
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp ON");
		else
			xsprintf(hmi->pHMI_Lines[0], "EN/DIS WF - Start Ramp OFF");


		
		

				
	return;
}



#endif

