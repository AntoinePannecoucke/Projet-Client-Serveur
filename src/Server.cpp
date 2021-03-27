#include "Socket.h"

void sendError(stdsock::TransportSocket& socket, int code){
    std::string error = ERROR;
    error.append(" ");
    error.append(std::to_string(code));
    socket.send(error);
}

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket player_one, player_two;
        player_one.setSock(connection.accept()); //Waiting connection of player 1
        player_one.receive(); //Waiting for HELLO
        player_one.send(WAIT); //Send WAIT
        std::string response = player_one.receive();
        if (response.compare(RECEIVE)){ //Waiting for RECEIVE
            sendError(player_one, BAD_RESPONSE);
        }
        player_two.setSock(connection.accept()); //Waiting connection of player 2
        player_two.receive(); //Waiting for HELLO

        player_two.send(DECK);
        player_one.send(DECK);


    }
    return 0;
}

