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
TTFFLAGS_Visu_M= -P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo'

#
# Font arial
#
ifneq ($(VC_FONT_arial),1)
VC_FONT_arial=1
$(AS_CPU_PATH)/VcFntDat/arial.vco:$(AS_CPU_PATH)/VcFntDat/arial.vci
	 $(VCC) -f '$<' -o '$@' $(TTFFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -sfas

$(AS_CPU_PATH)/VcFntDat/arial.ccf:$(AS_CPU_PATH)/VcFntDat/arial.vco
	 $(LINK) '$^' -o '$@' -warningLevel2 -m arial.ttf -name Visu_M -profile 'False' -vcr 4731 -sfas

$(AS_CPU_PATH)/arial.br:$(AS_CPU_PATH)/VcFntDat/arial.ccf
	 $(MODGEN) -so $(VC_STATIC_OPTIONS_Shared) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -f '$<' -o '$@' -d vcgclass -v V1.00.0 -profile False -vc '$(VCOBJECT_Visu_M)' -b
endif

#
# Font arialbd
#
ifneq ($(VC_FONT_arialbd),1)
VC_FONT_arialbd=1
$(AS_CPU_PATH)/VcFntDat/arialbd.vco:$(AS_CPU_PATH)/VcFntDat/arialbd.vci
	 $(VCC) -f '$<' -o '$@' $(TTFFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -sfas

$(AS_CPU_PATH)/VcFntDat/arialbd.ccf:$(AS_CPU_PATH)/VcFntDat/arialbd.vco
	 $(LINK) '$^' -o '$@' -warningLevel2 -m arialbd.ttf -name Visu_M -profile 'False' -vcr 4731 -sfas

$(AS_CPU_PATH)/arialbd.br:$(AS_CPU_PATH)/VcFntDat/arialbd.ccf
	 $(MODGEN) -so $(VC_STATIC_OPTIONS_Shared) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -f '$<' -o '$@' -d vcgclass -v V1.00.0 -profile False -vc '$(VCOBJECT_Visu_M)' -b
endif

#
# Font arialbi
#
ifneq ($(VC_FONT_arialbi),1)
VC_FONT_arialbi=1
$(AS_CPU_PATH)/VcFntDat/arialbi.vco:$(AS_CPU_PATH)/VcFntDat/arialbi.vci
	 $(VCC) -f '$<' -o '$@' $(TTFFLAGS_Visu_M) $(VCCFLAGS_Visu_M) -sfas

$(AS_CPU_PATH)/VcFntDat/arialbi.ccf:$(AS_CPU_PATH)/VcFntDat/arialbi.vco
	 $(LINK) '$^' -o '$@' -warningLevel2 -m arialbi.ttf -name Visu_M -profile 'False' -vcr 4731 -sfas

$(AS_CPU_PATH)/arialbi.br:$(AS_CPU_PATH)/VcFntDat/arialbi.ccf
	 $(MODGEN) -so $(VC_STATIC_OPTIONS_Shared) -fw '$(VCFIRMWAREPATH)' -m $(VCLOD) -f '$<' -o '$@' -d vcgclass -v V1.00.0 -profile False -vc '$(VCOBJECT_Visu_M)' -b
endif

FONT_MODULES_Visu_M=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/arial.br $(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/arialbd.br $(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/arialbi.br 
