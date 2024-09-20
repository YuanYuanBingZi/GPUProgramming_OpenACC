CC=gcc                               
CFLAGS=-fopenacc                
LDFLAGS=-lm                          
TARGET=matmult             
SRC=matmult.c                     


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)