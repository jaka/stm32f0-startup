OPTS += -g -O0 -mlittle-endian -mthumb -mcpu=cortex-m0 
CFLAGS += -Wall -Werror -Wextra -pedantic -std=c99 -ffunction-sections -fdata-sections -finline-functions -fno-tree-loop-distribute-patterns
LDFLAGS += -nostdlib --gc-sections
LIBS =

PREFIX := arm-none-eabi
CC := $(PREFIX)-gcc
LD := $(PREFIX)-ld
OBJCOPY := $(PREFIX)-objcopy

SRC := main.c
OBJ := $(patsubst %.c,%.o,$(SRC)) startup.o
NAME := blink

all: $(NAME).bin

%.o: %.c
	$(CC) $(OPTS) $(CFLAGS) -o $@ -c $^

$(NAME).elf: $(OBJ)
	$(LD) $(LDFLAGS) -T startup.ld -o $@ $^

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

size: $(NAME).elf
	$(PREFIX)-size $^

flash: $(NAME).bin
	st-flash write $(NAME).bin 0x8000000

clean:
	rm -f $(OBJ)
	rm -f $(NAME).elf
	rm -f $(NAME).bin
