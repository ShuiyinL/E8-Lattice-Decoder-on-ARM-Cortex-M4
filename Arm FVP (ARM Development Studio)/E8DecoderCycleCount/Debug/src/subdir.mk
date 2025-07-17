################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/E8DecoderCycleCount.c 

C_DEPS += \
./src/E8DecoderCycleCount.d 

OBJS += \
./src/E8DecoderCycleCount.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler for Embedded 6'
	armclang.exe --target=arm-arm-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -O0 -g -MD -MP -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/E8DecoderCycleCount.d ./src/E8DecoderCycleCount.o

.PHONY: clean-src

