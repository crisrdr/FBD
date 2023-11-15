CC = gcc -g
CFLAGS = -Wall -Wextra
LDLIBS = -lodbc

# recompile if this heder changes
HEADERS = odbc.h customers.h orders.h products.h

EXE = menu
OBJ = odbc.o products.o orders.o customers.o $(EXE).o

all : clean $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

$(EXE).o: $(EXE).c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

customers.o: customers.c customers.h
	$(CC) $(CFLAGS) -c -o $@ $<

products.o: products.c products.h odbc.h
	$(CC) $(CFLAGS) -c -o $@ $<

orders.o: orders.c orders.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f *.o core $(EXE) splint.log

splint:
	splint -nullpass *.c *.h | tee splint.log