#ifndef ORDERS_H
# define ORDERS_H

void	handle_post_orders(int client_fd);
void	handle_get_order_by_id(int client_fd, char *path);
void	handle_pay_order(int client_fd, char *path);

#endif