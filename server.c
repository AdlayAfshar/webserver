#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[30000] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(3000);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    listen(server_fd, 10);

    printf("Server running on port 3000...\n");

    while (1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        read(client_fd, buffer, 30000);
        // parse method + path
        char method[10];
        char path[100];
        char harchi[100];
        char harchi2[100];

        printf("==================================\n");
        printf("Buffer:\n");
        printf("==================================\n");
        printf("%s\n", buffer);
        printf("----------------------------------\n");

        sscanf(buffer, "%s %s %s %s", method, path, harchi, harchi2);

        printf("Method: %s\n", method);
        printf("Path: %s\n", path);
        printf("harchi: %s\n", harchi);
        printf("harchi2: %s\n", harchi);

        char *body = strstr(buffer, "\r\n\r\n");

        if (body)
        {
            body += 4; // رد شدن از \r\n\r\n
            printf("Body: %s\n", body);
        }


        if (strcmp(method, "GET") == 0 && strcmp(path, "/health") == 0)
        {
            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"status\":\"ok\"}";

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "GET") == 0 && strcmp(path, "/products") == 0)
        {
            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "[{\"id\":1,\"name\":\"Shoes\"},{\"id\":2,\"name\":\"Bag\"}]";

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "POST") == 0 && strcmp(path, "/cart") == 0)
        {
            int product_id = -1;

            if (body)
            {
                sscanf(body, "{\"product_id\":%d}", &product_id);
                printf("Product ID: %d\n", product_id);
            }

            char response[200];

            sprintf(response,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"message\":\"added\",\"product_id\":%d}",
                product_id
            );

            write(client_fd, response, strlen(response));
            // char *response =
            //     "HTTP/1.1 200 OK\r\n"
            //     "Content-Type: application/json\r\n"
            //     "\r\n"
            //     "{\"message\":\"added to cart\"}";

            // write(client_fd, response, strlen(response));
        }
        else
        {
            char *response =
                "HTTP/1.1 404 Not Found\r\n"
                "\r\n"
                "Not Found";

            write(client_fd, response, strlen(response));
        }

        close(client_fd);
    }

    return 0;
}