CC := sdcc
OBJCOPY := objcopy
LINTER := splint

TARGET := app.bin
SRCDIR := src
OUTDIR := bin
INCLUDES := ./include
SYS_INCLUDES := /opt/homebrew/share/sdcc/include/mcs51

RELS := $(patsubst $(SRCDIR)/%.c, $(OUTDIR)/%.rel, $(wildcard $(SRCDIR)/*.c))
MAIN := $(OUTDIR)/main.rel
DEPS := $(filter-out $(MAIN), $(RELS))

XRAM_SIZE ?= 0x0300
XRAM_LOC ?= 0x0100
IRAM_SIZE ?= 0x0100
CODE_SIZE ?= 0x3800

CFLAGS := -mmcs51 \
					--model-large \
					--xram-size $(XRAM_SIZE) \
					--xram-loc $(XRAM_LOC) \
					--iram-size $(IRAM_SIZE) \
					--code-size $(CODE_SIZE) \
					--stack-auto \
					-I$(INCLUDES)

$(OUTDIR)/%.rel: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/$(TARGET): $(OUTDIR)/$(TARGET:.bin=.ihx)
	$(OBJCOPY) -I ihex -O binary $< $@

$(OUTDIR)/$(TARGET:.bin=.ihx): $(RELS)
	$(CC) $(MAIN) $(DEPS) $(CFLAGS) -o $@

.PHONY: lint

lint:
	$(LINTER) -I$(SYS_INCLUDES) -I$(INCLUDES) -preproc $(SRCDIR)/*.c

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*
