#include "IA.h"

/**
 * Constructor
 * */
IA::IA(): Player(nullptr)
{
}

/**
 * Destructor
 * */
IA::~IA()
{
}

/**
 * Joue une carte dans les cartes disponibles
 * @return void
 * */
void IA::playCard(){
    int cards[DECK_SIZE];
    int cpt = 0;
    for (int i = 0; i < DECK_SIZE; i++){
        if (deck[i]){
            cards[cpt++] = i;
        }
    }
    int choose = cards[rand() % cpt];
    this->setCurrentCard(choose);
    deck[choose] = false;
}