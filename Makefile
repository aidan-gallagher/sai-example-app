CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lsaivs -lsaimetadata -lswsscommon -lpthread -lzmq -lhiredis

TARGET = sai_vs_example
SOURCES = src/sai_vs_example.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
