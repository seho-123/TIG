
#ifndef _DRILLMENULIB_C
#define _DRILLMENULIB_C


#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
#include <AsDefault.h>
#endif


void DrillDwnMenu_MGR(DRILL_DWN_MGR_obj *menu);

void DrillDwnMenu_INIT(DRILL_DWN_MGR_obj *menu , MASTER_typ *pMaster ,	LCD_NODE_obj *pNodes , Cfg_typ *cfg , CurCfg_typ *curCfg , HMI_typ *pPend_HMI);

void ParseType(DRILL_DWN_MGR_obj *menu , UINT  indx);
USINT ReDrawLine(DRILL_DWN_MGR_obj *menu , UINT indx);
USINT ReDrawPage(DRILL_DWN_MGR_obj *menu);
char* GetScaledValue(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var);
void SetScaledValue(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var , UINT indx);
//void SetScaledValueInEditor(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var );
void SetScaledValueInEditor(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var , UINT segm);
void GetScaledValueFromEditor(DRILL_DWN_MGR_obj* menu );

void SetScaledValueInSingleEditor(SINGLE_EDITOR_MGR_obj* menu , Scaled_Var_obj* var , UINT segm );
void GetScaledValueFromSingleEditor(SINGLE_EDITOR_MGR_obj* menu );


void HandleTicks_ENUM(DRILL_DWN_MGR_obj* menu );
void HandleTicks_SCALED(DRILL_DWN_MGR_obj* menu );

void SINGLE_EDITOR_MGR(SINGLE_EDITOR_MGR_obj *menu);
void SINGLE_EDITOR_INIT(SINGLE_EDITOR_MGR_obj *menu , MASTER_typ *pMaster , Cfg_typ *cfg , CurCfg_typ *curCfg);

float GetParamValue(MASTER_typ *m , Scaled_Var_obj* var , UINT segm );

void ReadScaledVar(READ_VAR_typ* menu , Scaled_Var_obj* var , UINT segm );
void ReadEnumVar(READ_ENUM_VAR_typ* menu , UINT enum_indx , UINT segm);




void SINGLE_ENUM_EDITOR_MGR(SINGLE_ENUM_EDITOR *menu){
UINT i;

	if(menu->RefreshEditor){

		if(SingleEnumParamEditorHMI.step == 0){
			menu->RefreshEditor = 0;
			SingleEnumParamEditorHMI.step = 1;	

		}

	}

	

	switch(menu->step){


		case 0:

		break;


		case 1:

			menu->EnumList_Indx_Max = Cfg.Param_ENUM[menu->ParamEnum_Indx].max;

			memset(menu->EnumList , 0 , sizeof(menu->EnumList));

			if(menu->EnumList_Indx_Max < 16){

				for(i = 0 ; i <= menu->EnumList_Indx_Max ; i++){


					strcpy(menu->EnumList[i] , Cfg.TXT_Resources[Cfg.EnumVar[menu->ParamEnum_Indx].TxtIndx + i]);
					 

				}

				
				menu->EnumList_Indx = (UINT)CurCfg.Param_ENUM[menu->ParamEnum_Indx].val[0];
			}
			
			strcpy(menu->ParamName , Cfg.EnumVar[menu->ParamEnum_Indx].name);

			menu->step++;

		break;

		case 2:

			if(menu->Enum_Editor_vis == 0){ //VISIBLE

				if(menu->Cancel_Return_Cmd)
					menu->step++;

				else if(menu->Save_Exit_Cmd){

					CurCfg.Param_ENUM[menu->ParamEnum_Indx].val[0] = menu->EnumList_Indx;
					menu->step++;			

				}
				else if(menu->LoadDefaultBtn){
					menu->LoadDefaultBtn = 0;
					CurCfg.Param_ENUM[menu->ParamEnum_Indx].val[0] = Cfg.Param_ENUM[menu->ParamEnum_Indx].defaultVal;
					menu->EnumList_Indx = (UINT)CurCfg.Param_ENUM[menu->ParamEnum_Indx].val[0];

				}
				else if(menu->TickPlus){
					menu->TickPlus = 0;

					if(menu->EnumList_Indx > 0)
						menu->EnumList_Indx--;	
								

				}else if(menu->TickMinus){
					menu->TickMinus = 0;

					if(menu->EnumList_Indx < Cfg.Param_ENUM[menu->ParamEnum_Indx].max)
						menu->EnumList_Indx++;							

				}

				


			}else
				menu->step++;
				

		break;	


		default:

			menu->Cancel_Return_Cmd = 0;
			menu->Save_Exit_Cmd = 0;
			menu->LoadDefaultBtn = 0;
			menu->TickPlus = 0;
			menu->TickMinus = 0;		
			

			menu->Enum_Editor_vis = 1; // INVISIBLE

			menu->step = 0;
			menu->RefreshEditor = 0;

		break;
		


	}
/*
	menu->ParamEnum_Indx

	 i = Cfg.EnumVar[enum_indx].TxtIndx + CurCfg.Param_ENUM[enum_indx].val[segm];

if(i < TXT_RESOURCES_MAX){

strcpy(menu->EnumTxt , Cfg.TXT_Resources[i]);

strcpy(menu->ParamName , Cfg.EnumVar[enum_indx].name);
*/

};

void SINGLE_EDITOR_MGR(SINGLE_EDITOR_MGR_obj *menu){
UINT i , k ;
UINT indx;
USINT logTicks = 0;
LCD_NODE_obj *newNode;

	if(!menu)
		return ;

	if(!menu->editPageData)
		return ;

	if(!menu->cfg)
		return;

	
	 if(menu->Cancel_Return_Cmd){		
		menu->Cancel_Return_Cmd = 0;
		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		
		//menu->RefreshPage = 1;		
	}

	else if(menu->Save_Exit_Cmd){
		menu->Save_Exit_Cmd = 0;
		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[0] = menu->editPageData->MainVal_Disp;		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[1] = menu->editPageData->MainVal_Disp;

		GetScaledValueFromSingleEditor(menu);

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		//menu->RefreshPage = 1;

		////////////////
	}
	else if(menu->Enum_Cancel_Return_Cmd){	
		menu->Enum_Cancel_Return_Cmd = 0;

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		//menu->RefreshPage = 1;		

		menu->Ticks_vis = 1;
	}
	else if(menu->Enum_Save_Exit_Cmd){
		menu->Enum_Save_Exit_Cmd = 0;
		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[0] = menu->editPageData->MainVal_Disp;		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[1] = menu->editPageData->MainVal_Disp;

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		//menu->RefreshPage = 1;		

		menu->Ticks_vis = 1;
		////////////////
	}
	else if(menu->TickPlus || menu->TickMinus){

			if (menu->Editor_vis == 0){
				//HandleTicks_SCALED(menu);
				
				if(menu->TickPlus){ 		
							
						//_curCfg->Param[i].val[k] += _curCfg->Param[i].incr;	
						menu->editPageData->MainVal_Disp += menu->editPageData->Incr_Disp;
							
				}
				else if(menu->TickMinus){
					
					//_curCfg->Param[i].val[k] -= _curCfg->Param[i].incr; 
					menu->editPageData->MainVal_Disp -= menu->editPageData->Incr_Disp;
					
				}


				if(menu->editPageData->MainVal_Disp > menu->editPageData->MaxVal_Cfg)
						menu->editPageData->MainVal_Disp = menu->editPageData->MaxVal_Cfg;

				else if(menu->editPageData->MainVal_Disp < menu->editPageData->MinVal_Cfg)
					menu->editPageData->MainVal_Disp = menu->editPageData->MinVal_Cfg;	

			}
			
		menu->TickPlus = 0;
		menu->TickMinus = 0;		
	}

	
	////////////////////////////////////////////////////////////
	if(menu->RefreshEditor
		|| menu->editPageData->LoadDefault_Btn){
		
		menu->RefreshEditor = 0;

		if(menu->editPageData->LoadDefault_Btn){
			menu->editPageData->LoadDefault_Btn = 0;

			menu->editPageData->MainVal_Disp = menu->editPageData->defaultVal;
			menu->editPageData->MainVal_Disp = menu->editPageData->MainVal_Disp * menu->editPageData->pScale->gain \
				+ menu->editPageData->pScale->offset;
			

		}else{
			//menu->KK_test[0] += 1;
			SetScaledValueInSingleEditor(menu , menu->editPageData->var , menu->editPageData->Segm_Indx);
		}
	}
		
	//////////////////////////////////////////

	


	
	return;
}


