IDIR =../include
CC=gcc
CFLAGI=-I$(IDIR)
CFLAGCUI=-I../include
GDB=-g

ODIR=../obj

#_DEPS = tabelas.h montador.h
_DEPS = tabelas.h ligador.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = tabelas.o montador.o
_OBJ = tabelas.o ligador.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -Wall -c -o $@ $< $(CFLAGI)

ligador: $(OBJ)
	$(CC) -Wall $(CFLAGCUI) $^ -o $@ $(CFLAGI) $(GDB)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
