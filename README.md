# dku_im_progr_1
deadline 24

compile server_socket.c:
gcc srv_socket.c -o srv -pthread

pc1-pc5 : 220.149.244.211-215 :63000

----------------------- class 2 -----------------------

    what is bellmanford equation?

check: 04socket_programing.ppt
github.com/seehwan/simple_socket  <-  multitread baseline program
    simple_socket
        what is flush();?

socket creation  ->  A conection to a/the server (IPadrss:port)

struct hostent *server = gethostbyname(hostname); //adress to IPadrss
if(server == NULL)
{
    print error
    exit
}
bcopy((char *)server->h_addr, (char *)&(serv_addr->sin_addr.s_addr), server->h_length);
serv_adrrs->sin_port = htons(portno);

function calls:
    * write(wfd, wdbuf, wlen);
    * n = read(rfd, rdbuf, rmax); //returns read bytes

//to handle multiple clients
epoll()
select()

use sockets

DO NOT USE 0-1023 PORTS  slide-25

threads use same gloval variables
    dangerous to write on those variables


----------------------- class 3 -----------------------

tcp: rtansport protocol

by recibing ack mesage you are shure what_you_sended arrived
    tcp has redundant feedback to make shure pakages dont get lost