#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// Function to parse query parameters from the URL
void parse_query(char *url) {
    char *query = strchr(url, '?');
    if (query != NULL) {
        printf("Query string: %s\n", query + 1);
        char *key_value_pair = strtok(query + 1, "&");
        while (key_value_pair != NULL) {
            printf("Key-Value: %s\n", key_value_pair);
            key_value_pair = strtok(NULL, "&");
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
        // Look for a query string in the request
        char *url_start = strchr(buffer, ' ');
        if (url_start != NULL) {
            char *url_end = strchr(url_start + 1, ' ');
            if (url_end != NULL) {
                *url_end = '\0';
                parse_query(url_start + 1);
            }
        }
    }

    send(client_fd, "HTTP/1.1 200 OK\r\n\r\n", 19, 0);

    close(client_fd);
    return EXIT_SUCCESS;
}

