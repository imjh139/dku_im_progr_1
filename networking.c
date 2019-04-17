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
#include <network_table.h> //defines structure for each row of the table

// my define's
#define MAX_NEIGHBORS 4 //relatively small numer. like smaer than 7
#define NODES_IN_TOPOLOGIE 5 //can be really big number

// global varibles
//int itself = 1;
int neighbor_count = 0;  //only modified by init
int entries = 0;  //only modified by init
struct rute ruteTbl[NODES_IN_TOPOLOGIE];

//functions declaration
void init();
FILE *my_fopen(const char *__restrict __filename, const char *__restrict __modes);
void trim_null_char(int length, char *line);

//main
int main(int argc, char *argv[])
{
    //read input file
    init();

	pthread_create( /* content */); //TODO: server  //tread to listen

	if (neighbor_count > 1)
		forech(rute in ruteTbl) 
			pthread_create( /* content */); //TODO: client  //  conect to server  //  if table is updated: share the changes

	read_commands(); //TODO: terminal    // console-like to input commands: "show_table" "exit" "snd_msg" "help" "change_link_cost"
	
	return 0;
}


void server(){
	//when x receives new DV estimate from neighbor, it updates its own DV using B-F equation:
	//Dx(y) ← minv{c(x,v) + Dv(y)}  for each node y ∊ N
}

void init()
{
	const static char delim[2] = "~";
	FILE *temp;
    char *line = NULL;
	int ret;
    size_t len = 0;
	char *token;

	//open file input
	temp = my_fopen("input.txt", "r"); 

	// my ip adress
	ret = getline(&line, &len, temp); // first line //TODO: free line after getline?
	printf ("myIP: %s", line); //TODO:

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
			printf ("%s ~ ", token); //control
			int ipadres = foo(token); //TODO: change foo() for a function that works
			table[neighbor_count].ip_addrs = ipadres;
			table[neighbor_count].nxt_jump = ipadres;

		// 2nd token // cost
		token = strtok(NULL, delim);
			printf ("%s\n", token); //control
			table[neighbor_count].rute_cost = strtoi(token); //TODO: change strtoi() for a string-to-int()

		neighbor_count++;
	}

    fclose(temp);
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