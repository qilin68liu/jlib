AT = @
CC = gcc -c
LD = gcc -o
DEBUG ?= no

SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include

TARGET = $(BINDIR)/main

INC = $(wildcard $(INCDIR)/*.h)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

CFLAGS = -I $(INCDIR) -I ../include -Wall -MMD

ifeq ($(DEBUG), yes)
CFLAGS += -g
endif

LDFLAGS = -L ../lib -ljlib

all: check-dir $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $@ $^ $(LDFLAGS)

check-dir:
	$(AT)if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
	$(AT)if [ ! -d $(BINDIR) ]; then mkdir $(BINDIR); fi

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ $< $(CFLAGS)

-include $(OBJDIR)/*.d

clean:
	rm -rfv $(OBJDIR)/* $(BINDIR)/*

.PHONY: all clean
