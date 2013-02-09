CXX = g++
OBJ = obj
SRC = src
BIN = bin

OPT := -Wall -Wextra -Os -g -I "$(SRC)"

ifeq ($(SHELL), sh.exe) 
OS := Win
FOLDERS := 
#MANUALLY CREATE LIST
else
OS := $(shell uname)
FOLDERSP := $(shell find $(SRC)/* -type d)
FOLDERS := $(patsubst src/%,$(OBJ)%,$(FOLDERSP))
POINTCPP := $(shell find $(SRC)/* -type f -name '*.cpp')
POINTO := $(patsubst $(SRC)/%,$(OBJ)/%,$(POINTCPP))
POINTO := $(POINTO:.cpp=.o)
endif

ifeq ($(OS), Win)
EXEC := tetris.exe
else
EXEC := tetris
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
	@mkdir $(OBJ) $(BIN) $(FOLDERS)
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
	@$(BIN)/$(EXEC).exe
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