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
	$(CC) $(CFLAGS) ./00_hello_parsec/main.c -o ./$(BUILD_DIR)/$@

password:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00_password-generator/main.c -o ./$(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)/*
