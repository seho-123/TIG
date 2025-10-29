######################################################
#                                                    #
# Automatic generated Makefile for Visual Components #
#                                                    #
#                  Do NOT edit!                      #
#                                                    #
######################################################

VCC:=@'$(AS_BIN_PATH)/br.vc.pc.exe'
LINK:=@'$(AS_BIN_PATH)/BR.VC.Link.exe'
MODGEN:=@'$(AS_BIN_PATH)/BR.VC.ModGen.exe'
VCPL:=@'$(AS_BIN_PATH)/BR.VC.PL.exe'
VCHWPP:=@'$(AS_BIN_PATH)/BR.VC.HWPP.exe'
VCDEP:=@'$(AS_BIN_PATH)/BR.VC.Depend.exe'
VCFLGEN:=@'$(AS_BIN_PATH)/BR.VC.lfgen.exe'
VCREFHANDLER:=@'$(AS_BIN_PATH)/BR.VC.CrossReferenceHandler.exe'
VCXREFEXTENDER:=@'$(AS_BIN_PATH)/BR.AS.CrossRefVCExtender.exe'
RM=CMD /C DEL
PALFILE_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Palette.vcr
VCCFLAGS_Visu_M=-server -proj Visu_M -vc '$(AS_PROJECT_PATH)/Logical/Visu_M/VCObject.vc' -prj_path '$(AS_PROJECT_PATH)' -temp_path '$(AS_TEMP_PATH)' -cfg $(AS_CONFIGURATION) -plc $(AS_PLC) -plctemp $(AS_TEMP_PLC) -cpu_path '$(AS_CPU_PATH)'
VCFIRMWARE=4.73.1
VCFIRMWAREPATH=$(AS_VC_PATH)/Firmware/V4.73.1/SG4
VCOBJECT_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/VCObject.vc
VCSTARTUP='vcstart.br'
VCLOD='vclod.br'
VCSTPOST='vcstpost.br'
TARGET_FILE_Visu_M=$(AS_CPU_PATH)/Visu_M.br
OBJ_SCOPE_Visu_M=
PRJ_PATH_Visu_M=$(AS_PROJECT_PATH)
SRC_PATH_Visu_M=$(AS_PROJECT_PATH)/Logical/$(OBJ_SCOPE_Visu_M)/Visu_M
TEMP_PATH_Visu_M=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu_M
TEMP_PATH_Shared=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared
TEMP_PATH_ROOT_Visu_M=$(AS_TEMP_PATH)
VC_LIBRARY_LIST_Visu_M=$(TEMP_PATH_Visu_M)/libraries.vci
VC_XREF_BUILDFILE_Visu_M=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.build
VC_XREF_CLEANFILE=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.clean
VC_LANGUAGES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr
CPUHWC='$(TEMP_PATH_Visu_M)/cpuhwc.vci'
VC_STATIC_OPTIONS_Visu_M='$(TEMP_PATH_Visu_M)/vcStaticOptions.xml'
VC_STATIC_OPTIONS_Shared='$(TEMP_PATH_Shared)/vcStaticOptions.xml'
# include Shared and Font Makefile (only once)
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCFntDat/Font_Visu_M.mak
ifneq ($(VCINC),1)
	VCINC=1
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/VCShared.mak
endif

DEPENDENCIES_Visu_M=-d vcgclass -profile 'False'
DEFAULT_STYLE_SHEET_Visu_M='Source[local].StyleSheet[Default]'
SHARED_MODULE=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcshared.br
LFNTFLAGS_Visu_M=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)'
BDRFLAGS_Visu_M=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)'

# Local Libs
LIB_LOCAL_OBJ_Visu_M=$(TEMP_PATH_Visu_M)/localobj.vca

# Hardware sources
PANEL_HW_OBJECT_Visu_M=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu_M/dis.Hardware.vco
PANEL_HW_VCI_Visu_M=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu_M/dis.Hardware.vci
PANEL_HW_SOURCE_Visu_M=C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Physical/X20CP1585_MIG_HW_PS/Hardware.hw 
DIS_OBJECTS_Visu_M=$(PANEL_HW_OBJECT_Visu_M) $(KEYMAP_OBJECTS_Visu_M)

