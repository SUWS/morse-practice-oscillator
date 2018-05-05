# WinAVR cross-compiler toolchain is used here
CC = avr-gcc
OBJCOPY = avr-objcopy
DUDE = avrdude

CFLAGS = -Wall -Os -mmcu=atmega328
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p atmega328p -c usbasp -F -v

# Object files for the firmware (usbdrv/oddebug.o not strictly needed I think)
OBJECTS = morse_code_avr.o uart.o

# By default, build the firmware and command-line client, but do not flash
all: morse_code_avr.hex

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: morse_code_avr.hex
	$(DUDE) $(DUDEFLAGS) -U flash:w:$<

eeprom: morse_code_avr.eep
	$(DUDE) $(DUDEFLAGS) -U eeprom:w:$<

# Housekeeping if you want it
clean:
	$(RM) *.o *.hex *.elf

# From .elf file to .hex
%.hex: %.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just main.o
morse_code_avr.elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change
# the config! I spent a few hours debugging because of this...
$(OBJECTS):

# From C source to .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
