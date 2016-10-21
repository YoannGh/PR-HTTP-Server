#			Programmation Repartie (4I400)
#		Makefile pour TM 5. Processus légers et signaux


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc -pthread 
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
.PHONY: all clean test test-signal_thread test-thread_arborescent_sync
all: $(BIN)signal_thread $(BIN)thread_arborescent_sync 

test: test-signal_thread test-thread_arborescent_sync

test-signal_thread: $(BIN)signal_thread
	-bin/signal_thread 4

test-thread_arborescent_sync: $(BIN)thread_arborescent_sync
	-bin/thread_arborescent_sync 4

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

