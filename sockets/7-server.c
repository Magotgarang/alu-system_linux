#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to extract and print HTTP headers
void parse_headers(char *request) {
    char *header_start = strchr(request, '\n');
    if (header_start != NULL) {
        header_start++;  // Move to start of headers
        while (header_start != NULL && *header_start != '\0' && *header_start != '\r') {
            char *header_end = strchr(header_start, '\n');
            if (header_end != NULL) {
                *header_end = '\0';  // End the current header
                printf("Header: %s\n", header_start);
                header_start = header_end + 1;
            }
            else {
                break;
            }
        }
    }
}

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("Socket failed");
        return EXIT_FAILURE;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        return EXIT_FAILURE;
    }

    printf("Server listening on port 8080\n");

    client_fd = accept(server_fd, (struct sockaddr *)&address, &addr_len);
    if (client_fd < 0)
    {
        perror("Accept failed");
        return EXIT_FAILURE;
    }

    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';
        printf("Raw request: \"%s\"\n", buffer);
        parse_headers(buffer);
    }

    send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);

    close(client_fd);
    return EXIT_SUCCESS;
}

