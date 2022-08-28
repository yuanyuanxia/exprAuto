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

main: main.o laxerAST.o parserAST.o printAST.o changeAST.o createExpr.o mathfunctransAST.o monoInfo.o variableInfo.o funcInfo.o exprAuto.o
	$(CPP) $^ -o main.exe $(LIBS)

main.o: main.cpp basic.hpp parserAST.h
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

laxerAST.o: laxerAST.cpp laxerAST.h basic.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

parserAST.o: parserAST.cpp parserAST.h basic.hpp laxerAST.h
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

printAST.o: printAST.cpp printAST.h basic.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

changeAST.o: changeAST.cpp changeAST.h basic.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

createExpr.o: createExpr.cpp createExpr.h basic.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

mathfunctransAST.o: mathfunctransAST.cpp basic.hpp changeAST.h mathfunctransAST.h
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

monoInfo.o: monoInfo.cpp monoInfo.hpp funcInfo.hpp polyInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

variableInfo.o: variableInfo.cpp variableInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

funcInfo.o: funcInfo.cpp funcInfo.hpp monoInfo.hpp
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

exprAuto.o: exprAuto.cpp basic.hpp monoInfo.hpp exprAuto.h
	$(CC) ${CFLAGS} $< -c $(INCLUDE)

.PHONY: clean
clean:
	rm -f *.o *.exe