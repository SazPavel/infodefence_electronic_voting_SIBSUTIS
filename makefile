LDIR=./lib
CC=gcc
CFLAGS= -Wall
LDFLAGS= -Llib -lcrypto -lciphers -linfodef -lrandombytes -lm
TARGET= server client
HASH=5

.PHONY: all clean $(TARGET)

default: $(TARGET) clean
all: default

%.o: %.c
	$(CC) -DHASH=$(HASH) $(CFLAGS) -c $< -o $@

$(TARGET): %: %.o
	$(CC) $<  $(LDFLAGS) -o $@

clean:
	-rm -f *.o
