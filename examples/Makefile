CC = gcc

CFLAGS = -std=c99 \
	-fsanitize=address -ggdb \
	-Wall -Wextra \
	-Wno-return-type \
	-Wno-discarded-qualifiers

BUILD_DIR = build

EXAMPLES = \
	00-hello-parsec \
	01-bools \
	02-strings \
	03-lists \
	10-password-generator \
	11-csv

all: $(EXAMPLES)

$(EXAMPLES):
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./$@/main.c -o ./$(BUILD_DIR)/$@

clean:
	rm -rf $(BUILD_DIR)/*
