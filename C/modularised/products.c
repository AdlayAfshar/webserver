#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "state.h"
#include "response.h"
#include "products.h"

int	get_price(int product_id)
{
	int	i;

	i = 0;
	while (i < g_products_count)
	{
		if (g_products[i].id == product_id)
			return (g_products[i].price);
		i++;
	}
	return (-1);
}

int	product_exists(int product_id)
{
	return (get_price(product_id) != -1);
}

void	handle_get_products(int client_fd)
{
	char	response[5000];
	int		i;
	int		len;

	len = 0;
	len += sprintf(response + len, "[");
	i = 0;
	while (i < g_products_count)
	{
		len += sprintf(response + len,
				"{\"id\":%d,\"name\":\"%s\",\"price\":%d}",
				g_products[i].id, g_products[i].name, g_products[i].price);
		if (i < g_products_count - 1)
			len += sprintf(response + len, ",");
		i++;
	}
	len += sprintf(response + len, "]");
	send_json_ok(client_fd, response);
}

void	handle_get_product_by_id(int client_fd, char *path)
{
	char	response[500];
	int		product_id;
	int		i;

	product_id = atoi(path + 10);
	i = 0;
	while (i < g_products_count)
	{
		if (g_products[i].id == product_id)
		{
			sprintf(response, "{\"id\":%d,\"name\":\"%s\",\"price\":%d}",
				g_products[i].id, g_products[i].name, g_products[i].price);
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	send_json_not_found(client_fd, "{\"error\":\"product not found\"}");
}