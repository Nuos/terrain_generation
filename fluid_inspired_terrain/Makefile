CFLAGS = -std=c++11
OFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

CC = g++

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TMP_DIRS = $(OBJ_DIR) $(BIN_DIR)

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

PROGRAM=bin/main

$(PROGRAM): $(OBJS) $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(OFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(TMP_DIRS):
	mkdir $@

clean:
	rm -rf $(TMP_DIRS)

run: $(PROGRAM)
	$(PROGRAM)
