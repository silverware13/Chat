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
bool setup_connection(char *argv[], char *handle, int port_num);
void chat(int socketFD, char *handle);

int main(int argc, char *argv[])
{
	//make sure the user passed valid arguments
	if(!check_args(argc, argv)){
		return 0;
	}

	//set variables	
	int port_num = strtol(argv[2], NULL, 10);
	char *handle;
	handle = (char *)calloc(MAX_CHARS_HANDLE + 1, sizeof(char));

	//get a handle for the user
	get_handle(handle);

	//setup the connection with the server
	if(!setup_connection(argv, handle, port_num)){
		fprintf(stderr, "Connection failed.\n");
		return 1;
	}
	
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
	printf("Your handle is: %s\n", handle);	
}

bool setup_connection(char *argv[], char *handle, int port_num)
{
	//setup variables
	int socketFD;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	
	//set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); //clear out the address struct
	serverAddress.sin_family = AF_INET; //create a network-capable socket
	serverAddress.sin_port = htons(port_num); //store the port number
	serverHostInfo = gethostbyname(argv[1]); //convert the machine name into a special form of address
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
		return false;
	}

	//start chatting with the host
	chat(socketFD, handle);
	
	return true;
}

void chat(int socketFD, char *handle)
{
	char buffer[MAX_CHARS_MESSAGE + MAX_CHARS_HANDLE + 2];
	char message[MAX_CHARS_MESSAGE + 1];
	memset(buffer, '\0', MAX_CHARS_MESSAGE + MAX_CHARS_HANDLE + 2);
	memset(message, '\0', MAX_CHARS_MESSAGE + 1);
	printf("Starting chat.\n");
	snprintf(message, sizeof(message), "hello everyone");
	snprintf(buffer, sizeof(buffer), "%s> %s", handle, message);
	printf("This is the message sent: %s\n", buffer);
	while(true){
		send(socketFD, buffer, strlen(buffer), 0); //write to the server
		recv(socketFD, &buffer[0], 100, 0); //read the client's message from the socket
		printf("%s\n", buffer); 
		sleep(1);
	}
}
