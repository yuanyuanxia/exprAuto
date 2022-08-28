export PROJECT_NAME = exprAuto
CC = gcc
CPP = g++
INCLUDE =
LIBS=
# $(info $(CFLAGS) )
override CFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
# $(info $(CFLAGS) )

.PHONY: all
all: main

main: main.o monoInfo.o variableInfo.o funcInfo.o
	$(CPP) $^ -o main.exe $(LIBS)

main.o: main.cpp exprAuto.hpp basic.hpp parserAST.hpp printAST.hpp changeAST.hpp createExpr.hpp mathfunctransAST.hpp exprAuto.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

monoInfo.o: monoInfo.cpp monoInfo.hpp funcInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

variableInfo.o: variableInfo.cpp variableInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

funcInfo.o: funcInfo.cpp funcInfo.hpp monoInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

.PHONY: clean
clean:
	rm -f *.o *.exe