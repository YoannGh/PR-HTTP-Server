#			Programmation Repartie (4I400)
#		Makefile pour TD 10. Entrées-Sorties asynchrones


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc
LDFLAGS =-lrt -lpthread 
CFLAGS =-W -Wall -ansi -pedantic -Iinclude

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/

HC=

.SUFFIXES:
.PHONY: all clean test test-notifier_async test-suspendre_async test-temporiser_async test-remonte_async test-inverseur_async
all: $(BIN)notifier_async $(BIN)suspendre_async $(BIN)temporiser_async $(BIN)remonte_async $(BIN)inverseur_async 

test: test-notifier_async test-suspendre_async test-temporiser_async test-remonte_async test-inverseur_async

test-notifier_async: $(BIN)notifier_async
	-$$PWD/bin/notifier_async contenant.txt contenu

test-suspendre_async: $(BIN)suspendre_async
	-$$PWD/bin/suspendre_async contenant.txt contenu

test-temporiser_async: $(BIN)temporiser_async
	-$$PWD/bin/temporiser_async contenant.txt contenu

test-remonte_async: $(BIN)remonte_async
	-$$PWD/bin/remonte_async  4

test-inverseur_async: $(BIN)inverseur_async
	-$$PWD/bin/inverseur_async source.txt dest.txt


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
