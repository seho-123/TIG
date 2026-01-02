UnmarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/Libraries/sprintf2
MarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/Libraries/sprintf2

$(AS_STATIC_ARCHIVES_PATH)/libsprintf2_s.a: \
	$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o \
	$(AS_CPU_PATH)/sprintf2/ofmt.c.o \
	$(AS_CPU_PATH)/sprintf2/sprintf.c.o
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -staticlink "$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o" "$(AS_CPU_PATH)/sprintf2/ofmt.c.o" "$(AS_CPU_PATH)/sprintf2/sprintf.c.o"  -o "$(AS_STATIC_ARCHIVES_PATH)/libsprintf2_s.a"  -G V4.1.2  -T SG4  -M IA32  -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/sprintf2/a.out: \
	$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o \
	$(AS_CPU_PATH)/sprintf2/ofmt.c.o \
	$(AS_CPU_PATH)/sprintf2/sprintf.c.o
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link "$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o" "$(AS_CPU_PATH)/sprintf2/ofmt.c.o" "$(AS_CPU_PATH)/sprintf2/sprintf.c.o"  -o "$(AS_CPU_PATH)/sprintf2/a.out"  -G V4.1.2  -T SG4  -M IA32  -$placeholder_archiv_files -specs=I386specs -Wl,-q,-T,bur_elf_i386.x

$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o: \
	$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/cvtflasc.c
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/cvtflasc.c" -o "$(AS_CPU_PATH)/sprintf2/cvtflasc.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "Libraries.sprintf2" -t "$(AS_TEMP_PATH)" -r Library -I "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -D _SPRINTF2_EXPORT -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/sprintf2/ofmt.c.o: \
	$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/ofmt.c
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/ofmt.c" -o "$(AS_CPU_PATH)/sprintf2/ofmt.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "Libraries.sprintf2" -t "$(AS_TEMP_PATH)" -r Library -I "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -D _SPRINTF2_EXPORT -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/sprintf2/sprintf.c.o: \
	$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/sprintf.c
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2/sprintf.c" -o "$(AS_CPU_PATH)/sprintf2/sprintf.c.o"  -T SG4  -M IA32  -B E4.26 -G V4.1.2  -s "Libraries.sprintf2" -t "$(AS_TEMP_PATH)" -r Library -I "$(AS_PROJECT_PATH)/Logical/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes/Libraries/sprintf2" "$(AS_TEMP_PATH)/Includes" -trigraphs -fno-asm -D _DEFAULT_INCLUDES -D _SG4 -fPIC -O0 -g -Wall -include "$(AS_CPU_PATH)/Libraries.h" -D _SPRINTF2_EXPORT -x c -P "$(AS_PROJECT_PATH)" -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

-include $(AS_CPU_PATH)/Force.mak 

