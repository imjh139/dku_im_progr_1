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

/* Don't try to look up hostname. */
#define NI_NUMERICHOST 1
/* Don't convert port number to name. */
#define NI_NUMERICSERV 2
#define MAX_NEIGHBORS 4

// global varibles
const static char delim[2] = "~";

//functions declaration
static void * handle(void *);
void init();


//main
int main(int argc, char *argv[])
{
    //read input file
    init();

	

	return 0;
}

void init()
{
	//open file input
    FILE *temp = fopen("input.txt", "r"); 
    if (temp == NULL) // error opening the file
    {
        perror("input file");
    	fclose(temp);
        exit(EXIT_FAILURE);
    }
	
    char *line = NULL;
    size_t len = 0;

	// my ip adress
	int ret = getline(&line, &len, temp); // first line
	printf ("myIP: %s", line); //TODO:

	char lineCount = 0;
	while ((ret = getline(&line, &len, temp)) != -1) // for each neigbord
	{
		//trim unecesary characters at the end
        ret--;
        while (ret >= 0)
        {
            if (line[ret] > ' ')
                break;
            line[ret] = '\0';
            ret--;
        }

		char *token;

        //try with first token
        token = strtok(line, delim); // 1st token  // (direct)neighbor
		printf ("%s ~ ", token); //TODO:
		token = strtok(NULL, delim); // 2nd token // cost
		printf ("%s\n", token); //TODO:

        if (++lineCount > MAX_NEIGHBORS){
			printf ("ERROR reading input: maximum number of neighbors reaached");
        	exit(EXIT_FAILURE);
		}
	}

    fclose(temp);
}