# KeyMapping flags
KEYMAP_SOURCES_Visu_M=
KEYMAP_OBJECTS_Visu_M=

# All Source Objects
TXTGRP_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/InstantMessages.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/HeaderBar.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/PageNames.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/Buttons_PageTexts.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/Languages.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/NumPad_Limits.txtgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/httpURL_SDM.txtgrp 

FNINFO_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Info.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Html_SDM.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Default.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Header.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Button.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Input.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Status.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial12.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial14.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial18.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial20.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial16.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial24.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial28.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial22.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial24Bold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/A_13.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/A_18_B.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial19.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial18ItalicBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Fonts/Arial32Bold.fninfo 

BMINFO_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_AcknowledgeReset.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Active.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassOFF.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassON.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Inactive.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Latched.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_NotQuit.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Quit.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Reset.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_ResetAcknowledge.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Triggered.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/ProgressBorder.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/alarm.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_checked.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/information.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio_selected.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/warning.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/frame_header.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_checked.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_09x09.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowRightGray.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowUpGray.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_BallGray.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_200x200_round_nodiv.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_NeedleRed100x11_1.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_gray.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/FrameInvisible.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_off.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_on.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_ready.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_error.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackTransparent.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackgroundWVGA_NoLogo.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Class.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/apt.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_pressed.bminfo 

