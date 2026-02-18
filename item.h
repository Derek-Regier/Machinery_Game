#ifndef ITEM_H
#define ITEM_H

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Item;

int get_value(Item *item);

#endif /* ITEM_H */

