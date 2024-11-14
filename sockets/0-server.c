#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

#define PORT 12345

/**
 * main - Opens a socket, binds to port 12345, listens, and hangs indefinitely
 *
 * Return: 0 on success, 1 on failure if binding or listening fails
 */
int main(void)
{
    int server_fd;
    struct sockaddr_in addr;
    int enable = 1;

    // Create the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Socket failed");
        return 1;
    }

    // Set socket options to allow reuse of address/port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(enable)) < 0)
    {
        perror("SO_REUSEADDR | SO_REUSEPORT failed");
        return 1;
    }

    // Setup the server address
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;  // Listen on any network interface

    // Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("Bind failed");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) == -1)
    {
        perror("Listen failed");
        return 1;
    }

    // Print message to indicate the server is listening
    printf("Server listening on port %d\n", PORT);

    // Hang indefinitely (the server will be killed by a signal during correction)
    while (1)
    {
        continue;
    }

    // Close the socket (although this point will never be reached in this task)
    close(server_fd);

    return 0;
}
