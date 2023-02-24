CC=g++
CFLAGS=-Wall
OBJS=main.o Math.o Scene.o Object.o
LINKER_FLAGS=-lSDL2
TARGET=out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp Math.h Core.h Scene.h Object.h
	$(CC) $(CFLAGS) -c main.cpp

Math.o: Math.h Core.h

Scene.o: Math.h Core.h Object.h

clean:
	rm -rf $(OBJS) $(TARGET)
