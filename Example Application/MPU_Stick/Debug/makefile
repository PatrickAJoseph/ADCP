################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./src/ADCP.obj" \
"./src/ADCP_Receive.obj" \
"./src/ADCP_Transmit.obj" \
"./src/ADCP_frames.obj" \
"./src/EEPROM.obj" \
"./src/LED.obj" \
"./src/MPU9250.obj" \
"./src/configuration.obj" \
"./src/i2c.obj" \
"./src/motion_processing.obj" \
"./src/spi.obj" \
"./src/system.obj" \
"./src/task_manager.obj" \
"./src/tick.obj" \
"./src/uart.obj" \
"C:/ti/msp430/IQMATHLIB_01_10_00_05/libraries/CCS/MPY32/5xx_6xx/IQmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_small_data.lib" \
"../lnk_msp430fr2355.cmd" \
$(GEN_CMDS__FLAG) \
-lfrwp_init.a \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
MPU_Stick.out 

EXE_OUTPUTS__QUOTED += \
"MPU_Stick.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "MPU_Stick.out"

# Tool invocations
MPU_Stick.out: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --advice:power=all --advice:hw_config=all --define=__MSP430FR2355__ --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"MPU_Stick.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"D:/MICROCONTROLLERS/TI/CCS/ccs/ccs_base/msp430/include" -i"D:/MICROCONTROLLERS/TI/CCS/ccs/ccs_base/msp430/lib/FR2xx" -i"D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" -i"D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --priority --reread_libs --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="MPU_Stick_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "MPU_Stick.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "src\ADCP.obj" "src\ADCP_Receive.obj" "src\ADCP_Transmit.obj" "src\ADCP_frames.obj" "src\EEPROM.obj" "src\LED.obj" "src\MPU9250.obj" "src\configuration.obj" "src\i2c.obj" "src\motion_processing.obj" "src\spi.obj" "src\system.obj" "src\task_manager.obj" "src\tick.obj" "src\uart.obj" 
	-$(RM) "main.d" "src\ADCP.d" "src\ADCP_Receive.d" "src\ADCP_Transmit.d" "src\ADCP_frames.d" "src\EEPROM.d" "src\LED.d" "src\MPU9250.d" "src\configuration.d" "src\i2c.d" "src\motion_processing.d" "src\spi.d" "src\system.d" "src\task_manager.d" "src\tick.d" "src\uart.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

