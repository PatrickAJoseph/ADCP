################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --use_hw_mpy=F5 --include_path="D:/MICROCONTROLLERS/TI/CCS/ccs/ccs_base/msp430/include" --include_path="F:/Projects/ADCP/Github_Push_Pull/ADCP/Example Application/MPU_Stick/inc" --include_path="F:/Projects/ADCP/Github_Push_Pull/ADCP/Example Application/MPU_Stick/src" --include_path="F:/Projects/ADCP/Github_Push_Pull/ADCP/Example Application/MPU_Stick" --include_path="C:/ti/msp430/IQMATHLIB_01_10_00_05/include" --include_path="D:/MICROCONTROLLERS/TI/CCS/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR2355__ --define=_FRWP_ENABLE --define=_INFO_FRWP_ENABLE -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="src/$(basename $(<F)).d_raw" --obj_directory="src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


