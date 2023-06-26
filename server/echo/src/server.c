/**
 * @file server.c
 * @brief TCP and UDP server that echoes the clients message
 *
 * @copyright Copyright (c) 2023 Jacob Hitchcox
 *
 */

#include "server_funcs.h"

#include "signals.h"

pthread_t tcp_thread;
pthread_t udp_thread;

int main(int argc, char *argv[])
{

	// add SIGINT handling capabilities
	if (0 != establish_handler(SIGINT, main_signal_handler)) {
		fprintf(stderr, "SIGINT Failed.\n");
	}
	// add SIGUSR1 handling capabilities
	if (0 != establish_handler(SIGUSR1, main_signal_handler)) {
		fprintf(stderr, "SIGUSR1 Failed.\n");
	}

	block_signals(2, SIGINT, SIGUSR1);

	int ret_val = 0;
	int port = 37;

	if (2 == argc) {
		if (!valid_port(argv[1], &port)) {
			fprintf(stderr,
				"Invalid argument, setting port to 37\n");
			port = 37;
		}
	} else if (2 < argc) {
		fprintf(stderr, "Usage:\n\t./server [optional port]\n");
		ret_val = -1;
		goto EARLY_EXIT;
	}

	struct sockaddr_in6 server = { 0 };
	int tcp_sock, udp_sock;

	tcp_sock = socket(AF_INET6, SOCK_STREAM, 0);
	if (-1 == tcp_sock) {
		perror("tcp socket error\n");
		ret_val = -1;
		goto EARLY_EXIT;
	}

	udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (-1 == udp_sock) {
		perror("tcp socket error\n");
		ret_val = -1;
		goto TCP_CLOSE;
	}
	// allows port to be reused if improperly closed
	if (setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &(int) { 1 },
		       sizeof(int)) < 0) {
		perror("setsockopt SO_REUSEADDR failed");
		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}
	// setup for IPV4 and IPV6
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(port);
	server.sin6_addr = in6addr_any;

	if (-1 == bind(tcp_sock, (struct sockaddr *)&server, sizeof(server))) {
		perror("TCP bind error");

		if (port < 1024 && 0 != geteuid()) {
			fprintf(stderr, "run with sudo for port under 1024\n");
		}

		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}

	if (-1 == bind(udp_sock, (struct sockaddr *)&server, sizeof(server))) {
		perror("UDP bind error");
		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}
	// listen for 5 total clients
	if (-1 == listen(tcp_sock, MAX_CLIENTS)) {
		perror("Listen TCP error");
		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}

	client_struct_t tcp_client;
	tcp_client.client_fd = tcp_sock;

	client_struct_t udp_client;
	udp_client.client_fd = udp_sock;

	unblock_signals(1, SIGUSR1);

	if (pthread_create(&tcp_thread, NULL, tcp_handler, &tcp_client)) {
		perror("tcp pthread create");
		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}

	if (pthread_create(&udp_thread, NULL, udp_handler, &udp_client)) {
		perror("udp pthread create");
		ret_val = -1;
		goto TCP_UDP_CLOSE;
	}

	unblock_signals(1, SIGINT);

	printf("Server Active: waiting for connections...\n");

	pthread_join(tcp_thread, NULL);
	fprintf(stdout, "TCP server disconnected\n");
	pthread_join(udp_thread, NULL);
	fprintf(stdout, "UDP server disconnected\n");

 TCP_UDP_CLOSE:
	close(udp_sock);

 TCP_CLOSE:
	close(tcp_sock);

 EARLY_EXIT:
	return ret_val;
}				/* main */

/* end of file */
