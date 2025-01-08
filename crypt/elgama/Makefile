

CC = gcc
PROJECT_ROOT = ../..
CFLAGS = -Wall -g $(INCLUDES) -Wextra -funroll-loops -march=native
INCLUDES = -I./inc -I$(PROJECT_ROOT)/utils/inc

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)
UTILS_SRC_DIR = $(PROJECT_ROOT)/utils/src


SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
UTILS_SRC = $(wildcard $(UTILS_SRC_DIR)/*.c)
UTILS_OBJ := $(patsubst %.c,%.o,$(UTILS_SRC))
TEST_EXEC = $(BIN_DIR)/test_el.exe # <=================== 将可执行文件名修改为test_{密码算法名称}.exe（可以不修改）

all: $(TEST_EXEC)
	./$(TEST_EXEC)

# rmdir /S /Q build
clean:
	rm -rf build
	rm -rf $(UTILS_OBJ)

.PHONY: clean

$(TEST_EXEC): $(OBJECTS) $(UTILS_OBJ) $(BUILD_DIR)/test.o | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

#                     ↓↓↓↓↓↓↓↓↓↓↓   将此处测试文件名修改为你的测试文件名
$(BUILD_DIR)/test.o: ./elgama_test.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTILS_SRC_DIR)/%.o: $(UTILS_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir $@
