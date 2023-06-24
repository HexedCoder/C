/** @file client.h
*
* @brief This module implements a raw layer 3 client socket
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include <arpa/inet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PKT_LEN 8192

/*
 * @brief Calculates checksum value for buffer of data.
 *
 * @param buf: Buffer containing the data
 * @param size: Size of the buffer
 *
 * @return The checksum value, or 0 on failure
 */
uint16_t get_checksum(uint8_t * buf, uint16_t size)
{
	if (!buf) {
		return 0;
	}

	uint32_t sum = 0;

	// Calculate the sum in 16-bit chunks
	while (size > 1) {
		sum += *(uint16_t *) buf;
		buf += 2;
		size -= 2;
	}

	// Handle the remaining byte if the size is odd
	if (size > 0) {
		sum += *(uint8_t *) buf;
	}
	// Fold 32-bit sum to 16 bits
	while (sum >> 16) {
		sum = (sum & 0xFFFF) + (sum >> 16);
	}

	return (uint16_t) (~sum);
}				/* get_checksum() */

/**
 * @brief Validates a port number represented as a string
 *
 * @param port The port number string to validate.
 *
 * @return long port on success, otherwise returns -1.
 */
long validate_port(const char *port)
{
	if (!port) {
		return -1;
	}
	char *end_ptr = (char *)port;
	long res = strtol(port, &end_ptr, 10);

	if (*end_ptr) {
		return 0;
	}

	return res;
}				/* validate_port() */

int main(int argc, char *argv[])
{

	// initialize to failure
	int ret_val = 1;

	// ensure root access and proper arguments
	if (5 != argc || 0 != geteuid()) {
		const char *arg_msg = "<src IP> <src port> <dst IP> <dst port>";
		fprintf(stderr, "Usage: %s %s\n", argv[0], arg_msg);
		goto EARLY_EXIT;
	}

	u_int32_t src_addr = inet_addr(argv[1]);
	u_int32_t dst_addr = inet_addr(argv[3]);

	// populate src and dst ports
	u_int16_t src_port = validate_port(argv[2]);
	u_int16_t dst_port = validate_port(argv[4]);

	if (!src_port || !dst_port) {
		fprintf(stderr, "Invalid %s port\n", !src_port ? "src" : "dst");
		goto EARLY_EXIT;
	}

	char packet_buffer[PKT_LEN] = { 0 };

	// create struct from buffer sections
	struct iphdr *ip = (struct iphdr *)packet_buffer;
	struct udphdr *udp = (struct udphdr *)(packet_buffer + sizeof(struct
								      iphdr));

	// setup intended destination details
	struct addrinfo hints = { 0 };
	hints.ai_family = PF_INET;

	// result storage struct
	struct addrinfo *results;

	// resolve server ip
	int err = getaddrinfo(argv[3], argv[4], &hints, &results);

	// check for successful dns result
	if (err != 0) {
		fprintf(stderr, "Cannot resolve requested address\n");
		goto EARLY_EXIT;
	}
	// raw UDP protocol socket
	int raw_socket = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
	if (raw_socket < 0) {
		perror("socket");
		goto FREE_ADDR_INFO;
	}
	// Tell kernel we are building packet
	if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, &(int) { 1 },
		       sizeof(int)) < 0) {
		perror("setsockopt");
		goto CLOSE_SOCKET;
	}
	// IP Header (values begin zeroed)
	ip->ihl = 5;
	ip->version = 4;
	ip->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr);
	ip->id = htons(12345);
	ip->ttl = 255;
	ip->protocol = IPPROTO_UDP;
	ip->saddr = src_addr;
	ip->daddr = dst_addr;

	// calculate the actual checksum (officially optional in IPV4)
	ip->check = get_checksum((uint8_t *) ip, sizeof(struct iphdr));

	// UDP Header
	udp->source = htons(src_port);
	udp->dest = htons(dst_port);
	udp->len = htons(sizeof(struct udphdr));

    // view crafted packet
    for (size_t i = 0; i < ip->tot_len; i++) {
        printf("%02X ", packet_buffer[i]);
    }
    puts("");


    if (sendto
	    (raw_socket, packet_buffer, ip->tot_len, 0, results->ai_addr,
	     results->ai_addrlen) < 0) {
		perror("Unable to send");
		goto CLOSE_SOCKET;
	}

	printf("Packet sent\n");
	ret_val = 0;

 CLOSE_SOCKET:
	close(raw_socket);

 FREE_ADDR_INFO:
	freeaddrinfo(results);

 EARLY_EXIT:
	return ret_val;

}				/* main() */
