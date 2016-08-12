#debug°æ±¾
DEFS = -D DEBUG
CFLAGS =  -g -Wall $(DEFS) -lpthread -lrt -I. -std=c++0x
CC = g++
objects := MutexPool.o

all:$(objects) main.cpp
	g++ -g -Wall -D DEBUG -lpthread -lrt -I. -std=c++0x $(objects)  -o main main.cpp
MutexPool.o:MutexPool.cpp
	g++  -g -Wall -D DEBUG -lpthread -lrt -I. -std=c++0x -c -o MutexPool.o MutexPool.cpp	

.PHONY:clean cleanall

clean:
	rm -rf *.o main
cleanall:
	rm -rf $(objects)
