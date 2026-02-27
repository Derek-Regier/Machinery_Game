#ifndef HEALTH_H
#define HEALTH_H

typedef struct {
    unsigned int x, y;
    int h,w;
    int value;
} Healthbar;

int update_value(Healthbar *healthbar, int value);

#endif /* HEALTH_H */
