/********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: ParMgr
 * File: ParMgr.c
 * Author: ckaczkowski
 * Created: August 13, 2015
 ********************************************************************
 * Implementation of program ParMgr
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif


//#include <..\..\Lib\utilityLib\bgdebug.h>


#include <..\..\Universal\SuperDeclaration.h>
#include <..\..\Lib\Statelib.c>
#include <..\..\Lib\WSI_lib.c>

#include <sscanf2.h>



_LOCAL Cfg_typ *cfg;
_LOCAL CurCfg_typ *curCfg;
_LOCAL CurCalib_typ *curCalib , *defCalib;
_LOCAL Scaled_Var_obj *var;

_LOCAL XML_Helper_typ *helper;


_LOCAL plcstring LineTxt[128];
_LOCAL INT parametersDump_Step , paramIndx;
_LOCAL DTStructure DTStruct;
_LOCAL DTGetTime_typ DTGetTime_1;
_LOCAL UINT   ParamNum , Index , ParamIndex;


UINT FindParamIndex(Cfg_typ *cfg , char *p);
UINT FindParamIntIndex(Cfg_typ *cfg , char *p);
UINT FindParamEnumIndex(Cfg_typ *cfg , char *p);


_LOCAL UINT   KK_ParamNum[128] , KK_Index[128];
_LOCAL UINT KK_ParamNum_i , KK_Index_i;
_LOCAL plcstring KK_par_Name[64][32];
_LOCAL plcbit RunDBG;


void _INIT ParMgrINIT(void)
{

	RunDBG = 0;
	
}

void _CYCLIC ParMgrCYCLIC(void)
{



	curCfg = &CurCfg;
	cfg = &Cfg;
	helper = &XMLHelper;	
	
	switch(helper->state){


		case  WAIT:
			
			/// Command listener	
			switch(helper->xCmd){


				case XML_CMD_WRITE:	

					switch ( helper->runState){

						case 0:							
							
							helper->state = CREATE_WRITER;  /*jump to the state CREATE_WRITER*/
							helper->runState ++;  // increase the autostate
							
							
						break;	

						case 1:
							
							helper->state = START_ELEM; 
							strcpy(helper->para.elementName , "WeldProgram");
							strcpy(helper->para.elementText , "");						
							helper->runState ++;  // increase the autostate
							
						break;					

						
						case 2:

							if(strlen(curCfg->NextProgramName)==0)
								strcpy(curCfg->NextProgramName , PRG_NO_NEXT);

							helper->state = START_ELEM; 
							strcpy(helper->para.elementName , "NextProgramName");
							strcpy(helper->para.elementText , curCfg->NextProgramName);						
							helper->runState ++;  // increase the autostate							
							
						break;	

						case 3:
							
							helper->state = WRITE_ELEM_TEXT; 
							helper->runState ++;  // increase the autostate

						break;						

						case 4:
							
							helper->state = CLOSE_ELEM;
							helper->runState ++;  // increase the autostate							
							
						break;	


						
						case 5:
						

							if( ParamList.IndexNum_Int> 0) {
								
								ParamNum = 0;								
								Index = 0;
								ParamIndex = ParamList.Index_Int[ParamNum];
								
								helper->runState = 6;
								
							}
							else if( ParamList.IndexNum > 0) {
									
								ParamNum = 0;								
								Index = 0;
								ParamIndex = ParamList.Index[ParamNum];
								
								helper->runState = 12;
								
							}else if( ParamList.IndexNum_Enum> 0) {
								
								ParamNum = 0;								
								Index = 0;
								ParamIndex = ParamList.Index_Enum[ParamNum];
								
								helper->runState = 18;
								
							}
							else{
								
								helper->runState = 25;
								
							}	


						break;	

						
						////////////////////////////////////////////////////////
						//////	Integer
						case 6:
							
							helper->state = START_ELEM; 
							strcpy(helper->para.elementName , cfg->IntVar[ParamIndex].enumTXT);
							strcpy(helper->para.elementText , cfg->IntVar[ParamIndex].enumTXT);
							if(curCfg->Param_INT[ParamIndex].segmYes)
								Index = 1;
							else	
								Index = 0;
							
							helper->runState ++;  // increase the autostate
													

						break;	

						// ELEMENT VALUE
						case 7:
							
							helper->state = START_ELEM; 
							xsprintf(helper->para.elementName, "ParValue");
							xsprintf(helper->para.elementText, "%d", curCfg->Param_INT[ParamIndex].val[Index]);
							helper->runState ++;  // increase the autostate
													

						break;	

						case 8:
							
							helper->state = WRITE_ATTR; 
							strcpy(helper->para.attributeName , "Index");
							xsprintf(helper->para.attributeValue, "%d", Index);						
							helper->runState ++;  // increase the autostate  
							
						break;

						case 9:
							
							helper->state = WRITE_ELEM_TEXT; 
							helper->runState ++;  // increase the autostate

						break;				
						

						case 10:
							
							helper->state = CLOSE_ELEM; 

							if(curCfg->Param_INT[ParamIndex].segmYes){

								Index++;
																
								if(Index > Master_numZone)									
									helper->runState ++;  // increase the autostate
									
								else
									helper->runState = 7;
								
							}else								
								helper->runState ++;  // increase the autostate
								

						break;

						

						case 11:

							helper->state = CLOSE_ELEM; 

							ParamNum++;	

							if(ParamNum <  ParamList.IndexNum_Int){								
								
								ParamIndex = ParamList.Index_Int[ParamNum];
								
								helper->runState = 6;							

							}
							else {								

								if( ParamList.IndexNum > 0) {
									
									ParamNum = 0;								
									Index = 0;
									ParamIndex = ParamList.Index[ParamNum];
									
									helper->runState = 12;
									
								}
								else if( ParamList.IndexNum_Enum> 0) {
									
									ParamNum = 0;								
									Index = 0;
									ParamIndex = ParamList.Index_Enum[ParamNum];
									
									helper->runState = 18;
									
								}
								else{
									
									helper->runState = 25;
									
								}	
								
							}


						break;	
						/// END  Integer Parameters
						
						
						////////////////////////////////////////////////////////
						//////	Float Parameters
						case 12:
							
							helper->state = START_ELEM; 
							strcpy(helper->para.elementName , cfg->ScaledVar[ParamIndex].enumTXT);
							
							if(curCfg->Param[ParamIndex].segmYes)
								Index = 1;
							else	
								Index = 0;
							
							helper->runState ++;  // increase the autostate
													

						break;	

						// ELEMENT VALUE
						case 13:
							
							helper->state = START_ELEM; 
							strcpy(helper->para.elementName, "ParValue");
							ftoa(curCfg->Param[ParamIndex].val[Index], (UDINT)helper->para.elementText);  
							helper->runState ++;  // increase the autostate
													

						break;	

						case 14:
							
							helper->state = WRITE_ATTR; 
							strcpy(helper->para.attributeName , "Index");
							xsprintf(helper->para.attributeValue, "%d", Index);						
							helper->runState ++;  // increase the autostate  
							
						break;

						case 15:
							
							helper->state = WRITE_ELEM_TEXT; 
							helper->runState ++;  // increase the autostate

						break;				
						

						case 16:
							
							helper->state = CLOSE_ELEM; 

							if(curCfg->Param[ParamIndex].segmYes){

								Index++;
																
								if(Index > Master_numZone)									
									helper->runState ++;  // increase the autostate
									
								else
									helper->runState = 13;
								
							}else								
								helper->runState ++;  // increase the autostate
								

						break;

						

						case 17:
							
							helper->state = CLOSE_ELEM; 

							ParamNum++;	

							if(ParamNum <  ParamList.IndexNum){								
								
								ParamIndex = ParamList.Index[ParamNum];
								
								helper->runState = 12;							

							}							
							else {								

								if( ParamList.IndexNum_Enum> 0) {
									
									ParamNum = 0;								
									Index = 0;
									ParamIndex = ParamList.Index_Enum[ParamNum];
									
									helper->runState = 18;
									
								}
								else{
									
									helper->runState = 25;
									
								}	
								
							}

								
						break;

						
						/// END  Float Parameters

						////////////////////////////////////////////////////////
						//////	Enum  Parameters
						case 18:
							
							helper->state = START_ELEM; 
							strcpy(helper->para.elementName , cfg->EnumVar[ParamIndex].enumTXT);							

							if(curCfg->Param_ENUM[ParamIndex].segmYes)
								Index = 1;
							else	
								Index = 0;
							
							helper->runState ++;  // increase the autostate
													

						break;	

						// ELEMENT VALUE
						case 19:
							
							helper->state = START_ELEM; 
							xsprintf(helper->para.elementName, "ParValue");
							xsprintf(helper->para.elementText, "%d", curCfg->Param_ENUM[ParamIndex].val[Index]);							
							helper->runState ++;  // increase the autostate
													

						break;	

						case 20:
							
							helper->state = WRITE_ATTR; 
							strcpy(helper->para.attributeName , "Index");
							xsprintf(helper->para.attributeValue, "%d", Index);						
							helper->runState ++;  // increase the autostate  
							
						break;

						case 21:
							
							helper->state = WRITE_ELEM_TEXT;							
							helper->runState ++;  // increase the autostate

						break;				
						

						case 22:
							
							helper->state = CLOSE_ELEM; 

							if(curCfg->Param_ENUM[ParamIndex].segmYes){

								Index++;
																
								if(Index > Master_numZone)									
									helper->runState ++;  // increase the autostate
									
								else
									helper->runState = 19;
								
							}else								
								helper->runState ++;  // increase the autostate
								

						break;						

						case 23:
							
							helper->state = CLOSE_ELEM; 

							ParamNum++;	

							if(ParamNum <  ParamList.IndexNum_Enum){								
								
								ParamIndex = ParamList.Index_Enum[ParamNum];
								
								helper->runState = 18;							

							}
							else						
								helper->runState = 25;  

								
						break;

						
						/// END  Enum Parameters

						case 25:
							
							helper->state = CLOSE_ELEM;
							helper->runState ++;  // increase the autostate
							
						break;
						

						case 26:
							helper->state = CLOSE_DOC;  
							helper->runState ++;  // increase the autostate

						break;

						case 27:

							helper->runState ++;  // increase the autostate
							helper->pMemory = helper->fub.xmlGetMemoryInfo_0.pXmlData;
							helper->MemorySize = helper->fub.xmlGetMemoryInfo_0.xmlDataSize;

						break;	


						case 28:

							helper->xCmd =  0;  //reset the auto Command
							helper->runState =  0;  //reset the auto state
																										

					}
					break;

					///////////////////////////////////////////////////////////////////////////////

					case XML_CMD_READ:


						
						switch ( helper->runState){


							case 0:

								helper->runState ++;  // increase the autostate


							break;	
						
							case 1:

								helper->pMemory = File_RW.pMem;
								helper->MemorySize = File_RW.Len;

								helper->para_read.elementIndex = 0;
								helper->para_read.attributeIndex = 0;

								
								KK_ParamNum_i = 0;
								KK_Index_i = 0;
								

								if(helper->pMemory && helper->MemorySize){
									
									helper->state = CREATE_READER;									
									helper->runState ++;  // increase the autostate

									
								}
								else
									helper->runState = 255;

								
							break;	

							case 2:

								helper->state = READ_NODE;
								helper->runState ++;  // increase the autostate
																	
								
							break;	


							case 3:

								if(helper->para_read.nodeType == xmlNODE_TYPE_ELEMENT){

									if(!strstr(helper->para_read.nodeName , "ParValue"))
										strcpy(helper->para_read.xmlElements[1].name, helper->para_read.nodeName);

									strcpy(helper->para_read.xmlElements[0].name, helper->para_read.nodeName);									


								    if(helper->para_read.attributeCount > 0){										

										helper->state = READ_ATTR;
										helper->runState ++;  // increase the autostate


									}
									else{
										
										helper->runState = 2;

									}
							
								}
								else if(helper->para_read.nodeType == xmlNODE_TYPE_TEXT){

									
									Index = (UINT)atoi(helper->para_read.attributeValue);

									helper->para_read.elementIndex++;

									if(strstr(helper->para_read.xmlElements[0].name , "NextProgramName")){

										strcpy(curCfg->NextProgramName , helper->para_read.nodeValue);
										
									}
									else if(strstr(helper->para_read.xmlElements[0].name , "ParValue")){										
										

										if(strstr(helper->para_read.xmlElements[1].name , "parENUM_")){

														
											ParamNum = FindParamEnumIndex(cfg ,helper->para_read.xmlElements[1].name); 
											
											//if(RunDBG)
												//DBG("ParENUM %s %d %d %s \n\r" ,helper->para_read.xmlElements[1].name ,  ParamNum , Index , helper->para_read.nodeValue);

											if(Index <= SEGM_NUM_MAX && (ParamNum > 0))
												curCfg->Param_ENUM[ParamNum-1].val[Index] = (DINT)atof((UDINT)helper->para_read.nodeValue);
											

										}
										else if(strstr(helper->para_read.xmlElements[1].name , "parINT_")){
											
											ParamNum = FindParamIntIndex(cfg ,helper->para_read.xmlElements[1].name);
											
											//if(RunDBG)
												//DBG("ParINT %s %d \n\r" ,helper->para_read.xmlElements[1].name ,  ParamNum );

											
											if(Index <= SEGM_NUM_MAX && (ParamNum > 0))
												curCfg->Param_INT[ParamNum-1].val[Index] = (DINT)atof((UDINT)helper->para_read.nodeValue);
																				
											

										}
										else if(strstr(helper->para_read.xmlElements[1].name , "par")){

											ParamNum = FindParamIndex(cfg ,helper->para_read.xmlElements[1].name); 
											
											//if(RunDBG)
												//DBG("Par %s %d %d %s \n\r" ,helper->para_read.xmlElements[1].name ,  ParamNum , Index , helper->para_read.nodeValue);
										
											if(Index <= SEGM_NUM_MAX && (ParamNum > 0))
												curCfg->Param[ParamNum-1].val[Index] = atof((UDINT)helper->para_read.nodeValue);


										}
													
									}
									
									//helper->para_read.elementIndex++;		
									helper->runState = 2;

								}
								else if((helper->para_read.nodeType == xmlNODE_TYPE_ENDELEMENT) 
									&& (helper->para_read.nodeDepth == 0))									
									helper->runState = 11;

								else
									helper->runState = 2;


							break;	

							case 4:

								strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[0].name , helper->para_read.attributeName);
								strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[0].value ,  helper->para_read.attributeValue);							

								helper->runState = 2;
																	
								
							break;	


							case 11:

								helper->state = CLOSE_READER;
								helper->runState ++;  // increase the autostate
																	
								
							break;	

							default:

								helper->xCmd =  0;  //reset the auto Command
								helper->runState =  0;  //reset the auto state


							break;	

						}



					break;	
					//////////////////////////////////////////////////////////////////			


					case XML_CMD_GET_NEXT_PROGR:


						
						switch ( helper->runState){


							case 0:

								helper->WeldProgramRepeat = 0.0;	
								helper->runState ++;  // increase the autostate


							break;	
						
							case 1:

								helper->pMemory = File_RW.pMem;
								helper->MemorySize = File_RW.Len;

								helper->para_read.elementIndex = 0;
								helper->para_read.attributeIndex = 0;
								

								if(helper->pMemory && helper->MemorySize){
									
									helper->state = CREATE_READER;									
									helper->runState ++;  // increase the autostate

									//KK_ParamNum_i = 0;
									//KK_Index_i = 0;

								}
								else
									helper->runState = 255;

								
							break;	

							case 2:

								helper->state = READ_NODE;
								helper->runState ++;  // increase the autostate
																	
								
							break;	


							case 3:

								if(helper->para_read.nodeType == xmlNODE_TYPE_ELEMENT){

									//strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].name, helper->para_read.nodeName);
									strcpy(helper->para_read.xmlElements[1].name, helper->para_read.xmlElements[0].name);	
									strcpy(helper->para_read.xmlElements[0].name, helper->para_read.nodeName);									
										
									helper->para_read.elementIndex++;
									helper->runState = 2;		
																	
							
								}
								else if(helper->para_read.nodeType == xmlNODE_TYPE_TEXT){

									//strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].value , helper->para_read.nodeValue);
									//strcpy(helper->para_read.xmlElements[0].value , helper->para_read.nodeValue);


									if(strstr(helper->para_read.xmlElements[0].name , "NextProgramName")){

										strcpy(helper->NextProgramName , helper->para_read.nodeValue);

											helper->runState = 2;

									}
									else if(strstr(helper->para_read.xmlElements[0].name , "ParValue")){

										if(strstr(helper->para_read.xmlElements[1].name , "parGEN_WeldProgramRepeat")){											
											
											helper->WeldProgramRepeat = atof(helper->para_read.nodeValue);
											
											helper->runState = 11;
												
										}
										else																			
											helper->runState = 2;
									}
									else									
										helper->runState = 2;

								}
								else if((helper->para_read.nodeType == xmlNODE_TYPE_ENDELEMENT) 
									&& (helper->para_read.nodeDepth == 0))									
									helper->runState = 11;

								else
									helper->runState = 2;


							break;	

							case 4:

								strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[0].name , helper->para_read.attributeName);
								strcpy(helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[0].value ,  helper->para_read.attributeValue);							

								helper->runState = 2;
																	
								
							break;	


							case 11:

								helper->state = CLOSE_READER;
								helper->runState ++;  // increase the autostate
																	
								
							break;	

							default:

								helper->xCmd =  0;  //reset the auto Command
								helper->runState =  0;  //reset the auto state


							break;	

						}



					break;	
					//////////////////////////////////////////////////////////////////

					case XML_CMD_SET_NEXT_PROGR:
						
						switch ( helper->runState){


							case 0:

								helper->runState ++;  // increase the autostate


							break;	
						
							case 1:

								helper->pMemory = File_RW.pMem;
								helper->MemorySize = File_RW.Len;

								helper->para_read.elementIndex = 0;
								helper->para_read.attributeIndex = 0;
								

								if(helper->pMemory && helper->MemorySize){
									
									helper->state = CREATE_READER;									
									helper->runState ++;  // increase the autostate

								}
								else
									helper->runState = 255;

								
							break;	

							case 2:							
							
								helper->state = CREATE_WRITER;  /*jump to the state CREATE_WRITER*/
								helper->runState = 5;  // increase the autostate
							
							
							break;								

							case 5:

								helper->state = READ_NODE;
								helper->runState ++;  // increase the autostate
																	
								
							break;	


							case 6:



								switch(helper->para_read.nodeType){


									case 1: // xmlNODE_TYPE_ELEMENT

										helper->para_read.xmlElements[0].attributeCount = helper->para_read.attributeCount;
										strcpy(helper->para_read.xmlElements[0].name, helper->para_read.nodeName);
										
										strcpy(helper->para.elementName, helper->para_read.nodeName);
										
										helper->state = START_ELEM;
										helper->runState++;


									break;	

									case 3: // xmlNODE_TYPE_TEXT

										strcpy(helper->para_read.xmlElements[0].value, helper->para_read.nodeValue);

										if(!strcmp(helper->para_read.xmlElements[0].name , "NextProgramName")){

											if(strlen(helper->NextProgramName))
												strcpy(helper->para.elementText , helper->NextProgramName);
											else
												strcpy(helper->para.elementText , PRG_NO_NEXT);
																							
										}
										else
											strcpy(helper->para.elementText , helper->para_read.nodeValue);

										
										if(strlen(helper->para.elementText))
											helper->state = WRITE_ELEM_TEXT;
										
										helper->runState = 5; // Next Node


									break;	

									case 15: // xmlNODE_TYPE_ENDELEMENT

										helper->state = CLOSE_ELEM;

										if(helper->para_read.nodeDepth == 0){
											
											helper->runState = 10; // Happy End
										}	
										else{
											
											helper->para_read.elementIndex++;											

											helper->runState = 5; // Next Node
										}
											


									break;	

									default:

										helper->runState = 5;

									break;	


								}									
								

							break;	


							case 7:

								if(helper->para_read.xmlElements[0].attributeCount > 0){
									
									helper->state = READ_ATTR;
									helper->runState++;	
								}	
								else
									helper->runState = 5; // Next Node



							break;	

							case 8: // After read Attribute

								strcpy(helper->para_read.xmlElements[0].attributes[0].name , helper->para_read.attributeName);
								strcpy(helper->para_read.xmlElements[0].attributes[0].value ,  helper->para_read.attributeValue);

								strcpy(helper->para.attributeName , helper->para_read.attributeName);
								strcpy(helper->para.attributeValue, helper->para_read.attributeValue);
								
								helper->state = WRITE_ATTR;

								
								helper->runState = 5; // Next Node
																	
								
							break;	


							case 10:

								helper->state = CLOSE_READER;
								helper->runState ++;  // increase the autostate
																	
								
							break;	


							case 11:

								helper->state = CLOSE_DOC;
								helper->runState ++;  // increase the autostate

							break;

							case 12:

								helper->pMemory = helper->fub.xmlGetMemoryInfo_0.pXmlData;
								helper->MemorySize = helper->fub.xmlGetMemoryInfo_0.xmlDataSize;

								helper->runState ++;  // increase the autostate
								
							break;	
								

							default:

								helper->xCmd =  0;  //reset the auto Command
								helper->runState =  0;  //reset the auto state


							break;	

						}



					break;	

					//////////////////////////////////////////////////////////////////
					
					case XML_CMD_CLOSE_WRITER:


						
						switch ( helper->runState){
												
							case 0:

								
								helper->state = CLOSE_WRITER;  
								helper->runState ++;  // increase the autostate
								
							break;	

							case 1:									

								helper->xCmd =	0;	//reset the auto Command
								helper->runState =	0;	//reset the auto state
																	
								
							break;	

						}						

					break;


					default:

						helper->xCmd =  0;  /*reset the auto Command*/
						helper->runState =  0;  /*reset the auto state*/
						

					break;	
					
				}

		/////////////////////////////////////////////////////////////////////////////////////	

		break;

		
		case CREATE_WRITER:	/*state to create a writer on the memory*/
			
			helper->fub.xmlCreateMemoryWriter_0.enable = 1;  /*enable the FUB*/
			//helper->fub.xmlCreateMemoryWriter_0();	/*call the Function Block*/
			xmlCreateMemoryWriter(&helper->fub.xmlCreateMemoryWriter_0);
			
			if (helper->fub.xmlCreateMemoryWriter_0.status == 0) {	/*check if the writer on the memory was created successfully*/
				helper->para.ident = helper->fub.xmlCreateMemoryWriter_0.ident;  /*copy the ident on a variable*/
				helper->state = START_DOC;  /*jump to the state START_DOC*/
			}
			else if(helper->fub.xmlCreateMemoryWriter_0.status != ERR_FUB_BUSY)  /*check if the FUB is still busy*/
				helper->state = ERROR;  /*go to the ERROR state*/
			
		break;	

		case START_DOC:  /*state to write the XML Start Document*/
			
			helper->fub.xmlWriteStartDocument_0.enable = 1;  /*enable the FUB*/
			helper->fub.xmlWriteStartDocument_0.ident = helper->para.ident;  /*write the ident*/
			helper->fub.xmlWriteStartDocument_0.pVersion = "1.0";  /*write the version*/
   			helper->fub.xmlWriteStartDocument_0.pEncoding = "UTF-8";  /*write the encoding*/
   			xmlWriteStartDocument(&helper->fub.xmlWriteStartDocument_0);  /*Call the FUB*/
									
			if (helper->fub.xmlWriteStartDocument_0.status == 0){  /*check if the WriteStartDoc was successful*/
				helper->state = WAIT;  /*jump to the state WAIT*/
			}
			else if(helper->fub.xmlWriteStartDocument_0.status != ERR_FUB_BUSY)  /*check if the FUB is still busy*/
			{
				helper->errorState = START_DOC;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;	


		case START_ELEM:  /*state to write XML Start Element*/
			helper->fub.xmlWriteStartElement_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlWriteStartElement_0.ident =  helper->para.ident;  /*write the ident*/
			
			if(strlen(helper->para.elementName)<= 0)
				strcpy(helper->para.elementName , "ERROR_Name");
			
			helper->fub.xmlWriteStartElement_0.pName =  (UDINT)helper->para.elementName;  /*write the name of the Element*/
			
   			xmlWriteStartElement(&helper->fub.xmlWriteStartElement_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteStartElement_0.status == 0 ){  /*check if the WriteStartElement was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if(helper->fub.xmlWriteStartElement_0.status != ERR_FUB_BUSY) /*check if the FUB is still busy*/
			{
				helper->errorState = START_ELEM;
				helper->state = ERROR;  /*go to the ERROR state*/

			}

		break;	
			
		
		case WRITE_ATTR:  /*state to write a XML Attribute*/
			helper->fub.xmlWriteAttribute_0.enable =  1;  /*enable the FUB*/				
			helper->fub.xmlWriteAttribute_0.ident =  helper->para.ident;  /*write the ident*/
			
			if(strlen(helper->para.attributeName)<= 0)
				strcpy(helper->para.attributeName , "ERROR_AttrName");
			
			helper->fub.xmlWriteAttribute_0.pName =  (UDINT)helper->para.attributeName;  /*write the name of the Attribute*/
			
			if(strlen(helper->para.attributeValue)<= 0)
				strcpy(helper->para.attributeValue , "ERROR_AttrValue");
			
   			helper->fub.xmlWriteAttribute_0.pValue =  (UDINT)helper->para.attributeValue;  /*write the value of the Attribute*/
			
   			xmlWriteAttribute(&helper->fub.xmlWriteAttribute_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteAttribute_0.status ==  0){  /*check if the WriteAttribute was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if (helper->fub.xmlWriteAttribute_0.status != ERR_FUB_BUSY)  /*check if the FUB is still busy*/
			{
				helper->errorState = WRITE_ATTR;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;	

		case WRITE_ELEM_TEXT:  /*state to write XML Element Text*/
			helper->fub.xmlWriteElementText_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlWriteElementText_0.ident =  helper->para.ident;  /*write the ident*/

			if(strlen(helper->para.elementText)<= 0)
				strcpy(helper->para.elementText , "ERROR_elemTest");
			
			helper->fub.xmlWriteElementText_0.pText =  (UDINT)helper->para.elementText;  /*write the text of the Element*/
			
   			xmlWriteElementText(&helper->fub.xmlWriteElementText_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteElementText_0.status == 0){  /*check if the Write Text was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if( helper->fub.xmlWriteElementText_0.status != ERR_FUB_BUSY)  /*check if the FUB is still busy*/
			{
				helper->errorState = WRITE_ELEM_TEXT;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;
		
		case WRITE_CDATA:  /*state to write CDATA section*/
			helper->fub.xmlWriteCData_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlWriteCData_0.ident =  helper->para.ident;  /*write the ident*/
			helper->fub.xmlWriteCData_0.pCData =  (UDINT)helper->para.cDATA;  /*address of the data to be written*/
			
   			xmlWriteCData(&helper->fub.xmlWriteCData_0);  /*Call the FUB*/
									
			if( helper->fub.xmlWriteCData_0.status == 0){  /*check if the write CDATA was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if( helper->fub.xmlWriteCData_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = WRITE_CDATA;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;
		
		case WRITE_PI:  /*state to write a project instruction*/
			helper->fub.xmlWritePI_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlWritePI_0.ident =  helper->para.ident;  /*write the ident*/
			helper->fub.xmlWritePI_0.pTarget =  (UDINT) helper->para.piTarget;  /*target of the project instruction*/
			helper->fub.xmlWritePI_0.pValue =  (UDINT)helper->para.piValue;  /*value of the project instruction*/

			xmlWritePI(&helper->fub.xmlWritePI_0);  /*Call the FUB*/
									
			if( helper->fub.xmlWritePI_0.status == 0){  /*check if writing the PI was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if( helper->fub.xmlWritePI_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = WRITE_PI;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;	
		
		case WRITE_COMMENT:  /*state to write a comment*/
			helper->fub.xmlWriteComment_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlWriteComment_0.ident =  helper->para.ident;  /*write the ident*/
			helper->fub.xmlWriteComment_0.pText =  (UDINT)helper->para.comment;  /*comment to be written*/
			
   			xmlWriteComment(&helper->fub.xmlWriteComment_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteComment_0.status == 0){  /*check if writing the PI was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if( helper->fub.xmlWriteComment_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = WRITE_COMMENT;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;			
			
		case CLOSE_ELEM:  /*state to close the Element*/
			helper->fub.xmlWriteEndElement_0.enable =  1;  /*enable the FUB*/				
			helper->fub.xmlWriteEndElement_0.ident =  helper->para.ident;  /*write the ident*/
			
			xmlWriteEndElement(&helper->fub.xmlWriteEndElement_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteEndElement_0.status == 0){  /*check if the WriteEndElement was successful*/
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if(helper->fub.xmlWriteEndElement_0.status != ERR_FUB_BUSY)  /*check if the FUB is still busy*/
			{
				helper->errorState = CLOSE_ELEM;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;		
			
		case CLOSE_DOC:  /*state to close the document*/
			
			helper->fub.xmlWriteEndDocument_0.enable =  1;  /*enable the FUB*/				
			helper->fub.xmlWriteEndDocument_0.ident =  helper->para.ident;  /*write the ident*/
			
			xmlWriteEndDocument(&helper->fub.xmlWriteEndDocument_0);  /*Call the FUB*/
									
			if(helper->fub.xmlWriteEndDocument_0.status == 0)  /*check if the WriteEndElement was successful*/
				helper->state =  GET_MEMINFO;  /*jump to the state WAIT*/
			
			else if( helper->fub.xmlWriteEndDocument_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = CLOSE_DOC;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;
		
		case GET_MEMINFO:  /*state to get the info about the memory of the XML Data*/
			
			helper->fub.xmlGetMemoryInfo_0.enable =  1;  /*enable the FUB*/
			helper->fub.xmlGetMemoryInfo_0.ident =  helper->para.ident;  /*write the ident*/
			
			xmlGetMemoryInfo(&helper->fub.xmlGetMemoryInfo_0);  /*Call the FUB*/
									
			if( helper->fub.xmlGetMemoryInfo_0.status == 0){  /*check if getting the INFO was successful*/
				
				//memcpy(Dane, helper->fub.xmlGetMemoryInfo_0.pXmlData, helper->fub.xmlGetMemoryInfo_0.xmlDataSize);  /*Copy the data on a buffer*/	

				parametersDump_Step = 1;
				//WriteXML(helper->fub.xmlGetMemoryInfo_0.pXmlData , helper->fub.xmlGetMemoryInfo_0.xmlDataSize);
				helper->state =  WAIT;  

			}
			else if( helper->fub.xmlGetMemoryInfo_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = GET_MEMINFO;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;
		
		case CLOSE_WRITER:  /*state to close the memory writer*/
			
			helper->fub.xmlCloseMemoryWriter_0.enable =  1;  /*enable the FUB*/	
			helper->fub.xmlCloseMemoryWriter_0.ident =  helper->para.ident;  /*write the ident*/
			
			xmlCloseMemoryWriter(&helper->fub.xmlCloseMemoryWriter_0);  /*Call the FUB*/
									
			if( helper->fub.xmlCloseMemoryWriter_0.status == 0){  /*check if closing the writer was successful*/	
				helper->pMemory = 0;
				helper->MemorySize = 0;
				helper->para.ident = 0;
				helper->state =  WAIT;  /*jump to the state WAIT*/
			}
			else if( helper->fub.xmlCloseMemoryWriter_0.status != ERR_FUB_BUSY )  /*check if the FUB is still busy*/
			{
				helper->errorState = CLOSE_WRITER;
				helper->state = ERROR;  /*go to the ERROR state*/

			}
			
		break;


		////////////////////////////////////

		case CREATE_READER:
			
			helper->fub.xmlCreateMemoryReader_0.enable = 1;  //enable the FUB  
			helper->fub.xmlCreateMemoryReader_0.pXmlMemory = helper->pMemory;  //give the pointer to the memory area  
			helper->fub.xmlCreateMemoryReader_0.memorySize = helper->MemorySize;  //give the size of of the memory area  
			xmlCreateMemoryReader(&helper->fub.xmlCreateMemoryReader_0);  //call the FUB  
									
			if(helper->fub.xmlCreateMemoryReader_0.status == 0){ //check if the Create Memory Reader was successful  
			
				helper->state = WAIT;  //jump to the state WAIT  				
				memset(&helper->para_read, 0, sizeof(helper->para_read));  //reset the information read until now  
				helper->para_read.ident = helper->fub.xmlCreateMemoryReader_0.ident;  //copy the ident on a variable 
			}
			else if(helper->fub.xmlCreateMemoryReader_0.status != ERR_FUB_BUSY){ //check if the FUB is still busy  

				helper->errorState = CREATE_READER;
				helper->state = ERROR;  //go to the ERROR state  

			}
			
		break;	
			
		case READ_NODE:
			
			helper->fub.xmlReadNextNode_0.enable = 1; //enable the FUB  
			helper->fub.xmlReadNextNode_0.ident = helper->para_read.ident;  //get the ident  
			helper->fub.xmlReadNextNode_0.forceRead = 0;  //Force the reading of the next Node  
			helper->fub.xmlReadNextNode_0.skipSubtree = 0;  //give command to skip Subtree or not  
			helper->fub.xmlReadNextNode_0.pName = helper->para_read.nodeName;  //address where the name should be stored  
   			helper->fub.xmlReadNextNode_0.pValue = helper->para_read.nodeValue;  //address where the value should be stored  
			helper->fub.xmlReadNextNode_0.nameSize = sizeof(helper->para_read.nodeName);  //size of the name  
   			helper->fub.xmlReadNextNode_0.valueSize = sizeof(helper->para_read.nodeValue);  //size of the value  
			xmlReadNextNode(&helper->fub.xmlReadNextNode_0);  //call the FUB  
									
			if(helper->fub.xmlReadNextNode_0.status == 0 ){ //check if reading the node was successful  
			
				helper->state = WAIT;  //jump to the state WAIT  
				helper->para_read.nodeType = helper->fub.xmlReadNextNode_0.nodeType;  //get the node Type  
				helper->para_read.nodeDepth = helper->fub.xmlReadNextNode_0.depth;  //get the depth of the node  
				helper->para_read.empty = helper->fub.xmlReadNextNode_0.isEmpty;  //get the Flag if the node is empty  
				helper->para_read.attributeCount = helper->fub.xmlReadNextNode_0.attributeCount;  //get the number of attributes  
			}	
			else if( helper->fub.xmlReadNextNode_0.status = xmlERR_NO_FURTHER_XML_NODE ){  //check if all elements were read  
			
				helper->state = WAIT;  //jump to the state WAIT  
			}
			else if(helper->fub.xmlReadNextNode_0.status != ERR_FUB_BUSY){  //check if the FUB is still busy  

				helper->errorState = READ_NODE;
				helper->state = ERROR;  //go to the ERROR state  

			}
		

		break;
			
		case READ_ATTR:
			
			helper->fub.xmlReadAttributeNr_0.enable = 1;  //enable the FUB  
			helper->fub.xmlReadAttributeNr_0.ident = helper->para_read.ident;  //get the ident  
			helper->fub.xmlReadAttributeNr_0.index = helper->para_read.attributeIndex;  //get the index to be read  
			helper->fub.xmlReadAttributeNr_0.pName = helper->para_read.attributeName;  //address where the name should be stored  
   			helper->fub.xmlReadAttributeNr_0.pValue = helper->para_read.attributeValue;  //address where the value should be stored  
   			helper->fub.xmlReadAttributeNr_0.nameSize = sizeof(helper->para_read.attributeName);  //size of the name  
   			helper->fub.xmlReadAttributeNr_0.valueSize = sizeof(helper->para_read.attributeValue);  //size of the value  			
			xmlReadAttributeNr(&helper->fub.xmlReadAttributeNr_0);  //call the FUB  
									
			if(helper->fub.xmlReadAttributeNr_0.status == 0){ //check if reading the attribute was successful  
			
				helper->state = WAIT;  //jump to the state WAIT  
			
				/*
				if(helper->Auto = 1){ //check if auto is enabled  
				
					helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[helper->para_read.attributeIndex].name = helper->para_read.attributeName;  //copy into the structure  
					helper->para_read.xmlElements[helper->para_read.elementIndex].attributes[helper->para_read.attributeIndex].value = helper->para_read.attributeValue;  //copy into the structure  
					helper->para_read.attributeIndex = helper->para.attributeIndex + 1;  //increase index of the attribute to be read  
					
				}
				*/
			}
			else if(helper->fub.xmlReadAttributeNr_0.status != ERR_FUB_BUSY){  //check if the FUB is still busy  
				
				helper->errorState = READ_ATTR;
				helper->state = ERROR;	//go to the ERROR state  

			}

		break;	
				
			
		case CLOSE_READER:
			
			helper->fub.xmlCloseMemoryReader_0.enable = 1;  //enable the FUB  
			helper->fub.xmlCloseMemoryReader_0.ident = helper->para_read.ident;  //copy the ident  
			xmlCloseMemoryReader(&helper->fub.xmlCloseMemoryReader_0);  //call the FUB  
									
			if(helper->fub.xmlCloseMemoryReader_0.status == 0){  //check if closing the Memory Reader was successful  
			
				helper->state = WAIT;  			//jump to the state WAIT  
				helper->para_read.ident = 0;  	//reset the ident  

			}
			else if(helper->fub.xmlCloseMemoryReader_0.status != ERR_FUB_BUSY){  //check if the FUB is still busy  
			
				helper->errorState = CLOSE_READER;
				helper->state = ERROR;	//go to the ERROR state  

			}
			
		break;	
		
			
	
	}
	
}


UINT FindParamIndex(Cfg_typ *cfg , char *p){
UINT i;

	for(i=0 ; i < PARAM_NUM_MAX ; i++){

		if(!strcmp(p , cfg->ScaledVar[i].enumTXT)){

			return i+1;


		}


	}

	return 0;

}

UINT FindParamEnumIndex(Cfg_typ *cfg , char *p){
UINT i;

	for(i=0 ; i < PARAM_ENUM_NUM_MAX ; i++){

		if(strstr(p , cfg->EnumVar[i].enumTXT)){

			return i+1;


		}


	}

	return 0;

}

UINT FindParamIntIndex(Cfg_typ *cfg , char *p){
UINT i;

	for(i=0 ; i < PARAM_INT_NUM_MAX ; i++){

		if(!strcmp(p , cfg->IntVar[i].enumTXT)){

			return i+1;


		}


	}

	return 0;

}




