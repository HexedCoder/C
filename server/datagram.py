#!/usr/bin/python3

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)

ip_addr = ''
port =
message = b""
s.sendto (message, (ip_addr, port))
data, addr = s.recvfrom(1024)
#Default encoding is 'utf-8'. errors may be given to set a different error handling scheme. The default for errors is 'strict', meaning that encoding errors raise a UnicodeError.
print(data.decode())

# using "utf-8" default
