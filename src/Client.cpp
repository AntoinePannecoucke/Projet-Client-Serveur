#include "Socket.h"
#include <iostream>

int main(){
    stdsock::TransportSocket transport;
    transport.connect("127.0.0.1") ;
    std::string str = transport.receive();
    std::cout << "Client receive :" << str << "\n" << std::endl;
}