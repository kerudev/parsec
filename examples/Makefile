CC = gcc

CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

all: hello bools strings lists password csv

hello:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-hello-parsec/main.c -o ./$(BUILD_DIR)/parsec_$@

bools:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./01-bools/main.c -o ./$(BUILD_DIR)/parsec_$@

strings:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./02-strings/main.c -o ./$(BUILD_DIR)/parsec_$@

lists:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./03-lists/main.c -o ./$(BUILD_DIR)/parsec_$@

password:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./10-password-generator/main.c -o ./$(BUILD_DIR)/parsec_$@

csv:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./11-csv/main.c -o ./$(BUILD_DIR)/parsec_$@

clean:
	rm -rf $(BUILD_DIR)/*
