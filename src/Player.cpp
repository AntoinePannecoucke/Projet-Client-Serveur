#include "Player.h"

/**
 * Constructor
 * @param stdsock::TransportSocket* socket
 * */
Player::Player(stdsock::TransportSocket* sock): socket(sock), points(0), general(false), spy(false)
{
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = true;
    }
}

/**
 * Destructor
 * */
Player::~Player() 
{
    delete this->socket;
}

/**
 * Return the std::string received by the socket
 * @return std::string
 * */
std::string Player::recv() 
{
    return this->socket->receive(); 
}


/**
 * @param std::string message
 * @return void
 * */
void Player::send(std::string msg) 
{
    this->socket->send(msg);
}


/**
 * @return void
 * */
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

/**
 * Return player's points
 * @return int
 * */
int Player::getPoints() 
{
    return this->points;
}

/**
 * Send the error passed in parameter
 * @param int code
 * @return void
 * */
void Player::sendError(int code) 
{
    std::string error = ERROR;
    error.append(" ");
    error.append(std::to_string(code));
    this->socket->send(error);
}

/**
 * Check card and send RECEIVE if it's ok or BAD_CARD if there is an error
 * @param int card
 * @return void
 * */
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


/**
 * Reset all the player for a new game
 * @return void
 * */
void Player::reset() 
{
    useGeneral();
    useSpy();
    for (int i = 0; i < DECK_SIZE; i++){
        deck[i] = true;
    }

    points = 0;
}

/**
 * Set general at false
 * @return void
 * */
void Player::useGeneral() 
{
    general = false;
}


/**
 * Set spy at false
 * @return void
 * */
void Player::useSpy() 
{
    spy = false;
}