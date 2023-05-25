/**
* @file server_driver.c
* @author Jacob Hitchcox
* @brief
*
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*
*/

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
	int ret_val = 0;

	char message[256] = "Welcome to the server, Jacob";
	int server_sock = 0;
	int client_sock = 0;

	// socket
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		ret_val = 1;
		goto EARLY_EXIT;
	}
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(9002);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
		perror("bind");
		ret_val = 1;
		goto EARLY_EXIT;
	}
	// listen
	if (listen(server_sock, 5) == -1) {
		perror("listen");
		ret_val = 1;
		goto EARLY_EXIT;
	}

	int client_count = 0;

	while (client_count < 5) {
		// accept a connection on socket
		if ((client_sock = accept(server_sock, NULL, NULL)) == -1) {
			continue;
		} else {
			send(client_sock, message, sizeof(message), 0);
			++client_count;
			close(client_sock);
		}
	}

	printf("Closing Server\n");
	close(server_sock);

 EARLY_EXIT:
	return ret_val;
}				/* main() */

/*** end of file ***/
