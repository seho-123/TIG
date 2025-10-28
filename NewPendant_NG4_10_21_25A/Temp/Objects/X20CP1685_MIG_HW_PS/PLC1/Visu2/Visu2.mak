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
PALFILE_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Palette.vcr
VCCFLAGS_Visu2=-server -proj Visu2 -vc '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/VCObject.vc' -prj_path '$(AS_PROJECT_PATH)' -temp_path '$(AS_TEMP_PATH)' -cfg $(AS_CONFIGURATION) -plc $(AS_PLC) -plctemp $(AS_TEMP_PLC) -cpu_path '$(AS_CPU_PATH)'
VCFIRMWARE=4.73.1
VCFIRMWAREPATH=$(AS_VC_PATH)/Firmware/V4.73.1/SG4
VCOBJECT_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/VCObject.vc
VCSTARTUP='vcstart.br'
VCLOD='vclod.br'
VCSTPOST='vcstpost.br'
TARGET_FILE_Visu2=$(AS_CPU_PATH)/Visu2.br
OBJ_SCOPE_Visu2=FourAxisTigCtrl
PRJ_PATH_Visu2=$(AS_PROJECT_PATH)
SRC_PATH_Visu2=$(AS_PROJECT_PATH)/Logical/$(OBJ_SCOPE_Visu2)/Visu2
TEMP_PATH_Visu2=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu2
TEMP_PATH_Shared=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared
TEMP_PATH_ROOT_Visu2=$(AS_TEMP_PATH)
VC_LIBRARY_LIST_Visu2=$(TEMP_PATH_Visu2)/libraries.vci
VC_XREF_BUILDFILE_Visu2=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.build
VC_XREF_CLEANFILE=$(AS_TEMP_PATH)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcxref.clean
VC_LANGUAGES_Visu2=$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr
CPUHWC='$(TEMP_PATH_Visu2)/cpuhwc.vci'
VC_STATIC_OPTIONS_Visu2='$(TEMP_PATH_Visu2)/vcStaticOptions.xml'
VC_STATIC_OPTIONS_Shared='$(TEMP_PATH_Shared)/vcStaticOptions.xml'
# include Shared and Font Makefile (only once)
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCFntDat/Font_Visu2.mak
ifneq ($(VCINC),1)
	VCINC=1
	include $(AS_TEMP_PATH)/objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/VCShared.mak
endif

DEPENDENCIES_Visu2=-d vcgclass -profile 'False'
DEFAULT_STYLE_SHEET_Visu2='Source[local].StyleSheet[Color]'
SHARED_MODULE=$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcshared.br
LFNTFLAGS_Visu2=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)'
BDRFLAGS_Visu2=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)'

# Local Libs
LIB_LOCAL_OBJ_Visu2=$(TEMP_PATH_Visu2)/localobj.vca

# Hardware sources
PANEL_HW_OBJECT_Visu2=$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu2/dis.Hardware.vco
PANEL_HW_VCI_Visu2=$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/Visu2/dis.Hardware.vci
PANEL_HW_SOURCE_Visu2=C:/projects/UnivCntrl/UniversalController/Physical/X20CP1685_MIG_HW_PS/Hardware.hw 
DIS_OBJECTS_Visu2=$(PANEL_HW_OBJECT_Visu2) $(KEYMAP_OBJECTS_Visu2)

# KeyMapping flags
$(TEMP_PATH_Visu2)/dis.PS2-Keyboard1.vco: $(AS_PROJECT_PATH)/Physical/X20CP1685_MIG_HW_PS/PLC1/VC/PS2-Keyboard1.dis $(PANEL_HW_SOURCE_Visu2)
	$(VCHWPP) -f '$(PANEL_HW_SOURCE_Visu2)' -o '$(subst .vco,.vci,$(TEMP_PATH_Visu2)/dis.PS2-Keyboard1.vco)' -n Visu2 -d Visu2 -pal '$(PALFILE_Visu2)' -c '$(AS_CONFIGURATION)' -p '$(AS_PLC)' -ptemp '$(AS_TEMP_PLC)' -B 'J4.93' -L 'AsRfbExt: V*, Visapi: V*' -hw '$(CPUHWC)' -warninglevel 2 -so $(VC_STATIC_OPTIONS_Visu2) -sos $(VC_STATIC_OPTIONS_Shared) -keyboard '$(AS_PROJECT_PATH)/Physical/X20CP1685_MIG_HW_PS/PLC1/VC/PS2-Keyboard1.dis' -fp '$(AS_VC_PATH)/Firmware/V4.73.1/SG4' -prj '$(AS_PROJECT_PATH)' -apj 'UnivCntrl' -sfas -vcob '$(VCOBJECT_Visu2)'
	$(VCC) -f '$(subst .vco,.vci,$@)' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -k '$(VCVK_SOURCES_Visu2)' $(VCCFLAGS_Visu2) -p Visu2 -sfas

