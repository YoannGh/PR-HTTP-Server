#			Programmation Repartie (4I400)
#		Makefile pour TD 2. Fonctions sur fichiers.


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
.PHONY: all clean test test-memefichier test-droitsfichier test-remplacedansfichier test-rediriger
all: $(BIN)memefichier $(BIN)droitsfichier $(BIN)remplacedansfichier $(BIN)rediriger 

test: test-memefichier test-droitsfichier test-remplacedansfichier test-rediriger

test-memefichier: $(BIN)memefichier
	-$$PWD/bin/memefichier $$PWD/src/memefichier.c $$PWD/src/../src/memefichier.c

test-droitsfichier: $(BIN)droitsfichier
	-$$PWD/bin/droitsfichier c makefile W

test-remplacedansfichier: $(BIN)remplacedansfichier
	-$$PWD/bin/remplacedansfichier texte toi a moi de jouer

test-rediriger: $(BIN)rediriger
	-$$PWD/bin/rediriger trace.txt

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

