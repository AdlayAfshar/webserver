#ifndef RESPONSE_H
# define RESPONSE_H

void	send_json_ok(int client_fd, const char *body);
void	send_json_created(int client_fd, const char *body);
void	send_json_bad_request(int client_fd, const char *body);
void	send_json_not_found(int client_fd, const char *body);

#endif