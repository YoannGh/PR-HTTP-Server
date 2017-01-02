#			Programmation Repartie (4I400)
#		Makefile pour TD 9. Socket et parallélisme


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc -DDEBUG
LDFLAGS = -lpthread
CFLAGS =-W -Wall -ansi -pedantic -Iinclude -std=c11 

DIR=.
BIN=$(DIR)/bin/
OBJ=$(DIR)/obj/
INCLUDE=$(DIR)/include/
LIB=$(DIR)/lib/
SRC=$(DIR)/src/
TEST=$(DIR)/test/

SOURCES = 	$(SRC)client.c\
	  		$(SRC)http_request.c \
	  		$(SRC)http_server.c \
	  		$(SRC)logger.c \
	  		$(SRC)main.c \
	  		$(SRC)mime_parser.c \
	  		$(SRC)util.c \

OBJS = 	$(OBJ)client.o\
	  	$(OBJ)http_request.o \
	  	$(OBJ)http_server.o \
	  	$(OBJ)logger.o \
	  	$(OBJ)main.o \
	  	$(OBJ)mime_parser.o \
	  	$(OBJ)util.o \

EXE = server

.SUFFIXES:
.PHONY: all clean test server
all: $(EXE) #$(BIN)serveur $(BIN)client

server: $(OBJS)
	$(CC) -o $(BIN)$@ $^ $(LDFLAGS)

test: $(TEST)test-mime_parser $(TEST)test-logger

$(TEST)test-mime_parser: $(OBJ)mime_parser.o $(OBJ)util.o
	@if [ -d $(TEST) ]; then : ; else mkdir $(TEST); fi
	$(CC) -o $@ $^ $(LDFLAGS)
$(TEST)test-logger: $(OBJ)util.o $(OBJ)logger.o
	@if [ -d $(TEST) ]; then : ; else mkdir $(TEST); fi
	$(CC) -o $@ $^ $(LDFLAGS)

# test: test-ftp_multi_serveur test-journal_serveur

# test-ftp_multi_serveur: $(BIN)ftp_multi_serveur
# 	-$$PWD/bin/ftp_multi_serveur 2000 /tmp &

# test-journal_serveur: $(BIN)journal_serveur
# 	-$$PWD/bin/journal_serveur 2820 2821 2822 2823 &


$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN)client: $(OBJ)client.o $(OBJ)http_request.o $(OBJ)mime_parser.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm -f $(OBJ)*.o $(BIN)*
