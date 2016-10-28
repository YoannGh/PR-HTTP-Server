#			Programmation Repartie (4I400)
#		Makefile pour TM 6. Introduction à la communication inter-processus (IPC)


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc 
LDFLAGS = -lrt 
CFLAGS =-W -Wall -ansi -pedantic -Iinclude 

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/

HC=include/remonte.c
O=

.SUFFIXES:
.PHONY: all clean test test-remonte_posix_partagee test-remonte_sv_partagee test-remonte_ipc test-multi_remonte test-multi_remonte_file_unique
all: $(BIN)remonte_posix_partagee $(BIN)remonte_sv_partagee $(BIN)remonte_ipc $(BIN)multi_remonte $(BIN)multi_remonte_file_unique 

test: test-remonte_posix_partagee test-remonte_sv_partagee test-remonte_ipc test-multi_remonte test-multi_remonte_file_unique

test-remonte_posix_partagee: $(BIN)remonte_posix_partagee
	-$$PWD/bin/remonte_posix_partagee 4

test-remonte_sv_partagee: $(BIN)remonte_sv_partagee
	-$$PWD/bin/remonte_sv_partagee 4

test-remonte_ipc: $(BIN)remonte_ipc
	-$$PWD/bin/remonte_ipc 4

test-multi_remonte: $(BIN)multi_remonte
	-bin/multi_remonte 4

test-multi_remonte_file_unique: $(BIN)multi_remonte_file_unique
	-bin/multi_remonte_file_unique 4

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

