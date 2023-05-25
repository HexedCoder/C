#!/usr/bin/python3

# For building the socket
import socket

# For system level commands
import sys

# For establishing the packet structure (Used later on), this will allow direct access to the methods and functions in the struct module
from struct import *

# For encoding

import base64               # base64 module
import binascii            # binascii module

# Create a raw socket.
try:
    s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
except socket.error as msg:
    print(msg)
    sys.exit()

# 0 or IPPROTO_TCP for STREAM and 0 or IPPROTO_UDP for DGRAM. (man ip7). For SOCK_RAW you may specify a valid IANA IP protocol defined in RFC 1700 assigned numbers.
# IPPROTO_IP creates a socket that sends/receives raw data for IPv4-based protocols (TCP, UDP, etc). It will handle the IP headers for you, but you are responsible for processing/creating additional protocol data inside the IP payload.
# IPPROTO_RAW creates a socket that sends/receives raw data for any kind of protocol. It will not handle any headers for you, you are responsible for processing/creating all payload data, including IP and additional headers.(link)

packet = ''

src_ip = ""
dst_ip = ""


#######################
##Build Packet Header##
#######################

# Lets add the IPv4 header information
ip_ver_ihl = 69                         # This is putting the decimal conversion of 0x45 for Version and Internet Header Length
ip_tos = 96                             # This combines the DSCP and ECN feilds. Type of service/QoS
ip_len = 0                              # The kernel will fill in the actually length of the packet
ip_id = 1984                            # This sets the IP Identification for the packet
ip_frag = 0                             # This sets fragmentation to off
ip_ttl = 64                             # This determines the TTL of the packet when leaving the machine
ip_proto = 16                           # This sets the IP protocol to 6 (TCP) so additional headers are required (reference IANA)
ip_check = 0                            # The kernel will fill in the checksum for the packet
ip_srcadd = socket.inet_aton(src_ip)    # inet_aton(string) will convert an IP address to a 32 bit binary number
ip_dstadd = socket.inet_aton(dst_ip)    # inet_aton(string) will convert an IP address to a 32 bit binary number


########################
## Pack the IP Header ##
########################

# This portion creates the header by packing the above variables into a structure. The ! in the string means 'Big-Endian' network order, while the code following specifies how to store the info. Endian explained. Refer to link for character meaning.

ip_header = pack('!BBHHHBBH4s4s' , ip_ver_ihl, ip_tos, ip_len, ip_id, ip_frag, ip_ttl, ip_proto, ip_check, ip_srcadd, ip_dstadd)


###########
##Message##
###########

# Your custom protocol fields or data. We are going to just insert data here. Add your message where the "?" is. Ensure you obfuscate it though...don't want any clear text messages being spotted! You can encode with various data encoding. Base64, binascii

message = b'last_name'                  #This should be the student's last name per the prompt
hidden_msg = binascii.hexlify(message)  #Students can choose which encoding they want to use.


#################
## Send Packet ##
#################

# final packet creation
packet = ip_header + hidden_msg

# Send the packet. Sendto is used when we do not already have a socket connected. Sendall or send if we do.
s.sendto(packet, (dst_ip, 0))

# socket.send is a low-level method and basically just the C/syscall method send(3) / send(2). It can send less bytes than you requested, but returns the number of bytes sent.

# socket.sendall is a high-level Python-only method that sends the entire buffer you pass or throws an exception. It does that by calling socket.send until everything has been sent or an error occurs.
