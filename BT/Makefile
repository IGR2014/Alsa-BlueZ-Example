OBJ		= btAddress.o btDevice.o btCore.o


.PHONY: all clean install

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(OBJ)

clean:
	rm -rf $(OBJ) $(EXE)

