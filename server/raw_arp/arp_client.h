#ifndef ARP_CLIENT_H
#define ARP_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netpacket/packet.h>

#define PKT_LEN 8192
#define MAC_ADDRESS_LENGTH 18
#define IP_ADDRESS_LENGTH 16
#define IPV4_PROTO 0x0800

struct arphdr_t {
    unsigned short int ar_hrd;	/* Format of hardware address.  */
    unsigned short int ar_pro;	/* Format of protocol address.  */
    unsigned char ar_hln;	/* Length of hardware address.  */
    unsigned char ar_pln;	/* Length of protocol address.  */
    unsigned short int ar_op;	/* ARP opcode (command).  */
    unsigned char __ar_sha[ETH_ALEN];	/* Sender hardware address.  */
    unsigned char __ar_sip[4];	/* Sender IP address.  */
    unsigned char __ar_tha[ETH_ALEN];	/* Target hardware address.  */
    unsigned char __ar_tip[4];	/* Target IP address.  */
};

/**
 * @brief Retrieves the IP address associated with a network interface
 *
 * @param interface The name of the network interface
 * @param ip_address A pointer to a character array to store the IP address
 *
 * @return A pointer to the IP address stored in ip_address on success, or NULL on failure
 */
char* get_ip_address(const char *interface, char *ip_address);

/**
 * @brief Retrieve the MAC address of a network interface
 *
 * @param interface The name of the network interface to retrieve the MAC address from
 * @param mac_address A buffer to store the retrieved MAC address
 *
 * @return A pointer to the mac_address buffer on success, otherwise NULL
 */
char* get_mac_address(const char *interface, char *mac_address);

#endif				/* ARP_CLIENT_H */

/* end of file */