void DrillDwnMenu_MGR(DRILL_DWN_MGR_obj *menu){
UINT i , k ;
UINT indx;
USINT logTicks = 0;
LCD_NODE_obj *newNode;

	if(!menu)
		return ;

	if(!menu->editPageData)
		return ;

	if(menu->editPageData->pMsgTmr)
		TON(menu->editPageData->pMsgTmr);

	
	/////////////////////////////////	

	if(menu->ShowPopUpMsg
		|| menu->PopUpMsg_Event){		

		if(menu->PendingFiredEvent
			|| menu->PopUpMsg_Event){

			if(menu->editPageData->Confrim_Btn == 10){ // OK
				menu->editPageData->Confrim_Btn = 0;

				if(menu->PendingFiredEvent){
					menu->FiredEvent = menu->PendingFiredEvent;
					menu->PendingFiredEvent = 0;
				}
				else
					menu->FiredEvent = 100; // EVNT_POPUP_OK;
					
				
				menu->ShowPopUpMsg = 0;
			}
			else if(menu->editPageData->Confrim_Btn == 1){ // Cancel
				menu->editPageData->Confrim_Btn = 0;

				if(menu->PendingFiredEvent){
					menu->PendingFiredEvent = 0;
					menu->PopUpMsg_Event = 0;
				}
				else
					menu->FiredEvent = 101; //EVNT_POPUP_CANCEL; // EVNT_POPUP_OK;

				
				menu->ShowPopUpMsg = 0;
			}
		}
		//else
			//menu->ShowPopUpMsg = 0;

	}

	////////////////////////////////
	
	/*if(menu->Refresh_Edit_Window_Cmd){

		k = menu->editPageData->Segm_Indx;
		
		SetScaledValueInEditor(menu , menu->editPageData->var , k);

		for(i = 0 ; i < menu->lineNum ; i++)
			menu->TxtLineHighLight[i] = 0;

		menu->Ticks_vis = 1;
	
	}
	else */if(menu->Cancel_Return_Cmd){		

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		menu->RefreshPage = 1;

		for(i = 0 ; i < menu->lineNum ; i++)
			menu->TxtLineHighLight[i] = 0;

		menu->Ticks_vis = 1;
	}

	else if(menu->Save_Exit_Cmd){		
		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[0] = menu->editPageData->MainVal_Disp;		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[1] = menu->editPageData->MainVal_Disp;

		GetScaledValueFromEditor(menu);

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		menu->RefreshPage = 1;

		////////////
		for(i = 0 ; i < menu->lineNum ; i++)
			menu->TxtLineHighLight[i] = 0;

		menu->Ticks_vis = 1;
		////////////////
	}
	else if(menu->Enum_Cancel_Return_Cmd){		

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		menu->RefreshPage = 1;

		for(i = 0 ; i < menu->lineNum ; i++)
			menu->TxtLineHighLight[i] = 0;

		menu->Ticks_vis = 1;
	}
	else if(menu->Enum_Save_Exit_Cmd){		
		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[0] = menu->editPageData->MainVal_Disp;		
		//menu->pMaster->pCurCfg->Param[menu->editPageData->Param_Indx].val[1] = menu->editPageData->MainVal_Disp;

		menu->Enum_Editor_vis = 1; //INVISIBLE;
		menu->Editor_vis = 1; //INVISIBLE;
		menu->RefreshPage = 1;

		////////////
		for(i = 0 ; i < menu->lineNum ; i++)
			menu->TxtLineHighLight[i] = 0;

		menu->Ticks_vis = 1;
		////////////////
	}

	
	////////////////////////////////////////////////////////////
	if(menu->RefreshEditor
		|| menu->editPageData->LoadDefault_Btn){
		menu->RefreshEditor = 0;

		if(menu->editPageData->LoadDefault_Btn){
			menu->editPageData->LoadDefault_Btn = 0;

			menu->editPageData->MainVal_Disp = menu->editPageData->defaultVal;
			menu->editPageData->MainVal_Disp = menu->editPageData->MainVal_Disp * menu->editPageData->pScale->gain \
				+ menu->editPageData->pScale->offset;
			

		}else
			SetScaledValueInEditor(menu , menu->editPageData->var , menu->editPageData->Segm_Indx);
			
	}
	else if(menu->RefreshPage){


		if(menu->TickPlus || menu->TickMinus){

			if(menu->nodes[menu->curNodeID].var.Type & T_ENUM)
				HandleTicks_ENUM(menu);

			else if((menu->nodes[menu->curNodeID].var.Type & T_SCALED)
				&& (menu->Editor_vis == 0))
				HandleTicks_SCALED(menu);
		}
		else if(menu->PageHeadClick){
			menu->PageHeadClick = 0;

			if(menu->curHeadID == P_LOGS){

				/*
				menu->curTopNodeID = P_LOGS;
				menu->curNodeID = menu->curTopNodeID;
				menu->curHeadID = menu->nodes[menu->curTopNodeID].HeadID;
				menu->RefreshPage = 1;
				*/

			}else{

				if((menu->curHeadID == menu->curNodeID)
					&& (menu->nodes[menu->curHeadID].Type & N_JUMP)){

					if(menu->nodes[menu->curHeadID].pJumpNode){

		
						newNode = (LCD_NODE_obj*) menu->nodes[menu->curHeadID].pJumpNode;
					
						menu->curHeadID = newNode->HeadID;
						menu->curTopNodeID = newNode->ID;
						menu->curNodeID = newNode->ID;
						
					}

				}else{	
			
					menu->curTopNodeID = menu->curHeadID;
					menu->curNodeID = menu->curHeadID;

				}

					////////////
				for(i = 0 ; i < menu->lineNum ; i++)
					menu->TxtLineHighLight[i] = 0;

				menu->Ticks_vis = 1;

			}
			////////////////				

		}
		else if(menu->PrevClick){
			menu->PrevClick = 0;


			if(menu->curHeadID == P_LOGS){

				menu->TickMinus = 1;
				menu->LCDEntr = 1;
				logTicks = 1;
			}
			else {
				
				indx = (menu->curTopNodeID > 0 )? (menu->curTopNodeID-1):0;
				if(menu->nodes[indx].HeadID == menu->curHeadID)
					menu->curTopNodeID = indx;

				menu->curNodeID = menu->curTopNodeID;				

			}

			////////////
			for(i = 0 ; i < menu->lineNum ; i++)
				menu->TxtLineHighLight[i] = 0;

			menu->Ticks_vis = 1;
			////////////////	
				

		}
		else if(menu->NextClick){
			menu->NextClick = 0;

			if(menu->curHeadID == P_LOGS){

				menu->TickPlus = 1;
				menu->LCDEntr = 1;
				logTicks = 1;
			}
			else {

				indx = menu->curTopNodeID + 1 ;

				if(menu->nodes[menu->curTopNodeID + menu->curlineNum].HeadID == menu->curHeadID)
					menu->curTopNodeID = indx;

				menu->curNodeID = menu->curTopNodeID;

			}

			////////////
			for(i = 0 ; i < menu->lineNum ; i++)
				menu->TxtLineHighLight[i] = 0;

			menu->Ticks_vis = 1;
			////////////////	

		}
		else{

	
			for(i = 0 ; i < menu->lineNum ; i++){

				if(menu->LineClick[i]){
					menu->LineClick[i] = 0;
					ParseType(menu , i);
					continue;
				}		
				

			}

		}

		
		////////////////////////////////////////

		//if(!menu->EditPageReq_Cmd){
			
			if(!ReDrawPage(menu))
				menu->RefreshPage = 0;

		//}
		
		////////////////////////	
		
		menu->Save_Exit_Cmd = 0;
		menu->Cancel_Return_Cmd = 0;
	}			

	if(!logTicks){
		if(menu->TickPlus || menu->TickMinus){
			menu->TickPlus = 0;
			menu->TickMinus = 0;
			menu->RefreshPage = 1;
		}
	}
	//////////  Auto Refresh
	if(menu->RefreshPage){
		menu->RefreshCntr = 10;
	}

	if(menu->RefreshCntr == 0){
		menu->RefreshCntr = 10;
		menu->RefreshPage = 1;
	}
	else
		menu->RefreshCntr--;

	//////////////////////////////////////////

	if(menu->PopUpMsg_Event){
		if(menu->editPageData->pMsgTmr->Q)
			menu->editPageData->Confrim_Btn_vis = 0; //VISIBLE
		else
			menu->editPageData->Confrim_Btn_vis = 1; //INVISIBLE
	}
		
	if(menu->editPageData->pMsgTmr)
		menu->editPageData->MsgBox_vis = (!menu->editPageData->pMsgTmr->Q || menu->ShowPopUpMsg || menu->PopUpMsg_Event)? 0:1;

	// Clear Pop Up Message
	if(menu->editPageData->pMsgTmr->Q)
		menu->ClearMsgBox_vis_Cmd = 0;

	if(menu->ClearMsgBox_vis_Cmd)
		menu->editPageData->MsgBox_vis = 1; // INVISIBLE


	/// Message from Second HMI like T Handle Pendant

	if(menu->pT_Pend_HMI){

		if(menu->pT_Pend_HMI->pMessage[0]
			&& menu->pT_Pend_HMI->pMessage[1]
			&& menu->pT_Pend_HMI->pMessage[2]
			&& menu->pT_Pend_HMI->pMessage[3]){

			if(!menu->pT_Pend_HMI->MsgTmr.Q){
				
				strcpy(menu->Message[0], menu->pT_Pend_HMI->pMessage[0]);
				strcpy(menu->Message[1], menu->pT_Pend_HMI->pMessage[1]);
				strcpy(menu->Message[2], menu->pT_Pend_HMI->pMessage[2]);
				strcpy(menu->Message[3], menu->pT_Pend_HMI->pMessage[3]);
				
				if(menu->editPageData->pMsgTmr)
					menu->editPageData->MsgBox_vis = 0; // VISIBLE
				
			}
		}
	}


	
	return;
}


