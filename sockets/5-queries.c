#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define BACKLOG 10

// Function prototypes
int start_server(void);
int handle_client(int client_fd);
void parse_request(char *buf);
int send_response(int client_fd, const char *status_code);

/**
 * start_server - sets up the server, binds, and listens for incoming connections
 *
 * Return: 0 on success, 1 on failure
 */
int start_server(void)
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket failed");
        return 1;
    }

    // Prepare the address structure
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, BACKLOG) == -1)
    {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    // Main server loop to accept connections and handle requests
    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
        if (client_fd == -1)
        {
            perror("accept failed");
            continue;
        }

        // Print the client's IP address
        printf("Client connected: %s\n", inet_ntoa(addr.sin_addr));

        // Handle the client request
        if (handle_client(client_fd) != 0)
        {
            perror("handle_client failed");
        }

        // Close the client connection
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

/**
 * handle_client - handles the HTTP request from the client
 * @client_fd: the client socket descriptor
 *
 * Return: 0 on success, 1 on failure
 */
int handle_client(int client_fd)
{
    char buf[1024];
    ssize_t read_size;

    // Read the HTTP request from the client
    read_size = read(client_fd, buf, sizeof(buf) - 1);
    if (read_size <= 0)
    {
        perror("read failed");
        return 1;
    }
    buf[read_size] = '\0'; // Null-terminate the buffer

    // Print the full HTTP request
    printf("Raw request: \"%s\"\n", buf);

    // Parse the HTTP request and extract path and query parameters
    parse_request(buf);

    // Send an HTTP 200 OK response
    return send_response(client_fd, "200 OK");
}

/**
 * parse_request - parses the HTTP request and extracts path and query parameters
 * @buf: string buffer containing the HTTP request
 */
void parse_request(char *buf)
{
    char *DELIMS = " \t\r\n";
    char *method, *path, *query, *save1, *key, *value, *save2;

    // Parse the method
    method = strtok(buf, DELIMS);
    if (method == NULL)
    {
        printf("Invalid request\n");
        return;
    }

    // Parse the path and query string
    path = strtok(NULL, DELIMS);
    if (path == NULL)
    {
        printf("Invalid path\n");
        return;
    }

    // Parse the path before any query parameters
    path = strtok_r(path, "?", &save1);
    printf("Path: %s\n", path);

    // If there are query parameters, process them
    query = strtok_r(NULL, "&", &save1);
    while (query)
    {
        key = strtok_r(query, "=", &save2);
        value = strtok_r(NULL, "=", &save2);
        printf("Query: \"%s\" -> \"%s\"\n", key, value);
        query = strtok_r(NULL, "&", &save1);
    }
}

/**
 * send_response - sends an HTTP response to the client
 * @client_fd: the client socket descriptor
 * @status_code: the HTTP status code to return
 *
 * Return: 0 on success, 1 on failure
 */
int send_response(int client_fd, const char *status_code)
{
    char response[1024];

    // Basic HTTP response structure
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\n"
             "Content-Type: text/html; charset=UTF-8\r\n"
             "Content-Length: 13\r\n"
             "\r\n"
             "<h1>%s</h1>\r\n", status_code, status_code);

    // Send the response
    if (send(client_fd, response, strlen(response), 0) == -1)
    {
        perror("send failed");
        return 1;
    }

    return 0;
}

/**
 * main - entry point
 *
 * Return: SUCCESS or FAILURE
 */
int main(void)
{
    return start_server();
}

