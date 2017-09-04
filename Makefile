MULTIARRAY_DIR := .
MULTIARRAY_INCLUDE := $(MULTIARRAY_DIR)/inc

CACTH_DIR ?= ../tplib/Catch
CACTH_INCLUDE := $(CACTH_DIR)/include

TEST_DIR := $(MULTIARRAY_DIR)/test

SRC_CPP := $(TEST_DIR)/src
OBJ := $(TEST_DIR)/obj
BUILD := $(TEST_DIR)/build

OUT_DIR := $(OBJ) $(BUILD)

TARGET := test

SRC_CPP_FILES := $(wildcard $(SRC_CPP)/*/*.cpp)
OBJ_CPP_FILES := $(patsubst $(SRC_CPP)/%.cpp, $(OBJ)/%.o, $(SRC_CPP_FILES))

SRC_MAIN := $(SRC_CPP)/maintest.cpp
OBJ_MAIN := $(OBJ)/maintest.o

CXX  ?= g++
CXX_FLAGS := -g -Wall -std=c++11 -I$(CACTH_INCLUDE) -I$(MULTIARRAY_INCLUDE)

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
	@./$(BUILD)/$(TARGET) -r console

clean :
	@echo clean
	@rm -rf $(OBJ) $(BUILD)
