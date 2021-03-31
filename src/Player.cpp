#include "Player.h"

Player::Player(stdsock::TransportSocket* sock): socket(sock), points(0)
{

}

Player::~Player() 
{
    
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
        msg.append(std::to_string(0));
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