KEYMAP_SOURCES_Visu2=$(AS_PROJECT_PATH)/Physical/X20CP1685_MIG_HW_PS/PLC1/VC/PS2-Keyboard1.dis 
KEYMAP_OBJECTS_Visu2=$(TEMP_PATH_Visu2)/dis.PS2-Keyboard1.vco 

# All Source Objects
TXTGRP_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TextGroups/Head.txtgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TextGroups/WRST_Name.txtgrp 

FNINFO_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/DefaultFont.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial9px.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial9pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial10pxBold.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial12px.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial9pxValue.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial24Bold.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial36Bold.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial48Bold.fninfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Fonts/Arial72Bold.fninfo 

BMINFO_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AcknowledgeReset.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmActive.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassOFF.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassON.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmInactive.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmLatched.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmNotQuit.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmQuit.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Reset.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ResetAcknowledge.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Triggered.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_active.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ProgressBorder.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_upside.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_downside.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/frame_header.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed_control.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA2_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA3_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditpadQVGA2_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadQVGA3_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_released.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_pressed.bminfo \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_pressed.bminfo 

BMGRP_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlphaPadQVGA.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/NumPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlphaPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlarmEvent.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlarmState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/BypassState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AcknowledgeState.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/NavigationPad.bmgrp \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/Borders.bmgrp 

PAGE_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Pages/Init_Page.page 

VCS_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/StyleSheets/Gray.vcs \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/StyleSheets/Color.vcs 

BDR_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Raised.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Sunken.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Etched.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Bump.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/SunkenOuter.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/RaisedInner.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Flat_black.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Flat_grey.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/BackwardActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/BackwardPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ControlActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ControlPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/DownActiveControl.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/DownPressedControl.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ForwardActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ForwardPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/GlobalAreaActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/GlobalAreaPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollDownActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollDownPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollUpActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollUpPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ProgressBarBorder.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollDownActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollDownPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollUpActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollUpPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollLeftActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollLeftPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollRightActive.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollRightPressed.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/UpActiveControl.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/UpPressedControl.bdr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/FrameHeader.bdr 

TPR_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NumPad.tpr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/AlphaPadQVGA.tpr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/AlphaPad.tpr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NavigationPad_ver.tpr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NavigationPad_hor.tpr \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/EditPad.tpr 

TDC_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Trends/TrendData.tdc 

CLM_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/ColorMaps/LED1.clm 

VCVK_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/VirtualKeys.vcvk 

VCR_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Palette.vcr 

# Runtime Object sources
VCR_OBJECT_Visu2=$(TEMP_PATH_Visu2)/vcrt.vco
VCR_SOURCE_Visu2=$(SRC_PATH_Visu2)/package.vcp
# All Source Objects END

#Panel Hardware
$(PANEL_HW_VCI_Visu2): $(PANEL_HW_SOURCE_Visu2) $(VC_LIBRARY_LIST_Visu2) $(KEYMAP_SOURCES_Visu2) $(PALFILE_Visu2)
	$(VCHWPP) -f '$<' -o '$@' -n Visu2 -d Visu2 -pal '$(PALFILE_Visu2)' -c '$(AS_CONFIGURATION)' -p '$(AS_PLC)' -ptemp '$(AS_TEMP_PLC)' -B 'J4.93' -L 'AsRfbExt: V*, Visapi: V*' -verbose 'False' -profile 'False' -hw '$(CPUHWC)' -warninglevel 2 -so $(VC_STATIC_OPTIONS_Visu2) -sos $(VC_STATIC_OPTIONS_Shared) -fp '$(AS_VC_PATH)/Firmware/V4.73.1/SG4' -sfas -prj '$(AS_PROJECT_PATH)' -apj 'UnivCntrl' -vcob '$(VCOBJECT_Visu2)'

