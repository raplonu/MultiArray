MULTIARRAY_DIR := ..
MULTIARRAY_INCLUDE := $(MULTIARRAY_DIR)/inc

CACTH_DIR ?= ../../other/Catch
CACTH_INCLUDE := $(CACTH_DIR)/include

SRC_CPP := src
OBJ := obj
BUILD := build

OUT_DIR := $(OBJ) $(BUILD)

TARGET := test

SRC_CPP_FILES := $(wildcard $(SRC_CPP)/*/*.cpp)
OBJ_CPP_FILES := $(patsubst $(SRC_CPP)/%.cpp, $(OBJ)/%.o, $(SRC_CPP_FILES))

SRC_MAIN := $(SRC_CPP)/maintest.cpp
OBJ_MAIN := $(OBJ)/maintest.o

TARGET := test

CXX  ?= g++
CXX_FLAGS := -g -Wall -std=c++14 -I$(CACTH_INCLUDE) -I$(MULTIARRAY_INCLUDE)

#Other
MKDIR_P					?= mkdir -p

.PHONY: all build directories gendir build_main exec clean

all : build exec

build : directories gendir $(BUILD)/$(TARGET)

gendir :
	@rsync -a -f"+ */" -f"- *" $(SRC_CPP)/ $(OBJ)/

build_main : $(OBJ_TARGET)

$(OBJ_MAIN) : $(SRC_MAIN)
	@echo preliminary build
	@g++ $< -c $(CXX_FLAGS) -o $@

$(OBJ)/%.o : $(SRC_CPP)/%.cpp
	@echo build
	@g++ $< -c $(CXX_FLAGS) -o $@

$(BUILD)/$(TARGET) : $(OBJ_MAIN) $(OBJ_CPP_FILES)
	@echo Build
	@g++ $^ $(CXX_FLAGS) -o $@

directories :
	@$(MKDIR_P) $(OUT_DIR)

exec : $(BUILD)/$(TARGET)
	@echo Exec
	@./build/test -r console

clean :
	@echo clean
	@rm -rf $(OUT_DIR)
