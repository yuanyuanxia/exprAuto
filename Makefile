export PROJECT_NAME = exprAuto

CC = gcc
CPP = g++
INCLUDE = -Iinclude
LIBS=

EXPRAUTO_ALL_SRCS_CPP  = $(wildcard src/*.cpp )
EXPRAUTO_ALL_SRCS_OBJS = $(addprefix objs/, $(subst /,_,$(EXPRAUTO_ALL_SRCS_CPP:.cpp=.cpp.o)))
# $(info $(CFLAGS) )
override CFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
# $(info $(CFLAGS) )

default: all

.PHONY: all
all: bin/main.exe

bin/main.exe: $(EXPRAUTO_ALL_SRCS_OBJS)
	$(CPP) -o $@ $^ $(LIBS)

objs/src_%.cpp.o: src/%.cpp
	$(CPP) -o $@ -c $< $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f objs/*.o bin/*.exe