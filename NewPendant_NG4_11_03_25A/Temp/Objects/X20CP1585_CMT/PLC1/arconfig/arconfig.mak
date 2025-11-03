$(AS_CPU_PATH)/arconfig.br: \
	$(AS_PROJECT_CONFIG_PATH)/Hardware.hw \
	$(AS_PROJECT_CONFIG_PATH)/$(AS_PLC)/IoMap.iom \
	$(AS_PROJECT_PATH)/Physical/$(AS_CONFIGURATION)/Hardware.hw
	@"$(AS_BIN_PATH)/BR.AS.ConfigurationBuilder.exe" "$(AS_PROJECT_CONFIG_PATH)/Hardware.hw" "$(AS_PROJECT_CPU_PATH)/IoMap.iom"  -arconfig -L "Acp10man: V3.15.1, Acp10par: V3.15.1, NcGlobal: V3.15.1" -c X20CP1585_CMT -v V1.00.0 -S "PLC1" -o "$(AS_CPU_PATH)/arconfig.br" "$(AS_CPU_PATH)/ChannelConfiguration.xml" "$(AS_CPU_PATH)/HardwareConfiguration.xml" "$(AS_CPU_PATH)/ReActionTechnologyConfiguration.xml" -T SG4  -B E4.26 -P "$(AS_PROJECT_PATH)" -s "X20CP1585_CMT" -secret "$(AS_PROJECT_PATH)_br.as.configurationbuilder.exe"

-include $(AS_CPU_PATH)/Force.mak 
