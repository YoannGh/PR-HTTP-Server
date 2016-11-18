#			Programmation Repartie (4I400)
#		Makefile pour TM 7. Une messagerie instantanée en mémoire partagée


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

HC=include/chat_common.h

.SUFFIXES:
.PHONY: all clean test test-chat_server test-chat_client
all: $(BIN)chat_server $(BIN)chat_client 

test: test-chat_server test-chat_client

test-chat_server: $(BIN)chat_server
	-bin/chat_server semserver &

test-chat_client: $(BIN)chat_client
	-bin/chat_client semclient semserver


$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm -rf $(OBJ)*.o $(BIN)*
