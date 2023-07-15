################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/config-utils.c \
../src/utils/file-utils.c \
../src/utils/logger-utils.c \
../src/utils/socket-utils.c 

C_DEPS += \
./src/utils/config-utils.d \
./src/utils/file-utils.d \
./src/utils/logger-utils.d \
./src/utils/socket-utils.d 

OBJS += \
./src/utils/config-utils.o \
./src/utils/file-utils.o \
./src/utils/logger-utils.o \
./src/utils/socket-utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c src/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Documents/TP-Tuki/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-utils

clean-src-2f-utils:
	-$(RM) ./src/utils/config-utils.d ./src/utils/config-utils.o ./src/utils/file-utils.d ./src/utils/file-utils.o ./src/utils/logger-utils.d ./src/utils/logger-utils.o ./src/utils/socket-utils.d ./src/utils/socket-utils.o

.PHONY: clean-src-2f-utils

