CC = gcc

CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

all: hello password

hello:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-hello-parsec/main.c -o ./$(BUILD_DIR)/parsec_$@

password:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00-password-generator/main.c -o ./$(BUILD_DIR)/parsec_$@

clean:
	rm -rf $(BUILD_DIR)/*
