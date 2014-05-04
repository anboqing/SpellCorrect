SOURCEDIR := ./src
INCLUDEDIR := ./include/
LIBDIR := ./lib 
DIR := .
EXE := ./bin/server
GCC := gcc
CC	:= g++
LIBS:= pthread json
OCPP:= $(patsubst $(SOURCEDIR)/%.cpp, $(SOURCEDIR)/%.o, $(wildcard $(SOURCEDIR)/*.cpp))
OC	:= $(patsubst $(SOURCEDIR)/%.c, $(SOURCEDIR)/%.co, $(wildcard $(SOURCEDIR)/*.c))
OCC	:= $(patsubst $(SOURCEDIR)/%.cc, $(SOURCEDIR)/%.cco, $(wildcard $(SOURCEDIR)/*.cc))
OBJS:= $(OC) $(OCC) $(OCPP)
RM	:= rm -rf
CXXFLAGS:= -Wall -g
start: $(EXE)
$(EXE):$(OBJS)
		$(CC) -o $@  $(OBJS) -L $(LIBDIR) $(addprefix -l, $(LIBS)); $(RM) $(SOURCEDIR)/*.o $(RM) $(SOURCEDIR)/*.co  $(RM) $(SOURCEDIR)/*.cco
$(SOURCEDIR)/%.o:$(SOURCEDIR)/%.cpp
		$(GCC) -c $(CXXFLAGS) $< -o $@ -I $(INCLUDEDIR) -D NDEBUG 
$(SOURCEDIR)/%.co:%.c
		$(CC) -c $(CXXFLAGS) $< -o $@ -I $(INCLUDEDIR)
$(SOURCEDIR)/%.cco:%.cc
		$(CC) -c $(CXXFLAGS) $< -o $@ -I $(INCLUDEDIR)
.PHONY:clean rebuild
clean:
		@$(RM) $(DIR)/*.exe $(RM) $(SOURCEDIR)/*.o $(RM) $(SOURCEDIR)/*.co  $(RM) $(SOURCEDIR)/*.cco 
rebuild: clean start
