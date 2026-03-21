CC = gcc

CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

all: bools hello password lists strings

bools:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-bools/main.c -o ./$(BUILD_DIR)/parsec_$@

hello:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-hello-parsec/main.c -o ./$(BUILD_DIR)/parsec_$@

password:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-password-generator/main.c -o ./$(BUILD_DIR)/parsec_$@

lists:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./01-lists/main.c -o ./$(BUILD_DIR)/parsec_$@

strings:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./01-strings/main.c -o ./$(BUILD_DIR)/parsec_$@

clean:
	rm -rf $(BUILD_DIR)/*
