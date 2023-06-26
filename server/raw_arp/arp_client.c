/** @file arp_client.c
*
* @brief This module implements a raw layer 2 arp request using the functions in arp_client.
*
* @par
* COPYRIGHT NOTICE: (c) 2023 Jacob Hitchcox
*/

#include "arp_client.h"

char *get_mac_address(const char *interface, char *mac_address)
{
	if (!interface || !mac_address) {
		return NULL;
	}

	char *ret_val = NULL;

	int h_file_descriptor = 0;
	struct ifreq network_interface = { 0 };
	unsigned char *mac = NULL;

	h_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if (!h_file_descriptor) {
		return ret_val;
	}

	network_interface.ifr_addr.sa_family = AF_INET;
	strncpy(network_interface.ifr_name, interface, IFNAMSIZ - 1);

	if (0 == ioctl(h_file_descriptor, SIOCGIFHWADDR, &network_interface)) {
		mac = (unsigned char *)network_interface.ifr_hwaddr.sa_data;

		// store mac address
		snprintf(mac_address, MAC_ADDRESS_LENGTH,
			 "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", mac[0], mac[1],
			 mac[2], mac[3], mac[4], mac[5]);
		ret_val = mac_address;
	}

	close(h_file_descriptor);

	return ret_val;
}

char *get_ip_address(const char *interface, char *ip_address)
{
	if (!interface || !ip_address) {
		return NULL;
	}

	char *ret_val = NULL;

	int h_file_descriptor = 0;
	struct ifreq network_interface = { 0 };

	h_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if (!h_file_descriptor) {
		return ret_val;
	}

	network_interface.ifr_addr.sa_family = AF_INET;
	strncpy(network_interface.ifr_name, interface, IFNAMSIZ - 1);

	if (0 == ioctl(h_file_descriptor, SIOCGIFADDR, &network_interface)) {
		struct sockaddr_in *ip_addr =
		    (struct sockaddr_in *)&network_interface.ifr_addr;

		// store ip address
		memcpy(ip_address, inet_ntoa(ip_addr->sin_addr),
		       IP_ADDRESS_LENGTH);
		ret_val = ip_address;
	}

	close(h_file_descriptor);

	return ret_val;
}

int main(int argc, char *argv[])
{

	// initialize to failure
	int ret_val = 1;

	// ensure root access and proper arguments
	if (3 != argc || 0 != geteuid()) {
		const char *arg_msg = "<Interface Name> <Target IP>";
		fprintf(stderr, "Usage: %s %s\n", argv[0], arg_msg);
		fprintf(stderr, "Must run as root\n");
		goto EARLY_EXIT;
	}

	unsigned char packet_buffer[PKT_LEN] = { 0 };
	char mac_address[MAC_ADDRESS_LENGTH] = { 0 };
	char ip_address[IP_ADDRESS_LENGTH] = { 0 };

	// create struct from buffer sections
	struct ether_header *eth = (struct ether_header *)packet_buffer;
	struct arphdr_t *arp =
	    (struct arphdr_t *)(packet_buffer + sizeof(struct ether_header));

	// Set destination MAC address for broadcast
	uint64_t num = 0xFFFFFFFFFFFF;
	memcpy(packet_buffer, &num, 6);

	// Convert Interface name to MAX address
	const char *interface = argv[1];
	char *mac_res = get_mac_address(interface, mac_address);

	if (!mac_res) {
		fprintf(stderr, "No MAC Address found for %s\n", argv[1]);
		goto EARLY_EXIT;
	}
	// Convert source MAC address from string to binary
	sscanf(mac_res, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth->ether_shost[0],
	       &eth->ether_shost[1], &eth->ether_shost[2], &eth->ether_shost[3],
	       &eth->ether_shost[4], &eth->ether_shost[5]);

	printf("%s\n", mac_res);

	// Set Ethernet frame type (IP)
	eth->ether_type = htons(ETHERTYPE_ARP);

	// raw layer 2 protocol socket
	int raw_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (raw_socket < 0) {
		perror("socket");
		goto EARLY_EXIT;
	}

	struct ifreq ifr = { 0 };
	strncpy(ifr.ifr_name, interface, IFNAMSIZ);
	ioctl(raw_socket, SIOCGIFINDEX, &ifr);
	int ifindex = ifr.ifr_ifindex;

	// Tell kernel we are building packet
	if (setsockopt
	    (raw_socket, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr)) < 0) {
		perror("setsockopt");
		goto CLOSE_SOCKET;
	}
	// ARP Header (values begin zeroed)
	arp->ar_hrd = htons(1);
	arp->ar_pro = htons(IPV4_PROTO);
	arp->ar_hln = 0x06;
	arp->ar_pln = 0x04;
	arp->ar_op = 0x0100;
	memcpy(arp->__ar_sha, eth->ether_shost, 6);

	char *my_ip = get_ip_address(interface, ip_address);

	if (!my_ip) {
		fprintf(stderr, "Unable to resolve user IP address\n");
		goto CLOSE_SOCKET;
	}
	// add src IP to packet
	sscanf(my_ip, "%hhu.%hhu.%hhu.%hhu", &arp->__ar_sip[0],
	       &arp->__ar_sip[1], &arp->__ar_sip[2], &arp->__ar_sip[3]);

	// add dst IP to packet
	sscanf(argv[2], "%hhu.%hhu.%hhu.%hhu", &arp->__ar_tip[0],
	       &arp->__ar_tip[1], &arp->__ar_tip[2], &arp->__ar_tip[3]);

	struct sockaddr_ll sa = { 0 };
	sa.sll_ifindex = ifindex;
	sa.sll_protocol = htons(ETH_P_ALL);

	size_t total_len =
	    sizeof(struct ether_header) + sizeof(struct iphdr) +
	    sizeof(struct udphdr);

	// view crafted packet
	for (size_t i = 0; i < total_len; i++) {
		printf("%02X ", packet_buffer[i]);
	}
	puts("");

	if (sendto
	    (raw_socket, packet_buffer, total_len, 0, (struct sockaddr *)&sa,
	     sizeof(struct sockaddr_ll)) < 0) {
		perror("Unable to send");
		goto CLOSE_SOCKET;
	}

	printf("ARP Request Sent\n");
	ret_val = 0;

 CLOSE_SOCKET:
	close(raw_socket);

 EARLY_EXIT:
	return ret_val;

}				/* main() */
