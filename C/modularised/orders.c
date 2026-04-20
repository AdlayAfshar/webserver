#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "response.h"
#include "products.h"
#include "orders.h"

void	handle_post_orders(int client_fd)
{
	t_order	*order;
	char	response[500];
	int		i;
	int		price;
	int		total;

	if (g_cart_count == 0)
	{
		send_json_bad_request(client_fd, "{\"error\":\"cart is empty\"}");
		return ;
	}
	order = &g_orders[g_order_count];
	order->id = g_order_count + 1;
	order->item_count = g_cart_count;
	order->paid = 0;
	total = 0;
	i = 0;
	while (i < g_cart_count)
	{
		price = get_price(g_cart[i].product_id);
		if (price == -1)
		{
			send_json_bad_request(client_fd, "{\"error\":\"invalid product\"}");
			return ;
		}
		order->items[i].product_id = g_cart[i].product_id;
		order->items[i].quantity = g_cart[i].quantity;
		total += price * g_cart[i].quantity;
		i++;
	}
	order->total = total;
	g_order_count++;
	g_cart_count = 0;
	sprintf(response, "{\"order_id\":%d,\"total\":%d}", order->id, order->total);
	send_json_created(client_fd, response);
}

void	handle_get_order_by_id(int client_fd, char *path)
{
	char	response[5000];
	int		order_id;
	int		i;
	int		j;
	int		len;

	order_id = atoi(path + 8);
	i = 0;
	while (i < g_order_count)
	{
		if (g_orders[i].id == order_id)
		{
			len = 0;
			len += sprintf(response + len,
					"{\"id\":%d,\"total\":%d,\"paid\":%d,\"items\":[",
					g_orders[i].id, g_orders[i].total, g_orders[i].paid);
			j = 0;
			while (j < g_orders[i].item_count)
			{
				len += sprintf(response + len,
						"{\"product_id\":%d,\"quantity\":%d}",
						g_orders[i].items[j].product_id,
						g_orders[i].items[j].quantity);
				if (j < g_orders[i].item_count - 1)
					len += sprintf(response + len, ",");
				j++;
			}
			len += sprintf(response + len, "]}");
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	send_json_not_found(client_fd, "{\"error\":\"order not found\"}");
}

void	handle_pay_order(int client_fd, char *path)
{
	char	response[300];
	int		order_id;
	int		i;

	order_id = atoi(path + 5);
	i = 0;
	while (i < g_order_count)
	{
		if (g_orders[i].id == order_id)
		{
			if (g_orders[i].paid == 1)
			{
				send_json_bad_request(client_fd, "{\"error\":\"already paid\"}");
				return ;
			}
			g_orders[i].paid = 1;
			sprintf(response,
				"{\"message\":\"payment successful\",\"order_id\":%d,\"total\":%d}",
				g_orders[i].id, g_orders[i].total);
			send_json_ok(client_fd, response);
			return ;
		}
		i++;
	}
	send_json_not_found(client_fd, "{\"error\":\"order not found\"}");
}