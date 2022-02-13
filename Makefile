CC = gcc
CFLAGS = -Wall
SOURCE = serverside.c
TARGET = hinfosvc
all:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

run:
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)
	./$(TARGET) 12345

clean:
	rm $(TARGET)