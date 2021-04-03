#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Socket.h"

#define DECK_SIZE 8

class Player
{
protected:
    /* data */
    stdsock::TransportSocket* socket;
    bool deck[DECK_SIZE];
    int current_card;
    int points;
    bool general, spy;
public:
    Player(stdsock::TransportSocket*);
    virtual ~Player();
    std::string recv();
    void send(std::string);
    void sendDeck();
    int getPoints();
    void addPoints(int score){ this->points += score; }
    void sendError(int);
    virtual void playCard(int);
    void setCurrentCard(int card){ this->current_card = card; }
    int getCurrentCard(){ return this->current_card; }
    bool getGeneral(){ return this->general; }
    bool getSpy(){ return this->spy; }
    void reset();
    void useGeneral();
    void useSpy();
};

#endif // __PLAYER_H__