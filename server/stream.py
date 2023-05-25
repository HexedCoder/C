#!/usr/bin/python3

import socket
# create a socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
# s = socket.socket() this is the same as above because we are using all defaults.

# use quotes ' ' to convert into a string
ip_addr = ''
port =

# connect is use to tell it to connect to the ((address, port))
# no quotes needed because its already a integer value
s.connect((ip_addr, port))


# send a string to the target - prefix b'message' to make a bytes-like object.
# Python3 does not like strings sent by itself
message = b""


# = \n is for inline or send carriage return.
# Send the message to the target. The socket must be connected to a remote socket.
s.send(message)

# s = socket variable
# send = method
# (message) = the message variable

# Receive data from the socket.
# Split the return message into two variables. The return value is a pair (bytes(data), address(ip and port))
data, conn = s.recvfrom(1024)

# You can optionally use s.recv but it will only return (bytes)

# Default encoding is 'utf-8'. errors may be given to set a different error handling scheme.
# The default for errors is 'strict', meaning that encoding errors raise a UnicodeError.
# print the response from the receiver. UTF-8 is default. Others are UTF-16, UTF-32, ASCII, EBCIDIC
print(data.decode('utf-8'))

# close = function to close connection else it will hang
s.close()
