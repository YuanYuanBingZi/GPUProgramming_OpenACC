CC=gcc                               
CFLAGS=-fopenacc                
LDFLAGS=-lm                          
TARGET=stream                
SRC=stream.c                    



all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


clean:
	rm -f $(TARGET)
