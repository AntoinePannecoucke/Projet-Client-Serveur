#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#define MYPORT 3490

#define BACKLOG 10

#define MAXDATASIZE 100

/**
 * Protocole Solo
 */

#define HELLO   "helo"
#define MENU    "menu"
#define SOLO    "solo"
#define BEGIN   "bgin"

                        // (boolean pour chaque carte)
#define DECK    "deck"  // [0-1] [0-1] [0-1] [0-1] [0-1] [0-1] [0-1] [0-1]
#define ERROR   "eror"  // [Code Erreur]

                        // (état du round, score player 1, score player 2, carte player 1, carte player 2)
#define ROUND   "tour"  // [0-2] [0-4] [0-4] [0-7] [0-7]

                        // (carte jouée)
#define CARD    "card"  // [1-8]
#define NEXT    "next"
#define QUIT    "quit"
#define RECEIVE "recv"

/**
 * Code Erreur
 */
#define TIME_OUT 408
#define BAD_RESPONSE 409
#define BAD_CARD 400

namespace stdsock {
    

    class Socket {
    protected:
        struct sockaddr_in addr;
        unsigned int sock_fd;
    public:
        Socket();
        virtual ~Socket();
        unsigned int getSockFd();
        void setSock(unsigned int sock);
    };
    

    class ConnectionSocket : public Socket
    {
    private:
        int bind();
        int listen();
    public:
        ConnectionSocket() {};
        ~ConnectionSocket() {};
        
        unsigned int accept();
        void openConnection();
    };
    

    class TransportSocket : public Socket
    {
    public:
        TransportSocket() {};
        ~TransportSocket() {};
        int send(std::string);
        std::string receive();
        void connect(std::string ip_address);
    };
    
}

#endif // __SOCKET_H__