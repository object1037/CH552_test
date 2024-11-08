CC := sdcc
OBJCOPY := objcopy

TARGET := app.bin
SRCDIR := src
OUTDIR := bin
INCLUDES := include

RELS := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.rel, $(wildcard $(SRCDIR)/*.c))
MAIN := $(OUTDIR)/main.rel
DEPS := $(filter-out $(MAIN), $(RELS))

XRAM_SIZE ?= 0x0400
IRAM_SIZE ?= 0x0100
CODE_SIZE ?= 0x3800

CFLAGS := -mmcs51 \
					--model-large \
					--xram-size $(XRAM_SIZE) \
					--iram-size $(IRAM_SIZE) \
					--code-size $(CODE_SIZE) \
					--stack-auto \
					-I./$(INCLUDES)

$(OUTDIR)/%.rel: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/$(TARGET): $(OUTDIR)/$(TARGET:.bin=.ihx)
	$(OBJCOPY) -I ihex -O binary $< $@

$(OUTDIR)/$(TARGET:.bin=.ihx): $(RELS)
	$(CC) $(MAIN) $(DEPS) $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*
