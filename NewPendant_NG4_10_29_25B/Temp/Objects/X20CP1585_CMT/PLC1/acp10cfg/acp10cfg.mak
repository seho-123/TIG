$(AS_CPU_PATH)/acp10cfg.br: \
	$(AS_PROJECT_CPU_PATH)/Motion/Acp10cfg.ncc \
	$(AS_PROJECT_PATH)/physical/x20cp1585_cmt/plc1/motion/acp10cfg.ncc \
	$(AS_PROJECT_PATH)/physical/x20cp1585_cmt/hardware.hw \
	$(AS_PROJECT_PATH)/physical/x20cp1585_cmt/plc1/motion/acp10map.ncm
	@"$(AS_BIN_PATH)/BR.MC.Builder.exe" "$(AS_PROJECT_CPU_PATH)/Motion/Acp10cfg.ncc"  -a "$(AS_PROJECT_CONFIG_PATH)/Hardware.hw" -m "$(AS_PROJECT_CPU_PATH)/Motion/Acp10map.ncm" -L "Acp10man: V3.15.1, Acp10par: V3.15.1, NcGlobal: V3.15.1" -P "$(AS_PROJECT_PATH)" -o "$(AS_CPU_PATH)/acp10cfg.br" -T SG4  -s "X20CP1585_CMT.PLC1.Motion" -secret "$(AS_PROJECT_PATH)_br.mc.builder.exe"

-include $(AS_CPU_PATH)/Force.mak 