$(PANEL_HW_OBJECT_Visu2): $(PANEL_HW_VCI_Visu2) $(VC_LIBRARY_LIST_Visu2)
	$(VCC) -f '$(subst .vco,.vci,$@)' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -k '$(VCVK_SOURCES_Visu2)' $(VCCFLAGS_Visu2) -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


# Pages
PAGE_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/page., $(notdir $(PAGE_SOURCES_Visu2:.page=.vco)))

$(TEMP_PATH_Visu2)/page.Init_Page.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Pages/Init_Page.page $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -P '$(AS_PROJECT_PATH)' -ds '$(SRC_PATH_Visu2)/StyleSheets/Color.vcs' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Pages END




# Stylesheets
VCS_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/vcs., $(notdir $(VCS_SOURCES_Visu2:.vcs=.vco)))

$(TEMP_PATH_Visu2)/vcs.Gray.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/StyleSheets/Gray.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu2) -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/vcs.Color.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/StyleSheets/Color.vcs
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -P '$(AS_PROJECT_PATH)' -ds $(DEFAULT_STYLE_SHEET_Visu2) -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Stylesheets END




# Virtual Keys
VCVK_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/vcvk., $(notdir $(VCVK_SOURCES_Visu2:.vcvk=.vco)))

$(TEMP_PATH_Visu2)/vcvk.VirtualKeys.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/VirtualKeys.vcvk
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas

$(VCVK_OBJECTS_Visu2): $(VC_LANGUAGES_Visu2)

#Virtual Keys END




# Touch Pads
TPR_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/tpr., $(notdir $(TPR_SOURCES_Visu2:.tpr=.vco)))

$(TEMP_PATH_Visu2)/tpr.NumPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NumPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/tpr.AlphaPadQVGA.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/AlphaPadQVGA.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/tpr.AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/AlphaPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/tpr.NavigationPad_ver.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NavigationPad_ver.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/tpr.NavigationPad_hor.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/NavigationPad_hor.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/tpr.EditPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TouchPads/EditPad.tpr
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2) -prj 'C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/Visu2' -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Touch Pads END




# Text Groups
TXTGRP_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/txtgrp., $(notdir $(TXTGRP_SOURCES_Visu2:.txtgrp=.vco)))

$(TEMP_PATH_Visu2)/txtgrp.Head.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TextGroups/Head.txtgrp $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/txtgrp.WRST_Name.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/TextGroups/WRST_Name.txtgrp $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Text Groups END




# BitmapGroups
BMGRP_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/bmgrp., $(notdir $(BMGRP_SOURCES_Visu2:.bmgrp=.vco)))

$(TEMP_PATH_Visu2)/bmgrp.AlphaPadQVGA.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlphaPadQVGA.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.NumPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/NumPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.AlphaPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlphaPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.AlarmEvent.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlarmEvent.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.AlarmState.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AlarmState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.BypassState.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/BypassState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.AcknowledgeState.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/AcknowledgeState.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.NavigationPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/NavigationPad.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bmgrp.Borders.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/BitmapGroups/Borders.bmgrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#BitmapGroups END




# Bitmaps
BMINFO_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/bminfo., $(notdir $(BMINFO_SOURCES_Visu2:.bminfo=.vco)))

