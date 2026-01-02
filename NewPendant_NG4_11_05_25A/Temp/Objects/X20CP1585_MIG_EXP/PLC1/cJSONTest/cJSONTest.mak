UnmarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/UDP/cJSON/cJSONTest
MarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/UDP/cJSON/cJSONTest

$(AS_CPU_PATH)/cJSONTest.br: \
	$(AS_PROJECT_CPU_PATH)/Cpu.per \
	$(AS_CPU_PATH)/cJSONTest/cJSONTest.ox
	@"$(AS_BIN_PATH)/BR.AS.TaskBuilder.exe" "$(AS_CPU_PATH)/cJSONTest/cJSONTest.ox" -o "$(AS_CPU_PATH)/cJSONTest.br" -v V1.00.0 -f "$(AS_CPU_PATH)/NT.ofs" -offsetLT "$(AS_BINARIES_PATH)/$(AS_CONFIGURATION)/$(AS_PLC)/LT.ofs" -T SG4  -M IA32  -B E4.26 -extMembers -extNames -extConstants -r Cyclic8 -p 4 -s "UDP.cJSON.cJSONTest" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, AsARCfg: V*, AsBrMath: V*, AsIecCon: V*, AsIOTime: V*, AsRfbExt: V*, asstring: V*, AsTCP: V*, AsTime: V*, AsUDP: V*, AsXml: V*, bglibas: V*, brsystem: V*, DataObj: V*, dvframe: V*, Ethernet: V*, FileIO: V*, Library: V*, NcGlobal: V3.15.1, Operator: V*, Runtime: V*, sprintf2: V*, standard: V*, sys_lib: V*, Visapi: V*" -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.taskbuilder.exe"

$(AS_CPU_PATH)/cJSONTest/cJSONTest.ox: \
	$(AS_CPU_PATH)/cJSONTest/a.out
	@"$(AS_BIN_PATH)/BR.AS.Backend.exe" "$(AS_CPU_PATH)/cJSONTest/a.out" -o "$(AS_CPU_PATH)/cJSONTest/cJSONTest.ox" -T SG4 -r Cyclic8   -G V4.1.2  -secret "$(AS_PROJECT_PATH)_br.as.backend.exe"

$(AS_CPU_PATH)/cJSONTest/a.out: \
	$(AS_CPU_PATH)/cJSONTest/cJSON.c.o \
	$(AS_CPU_PATH)/cJSONTest/test.c.o \
	$(AS_CPU_PATH)/cJSONTest/cJSONTest.c.o \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a \
	$(AS_PROJECT_PATH)/Logical/UDP/sscanf2/libsscanf2-i386.a
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link  -O "$(AS_CPU_PATH)//cJSONTest/cJSONTest.out.opt" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/cJSONTest/cJSON.c.o: \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSON.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/sys_lib/sys_lib.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSONTest.c \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSON.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsUDP/AsUDP.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.var \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSON.h \
	$(AS_PROJECT_PATH)/Logical/UDP/sscanf2/sscanf2.h \
	$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/sprintf2.h
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSON.c" -o "$(AS_CPU_PATH)/cJSONTest/cJSON.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "UDP.cJSON.cJSONTest" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest" "$(AS_TEMP_PATH)/Includes/UDP/cJSON/cJSONTest"  "$(AS_TEMP_PATH)/Includes/UDP/cJSON"  "$(AS_TEMP_PATH)/Includes/UDP" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/cJSONTest/test.c.o: \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/test.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/sys_lib/sys_lib.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSONTest.c \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSON.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsUDP/AsUDP.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.var \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSON.h
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/test.c" -o "$(AS_CPU_PATH)/cJSONTest/test.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "UDP.cJSON.cJSONTest" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest" "$(AS_TEMP_PATH)/Includes/UDP/cJSON/cJSONTest"  "$(AS_TEMP_PATH)/Includes/UDP/cJSON"  "$(AS_TEMP_PATH)/Includes/UDP" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/cJSONTest/cJSONTest.c.o: \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSONTest.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/sys_lib/sys_lib.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSONTest.c \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSON.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.var \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsUDP/AsUDP.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/bglibas/bglibas.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.var \
	$(AS_PROJECT_PATH)/Logical/bglib/bgsprintf.c \
	$(AS_PROJECT_PATH)/Logical/bglib/bgPVSerializeJSON.c \
	$(AS_PROJECT_PATH)/Logical/bglib/bgstring.h \
	$(AS_PROJECT_PATH)/Logical/bglib/bgsprintf.h \
	$(AS_PROJECT_PATH)/Logical/bglib/bgPVLogger2.c \
	$(AS_PROJECT_PATH)/Logical/bglib/bgstring.c \
	$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSON.h
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest/cJSONTest.c" -o "$(AS_CPU_PATH)/cJSONTest/cJSONTest.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "UDP.cJSON.cJSONTest" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/UDP/cJSON/cJSONTest" "$(AS_TEMP_PATH)/Includes/UDP/cJSON/cJSONTest"  "$(AS_TEMP_PATH)/Includes/UDP/cJSON"  "$(AS_TEMP_PATH)/Includes/UDP" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

-include $(AS_CPU_PATH)/Force.mak 

