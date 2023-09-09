#include "mdl.h"

Mdl_t * cree_mdl(uint C, uint * y, uint * n, uint * type) {
    Mdl_t * mdl = calloc(1, sizeof(Mdl_t));
    //
    
};

void liberer_mdl(Mdl_t * mdl) {
    free(mdl);
};

Mdl_t * copier_mdl(Mdl_t * mdl) {
    Mdl_t * ret = malloc(sizeof(Mdl_t));
    memcpy(ret, mdl, sizeof(Mdl_t));
    return ret;
};
