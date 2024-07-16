################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/GPIO_latch_lcd.c" \
"../Sources/GPIO_switches.c" \
"../Sources/ISR.c" \
"../Sources/PIT.c" \
"../Sources/RTC.c" \
"../Sources/SIM.c" \
"../Sources/main.c" \
"../Sources/sa_mtb.c" \
"../Sources/util.c" \

C_SRCS += \
../Sources/GPIO_latch_lcd.c \
../Sources/GPIO_switches.c \
../Sources/ISR.c \
../Sources/PIT.c \
../Sources/RTC.c \
../Sources/SIM.c \
../Sources/main.c \
../Sources/sa_mtb.c \
../Sources/util.c \

OBJS += \
./Sources/GPIO_latch_lcd.o \
./Sources/GPIO_switches.o \
./Sources/ISR.o \
./Sources/PIT.o \
./Sources/RTC.o \
./Sources/SIM.o \
./Sources/main.o \
./Sources/sa_mtb.o \
./Sources/util.o \

C_DEPS += \
./Sources/GPIO_latch_lcd.d \
./Sources/GPIO_switches.d \
./Sources/ISR.d \
./Sources/PIT.d \
./Sources/RTC.d \
./Sources/SIM.d \
./Sources/main.d \
./Sources/sa_mtb.d \
./Sources/util.d \

OBJS_QUOTED += \
"./Sources/GPIO_latch_lcd.o" \
"./Sources/GPIO_switches.o" \
"./Sources/ISR.o" \
"./Sources/PIT.o" \
"./Sources/RTC.o" \
"./Sources/SIM.o" \
"./Sources/main.o" \
"./Sources/sa_mtb.o" \
"./Sources/util.o" \

C_DEPS_QUOTED += \
"./Sources/GPIO_latch_lcd.d" \
"./Sources/GPIO_switches.d" \
"./Sources/ISR.d" \
"./Sources/PIT.d" \
"./Sources/RTC.d" \
"./Sources/SIM.d" \
"./Sources/main.d" \
"./Sources/sa_mtb.d" \
"./Sources/util.d" \

OBJS_OS_FORMAT += \
./Sources/GPIO_latch_lcd.o \
./Sources/GPIO_switches.o \
./Sources/ISR.o \
./Sources/PIT.o \
./Sources/RTC.o \
./Sources/SIM.o \
./Sources/main.o \
./Sources/sa_mtb.o \
./Sources/util.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/GPIO_latch_lcd.o: ../Sources/GPIO_latch_lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPIO_latch_lcd.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPIO_latch_lcd.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/GPIO_switches.o: ../Sources/GPIO_switches.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/GPIO_switches.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/GPIO_switches.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/ISR.o: ../Sources/ISR.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ISR.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ISR.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/PIT.o: ../Sources/PIT.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/PIT.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/PIT.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/RTC.o: ../Sources/RTC.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/RTC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/RTC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/SIM.o: ../Sources/SIM.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/SIM.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/SIM.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/util.o: ../Sources/util.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/util.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/util.o"
	@echo 'Finished building: $<'
	@echo ' '


