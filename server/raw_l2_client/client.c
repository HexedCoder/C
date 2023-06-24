/** @file client.c
*
* @brief This module implements a raw layer 2 client socket
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netpacket/packet.h>

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
	if (7 != argc || 0 != geteuid()) {
		const char *arg_msg =
		    "<src MAC> <dst MAC> <src IP> <src port> <dst IP> <dst port>";
		fprintf(stderr, "Usage: %s %s\n", argv[0], arg_msg);
		goto EARLY_EXIT;
	}

	u_int32_t src_addr, dst_addr;
	src_addr = inet_addr(argv[3]);
	dst_addr = inet_addr(argv[5]);

	// populate src and dst ports
	u_int16_t src_port = validate_port(argv[4]);
	u_int16_t dst_port = validate_port(argv[6]);

	if (!src_port || !dst_port) {
		fprintf(stderr, "Invalid %s port\n", !src_port ? "src" : "dst");
		goto EARLY_EXIT;
	}

	unsigned char packet_buff[PKT_LEN] = { 0 };

	// create struct from buffer sections
	struct ether_header *eth = (struct ether_header *)packet_buff;
	struct iphdr *ip =
	    (struct iphdr *)(packet_buff + sizeof(struct ether_header));
	struct udphdr *udp =
	    (struct udphdr *)(packet_buff + sizeof(struct ether_header) +
			      sizeof(struct iphdr));

	// Convert source and dest MAC addresses from string to binary
	sscanf(argv[1], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth->ether_shost[0],
	       &eth->ether_shost[1], &eth->ether_shost[2], &eth->ether_shost[3],
	       &eth->ether_shost[4], &eth->ether_shost[5]);
	sscanf(argv[2], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth->ether_dhost[0],
	       &eth->ether_dhost[1], &eth->ether_dhost[2], &eth->ether_dhost[3],
	       &eth->ether_dhost[4], &eth->ether_dhost[5]);

	// Set Ethernet frame type (IP)
	eth->ether_type = htons(ETHERTYPE_IP);

	// setup intended destination details
	struct addrinfo hints = { 0 };
	hints.ai_family = PF_INET;

	// result storage struct
	struct addrinfo *results;

	// resolve server ip
	int err = getaddrinfo(argv[5], argv[6], &hints, &results);

	// check for successful dns result
	if (err != 0) {
		fprintf(stderr, "Cannot resolve requested address\n");
		goto EARLY_EXIT;
	}
	// raw layer 2 protocol socket
	int raw_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (raw_socket < 0) {
		perror("socket");
		goto FREE_ADDR_INFO;
	}

	struct ifreq ifr = { 0 };
	strncpy(ifr.ifr_name, "lo", IFNAMSIZ);
	ioctl(raw_socket, SIOCGIFINDEX, &ifr);
	int ifindex = ifr.ifr_ifindex;

	// Tell kernel we are building packet
	if (setsockopt
	    (raw_socket, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0) {
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

	struct sockaddr_ll sa = { 0 };
	sa.sll_ifindex = ifindex;
	sa.sll_protocol = htons(ETH_P_ALL);

	size_t total_len =
	    sizeof(struct ether_header) + sizeof(struct iphdr) +
	    sizeof(struct udphdr);

	// view crafted packet
	for (size_t i = 0; i < total_len; i++) {
		printf("%02X ", packet_buff[i]);
	}
	puts("");

	if (sendto
	    (raw_socket, packet_buff, total_len, 0, (struct sockaddr *)&sa,
	     sizeof(struct sockaddr_ll)) < 0) {
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
