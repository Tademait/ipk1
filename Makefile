CC = gcc
CFLAGS = -Wall
SOURCE = serverside.c
TARGET = hinfosvc
all:
	$(CC) $(SOURCE) -o $(TARGET)

run:
	$(CC) $(SOURCE) -o $(TARGET)
	./$(TARGET) 12345

clean:
	rm $(TARGET)