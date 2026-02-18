#ifndef MODEL_H
#define MODEL_H

#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "item.h"

typedef struct {
    Player player;
    Enemy enemy[1]; //Placeholder, actual enemies will depend on gameplay
    Boss boss;
    Item item[1]; //Number of items will also depend on gameplay
} Model;


#endif