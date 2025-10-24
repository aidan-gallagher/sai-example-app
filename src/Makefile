CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include/sai
LDFLAGS = 

TARGET = sai_example
SOURCES = sai_example.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