BMGRP_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmAcknowledgeState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmBypassState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmEvent.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Borders.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/GlobalArea.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Pads.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/msgBox.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Line_Atr.bmgrp \
	$(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/CurtiusPads.bmgrp 

PAGE_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p1_RUN.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p10_MainDrillDown.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p6_PARAM_Drill.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p2_SEGM_Editor.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p3_LogBook.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p5_GLOBAL_Settings.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p20_ErrorsPage.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p30_Edtor.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p4_APPLIC_Settings.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p7_Diagnostic.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p8_Errors.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p11_Calibration.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p9_Alt_RUN.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p12_FilesOper.page \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p13_Main.page 

LAYER_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/globalArea.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/msgBox.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Background.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/PopUp_box.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/ClickLayer.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Editor.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown1.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown2.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/globalJogBtns.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Main_StartStop_Btns.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/SingleEditor.layer \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Layers/SingleEnumEditor.layer 

VCS_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/StyleSheets/Default.vcs \
	$(AS_PROJECT_PATH)/Logical/Visu_M/StyleSheets/MenuItem.vcs 

BDR_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Decrease.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Decrease_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Global_Area.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Global_Area_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Increase.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Increase_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Down.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Down_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Up.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Up_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Radio.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Radio_selected.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scoll_Up.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scoll_Up_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Down.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Down_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Left.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Left_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Right.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Right_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_pressed.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenNG.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/CheckBox_checked.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Flat_black.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Flat_grey.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameHeader.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/OverdriveBorder.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/ProgressBarBorder.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/RaisedInner.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Raised.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SliderBorder09.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenOuter.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Sunken.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenNGgray.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameGlobal.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameInvisible.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_KeyRingOff.bdr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_KeyRingOn.bdr 

TPR_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/AlphaPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NavigationPad_ver.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NavigationPad_hor.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/EditPad.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPad_Limits.tpr \
	$(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPadBig.tpr 

TDC_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Trends/TrendData.tdc 

TRD_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Trends/CPUTemperature.trd \
	$(AS_PROJECT_PATH)/Logical/Visu_M/Trends/ROOMTemperature.trd 

TRE_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Trends/Trend_Temperature.tre 

CLM_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/ColorOn_Grey.clm \
	$(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/Line_Select.clm \
	$(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/ColorAtr.clm 

VCVK_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/VirtualKeys.vcvk 

VCR_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Palette.vcr 

# Runtime Object sources
VCR_OBJECT_Visu_M=$(TEMP_PATH_Visu_M)/vcrt.vco
VCR_SOURCE_Visu_M=$(SRC_PATH_Visu_M)/package.vcp
# All Source Objects END

#Panel Hardware
$(PANEL_HW_VCI_Visu_M): $(PANEL_HW_SOURCE_Visu_M) $(VC_LIBRARY_LIST_Visu_M) $(KEYMAP_SOURCES_Visu_M) $(PALFILE_Visu_M)
	$(VCHWPP) -f '$<' -o '$@' -n Visu_M -d Visu_M -pal '$(PALFILE_Visu_M)' -c '$(AS_CONFIGURATION)' -p '$(AS_PLC)' -ptemp '$(AS_TEMP_PLC)' -B 'J4.93' -L 'AsRfbExt: V*, Visapi: V*' -verbose 'False' -profile 'False' -hw '$(CPUHWC)' -warninglevel 2 -so $(VC_STATIC_OPTIONS_Visu_M) -sos $(VC_STATIC_OPTIONS_Shared) -fp '$(AS_VC_PATH)/Firmware/V4.73.1/SG4' -sfas -prj '$(AS_PROJECT_PATH)' -apj 'UnivCntrl' -vcob '$(VCOBJECT_Visu_M)'

$(PANEL_HW_OBJECT_Visu_M): $(PANEL_HW_VCI_Visu_M) $(VC_LIBRARY_LIST_Visu_M)
	$(VCC) -f '$(subst .vco,.vci,$@)' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -k '$(VCVK_SOURCES_Visu_M)' $(VCCFLAGS_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


# Pages
PAGE_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/page., $(notdir $(PAGE_SOURCES_Visu_M:.page=.vco)))

$(TEMP_PATH_Visu_M)/page.p1_RUN.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p1_RUN.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p10_MainDrillDown.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p10_MainDrillDown.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p6_PARAM_Drill.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p6_PARAM_Drill.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p2_SEGM_Editor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p2_SEGM_Editor.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p3_LogBook.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p3_LogBook.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p5_GLOBAL_Settings.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p5_GLOBAL_Settings.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p20_ErrorsPage.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p20_ErrorsPage.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p30_Edtor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p30_Edtor.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p4_APPLIC_Settings.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p4_APPLIC_Settings.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p7_Diagnostic.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p7_Diagnostic.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p8_Errors.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p8_Errors.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p11_Calibration.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p11_Calibration.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p9_Alt_RUN.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p9_Alt_RUN.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p12_FilesOper.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p12_FilesOper.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/page.p13_Main.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Pages/p13_Main.page $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu_M)/StyleSheets/Default.vcs' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Pages END




# Stylesheets
VCS_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/vcs., $(notdir $(VCS_SOURCES_Visu_M:.vcs=.vco)))

$(TEMP_PATH_Visu_M)/vcs.Default.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/StyleSheets/Default.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/vcs.MenuItem.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/StyleSheets/MenuItem.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Stylesheets END




# Layers
LAYER_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/layer., $(notdir $(LAYER_SOURCES_Visu_M:.layer=.vco)))

$(TEMP_PATH_Visu_M)/layer.globalArea.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/globalArea.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.msgBox.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/msgBox.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.Background.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Background.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.PopUp_box.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/PopUp_box.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.DrillDown.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.ClickLayer.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/ClickLayer.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.Editor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Editor.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.DrillDown1.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown1.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.DrillDown2.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/DrillDown2.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.globalJogBtns.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/globalJogBtns.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.Main_StartStop_Btns.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/Main_StartStop_Btns.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.SingleEditor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/SingleEditor.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/layer.SingleEnumEditor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Layers/SingleEnumEditor.layer $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -ds $(DEFAULT_STYLE_SHEET_Visu_M) -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Layers END




# Virtual Keys
VCVK_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/vcvk., $(notdir $(VCVK_SOURCES_Visu_M:.vcvk=.vco)))

$(TEMP_PATH_Visu_M)/vcvk.VirtualKeys.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/VirtualKeys.vcvk
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas

$(VCVK_OBJECTS_Visu_M): $(VC_LANGUAGES_Visu_M)

#Virtual Keys END




# Touch Pads
TPR_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/tpr., $(notdir $(TPR_SOURCES_Visu_M:.tpr=.vco)))