$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA1.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA2.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA3.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadVGA_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA2_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA2_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA3_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA3_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadQVGA1_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadQVGA1_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlphaPadVGA.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlphaPadVGA.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.NumPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.NumPad.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/NumPad.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AcknowledgeReset.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AcknowledgeReset.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AcknowledgeReset.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmActive.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmActive.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmActive.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmBypassOFF.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassOFF.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassOFF.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmBypassON.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassON.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmBypassON.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmInactive.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmInactive.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmInactive.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmLatched.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmLatched.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmLatched.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmNotQuit.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmNotQuit.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmNotQuit.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.AlarmQuit.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmQuit.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/AlarmQuit.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.Reset.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Reset.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Reset.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ResetAcknowledge.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ResetAcknowledge.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ResetAcknowledge.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.Triggered.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Triggered.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/Triggered.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ListPadHor.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ListPadHor_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadHor_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ListPadVer.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ListPadVer_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ListPadVer_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.backward_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.backward_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/backward_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.global_area_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.global_area_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.forward_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.forward_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/forward_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.control_button_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.control_button_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/control_button_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.right_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.right_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/right_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.left_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.left_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/left_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.up_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.up_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.down_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.down_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.multi_up_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.multi_up_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_up_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.multi_down_active.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_active.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_active.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.multi_down_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/multi_down_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.ProgressBorder.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ProgressBorder.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/ProgressBorder.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.down_active_control.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active_control.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_active_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.down_pressed_control.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed_control.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/down_pressed_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.up_active_control.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active_control.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_active_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.global_area_gradient_upside.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_upside.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_upside.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.global_area_gradient_downside.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_downside.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/global_area_gradient_downside.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.frame_header.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/frame_header.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/frame_header.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.up_pressed_control.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed_control.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/up_pressed_control.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.EditPadVGA.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.EditPadVGA_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/EditPadVGA_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneNumPad_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneListPadVer_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPad_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPad_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPadQVGA1_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPadQVGA1_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA1_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPadQVGA2_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA2_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA2_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneAlphaPadQVGA3_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA3_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneAlphaPadQVGA3_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneEditpadQVGA2_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditpadQVGA2_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditpadQVGA2_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneEditPadQVGA3_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadQVGA3_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadQVGA3_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneEditPadVga_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneEditPadVga_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneEditPadVga_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneListPadHor_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneListPadHor_released.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_released.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadHor_released.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneListPadVer_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneListPadVer_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


$(TEMP_PATH_Visu2)/bminfo.zuneNumPad_pressed.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_pressed.bminfo $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Bitmaps/zuneNumPad_pressed.bmp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Bitmaps END




# Trend Data Configuration
TDC_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/tdc., $(notdir $(TDC_SOURCES_Visu2:.tdc=.vco)))

$(TEMP_PATH_Visu2)/tdc.TrendData.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Trends/TrendData.tdc
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#Trend Data Configuration END




# ColorMap Table
CLM_OBJECTS_Visu2 = $(addprefix $(TEMP_PATH_Visu2)/clm., $(notdir $(CLM_SOURCES_Visu2:.clm=.vco)))

$(TEMP_PATH_Visu2)/clm.LED1.vco: $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/ColorMaps/LED1.clm
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu2)' $(VCCFLAGS_Visu2)  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas


#ColorMap Table END


#
# Borders
#
BDR_SOURCES_Visu2=$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Raised.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Sunken.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Etched.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Bump.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/SunkenOuter.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/RaisedInner.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Flat_black.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/Flat_grey.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/BackwardActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/BackwardPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ControlActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ControlPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/DownActiveControl.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/DownPressedControl.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ForwardActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ForwardPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/GlobalAreaActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/GlobalAreaPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollDownActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollDownPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollUpActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/MultiScrollUpPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ProgressBarBorder.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollDownActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollDownPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollUpActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollUpPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollLeftActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollLeftPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollRightActive.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/ScrollRightPressed.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/UpActiveControl.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/UpPressedControl.bdr $(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Borders/FrameHeader.bdr 
BDR_OBJECTS_Visu2=$(TEMP_PATH_Visu2)/bdr.Bordermanager.vco
$(TEMP_PATH_Visu2)/bdr.Bordermanager.vco: $(BDR_SOURCES_Visu2)
	$(VCC) -f '$<' -o '$@' -pkg '$(SRC_PATH_Visu2)' $(BDRFLAGS_Visu2) $(VCCFLAGS_Visu2) -p Visu2$(SRC_PATH_Visu2)
#
# Logical fonts
#
$(TEMP_PATH_Visu2)/lfnt.de.vco: $(TEMP_PATH_Visu2)/de.lfnt $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu2) $(VCCFLAGS_Visu2) -p Visu2 -sfas
$(TEMP_PATH_Visu2)/lfnt.en.vco: $(TEMP_PATH_Visu2)/en.lfnt $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu2) $(VCCFLAGS_Visu2) -p Visu2 -sfas
$(TEMP_PATH_Visu2)/lfnt.es.vco: $(TEMP_PATH_Visu2)/es.lfnt $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu2) $(VCCFLAGS_Visu2) -p Visu2 -sfas
$(TEMP_PATH_Visu2)/lfnt.zh-CN.vco: $(TEMP_PATH_Visu2)/zh-CN.lfnt $(VC_LANGUAGES_Visu2)
	 $(VCC) -f '$<' -o '$@' $(LFNTFLAGS_Visu2) $(VCCFLAGS_Visu2) -p Visu2 -sfas
