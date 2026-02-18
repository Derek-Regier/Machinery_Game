#ifndef HEALTHBAR_H
#define HEALTHBAR_H

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Healthbar;

int update_value(Healthbar *healthbar, int value);

#endif /* HEALTHBAR_H */