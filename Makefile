SOURCEDIR := ./src
INCLUDEDIR := ./include/
LIBDIR := ./lib 
DIR := .
EXE := ./bin/server
CONFIG_PATH:=./conf
CC	:= g++
LIBS:= pthread json
OCPP:= $(patsubst $(SOURCEDIR)/%.cpp, $(SOURCEDIR)/%.o, $(wildcard $(SOURCEDIR)/*.cpp))
OBJS:= $(OCPP)
RM	:= rm -rf
CXXFLAGS:= -Wall -g
start: $(EXE)
$(EXE):$(OBJS)
		$(CC) -o $@  $(OBJS) -L $(LIBDIR) $(addprefix -l, $(LIBS)); $(RM) $(SOURCEDIR)/*.o $(RM) $(SOURCEDIR)/*.co  $(RM) $(SOURCEDIR)/*.cco
$(SOURCEDIR)/%.o:$(SOURCEDIR)/%.cpp
		$(CC) -c $(CXXFLAGS) $< -o $@ -I $(INCLUDEDIR) -D NDEBUG
.PHONY:clean rebuild
clean:
		@$(RM) $(DIR)/*.exe $(RM) $(SOURCEDIR)/*.o $(RM) $(SOURCEDIR)/*.co  $(RM) $(SOURCEDIR)/*.cco 
rebuild: clean start