void SINGLE_EDITOR_INIT(SINGLE_EDITOR_MGR_obj *menu , MASTER_typ *pMaster , Cfg_typ *cfg , CurCfg_typ *curCfg){
UINT i;
Units_obj *unit;


	menu->pMaster = pMaster;
	menu->cfg = cfg;
	menu->curCfg = curCfg;

	if(!menu->editPageData
		|| !menu->pMaster
		|| !menu->cfg
		|| !menu->curCfg)
		return;

	
	menu->Editor_vis = 1; //INVISIBLE;
	menu->Enum_Editor_vis = 1; //INVISIBLE;
	//menu->Ticks_vis = 1; //INVISIBLE;
	//menu->ShowPopUpMsg = 0;
	menu->editPageData->Confrim_Btn_vis = 1; //INVISIBLE;
	menu->editPageData->MsgBox_vis = 1; //INVISIBLE;

	
	//menu->TxtLineHighLight[1] = 1;
	//menu->RefreshPage = 1;	
	
	unit = (CurCalib_typ*) pMaster->pUnits;
	
	if(!unit) 
		return -1;


	/************************************************/
	/**********   Units *******************************/

	unit[unitIU].unitType = unitIU;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;

	/************/
	unit[unitLength].unitType=unitLength;
	unit[unitLength].unitScale[UNIT_IU].gain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].offset = 0.0;
	unit[unitLength].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_IU].txt , "");

	unit[unitLength].unitScale[UNIT_mm].gain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_mm].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_mm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_mm].txt , "mm");

	
	unit[unitLength].unitScale[UNIT_inch].gain = mm2inch;
	unit[unitLength].unitScale[UNIT_inch].offset = 0.0;
	unit[unitLength].unitScale[UNIT_inch].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_inch].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_inch].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_inch].txt , "in");

	
	unit[unitLength].unitScale[UNIT_cm].gain = 0.1;
	unit[unitLength].unitScale[UNIT_cm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_cm].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_cm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_cm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_cm].txt , "cm");


	/************/
	
	unit[unitSpeed].unitType=unitSpeed;
	unit[unitSpeed].unitScale[UNIT_IU].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_IU].txt , "");
		
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_mm_per_sec].txt , "mm/s");

	unit[unitSpeed].unitScale[UNIT_inch_per_min].gain = mm2inch * 60;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].extraDigits = 1;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_min].txt , "ipm");

	
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].gain = 1.0/25.4;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].extraDigits = 2;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_sec].txt , "ips");


	/************/
	
	unit[unitAcc].unitType=unitAcc;
	unit[unitAcc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_IU].txt , "");
		
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_mm_per_sec2].txt , "mm/s2");

	unit[unitAcc].unitScale[UNIT_inch_per_sec2].gain = 0.03937;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_inch_per_sec2].txt , "in/s2");

	
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].gain = 0.1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_cm_per_sec2].txt , "cm/s2");

	
	/************/
		
	unit[unitRot].unitType=unitRot;
	unit[unitRot].unitScale[UNIT_IU].gain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].offset = 0.0;
	unit[unitRot].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_IU].txt , "");
		
	unit[unitRot].unitScale[UNIT_RPS].gain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPS].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_RPS].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPS].txt , "rps");

	unit[unitRot].unitScale[UNIT_RPM].gain = 0.01666666;
	unit[unitRot].unitScale[UNIT_RPM].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPM].extraDigits = 1;
	unit[unitRot].unitScale[UNIT_RPM].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPM].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPM].txt , "rpm");

	unit[unitRot].unitScale[UNIT_RPh].gain = 2.777e-8;
	unit[unitRot].unitScale[UNIT_RPh].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPh].extraDigits = 2;
	unit[unitRot].unitScale[UNIT_RPh].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPh].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPh].txt , "rph");

	/************/
		
	unit[unitTime].unitType=unitTime;
	unit[unitTime].unitScale[UNIT_IU].gain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].offset = 0.0;
	unit[unitTime].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_IU].txt , "");
		
	unit[unitTime].unitScale[UNIT_sec].gain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].offset = 0.0;
	unit[unitTime].unitScale[UNIT_sec].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_sec].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_sec].txt , "sec");

	unit[unitTime].unitScale[UNIT_min].gain = 0.0166666;
	unit[unitTime].unitScale[UNIT_min].offset = 0.0;
	unit[unitTime].unitScale[UNIT_min].extraDigits = 2;
	unit[unitTime].unitScale[UNIT_min].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_min].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_min].txt , "min");

	unit[unitTime].unitScale[UNIT_h].gain = 2.777777e-4;
	unit[unitTime].unitScale[UNIT_h].offset = 0.0;
	unit[unitTime].unitScale[UNIT_h].extraDigits = 4;
	unit[unitTime].unitScale[UNIT_h].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_h].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_h].txt , "h");


	/************/
		
	unit[unitAmp].unitType=unitAmp;
	unit[unitAmp].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_IU].txt , "");
		
	unit[unitAmp].unitScale[UNIT_Amp].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_Amp].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_Amp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_Amp].txt , "A");

	unit[unitAmp].unitScale[UNIT_kAmp].gain = 0.001;
	unit[unitAmp].unitScale[UNIT_kAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_kAmp].extraDigits = 3;
	unit[unitAmp].unitScale[UNIT_kAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_kAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_kAmp].txt , "kA");

	unit[unitAmp].unitScale[UNIT_mAmp].gain = 1000.0;
	unit[unitAmp].unitScale[UNIT_mAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_mAmp].extraDigits = -3;
	unit[unitAmp].unitScale[UNIT_mAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_mAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_mAmp].txt , "mA");

	/************/
		
	unit[unitVolt].unitType=unitVolt;
	unit[unitVolt].unitScale[UNIT_IU].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_IU].txt , "");
		
	unit[unitVolt].unitScale[UNIT_Volt].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_Volt].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_Volt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_Volt].txt , "V");

	unit[unitVolt].unitScale[UNIT_kVolt].gain = 0.001;
	unit[unitVolt].unitScale[UNIT_kVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_kVolt].extraDigits = 3;
	unit[unitVolt].unitScale[UNIT_kVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_kVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_kVolt].txt , "kV");


	unit[unitVolt].unitScale[UNIT_mVolt].gain = 1000.0;
	unit[unitVolt].unitScale[UNIT_mVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_mVolt].extraDigits = -3;
	unit[unitVolt].unitScale[UNIT_mVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_mVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_mVolt].txt , "mV");


	
	/************/
		
	unit[unitFreq].unitType=unitFreq;
	unit[unitFreq].unitScale[UNIT_IU].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_IU].txt , "");
		
	unit[unitFreq].unitScale[UNIT_Hz].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_Hz].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_Hz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_Hz].txt , "Hz");

	unit[unitFreq].unitScale[UNIT_kHz].gain = 0.001;
	unit[unitFreq].unitScale[UNIT_kHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_kHz].extraDigits = 3;
	unit[unitFreq].unitScale[UNIT_kHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_kHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_kHz].txt , "kHz");


	unit[unitFreq].unitScale[UNIT_mHz].gain = 0.000001;
	unit[unitFreq].unitScale[UNIT_mHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_mHz].extraDigits = 6;
	unit[unitFreq].unitScale[UNIT_mHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_mHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_mHz].txt , "mHz");


	
	/************/
			
	unit[unitPerc].unitType=unitPerc;
	unit[unitPerc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_IU].txt , "");
		
	unit[unitPerc].unitScale[UNIT_PERCENT].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT].txt , "%");

	unit[unitPerc].unitScale[UNIT_PERCENT+1].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+1].txt , "%");


	unit[unitPerc].unitScale[UNIT_PERCENT+2].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+2].txt , "%");


	/************/
			
	unit[unitAngle].unitType=unitAngle;
	unit[unitAngle].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_IU].txt , "");
		
	unit[unitAngle].unitScale[UNIT_DEG].gain = 360.0;
	unit[unitAngle].unitScale[UNIT_DEG].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_DEG].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_DEG].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_DEG].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_DEG].txt , " deg");

	unit[unitAngle].unitScale[UNIT_RAD].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD].txt , " rad");

	unit[unitAngle].unitScale[UNIT_RAD+1].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD+1].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD+1].txt , " rad");

}


