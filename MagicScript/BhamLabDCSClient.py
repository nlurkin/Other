#!/bin/env python
# encoding: utf-8

import socket
import sys

HOST, PORT = "eplxp001", 9999
data = " ".join(sys.argv[1:])

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.settimeout(30)

try:
    # Connect to server and send data
    sock.connect((HOST, PORT))
    sock.sendall(data + "\n")

    # Receive data from the server and shut down
    while True:
    	try:
    		data = sock.recv(1024, 0)
    		print data,
    		if not data: break
    	except IOError:
    		break
finally:
    sock.close()
