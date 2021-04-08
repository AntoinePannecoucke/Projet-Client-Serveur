#include "Socket.h"
#include "Player.h"
#include "Table.h"
#include "IA.h"
#include <iostream>
#include <vector>
#include <thread>

void game(Player*);
void playRound(Player*, IA*, std::string&, int&);

/**
 * Transform char* to int for use into a switch.
 * @author Serhiy, https://stackoverflow.com/questions/16388510/evaluate-a-string-with-switch-in-c/16388594 
 * @param const char* str
 * @param int h = 0
 * @return constexpr unsigned int
 */
constexpr unsigned int str2int(const char* str, int h = 0){
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(){
    stdsock::ConnectionSocket connection;
    connection.openConnection();
    while(true){
        stdsock::TransportSocket* player_one = new stdsock::TransportSocket();
        
        player_one->setSock(connection.accept());
        Player* p_one = new Player(player_one);
        std::thread game_thread (game, p_one);
        game_thread.detach();
    }
    return 0;
}

/**
 * Déroulé d'une partie
 * @param Player* player
 * @return void
 * */
void game(Player* player){
    IA* ia;
    int wait_rounds = 0;
    while (player->getPoints() < 4){
        std::string request = player->recv();
        std::string response;
        
        switch (str2int(request.substr(0, 4).c_str()))
        {

        // Envoi la suite des événements au client
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
        
        // Gestion du tour
        case str2int(ROUND):
            ia->playCard();
            playRound(player, ia, response, wait_rounds);
            break;

        // Gestion du tour avec l'espion
        case str2int(ROUND_SPY):
            playRound(player, ia, response, wait_rounds);
            break;

        // Gestion du début d'un jeu en solo
        case str2int(SOLO):
            ia = new IA();
            player->send(BEGIN);
            break;

        // Envoi du deck du joueur
        case str2int(DECK):
            player->sendDeck();
            break;
        
        // Envoi MENU au client
        case str2int(HELLO):
            player->send(MENU);
            break;

        // Le client quitte
        case str2int(QUIT):
            delete player;
            return;
            break;

        // Gestion de la réception d'une carte
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

/**
 * Déroulé d'un tour de jeu en solo
 * @param Player* player
 * @param IA* ia
 * @param std::string& response
 * @param int& wait_rounds
 * @return void
 * */
void playRound(Player* player, IA* ia, std::string& response, int& wait_rounds){
    response = ROUND;
    int result;
            

    if (player->getGeneral() && !ia->getGeneral()){ // Player à utiliser son général
        result = table[1][player->getCurrentCard()][ia->getCurrentCard()];
        player->useGeneral();
    }
    else if (ia->getGeneral() && !player->getGeneral()){ // IA à utiliser son général
        result = table[2][player->getCurrentCard()][ia->getCurrentCard()];
        ia->useGeneral();
    }
    else { // Les deux ont utilisé leur général ou aucun des deux
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

    // Send ROUND avec les paramètres      
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
            