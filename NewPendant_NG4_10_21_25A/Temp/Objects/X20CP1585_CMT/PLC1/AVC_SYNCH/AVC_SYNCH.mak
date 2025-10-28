UnmarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/AVC_SYNCH
MarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/FourAxisTigCtrl/AVC_SYNCH

$(AS_CPU_PATH)/AVC_SYNCH.br: \
	$(AS_PROJECT_CPU_PATH)/Cpu.per \
	$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.ox
	@"$(AS_BIN_PATH)/BR.AS.TaskBuilder.exe" "$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.ox" -o "$(AS_CPU_PATH)/AVC_SYNCH.br" -v V1.00.0 -f "$(AS_CPU_PATH)/NT.ofs" -offsetLT "$(AS_BINARIES_PATH)/$(AS_CONFIGURATION)/$(AS_PLC)/LT.ofs" -T SG4  -M IA32  -B E4.26 -extMembers -extNames -extConstants -r Cyclic1 -p 5 -s "FourAxisTigCtrl.AVC_SYNCH" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, AsARCfg: V*, AsBrMath: V*, AsIecCon: V*, AsIOTime: V*, AsRfbExt: V*, asstring: V*, AsTCP: V*, AsTime: V*, AsUDP: V*, AsXml: V*, bglibas: V*, brsystem: V*, DataObj: V*, dvframe: V*, Ethernet: V*, FileIO: V*, Library: V*, NcGlobal: V3.15.1, Operator: V*, Runtime: V*, sprintf2: V*, standard: V*, sys_lib: V*, Visapi: V*" -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.taskbuilder.exe"

$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.ox: \
	$(AS_CPU_PATH)/AVC_SYNCH/a.out
	@"$(AS_BIN_PATH)/BR.AS.Backend.exe" "$(AS_CPU_PATH)/AVC_SYNCH/a.out" -o "$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.ox" -T SG4 -r Cyclic1   -G V4.1.2  -secret "$(AS_PROJECT_PATH)_br.as.backend.exe"

$(AS_CPU_PATH)/AVC_SYNCH/a.out: \
	$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.c.o \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a \
	$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link "$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.c.o"  -o "$(AS_CPU_PATH)/AVC_SYNCH/a.out"  -G V4.1.2  -T SG4  -M IA32  "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libbrsystem.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libLibrary_s.a" "-Wl,$(AS_PROJECT_PATH)/Logical/Libraries/NcGlobal/SG4/IA32/libncglobal.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libsprintf2_s.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsXml.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libasstring.a" "-Wl,$(AS_TEMP_PATH)/Archives/$(AS_CONFIGURATION)/$(AS_PLC)/libbglibas.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsIOTime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsIecCon.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libstandard.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsBrMath.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libDataObj.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsUDP.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsARCfg.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libFileIO.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libAsTCP.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libdvframe.a" "-Wl,$(AS_VC_PATH)/Firmware/V4.26.8/SG4/IA32/libAsRfbExt.a" "-Wl,$(AS_VC_PATH)/Firmware/V4.26.8/SG4/IA32/libvisapi.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libETHERNET.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libsys_lib.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libastime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/libruntime.a" "-Wl,$(AS_SYSTEM_PATH)/E0426/SG4/IA32/liboperator.a" -specs=I386specs -Wl,-q,-T,bur_elf_i386.x -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.c.o: \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/AVC_SYNCH/AVC_SYNCH.c \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsIOTime/AsIOTime.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/sys_lib/sys_lib.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/standard/standard.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/asstring/asstring.fun \
	$(AS_PROJECT_PATH)/Logical/Libraries/AsTime/astime.fun \
	$(AS_PROJECT_PATH)/Logical/UDP/paiUDPService/paiUDPService.typ \
	$(AS_PROJECT_PATH)/Logical/UDP/udpApp.typ \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/AVC_SYNCH/AVC_SYNCH.c \
	$(AS_PROJECT_PATH)/Logical/Universal/LincolnPS/../../Lib/WSI_types.h \
	$(AS_PROJECT_PATH)/Logical/Universal/LincolnPS/../../lib/Statelib.h \
	$(AS_PROJECT_PATH)/Logical/Universal/TML_srv/../../Universal/SuperDeclaration.h \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/GlobDef.h \
	$(AS_PROJECT_PATH)/Logical/Lib/WSI_types.h \
	$(AS_PROJECT_PATH)/Logical/lib/Statelib.h \
	$(AS_PROJECT_PATH)/Logical/Universal/SuperDefinition.h \
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
	$(AS_PROJECT_PATH)/Logical/Lib/WSI_Lib.c \
	$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/GlobDecl.h \
	$(AS_PROJECT_PATH)/Logical/Universal/SuperDeclaration.h
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/AVC_SYNCH/AVC_SYNCH.c" -o "$(AS_CPU_PATH)/AVC_SYNCH/AVC_SYNCH.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "FourAxisTigCtrl.AVC_SYNCH" -t "$(AS_TEMP_PATH)" -I "$(AS_PROJECT_PATH)/Logical/FourAxisTigCtrl/AVC_SYNCH" "$(AS_TEMP_PATH)/Includes/FourAxisTigCtrl/AVC_SYNCH"  "$(AS_TEMP_PATH)/Includes/FourAxisTigCtrl" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

-include $(AS_CPU_PATH)/Force.mak 

