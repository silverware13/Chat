# chatserve
# CS372 Spring 2018
# -----------------
# Name: Zachary Thomas
# Email: thomasza@oregonstate.edu
# Date: 4/22/2018
# ------------------------------------
# Awaits connection from chatclient, send
# and receive messages up to 500 characters long.
# ------------------------------------
# Cited references:
# Lecture 15: Socket Programming Primer 

from socket import *
import sys

# start_up
# -------
# setup the server, get a handle, listen for connections.
# when a connection is made accept and receive messages
# until one side quits. When that happens resume listening.
def start_up():
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
	while True:
		print "The server is waiting to receive connection."
		connectionSocket, addr = serverSocket.accept()

		#we made a connection. keep sending messages to and from client
		#until the client or server quits
		while True:
			#receive message from client
			if not receive_message(connectionSocket):
				break
			#send message to client
			if not send_message(handle, connectionSocket):
				break

# receive_message
# ------------
# get a message from the client.
# If the client quit we return to listening
# for a new connection.
#
# connectionSocket: The current socket being used.
#
# return: Returns true if received a message. Returns false if the client quit.
def receive_message(connectionSocket):
	#get message from client and remove newline
	clientMessage = connectionSocket.recv(1024)
	clientMessage = clientMessage.rstrip()

	#if client quit stop here
	if(clientMessage == "\quit"):
		connectionSocket.close()
		return False
	
	#print clients message	
	print clientMessage
	return True

# send_message
# ------------
# send a message to the client or quit.
# If we quit let the client know.
#
# handle: The handle the user selected previously.
# connectionSocket: The current socket being used.
#
# return: Returns true if we sent a message. Returns false if we quit.
def send_message(handle, connectionSocket):
	#get message from user
	serverMessage = raw_input(handle + "> ")
		
	#if we typed /quit instead of a message let client know
	if(serverMessage == "\quit"):
		connectionSocket.send(serverMessage + '\n')
		connectionSocket.close()
		return False

	#send message to client
	connectionSocket.send(handle + "> " + serverMessage[:500] + '\n')
	return True

def main():
	start_up()

if __name__ == "__main__":
	main()
