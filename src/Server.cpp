#include "Socket.h"
#include "Player.h"
#include "Table.h"
#include <iostream>
#include <vector>
#include <thread>

bool checkRecv(std::string, std::vector<std::string>);
void multiplayerGame(Player*, Player*);

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket* player_one = new stdsock::TransportSocket();
        stdsock::TransportSocket* player_two = new stdsock::TransportSocket();

        player_one->setSock(connection.accept()); //Waiting connection of player 1
        Player* p_one = new Player(player_one);
        p_one->recv();
        p_one->send(WAIT);
        p_one->recv();

        player_two->setSock(connection.accept()); //Waiting connection of player 2
        Player* p_two = new Player(player_two);
        p_two->recv();
        p_two->send(WAIT);
        p_two->recv();

        multiplayerGame(p_one, p_two);
    }
    return 0;
}

/*void sendError(stdsock::TransportSocket& socket, int code){
    std::string error = ERROR;
    error.append(" ");
    error.append(std::to_string(code));
    socket.send(error);
}
*/
bool checkRecv(std::string data, std::vector<std::string> valid){
    for (std::string str : valid){
        if (strcmp(str.c_str(), data.c_str()) == 0){
            return true;
        }
    }
    return false;
}

void multiplayerGame(Player* player_one, Player* player_two){
    /*while (player_one->getPoints() < 4 && player_two->getPoints() < 4){*/
        player_one->sendDeck();
        player_two->sendDeck();
/*
        std::string response_one, response_two;

        bool request_one_ok, request_two_ok;
        request_one_ok = request_two_ok = false;

        do {

            if (!request_one_ok){
                response_one = player_one->recv();
                request_one_ok = (strcmp(CARD, response_one.substr(0,4).c_str()) == 0);
                if (!request_one_ok){
                    player_one->sendError(BAD_RESPONSE);
                }
            }

            if (!request_two_ok){
                response_two = player_one->recv();
                request_one_ok = (strcmp(CARD, response_two.substr(0,4).c_str()) == 0);
                if (!request_two_ok){
                    player_two->sendError(BAD_RESPONSE);
                }
            }

        } while (!request_one_ok && !request_two_ok);
*/
        
    //}
}