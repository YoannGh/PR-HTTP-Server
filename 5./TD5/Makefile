#			Programmation Repartie (4I400)
#		Makefile pour TD 5. Processus légers et fichiers


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

HC=include/producteur_consommateur.h
O=$(OBJ)lib_upper.o

.SUFFIXES:
.PHONY: all clean test test-thread_upper test-thread_uppermax test-producteur_consommateur test-producteurs_consommateurs
all: $(BIN)thread_upper $(BIN)thread_uppermax $(BIN)producteur_consommateur $(BIN)producteurs_consommateurs 

test: test-thread_upper test-thread_uppermax test-producteur_consommateur test-producteurs_consommateurs

test-thread_upper: $(BIN)thread_upper
	-bin/thread_upper fichier1 fichier2 fichier3 fichier4

test-thread_uppermax: $(BIN)thread_uppermax
	-bin/thread_uppermax 2 fichier1 fichier2 fichier3

test-producteur_consommateur: $(BIN)producteur_consommateur
	-echo "123456789" | bin/producteur_consommateur

test-producteurs_consommateurs: $(BIN)producteurs_consommateurs
	-echo "123456789" | bin/producteurs_consommateurs 5 3

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

