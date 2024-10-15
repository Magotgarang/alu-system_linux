CC = gcc
CFLAGS = -Wall -Wextra -pedantic -Werror
TARGET = hnm
SRC = hnm.c elf_utils.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

re: clean all
