################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 11
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
rot9_cooler_multi_velocidade.elf \

EXECUTABLES_QUOTED += \
"rot9_cooler_multi_velocidade.elf" \

EXECUTABLES_OS_FORMAT += \
rot9_cooler_multi_velocidade.elf \

SECONDARY_SIZE += \
rot9_cooler_multi_velocidade.siz \

SECONDARY_SIZE_QUOTED += \
"rot9_cooler_multi_velocidade.siz" \

SECONDARY_SIZE_OS_FORMAT += \
rot9_cooler_multi_velocidade.siz \


# All Target
all: warning-messages explicit-dependencies rot9_cooler_multi_velocidade.elf secondary-outputs

# Tool invocations
rot9_cooler_multi_velocidade.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/MKL25Z128_flash.ld
	@echo 'Building target: $@'
	@echo 'Executing target #12 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) @"rot9_cooler_multi_velocidade.args" -o"rot9_cooler_multi_velocidade.elf"
	@echo 'Finished building target: $@'
	@echo ' '

rot9_cooler_multi_velocidade.siz: rot9_cooler_multi_velocidade.elf
	@echo 'Executing target #13 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Print Size'
	"$(ARMSourceryDirEnv)/arm-none-eabi-size"  --format=berkeley rot9_cooler_multi_velocidade.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) "./*/*/*.o" "./*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(SECONDARY_SIZE_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*/*.d" "./*/*.d"  $(ProjDirPath)/FLASH/rot9_cooler_multi_velocidade.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/MKL25Z128_flash.ld rot9_cooler_multi_velocidade.elf 

warning-messages: 