$(TEMP_PATH_Visu_M)/tpr.NumPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/AlphaPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.NavigationPad_ver.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NavigationPad_ver.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.NavigationPad_hor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NavigationPad_hor.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.EditPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/EditPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.NumPad_Limits.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPad_Limits.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/tpr.NumPadBig.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TouchPads/NumPadBig.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M) -prj 'C:/projects/UnivCntrl_NewPendant/UnivCntrl_NewPendant/Logical/Visu_M' -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Touch Pads END




# Text Groups
TXTGRP_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/txtgrp., $(notdir $(TXTGRP_SOURCES_Visu_M:.txtgrp=.vco)))

$(TEMP_PATH_Visu_M)/txtgrp.InstantMessages.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/InstantMessages.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.HeaderBar.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/HeaderBar.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.PageNames.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/PageNames.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.Buttons_PageTexts.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/Buttons_PageTexts.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.Languages.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/Languages.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.NumPad_Limits.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/NumPad_Limits.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/txtgrp.httpURL_SDM.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/TextGroups/httpURL_SDM.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Text Groups END




# BitmapGroups
BMGRP_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/bmgrp., $(notdir $(BMGRP_SOURCES_Visu_M:.bmgrp=.vco)))

$(TEMP_PATH_Visu_M)/bmgrp.AlarmAcknowledgeState.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmAcknowledgeState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.AlarmBypassState.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmBypassState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.AlarmEvent.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmEvent.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.AlarmState.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/AlarmState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.Borders.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Borders.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.GlobalArea.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/GlobalArea.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.Pads.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Pads.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.msgBox.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/msgBox.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.Line_Atr.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/Line_Atr.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bmgrp.CurtiusPads.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/BitmapGroups/CurtiusPads.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#BitmapGroups END




# Bitmaps
BMINFO_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/bminfo., $(notdir $(BMINFO_SOURCES_Visu_M:.bminfo=.vco)))

$(TEMP_PATH_Visu_M)/bminfo.Key_NumPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_NumPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPad.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_AcknowledgeReset.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_AcknowledgeReset.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_AcknowledgeReset.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Active.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Active.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Active.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_BypassOFF.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassOFF.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassOFF.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_BypassON.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassON.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_BypassON.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Inactive.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Inactive.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Inactive.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Latched.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Latched.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Latched.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_NotQuit.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_NotQuit.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_NotQuit.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Quit.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Quit.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Quit.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Reset.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Reset.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Reset.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_ResetAcknowledge.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_ResetAcknowledge.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_ResetAcknowledge.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Alarm_Triggered.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Triggered.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Alarm_Triggered.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.ProgressBorder.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/ProgressBorder.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/ProgressBorder.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.alarm.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/alarm.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/alarm.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.checkbox.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.checkbox_checked.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_checked.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_checked.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_default.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_default_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_default_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_down.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_down_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_global_area.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_global_area_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_global_area_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.information.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/information.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/information.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_left.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_left_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_left_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_down_multi.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_down_multi_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_down_multi_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_up_multi.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_up_multi_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_multi_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_radio_selected.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio_selected.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio_selected.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_radio.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_radio.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_right.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_right_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_right_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_up.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_scroll_up_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_scroll_up_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.warning.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/warning.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/warning.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_decrease_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_increase.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_increase_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_increase_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_decrease.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_decrease.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.frame_header.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/frame_header.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/frame_header.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.checkbox_small.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.checkbox_small_checked.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_checked.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_checked.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_AlphaPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_AlphaPad_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Slider_09x09.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_09x09.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_09x09.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Slider_ArrowRightGray.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowRightGray.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowRightGray.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Slider_ArrowUpGray.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowUpGray.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_ArrowUpGray.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Slider_BallGray.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_BallGray.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Slider_BallGray.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_ListPadVer.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_ListPadHor_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_ListPadHor.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadHor.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_ListPadVer_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_ListPadVer_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.gauge_200x200_round_nodiv.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_200x200_round_nodiv.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_200x200_round_nodiv.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.gauge_NeedleRed100x11_1.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_NeedleRed100x11_1.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/gauge_NeedleRed100x11_1.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.checkbox_small_gray.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_gray.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/checkbox_small_gray.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_EditPad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_EditPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_EditPad_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.FrameInvisible.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/FrameInvisible.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/FrameInvisible.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_off.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_off.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_off.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_on.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_on.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_on.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_ready.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_ready.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_ready.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.button_error.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_error.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/button_error.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.BackTransparent.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackTransparent.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackTransparent.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.BackgroundWVGA_NoLogo.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackgroundWVGA_NoLogo.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/BackgroundWVGA_NoLogo.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_NumPadLimits.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Key_NumPadLimits_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Key_NumPadLimits_pressed.png
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Class.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Class.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Class.ico
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.apt.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/apt.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/apt.ico
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Curtius_Key_Numpad.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Curtius_Key_Numpad_Big.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Curtius_Key_Numpad_Big_Pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_Big_Pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/bminfo.Curtius_Key_Numpad_pressed.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/Visu_M/Bitmaps/Curtius_Key_Numpad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Bitmaps END




