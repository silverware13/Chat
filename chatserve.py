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
# Lecture 15: Socket Programming Primer 

from socket import *
import sys

if (len(sys.argv) < 2 or len(sys.argv) > 2):
	print "useage: " + str(sys.argv[0]) + " [port number]"
	sys.exit(0)
serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print "The server is ready to receive."
while 1:
	connectionSocket, addr = serverSocket.accept()

	while 1:
		clientMessage = connectionSocket.recv(1024)
		clientMessage = clientMessage.rstrip()
		print clientMessage
		serverMessage = raw_input("server> ")
		connectionSocket.send("server> " + serverMessage + '\n')
		#connectionSocket.close()
