#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "state.h"
#include "response.h"
#include "products.h"
#include "cart.h"

void	handle_post_cart(int client_fd, char *body)
{
	int		product_id;
	int		i;
	char	response[300];

	product_id = -1;
	if (body)
		sscanf(body, "{\"product_id\":%d}", &product_id);
	if (product_id <= 0 || !product_exists(product_id))
	{
		send_json_bad_request(client_fd, "{\"error\":\"invalid product_id\"}");
		return ;
	}
	i = 0;
	while (i < g_cart_count)
	{
		if (g_cart[i].product_id == product_id)
		{
			g_cart[i].quantity++;
			sprintf(response, "{\"message\":\"added\",\"product_id\":%d}", product_id);
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	g_cart[g_cart_count].product_id = product_id;
	g_cart[g_cart_count].quantity = 1;
	g_cart_count++;
	sprintf(response, "{\"message\":\"added\",\"product_id\":%d}", product_id);
	send_json_ok(client_fd, response);
}

void	handle_get_cart(int client_fd)
{
	char	response[5000];
	int		i;
	int		len;

	len = 0;
	len += sprintf(response + len, "[");
	i = 0;
	while (i < g_cart_count)
	{
		len += sprintf(response + len,
				"{\"product_id\":%d,\"quantity\":%d}",
				g_cart[i].product_id, g_cart[i].quantity);
		if (i < g_cart_count - 1)
			len += sprintf(response + len, ",");
		i++;
	}
	len += sprintf(response + len, "]");
	send_json_ok(client_fd, response);
}

void	handle_put_cart(int client_fd, char *body)
{
	int		product_id;
	int		quantity;
	int		i;
	char	response[300];

	product_id = -1;
	quantity = -1;
	if (body)
		sscanf(body, "{\"product_id\":%d,\"quantity\":%d}", &product_id, &quantity);
	if (product_id <= 0 || quantity <= 0 || !product_exists(product_id))
	{
		send_json_bad_request(client_fd, "{\"error\":\"invalid product_id or quantity\"}");
		return ;
	}
	i = 0;
	while (i < g_cart_count)
	{
		if (g_cart[i].product_id == product_id)
		{
			g_cart[i].quantity = quantity;
			sprintf(response,
				"{\"message\":\"updated\",\"product_id\":%d,\"quantity\":%d}",
				product_id, quantity);
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	send_json_not_found(client_fd, "{\"error\":\"item not found\"}");
}

void	handle_delete_cart(int client_fd)
{
	g_cart_count = 0;
	send_json_ok(client_fd, "{\"message\":\"cart cleared\"}");
}

void	handle_delete_cart_item(int client_fd, char *path)
{
	int		product_id;
	int		i;
	int		j;
	char	response[300];

	product_id = atoi(path + 6);
	i = 0;
	while (i < g_cart_count)
	{
		if (g_cart[i].product_id == product_id)
		{
			j = i;
			while (j < g_cart_count - 1)
			{
				g_cart[j] = g_cart[j + 1];
				j++;
			}
			g_cart_count--;
			sprintf(response,
				"{\"message\":\"item removed\",\"product_id\":%d}", product_id);
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	send_json_not_found(client_fd, "{\"error\":\"item not found\"}");
}