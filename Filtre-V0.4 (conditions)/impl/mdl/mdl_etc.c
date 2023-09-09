#include "mdl.h"

static float p_neu[] = {-2, -1, -0.5, 0, +0.5, +1, +2}; 
float poid_neu_rnd() {
    return p_neu[rand() % 7];
};



static float p_cond[] = {-1, -0.5, 0, +0.5, +1}; 
float poid_cond_rnd() {
    return p_cond[rand() % 5];
};

