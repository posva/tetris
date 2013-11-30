CXX = gcc
OBJ = obj
SRC = src
BIN = bin

OPT := -Wall -Wextra -std=c99 -Os -D _BSD_SOURCE -I "$(SRC)"

LIBS := -lncurses

ifeq ($(SHELL), sh.exe)
OS := Win
FOLDERS :=
#MANUALLY CREATE LIST
else
OS := $(shell uname)
FOLDERSP := $(shell find $(SRC)/* -type d)
FOLDERS := $(patsubst $(SRC)/%,$(OBJ)/%,$(FOLDERSP))
POINTC := $(shell find $(SRC)/* -type f -name '*.c')
POINTO := $(patsubst $(SRC)/%,$(OBJ)/%,$(POINTC))
POINTO := $(POINTO:.c=.o)
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
	$(RM) -rf $(OBJ) $(BIN)
.PHONY : clean

run: dirs $(EXEC)
ifeq ($(OS), Darwin)
	./$(BIN)/$(EXEC)
endif
ifeq ($(OS), Linux)
	./$(BIN)/$(EXEC)
endif
ifeq ($(OS), Win)
	$(BIN)/$(EXEC).exe
endif
.PHONY : run



$(OBJ)/%.o : $(SRC)/%.c $(SRC)/%.h
	$(CXX) $(OPT) $< -c -o $@


$(OBJ)/main.o : $(SRC)/main.c
	$(CXX) $(OPT) $< -c -o $@

$(EXEC) : $(POINTO)
	$(CXX) $(OPT) $^ -o $(BIN)/$(EXEC) $(LIBS)

valgrind : dirs $(EXEC)
	valgrind -v --leak-check=full --tool=memcheck ./$(BIN)/$(EXEC)
	
info:
	@echo "Lines\tWords\tBytes"
	@cd $(SRC) ; \
	find -E . -regex "(.+)\.(h|cpp|c|hpp)" -print0 | xargs -0 cat | wc ; \
	cd ..
.PHONY : info