LFNT_OBJECTS_Visu2=$(TEMP_PATH_Visu2)/lfnt.de.vco $(TEMP_PATH_Visu2)/lfnt.en.vco $(TEMP_PATH_Visu2)/lfnt.es.vco $(TEMP_PATH_Visu2)/lfnt.zh-CN.vco 

#Runtime Object
$(VCR_OBJECT_Visu2) : $(VCR_SOURCE_Visu2)
	$(VCC) -f '$<' -o '$@' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -sl en $(VCCFLAGS_Visu2) -rt  -p Visu2 -so $(VC_STATIC_OPTIONS_Visu2) -vcr 4731 -sfas
# Local resources Library rules
LIB_LOCAL_RES_Visu2=$(TEMP_PATH_Visu2)/localres.vca
$(LIB_LOCAL_RES_Visu2) : $(TEMP_PATH_Visu2)/Visu202.ccf

# Bitmap Library rules
LIB_BMP_RES_Visu2=$(TEMP_PATH_Visu2)/bmpres.vca
$(LIB_BMP_RES_Visu2) : $(TEMP_PATH_Visu2)/Visu203.ccf
$(BMGRP_OBJECTS_Visu2) : $(PALFILE_Visu2) $(VC_LANGUAGES_Visu2)
$(BMINFO_OBJECTS_Visu2) : $(PALFILE_Visu2)

BUILD_FILE_Visu2=$(TEMP_PATH_Visu2)/BuildFiles.arg
$(BUILD_FILE_Visu2) : BUILD_FILE_CLEAN_Visu2 $(BUILD_SOURCES_Visu2)
BUILD_FILE_CLEAN_Visu2:
	$(RM) /F /Q '$(BUILD_FILE_Visu2)' 2>nul
#All Modules depending to this project
PROJECT_MODULES_Visu2=$(AS_CPU_PATH)/Visu201.br $(AS_CPU_PATH)/Visu202.br $(AS_CPU_PATH)/Visu203.br $(FONT_MODULES_Visu2) $(SHARED_MODULE)

# General Build rules

$(TARGET_FILE_Visu2): $(PROJECT_MODULES_Visu2) $(TEMP_PATH_Visu2)/Visu2.prj
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu2) -fw '$(VCFIRMWAREPATH)' -m $(VCSTPOST) -v V1.00.0 -f '$(TEMP_PATH_Visu2)/Visu2.prj' -o '$@' -vc '$(VCOBJECT_Visu2)' $(DEPENDENCIES_Visu2) $(addprefix -d ,$(notdir $(PROJECT_MODULES_Visu2:.br=)))

$(AS_CPU_PATH)/Visu201.br: $(TEMP_PATH_Visu2)/Visu201.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu2) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu2)' -f '$<' -o '$@' $(DEPENDENCIES_Visu2)

$(AS_CPU_PATH)/Visu202.br: $(TEMP_PATH_Visu2)/Visu202.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu2) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu2)' -f '$<' -o '$@' $(DEPENDENCIES_Visu2)

$(AS_CPU_PATH)/Visu203.br: $(TEMP_PATH_Visu2)/Visu203.ccf
	$(MODGEN) -so $(VC_STATIC_OPTIONS_Visu2) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -v V1.00.0 -b -vc '$(VCOBJECT_Visu2)' -f '$<' -o '$@' $(DEPENDENCIES_Visu2)

# General Build rules END
$(LIB_LOCAL_OBJ_Visu2) : $(TEMP_PATH_Visu2)/Visu201.ccf

