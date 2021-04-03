#include "Player.h"

Player::Player(stdsock::TransportSocket* sock): socket(sock), points(0), general(false), spy(false)
{
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = true;
    }
}

Player::~Player() 
{
    delete this->socket;
}

std::string Player::recv() 
{
    return this->socket->receive(); 
}

void Player::send(std::string msg) 
{
    this->socket->send(msg);
}

void Player::sendDeck() 
{
    std::string msg = DECK;
    for (int i = 0; i < DECK_SIZE; i++){
        msg.append(" ");
        if (this->deck[i]){
            msg.append(std::to_string(0));
        }
        else {
            msg.append(std::to_string(1));
        }
    }
    this->send(msg);
}

int Player::getPoints() 
{
    return this->points;
}

void Player::sendError(int code) 
{
    std::string error = ERROR;
    error.append(" ");
    error.append(std::to_string(code));
    this->socket->send(error);
}

void Player::playCard(int card){
    if (this->deck[card]){
        this->setCurrentCard(card);
        this->deck[card] = false;
        switch (card)
        {
        case 6: // General
            general = true;
            break;

        case 2: // Espion
            spy = true;
            break;
        
        default:
            break;
        }
        this->send(RECEIVE);
    }
    else {
        this->sendError(BAD_CARD);
    }
}

void Player::reset() 
{
    useGeneral();
    spy = false;
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = true;
    }

    points = 0;
}

void Player::useGeneral() 
{
    general = false;
}