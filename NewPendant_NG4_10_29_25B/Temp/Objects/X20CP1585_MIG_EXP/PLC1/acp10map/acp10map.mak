$(AS_CPU_PATH)/acp10map.br: \
	$(AS_PROJECT_CPU_PATH)/Motion/Acp10map.ncm \
	$(AS_PROJECT_PATH)/physical/x20cp1585_mig_exp/plc1/motion/acp10map.ncm \
	$(AS_PROJECT_PATH)/physical/x20cp1585_mig_exp/hardware.hw
	@"$(AS_BIN_PATH)/BR.MC.Builder.exe" "$(AS_PROJECT_CPU_PATH)/Motion/Acp10map.ncm"  -v V1.00.0 -a "$(AS_PROJECT_CONFIG_PATH)/Hardware.hw" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, NcGlobal: V3.15.1" -P "$(AS_PROJECT_PATH)" -o "$(AS_CPU_PATH)/Acp10map.br" -T SG4  -s "X20CP1585_MIG_EXP.PLC1.Motion" -secret "$(AS_PROJECT_PATH)_br.mc.builder.exe"

-include $(AS_CPU_PATH)/Force.mak 
