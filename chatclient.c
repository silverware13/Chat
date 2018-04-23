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
void setup_connection();
void chat();

int main(int argc, char *argv[])
{
	//make sure the user passed valid arguments
	if(!check_args(argc, argv)){
		return 0;
	}

	//set variables	
	int port_num = strtol(argv[2], NULL, 10);
	char *handle = handle;
	handle = (char *)malloc(MAX_CHARS_HANDLE * sizeof(char));

	//get a handle for the user
	get_handle(handle);
	printf("This is your handle: %s\n", handle);	

	//setup the connection with the server
	setup_connection();

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

void setup_connection()
{
		
}

void chat()
{

}