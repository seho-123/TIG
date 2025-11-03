export AS_BUILD_MODE := Build
export AS_SYSTEM_PATH := C:/Program\ Files/BRAutomation4/AS/System
export AS_BIN_PATH := C:/Program Files/BRAutomation4/AS412/bin-en
export AS_INSTALL_PATH := C:/Program\ Files/BRAutomation4/AS412
export AS_PATH := C:/Program Files/BRAutomation4/AS412
export AS_VC_PATH := C:/Program\ Files/BRAutomation4/AS412/AS/VC
export AS_GNU_INST_PATH := C:/Program\ Files/BRAutomation4/AS412/AS/gnuinst/V4.1.2
export AS_STATIC_ARCHIVES_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Temp/Archives/X20CP1585_MIG_HW_PS/PLC1
export AS_CPU_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Temp/Objects/X20CP1585_MIG_HW_PS/PLC1
export AS_CPU_PATH_2 := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Temp/Objects/X20CP1585_MIG_HW_PS/PLC1
export AS_TEMP_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Temp
export AS_BINARIES_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Binaries
export AS_PROJECT_CPU_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Physical/X20CP1585_MIG_HW_PS/PLC1
export AS_PROJECT_CONFIG_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B/Physical/X20CP1585_MIG_HW_PS
export AS_PROJECT_PATH := C:/Users/SPark/Documents/GitHub/TIG/NewPendant_NG4_10_29_25B
export AS_PROJECT_NAME := UnivCntrl
export AS_PLC := PLC1
export AS_TEMP_PLC := PLC1
export AS_USER_NAME := SPark
export AS_CONFIGURATION := X20CP1585_MIG_HW_PS
export AS_COMPANY_NAME := \ 
export AS_VERSION := 4.12.7.113\ SP
export AS_WORKINGVERSION := 4.12


default: \
	$(AS_CPU_PATH)/Visu1.br \
	vcPostBuild_Visu1 \
	$(AS_CPU_PATH)/Visu2.br \
	vcPostBuild_Visu2 \



include $(AS_CPU_PATH)/Visu1/Visu1.mak
include $(AS_CPU_PATH)/Visu2/Visu2.mak
