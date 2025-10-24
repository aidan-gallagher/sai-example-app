CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include/sai
LDFLAGS = -lsai -lpthread

TARGET = sai_app

.PHONY: all clean

all: $(TARGET)

$(TARGET): sai_app.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)
