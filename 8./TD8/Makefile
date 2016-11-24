#			Programmation Repartie (4I400)
#		Makefile pour TD 8. Introduction aux sockets


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

.SUFFIXES:
.PHONY: all clean test test-remonte_udp test-env_serveur test-env_client test-recvfile test-sendfile
all: $(BIN)remonte_udp $(BIN)env_serveur $(BIN)env_client $(BIN)recvfile $(BIN)sendfile 

test: test-remonte_udp test-env_serveur test-env_client test-recvfile test-sendfile

test-remonte_udp: $(BIN)remonte_udp
	-$$PWD/bin/remonte_udp sockudp 4

test-env_serveur: $(BIN)env_serveur
	-$$PWD/bin/env_serveur 2001 &

test-env_client: $(BIN)env_client
	-echo "S USER moi;G USER;Q;"|tr ";" "\n"|$$PWD/bin/env_client 127.0.0.1 2001

test-recvfile: $(BIN)recvfile
	-$$PWD/bin/recvfile 2000 &

test-sendfile: $(BIN)sendfile
	-$$PWD/bin/sendfile 127.0.0.1 2000 makefile


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
