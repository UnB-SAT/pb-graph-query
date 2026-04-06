CC = gcc
INCDIR = inc
SRCDIR = src
OBJDIR = obj
CFLAGS = -c -g -O0 -pthread -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = ./gdb-sat.out

all: clean $(EXE)

$(EXE): $(OBJ)
				$(CC) $(OBJDIR)/*.o -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
								@mkdir -p $(@D)
							 	$(CC) $(CFLAGS) $< -o $@

clean:
			-rm -f $(OBJDIR)/*.o $(EXE) 
