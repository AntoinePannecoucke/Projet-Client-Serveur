#ifndef __IA_H__
#define __IA_H__

#include "Player.h"

class IA : public Player
{
public:
    IA();
    ~IA();
    void playCard();
};

#endif // __IA_H__