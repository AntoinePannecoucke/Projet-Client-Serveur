#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Socket.h"
#include "Card.h"

#define DECK_SIZE 8

class Player
{
private:
    /* data */
    stdsock::TransportSocket* socket;
    Card deck[DECK_SIZE];
    int points;
public:
    Player(stdsock::TransportSocket*);
    ~Player();
    std::string recv();
    void send(std::string);
    void sendDeck();
    int getPoints();
    void addPoints(int);
    void sendError(int);
};

#endif // __PLAYER_H__