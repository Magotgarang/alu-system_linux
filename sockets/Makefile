# Define compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pedantic

# List of source files
SERVER_SRC = 1-server.c 2-server.c 3-server.c 4-server.c 5-server.c 6-server.c 7-server.c 8-server.c 9-server.c

# Object files generated from the source files
SERVER_OBJ = $(SERVER_SRC:.c=.o)

# Name of the final executable
EXEC = server

# Default target to build the server
all: $(EXEC)

# Rule to link the object files into an executable
$(EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(SERVER_OBJ)

# Rule to compile each .c file into .o object file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Rule to clean up object files and the executable
clean:
	rm -f $(SERVER_OBJ) $(EXEC)

# Rule to rebuild everything
rebuild: clean all

