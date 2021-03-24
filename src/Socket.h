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
