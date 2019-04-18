#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include "network_table.h" //defines structure for each row of the table

// my define's
#define MAX_NEIGHBORS 4 //relatively small numer. like smaer than 7
#define NODES_IN_TOPOLOGIE 5 //can be really big number

// global varibles
int neighbor_count = 0;  //only modified by init
int entries = 0;  //only modified by init
int myIP = 0;  //only modified by init
struct rute ruteTbl[NODES_IN_TOPOLOGIE];

//main functions declaration
void init();
void server();
void client();
void read_commands();
//helper functions declaration
void print_head();
void print_table();
int fix_hexa(unsigned int readed);
void trim_null_char(int length, char *line);
FILE *my_fopen(const char *__restrict __filename, const char *__restrict __modes);

//main
int main(int argc, char *argv[])
{
    //read input file
    init();

	//pthread_create( /* content */, servee); //TODO: server  //tread to listen

	if (neighbor_count > 1)
	{
		for(int i=0; i<neighbor_count; i++)
		{
			//pthread_create( /* content */); //TODO: client  //  conect to server  //  if table is updated: share the changes
		}
	}
	read_commands(); //TODO: terminal    // console-like to input commands: "show_table" "exit" "snd_msg" "help" "change_link_cost"
	
	return 0;
}

void server()
{
	printf ("this is server\n"); //control

	//when x receives new DV estimate from neighbor, it updates its own DV using B-F equation:
	//Dx(y) ← minv{c(x,v) + Dv(y)}  for each node y ∊ N
}

void client(){
	printf ("this is client\n"); //control

	//when x receives new DV estimate from neighbor, it updates its own DV using B-F equation:
	//Dx(y) ← minv{c(x,v) + Dv(y)}  for each node y ∊ N
}

void read_commands(){
	char *buffer;
	int ret, len;
	size_t getline_len;

	print_head();

	while (1)
	{
		buffer = NULL;
		printf("-$ ");

		// read user imput into the buffer
		ret = getline(&buffer, &getline_len, stdin);
		if (ret == -1) { // EOF
			perror("getline");
			break;
		}
		len = strlen(buffer);
		trim_null_char(ret, buffer);
		//printf("input: '%s'\n", buffer); //control

		if (len == 0)
			continue;
		else if( strcmp(buffer, "exit")==0)
			break;
			//printf("exiting\n"); //control
		else if( strcmp(buffer, "table")==0)
			print_table();

		free(buffer);
	}

	exit(EXIT_SUCCESS);
}

void print_table(){
	printf ("  myIP: %#X\n", myIP); //control
	for(int i=0; i<neighbor_count; i++)
	{
		printf(
			"  %d: %#X %#X %d\n",
			i,
			ruteTbl[i].ip_addrs,
			ruteTbl[i].nxt_jump,
			ruteTbl[i].rute_cost
			);
	}
}

void print_head(){
	printf("enter your commands:\n");
}

void init()
{
	const static char delim[2] = "~";
	FILE *temp;
    char *line = NULL;
	int ret;
    size_t len = 0;
	char *token;
	int ipadres = -1;

	memset(ruteTbl, 0, sizeof(ruteTbl));

	//open file input
	temp = my_fopen("input.txt", "r"); 

	// my ip adress
	ret = getline(&line, &len, temp); // first line //TODO: free line after getline?
	//printf ("myIP: %s", line); //control
    trim_null_char(ret, line);
	inet_pton(AF_INET, line, &myIP);
	myIP = fix_hexa(myIP);

	// for each line(neigbord):
	while ((ret = getline(&line, &len, temp)) != -1)
	{
		if (neighbor_count >= MAX_NEIGHBORS){
			printf ("ERROR: maximum number of neighbors reaached");
			exit(EXIT_FAILURE);
		}

		//trim unecesary characters at the end
        trim_null_char(ret, line);

		// neighbor IP-adrss
        token = strtok(line, delim);
			//printf ("%s ~ ", token); //control
			inet_pton(AF_INET, token, &ipadres);
			ipadres = fix_hexa(ipadres);
			ruteTbl[neighbor_count].ip_addrs = ipadres;
			ruteTbl[neighbor_count].nxt_jump = ipadres;

		// 2nd token // cost
		token = strtok(NULL, delim);
			//printf ("%s\n", token); //control
			ruteTbl[neighbor_count].rute_cost = atoi(token);

		neighbor_count++;
	}

    fclose(temp);
}

int fix_hexa(unsigned int readed)
{
    unsigned int temp = 0;

    temp += (readed & 0xff) << 24;
    temp += (readed & 0xff00) << 8;
    temp += (readed & 0xff0000) >> 8;
    temp += (readed & 0xff000000) >> 24;

    return temp;
}

void trim_null_char(int length, char *line){
	length--;

	while (length >= 0)
	{
		if (line[length] > ' ')
			break;
		line[length] = '\0';
		length--;
	}
}

FILE *my_fopen(const char *__restrict __filename, const char *__restrict __modes)
{
    FILE *temp = fopen(__filename, __modes);

    if (temp == NULL) // error opening the file
    {
        perror(__filename);
        exit(EXIT_FAILURE);
    }

    return temp;
}