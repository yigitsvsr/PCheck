CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude
SRC = src/main.c src/crawler.c src/hasher.c src/queue.c src/worker.c src/config.c
OBJ = $(SRC:.c=.o)
TARGET = pcheck


all: $(TARGET)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)

test: all
	@echo "--Test Case Being Prepared--"
	@mkdir -p test/source
	@mkdir -p test/backup
	@echo "Hello World" > test/source/file1.txt
	@echo "Constant File" > test/source/file2.txt
	@cp test/source/file2.txt test/backup/file2.txt
	@echo "Hello THWS (Changed)" > test/backup/file1.txt
	@echo "Brand New File" > test/source/new.txt
	@echo "\n#1.TEST: Hash Calculation - Basic Mode"
	./pcheck test/source
	@echo "\n#2.TEST: Comparison - Compare Mode"
	./pcheck test/source -c test/backup
	@echo "\nTest Environment Being Cleared..."
	@rm -rf test
	@echo "\n--Test Successfully Completed--"


.PHONY: all clean test
