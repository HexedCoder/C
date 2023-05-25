/** @file client_driver.h
*
* @brief This module implements the functions in atm_funcs.h
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

# define MAX_BUFFER 256

int main(void)
{
	int sock = 0;

	// create endpoint for communication, returns fd referring to the endpoint
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9002);
//	server_address.sin_addr.s_addr = inet_addr("0.0.0.0");
	server_address.sin_addr.s_addr = INADDR_ANY;

	// initiate connection on a socket
	if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		perror("connect");
		exit(1);
	}

	char str[MAX_BUFFER] = {0};
	for (;;) {
		ssize_t read = 0;
		if ((read = recv(sock, str, sizeof(str) - 1, 0)) > 0) {
			str[strcspn(str, "\n")] = 0;
			printf("%s\n", str);
		}
		if (!strcmp("quit", buffer)) {
			break;
		}
	}
	close(sock);

	return 0;
}				/* main() */
