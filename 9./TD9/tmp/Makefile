#			Programmation Repartie (4I400)
#		Makefile pour TD 9. Socket et parallélisme


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

.SUFFIXES:
.PHONY: all clean test test-ftp_multi_serveur test-journal_serveur
all: $(BIN)ftp_multi_serveur $(BIN)sendfile $(BIN)journal_serveur $(OBJ)journal_client.o

test: test-ftp_multi_serveur test-journal_serveur

test-ftp_multi_serveur: $(BIN)ftp_multi_serveur
	-$$PWD/bin/ftp_multi_serveur 2000 /tmp &

test-journal_serveur: $(BIN)journal_serveur
	-$$PWD/bin/journal_serveur 2820 2821 2822 2823 &


$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm $(OBJ)*.o $(BIN)*
