#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>

typedef struct s_cart_item
{
    int product_id;
    int quantity;
}   t_cart_item;

t_cart_item cart[100];
int cart_count = 0;

// cart[0] → { ?, ? }
// cart[1] → { ?, ? }
// ...
// cart[99] → { ?, ? }

// e.g.:
// cart[0] = { 124123, -55 }
// cart[1] = { 0, 99999 }

typedef struct s_order_item
{
    int product_id;
    int quantity;
}   t_order_item;

typedef struct s_order
{
    int id;
    t_order_item items[100];
    int item_count;
    int total;
    int paid; // 0 = no, 1 = yes
}   t_order;

t_order orders[100];
int order_count = 0;

typedef struct s_product
{
    int id;
    char *name;
    int price;
}   t_product;

t_product products[] = {
    {1, "Shoes", 100},
    {2, "Bag", 200},
    {3, "Hat", 50}
};

int product_count = 3;

int get_price(int product_id)
{
    int i = 0;

    while (i < product_count)
    {
        if (products[i].id == product_id)
            return products[i].price;
        i++;
    }
    return -1;
}

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

        memset(buffer, 0, sizeof(buffer));
        read(client_fd, buffer, 30000); // buffer == request value
        // parse method + path
        char method[10];
        char path[100];
        char harchi[100];
        char harchi2[100];

        printf("==================================\n");
        printf("Buffer:\n");
        printf("==================================\n");
        printf("%s\n", buffer);
        printf("+++++++++++++++++++++++++++++++++++\n");

        sscanf(buffer, "%s %s %s %s", method, path, harchi, harchi2);

        printf("----------------------------------\n");
        printf("Method: %s\n", method);
        printf("Path: %s\n", path);
        printf("harchi: %s\n", harchi);
        printf("harchi2: %s\n", harchi2);
        printf("--------------\n");

        char *body = strstr(buffer, "\r\n\r\n");

        if (body)
        {
            body += 4; // رد شدن از \r\n\r\n
            printf("Body: %s\n", body);
        }
        printf("----------------------------------\n");


        if (strcmp(method, "GET") == 0 && strcmp(path, "/health") == 0)
        {
            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"status\":\"ok\"}";

            write(client_fd, response, strlen(response));
        }
        // if (strcmp(method, "GET") == 0 && strcmp(path, "/products") == 0)
        // {
        //     char *response =
        //         "HTTP/1.1 200 OK\r\n"
        //         "Content-Type: application/json\r\n"
        //         "\r\n"
        //         "[{\"id\":1,\"name\":\"Shoes\"}]";
        //     write(client_fd, response, strlen(response));
        // }
        else if (strcmp(method, "GET") == 0 && strcmp(path, "/products") == 0)
        {
            char response[5000];
            int i;
            int len;

            len = 0;
            len += sprintf(response + len,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "[");

            i = 0;
            while (i < product_count)
            {
                len += sprintf(response + len,
                    "{\"id\":%d,\"name\":\"%s\",\"price\":%d}",
                    products[i].id,
                    products[i].name,
                    products[i].price);
                if (i < product_count - 1)
                    len += sprintf(response + len, ",");
                i++;
            }
            len += sprintf(response + len, "]");
            // char *response =
            //     "HTTP/1.1 200 OK\r\n"
            //     "Content-Type: application/json\r\n"
            //     "\r\n"
            //     "[{\"id\":1,\"name\":\"Shoes\"}]";

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "POST") == 0 && strcmp(path, "/cart") == 0)
        {
            int product_id;
            int i;
            int found;
            char response[300];

            product_id = -1;
            found = 0;

            if (body)
                sscanf(body, "{\"product_id\":%d}", &product_id);

            i = 0;
            while (i < cart_count)
            {
                if (cart[i].product_id == product_id)
                {
                    cart[i].quantity++;
                    found = 1;
                    break;
                }
                i++;
            }

            if (!found && product_id != -1 && cart_count < 100)
            {
                cart[cart_count].product_id = product_id;
                cart[cart_count].quantity = 1;
                cart_count++;
            }

            sprintf(response,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"message\":\"added\",\"product_id\":%d}",
                product_id);

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "GET") == 0 && strcmp(path, "/cart") == 0)
        {
            char response[5000];
            int i;
            int len;

            len = 0;
            len += sprintf(response + len,
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "[");

            i = 0;
            while (i < cart_count)
            {
                len += sprintf(response + len,
                    "{\"product_id\":%d,\"quantity\":%d}",
                    cart[i].product_id,
                    cart[i].quantity);
                if (i < cart_count - 1)
                    len += sprintf(response + len, ",");
                i++;
            }

            len += sprintf(response + len, "]");

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "GET") == 0 && strncmp(path, "/products/", 10) == 0)
        {
            char response[500];
            int product_id;
            int i;
            int found;

            product_id = atoi(path + 10);
            found = 0;
            i = 0;
            while (i < product_count)
            {
                if (products[i].id == product_id)
                {
                    sprintf(response,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"id\":%d,\"name\":\"%s\",\"price\":%d}",
                        products[i].id,
                        products[i].name,
                        products[i].price);
                    write(client_fd, response, strlen(response));
                    found = 1;
                    break;
                }
                i++;
            }
            if (!found)
            {
                char *not_found =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\":\"product not found\"}";
                write(client_fd, not_found, strlen(not_found));
            }
        }
        else if (strcmp(method, "DELETE") == 0 && strcmp(path, "/cart") == 0)
        {
            cart_count = 0;

            char *response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"message\":\"cart cleared\"}";

            write(client_fd, response, strlen(response));
        }
        else if (strcmp(method, "DELETE") == 0 && strncmp(path, "/cart/", 6) == 0)
        {
            int product_id;
            int i;
            int j;
            int found;

            product_id = atoi(path + 6);
            found = 0;

            i = 0;
            while (i < cart_count)
            {
                if (cart[i].product_id == product_id)
                {
                    j = i;
                    while (j < cart_count - 1)
                    {
                        cart[j] = cart[j + 1];
                        j++;
                    }
                    cart_count--;
                    found = 1;
                    break;
                }
                i++;
            }

            if (found)
            {
                char response[200];
                sprintf(response,
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"message\":\"item removed\",\"product_id\":%d}",
                    product_id);
                write(client_fd, response, strlen(response));
            }
            else
            {
                char *response =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\":\"item not found\"}";
                write(client_fd, response, strlen(response));
            }
        }
        else if (strcmp(method, "POST") == 0 && strcmp(path, "/orders") == 0)
        {
            int i;
            int total = 0;

            if (cart_count == 0)
            {
                char *response =
                    "HTTP/1.1 400 Bad Request\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\":\"cart is empty\"}";
                write(client_fd, response, strlen(response));
            }
            else
            {
                t_order *order = &orders[order_count];

                order->id = order_count + 1;
                order->item_count = cart_count;
                order->total = total;
                order->paid = 0;

                i = 0;
                while (i < cart_count)
                {
                    int price = get_price(cart[i].product_id);

                    // ❗ validation
                    if (price == -1)
                    {
                        char *response =
                            "HTTP/1.1 400 Bad Request\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"error\":\"invalid product\"}";

                        write(client_fd, response, strlen(response));
                        return 0;
                    }
                    order->items[i].product_id = cart[i].product_id;
                    order->items[i].quantity = cart[i].quantity;

                    total += price * cart[i].quantity;
                    i++;
                }

                order_count++;

                // خالی کردن cart بعد از order
                cart_count = 0;

                char response[500];
                sprintf(response,
                    "HTTP/1.1 201 Created\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"order_id\":%d,\"total\":%d}",
                    order->id, total);

                write(client_fd, response, strlen(response));
            }
        }
        else if (strcmp(method, "GET") == 0 && strncmp(path, "/orders/", 8) == 0)
        {
            int order_id;
            int i;
            int found;

            order_id = atoi(path + 8);
            i = 0;
            found = 0;

            while (i < order_count)
            {
                if (orders[i].id == order_id)
                {
                    char response[5000];
                    int j;
                    int len;

                    len = 0;
                    len += sprintf(response + len,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"id\":%d,\"items\":[",
                        orders[i].id);

                    j = 0;
                    while (j < orders[i].item_count)
                    {
                        len += sprintf(response + len,
                            "{\"product_id\":%d,\"quantity\":%d}",
                            orders[i].items[j].product_id,
                            orders[i].items[j].quantity);
                        if (j < orders[i].item_count - 1)
                            len += sprintf(response + len, ",");
                        j++;
                    }

                    len += sprintf(response + len, "]}");

                    write(client_fd, response, strlen(response));
                    found = 1;
                    break;
                }
                i++;
            }

            if (!found)
            {
                char *response =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\":\"order not found\"}";

                write(client_fd, response, strlen(response));
            }
        }
        else if (strcmp(method, "PUT") == 0 && strcmp(path, "/cart") == 0)
        {
            int product_id;
            int quantity;
            int i;
            int found;

            product_id = -1;
            quantity = -1;
            found = 0;

            if (body)
                sscanf(body, "{\"product_id\":%d,\"quantity\":%d}", &product_id, &quantity);

            // ❗ validation
            if (product_id <= 0 || quantity <= 0)
            {
                char *response =
                    "HTTP/1.1 400 Bad Request\r\n"
                    "Content-Type: application/json\r\n"
                    "\r\n"
                    "{\"error\":\"invalid product_id or quantity\"}";
                write(client_fd, response, strlen(response));
            }
            else
            {
                i = 0;
                while (i < cart_count)
                {
                    if (cart[i].product_id == product_id)
                    {
                        cart[i].quantity = quantity;
                        found = 1;
                        break;
                    }
                    i++;
                }

                if (found)
                {
                    char response[200];
                    sprintf(response,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"message\":\"updated\",\"product_id\":%d,\"quantity\":%d}",
                        product_id, quantity);
                    write(client_fd, response, strlen(response));
                }
                else
                {
                    char *response =
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"error\":\"item not found\"}";
                    write(client_fd, response, strlen(response));
                }
            }
        }
        else if (strcmp(method, "POST") == 0 && strncmp(path, "/pay/", 5) == 0)
        {
            int order_id = atoi(path + 5);
            int i = 0;

            while (i < order_count)
            {
                if (orders[i].id == order_id)
                {
                    if (orders[i].paid == 1)
                    {
                        char *response =
                            "HTTP/1.1 400 Bad Request\r\n"
                            "Content-Type: application/json\r\n"
                            "\r\n"
                            "{\"error\":\"already paid\"}";
                        write(client_fd, response, strlen(response));
                        close(client_fd);
                        return;
                    }

                    orders[i].paid = 1;

                    char response[300];
                    sprintf(response,
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "\r\n"
                        "{\"message\":\"payment successful\",\"order_id\":%d,\"total\":%d}",
                        orders[i].id,
                        orders[i].total);

                    write(client_fd, response, strlen(response));
                    close(client_fd);
                    return;
                }
                i++;
            }

            char *response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "\r\n"
                "{\"error\":\"order not found\"}";

            write(client_fd, response, strlen(response));
            close(client_fd);
            return;
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

