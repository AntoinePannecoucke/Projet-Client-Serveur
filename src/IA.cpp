#include "IA.h"

IA::IA(): Player(nullptr)
{
}

IA::~IA()
{
}

void IA::playCard(){
    int cards[DECK_SIZE];
    int cpt = 0;
    for (int i = 0; i < DECK_SIZE; i++){
        if (deck[i]){
            cards[cpt++] = i;
            
        }
        else{
            puts(std::to_string(i).c_str());
        }
    }
    int choose = cards[rand() % cpt];
    this->setCurrentCard(choose);
    deck[choose] = false;
}