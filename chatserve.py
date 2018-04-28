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
serverPort = 12556
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print "The server is ready to receive."
while 1:
	connectionSocket, addr = serverSocket.accept()

	sentence = connectionSocket.recv(1024)
	capitalizedSentence = sentence.upper()
	connectionSocket.send(capitalizedSentence)
	connectionSocket.close()
