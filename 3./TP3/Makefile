#			Programmation Repartie (4I400)
#		Makefile pour TM 3. Signaux  de l'utilisateur


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
.PHONY: all clean test test-synchro_proc test-forkfilemax test-mon_petit_frere
all: $(BIN)synchro_proc $(BIN)forkfilemax $(BIN)mon_petit_frere 

test: test-synchro_proc test-forkfilemax test-mon_petit_frere

test-synchro_proc: $(BIN)synchro_proc
	-$$PWD/bin/synchro_proc

test-forkfilemax: $(BIN)forkfilemax
	-$$PWD/bin/forkfilemax

test-mon_petit_frere: $(BIN)mon_petit_frere
	-$$PWD/bin/mon_petit_frere

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

