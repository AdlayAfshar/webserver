#include <stdio.h>
#include <string.h>
#include "response.h"
#include "products.h"
#include "cart.h"
#include "orders.h"
#include "routes.h"

void	handle_request(int client_fd, char *buffer)
{
	char	method[10];
	char	path[100];
	char	*body;

	sscanf(buffer, "%s %s", method, path);
	body = strstr(buffer, "\r\n\r\n");
	if (body)
		body += 4;
	printf("Method: %s\n", method);
	printf("Path: %s\n", path);
	if (body)
		printf("Body: %s\n", body);
	if (strcmp(method, "GET") == 0 && strcmp(path, "/health") == 0)
		send_json_ok(client_fd, "{\"status\":\"ok\"}");
	else if (strcmp(method, "GET") == 0 && strcmp(path, "/products") == 0)
		handle_get_products(client_fd);
	else if (strcmp(method, "GET") == 0 && strncmp(path, "/products/", 10) == 0)
		handle_get_product_by_id(client_fd, path);
	else if (strcmp(method, "POST") == 0 && strcmp(path, "/cart") == 0)
		handle_post_cart(client_fd, body);
	else if (strcmp(method, "GET") == 0 && strcmp(path, "/cart") == 0)
		handle_get_cart(client_fd);
	else if (strcmp(method, "PUT") == 0 && strcmp(path, "/cart") == 0)
		handle_put_cart(client_fd, body);
	else if (strcmp(method, "DELETE") == 0 && strcmp(path, "/cart") == 0)
		handle_delete_cart(client_fd);
	else if (strcmp(method, "DELETE") == 0 && strncmp(path, "/cart/", 6) == 0)
		handle_delete_cart_item(client_fd, path);
	else if (strcmp(method, "POST") == 0 && strcmp(path, "/orders") == 0)
		handle_post_orders(client_fd);
	else if (strcmp(method, "GET") == 0 && strncmp(path, "/orders/", 8) == 0)
		handle_get_order_by_id(client_fd, path);
	else if (strcmp(method, "POST") == 0 && strncmp(path, "/pay/", 5) == 0)
		handle_pay_order(client_fd, path);
	else
		send_json_not_found(client_fd, "{\"error\":\"route not found\"}");
}