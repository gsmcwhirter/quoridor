CC?=gcc-6
SRC?=src
DEPS=$(SRC)/deps
BUILD=build

CFLAGS=-O2 -Wall -Wextra -Isrc -Isrc/deps
TESTSOURCES=$(wildcard $(SRC)/*.c $(DEPS)/term/term.c)

.PHONY: clib

all: $(DEPS) clib $(BUILD) test

$(BUILD):
	@mkdir -p $(BUILD)

test: $(BUILD)
	$(CC) $(CFLAGS) $(TESTSOURCES) -o $(BUILD)/$@

$(DEPS):
	@mkdir -p $(DEPS)

clib: $(DEPS)
	clib install -o $(DEPS) term
	clib install -o $(DEPS) commander
	clib install -o $(DEPS) ms
	clib install -o $(DEPS) hash
	clib install -o $(DEPS) timer
	clib install -o $(DEPS) coro
	clib install -o $(DEPS) stephenmathieson/case.c
	clib install -o $(DEPS) stephenmathieson/substr.c
	clib install -o $(DEPS) stephenmathieson/trim.c
	clib install -o $(DEPS) thlorenz/log.h
