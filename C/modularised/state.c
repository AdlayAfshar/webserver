#include "state.h"

t_cart_item	g_cart[100];
int			g_cart_count = 0;

t_product	g_products[] = {
	{1, "Shoes", 100},
	{2, "Bag", 200},
	{3, "Hat", 50}
};

int			g_products_count = 3;

t_order		g_orders[100];
int			g_order_count = 0;