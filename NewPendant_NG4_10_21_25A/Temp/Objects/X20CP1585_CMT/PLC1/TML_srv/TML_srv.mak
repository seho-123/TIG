UnmarkedObjectFolder := C:/Users/ckaczkowski/Documents/Official Release/UnivCntrl-10-19-21F CMT version installed in Oct 2021/UniversalController_10-19-21F/Logical/Universal/TML_srv
MarkedObjectFolder := C:/Users/ckaczkowski/Documents/Official\ Release/UnivCntrl-10-19-21F\ CMT\ version\ installed\ in\ Oct\ 2021/UniversalController_10-19-21F/Logical/Universal/TML_srv

$(AS_CPU_PATH)/TML_srv.br: \
	$(AS_PROJECT_CPU_PATH)/Cpu.per \
	$(AS_CPU_PATH)/TML_srv/TML_srv.ox
	@"$(AS_BIN_PATH)/BR.AS.TaskBuilder.exe" "$(AS_CPU_PATH)/TML_srv/TML_srv.ox" -o "$(AS_CPU_PATH)/TML_srv.br" -v V1.00.0 -f "$(AS_CPU_PATH)/NT.ofs" -offsetLT "$(AS_BINARIES_PATH)/$(AS_CONFIGURATION)/$(AS_PLC)/LT.ofs" -T SG4  -M IA32  -B E4.26 -extMembers -extNames -extConstants -r Cyclic1 -p 4 -s "Universal.TML_srv" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, AsARCfg: V*, AsBrMath: V*, AsIecCon: V*, AsIOTime: V*, AsRfbExt: V*, asstring: V*, AsTCP: V*, AsTime: V*, AsUDP: V*, AsXml: V*, bglibas: V*, brsystem: V*, DataObj: V*, dvframe: V*, Ethernet: V*, FileIO: V*, Library: V*, NcGlobal: V3.15.1, Operator: V*, Runtime: V*, sprintf2: V*, standard: V*, sys_lib: V*, Visapi: V*" -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.taskbuilder.exe"

$(AS_CPU_PATH)/TML_srv/TML_srv.ox: \
	$(AS_CPU_PATH)/TML_srv/a.out
	@"$(AS_BIN_PATH)/BR.AS.Backend.exe" "$(AS_CPU_PATH)/TML_srv/a.out" -o "$(AS_CPU_PATH)/TML_srv/TML_srv.ox" -T SG4 -r Cyclic1   -G V4.1.2  -secret "$(AS_PROJECT_PATH)_br.as.backend.exe"

$(AS_CPU_PATH)/TML_srv/a.out: \
	$(AS_CPU_PATH)/TML_srv/TML_srv.c.o \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link  -O "$(AS_CPU_PATH)//TML_srv/TML_srv.out.opt" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/TML_srv/TML_srv.c.o: \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/TML_srv.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsIOTime/AsIOTime.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsTime/astime.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/DataObj/DataObj.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/sys_lib/sys_lib.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/TML_srv.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/DataObj/DataObj.var \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/../../Universal/SuperDeclaration.h \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/../../Lib/WSI_types.h \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/../../Lib/LoggerLib.h \
	$(AS_PROJECT_PATH)/Logical/Libraries/FileIO/FileIO.typ \
	$(AS_PROJECT_PATH)/Logical/Libraries/FileIO/FileIO.fun \
	$(AS_PROJECT_PATH)/Logical/Universal/SuperDeclaration.h \
	$(AS_PROJECT_PATH)/Logical/Universal/SuperDefinition.h \
	$(AS_PROJECT_PATH)/Logical/Lib/WSI_types.h \
	$(AS_PROJECT_PATH)/Logical/lib/Statelib.h \
	$(AS_PROJECT_PATH)/Logical/Lib/LoggerLib.h \
	$(AS_PROJECT_PATH)/Logical/lib/xstring.c \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/arclink.h \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgdebug.h \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgsprintf.h \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgringbuffer.h \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgstring.h \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgringbuffer.c \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgsprintf.c \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/bgstring.c \
	$(AS_PROJECT_PATH)/Logical/Lib/utilityLib/libRingBuffer.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/NetworkActions.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/NetworkAttribute.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/ArcLinkObject.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/Robot.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/WeldController.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/NetworkBlocks.h \
	$(AS_PROJECT_PATH)/Logical/Lib/arclink/Attribute.h \
	$(AS_PROJECT_PATH)/Logical/Lib/Statelib.c \
	$(AS_PROJECT_PATH)/Logical/lib/WSI_types.h \
	$(AS_PROJECT_PATH)/Logical/lib/statelib.h \
	$(AS_PROJECT_PATH)/Logical/Lib/LoggerLib.c \
	$(AS_PROJECT_PATH)/Logical/Lib/WSI_lib.c
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/TML_srv.c" -o "$(AS_CPU_PATH)/TML_srv/TML_srv.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "Universal.TML_srv" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/Universal/TML_srv" "$(AS_TEMP_PATH)/Includes/Universal/TML_srv"  "$(AS_TEMP_PATH)/Includes/Universal" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

-include $(AS_CPU_PATH)/Force.mak 

