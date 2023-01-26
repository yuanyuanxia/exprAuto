export PROJECT_NAME = exprAuto

CC = gcc
CPP = g++
INCLUDE = -I../include -I../includeTEST
LIBS = 
ECHO = printf

# $(info $(CFLAGS) )
override CFLAGS += -O3 -g -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
# $(info $(CFLAGS) )

# default: dataClean.exe

dataClean.exe: dataClean.o
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CC) -o $@ $^ $(LIBS)

dataClean.o: dataClean.c
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

gccPerformanceTest.o: gccPerformanceTest.c
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

binary.o: binary.c
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f *.o *.exe