#include "Socket.h"
#include <iostream>

int main(){
    stdsock::TransportSocket transport;
    transport.connect("127.0.0.1") ;

    transport.send(HELLO);

    std::string str = transport.receive();

    transport.send(RECEIVE);
    /*
    if (!str.compare(WAIT)){
        transport.send(RECEIVE);
        transport.receive();
    }
    else {
        std::cout << "Choose a card" << std::endl;
        std::string card_choose;
        std::cin >> card_choose;
        std::string card = CARD;
        card.append(" ");
        card.append(card_choose);
        transport.send(card);
    }
    */
    

}