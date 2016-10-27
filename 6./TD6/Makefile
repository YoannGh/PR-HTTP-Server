#			Programmation Repartie (4I400)
#		Makefile pour TD 6.  Pipes et tubes


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
.PHONY: all clean test test-pipe_maj test-serveur_maj test-client_maj
all: $(BIN)pipe_maj $(BIN)serveur_maj $(BIN)client_maj 

test: test-pipe_maj test-serveur_maj test-client_maj

test-pipe_maj: $(BIN)pipe_maj
	-echo abcd.ext | $$PWD/bin/pipe_maj

test-serveur_maj: $(BIN)serveur_maj
	-$$PWD/bin/serveur_maj minmaj &

test-client_maj: $(BIN)client_maj
	-$$PWD/bin/client_maj minmaj

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