# Trend Configuration
TRE_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/tre., $(notdir $(TRE_SOURCES_Visu_M:.tre=.vco)))

$(TEMP_PATH_Visu_M)/tre.Trend_Temperature.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Trends/Trend_Temperature.tre
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Trend Configuration END




# Trend Data
TRD_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/trd., $(notdir $(TRD_SOURCES_Visu_M:.trd=.vco)))

$(TEMP_PATH_Visu_M)/trd.CPUTemperature.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Trends/CPUTemperature.trd
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/trd.ROOMTemperature.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Trends/ROOMTemperature.trd
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Trend Data END




# Trend Data Configuration
TDC_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/tdc., $(notdir $(TDC_SOURCES_Visu_M:.tdc=.vco)))

$(TEMP_PATH_Visu_M)/tdc.TrendData.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/Trends/TrendData.tdc
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#Trend Data Configuration END




# ColorMap Table
CLM_OBJECTS_Visu_M = $(addprefix $(TEMP_PATH_Visu_M)/clm., $(notdir $(CLM_SOURCES_Visu_M:.clm=.vco)))

$(TEMP_PATH_Visu_M)/clm.ColorOn_Grey.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/ColorOn_Grey.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/clm.Line_Select.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/Line_Select.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


$(TEMP_PATH_Visu_M)/clm.ColorAtr.vco: $(AS_PROJECT_PATH)/Logical/Visu_M/ColorMaps/ColorAtr.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas


#ColorMap Table END


