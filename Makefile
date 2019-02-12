export CC		= g++
export CFLAGS		= -I. -pedantic -O3 -Wall -w -Wextra -std=gnu++11
export LIBS		= -lbluetooth -lasound -lm
OBJ			= main.o BT/btAddress.o BT/btDevice.o BT/btCore.o
EXE			= bluez-c
prefix			= /usr
DESTDIR			=


.PHONY: all clean install

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $@ $(EXE)

clean:
	rm -rf $(OBJ) $(EXE)

install : $(DESTDIR)/$(prefix)/bin
	cp -a $(EXE) $(DESTDIR)/$(prefix)/bin/

$(DESTDIR)/$(prefix)/bin:
	mkdir -p $@

