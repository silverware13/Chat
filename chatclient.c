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
void get_handle(char *handle, size_t handle_size);
bool setup_connection(char *argv[], char *handle, size_t handle_size, int port_num);
void chat(int socketFD, char *handle, size_t handle_size);

int main(int argc, char *argv[])
{
	//make sure the user passed valid arguments
	if(!check_args(argc, argv)){
		return 0;
	}

	//set variables	
	int port_num = strtol(argv[2], NULL, 10);
	char *handle;
	size_t handle_size = MAX_CHARS_HANDLE + 1;
	handle = (char *)calloc(MAX_CHARS_HANDLE + 1, sizeof(char));

	//get a handle for the user
	get_handle(handle, handle_size);

	//setup the connection with the server and start chat
	if(!setup_connection(argv, handle, handle_size, port_num)){
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

void get_handle(char *handle, size_t handle_size)
{
	printf("Please enter a handle, it must be no longer than %d characters.\nHandle: ", MAX_CHARS_HANDLE);
	fflush(stdout); //make sure we printed the prompt
	fgets(handle, handle_size, stdin);

	//remove newline from handle, if there is none clear stdin until we find one
	if(handle[strlen(handle)-1] == '\n'){
		strtok(handle, "\n"); //remove newline
	} else {
		char c;
		while((c = getchar()) != '\n' && c != EOF); //clear stdin
	}

	//show the handle
	printf("Your handle is: %s\n", handle);	
}

bool setup_connection(char *argv[], char *handle, size_t handle_size, int port_num)
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
	chat(socketFD, handle, handle_size);
	
	return true;
}

void chat(int socketFD, char *handle, size_t handle_size)
{
	//setup variables
	int chars_written, chars_read;
	char buffer[MAX_CHARS_MESSAGE + MAX_CHARS_HANDLE + 4];
	char message[MAX_CHARS_MESSAGE + 1];
	memset(buffer, '\0', MAX_CHARS_MESSAGE + MAX_CHARS_HANDLE + 4);
	memset(message, '\0', MAX_CHARS_MESSAGE + 1);

	while(true){
		//get message from user
		printf("%s> ", handle); //print a prompt for the user
		fflush(stdout); //make sure we printed the prompt
		fgets(message, sizeof(message), stdin); //get a message from the user
		
		//remove newline from message, if there is none clear stdin until we find one
		if(message[strlen(message)-1] == '\n'){
			strtok(message, "\n"); //remove newline
		} else {
			char c;
			while((c = getchar()) != '\n' && c != EOF); //clear stdin
		}

		//see if we are quiting. if yes let server know
		if(!strcmp(message, "/quit")){
			send(socketFD, "/quit\n", strlen("/quit\n"), 0);	
			exit(0);
		}

		snprintf(buffer, sizeof(buffer), "%s> %s\n", handle, message); //add our handle to the message

		//send message to server
		chars_written = 0;
		do{
			chars_written += send(socketFD, buffer, strlen(buffer), 0); //write to socket
			if(chars_written < 0){
				fprintf(stderr, "Error writing to socket.\n");
				exit(2); 
			}
		} while(chars_written < strlen(buffer));
		
		//read message from server
		chars_read = 0;
		int bufLen; //holds the buffer length
		int bufSum = 0; //the number of chars we have writen to our buffer
		memset(buffer, '\0', MAX_CHARS_MESSAGE + MAX_CHARS_HANDLE + 3);
		do{
			chars_read = recv(socketFD, &buffer[bufSum], 100, 0); //read from socket
			bufSum += chars_read;
			bufLen = strlen(buffer);
			if(chars_read < 0){
				fprintf(stderr, "Error reading from socket.\n");
				exit(2); 
			}
		} while(buffer[bufLen - 1] != '\n');

		//make sure server is not quiting
		if(!strcmp(buffer, "/quit\n")){
			exit(0);
		}
	
		//show message from server
		printf("%s", buffer); 
	}

}