#
# Borders
#
BDR_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Decrease.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Decrease_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Global_Area.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Global_Area_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Increase.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Increase_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Down.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Down_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Up.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Multi_Scroll_Up_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Radio.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Radio_selected.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scoll_Up.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scoll_Up_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Down.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Down_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Left.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Left_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Right.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_Scroll_Right_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_pressed.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenNG.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/CheckBox_checked.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Flat_black.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Flat_grey.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameHeader.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/OverdriveBorder.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/ProgressBarBorder.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/RaisedInner.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Raised.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SliderBorder09.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenOuter.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Sunken.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/SunkenNGgray.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameGlobal.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/FrameInvisible.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_KeyRingOff.bdr $(AS_PROJECT_PATH)/Logical/Visu_M/Borders/Button_KeyRingOn.bdr 
BDR_OBJECTS_Visu_M=$(TEMP_PATH_Visu_M)/bdr.Bordermanager.vco
$(TEMP_PATH_Visu_M)/bdr.Bordermanager.vco: $(BDR_SOURCES_Visu_M)
	$(VCC) -f '$<' -o '$@' -pkg '$(SRC_PATH_Visu_M)' $(BDRFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M$(SRC_PATH_Visu_M)
#
# Logical fonts
#
$(TEMP_PATH_Visu_M)/lfnt.sv.vco: $(TEMP_PATH_Visu_M)/sv.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.fr.vco: $(TEMP_PATH_Visu_M)/fr.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.zh-CN.vco: $(TEMP_PATH_Visu_M)/zh-CN.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.de.vco: $(TEMP_PATH_Visu_M)/de.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.pt-BR.vco: $(TEMP_PATH_Visu_M)/pt-BR.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.es.vco: $(TEMP_PATH_Visu_M)/es.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.en.vco: $(TEMP_PATH_Visu_M)/en.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
$(TEMP_PATH_Visu_M)/lfnt.pt.vco: $(TEMP_PATH_Visu_M)/pt.lfnt $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -p Visu_M -sfas
LFNT_OBJECTS_Visu_M=$(TEMP_PATH_Visu_M)/lfnt.sv.vco $(TEMP_PATH_Visu_M)/lfnt.fr.vco $(TEMP_PATH_Visu_M)/lfnt.zh-CN.vco $(TEMP_PATH_Visu_M)/lfnt.de.vco $(TEMP_PATH_Visu_M)/lfnt.pt-BR.vco $(TEMP_PATH_Visu_M)/lfnt.es.vco $(TEMP_PATH_Visu_M)/lfnt.en.vco $(TEMP_PATH_Visu_M)/lfnt.pt.vco 

#Runtime Object
$(VCR_OBJECT_Visu_M) : $(VCR_SOURCE_Visu_M)
	$(VCC) -f '$<' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -sl en $(VCCFLAGS_Visu_M) -rt  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4731 -sfas
# Local resources Library rules
LIB_LOCAL_RES_Visu_M=$(TEMP_PATH_Visu_M)/localres.vca
$(LIB_LOCAL_RES_Visu_M) : $(TEMP_PATH_Visu_M)/Visu_M02.ccf

# Bitmap Library rules
LIB_BMP_RES_Visu_M=$(TEMP_PATH_Visu_M)/bmpres.vca
$(LIB_BMP_RES_Visu_M) : $(TEMP_PATH_Visu_M)/Visu_M03.ccf
$(BMGRP_OBJECTS_Visu_M) : $(PALFILE_Visu_M) $(VC_LANGUAGES_Visu_M)
$(BMINFO_OBJECTS_Visu_M) : $(PALFILE_Visu_M)

BUILD_FILE_Visu_M=$(TEMP_PATH_Visu_M)/BuildFiles.arg
$(BUILD_FILE_Visu_M) : BUILD_FILE_CLEAN_Visu_M $(BUILD_SOURCES_Visu_M)
BUILD_FILE_CLEAN_Visu_M:
	$(RM) /F /Q '$(BUILD_FILE_Visu_M)' 2>nul
#All Modules depending to this project
PROJECT_MODULES_Visu_M=$(AS_CPU_PATH)/Visu_M01.br $(AS_CPU_PATH)/Visu_M02.br $(AS_CPU_PATH)/Visu_M03.br $(FONT_MODULES_Visu_M) $(SHARED_MODULE)

# General Build rules

$(TARGET_FILE_Visu_M): $(PROJECT_MODULES_Visu_M) $(TEMP_PATH_Visu_M)/Visu_M.prj
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu_M) -fw '$(VCFIRMWAREPATH)' -m $(VCSTPOST) -v V1.00.0 -f '$(TEMP_PATH_Visu_M)/Visu_M.prj' -o '$@' -vc '$(VCOBJECT_Visu_M)' $(DEPENDENCIES_Visu_M) $(addprefix -d ,$(notdir $(PROJECT_MODULES_Visu_M:.br=)))

$(AS_CPU_PATH)/Visu_M01.br: $(TEMP_PATH_Visu_M)/Visu_M01.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu_M) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu_M)' -f '$<' -o '$@' $(DEPENDENCIES_Visu_M)

$(AS_CPU_PATH)/Visu_M02.br: $(TEMP_PATH_Visu_M)/Visu_M02.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu_M) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu_M)' -f '$<' -o '$@' $(DEPENDENCIES_Visu_M)

$(AS_CPU_PATH)/Visu_M03.br: $(TEMP_PATH_Visu_M)/Visu_M03.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu_M) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu_M)' -f '$<' -o '$@' $(DEPENDENCIES_Visu_M)

# General Build rules END
$(LIB_LOCAL_OBJ_Visu_M) : $(TEMP_PATH_Visu_M)/Visu_M01.ccf

