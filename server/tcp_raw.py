#!/usr/bin/python3

# For building the socket
import socket

# For system level commands
import sys

# For establishing the packet structure (Used later on), this will allow direct access to the methods and functions in the struct module
from struct import *
import array

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
ip_tos = 0                             # This combines the DSCP and ECN fields. Type of service/QoS
ip_len = 0                              # The kernel will fill in the actually length of the packet
ip_id = 2020                            # This sets the IP Identification for the packet
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

####################
##Build TCP Header##
####################

tcp_src = 54321                         # source port
tcp_dst = 1234                          # destination port
tcp_seq = 90210                         # sequence number
tcp_ack_seq = 30905                     # tcp ack sequence number
tcp_data_off = 5                        # data offset specifying the size of tcp header * 4 which is 20
tcp_reserve = 0                         # the 3 reserve bits + ns flag in reserve field
tcp_flags = 0                           # tcp flags field before the bits are turned on
tcp_win = 65535                         # maximum allowed window size reordered to network order (socket.htons is deprecated)
tcp_chk = 0                             # tcp checksum which will be calculated later on
tcp_urg_ptr = 0                         # urgent pointer only if urg flag is set

# Combine the left shifted 4 bit tcp offset and the reserve field
tcp_off_res = (tcp_data_off << 4) + tcp_reserve

# Tcp flags by bit starting from right to left
tcp_fin = 0                             # Finished
tcp_syn = 1                             # Synchronization
tcp_rst = 0                             # Reset
tcp_psh = 0                             # Push
tcp_ack = 0                             # Acknowledgment
tcp_urg = 0                             # Urgent
tcp_ece = 0                             # Explicit Congestion Notification Echo
tcp_cwr = 0                             # Congestion Window Reduced

# Combine the tcp flags by left shifting the bit locations and adding the bits together
tcp_flags = tcp_fin + (tcp_syn << 1) + (tcp_rst << 2) + (tcp_psh << 3) + (tcp_ack << 4) + (tcp_urg << 5) + (tcp_ece << 6) + (tcp_cwr << 7)


# The ! in the pack format string means network order
tcp_hdr = pack('!HHLLBBHHH', tcp_src, tcp_dst, tcp_seq, tcp_ack_seq, tcp_off_res, tcp_flags, tcp_win, tcp_chk, tcp_urg_ptr)


###########
##Message##
###########

# Your custom protocol fields or data. We are going to just insert data here. Add your message where the "?" is. Ensure you obfuscate it though...don't want any clear text messages being spotted! You can encode with various data encoding. Base64, binascii

message = b'last_name'                  #This should be the student's last name per the prompt


############################
##Create the Pseudo Header##
############################

# After you create the tcp header, create the pseudo header for the tcp checksum.

src_address = socket.inet_aton(src_ip)
dst_address = socket.inet_aton(dst_ip)
reserved = 0
protocol = socket.IPPROTO_TCP
tcp_length = len(tcp_hdr) + len(message)


##########################
##Pack the Pseudo Header##
##########################

ps_hdr = pack('!4s4sBBH', src_address, dst_address, reserved, protocol, tcp_length)
ps_hdr = ps_hdr + tcp_hdr + message


################################
##Define the Checksum Function##
################################

def checksum(data):
        if len(data) % 2 != 0:
                data += b'\0'
        res = sum(array.array("H", data))
        res = (res >> 16) + (res & 0xffff)
        res += res >> 16
        return (~res) & 0xffff

tcp_chk = checksum(ps_hdr)

##################
##Final TCP Pack##
##################

# Pack the tcp header to fill in the correct checksum - remember checksum is NOT in network byte order
tcp_hdr = pack('!HHLLBBH', tcp_src, tcp_dst, tcp_seq, tcp_ack_seq, tcp_off_res, tcp_flags, tcp_win) + pack('H', tcp_chk) + pack('!H', tcp_urg_ptr)


#################
## Send Packet ##
#################

# Combine all of the headers and the user data
packet = ip_header + tcp_hdr + message

# Send the packet. Sendto is used when we do not already have a socket connected. Sendall or send if we do.
s.sendto(packet, (dst_ip, 0))

# socket.send is a low-level method and basically just the C/syscall method send(3) / send(2). It can send less bytes than you requested, but returns the number of bytes sent.

# socket.sendall is a high-level Python-only method that sends the entire buffer you pass or throws an exception. It does that by calling socket.send until everything has been sent or an error occurs.
