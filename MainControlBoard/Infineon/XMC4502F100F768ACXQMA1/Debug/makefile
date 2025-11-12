################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Libraries/XMCLib/src/subdir.mk
-include Libraries/Newlib/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
XMC4502F100F768ACXQMA1.hex \

SECONDARY_LIST += \
XMC4502F100F768ACXQMA1.lst \

SECONDARY_SIZE += \
XMC4502F100F768ACXQMA1.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "XMC4502F100F768ACXQMA1.map"

TOOLCHAIN_ROOT=D:/Programy/4.5.0.202105191637/eclipse/ARM-GCC-49
PROJECT_LOC=D:/Projekty/TrinityTrack6000/MainControlBoard/Infineon/XMC4502F100F768ACXQMA1

# All Target
all: XMC4502F100F768ACXQMA1.elf secondary-outputs

# Tool invocations
XMC4502F100F768ACXQMA1.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,$(MAP_FILE) -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb -g -gdwarf-2 -o "XMC4502F100F768ACXQMA1.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
XMC4502F100F768ACXQMA1.hex: XMC4502F100F768ACXQMA1.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "XMC4502F100F768ACXQMA1.elf" "XMC4502F100F768ACXQMA1.hex"
	@echo 'Finished building: $@'
	@echo.
XMC4502F100F768ACXQMA1.lst: XMC4502F100F768ACXQMA1.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "XMC4502F100F768ACXQMA1.elf" > "XMC4502F100F768ACXQMA1.lst"
	@echo 'Finished building: $@'
	@echo.
XMC4502F100F768ACXQMA1.siz: XMC4502F100F768ACXQMA1.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "XMC4502F100F768ACXQMA1.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) XMC4502F100F768ACXQMA1.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
