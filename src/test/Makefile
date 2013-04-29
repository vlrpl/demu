NAME := i8086

BIN := $(NAME).bin
ELF := $(NAME).elf
OBJ := $(NAME).o

all: $(BIN)

$(BIN): $(ELF)
	objcopy -O binary bin/$< bin/$@

$(ELF): $(OBJ)
	ld -nostdlib -static $< -o bin/$@

%.o: %.S
	gcc -nostdinc -c $< -o $@

clean:
	rm -f bin/$(BIN) bin/$(ELF) $(OBJ)

.PHONY: clean