void DrillDwnMenu_INIT(DRILL_DWN_MGR_obj *menu , MASTER_typ *pMaster ,	LCD_NODE_obj *pNodes , Cfg_typ *cfg , CurCfg_typ *curCfg , HMI_typ *pPend_HMI){
UINT i;
Units_obj *unit;


	menu->pMaster = pMaster;
	menu->nodes = pNodes;	
	menu->cfg = cfg;
	menu->curCfg = curCfg;

	if(!menu->editPageData
		|| !menu->pMaster
		|| !menu->nodes
		|| !menu->cfg
		|| !menu->curCfg)
		return;

	menu->pT_Pend_HMI = pPend_HMI;	

	menu->Editor_vis = 1; //INVISIBLE;
	menu->Enum_Editor_vis = 1; //INVISIBLE;
	menu->Ticks_vis = 1; //INVISIBLE;
	menu->ShowPopUpMsg = 0;
	menu->editPageData->Confrim_Btn_vis = 1; //INVISIBLE;
	menu->editPageData->MsgBox_vis = 1; //INVISIBLE;

	menu->curTopNodeID = 100;
	menu->curHeadID = 100;

	menu->lineNum = DRILLMENU_LINE_NUM -1;
	menu->curlineNum = menu->lineNum;

	menu->editPageData->pMsgTmr = &pMaster->Hmi[0].MsgTmr;
	menu->editPageData->pMsgTmr->PT = 2000;

	
	for( i = 0 ; i < DRILLMENU_LINE_NUM ; i++){
		menu->TxtLineHighLight[i] = 0;
		
	
		xsprintf(&menu->TxtLines[i] , "Linia %d", i);

	}

	
	//menu->TxtLineHighLight[1] = 1;
	menu->RefreshPage = 1;

	/////////////
	if(menu->editPageData->pMsgTmr)
		menu->editPageData->pMsgTmr->IN = 1;

	////////////

	
	unit = (CurCalib_typ*) pMaster->pUnits;
	
	if(!unit) 
		return -1;


	/************************************************/
	/**********   Units *******************************/

	unit[unitIU].unitType = unitIU;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+1].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+2].incrGain = 1.0;

	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].gain = 1.0;
	unit[unitIU].unitScale[UNIT_IU+3].incrGain = 1.0;

	/************/
	unit[unitLength].unitType=unitLength;
	unit[unitLength].unitScale[UNIT_IU].gain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].offset = 0.0;
	unit[unitLength].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_IU].txt , "");

	unit[unitLength].unitScale[UNIT_mm].gain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_mm].extraDigits = 0;
	unit[unitLength].unitScale[UNIT_mm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_mm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_mm].txt , "mm");

	
	unit[unitLength].unitScale[UNIT_inch].gain = mm2inch;
	unit[unitLength].unitScale[UNIT_inch].offset = 0.0;
	unit[unitLength].unitScale[UNIT_inch].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_inch].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_inch].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_inch].txt , "in");

	
	unit[unitLength].unitScale[UNIT_cm].gain = 0.1;
	unit[unitLength].unitScale[UNIT_cm].offset = 0.0;
	unit[unitLength].unitScale[UNIT_cm].extraDigits = 1;
	unit[unitLength].unitScale[UNIT_cm].incrGain = 1.0;
	unit[unitLength].unitScale[UNIT_cm].incrOffset = 0.0;
	strcpy(unit[unitLength].unitScale[UNIT_cm].txt , "cm");


	/************/
	
	unit[unitSpeed].unitType=unitSpeed;
	unit[unitSpeed].unitScale[UNIT_IU].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_IU].txt , "");
		
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].gain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].extraDigits = 0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_mm_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_mm_per_sec].txt , "mm/s");

	unit[unitSpeed].unitScale[UNIT_inch_per_min].gain = mm2inch * 60;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].extraDigits = 1;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_min].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_min].txt , "ipm");

	
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].gain = 1.0/25.4;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].offset = 0.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].extraDigits = 2;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrGain = 1.0;
	unit[unitSpeed].unitScale[UNIT_inch_per_sec].incrOffset = 0.0;
	strcpy(unit[unitSpeed].unitScale[UNIT_inch_per_sec].txt , "ips");


	/************/
	
	unit[unitAcc].unitType=unitAcc;
	unit[unitAcc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_IU].txt , "");
		
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].gain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].extraDigits = 0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_mm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_mm_per_sec2].txt , "mm/s2");

	unit[unitAcc].unitScale[UNIT_inch_per_sec2].gain = 0.03937;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_inch_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_inch_per_sec2].txt , "in/s2");

	
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].gain = 0.1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].offset = 0.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].extraDigits = 1;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrGain = 1.0;
	unit[unitAcc].unitScale[UNIT_cm_per_sec2].incrOffset = 0.0;
	strcpy(unit[unitAcc].unitScale[UNIT_cm_per_sec2].txt , "cm/s2");

	
	/************/
		
	unit[unitRot].unitType=unitRot;
	unit[unitRot].unitScale[UNIT_IU].gain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].offset = 0.0;
	unit[unitRot].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_IU].txt , "");
		
	unit[unitRot].unitScale[UNIT_RPS].gain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPS].extraDigits = 0;
	unit[unitRot].unitScale[UNIT_RPS].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPS].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPS].txt , "rps");

	unit[unitRot].unitScale[UNIT_RPM].gain = 0.01666666;
	unit[unitRot].unitScale[UNIT_RPM].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPM].extraDigits = 1;
	unit[unitRot].unitScale[UNIT_RPM].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPM].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPM].txt , "rpm");

	unit[unitRot].unitScale[UNIT_RPh].gain = 2.777e-8;
	unit[unitRot].unitScale[UNIT_RPh].offset = 0.0;
	unit[unitRot].unitScale[UNIT_RPh].extraDigits = 2;
	unit[unitRot].unitScale[UNIT_RPh].incrGain = 1.0;
	unit[unitRot].unitScale[UNIT_RPh].incrOffset = 0.0;
	strcpy(unit[unitRot].unitScale[UNIT_RPh].txt , "rph");

	/************/
		
	unit[unitTime].unitType=unitTime;
	unit[unitTime].unitScale[UNIT_IU].gain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].offset = 0.0;
	unit[unitTime].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_IU].txt , "");
		
	unit[unitTime].unitScale[UNIT_sec].gain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].offset = 0.0;
	unit[unitTime].unitScale[UNIT_sec].extraDigits = 0;
	unit[unitTime].unitScale[UNIT_sec].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_sec].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_sec].txt , "sec");

	unit[unitTime].unitScale[UNIT_min].gain = 0.0166666;
	unit[unitTime].unitScale[UNIT_min].offset = 0.0;
	unit[unitTime].unitScale[UNIT_min].extraDigits = 2;
	unit[unitTime].unitScale[UNIT_min].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_min].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_min].txt , "min");

	unit[unitTime].unitScale[UNIT_h].gain = 2.777777e-4;
	unit[unitTime].unitScale[UNIT_h].offset = 0.0;
	unit[unitTime].unitScale[UNIT_h].extraDigits = 4;
	unit[unitTime].unitScale[UNIT_h].incrGain = 1.0;
	unit[unitTime].unitScale[UNIT_h].incrOffset = 0.0;
	strcpy(unit[unitTime].unitScale[UNIT_h].txt , "h");


	/************/
		
	unit[unitAmp].unitType=unitAmp;
	unit[unitAmp].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_IU].txt , "");
		
	unit[unitAmp].unitScale[UNIT_Amp].gain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_Amp].extraDigits = 0;
	unit[unitAmp].unitScale[UNIT_Amp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_Amp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_Amp].txt , "A");

	unit[unitAmp].unitScale[UNIT_kAmp].gain = 0.001;
	unit[unitAmp].unitScale[UNIT_kAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_kAmp].extraDigits = 3;
	unit[unitAmp].unitScale[UNIT_kAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_kAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_kAmp].txt , "kA");

	unit[unitAmp].unitScale[UNIT_mAmp].gain = 1000.0;
	unit[unitAmp].unitScale[UNIT_mAmp].offset = 0.0;
	unit[unitAmp].unitScale[UNIT_mAmp].extraDigits = -3;
	unit[unitAmp].unitScale[UNIT_mAmp].incrGain = 1.0;
	unit[unitAmp].unitScale[UNIT_mAmp].incrOffset = 0.0;
	strcpy(unit[unitAmp].unitScale[UNIT_mAmp].txt , "mA");

	/************/
		
	unit[unitVolt].unitType=unitVolt;
	unit[unitVolt].unitScale[UNIT_IU].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_IU].txt , "");
		
	unit[unitVolt].unitScale[UNIT_Volt].gain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_Volt].extraDigits = 0;
	unit[unitVolt].unitScale[UNIT_Volt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_Volt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_Volt].txt , "V");

	unit[unitVolt].unitScale[UNIT_kVolt].gain = 0.001;
	unit[unitVolt].unitScale[UNIT_kVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_kVolt].extraDigits = 3;
	unit[unitVolt].unitScale[UNIT_kVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_kVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_kVolt].txt , "kV");


	unit[unitVolt].unitScale[UNIT_mVolt].gain = 1000.0;
	unit[unitVolt].unitScale[UNIT_mVolt].offset = 0.0;
	unit[unitVolt].unitScale[UNIT_mVolt].extraDigits = -3;
	unit[unitVolt].unitScale[UNIT_mVolt].incrGain = 1.0;
	unit[unitVolt].unitScale[UNIT_mVolt].incrOffset = 0.0;
	strcpy(unit[unitVolt].unitScale[UNIT_mVolt].txt , "mV");


	
	/************/
		
	unit[unitFreq].unitType=unitFreq;
	unit[unitFreq].unitScale[UNIT_IU].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_IU].txt , "");
		
	unit[unitFreq].unitScale[UNIT_Hz].gain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_Hz].extraDigits = 0;
	unit[unitFreq].unitScale[UNIT_Hz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_Hz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_Hz].txt , "Hz");

	unit[unitFreq].unitScale[UNIT_kHz].gain = 0.001;
	unit[unitFreq].unitScale[UNIT_kHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_kHz].extraDigits = 3;
	unit[unitFreq].unitScale[UNIT_kHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_kHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_kHz].txt , "kHz");


	unit[unitFreq].unitScale[UNIT_mHz].gain = 0.000001;
	unit[unitFreq].unitScale[UNIT_mHz].offset = 0.0;
	unit[unitFreq].unitScale[UNIT_mHz].extraDigits = 6;
	unit[unitFreq].unitScale[UNIT_mHz].incrGain = 1.0;
	unit[unitFreq].unitScale[UNIT_mHz].incrOffset = 0.0;
	strcpy(unit[unitFreq].unitScale[UNIT_mHz].txt , "mHz");


	
	/************/
			
	unit[unitPerc].unitType=unitPerc;
	unit[unitPerc].unitScale[UNIT_IU].gain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_IU].txt , "");
		
	unit[unitPerc].unitScale[UNIT_PERCENT].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT].txt , "%");

	unit[unitPerc].unitScale[UNIT_PERCENT+1].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+1].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+1].txt , "%");


	unit[unitPerc].unitScale[UNIT_PERCENT+2].gain = 100.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].offset = 0.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].extraDigits = 0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrGain = 1.0;
	unit[unitPerc].unitScale[UNIT_PERCENT+2].incrOffset = 0.0;
	strcpy(unit[unitPerc].unitScale[UNIT_PERCENT+2].txt , "%");


	/************/
			
	unit[unitAngle].unitType=unitAngle;
	unit[unitAngle].unitScale[UNIT_IU].gain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_IU].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_IU].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_IU].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_IU].txt , "");
		
	unit[unitAngle].unitScale[UNIT_DEG].gain = 360.0;
	unit[unitAngle].unitScale[UNIT_DEG].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_DEG].extraDigits = 0;
	unit[unitAngle].unitScale[UNIT_DEG].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_DEG].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_DEG].txt , " deg");

	unit[unitAngle].unitScale[UNIT_RAD].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD].txt , " rad");

	unit[unitAngle].unitScale[UNIT_RAD+1].gain = M_TWOPI;
	unit[unitAngle].unitScale[UNIT_RAD+1].offset = 0.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].extraDigits = 2;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrGain = 1.0;
	unit[unitAngle].unitScale[UNIT_RAD+1].incrOffset = 0.0;
	strcpy(unit[unitAngle].unitScale[UNIT_RAD+1].txt , " rad");

}

