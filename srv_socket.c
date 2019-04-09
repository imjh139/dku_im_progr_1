#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>

#define NI_DGRAM 16 /* Look up UDP service rather than TCP. */
#define NI_NAMEREQD 8 /* Don't return numeric addresses. */
#define NI_NOFQDN 4 /* Only return nodename portion. */
#define NI_NUMERICHOST 1 /* Don't try to look up hostname. */
#define NI_NUMERICSERV 2 /* Don't convert port number to name. */
#define MAXHOST 25
#define MAXSERV 3

char buffer[1024];
pthread_t thds[100];
int thdc;
// int pid; 	// I dont know what is this for

//functions declaration
static void * handle(void *);

//main
int main(int argc, char *argv[])
{
	int srv_sock, cli_sock;
	int port_num, ret;
	struct sockaddr_in addr;
	int len;

	// arg parsing
	if (argc != 2) {
		printf("usage: srv port\n");
		return 0;
	}
	port_num = atoi(argv[1]);

	// socket creation
	srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_sock == -1) { // socket CREATE error
		perror("Server socket create");
		return 0;
	}

	// addr binding
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY); //listen to all ports // 32bit IPV4 addr that not use static IP addr
	addr.sin_port = htons(port_num); // using port num
	
	ret = bind (srv_sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) { //BIND error
		perror("BIND error!!");
		close(srv_sock);
		return 0;
	}
	
	printf("listening ...\n");
	while (1) { // Listen part

		ret = listen(srv_sock, 0);
		if (ret == -1) { //LISTEN stanby error
			perror("LISTEN stanby mode fail");
			close(srv_sock);
			return 0;
		}

		// Accept a client ( get new socket to communicate)
		cli_sock = accept( srv_sock, NULL, NULL);
		if (cli_sock == -1) { //cli_sock connect error
			perror("cli_sock connect ACCEPT fail");
			close(srv_sock);
		}

		// cli handler
		thdc++;
		pthread_create(&thds[thdc], NULL, handle, &cli_sock);
		
		//it will go back to listenig state
	} // ctrl+c to end
	return 0;
}

static void * handle(void * arg)
{
	int cli_sockfd = *(int *)arg;

	int ret = 0; //-1;
	char *recv_buffer = (char *)malloc(1024);
	char *send_buffer = (char *)malloc(1024);
	char host_buf[MAXHOST], serv_buf[MAXSERV];
           
	/* get peer addr */
	struct sockaddr peer_addr;
	socklen_t peer_addr_len;
	memset(&peer_addr, 0, sizeof(peer_addr));
	peer_addr_len = sizeof(peer_addr);
	
	ret = getpeername(cli_sockfd, &peer_addr, &peer_addr_len);
	ret = getnameinfo(
		&peer_addr,
		peer_addr_len, 
		host_buf, sizeof(host_buf),
		serv_buf, sizeof(serv_buf), 
		NI_NUMERICHOST | NI_NUMERICSERV); 

	if (ret != 0) {
		ret = -1;
		pthread_exit(&ret);
	}

	printf("%s conected\n", host_buf);

	/* read from client host:port */
	while (1) {
		int len = 0;

		printf("recibing from client:\n");
		memset(recv_buffer, 0, sizeof(recv_buffer));
		len = recv(cli_sockfd, recv_buffer, sizeof(recv_buffer), 0);
		if (len == 0)
			continue;
		printf("%s\n len:%d\n", recv_buffer, len);
		memset(send_buffer, 0, sizeof(send_buffer));
		sprintf(send_buffer, "[%s:%s]%s len:%d\n", 
					host_buf, serv_buf, recv_buffer, len);
		len = strlen(send_buffer);

		ret = send(cli_sockfd, send_buffer, len, 0);
		if (ret == -1)
			break;
		printf("----\n");
		fflush(NULL);
	}

	close(cli_sockfd);
	ret = 0;
	pthread_exit(&ret);
}

