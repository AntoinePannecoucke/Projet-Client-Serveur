#include "Socket.h"
#include "Player.h"
#include "Table.h"
#include "IA.h"
#include <iostream>
#include <vector>
#include <thread>

bool checkRecv(std::string, std::vector<std::string>);
void game(Player*);
void playRound(Player*, IA*, std::string&, int&);

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
        std::thread game_thread (game, p_one);
        game_thread.detach();
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
    IA* ia;
    int wait_rounds = 0;
    while (player->getPoints() < 4){
        std::string request = player->recv();
        std::string response;
        
        switch (str2int(request.substr(0, 4).c_str()))
        {
        case str2int(NEXT):
            if (player->getPoints() < 4 && ia->getPoints() < 4){
                if (player->getSpy()){
                    ia->playCard();
                    std::string msg = CARD;
                    msg.append(" ");
                    msg.append(std::to_string(ia->getCurrentCard()));
                    player->send(msg);
                    player->useSpy();
                }
                else {
                    player->send(BEGIN);
                }
            }
            else {
                player->reset();
                ia->reset();
                player->send(MENU);
            }
            break;

        case str2int(ROUND):
            ia->playCard();
            playRound(player, ia, response, wait_rounds);
            break;
        
        case str2int(ROUND_SPY):
            playRound(player, ia, response, wait_rounds);
            break;

        case str2int(SOLO):
            ia = new IA();
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
                if (choice >= '0' && choice <= '7'){
                    player->playCard(choice - '0');
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

void playRound(Player* player, IA* ia, std::string& response, int& wait_rounds){
    response = ROUND;
    int result;
            

    if (player->getGeneral() && !ia->getGeneral()){ // [1]
        result = table[1][player->getCurrentCard()][ia->getCurrentCard()];
        player->useGeneral();
    }
    else if (ia->getGeneral() && !player->getGeneral()){ // [2]
        result = table[2][player->getCurrentCard()][ia->getCurrentCard()];
        ia->useGeneral();
    }
    else { // [0]
        result = table[0][player->getCurrentCard()][ia->getCurrentCard()];
    }

    response.append(" ");

    switch (result)
    {
    case -2: // client gagne la partie
        player->addPoints(4);
        response.append("1");
        break;

    case -1: // ia gagne la partie
        ia->addPoints(4);
        response.append("2");
        break;

    case 0: // mis en attente
        wait_rounds++;
        response.append("0");
        break;

    case 1: // client gagne round
        if (wait_rounds > 0){
            player->addPoints(wait_rounds + 1);
            wait_rounds = 0;
        }
        else {
            player->addPoints(1);
        }
            response.append("1");
            break;

    case 2: // ia gagne round
        if (wait_rounds > 0){
            ia->addPoints(wait_rounds + 1);
            wait_rounds = 0;
        }
        else {
            ia->addPoints(1);
        }
        response.append("2");
        break;
    }
            
    response.append(" ");

    response.append(std::to_string(player->getPoints()));
    response.append(" ");
    response.append(std::to_string(ia->getPoints()));

    response.append(" ");

    response.append(std::to_string(player->getCurrentCard()));
    response.append(" ");
    response.append(std::to_string(ia->getCurrentCard()));
    player->send(response);
}
            