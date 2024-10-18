CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
TARGET = hnm
SRCS = hnm.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
