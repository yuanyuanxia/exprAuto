export PROJECT_NAME = exprAuto

CC = gcc
CPP = g++
INCLUDE = -Iinclude -I/usr/include/python3.8
# LIBS=  -L/usr/lib/python3.8/config-3.8-x86_64-linux-gnu -lpython3.8 # may need -L to assign the Python lobrary path
LIBS=  -lpython3.8
ECHO = printf

# $(info $(CFLAGS) )
override CFLAGS += -g -Wall -Wextra -Wpedantic -Wno-unused-function -fdiagnostics-color=always
# $(info $(CFLAGS) )

EXPRAUTO_ALL_SRCS_CPP  = $(wildcard src/*.cpp)
EXPRAUTO_ALL_SRCS_OBJS = $(addprefix objs/, $(subst /,_,$(EXPRAUTO_ALL_SRCS_CPP:.cpp=.cpp.o)))

default: dirs \
	bin/exprAuto.exe
dirs:
	mkdir -p bin objs
new: dirs \
	bin/exprAutoNew.exe

.PHONY: dirs

bin/exprAuto.exe: $(EXPRAUTO_ALL_SRCS_OBJS)
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ $^ $(LIBS)

bin/exprAutoNew.exe: exprAutoNew.o objs/src_preprocess.cpp.o objs/src_mathfuncRewrite.cpp.o objs/src_basic.cpp.o objs/src_simplifyExpr.cpp.o objs/src_laxerASTLY.cpp.o objs/src_parserASTLY.cpp.o objs/src_expandAST.cpp.o objs/src_polyRewrite.cpp.o objs/src_monoInfo.cpp.o objs/src_funcInfo.cpp.o objs/src_polyInfo.cpp.o objs/src_variableInfo.cpp.o objs/src_geneExpr.cpp.o
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ $^ $(LIBS)

exprAutoNew.o: exprAutoNew.cpp
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ -c $< $(CFLAGS) $(INCLUDE)

objs/src_%.cpp.o: src/%.cpp
	@$(ECHO) "\033[1;32mBuilding $@ \n\033[0m"
	$(CPP) -o $@ -c $< $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f objs/*.o bin/*.exe *.o *.exe