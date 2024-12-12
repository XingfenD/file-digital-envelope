# ./Makefile

# defination of compile commands, args and flags
CC = gcc
CFLAGS = -Wall -g $(INCLUDES) -Wextra -funroll-loops -march=native
LDFLAGS =
INCLUDES =
# INCLUDES += $(addprefix -I, $(CRYPTS_INC_DIRS))

# defination of directory varibles
TEST_DIR = ./test

CRYPT_DIR = ./crypt
CRYPTS_DIR := $(filter-out $(CRYPT_DIR),$(shell find $(CRYPT_DIR) -maxdepth 1 -type d ! -name '.'))

UTILS_SRC_DIR = ./utils/src
UTILS_INC_DIR = ./utils/inc
CRYPTS_SRC_DIRS = $(addsuffix /src, $(CRYPTS_DIR))
CRYPTS_INC_DIRS = $(addsuffix /inc, $(CRYPTS_DIR))

BUILD_DIR = ./build
BIN_DIR = $(BUILD_DIR)/bin

# defination of source file varibles
UTILS_SRCS := $(wildcard $(UTILS_SRC_DIR)/*.c)
CRYPTS_SRCS := $(foreach dir,$(CRYPTS_SRC_DIRS),$(wildcard $(dir)/*.c))
TEST_SRCS := $(wildcard $(TEST_DIR)/*.c)

# defination of object file varibles
UTILS_OBJS := $(patsubst %.c,%.o,$(UTILS_SRCS))
CRYPTS_OBJS := $(patsubst %.c,%.o,$(CRYPTS_SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# defination of executable file varibles
MAIN_EXEC = $(BIN_DIR)/fde.exe
TEST_EXECS = $(patsubst $(TEST_DIR)/%.c,$(BIN_DIR)/%.exe,$(TEST_SRCS))

# definations and operations of targets

all: CFLAGS += -O0
all: INCLUDES += $(addprefix -I, $(CRYPTS_INC_DIRS)) -I$(UTILS_INC_DIR)
all: $(MAIN_EXEC)

test-enc: all
	$(MAIN_EXEC) -m enc -f ./IOfile/plaintext.txt -o ./IOfile/enc_rst.fde -k ./IOfile/rsa.pub

test-dec: all
	$(MAIN_EXEC) -m dec -f ./IOfile/enc_rst.fde -k ./IOfile/rsa.key

release: CFLAGS += -O3
release: INCLUDES += $(addprefix -I, $(CRYPTS_INC_DIRS)) -I$(UTILS_INC_DIR)
release: clean | $(MAIN_EXEC)

test: INCLUDES += $(addprefix -I, $(CRYPTS_INC_DIRS)) -I$(UTILS_INC_DIR)
test: $(TEST_EXECS)
	@for exe in $(TEST_EXECS); do \
		echo; \
		echo ">===================  Running $$(basename $$exe) ===================<"; \
		echo; \
		./$$exe; \
		echo; \
		echo ">=============== Finished running $$(basename $$exe) ===============<"; \
		echo; \
	done


clean:
	rm -rf build
	rm -rf $(CRYPTS_OBJS) $(UTILS_OBJS)

.PHONY: clean test
.SECONDARY: $(TEST_OBJS)
# link
$(MAIN_EXEC): $(CRYPTS_OBJS) $(UTILS_OBJS) $(BUILD_DIR)/main.o | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BIN_DIR)/%.exe: $(CRYPTS_OBJS) $(UTILS_OBJS) $(BUILD_DIR)/%.o | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# compile generally
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# compile specifically
$(BUILD_DIR)/main.o: ./main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR): | $(BUILD_DIR)
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@
