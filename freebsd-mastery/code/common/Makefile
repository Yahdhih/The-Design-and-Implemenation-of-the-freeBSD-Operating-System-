# Makefile générique — FreeBSD Mastery
# Utilisation : cc (clang sur FreeBSD/macOS) ou gcc sur Linux
# Remplace CC=cc par CC=gcc si nécessaire.

CC      = cc
CFLAGS  = -Wall -Wextra -g -std=c11
SRCS    = $(wildcard *.c)
BINS    = $(SRCS:.c=)

.PHONY: all clean

all: $(BINS)

# Règle générique : compile chaque .c en binaire du même nom
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) *.o core *.core

# Pour lier utils.c depuis common/ dans un sous-projet :
# COMMON = ../common/utils.c
# target: target.c $(COMMON)
#     $(CC) $(CFLAGS) -o $@ $^
