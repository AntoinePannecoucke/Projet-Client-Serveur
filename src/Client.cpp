#include "Socket.h"
#include <iostream>

const std::string card[] = {"0 - Musicien : \"Cette manche est annulee et mise en attente\"",
                            "1 - Princesse : \"Si votre adversaire a jouee son Prince vous remportez la partie\"",
                            "2 - Espion : \"A la prochaine manche, votre adversaire revele sa carte avant que vous choisissiez la votre\"",
                            "3 - Assasin : \"La valeur la plus faible l'emporte\"",
                            "4 - Ambassadeur : \"Si vous gagnez avec cette carte, cela compte pour 2 manches\"",
                            "5 - Magicien : \"Annule la capacité speciale de la carte jouee par l'adversaire\"",
                            "6 - General : \"Votre carte de la prochaine manche voit sa valeur augmentee de +2\"",
                            "7 - Prince : \"Vous gagnez la manche\""
                            };

void deckParse(std::string, bool[]);
void roundParse(std::string);

/**
 * @author Serhiy
 * * From https://stackoverflow.com/questions/16388510/evaluate-a-string-with-switch-in-c/16388594
 */
constexpr unsigned int str2int(const char* str, int h = 0){
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int main(){
    stdsock::TransportSocket transport;
    transport.connect("127.0.0.1") ;

    transport.send(HELLO);
    std::string server_response = transport.receive();

    while (true){
        switch (str2int(server_response.substr(0,4).c_str())){

        case str2int(CARD):
            puts("\nL'autre joueur à joué :");
            puts(card[server_response.at(5) - '0'].c_str());

            //Réception du deck et affichage
            transport.send(DECK);
            server_response = transport.receive();
            bool deck2[8];
            deckParse(server_response.substr(5,15), deck2);
            puts("\nVoici votre main :");
            for (int i = 0; i < 8; i++){
                if (deck2[i]){
                    puts(card[i].c_str());
                }
            }

            //Input de sélection de la carte à jouer
            int carte2;
            do {
                puts("\nChoisissez la carte que vous voulez jouez");
                std::cin >> carte2;
                std::string request = CARD;
                request.append(" ");
                request.append(std::to_string(carte2));
                transport.send(request);
                server_response = transport.receive();
            } while (strcmp(server_response.c_str(),RECEIVE) != 0);

            transport.send(ROUND_SPY);

            server_response = transport.receive();
            roundParse(server_response.substr(5, 9));

            //Demande la suite des événements
            transport.send(NEXT);
            break;

        // Show the menu
        case str2int(MENU):
            int mode;
            do {
            puts("Bienvenu dans cette version étudiante de BraveRats, veuillez choisir votre mode de jeu:");
            puts("1 - Jouer contre l'ordinateur");
            puts("2 - Jouer contre un autre joueur"); // TODO
            puts("3 - Quitter");
            std::cin >> mode;
            if (mode < 1 || mode > 3){
                puts("Veulliez entrer un choix présent dans le menu !");
            }
            } while (mode < 1 || mode > 3);
            switch (mode)
            {
            case 1:
                transport.send(SOLO);
                break;
            case 2:
                /* code */
                break;
            case 3:
                transport.send(QUIT);
                puts("Au revoir");
                return EXIT_SUCCESS;
                break;
            }
            break;

        // Game
        case str2int(BEGIN):
            //Réception du deck et affichage
            transport.send(DECK);
            server_response = transport.receive();
            bool deck[8];
            deckParse(server_response.substr(5,15), deck);
            puts("\nVoici votre main :");
            for (int i = 0; i < 8; i++){
                if (deck[i]){
                    puts(card[i].c_str());
                }
            }

            //Input de sélection de la carte à jouer
            int carte;
            do {
                puts("\nChoisissez la carte que vous voulez jouez");
                std::cin >> carte;
                std::string request = CARD;
                request.append(" ");
                request.append(std::to_string(carte));
                transport.send(request);
                server_response = transport.receive();
            } while (strcmp(server_response.c_str(),RECEIVE) != 0);

            //Récupère les informations du tour et les affiche
            transport.send(ROUND);
            server_response = transport.receive();
            roundParse(server_response.substr(5, 9));

            //Demande la suite des événements
            transport.send(NEXT);
            break;
        }

        server_response = transport.receive();
    }
        
}

/**
 * @author Antoine Pannecoucke
 * @param string with 8 digits separated by ',' like example "1,0,0,1,0,1,0,1"
 */
void deckParse(std::string deck, bool cards[]){
    for (int i = 0; i < 15; i+=2){
        if (deck.at(i) == '0'){
            cards[i/2] = true;
        }
        else if (deck.at(i) == '1'){
            cards[i/2] = false;
        }
    }
}

void roundParse(std::string round){
    std::string played = "\n";
    played.append(card[round.at(6) - '0'].substr(4));
    played.append(" contre ");
    played.append(card[round.at(8) - '0'].substr(4));
    puts(played.c_str());

    if (round.at(0) == '0'){
        puts("\nTour en attente");
    }
    else if (round.at(0) == '1'){
        puts("\nTour gagné");
    }
    else if (round.at(0) == '2'){
        puts("\nTour en perdu");
    }

    std::string score = "\nVous : ";
    score.append(round.substr(2,1));
    score.append(" - ");
    score.append(round.substr(4,1));
    score.append(" : L'ordinateur");
    puts(score.c_str());

    puts(round.c_str());
    if (round.at(2) >= '4'){
        puts("\nPartie gagnée");
    }
    else if (round.at(4) >= '4'){
        puts("\nPartie perdue");
    }
}