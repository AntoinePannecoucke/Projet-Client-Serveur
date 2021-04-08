#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "Socket.h"

#define DECK_SIZE 8

/**
 * @author Antoine Pannecoucke
 * */
class Player
{
protected:
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
    void reset();
    void useGeneral();
    void useSpy();
    void sendError(int);
    virtual void playCard(int);

    /**
     * Set the card
     * @param int card
     * @return void
     * */
    void setCurrentCard(int card){ this->current_card = card; }

    /**
     * Return card
     * @return int
     * */
    int getCurrentCard(){ return this->current_card; }

    /**
     * Return general
     * @return bool
     * */
    bool getGeneral(){ return this->general; }

    /**
     * Return spy
     * @return bool
     * */
    bool getSpy(){ return this->spy; }

    /**
     * Add points to player's score
     * @param int score
     * @return void
     * */
    void addPoints(int score){ this->points += score; }
};

#endif // __PLAYER_H__