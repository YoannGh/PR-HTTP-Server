
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
	  		$(SRC)antidos.c \
	  		$(SRC)http_server.c \
	  		$(SRC)list.c \
	  		$(SRC)logger.c \
	  		$(SRC)main.c \
	  		$(SRC)mime_parser.c \
	  		$(SRC)util.c \

OBJS = 	$(OBJ)client.o\
	  	$(OBJ)http_request.o \
	  	$(OBJ)antidos.o \
	  	$(OBJ)http_server.o \
	  	$(OBJ)list.o \
	  	$(OBJ)logger.o \
	  	$(OBJ)main.o \
	  	$(OBJ)mime_parser.o \
	  	$(OBJ)util.o \

EXE = server

.SUFFIXES:
.PHONY: all clean test server

all: $(EXE)

server: $(OBJS)
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $(BIN)$@ $^ $(LDFLAGS)

test: $(EXE)
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(BIN)server 80 8 1024000

$(BIN)%: $(OBJ)%.o
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

clean: 
	rm -f $(OBJ)*.o $(BIN)*