void ParseType(DRILL_DWN_MGR_obj *menu , UINT  indx){
UINT i , k;

//#define N_STAT 		0  //display as normal, no icon, focus box skips
//#define N_EDIT		1  //display as normal, edit icon, focus box, enter jumps to edit page (with type)
//#define N_JUMP 		2  //display as normal, jump icon, focus box, enter jumps to new node
//#define N_SCROLL_PG 4  //not for Android pendant, treat as N_SCROLL  
//#define N_SCROLL 	8  //display as normal, no edit icon, focus box, ne edit, no jump


// Additive
//#define N_EVENT 	0x10 //can be combined with N_STAT, N_EDIT, N_JUMP, N_SCROLL_PG, N_SCROLL (set event value into fire event) 
//#define N_EVENT_PROMPT_YES_NO 0x20 // like N_EVENT but ask YES NO before send event
//#define N_EVENT_PROMPT_OK 0x40  // like N_EVENT but ask for  YES before send event
//#define N_EDIT_IN_PLACE 0x80 // not focus scroll , send index , encoder and enter Btn state to plc, plc updates node text

LCD_NODE_obj *newNode;

Cfg_typ *_cfg;

	_cfg = menu->cfg;
	
	if(!_cfg)
			return;

	if(!menu->editPageData)
		return;

	
	//////////////////////

	i = menu->curTopNodeID + indx;

	menu->curNodeID = i;


	if(menu->nodes[i].Type & (N_EVENT + N_EVENT_PROMPT_YES_NO + N_EVENT_PROMPT_OK)){

		menu->editPageData->Confrim_Btn_vis = 1; // INVISIBLE
		if(menu->nodes[i].Type & (N_EVENT_PROMPT_YES_NO + N_EVENT_PROMPT_OK)){
			menu->PendingFiredEvent = menu->nodes[i].Event;
			menu->ShowPopUpMsg = 1;
			menu->editPageData->Confrim_Btn_vis = 0; //VISIBLE
			
		}		
		else{

			menu->FiredEvent = menu->nodes[i].Event;
			//menu->PleaseUpdate = 1; 

		}

	}
	

	if(menu->nodes[i].Type & N_JUMP){

		
		if(menu->nodes[i].pJumpNode){

		
			newNode = (LCD_NODE_obj*) menu->nodes[i].pJumpNode;
		
			menu->curHeadID = newNode->HeadID;
			menu->curTopNodeID = newNode->ID;
			menu->curNodeID = newNode->ID;
			
		}

		for(k = 0 ; k < menu->lineNum ; k++)
			menu->TxtLineHighLight[k] = 0;
		

	}
	
	else if(menu->nodes[i].Type & N_EDIT){	

		/*
		#define T_NATIVE		0x0 // native PVI type
		#define T_INTEGER		0x1
		#define T_ENUM			0x2
		#define T_SCALED		0x4   // This cover Float
		#define T_STRING		0x8
		*/

		if(menu->nodes[i].var.Type == T_SCALED){

			if(menu->editPageData){

				//
				//menu->Editor_vis = (menu->Editor_vis > 0)? 0:1;

				k = (_cfg->ScaledVar[menu->nodes[i].var.Var1ID].segm)? menu->pMaster->curSegm[ZONE]:0;
				SetScaledValueInEditor(menu , &_cfg->ScaledVar[menu->nodes[i].var.Var1ID], k);

				if(!menu->Save_Exit_Cmd	
					&& !menu->Cancel_Return_Cmd){
					
					menu->Editor_vis = 0; //VISIBLE;
					//menu->PleaseUpdate = menu->nodes[i].var.Var1ID + k * 1024 ;

				}				
			
			}
		
			for(k = 0 ; k < menu->lineNum ; k++)
				menu->TxtLineHighLight[k] = 0;

			menu->curActiveLine = 0;

		}
		else if(menu->nodes[i].var.Type == T_ENUM){
			
			
			for(k = 0 ; k < menu->lineNum ; k++){

				if(k == indx)
					menu->TxtLineHighLight[indx] = (menu->TxtLineHighLight[indx] > 0)? 0:1; 	
				else
					menu->TxtLineHighLight[k] = 0;

			}
			menu->curActiveLine = (menu->TxtLineHighLight[indx] > 0)?   (indx+1):0;
			//menu->curNodeID = i;				

		}
		

	}
	
	else if(menu->nodes[i].Type & N_EDIT_IN_PLACE){

		for(k = 0 ; k < menu->lineNum ; k++){

			if(k == indx)
				menu->TxtLineHighLight[indx] = (menu->TxtLineHighLight[indx] > 0)? 0:1;		
			else
				menu->TxtLineHighLight[k] = 0;

		}
		menu->curActiveLine = (menu->TxtLineHighLight[indx] > 0)?   (indx+1):0;
		//menu->curNodeID = i;

	}
	else{

		for(k = 0 ; k < menu->lineNum ; k++)
			menu->TxtLineHighLight[k] = 0;

		menu->curActiveLine = 0;
	}
		

	//menu->Ticks_vis = ((menu->TxtLineHighLight[indx] > 0) || (menu->curHeadID == P_LOGS))? 0:1;
	menu->Ticks_vis = ((menu->TxtLineHighLight[indx] > 0) || (menu->Editor_vis == 0))? 0:1;
	menu->LCDEntr = (menu->Ticks_vis)? 0:1;

}

