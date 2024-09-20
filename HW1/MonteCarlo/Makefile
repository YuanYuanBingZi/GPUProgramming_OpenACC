CC=gcc                               
CFLAGS=-fopenacc                
LDFLAGS=-lm                          
TARGET=pi                 
SRC=pi.c                     


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

