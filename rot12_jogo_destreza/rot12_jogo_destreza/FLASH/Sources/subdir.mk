################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/GPIO_Switches.c" \
"../Sources/GPIO_latch_lcd.c" \
"../Sources/ISR.c" \
"../Sources/TPM.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \
"../Sources/timers.c" \
"../Sources/util.c" \

C_SRCS += \
../Sources/GPIO_Switches.c \
../Sources/GPIO_latch_lcd.c \
../Sources/ISR.c \
../Sources/TPM.c \
../Sources/main.c \
../Sources/sa_mtb.c \
../Sources/timers.c \
../Sources/util.c \

OBJS += \
./Sources/GPIO_Switches.o \
./Sources/GPIO_latch_lcd.o \
./Sources/ISR.o \
./Sources/TPM.o \
./Sources/main.o \
./Sources/sa_mtb.o \
./Sources/timers.o \
./Sources/util.o \

C_DEPS += \
./Sources/GPIO_Switches.d \
./Sources/GPIO_latch_lcd.d \
./Sources/ISR.d \
./Sources/TPM.d \
./Sources/main.d \
./Sources/sa_mtb.d \
./Sources/timers.d \
./Sources/util.d \

OBJS_QUOTED += \
"./Sources/GPIO_Switches.o" \
"./Sources/GPIO_latch_lcd.o" \
"./Sources/ISR.o" \
"./Sources/TPM.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \
"./Sources/timers.o" \
"./Sources/util.o" \

C_DEPS_QUOTED += \
"./Sources/GPIO_Switches.d" \
"./Sources/GPIO_latch_lcd.d" \
"./Sources/ISR.d" \
"./Sources/TPM.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \
"./Sources/timers.d" \
"./Sources/util.d" \

OBJS_OS_FORMAT += \
./Sources/GPIO_Switches.o \
./Sources/GPIO_latch_lcd.o \
./Sources/ISR.o \
./Sources/TPM.o \
./Sources/main.o \
./Sources/sa_mtb.o \
./Sources/timers.o \
./Sources/util.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/GPIO_Switches.o: ../Sources/GPIO_Switches.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPIO_Switches.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPIO_Switches.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GPIO_latch_lcd.o: ../Sources/GPIO_latch_lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPIO_latch_lcd.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPIO_latch_lcd.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/ISR.o: ../Sources/ISR.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ISR.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ISR.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/TPM.o: ../Sources/TPM.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/TPM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/TPM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/timers.o: ../Sources/timers.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/timers.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/timers.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/util.o: ../Sources/util.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/util.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/util.o"
	@echo 'Finished building: $<'
	@echo ' '


