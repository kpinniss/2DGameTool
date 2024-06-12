### VARIBLES ###
CC = g++
LANG_STD = -std=c++17
COMP_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATH = -I"./libs/"
SRC_FILES = ./src/*.cpp ./src/Game/*.cpp ./src/Logger/*.cpp
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
OBJ_NAME = gameengine
RUN_PATH = ./$(OBJ_NAME)
CLEAN_CMD = rm $(OBJ_NAME)

### BUILD / RUN SETTINGS ###
build:
	$(CC) $(COMP_FLAGS) $(LANG_STD) $(INCLUDE_PATH) $(SRC_FILES) $(LINKER_FLAGS) -o $(OBJ_NAME)

run:
	$(RUN_PATH)

clean:
	$(CLEAN_CMD)