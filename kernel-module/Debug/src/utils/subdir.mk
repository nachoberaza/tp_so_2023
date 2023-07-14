################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/config-utils.c \
../src/utils/cpu-communication-utils.c \
../src/utils/file-system-communication-utils.c \
../src/utils/logger-utils.c \
../src/utils/long-term-scheduler-utils.c \
../src/utils/memory-communication-utils.c \
../src/utils/pcb-utils.c \
../src/utils/resource-utils.c \
../src/utils/semaphore-utils.c \
../src/utils/short-term-scheduler-utils.c \
../src/utils/socket-utils.c \
../src/utils/thread-utils.c 

C_DEPS += \
./src/utils/config-utils.d \
./src/utils/cpu-communication-utils.d \
./src/utils/file-system-communication-utils.d \
./src/utils/logger-utils.d \
./src/utils/long-term-scheduler-utils.d \
./src/utils/memory-communication-utils.d \
./src/utils/pcb-utils.d \
./src/utils/resource-utils.d \
./src/utils/semaphore-utils.d \
./src/utils/short-term-scheduler-utils.d \
./src/utils/socket-utils.d \
./src/utils/thread-utils.d 

OBJS += \
./src/utils/config-utils.o \
./src/utils/cpu-communication-utils.o \
./src/utils/file-system-communication-utils.o \
./src/utils/logger-utils.o \
./src/utils/long-term-scheduler-utils.o \
./src/utils/memory-communication-utils.o \
./src/utils/pcb-utils.o \
./src/utils/resource-utils.o \
./src/utils/semaphore-utils.o \
./src/utils/short-term-scheduler-utils.o \
./src/utils/socket-utils.o \
./src/utils/thread-utils.o 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c src/utils/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Documents/dev/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library" -I"/home/utnso/Documents/dev/tp-2023-1c-Compu-mundo-hiper-mega-red" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-utils

clean-src-2f-utils:
	-$(RM) ./src/utils/config-utils.d ./src/utils/config-utils.o ./src/utils/cpu-communication-utils.d ./src/utils/cpu-communication-utils.o ./src/utils/file-system-communication-utils.d ./src/utils/file-system-communication-utils.o ./src/utils/logger-utils.d ./src/utils/logger-utils.o ./src/utils/long-term-scheduler-utils.d ./src/utils/long-term-scheduler-utils.o ./src/utils/memory-communication-utils.d ./src/utils/memory-communication-utils.o ./src/utils/pcb-utils.d ./src/utils/pcb-utils.o ./src/utils/resource-utils.d ./src/utils/resource-utils.o ./src/utils/semaphore-utils.d ./src/utils/semaphore-utils.o ./src/utils/short-term-scheduler-utils.d ./src/utils/short-term-scheduler-utils.o ./src/utils/socket-utils.d ./src/utils/socket-utils.o ./src/utils/thread-utils.d ./src/utils/thread-utils.o

.PHONY: clean-src-2f-utils

