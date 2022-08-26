export PROJECT_NAME = exprAuto
CC = gcc
CPP = g++
INCLUDE =
LIBS=
# $(info $(CFLAGS) )
override CFLAGS +=
# $(info $(CFLAGS) )

.PHONY: all
all: main

main: main.o monoInfo.o variableInfo.o funcInfo.o
	$(CPP) $^ -o main.exe $(LIBS)

main.o: main.cpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

monoInfo.o: monoInfo.cpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

variableInfo.o: variableInfo.cpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

funcInfo.o: funcInfo.cpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

.PHONY: clean
clean:
	rm -f *.o *.exe