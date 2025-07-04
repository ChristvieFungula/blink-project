# MCU och CPU-hastighet
MCU = atmega328p
F_CPU = 16000000UL

# Verktyg
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Filer
SRC = main.c gpio_driver.c
OBJ = $(SRC:.c=.o)
TARGET = blink.elf
HEX = blink.hex

# Standardmål
all: $(HEX)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flasha till Arduino via avrdude
flash: $(HEX)
	avrdude -c arduino -p m328p -P /dev/tty.usbserial-1410 -b 115200 -U flash:w:$(HEX):i

# Rensa
clean:
	rm -f *.o *.elf *.hex
