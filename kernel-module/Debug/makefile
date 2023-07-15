################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/utils/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := kernel-module
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: kernel-module

dependents:
	-cd /home/utnso/Documents/TP-Tuki/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library/Debug && $(MAKE) clean all

# Tool invocations
kernel-module: $(OBJS) $(USER_OBJS) /home/utnso/Documents/TP-Tuki/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library/Debug/libshared-library.so makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/Documents/TP-Tuki/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library/Debug" -o "kernel-module" $(OBJS) $(USER_OBJS) $(LIBS) -lshared-library -lcommons
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) kernel-module
	-@echo ' '

.PHONY: all clean dependents main-build
/home/utnso/Documents/TP-Tuki/tp-2023-1c-Compu-mundo-hiper-mega-red/shared-library/Debug/libshared-library.so:

-include ../makefile.targets
