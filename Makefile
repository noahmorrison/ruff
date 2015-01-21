BINARY ?= ruff
SOURCE ?= ruff.c hash.c
OBJECTS ?= $(SOURCE:.c=.o)
CFLAGS += -Wall -lcrypto
INCLUDES +=

all: $(BINARY)


$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCE) -o $@


clean:
	rm -f $(OBJECTS)
	rm -f $(BINARY)
	rm -rf test/


run: all test-dir
	./$(BINARY) -r test/ref -n test/dup


test-dir:
	@mkdir test
	@mkdir test/ref
	@echo "Hello, World!" > test/ref/hw
	@echo "This does not match the file in dup/" > test/ref/no_match
	@echo "This file does match!" > test/ref/match
	@mkdir test/ref/sub
	@echo "This is inside a subdirectory" > test/ref/sub/match
	@echo "This is inside a subdirectory (ref)" > test/ref/sub/no_match
	@mkdir test/dup
	@echo "Hello, World!" > test/dup/hw
	@echo "This does not match the file in ref/" > test/dup/no_match
	@echo "This file does match!" > test/dup/match
	@mkdir test/dup/sub
	@echo "This is inside a subdirectory" > test/dup/sub/match
	@echo "This is inside a subdirectory (dup)" > test/dup/sub/no_match
