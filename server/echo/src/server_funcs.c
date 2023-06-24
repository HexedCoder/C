/** @file timed_funcs.c
*
* @brief This module implements the functions in timed_funcs.h
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include "server_funcs.h"

// 1 for run, 0 for stop
volatile sig_atomic_t gb_is_running = 1;

void main_signal_handler(int sig)
{
	if (!sig) {
		return;
	}

	switch (sig) {
	case SIGINT:
		// handle SIGINT (ctrl + c)
		main_set_shutdown(sig);
		break;

	case SIGUSR1:		// intentional fall through
	default:		// other signals not explicitly handled
		break;
	}
}				/* main_signal_handler */

bool valid_port(const char *user_port_str, int *port)
{
	if (!user_port_str || !port) {
		return false;
	}

	char *p_remaining = NULL;
	long num_found = strtol(user_port_str, &p_remaining, 10);

	// Check if strtol successfully parsed a number and reached the end
	if (p_remaining && *p_remaining == '\0' && 65536 > num_found
	    && 0 < num_found) {
		*port = num_found;
	} else {
		*port = 0;
	}

	return *port > 0 && *port < 65536;
}				/* valid_port */

void main_set_shutdown(int sig)
{
	// Avoid compiler warning
	(void)sig;

	gb_is_running = 0;
	pthread_kill(tcp_thread, SIGUSR1);
	pthread_kill(udp_thread, SIGUSR1);

}				/* main_set_shutdown */

void *tcp_handler(void *arg)
{

	client_struct_t *tcp = arg;
    char recv_buff[BUFF_LEN] = {0};
    strcpy(recv_buff, "Echo: ");

	while (gb_is_running) {
        int tcp_sock = accept(tcp->client_fd, NULL, NULL);

        if (-1 == tcp_sock) {
            continue;
        }

        for (;;) {
            size_t bytes_rcvd = recv(tcp_sock, recv_buff + 6, BUFF_LEN - 1, 0);

            if (2 > bytes_rcvd) {
                break;
            }

            send(tcp_sock, recv_buff, bytes_rcvd + 6, 0);
        }
		close(tcp_sock);

	}

	close(tcp->client_fd);

	return arg;

}				/* tcp_handler */

void *udp_handler(void *arg)
{
	struct sockaddr_in6 client = { 0 };
	client_struct_t *udp = arg;

	char recv_buff[BUFF_LEN] = {0};
    strcpy(recv_buff, "Echo: ");

	int addr_len = sizeof(struct sockaddr_in6);

	while (gb_is_running) {

		ssize_t bytes_recv =
		    recvfrom(udp->client_fd, recv_buff + 6, BUFF_LEN - 7, 0,
			     (struct sockaddr *)&client,
			     (socklen_t *) & addr_len);

		if (-1 == bytes_recv) {
			continue;
		} else if (2 > bytes_recv) {
            break;
        }

		sendto(udp->client_fd, recv_buff, bytes_recv + 6, 0,
		       (struct sockaddr *)&client, addr_len);
	}

	close(udp->client_fd);

	return arg;

}				/* udp_handler */

/* end of file */
