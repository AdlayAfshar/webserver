#ifndef CART_H
# define CART_H

void	handle_post_cart(int client_fd, char *body);
void	handle_get_cart(int client_fd);
void	handle_put_cart(int client_fd, char *body);
void	handle_delete_cart(int client_fd);
void	handle_delete_cart_item(int client_fd, char *path);

#endif