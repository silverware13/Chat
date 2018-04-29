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

#confirm command line arguments include port number
if (len(sys.argv) < 2 or len(sys.argv) > 2):
	print "useage: " + str(sys.argv[0]) + " [port number]"
	sys.exit(0)

#get handle
print "Please enter a handle, it must be no longer than 10 characters."
handle = raw_input("Handle: ")
handle = handle[:10] #make sure handle is not longer than 10 characters
print "Your handle is: " + handle

#setup socket and begin listening 
serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)

#wait to accept incoming connection
while 1:
	print "The server is waiting to receive connection."
	connectionSocket, addr = serverSocket.accept()

	#we made a connection keep sending messages to and from client
	while 1:
		#get message from client and remove newline
		clientMessage = connectionSocket.recv(1024)
		clientMessage = clientMessage.rstrip()

		#if client quit stop here
		if(clientMessage == "\quit"):
			connectionSocket.close()
			break
	
		#print clients message	
		print clientMessage

		#get message from user
		serverMessage = raw_input(handle + "> ")
		
		#if we typed /quit instead of a message let client know
		if(serverMessage == "\quit"):
			connectionSocket.send(serverMessage + '\n')
			connectionSocket.close()
			break

		#send message to client
		connectionSocket.send(handle + "> " + serverMessage[:500] + '\n')

