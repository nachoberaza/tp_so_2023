################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/config-utils.c \
../src/utils/logger-utils.c 

C_DEPS += \
./src/utils/config-utils.d \
./src/utils/logger-utils.d 

OBJS += \
./src/utils/config-utils.o \
./src/utils/logger-utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c src/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-utils

clean-src-2f-utils:
	-$(RM) ./src/utils/config-utils.d ./src/utils/config-utils.o ./src/utils/logger-utils.d ./src/utils/logger-utils.o

.PHONY: clean-src-2f-utils

