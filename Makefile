export PROJECT_NAME = exprAuto

CC = gcc
CPP = g++
INCLUDE = -Iinclude
LIBS=
ECHO = printf

# $(info $(CFLAGS) )
override CFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
# $(info $(CFLAGS) )

EXPRAUTO_ALL_SRCS_CPP  = $(wildcard src/*.cpp )
EXPRAUTO_ALL_SRCS_OBJS = $(addprefix objs/, $(subst /,_,$(EXPRAUTO_ALL_SRCS_CPP:.cpp=.cpp.o)))

default: dirs \
		 bin/exprAuto.exe
dirs:
	mkdir -p bin objs

.PHONY: dirs

bin/exprAuto.exe: $(EXPRAUTO_ALL_SRCS_OBJS)
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ $^ $(LIBS)

objs/src_%.cpp.o: src/%.cpp
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ -c $< $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f objs/*.o bin/*.exe