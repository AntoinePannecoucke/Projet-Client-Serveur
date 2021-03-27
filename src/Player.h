#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Socket.h"


class Player
{
private:
    /* data */
    stdsock::TransportSocket socket;
    int deck[8];
    int rounds_win;
public:
    Player(stdsock::TransportSocket);
    ~Player();
    std::string recv();
    void send(std::string);
    void sendDeck();
};

#endif // __PLAYER_H__