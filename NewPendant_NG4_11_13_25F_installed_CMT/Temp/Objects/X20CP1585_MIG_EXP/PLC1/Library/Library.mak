UnmarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/Libraries/Library
MarkedObjectFolder := C:/projects/UnivCntrl/UniversalController/Logical/Libraries/Library

$(AS_STATIC_ARCHIVES_PATH)/libLibrary_s.a:
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -staticlink  -o "$(AS_STATIC_ARCHIVES_PATH)/libLibrary_s.a"  -G V4.1.2  -T SG4  -M IA32  -secret "$(AS_PROJECT_PATH)_br.as.ccompiler.exe"

$(AS_CPU_PATH)/Library/a.out:
	@"$(AS_BIN_PATH)/BR.AS.CCompiler.exe" -link  -o "$(AS_CPU_PATH)/Library/a.out"  -G V4.1.2  -T SG4  -M IA32  -$placeholder_archiv_files -specs=I386specs -Wl,-q,-T,bur_elf_i386.x

-include $(AS_CPU_PATH)/Force.mak 

