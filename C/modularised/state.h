#ifndef STATE_H
# define STATE_H

typedef struct s_cart_item
{
	int	product_id;
	int	quantity;
}	t_cart_item;

typedef struct s_product
{
	int		id;
	char	*name;
	int		price;
}	t_product;

typedef struct s_order_item
{
	int	product_id;
	int	quantity;
}	t_order_item;

typedef struct s_order
{
	int				id;
	t_order_item	items[100];
	int				item_count;
	int				total;
	int				paid;
}	t_order;

extern t_cart_item	g_cart[100];
extern int			g_cart_count;

extern t_product	g_products[];
extern int			g_products_count;

extern t_order		g_orders[100];
extern int			g_order_count;

#endif