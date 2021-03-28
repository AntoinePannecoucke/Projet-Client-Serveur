#include "Socket.h"
#include "Player.h"
#include <iostream>
#include <vector>

void sendError(stdsock::TransportSocket&, int);
bool checkRecv(std::string, std::vector<std::string>);

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket player_one, player_two;
        player_one.setSock(connection.accept()); //Waiting connection of player 1
        //Player* p_one = new Player(player_one);
        player_one.receive();
        player_one.send(WAIT);
        //p_one->recv();
        //p_one->send(WAIT);
        
        std::string response = player_one.receive();
        std::vector<std::string> valid;
        valid.push_back(RECEIVE);
        checkRecv(response, valid);
        /*if (response.compare(RECEIVE)){ //Waiting for RECEIVE
            sendError(player_one, BAD_RESPONSE);
        }
        player_two.setSock(connection.accept()); //Waiting connection of player 2
        Player* p_two = new Player(player_two);

        p_two->recv();

        p_one->sendDeck();
        p_two->sendDeck();
*/

    }
    return 0;
}

void sendError(stdsock::TransportSocket& socket, int code){
    std::string error = ERROR;
    error.append(" ");
    error.append(std::to_string(code));
    socket.send(error);
}

bool checkRecv(std::string data, std::vector<std::string> valid){
    for (std::string str : valid){
        if (strcmp(str.c_str(), data.c_str()) == 0){
            return true;
        }
    }
    return false;
}