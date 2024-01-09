CC=g++
CFLAGS=-Wall
OBJS=main.o Math.o Scene.o Object.o
LINKER_FLAGS=-lSDL2 -lSDL2_ttf
TARGET=out
DBG_TARGET=$(TARGET).dbg

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(TARGET) $(OBJS)

debug: $(DBG_TARGET)

$(DBG_TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(DBG_TARGET) $(OBJS)

main.o: main.cpp Math.h Core.h Scene.h Object.h
	$(CC) $(CFLAGS) -c main.cpp

Math.o: Math.h Core.h

Scene.o: Math.h Core.h Object.h

clean:
	rm -rf $(OBJS) $(TARGET)
