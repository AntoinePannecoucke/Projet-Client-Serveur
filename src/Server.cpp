#include "Socket.h"
#include "Player.h"
#include "Table.h"
#include <iostream>
#include <vector>
#include <thread>

bool checkRecv(std::string, std::vector<std::string>);
void game(Player*);

constexpr unsigned int str2int(const char* str, int h = 0){
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket* player_one = new stdsock::TransportSocket();
        

        player_one->setSock(connection.accept()); //Waiting connection of player 1
        Player* p_one = new Player(player_one);

        game(p_one);
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

void game(Player* player){
    while (player->getPoints() < 4){
        std::string request = player->recv();
        std::string response;

        switch (str2int(request.substr(0, 4).c_str()))
        {
        case str2int(NEXT):
            player->send(BEGIN);
            break;

        case str2int(ROUND):
            response = ROUND;
            response.append(" 0 1 0 ");
            response.append(std::to_string(player->getCurrentCard()));
            response.append(" 4");
            player->send(response);
            break;
        

        case str2int(SOLO):
            //TODO make IA
            player->send(BEGIN);
            break;
        case str2int(DECK):
            player->sendDeck();
            break;
        
        case str2int(HELLO):
            player->send(MENU);
            break;

        case str2int(QUIT):
            delete player;
            return;
            break;

        case str2int(CARD):
            if (request.length() == 6){
                char choice = request.at(5);
                if (choice >= '1' && choice <= '8'){
                    player->setCurrentCard(choice - '0');
                    player->send(RECEIVE);
                }
                else {
                    player->sendError(BAD_CARD);
                }
            }
            else {
                player->sendError(BAD_RESPONSE);
            }
            break;

        default:
            delete player;
            return;
            break;
        }
        
    }
}