MCU:=atmega328p
F_CPU:=16000000UL
BUILD:=build
TARGET:=$(BUILD)/blink
SRCS:=main.c gpio_driver.c
OBJS:=$(SRCS:%.c=$(BUILD)/%.o)

CC:=avr-gcc
OBJCOPY:=avr-objcopy
CFLAGS:=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -std=c11 -Wall -Wextra -ffunction-sections -fdata-sections
LDFLAGS:=-mmcu=$(MCU) -Wl,--gc-sections

all: $(BUILD) $(TARGET).hex

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	avrdude -v -p$(MCU) -carduino -P $(PORT) -b$(BAUD) -D -U flash:w:$(TARGET).hex:i

clean:
	rm -rf $(BUILD)
