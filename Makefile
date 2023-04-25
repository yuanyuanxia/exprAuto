export PROJECT_NAME = exprAuto

CC = gcc
CPP = g++
INCLUDE = -Iinclude -I/usr/include/python3.8
# LIBS=  -L/usr/lib/python3.8/config-3.8-x86_64-linux-gnu -lpython3.8 # may need -L to assign the Python lobrary path
LIBS = -lm -lpython3.8 -lfmt -lmpfr
ECHO = printf

# $(info $(CFLAGS) )
override CFLAGS += -g -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always  --std=c++2a
# $(info $(CFLAGS) )

EXPRAUTO_ALL_SRCS_CPP  = $(wildcard src/*.cpp)
EXPRAUTO_ALL_SRCS_OBJS = $(addprefix objs/, $(subst /,_,$(EXPRAUTO_ALL_SRCS_CPP:.cpp=.cpp.o)))

default: dirs \
	bin/exprAuto.exe
dirs:
	@mkdir -p bin objs outputs

.PHONY: dirs

bin/exprAuto.exe: $(EXPRAUTO_ALL_SRCS_OBJS)
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ $^ $(LIBS)

objs/src_%.cpp.o: src/%.cpp
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ -c $< $(CFLAGS) $(INCLUDE)

objs/src_%.cpp.d: src/%.cpp
# $(info include ruler about $* )
	@set -e; rm -f $@; $(CPP) -MM $< $(INCLUDE) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,objs/src_\1.cpp.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

-include $(EXPRAUTO_ALL_SRCS_OBJS:.o=.d)

.PHONY: clean
clean:
	rm -f objs/*.d objs/*.o bin/*.exe