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

//function prototype(s)
void get_handle();
void setup_connection();
void chat();

int main(int argc, char *argv[])
{
	//make sure the user entered correct number of arguments.
	if(argc < 3){
		printf("useage: %s [host name] [port number]\n", argv[0]);
		return 0;
	}

	//make sure the user has entered the port number as digits
	if(!isdigit(*argv[2])){
		printf("Please enter port number as an unsigned integer.\n");
		return 0;
	}
	
	int port_num = strtol(argv[2], NULL, 10);

	char *handle = handle;
	handle = (char *)malloc(MAX_CHARS_HANDLE * sizeof(char));

	//get a handle for the user
	get_handle(handle);
	printf("This is your handle: %s\n", handle);	

	//setup the connection to server
	setup_connection();

	//start chatting with the host
	chat();
	
	return 0;
}

void get_handle(char *handle)
{
	while(true){
		printf("Please enter a handle, it must be no longer than %d characters.\nHandle: ", MAX_CHARS_HANDLE);
		scanf("%s", handle, 10);
		return;	
	}
}

void setup_connection()
{

}

void chat()
{

}
