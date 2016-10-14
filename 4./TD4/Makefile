#			Programmation Repartie (4I400)
#		Makefile pour TD 4. Introduction aux processus légers


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
.PHONY: all clean test test-thread_create test-thread_rand test-thread_wait
all: $(BIN)thread_create $(BIN)thread_rand $(BIN)thread_wait 

test: test-thread_create test-thread_rand test-thread_wait

test-thread_create: $(BIN)thread_create
	-$$PWD/bin/thread_create 10

test-thread_rand: $(BIN)thread_rand
	-$$PWD/bin/thread_rand 10

test-thread_wait: $(BIN)thread_wait
	-$$PWD/bin/thread_wait 10

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

