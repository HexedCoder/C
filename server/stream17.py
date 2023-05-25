#!/usr/bin/python3

import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
ip_addr = '192.168.240.182'
port = 9002

s.connect((ip_addr, port))

message = b"-=EVIL=-"


s.send(message)

data, conn = s.recvfrom(1024)
print(data.decode('utf-8'))

s.close()
