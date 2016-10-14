#			Programmation Repartie (4I400)
#		Makefile pour TM 4. Synchronisation de threads


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc
LDFLAGS = -lpthread
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
.PHONY: all clean test test-thread_detach test-thread_setdetach test-thread_broadcast
all: $(BIN)thread_detach $(BIN)thread_setdetach $(BIN)thread_broadcast 

test: test-thread_detach test-thread_setdetach test-thread_broadcast

test-thread_detach: $(BIN)thread_detach
	-$$PWD/bin/thread_detach 10

test-thread_setdetach: $(BIN)thread_setdetach
	-$$PWD/bin/thread_setdetach 10

test-thread_broadcast: $(BIN)thread_broadcast
	-$$PWD/bin/thread_broadcast 10

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

