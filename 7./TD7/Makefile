#			Programmation Repartie (4I400)
#		Makefile pour TD 7. Introduction aux sémaphores


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

HC=include/producteur_consommateur.h

.SUFFIXES:
.PHONY: all clean test test-posix_barrier test-wait_barrier test-prod_conso_partagees test-sem_affiche
all: $(BIN)posix_barrier $(BIN)sem_affiche $(BIN)wait_barrier $(BIN)prod_conso_partagees 

test: test-posix_barrier test-wait_barrier test-prod_conso_partagees test-sem_affiche

test-posix_barrier: $(BIN)posix_barrier
	-bin/posix_barrier 4

test-wait_barrier: $(BIN)wait_barrier
	-bin/wait_barrier 4

test-prod_conso_partagees: $(BIN)prod_conso_partagees
	-echo "123456789" | bin/prod_conso_partagees 5 3

test-sem_affiche: $(BIN)sem_affiche
	-$$PWD/bin/sem_affiche 7


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
