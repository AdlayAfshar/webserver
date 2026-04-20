#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "routes.h"
#include "server.h"

void	start_server(void)
{
	int					server_fd;
	int					client_fd;
	int					addrlen;
	struct sockaddr_in	address;
	char				buffer[30000];

	addrlen = sizeof(address);
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(3000);
	bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	listen(server_fd, 10);
	printf("Server running on port 3000...\n");
	while (1)
	{
		client_fd = accept(server_fd,
				(struct sockaddr *)&address, (socklen_t *)&addrlen);
		memset(buffer, 0, sizeof(buffer));
		read(client_fd, buffer, 30000);
		printf("==================================\n");
		printf("%s\n", buffer);
		printf("==================================\n");
		handle_request(client_fd, buffer);
		close(client_fd);
	}
}