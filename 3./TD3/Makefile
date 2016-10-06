#			Programmation Repartie (4I400)
#		Makefile pour TD 3. Signaux et processus


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
.PHONY: all clean test test-chaine_proc test-kill_proc test-stop_and_go
all: $(BIN)chaine_proc $(BIN)kill_proc $(BIN)stop_and_go 

test: test-chaine_proc test-kill_proc test-stop_and_go

test-chaine_proc: $(BIN)chaine_proc
	-$$PWD/bin/chaine_proc 10

test-kill_proc: $(BIN)kill_proc
	-$$PWD/bin/kill_proc 10

test-stop_and_go: $(BIN)stop_and_go
	-$$PWD/bin/stop_and_go 10

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