USINT ReDrawLine(DRILL_DWN_MGR_obj *menu , UINT indx){
UINT i , k;
plcstring format[64];
char* p;
DINT *pDINT;
Cfg_typ *_cfg;
CurCfg_typ *_curCfg;
float x;
USINT status;

/*
#define T_NATIVE		0x0 // native PVI type
#define T_INTEGER		0x1
#define T_ENUM			0x2
#define T_SCALED		0x4   // This cover Float
#define T_STRING		0x8
*/

	i = menu->curTopNodeID + indx;

	_cfg = menu->cfg;
	_curCfg = menu->curCfg;

	status = 0;
	
	if( i  < NODES_NUM_MAX){

		//xsprintf(&menu->UnitLines[indx], "ID %d - &d", menu->nodes[i].ID , menu->nodes[i].var.Var1ID);
		
		p = strstr(menu->nodes[i].var.Format , "%name%");
			
		switch((menu->nodes[i].var.Type & 0xF)){

			case T_SCALED:

				p = strstr(menu->nodes[i].var.Format , "%name%");
		
				if(p && menu->nodes[i].var.Var1ID){
					
					strcpy(format , _cfg->ScaledVar[menu->nodes[i].var.Var1ID].name);					
					strcpy(&menu->TxtLines[indx] , format);
				
				}else{
					
					strcpy(&menu->TxtLines[indx] , menu->nodes[i].var.Format);
				}

				if(menu->nodes[i].var.Var1ID < parLAST_PARAM){

					if(_curCfg->Param[menu->nodes[i].var.Var1ID].segmYes)
						k = menu->pMaster->curSegm[ZONE];
					else
						k = 0;

					switch(_cfg->ScaledVar[menu->nodes[i].var.Var1ID].precision){


					}

					SetScaledValue(menu , &_cfg->ScaledVar[menu->nodes[i].var.Var1ID] , indx);
					//strcpy(&menu->VarLines[indx] , GetScaledValue(menu , &_cfg->ScaledVar[menu->nodes[i].var.Var1ID]));
					//xsprintf(&menu->VarLines[indx], "%5.2f" , _curCfg->Param[menu->nodes[i].var.Var1ID].val[k]);
				}
				else {

					strcpy(&menu->VarLines[indx], "<<  >>");
				}

				menu->TxtLines_vis[indx] = 0 ; //VISIBLE ; 
				menu->TwoTxtLines_vis[indx] = 1 ; //INVISIBLE ; 
				menu->LongTxtLines_vis[indx] = 1 ; //INVISIBLE ; 
				

			break;

			
			case T_ENUM: 				

				if(p && menu->nodes[i].var.Var1ID){
					strcpy(format , _cfg->EnumVar[menu->nodes[i].var.Var1ID].name);					
					strcpy(&menu->TxtLines[indx] , format);
				}else{
					
					strcpy(&menu->TxtLines[indx] , menu->nodes[i].var.Format);
				}

				

				 if(_cfg->EnumVar[menu->nodes[i].var.Var1ID].pVal) // by pointer
				 
					pDINT = (DINT*) _cfg->EnumVar[menu->nodes[i].var.Var1ID].pVal;	

				 else if(_cfg->EnumVar[menu->nodes[i].var.Var1ID].Indx < menu->pMaster->paramEnumLastNum) // in CurCfg area
				 
					pDINT = &_curCfg->Param_ENUM[_cfg->EnumVar[menu->nodes[i].var.Var1ID].Indx].val[0];
				
				else
					break;

				strcpy(&menu->VarLines[indx] , _cfg->TXT_Resources[_cfg->EnumVar[menu->nodes[i].var.Var1ID].TxtIndx +(*pDINT)]);
				
				//strcpy(&menu->VarLines[indx], "Enum");

				menu->TwoTxtLines_vis[indx] = 0 ; //VISIBLE ; 
				menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 
				menu->LongTxtLines_vis[indx] = 1 ; //INVISIBLE ; 

			break;
			
			case T_INTEGER:					

				if(p && menu->nodes[i].var.Var1ID){
					strcpy(format , _cfg->IntVar[menu->nodes[i].var.Var1ID].name);					
					strcpy(&menu->TxtLines[indx] , format);
				}else{
					
					strcpy(&menu->TxtLines[indx] , menu->nodes[i].var.Format);
				}

				if(_cfg->IntVar[menu->nodes[i].var.Var1ID].pVal) // by pointer
				 
					pDINT = (DINT*) _cfg->IntVar[menu->nodes[i].var.Var1ID].pVal;	

				 else if(_cfg->IntVar[menu->nodes[i].var.Var1ID].Indx < menu->pMaster->paramIntgLastNum) // in CurCfg area
				 
					pDINT = &_curCfg->Param_INT[_cfg->IntVar[menu->nodes[i].var.Var1ID].Indx].val[0];
				
				else
					break;
				
				//strcpy(&menu->VarLines[indx], "Integer");
				xsprintf(&menu->VarLines[indx] , " %d" , *pDINT);
				

				menu->TwoTxtLines_vis[indx] = 0 ; //VISIBLE ; 
				menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 
				menu->LongTxtLines_vis[indx] = 1 ; //INVISIBLE ;  

			break;

			case T_STRING:	


				if(_cfg->StringVar[menu->nodes[i].var.Var1ID].pStr){
				
					if(strlen(menu->nodes[i].var.Format) > strlen(_cfg->StringVar[menu->nodes[i].var.Var1ID].pStr)){

						xsprintf(&menu->TxtLines[indx], menu->nodes[i].var.Format , " ");

						strcpy(&menu->VarLines[indx] ,  _cfg->StringVar[menu->nodes[i].var.Var1ID].pStr);
						
						menu->TwoTxtLines_vis[indx] = 0 ; //VISIBLE ; 
						menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 
						menu->LongTxtLines_vis[indx] = 1 ; //INVISIBLE ; 

						
					}
					else {
						
						xsprintf(&menu->TxtLines[indx], menu->nodes[i].var.Format , _cfg->StringVar[menu->nodes[i].var.Var1ID].pStr);

						menu->LongTxtLines_vis[indx] = 0 ; //VISIBLE ;
						menu->TwoTxtLines_vis[indx] = 1 ; //VISIBLE ; 
						menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 

					}
					
				}
				else{

					strcpy(&menu->TxtLines[indx], menu->nodes[i].var.Format);
				
					menu->LongTxtLines_vis[indx] = 0 ; //VISIBLE ;
					menu->TwoTxtLines_vis[indx] = 1 ; //VISIBLE ; 
					menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 



				}

				
				
				
				
				
			break;

			default:
				
				strcpy(&menu->TxtLines[indx] , menu->nodes[i].var.Format);
				
				menu->LongTxtLines_vis[indx] = 0 ; //VISIBLE ;
				menu->TwoTxtLines_vis[indx] = 1 ; //VISIBLE ; 
				menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 
					

			break;

		}


		 
	}

	if(menu->nodes[i].HeadID != menu->curHeadID){
		
		menu->LongTxtLines_vis[indx] = 1 ; //INVISIBLE ;
		menu->TwoTxtLines_vis[indx] = 1 ; //INVISIBLE ; 
		menu->TxtLines_vis[indx] = 1 ; //INVISIBLE ; 

		menu->LineAtr_vis[indx] = 1;

	}
	else
		menu->LineAtr_vis[indx] = 0;


					
	
/*
	// Non Additive
#define N_STAT 		0  //display as normal, no icon, focus box skips
#define N_EDIT		1  //display as normal, edit icon, focus box, enter jumps to edit page (with type)
#define N_JUMP 		2  //display as normal, jump icon, focus box, enter jumps to new node
#define N_SCROLL_PG 4  //not for Android pendant, treat as N_SCROLL  
#define N_SCROLL 	8  //display as normal, no edit icon, focus box, ne edit, no jump
	
	
	// Additive
#define N_EVENT 	0x10 //can be combined with N_STAT, N_EDIT, N_JUMP, N_SCROLL_PG, N_SCROLL (set event value into fire event) 
#define N_EVENT_PROMPT_YES_NO 0x20 // like N_EVENT but ask YES NO before send event
#define N_EVENT_PROMPT_OK 0x40  // like N_EVENT but ask for  YES before send event
#define N_EDIT_IN_PLACE 0x80 // not focus scroll , send index , encoder and enter Btn state to plc, plc updates node text
*/
	menu->LineAtr[indx] = 0;
	
	switch(menu->nodes[i].Type & 0x0F){

		case N_STAT:

		break;

		case N_EDIT:

			menu->LineAtr[indx] = 1;

		break;

		case N_JUMP:

			menu->LineAtr[indx] = 2;

		break;

		case N_SCROLL_PG:

		break;

		case N_SCROLL:

		break;	


	}

	switch(menu->nodes[i].Type & 0xF0){

		case N_EVENT:

			menu->LineAtr[indx] = 3;
			

		break;

		case N_EVENT_PROMPT_YES_NO:

		break;

		case N_EVENT_PROMPT_OK:

		break;

		case N_SCROLL_PG:

		break;

		case N_EDIT_IN_PLACE:

			menu->LineAtr[indx] = 4;

		break;	


	}

	//menu->LineAtr[menu->curActiveLine] = 5;
	

	return status;

}
 
USINT ReDrawPage(DRILL_DWN_MGR_obj *menu){
UINT i;
USINT status;

		status = 0;
		for(i = 0 ; i < menu->lineNum ; i++){


			if(ReDrawLine(menu , i))
				status = 1;
			
		}

		return status;

}

USINT ConvertUnit(DRILL_DWN_MGR_obj *menu , UINT Var1ID){



	
}

