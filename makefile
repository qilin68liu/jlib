CC = gcc -c
AR = ar rsc
DEBUG ?= no

SRCDIR = ./src
OBJDIR = ./obj
LIBDIR = ./lib
INCDIR = ./include
TARGET = $(LIBDIR)/libjlib.a

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRC))

CFLAGS = -I $(INCDIR) -Wall -MMD
ifeq ($(DEBUG), yes)
CFLAGS += -g
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AR) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

-include $(OBJDIR)/*.d

clean:
	rm -rf $(OBJDIR)/* $(LIBDIR)/*

.PHONY: all clean
