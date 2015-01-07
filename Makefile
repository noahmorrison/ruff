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

run:
	./$(BINARY)
