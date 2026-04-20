#include <unistd.h>
#include <string.h>
#include "response.h"

void	send_json_ok(int client_fd, const char *body)
{
	write(client_fd, "HTTP/1.1 200 OK\r\n", 17);
	write(client_fd, "Content-Type: application/json\r\n", 32);
	write(client_fd, "\r\n", 2);
	write(client_fd, body, strlen(body));
}

void	send_json_created(int client_fd, const char *body)
{
	write(client_fd, "HTTP/1.1 201 Created\r\n", 22);
	write(client_fd, "Content-Type: application/json\r\n", 32);
	write(client_fd, "\r\n", 2);
	write(client_fd, body, strlen(body));
}

void	send_json_bad_request(int client_fd, const char *body)
{
	write(client_fd, "HTTP/1.1 400 Bad Request\r\n", 26);
	write(client_fd, "Content-Type: application/json\r\n", 32);
	write(client_fd, "\r\n", 2);
	write(client_fd, body, strlen(body));
}

void	send_json_not_found(int client_fd, const char *body)
{
	write(client_fd, "HTTP/1.1 404 Not Found\r\n", 24);
	write(client_fd, "Content-Type: application/json\r\n", 32);
	write(client_fd, "\r\n", 2);
	write(client_fd, body, strlen(body));
}