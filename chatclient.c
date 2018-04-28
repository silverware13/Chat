/* chatclient
 * CS372 Spring 2018
 * -----------------
 * Name: Zachary Thomas
 * Email: thomasza@oregonstate.edu
 * Date: 4/22/2018
 * -----------------
 * Connects to chatserve, is able to send
 * messages up to 500 characters long. 
 */

#define MAX_CHARS_MESSAGE 500
#define MAX_CHARS_HANDLE 10
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>

//function prototype(s)
bool check_args(int argc, char *argv[]);
void get_handle(char *handle);
bool setup_connection(char *argv[], char buffer[], int port_num);
void chat();

int main(int argc, char *argv[])
{
	//make sure the user passed valid arguments
	if(!check_args(argc, argv)){
		return 0;
	}

	//set variables	
	int port_num = strtol(argv[2], NULL, 10);
	char handle[MAX_CHARS_HANDLE + 1];
	char buffer[MAX_CHARS_MESSAGE + 1];
	memset(handle, 0, MAX_CHARS_HANDLE + 1);
	memset(buffer, 0, MAX_CHARS_MESSAGE + 1);

	//get a handle for the user
	get_handle(handle);
	printf("This is your handle: %s\n", handle);	

	//setup the connection with the server
	if(!setup_connection(argv, buffer, port_num)){
		fprintf(stderr, "Connection failed.\n");
		return 1;
	} 

	//start chatting with the host
	chat();
	
	return 0;
}

bool check_args(int argc, char *argv[])
{
	//make sure the user entered correct number of arguments.
	if(argc < 3){
		printf("useage: %s [host name] [port number]\n", argv[0]);
		return false;
	}

	//make sure the user has entered the port number as digits
	if(!isdigit(*argv[2])){
		printf("Please enter port number as an unsigned integer.\n");
		return false;
	}
	
	return true;
}

void get_handle(char *handle)
{
	printf("Please enter a handle, it must be no longer than %d characters.\nHandle: ", MAX_CHARS_HANDLE);
	scanf("%10s", handle, 10);
}

bool setup_connection(char *argv[], char buffer[], int port_num)
{
	//setup variables
	int socketFD, portNumber, charsWritten, charsRead, i, ii;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	
	//set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); //clear out the address struct
	serverAddress.sin_family = AF_INET; //create a network-capable socket
	//serverAddress.sin_port = htons(portNumber); //store the port number
	//serverHostInfo = gethostbyname(argv[2]); //convert the machine name into a special form of address
	serverAddress.sin_port = htons(12556); //store the port number
	serverHostInfo = gethostbyname("localhost"); //convert the machine name into a special form of address
	if (serverHostInfo == NULL){
		return false;
	} 
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); //copy in the address
	
	//set up socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); //create the socket
	if (socketFD < 0){
		return false;
	}

	//connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){ //connect socket to address
		//return false;
	}

	send(socketFD, "hey dude", strlen("hey dude"), 0); //write to the server
	charsRead = recv(socketFD, &buffer[0], 100, 0); //read the client's message from the socket
	printf("%s\n", buffer); 

	return true;
}

void chat()
{
	printf("Starting chat.\n");
	while(true){
		sleep(1);
	}
}