char* GetScaledValue(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var){
UINT  i;
//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;


	if(!var || !menu)
			return "N/A1";	
		
	//var = (Scaled_Var_obj*)pVal;

	//if(var->Indx >= lcd->pMaster->varLastNum)
		//return "N/A2";

	//if((var->Indx >= lcd->pMaster->varLastNum))
		//return "N/A3";


	if(!menu->pMaster->pUnits)
		return "N/A4";

	//if(var->segm > menu->pMaster->numZone)
		//return "Wrong Segm";

	i = (var->segm)? menu->pMaster->curSegm[ZONE]:0;
	if(i > SEGM_NUM_MAX)
		i = 0;
	
	if(var->pfVal)		
		//val = (*(var->pfVal+ var->segm));
		val = (*(var->pfVal));
	
	else if(var->Indx < menu->pMaster->paramLastNum)
		
		val = menu->pMaster->pCurCfg->Param[var->Indx].val[i];
	
	else
		return "Wrong Var";
	
	units = (Units_obj*)menu->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;


	
	if(var->segm)
		xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
	else
		xsprintf(ret,"%s %s", dat , scale->txt );
	
		
	return ret;

}

void SetScaledValue(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var , UINT indx){
UINT  i;
//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;


	if(!var || !menu)
			return;		

	if(!menu->pMaster->pUnits)
		return ;

	//if(var->segm > menu->pMaster->numZone)
		//return "Wrong Segm";

	i = (var->segm)? menu->pMaster->curSegm[ZONE]:0;
	if(i > SEGM_NUM_MAX)
		i = 0;
	
	if(var->pfVal)		
		//val = (*(var->pfVal+ var->segm));
		val = (*(var->pfVal));
	
	else if(var->Indx < menu->pMaster->paramLastNum)
		
		val = menu->pMaster->pCurCfg->Param[var->Indx].val[i];
	
	else
		return "Wrong Var";
	
	units = (Units_obj*)menu->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;


	/*	
	if(var->segm)
		xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
	else
		xsprintf(ret,"%s %s", dat , scale->txt );

	*/

	
	strcpy(menu->VarLines[indx] , dat);

	if(var->segm){

		xsprintf(menu->UnitLines[indx],"%d: %s", menu->pMaster->curSegm[ZONE] , scale->txt );

	}
	else{

		xsprintf(menu->UnitLines[indx],"  %s", scale->txt );
	}
		
	return;

}


void ReadEnumVar(READ_ENUM_VAR_typ* menu , UINT enum_indx , UINT segm){
UINT i;

	if(enum_indx >= PARAM_ENUM_NUM_MAX)
		return;

	 i = Cfg.EnumVar[enum_indx].TxtIndx + CurCfg.Param_ENUM[enum_indx].val[segm];
	
	if(i < TXT_RESOURCES_MAX){

		strcpy(menu->EnumTxt , Cfg.TXT_Resources[i]);

		strcpy(menu->ParamName , Cfg.EnumVar[enum_indx].name);


	}
	else
		strcpy(menu->EnumTxt , "Fault");

	menu->ParamEnum_Indx = enum_indx;
	menu->Param_Enum_Segm = segm;
	

}
void ReadScaledVar(READ_VAR_typ* menu , Scaled_Var_obj* var , UINT segm ){
UINT  i;
//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;
Cfg_typ *_cfg;

	if(!var || !menu)
			return;	
	
	if(!menu->pMaster)
		return;
	
	if(!menu->pMaster->pUnits)
		return ;	

	_cfg = menu->pMaster->pCfg;

	if(!_cfg)
			return;	 

	if(!menu->pMaster->pCurCfg)
		return;

	if(!menu->pMaster->pScaledUnitIndx)
		return;

	//if(var->segm > menu->pMaster->numZone)
		//return "Wrong Segm";

	//i = (var->segm)? menu->pMaster->curSegm[ZONE]:0;

	i = segm;
	
	if(i > SEGM_NUM_MAX)
		i = 0;

	//////////  Value ////////////
	if(var->pfVal)	
		//val = var->pfVal[i];
		//val = (*(var->pfVal+ var->segm));
		val = (*(var->pfVal));
	
	else if(var->Indx < menu->pMaster->paramLastNum)
		
		val = menu->pMaster->pCurCfg->Param[var->Indx].val[i];
	
	else
		return ;

	
	
	units = (Units_obj*)menu->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->PrecDigits = _cfg->ScaledVar[var->Indx].precision + scale->extraDigits;
	
	menu->MainVal_Disp = val;

	if(var->pfVal) // Read only var
		xsprintf(menu->ParamName,"%s [%s]", _cfg->ScaledVar[var->Indx].name ,scale->txt);

	else if(i > 0)
		xsprintf(menu->ParamName,"%s [%s]:%d", _cfg->ScaledVar[var->Indx].name ,scale->txt , i );
	else
		xsprintf(menu->ParamName,"%s [%s]", _cfg->ScaledVar[var->Indx].name ,scale->txt);

	//strcpy(menu->ParamName , _cfg->ScaledVar[var->Indx].name);
	//strcpy(menu->UnitsName , scale->txt);


	/////

	menu->pScale = scale;
	
	menu->Param_Indx = var->Indx;

	menu->Segm_Indx = i;

	menu->var = var;

	return;

	/////////////////////////////////

	val = menu->pMaster->pCurCfg->Param[var->Indx].min;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->MinVal_Disp = val;

	/////////////////////////////////

	val =  menu->pMaster->pCurCfg->Param[var->Indx].max;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->MaxVal_Disp = val;
	
	/////////////////////////////////
	
	val =  menu->pMaster->pCurCfg->Param[var->Indx].incr;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->Incr_Disp = val;

	//////////////////////////	

	

	/////
	
		
	menu->MinVal_Cfg = _cfg->Param[var->Indx].min;
	menu->MaxVal_Cfg = _cfg->Param[var->Indx].max;
	menu->MinIncr_Cfg = _cfg->Param[var->Indx].incrMin;
	menu->MaxIncr_Cfg = _cfg->Param[var->Indx].incrMax;
	menu->defaultVal = _cfg->Param[var->Indx].defaultVal;

	////////////////////////////////

	/*	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;


	
	if(var->segm)
		xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
	else
		xsprintf(ret,"%s %s", dat , scale->txt );

	*/

	
		
	return;

}

