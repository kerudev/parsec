CC = gcc

CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

password:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./00_password-generator/main.c -o ./$(BUILD_DIR)/password 

clean:
	rm -rf $(BUILD_DIR)/*
