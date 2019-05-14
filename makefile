AT = @
CC = gcc -c
AR = ar rsc
DEBUG ?= no

SRCDIR = ./src
OBJDIR = ./obj
LIBDIR = ./lib
INCDIR = ./include
INSTALL_INCDIR = /usr/local/include
INSTALL_LIBDIR = /usr/local/lib

TARGET = $(LIBDIR)/libjlib.a

INC = $(wildcard $(INCDIR)/*.h)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

INSTALL_INC = $(patsubst $(INCDIR)/%,$(INSTALL_INCDIR)/%,$(INC))
INSTALL_LIB = $(patsubst $(LIBDIR)/%,$(INSTALL_LIBDIR)/%,$(TARGET))

CFLAGS = -I $(INCDIR) -Wall -MMD
ifeq ($(DEBUG), yes)
CFLAGS += -g
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(AT)$(AR) $@ $^

#check-dir:
#	$(AT)if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
#	$(AT)if [ ! -d $(LIBDIR) ]; then mkdir $(LIBDIR); fi

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

install: $(TARGET)
	$(AT)cp $(INC) $(INSTALL_INCDIR)
	$(AT)cp $(TARGET) $(INSTALL_LIBDIR)
	$(AT)echo "\033[32m[OK]\033[0m jlib has been installed successfully"

uninstall:
	$(AT)rm -f $(INSTALL_INC)
	$(AT)rm -f $(INSTALL_LIB)
	$(AT)echo "\033[32m[OK]\033[0m jlib has been uninstalled successfully"

-include $(OBJDIR)/*.d

clean:
	rm -rf $(OBJDIR)/* $(LIBDIR)/*

.PHONY: all install uninstall clean
