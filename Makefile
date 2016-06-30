CC?=gcc-6
SRC?=src
DEPS=$(SRC)/deps
BUILD=build
WALLSCORE?=1
PATHSCORE?=1

CFLAGS=-O2 -std=c11 -Wall -Wextra -Isrc -Isrc/deps -Isrc/lib -pthread -DWALL_SCORE=$(WALLSCORE) -DPATH_SCORE=$(PATHSCORE)
# -pg, -DDEBUG, -DSEVERAL -DDEBUGAI -DDEBUGAI2 -DDEBUGAI3
# LIBSOURCES=$(wildcard $(SRC)/lib/*.c $(DEPS)$(DEPS)/term/term.c $(DEPS)/commander/commander.c $(DEPS)/strdup/strdup.c $(DEPS)/case/case.c $(DEPS)/occurrences/occurrences.c $(DEPS)/trim/trim.c $(DEPS)/strsplit/strsplit.c)
LIBSOURCES=$(wildcard $(SRC)/lib/*.c $(DEPS)/**/*.c)

.PHONY: clib

all: $(DEPS) clib $(BUILD) test game

$(BUILD):
	@mkdir -p $(BUILD)

game test: $(BUILD)
	$(CC) $(CFLAGS) src/$@.c $(LIBSOURCES) -o $(BUILD)/$@

$(DEPS):
	@mkdir -p $(DEPS)

clib: $(DEPS)
	clib install -o $(DEPS) term
	clib install -o $(DEPS) commander
	clib install -o $(DEPS) strdup
	clib install -o $(DEPS) timer
	clib install -o $(DEPS) stephenmathieson/case.c
	clib install -o $(DEPS) stephenmathieson/occurrences.c
	clib install -o $(DEPS) stephenmathieson/trim.c
	clib install -o $(DEPS) jwerle/strsplit.c

	# clib install -o $(DEPS) ms
	# clib install -o $(DEPS) hash
	# clib install -o $(DEPS) coro
	# clib install -o $(DEPS) stephenmathieson/substr.c
	# clib install -o $(DEPS) thlorenz/log.h
