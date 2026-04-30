#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[30000] = {0};

    // 1. socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 3. listen
    listen(server_fd, 10);

    printf("Server running on http://localhost:8000\n");

    while (1) {
        // 4. accept
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        // 5. read request
        read(client_fd, buffer, 30000);

        // فقط خط اول رو چک می‌کنیم
        if (strncmp(buffer, "GET /products", 13) == 0) {

            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "[{\"id\":1,\"name\":\"Shoes\"},{\"id\":2,\"name\":\"Bag\"}]";

            write(client_fd, response, strlen(response));
        } else {
            char *response =
                "HTTP/1.1 404 Not Found\r\n"
                "\r\n";

            write(client_fd, response, strlen(response));
        }

        close(client_fd);
    }

    return 0;
}

// ---------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[30000] = {0};

    // 1. socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. bind
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // 3. listen
    listen(server_fd, 10);

    printf("Server running on http://localhost:8000\n");

    while (1) {
        // 4. accept
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        // 5. read request
        read(client_fd, buffer, 30000);

        // فقط خط اول رو چک می‌کنیم
        if (strncmp(buffer, "GET /health", 11) == 0) {

            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"status\":\"ok\"}";
            write(client_fd, response, strlen(response));
        } else {
            char *response =
                "HTTP/1.1 404 Not Found\r\n"
                "\r\n";

            write(client_fd, response, strlen(response));
        }

        close(client_fd);
    }

    return 0;
}