# Main Module
$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu_M.vcm:
$(TEMP_PATH_Visu_M)/Visu_M.prj: $(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu_M.vcm
	$(VCDEP) -m '$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu_M.vcm' -s '$(AS_CPU_PATH)/VCShared/Shared.vcm' -p '$(AS_PATH)/AS/VC/Firmware' -c '$(AS_CPU_PATH)' -fw '$(VCFIRMWAREPATH)' -hw '$(CPUHWC)' -so $(VC_STATIC_OPTIONS_Visu_M) -o Visu_M -proj Visu_M
	$(VCPL) $(notdir $(PROJECT_MODULES_Visu_M:.br=,4)) Visu_M,2 -o '$@' -p Visu_M -vc 'Visu_M' -verbose 'False' -fl '$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu_M.vcm' -vcr '$(VCR_SOURCE_Visu_M)' -prj '$(AS_PROJECT_PATH)' -warningLevel2 -sfas

# 01 Module

DEL_TARGET01_LFL_Visu_M=$(TEMP_PATH_Visu_M)\Visu_M01.ccf.lfl
$(TEMP_PATH_Visu_M)/Visu_M01.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu_M) $(TEMP_PATH_Visu_M)/Visu_M03.ccf $(LIB_LOCAL_RES_Visu_M) $(TEMP_PATH_Visu_M)/Visu_M02.ccf $(DIS_OBJECTS_Visu_M) $(PAGE_OBJECTS_Visu_M) $(VCS_OBJECTS_Visu_M) $(VCVK_OBJECTS_Visu_M) $(VCRT_OBJECTS_Visu_M) $(TPR_OBJECTS_Visu_M) $(TXTGRP_OBJECTS_Visu_M) $(LAYER_OBJECTS_Visu_M) $(VCR_OBJECT_Visu_M) $(TDC_OBJECTS_Visu_M) $(TRD_OBJECTS_Visu_M) $(TRE_OBJECTS_Visu_M) $(PRC_OBJECTS_Visu_M) $(SCR_OBJECTS_Visu_M)
	-@CMD /Q /C if exist "$(DEL_TARGET01_LFL_Visu_M)" DEL /F /Q "$(DEL_TARGET01_LFL_Visu_M)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu_M)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_LOCAL_RES_Visu_M)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(DIS_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .page -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCS_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .vcvk -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCRT_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(TPR_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .txtgrp -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .layer -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCR_OBJECT_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .tdc -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .trd -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(TEMP_PATH_Visu_M)/tre.Trend_Temperature.vco' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(SCR_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu_M -lib '$(LIB_LOCAL_OBJ_Visu_M)' -P '$(AS_PROJECT_PATH)' -m 'local objects' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 01 Module END

# 02 Module

DEL_TARGET02_LFL_Visu_M=$(TEMP_PATH_Visu_M)\Visu_M02.ccf.lfl
$(TEMP_PATH_Visu_M)/Visu_M02.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu_M) $(TEMP_PATH_Visu_M)/Visu_M03.ccf $(BDR_OBJECTS_Visu_M) $(LFNT_OBJECTS_Visu_M) $(CLM_OBJECTS_Visu_M)
	-@CMD /Q /C if exist "$(DEL_TARGET02_LFL_Visu_M)" DEL /F /Q "$(DEL_TARGET02_LFL_Visu_M)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu_M)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(BDR_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LFNT_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(CLM_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu_M -lib '$(LIB_LOCAL_RES_Visu_M)' -P '$(AS_PROJECT_PATH)' -m 'local resources' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 02 Module END

# 03 Module

DEL_TARGET03_LFL_Visu_M=$(TEMP_PATH_Visu_M)\Visu_M03.ccf.lfl
$(TEMP_PATH_Visu_M)/Visu_M03.ccf: $(LIB_SHARED) $(SHARED_CCF) $(BMGRP_OBJECTS_Visu_M) $(BMINFO_OBJECTS_Visu_M) $(PALFILE_Visu_M)
	-@CMD /Q /C if exist "$(DEL_TARGET03_LFL_Visu_M)" DEL /F /Q "$(DEL_TARGET03_LFL_Visu_M)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bmgrp -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bminfo -vcp '$(AS_PROJECT_PATH)/Logical/Visu_M/Package.vcp' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu_M -lib '$(LIB_BMP_RES_Visu_M)' -P '$(AS_PROJECT_PATH)' -m 'bitmap resources' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 03 Module END

