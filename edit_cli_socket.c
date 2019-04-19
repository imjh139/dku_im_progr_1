#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

char *buffer;
char read_buffer[1024];

#define LENGTH 512
struct neighbour {
    char * IP;
    int cost;
};
struct neighbour neighbours[5];
static char newAddress[19];

struct neighbour decodeNeighbourFromDelimiter(char * values) {
	struct neighbour read;
	char val[19];
	strcpy(val, values);
	strcpy(newAddress, strtok(val, "~"));
	int cost = atoi(strtok(0, "\n"));
	//strcpy(read.IP, address);
	read.IP = newAddress;
    read.cost = cost;
	return read;
}

void neighbourFileUpdate() {
	fp = fopen("neighbour.txt", "w");
	if (fp != NULL) {
		for (int i = 0; i <= sizeof(neighbours) / sizeof(struct neighbour); i++) {
			if (neighbours[i].IP != NULL) {
				printf("Encoding Neighbour %s~%d\n", neighbours[i].IP, neighbours[i].cost);
				if (i > 0) fprintf(fp, "\n");
				fprintf(fp, "%s%c%d", neighbours[i].IP, '~', neighbours[i].cost);
			}
		}
	}
	else printf("Error Neighbours File Not Found!");
	fclose(fp);
}


int main(int argc, char *argv[])
{
	int fd_sock;
	int port_num, ret, len;
	struct sockaddr_in addr;
	size_t getline_len;

	// arg parsing
	if (argc != 3) {
		printf("usage: cli srv_ip_addr port\n");
		return 0;
	}
	port_num = atoi(argv[2]);

	// socket creation
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_sock == -1) { // socket CREATE error
		perror("socket");
		return 0;
	}

	// addr binding
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_num); // using port num (argv)
	inet_pton(AF_INET, argv[1], &addr.sin_addr); // using IPadrss (argv)
	
	//connect
	ret = connect(fd_sock, (struct sockaddr *)&addr, sizeof(addr));  //expecting an accept()
	if (ret == -1) { //connection error
		perror("connect");
		close(fd_sock);
		return 0;
	}

	//get table
	// FILE * fp; = fopen("neighbour.txt", "r");
	// if (fp != NULL)
	// {
	// 	char line[128];
	// 	int i = 0;

	// 	while (fgets(line, 128, fp) != NULL)
	// 	{
	// 		if (line != "") {
	// 			neighbours[i] = decodeNeighbourFromDelimiter(line);
	// 			i++;
	// 		}
	// 	}
	// 	
	// }
	// fclose(fp);
	// else printf("Error Neighbours File Not Found!");
	// neighbourFileUpdate();




	//send file to server

		char sdbuf[512];
		FILE *fs = fopen("neighbour.txt", "r");
		if(fs == NULL)
		{
			printf("ERROR: File not found.\n");
		}
		bzero(sdbuf, 512); 
		int fs_block_sz;
		while((fs_block_sz = fread(sdbuf, sizeof(char), 512, fs)) > 0)
		{
		    if(send(fd_sock, sdbuf, fs_block_sz, 0) < 0)
		    {
		    	printf("FAIL");
		    	break;
		    }
		    bzero(sdbuf, 512);
		}
		printf("Ok File %s from Client was Sent!\n", "neighbour.txt");

	//if table changed
	//add new neighbour
	//neighbourFileUpdate();
	//or routing table file update
	//send file to server

	while (1) // { getline() -> send() -> recv() }
	{
		buffer = NULL;
		printf("send$ ");

		// read user imput into the buffer
		ret = getline(&buffer, &getline_len, stdin);
		if (ret == -1) { // EOF
			perror("getline");
			close(fd_sock);
			break;
		}
		len = strlen(buffer);
		if(buffer[len-1] == '\n')  //remove '\n' at the end
		{
			buffer[len-1] = '\0';
			len--;
		}
		if (len == 0) {
			free(buffer);
			continue;
		}

		// send the buffer
		send(fd_sock, buffer, len, 0);
		free(buffer);

		// read echo
		memset(read_buffer, 0, sizeof(read_buffer));
		len = recv(fd_sock, read_buffer, sizeof(read_buffer), 0);
		if (len < 0)
			break;
		printf("server responded: %s\n", read_buffer);
		fflush(NULL);
	}

	// bye-bye
	close(fd_sock);
	return 0;
}
