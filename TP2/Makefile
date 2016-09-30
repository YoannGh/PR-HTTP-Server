#			Programmation Repartie (4I400)
#		Makefile pour TM 2. Toujours des fichiers


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc
LDFLAGS =
CFLAGS =-W -Wall -ansi -pedantic -Iinclude

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/

HC=
O=

.SUFFIXES:
.PHONY: all clean test test-extended-grep test-inverser-fichier test-inverser-pread
all: $(BIN)extended-grep $(BIN)inverser-fichier $(BIN)inverser-pread $(OBJ)liste-rep.o

test: test-extended-grep test-inverser-fichier test-inverser-pread

test-extended-grep: $(BIN)extended-grep
	-bin/extended-grep if src

test-inverser-fichier: $(BIN)inverser-fichier
	-bin/inverser-fichier src/inverser-fichier.c

test-inverser-pread: $(BIN)inverser-pread
	-bin/inverser-pread src/inverser-pread.c

clean: 
	rm -rf $(OBJ)*.o $(BIN)*

$(BIN)%: $(OBJ)%.o $O
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