float GetParamValue(MASTER_typ *m , Scaled_Var_obj* var , UINT segm ){
Unit_Scale_obj *scale;	
Units_obj* units;;
float val;

	if(!m)
		return 0.0;

	if(!var)
		return 0.0;


	if(var->Indx < m->paramLastNum)
			
		val = m->pCurCfg->Param[var->Indx].val[segm];
		
	else
		return 0.0;
		
		
	units = (Units_obj*)m->pUnits;
	scale = &units[var->unitType].unitScale[m->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	return val;

}

void SetScaledValueInSingleEditor(SINGLE_EDITOR_MGR_obj* menu , Scaled_Var_obj* var , UINT segm ){
UINT  i;
//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;
Cfg_typ *_cfg;
	
	_cfg = menu->cfg;

	if(!_cfg)
			return;



	if(!var || !menu)
			return;	
	 
	
	if(!menu->pMaster->pUnits)
		return ;

	if(!menu->editPageData)
		return ;

	//if(var->segm > menu->pMaster->numZone)
		//return "Wrong Segm";

	//i = (var->segm)? menu->pMaster->curSegm[ZONE]:0;

	i = segm;
	
	if(i > SEGM_NUM_MAX)
		i = 0;

	//////////  Value ////////////
	if(var->pfVal)	
		val = var->pfVal[i];
		//val = (*(var->pfVal+ var->segm));
		//val = (*(var->pfVal));
	
	else if(var->Indx < menu->pMaster->paramLastNum)
		
		val = menu->pMaster->pCurCfg->Param[var->Indx].val[i];
	
	else
		return "Wrong Var";
	
	units = (Units_obj*)menu->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;
	
	menu->editPageData->MainVal_Disp = val;

	/////////////////////////////////

	val = menu->pMaster->pCurCfg->Param[var->Indx].min;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->MinVal_Disp = val;

	/////////////////////////////////

	val =  menu->pMaster->pCurCfg->Param[var->Indx].max;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->MaxVal_Disp = val;
	
	/////////////////////////////////
	
	val =  menu->pMaster->pCurCfg->Param[var->Indx].incr;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->Incr_Disp = val;

	//////////////////////////	

	
	
	strcpy(menu->editPageData->ParamName , _cfg->ScaledVar[var->Indx].name);

	strcpy(menu->editPageData->UnitsName , scale->txt);

	menu->editPageData->pScale = scale;
	
	menu->editPageData->Param_Indx = var->Indx;

	menu->editPageData->Segm_Indx = i;

	menu->editPageData->var = var;

	/////
	menu->editPageData->PrecDigits = _cfg->ScaledVar[var->Indx].precision + scale->extraDigits;
		
	menu->editPageData->MinVal_Cfg = _cfg->Param[var->Indx].min;
	menu->editPageData->MaxVal_Cfg = _cfg->Param[var->Indx].max;
	menu->editPageData->MinIncr_Cfg = _cfg->Param[var->Indx].incrMin;
	menu->editPageData->MaxIncr_Cfg = _cfg->Param[var->Indx].incrMax;
	menu->editPageData->defaultVal = _cfg->Param[var->Indx].defaultVal;

	////////////////////////////////

	/*	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;


	
	if(var->segm)
		xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
	else
		xsprintf(ret,"%s %s", dat , scale->txt );

	*/

	
		
	return;

}

void SetScaledValueInEditor(DRILL_DWN_MGR_obj* menu , Scaled_Var_obj* var , UINT segm ){
UINT  i;
//Scaled_Var_obj *var;
char *dat;
static char ret[64] , blankField[32];
float val , gain , offset;
Unit_Scale_obj *scale;
Units_obj *units;
Cfg_typ *_cfg;
CurCfg_typ *_curCfg;

	
	_cfg = menu->cfg;

	_curCfg = menu->curCfg;

	if(!_cfg)
			return;

	if(!_curCfg)
		return;



	if(!var || !menu)
			return;	
	 
	
	if(!menu->pMaster->pUnits)
		return ;

	if(!menu->editPageData)
		return ;

	//if(var->segm > menu->pMaster->numZone)
		//return "Wrong Segm";

	//i = (var->segm)? menu->pMaster->curSegm[ZONE]:0;

	i = segm;
	
	if(i > SEGM_NUM_MAX)
		i = 0;

	//////////  Value ////////////
	if(var->pfVal)	
		val = var->pfVal[i];
		//val = (*(var->pfVal+ var->segm));
		//val = (*(var->pfVal));
	
	else if(var->Indx < menu->pMaster->paramLastNum)
		
		val = menu->pMaster->pCurCfg->Param[var->Indx].val[i];
	
	else
		return "Wrong Var";
	
	units = (Units_obj*)menu->pMaster->pUnits;	
	scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;
	
	menu->editPageData->MainVal_Disp = val;

	/////////////////////////////////

	val = menu->pMaster->pCurCfg->Param[var->Indx].min;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->MinVal_Disp = val;

	/////////////////////////////////

	val =  menu->pMaster->pCurCfg->Param[var->Indx].max;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->MaxVal_Disp = val;
	
	/////////////////////////////////
	
	val =  menu->pMaster->pCurCfg->Param[var->Indx].incr;
	//scale = &units[var->unitType].unitScale[menu->pMaster->pScaledUnitIndx[var->Indx]];
	val = val * scale->gain + scale->offset;

	menu->editPageData->Incr_Disp = val;

	//////////////////////////	
	menu->editPageData->Segm_Indx_Locked = (_curCfg->Param[ var->Indx].segmYes)? 0:1;
	//////////////////////////////
	
	
	strcpy(menu->editPageData->ParamName , _cfg->ScaledVar[var->Indx].name);

	strcpy(menu->editPageData->UnitsName , scale->txt);

	menu->editPageData->pScale = scale;
	
	menu->editPageData->Param_Indx = var->Indx;

	menu->editPageData->Segm_Indx = i;

	menu->editPageData->var = var;

	/////
	menu->editPageData->PrecDigits = _cfg->ScaledVar[var->Indx].precision + scale->extraDigits;
		
	menu->editPageData->MinVal_Cfg = _cfg->Param[var->Indx].min;
	menu->editPageData->MaxVal_Cfg = _cfg->Param[var->Indx].max;
	menu->editPageData->MinIncr_Cfg = _cfg->Param[var->Indx].incrMin;
	menu->editPageData->MaxIncr_Cfg = _cfg->Param[var->Indx].incrMax;
	menu->editPageData->defaultVal = _cfg->Param[var->Indx].defaultVal;

	////////////////////////////////

	/*	
	dat = xfdtoa((double)val , var->precision + scale->extraDigits);

	if(dat[0]== '+')
		dat++;


	
	if(var->segm)
		xsprintf(ret,"%d: %s %s", var->segm , dat , scale->txt );
	else
		xsprintf(ret,"%s %s", dat , scale->txt );

	*/

	
		
	return;

}
void GetScaledValueFromSingleEditor(SINGLE_EDITOR_MGR_obj* menu ){
UINT  i , k;
float val;
Unit_Scale_obj *scale;
Units_obj *units;
Cfg_typ *_cfg;

	if(!menu)
		return;
	
	_cfg = menu->cfg;
	if(!_cfg)
		return;
	
	if(!menu->pMaster->pUnits)
		return ;

	if(!menu->editPageData)
		return ;

	////////////////////////////

	scale = menu->editPageData->pScale;
	i = menu->editPageData->Param_Indx;
	k = menu->editPageData->Segm_Indx;
	
	if(scale){
		
		if((scale->gain != 0.0)
			&&(i < NODES_NUM_MAX)
			&&( k < SEGM_NUM_MAX)){
			
			val = menu->editPageData->MainVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].val[k] = val ;

			
			val = menu->editPageData->MinVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].min = val ;

			val = menu->editPageData->MaxVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].max = val ;

			val = menu->editPageData->Incr_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].incr = val ;
		}
	}


	
	return;

}

void GetScaledValueFromEditor(DRILL_DWN_MGR_obj* menu ){
UINT  i , k;
float val;
Unit_Scale_obj *scale;
Units_obj *units;
Cfg_typ *_cfg;

	if(!menu)
		return;
	
	_cfg = menu->cfg;
	if(!_cfg)
		return;
	
	if(!menu->pMaster->pUnits)
		return ;

	if(!menu->editPageData)
		return ;

	////////////////////////////

	scale = menu->editPageData->pScale;
	i = menu->editPageData->Param_Indx;
	k = menu->editPageData->Segm_Indx;
	
	if(scale){
		
		if((scale->gain != 0.0)
			&&(i < NODES_NUM_MAX)
			&&( k < SEGM_NUM_MAX)){
			
			val = menu->editPageData->MainVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].val[k] = val ;

			
			val = menu->editPageData->MinVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].min = val ;

			val = menu->editPageData->MaxVal_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].max = val ;

			val = menu->editPageData->Incr_Disp;
			val = (val - scale->offset) /scale->gain;			
			menu->pMaster->pCurCfg->Param[i].incr = val ;
		}
	}


	
	return;

}

void HandleTicks_ENUM(DRILL_DWN_MGR_obj* menu ){
UINT  i , k;
Cfg_typ *_cfg;
CurCfg_typ *_curCfg;

	menu->KK_test[0]++;

	if(!menu)
		return;
	
	_cfg = menu->cfg;
	
	if(!_cfg)
		return;

	_curCfg = menu->curCfg;

	if(!_curCfg)
		return;

	i = menu->nodes[menu->curNodeID].var.Var1ID;
	if(i >= PARAM_ENUM_NUM_MAX)
		return;

	
	if(menu->TickPlus){
		

		if(_curCfg->Param_ENUM[i].val[0] < _cfg->Param_ENUM[i].max){
			_curCfg->Param_ENUM[i].val[0]++;			
		}
	}
	else if(menu->TickMinus){		

		if(_curCfg->Param_ENUM[i].val[0] > _cfg->Param_ENUM[i].min){
			_curCfg->Param_ENUM[i].val[0]--;			
		}

	}

	return;

}

void HandleTicks_SCALED(DRILL_DWN_MGR_obj* menu ){
UINT  i , k;
Cfg_typ *_cfg;
CurCfg_typ *_curCfg;

	menu->KK_test[0]++;

	if(!menu)
		return;
	
	_cfg = menu->cfg;
	
	if(!_cfg)
		return;

	_curCfg = menu->curCfg;

	if(!_curCfg)
		return;

	if(!menu->editPageData)
		return;

	i = menu->editPageData->Param_Indx;
	k = menu->editPageData->Segm_Indx;

	//i = menu->nodes[menu->curNodeID].var.Var1ID;
	//k = (_cfg->ScaledVar[menu->nodes[i].var.Var1ID].segm)? menu->pMaster->curSegm[ZONE]:0;
	
	if(i >= PARAM_NUM_MAX)
		return;

	if(k > SEGM_NUM_MAX)
		return;
	

#if 0
	////// save to  curcfg
	
	if(menu->TickPlus){			
		
		_curCfg->Param[i].val[k] += _curCfg->Param[i].incr;	

		
	
		
	}
	else if(menu->TickMinus){
		
		_curCfg->Param[i].val[k] -= _curCfg->Param[i].incr;	
		
	}

	if(_curCfg->Param[i].val[k] > _cfg->Param[i].max)
			_curCfg->Param[i].val[k] = _cfg->Param[i].max;

	else if(_curCfg->Param[i].val[k] < _cfg->Param[i].min)
		_curCfg->Param[i].val[k] = _cfg->Param[i].min;			

	SetScaledValueInEditor(menu , &_cfg->ScaledVar[i], k);

	///////////
#else
	
	if(menu->TickPlus){ 		
			
		//_curCfg->Param[i].val[k] += _curCfg->Param[i].incr; 	
		menu->editPageData->MainVal_Disp += menu->editPageData->Incr_Disp;
			
	}
	else if(menu->TickMinus){
		
		//_curCfg->Param[i].val[k] -= _curCfg->Param[i].incr; 
		menu->editPageData->MainVal_Disp -= menu->editPageData->Incr_Disp;
		
	}

	if(menu->editPageData->MainVal_Disp > _cfg->Param[i].max)
			menu->editPageData->MainVal_Disp = _cfg->Param[i].max;

	else if(menu->editPageData->MainVal_Disp < _cfg->Param[i].min)
		menu->editPageData->MainVal_Disp = _cfg->Param[i].min;	

#endif		

	return;

}

////////
#endif		me
