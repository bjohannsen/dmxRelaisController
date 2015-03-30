################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
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
LSS += \
DMX_dosencontroller.lss \

FLASH_IMAGE += \
DMX_dosencontroller.hex \

EEPROM_IMAGE += \
DMX_dosencontroller.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: DMX_dosencontroller.elf secondary-outputs

# Tool invocations
DMX_dosencontroller.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,DMX_dosencontroller.map -mmcu=attiny2313 -o"DMX_dosencontroller.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DMX_dosencontroller.lss: DMX_dosencontroller.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S DMX_dosencontroller.elf  >"DMX_dosencontroller.lss"
	@echo 'Finished building: $@'
	@echo ' '

DMX_dosencontroller.hex: DMX_dosencontroller.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex DMX_dosencontroller.elf  "DMX_dosencontroller.hex"
	@echo 'Finished building: $@'
	@echo ' '

DMX_dosencontroller.eep: DMX_dosencontroller.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex DMX_dosencontroller.elf  "DMX_dosencontroller.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: DMX_dosencontroller.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=attiny2313 DMX_dosencontroller.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) DMX_dosencontroller.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
