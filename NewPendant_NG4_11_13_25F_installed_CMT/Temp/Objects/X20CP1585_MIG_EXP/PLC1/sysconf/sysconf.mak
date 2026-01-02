$(AS_CPU_PATH)/sysconf.br: \
	$(AS_PROJECT_CONFIG_PATH)/Hardware.hw 
	@"$(AS_BIN_PATH)/BR.AS.ConfigurationBuilder.exe" "$(AS_PROJECT_CONFIG_PATH)/Hardware.hw"  -c X20CP1585_MIG_EXP -sysconf -S "PLC1" -o "$(AS_CPU_PATH)/sysconf.br" -T SG4  -B E4.26 -P "$(AS_PROJECT_PATH)" -s "X20CP1585_MIG_EXP" -secret "$(AS_PROJECT_PATH)_br.as.configurationbuilder.exe"

-include $(AS_CPU_PATH)/Force.mak 
