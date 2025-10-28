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
VCFIRMWARE=4.73.2
VCFIRMWAREPATH=$(AS_VC_PATH)/Firmware/V4.73.2/SG4
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

DSOFLAGS=-P '$(AS_PROJECT_PATH)' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)'
LIB_SHARED=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/vcshared.vca

#
# Shared Runtime Options
#
VCRS_OBJECT=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/vcrt_s.vco
VCRS_SOURCE=$(AS_PROJECT_PATH)/Logical/VCShared/Package.vcp

# All Shared Source Objects
VCR_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr 

TXTGRP_SHARED_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmAcknowledgeState.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmBypassState.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmEvent.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmState.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/DateTimeFormats.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/httpURL_SDM.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/DWN_UP.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/IN_OUT_DWN_UP.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/Indx_Mode.txtgrp \
	$(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/Torch_Weld.txtgrp 

VCUG_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Mass.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Volume.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Power.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Memory.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Pressure.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Temperatures.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Angle.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/LengthVar.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Accel.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Amps.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Frequency.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length1.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length2.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/RotAcc.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Rotation.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Time1.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Time2.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Velocity.vcug \
	$(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Volts.vcug 

ALCFG_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/AlarmGroups/AlarmSystem.alcfg 

ALGRP_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/AlarmGroups/SystemAlarms.algrp 

DSO_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/DataSources/Internal.dso \
	$(AS_PROJECT_PATH)/Logical/VCShared/DataSources/DataSource.dso 

CVINFO_SOURCES_Visu_M=$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo 



# UnitGroups
VCUG_OBJECTS_Visu_M = $(addprefix $(AS_CPU_PATH)/VCShared/vcug., $(notdir $(VCUG_SOURCES_Visu_M:.vcug=.vco)))

$(AS_CPU_PATH)/VCShared/vcug.Length.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Mass.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Mass.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Volume.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Volume.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Power.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Power.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Memory.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Memory.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Pressure.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Pressure.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Temperatures.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Temperatures.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Angle.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Angle.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.LengthVar.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/LengthVar.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Accel.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Accel.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Amps.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Amps.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Frequency.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Frequency.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Length1.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length1.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Length2.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Length2.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.RotAcc.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/RotAcc.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Rotation.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Rotation.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Time1.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Time1.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Time2.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Time2.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Velocity.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Velocity.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/vcug.Volts.vco: $(AS_PROJECT_PATH)/Logical/VCShared/UnitGroups/Volts.vcug
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


#UnitGroups END




# AlarmGroups
ALGRP_OBJECTS_Visu_M = $(addprefix $(AS_CPU_PATH)/VCShared/algrp., $(notdir $(ALGRP_SOURCES_Visu_M:.algrp=.vco)))

$(AS_CPU_PATH)/VCShared/algrp.SystemAlarms.vco: $(AS_PROJECT_PATH)/Logical/VCShared/AlarmGroups/SystemAlarms.algrp
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


#AlarmGroups END




# AlarmSystem
ALCFG_OBJECTS_Visu_M = $(addprefix $(AS_CPU_PATH)/VCShared/alcfg., $(notdir $(ALCFG_SOURCES_Visu_M:.alcfg=.vco)))

$(AS_CPU_PATH)/VCShared/alcfg.AlarmSystem.vco: $(AS_PROJECT_PATH)/Logical/VCShared/AlarmGroups/AlarmSystem.alcfg
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


#AlarmSystem END




# Text Groups
TXTGRP_SHARED_OBJECTS_Visu_M = $(addprefix $(AS_CPU_PATH)/VCShared/txtgrp., $(notdir $(TXTGRP_SHARED_SOURCES_Visu_M:.txtgrp=.vco)))

$(AS_CPU_PATH)/VCShared/txtgrp.AlarmAcknowledgeState.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmAcknowledgeState.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.AlarmBypassState.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmBypassState.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.AlarmEvent.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmEvent.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.AlarmState.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/AlarmState.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.DateTimeFormats.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/DateTimeFormats.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.httpURL_SDM.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/httpURL_SDM.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.DWN_UP.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/DWN_UP.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.IN_OUT_DWN_UP.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/IN_OUT_DWN_UP.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.Indx_Mode.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/Indx_Mode.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


$(AS_CPU_PATH)/VCShared/txtgrp.Torch_Weld.vco: $(AS_PROJECT_PATH)/Logical/VCShared/TextGroups/Torch_Weld.txtgrp $(VC_LANGUAGES_Visu_M)
	 $(VCC) -f '$<' -o '$@' -l '$(AS_PROJECT_PATH)/Logical/VCShared/Languages.vcr' -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -pal '$(PALFILE_Visu_M)' $(VCCFLAGS_Visu_M)  -p Visu_M -so $(VC_STATIC_OPTIONS_Visu_M) -vcr 4732 -sfas


#Text Groups END


#
# Datapoint Objects
#
$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/dso.Internal.vco: $(AS_PROJECT_PATH)/Logical/VCShared/DataSources/Internal.dso 
	 $(VCC) -f '$<' -o '$@' $(DSOFLAGS) $(VCCFLAGS_Visu_M) -p Visu_M -vcr 4732 -sfas

$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/dso.DataSource.vco: $(AS_PROJECT_PATH)/Logical/VCShared/DataSources/DataSource.dso 
	 $(VCC) -f '$<' -o '$@' $(DSOFLAGS) $(VCCFLAGS_Visu_M) -p Visu_M -vcr 4732 -sfas

DPT_OBJECTS = $(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/dpt.DataPointList.vco
DSO_OBJECTS_Visu_M=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/dso.Internal.vco $(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/dso.DataSource.vco 
$(DSO_OBJECTS_Visu_M): $(DSO_SOURCES_Visu_M)


#
# Building the Shared Runtime Options
#
$(VCRS_OBJECT) : $(VCRS_SOURCE)
	$(VCC) -f '$<' -o '$@' -ct shared -cv '$(AS_PROJECT_PATH)/Logical/VCShared/ControlVersion.cvinfo' -sl  $(VCCFLAGS_Visu_M) -p Visu_M -vcr 4732 -sfas

#
# The Shared Module
#
SHARED_MODULE=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/vcshared.br
SHARED_CCF=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/vcshared.ccf
DEL_SHARED_CCF=$(TEMP_PATH_ROOT_Visu_M)/Objects/$(AS_CONFIGURATION)/$(AS_TEMP_PLC)/VCShared/vcshared.ccf.lfl

$(SHARED_MODULE) : $(SHARED_CCF)
	 $(MODGEN) -so $(VC_STATIC_OPTIONS_Shared) -fw '$(VCFIRMWAREPATH)' -m $(VCSTARTUP) -v V1.00.0 -vc '$(VCOBJECT_Visu_M)' -f '$<' -o '$@' -d vcgclass -profile 'False'

$(VCUG_OBJECTS_Visu_M): $(VC_LANGUAGES_Visu_M)
$(TXTGRP_SHARED_OBJECTS_Visu_M): $(VC_LANGUAGES_Visu_M)
$(ALGRP_OBJECTS_Visu_M): $(VC_LANGUAGES_Visu_M)
$(ALCFG_OBJECTS_Visu_M): $(VC_LANGUAGES_Visu_M)

$(SHARED_CCF): $(VCRS_OBJECT) $(VCR_OBJECTS_Visu_M) $(VCUG_OBJECTS_Visu_M) $(ALGRP_OBJECTS_Visu_M) $(ALCFG_OBJECTS_Visu_M) $(DSO_OBJECTS_Visu_M) $(TXTGRP_SHARED_OBJECTS_Visu_M) $(CVINFO_OBJECTS_Visu_M)
	-@CMD /Q /C if exist "$(DEL_SHARED_CCF)" DEL /F /Q "$(DEL_SHARED_CCF)" 2>nul
	 @$(VCFLGEN) '$@.lfl' '$(VCR_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' -mask .vcug -vcp '$(AS_PROJECT_PATH)/Logical/VCShared/Package.vcp' -temp '$(TEMP_PATH_Shared)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' -mask .algrp -vcp '$(AS_PROJECT_PATH)/Logical/VCShared/Package.vcp' -temp '$(TEMP_PATH_Shared)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' '$(ALCFG_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' -mask .txtgrp -vcp '$(AS_PROJECT_PATH)/Logical/VCShared/Package.vcp' -temp '$(TEMP_PATH_Shared)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' '$(DSO_OBJECTS_Visu_M:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' '$(DPT_OBJECTS:.vco=.vco|)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)'
	 @$(VCFLGEN) '$@.lfl' '$(VCRS_OBJECT)' -temp '$(TEMP_PATH_Visu_M)' -prj '$(PRJ_PATH_Visu_M)'
	 $(LINK) '$@.lfl' -o '$@' -lib '$(LIB_SHARED)' -P '$(AS_PROJECT_PATH)' -m 'shared resources' -profile 'False' -warningLevel2 -name Visu_M -vcr 4732 -sfas


$(LIB_SHARED): $(SHARED_CCF)
