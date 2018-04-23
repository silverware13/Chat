# chatserve
# CS372 Spring 2018
# -----------------
# Name: Zachary Thomas
# Email: thomasza@oregonstate.edu
# Date: 4/22/2018
# ------------------------------------
# Awaits connection from chatclient, is able to send
# messages up to 500 characters long.
# ------------------------------------
# Cited references:
# Lecture 15: Socket Programming Primer (slide 6) 

from socket import *
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind(('', serverPort))print “The server is ready to receive”
while 1:
	message, clientAddress = serverSocket.recvfrom(2048)
	modifiedMessage = message.upper()
	serverSocket.sendto(modifiedMessage, clientAddress)
