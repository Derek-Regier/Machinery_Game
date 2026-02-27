#include "health.h"

int update_value(Healthbar *healthbar, int value){
    healthbar->value += value;
    return healthbar->value;
}