# Main Module
$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu2.vcm:
$(TEMP_PATH_Visu2)/Visu2.prj: $(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu2.vcm
	$(VCDEP) -m '$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu2.vcm' -s '$(AS_CPU_PATH)/VCShared/Shared.vcm' -p '$(AS_PATH)/AS/VC/Firmware' -c '$(AS_CPU_PATH)' -fw '$(VCFIRMWAREPATH)' -hw '$(CPUHWC)' -so $(VC_STATIC_OPTIONS_Visu2) -o Visu2 -proj Visu2
	$(VCPL) $(notdir $(PROJECT_MODULES_Visu2:.br=,4)) Visu2,2 -o '$@' -p Visu2 -vc 'Visu2' -verbose 'False' -fl '$(TEMP_PATH_ROOT_Visu2)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/Visu2.vcm' -vcr '$(VCR_SOURCE_Visu2)' -prj '$(AS_PROJECT_PATH)' -warningLevel2 -sfas

# 01 Module

DEL_TARGET01_LFL_Visu2=$(TEMP_PATH_Visu2)\Visu201.ccf.lfl
$(TEMP_PATH_Visu2)/Visu201.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu2) $(TEMP_PATH_Visu2)/Visu203.ccf $(LIB_LOCAL_RES_Visu2) $(TEMP_PATH_Visu2)/Visu202.ccf $(DIS_OBJECTS_Visu2) $(PAGE_OBJECTS_Visu2) $(VCS_OBJECTS_Visu2) $(VCVK_OBJECTS_Visu2) $(VCRT_OBJECTS_Visu2) $(TPR_OBJECTS_Visu2) $(TXTGRP_OBJECTS_Visu2) $(LAYER_OBJECTS_Visu2) $(VCR_OBJECT_Visu2) $(TDC_OBJECTS_Visu2) $(TRD_OBJECTS_Visu2) $(TRE_OBJECTS_Visu2) $(PRC_OBJECTS_Visu2) $(SCR_OBJECTS_Visu2)
	-@CMD /Q /C if exist "$(DEL_TARGET01_LFL_Visu2)" DEL /F /Q "$(DEL_TARGET01_LFL_Visu2)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu2)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_LOCAL_RES_Visu2)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(DIS_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .page -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCS_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .vcvk -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCRT_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(TPR_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .txtgrp -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .layer -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(VCR_OBJECT_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .tdc -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .trd -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(SCR_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu2 -lib '$(LIB_LOCAL_OBJ_Visu2)' -P '$(AS_PROJECT_PATH)' -m 'local objects' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 01 Module END

# 02 Module

DEL_TARGET02_LFL_Visu2=$(TEMP_PATH_Visu2)\Visu202.ccf.lfl
$(TEMP_PATH_Visu2)/Visu202.ccf: $(LIB_SHARED) $(SHARED_CCF) $(LIB_BMP_RES_Visu2) $(TEMP_PATH_Visu2)/Visu203.ccf $(BDR_OBJECTS_Visu2) $(LFNT_OBJECTS_Visu2) $(CLM_OBJECTS_Visu2)
	-@CMD /Q /C if exist "$(DEL_TARGET02_LFL_Visu2)" DEL /F /Q "$(DEL_TARGET02_LFL_Visu2)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LIB_BMP_RES_Visu2)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(BDR_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(LFNT_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' '$(CLM_OBJECTS_Visu2:.vco=.vco|)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu2 -lib '$(LIB_LOCAL_RES_Visu2)' -P '$(AS_PROJECT_PATH)' -m 'local resources' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 02 Module END

# 03 Module

DEL_TARGET03_LFL_Visu2=$(TEMP_PATH_Visu2)\Visu203.ccf.lfl
$(TEMP_PATH_Visu2)/Visu203.ccf: $(LIB_SHARED) $(SHARED_CCF) $(BMGRP_OBJECTS_Visu2) $(BMINFO_OBJECTS_Visu2) $(PALFILE_Visu2)
	-@CMD /Q /C if exist "$(DEL_TARGET03_LFL_Visu2)" DEL /F /Q "$(DEL_TARGET03_LFL_Visu2)" 2>nul
	@$(VCFLGEN) '$@.lfl' '$(LIB_SHARED)' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bmgrp -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	@$(VCFLGEN) '$@.lfl' -mask .bminfo -vcp '$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/Visu2/Package.vcp' -temp '$(TEMP_PATH_Visu2)' -prj '$(PRJ_PATH_Visu2)' -sfas
	$(LINK) '$@.lfl' -o '$@' -p Visu2 -lib '$(LIB_BMP_RES_Visu2)' -P '$(AS_PROJECT_PATH)' -m 'bitmap resources' -profile 'False' -warningLevel2 -vcr 4731 -sfas
# 03 Module END

# Post Build Steps

.PHONY : vcPostBuild_Visu2

vcPostBuild_Visu2 :
	$(VCC) -pb -vcm '$(TEMP_PATH_Visu2)/MODULEFILES.vcm' -fw '$(VCFIRMWAREPATH)' $(VCCFLAGS_Visu2) -p Visu2 -vcr 4731 -sfas

# Post Build Steps END
