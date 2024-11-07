CC := sdcc
OBJCOPY := objcopy

TARGET := main
SRCDIR := src
OUTDIR := bin

XRAM_SIZE ?= 0x0400
IRAM_SIZE ?= 0x0100
CODE_SIZE ?= 0x3800

CFLAGS := -mmcs51 --model-large --xram-size $(XRAM_SIZE) --iram-size $(IRAM_SIZE) --code-size $(CODE_SIZE) --stack-auto

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/$(TARGET).ihx
	$(OBJCOPY) -I ihex -O binary $< $@

$(OUTDIR)/$(TARGET).ihx: $(SRCDIR)/$(TARGET).c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*
