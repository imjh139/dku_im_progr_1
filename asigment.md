# dku_im_progr_1
# Application Routing Protocol Simulation
Make your own application-level routing protocol

assumed that you know Dijkstra and/or Bellman-Ford routing algorithm

use multiple pcs (or ports on the same pc)
    they assume virtual links and distances
    define your own metric

Primary goals:
* Complete the routing table
    *  Each application simulates a router
        * Maintains routing table
        * Forward data packet according to the routing table whenever a packet is arrived
    * Start-up
        * Each application begins with empty routing table
        * You may assume that a router knows its direct neighbor nodes (port)
    * Using Link-State or Distance-Vector algorithm, each application updates its routing table
        * Routing is completed when there’s no further updates
* Send-recv data over a given path  (implement forwarding engine)
    * A router forward data packet toward the destination
        * Whenever a data packet arrives, it checks the destination address
        * Look up the routing table
        * Send data packet to the next hop toward the destination
    * You should be able to distinguish data packets from routing packets
    * You should define the data packet format
        * It should contain source address and destination address
    * You should make the applications at the end hosts
        * Source node initiates the network session, and begins to send data
        * Destination node accepts the network session, and save the received data packets

Related studies: Resilient Overlay Networks
* Academic research paper by 
    * David Andersen, Hari Balakrishnan, Frans Kaashoek, and Robert Morris. 2001. Resilient overlay networks. In Proceedings of the eighteenth ACM symposium on Operating systems principles (SOSP '01). ACM, New York, NY, USA, 131-145. [paper] [slides]
* Network over the Internet
    * Goals:
        * Fast failure detection & recovery
        * Tighter application integration
        * Policy-based routing
    * Design
        * Routing & Path selection
        * Policy routing
        * Data forwarding
        * Bootstrap & membership mgmt.

Chatting app: Practice
* Server
    * Listen on the socket, accept connection
    * Recv from socket
    * Send it back
* Client
    * Connect to the server
    * Getline from stdin
    * Send the input string to the server
    * Recv from the server

Chatting app: real deal:
* Server
    * Listen on the socket, accept connection
    * Fork after accept
    * child:
        * Recv from socket
        * Send it back
* Client
    * Connect to the server
    * Getline from stdin
    * Send the input string to the server
    * Recv from the server

Echo into chatting:
* Make server send input string to all the clients
    * fork(): makes a new process
    * How to send data to socket to the clients?
        * Socket is owned by another process
    * Use either 1) IPC, 2) threads or 3) I/O multiplexing

Use threads:
* All data are shared
* But, be careful (synchronization) for global data
* Easy, but limited to scale

I/O multiplexing:
* While you’re recving, you cannot send data
    * recv() function is blocked
    * How to recv from multiple clients from a single thread?
        * select() system call
        * OS tells you which socket is ready to recv
        * FD_SET, FD_ISSET, FD_CLR

You may think about further extensions
* chat rooms
* nicknames

Consider for routing protocol:
* Takes route information from another computer
    * Who is the server and who is the client?
    * All are servers, all are clients
    * Routing application runs in background
* Client initiation
    * If one knows the server’s target IP and destination port number, you can connect
* Server acceptance
    * Take all incoming connection requests,
    * Exchange route information

