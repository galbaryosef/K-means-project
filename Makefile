CC := gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
CFLAGS += -Iinc
CFLAGS += -g
LFLAGS = -lm
SRC_DIR := src
OBJ_DIR := obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES += $(OBJ_DIR)/spkmeans.o

TARGET := spkmeans

PYTHON_SO = mykmeanssp.cpython-310-x86_64-linux-gnu.so

.PHONY: all clean clean_py build_py

all: $(TARGET)

build_py:
	python3 setup.py build_ext --inplace

$(TARGET): $(OBJ_FILES)
	$(CC) $^ -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@ $(LFLAGS)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@ $(LFLAGS)

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

clean_py:
	rm -rf $(PYTHON_SO) build
