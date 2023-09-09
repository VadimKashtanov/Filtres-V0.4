#include "mdl.h"

uint meme_mdl(Mdl_t * A, Mdl_t * B) {

};

#define HASH(v) ((123*((v) % (1<<16)) + 321)  % 10000)

uint hash_mdl(Mdl_t * mdl) {
    uint h = 0;
    for (uint i=0; i < sizeof(Mdl_t); i+) {
        h = HASH(h + ((uint*)mdl)[i]);
    };
};
