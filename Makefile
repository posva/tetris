#Corrige certains problèmes avec le PATH de windows... Commenter si autre système.

CXX = g++
OBJ = obj
SRC = src
BIN = bin

OPT := -Wall -Wextra -Os -g -I "$(SRC)" -I "extlibs/include" -L "extlibs"

#C'est une méthode à la con pour trouver l'OS >.<"
ifeq ($(SHELL), sh.exe) 
OS := Win
FOLDERS := 
#MANUALLY CREATE LIST
else
ifeq ($(SHELL), C:/Windows/System32/cmd.exe)
OS := Win
FOLDERS := 
POINTCPP = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp) $(wildcard $(SRC)/*/*/*.cpp) $(wildcard $(SRC)/*/*/*/*.cpp)
POINTOP := $(POINTCPP:.cpp=.o)
POINTO = $(patsubst $(SRC)/%,$(OBJ)/%,$(POINTOP)) #$(POINTOP:src=obj)
else
OS := $(shell uname)
FOLDERSP := $(shell find $(SRC)/* -type d)
FOLDERS := $(patsubst src/%,$(OBJ)%,$(FOLDERSP))
POINTCPP := $(shell find $(SRC)/* -type f -name '*.cpp')
POINTO := $(patsubst $(SRC)/%,$(OBJ)/%,$(POINTCPP))
POINTO := $(POINTO:.cpp=.o)
endif
endif

ifeq ($(OS), Win)
EXEC := tetris.exe
LIBS := -lpdcurses
else
EXEC := tetris
LIBS := -lncurses
endif

all : dirs $(EXEC)
		

dirs : 
ifeq ($(OS), Darwin)
	@mkdir -p $(OBJ) $(FOLDERS) $(BIN)
endif
ifeq ($(OS), Linux)
	@mkdir -p $(OBJ) $(FOLDERS) $(BIN)
endif
	
ifeq ($(OS), Win)
#@mkdir $(OBJ) $(BIN) $(FOLDERS)
endif

clean: 
	@$(RM) -vrf $(OBJ) $(BIN)
.PHONY : clean

run: dirs $(EXEC)
	@echo "Launching $(EXEC)"
ifeq ($(OS), Darwin)
	@./$(BIN)/$(EXEC)
endif
ifeq ($(OS), Linux)
	@./$(BIN)/$(EXEC)
endif
ifeq ($(OS), Win)
	@$(BIN)/$(EXEC)
endif
.PHONY : run



$(OBJ)/%.o : $(SRC)/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(OPT) $< -c -o $@


$(EXEC) : $(POINTO)
	@echo "Linking $@"
	@$(CXX) $(OPT) $^ -o $(BIN)/$(EXEC) $(LIBS)

valgrind : dirs $(EXEC)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)/$(EXEC)
	
info:
	@echo "Lines\tWords\tBytes"
	@cd $(SRC) ; \
	find -E . -regex "(.+)\.(h|cpp|c|hpp)" -print0 | xargs -0 cat | wc ; \
	cd ..
.PHONY : info
