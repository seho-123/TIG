UnmarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/UDP/paiUDPService/udpserv
MarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/UDP/paiUDPService/udpserv

$(AS_CPU_PATH)/udpserv.br: \
	$(AS_PROJECT_CPU_PATH)/Cpu.per \
	$(AS_CPU_PATH)/udpserv/udpserv.ox
	@"$(AS_BIN_PATH)/BR.AS.TaskBuilder.exe" "$(AS_CPU_PATH)/udpserv/udpserv.ox" -o "$(AS_CPU_PATH)/udpserv.br" -v V1.00.0 -f "$(AS_CPU_PATH)/NT.ofs" -offsetLT "$(AS_BINARIES_PATH)/$(AS_CONFIGURATION)/$(AS_PLC)/LT.ofs" -T SG4  -M IA32  -B E4.26 -extMembers -extNames -extConstants -r Cyclic8 -p 5 -s "UDP.paiUDPService.udpserv" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, AsARCfg: V*, AsBrMath: V*, AsIecCon: V*, AsIOTime: V*, AsRfbExt: V*, asstring: V*, AsTCP: V*, AsTime: V*, AsUDP: V*, AsXml: V*, bglibas: V*, brsystem: V*, DataObj: V*, dvframe: V*, Ethernet: V*, FileIO: V*, Library: V*, NcGlobal: V3.15.1, Operator: V*, Runtime: V*, sprintf2: V*, standard: V*, sys_lib: V*, Visapi: V*" -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.taskbuilder.exe"

$(AS_CPU_PATH)/udpserv/udpserv.ox: \
	$(AS_CPU_PATH)/udpserv/a.out
	@"$(AS_BIN_PATH)/BR.AS.Backend.exe" "$(AS_CPU_PATH)/udpserv/a.out" -o "$(AS_CPU_PATH)/udpserv/udpserv.ox" -T SG4 -r Cyclic8   -G V4.1.2  -secret "$(AS_PROJECT_PATH)_br.as.backend.exe"

$(AS_CPU_PATH)/udpserv/a.out: \
	$(AS_CPU_PATH)/udpserv/udpserv.c.o \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link "$(AS_CPU_PATH)/udpserv/udpserv.c.o"  -o "$(AS_CPU_PATH)/udpserv/a.out"  -G V4.1.2  -T SG4  -M IA32  "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libbrsystem.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a" "-Wl,$(AS_PROJECT_PATH)/Logical/Libraries/NcGlobal/SG4/IA32/libncglobal.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsXml.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libasstring.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libbglibas.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsIOTime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsIecCon.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libstandard.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsBrMath.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libDataObj.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsUDP.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsARCfg.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libFileIO.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsTCP.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libdvframe.a" "-Wl,$(AS_VC_PATH)/Firmware/V4.26.8/SG4/IA32/libAsRfbExt.a" "-Wl,$(AS_VC_PATH)/Firmware/V4.26.8/SG4/IA32/libvisapi.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libETHERNET.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libsys_lib.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libastime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libruntime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/liboperator.a" -specs=I386specs -Wl,-q,-T,bur_elf_i386.x -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/udpserv/udpserv.c.o: \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/udpserv/udpserv.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsUDP/AsUDP.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsUDP/AsUDP.var \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/udpserv/udpserv.c \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.typ \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/Universal/SuperMstr/../../Universal/SuperDeclaration.h \
	$(AS_PROJECT_PATH)/Logical/Libraries/Runtime/runtime.var \
	$(AS_PROJECT_PATH)/Logical/bglib/bgPVLogger.c \
	$(AS_PROJECT_PATH)/Logical/bglib/bgsprintf.h \
	$(AS_PROJECT_PATH)/Logical/bglib/bgsprintf.c \
	$(AS_PROJECT_PATH)/Logical/bglib/bgstring.h \
	$(AS_PROJECT_PATH)/Logical/bglib/bgstring.c
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/udpserv/udpserv.c" -o "$(AS_CPU_PATH)/udpserv/udpserv.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "UDP.paiUDPService.udpserv" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/udpserv" "$(AS_TEMP_PATH)/Includes/UDP/paiUDPService/udpserv"  "$(AS_TEMP_PATH)/Includes/UDP/paiUDPService"  "$(AS_TEMP_PATH)/Includes/UDP" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

-include $(AS_CPU_PATH)/Force.mak 

