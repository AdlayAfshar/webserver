#ifndef PRODUCTS_H
# define PRODUCTS_H

int		get_price(int product_id);
int		product_exists(int product_id);
void	handle_get_products(int client_fd);
void	handle_get_product_by_id(int client_fd, char *path);

#endif