#include "Socket.h"
#include <iostream>

stdsock::Socket::~Socket(){
    ::close(sock_fd);
}

stdsock::Socket::Socket(){
    int yes = 1;
    if ((sock_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0){
        ::puts("Error socket creation");
        exit(-1);
    }
    ::setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(MYPORT);
	addr.sin_addr.s_addr = INADDR_ANY;
	::bzero(&(addr.sin_zero), 8);
}

void stdsock::Socket::setSock(unsigned int sock){
    sock_fd = sock;
}

unsigned int stdsock::ConnectionSocket::accept(){
    unsigned int sin_size = sizeof(struct sockaddr_in);
    unsigned int tmp_sock = ::accept(sock_fd, (struct sockaddr *) &addr, &sin_size);
    return tmp_sock;
}

int stdsock::ConnectionSocket::bind(){
    return ::bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr));
}

int stdsock::ConnectionSocket::listen(){
    return ::listen(sock_fd, BACKLOG);
}

std::string stdsock::TransportSocket::receive(){
    char buff[MAXDATASIZE];
    ::bzero(buff, MAXDATASIZE);
    ::recv(sock_fd, buff, sizeof(buff), 0);
    std::string str(buff);
    return str;
}

int stdsock::TransportSocket::send(std::string msg){
    
    if (::send(sock_fd, msg.c_str(), strlen(msg.c_str()), 0) <= 0)
        puts("send erreur");
    else{
        char message[msg.size() + 1];
        msg.copy(message, msg.size() + 1);
    }
        
    return 0;
}

unsigned int stdsock::Socket::getSockFd(){
    return sock_fd;
}

void stdsock::ConnectionSocket::openConnection(){
    if (stdsock::ConnectionSocket::bind() == -1)
        puts("erreur bind");
    if (stdsock::ConnectionSocket::listen()==-1)
        puts("erreur listen");
}

void stdsock::TransportSocket::connect(std::string ip_address){
    char buff[ip_address.size() + 1];
    strcpy(buff, ip_address.c_str());
    inet_pton(AF_INET, buff, &addr.sin_addr);
    unsigned int sin_size = sizeof(struct sockaddr_in);
    ::connect(sock_fd, (struct sockaddr *) &addr, sin_size);
}