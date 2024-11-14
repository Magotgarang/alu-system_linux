#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Function to parse the query and extract the value for 'name'
void handle_dynamic_content(char *url, int client_fd) {
    char *query = strchr(url, '?');
    if (query != NULL) {
        char *name_param = strstr(query, "name=");
        if (name_param != NULL) {
            name_param += 5;  // Skip 'name='
            char *end = strchr(name_param, '&');
            if (end != NULL) *end = '\0';

            char response[BUFFER_SIZE];
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\n"
                     "Content-Type: text/html\r\n"
                     "\r\n"
                     "<html><body><h1>Hello, %s!</h1></body></html>\r\n", name_param);
            send(client_fd, response, strlen(response), 0);
            return;
        }
    }

    // Default response if no 'name' parameter is found
    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html><body><h1>Hello, Guest!</h1></body></html>\r\n";
    send(client_fd, http_response, strlen(http_response), 0);
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
        char *url_start = strchr(buffer, ' ');
        if (url_start != NULL) {
            char *url_end = strchr(url_start + 1, ' ');
            if (url_end != NULL) {
                *url_end = '\0';
                handle_dynamic_content(url_start + 1, client_fd);
            }
        }
    }

    close(client_fd);
    return EXIT_SUCCESS;
}

