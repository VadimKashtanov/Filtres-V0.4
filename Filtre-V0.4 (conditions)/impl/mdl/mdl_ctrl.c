#include "mdl.h"

uint meme_mdl(Mdl_t * A, Mdl_t * B) {
    for (uint i=0; i < sizeof(Mdl_t); i++) {
        if (((char*)A)[i] != ((char*)B)[i]) return 0;
    };
    return 1;   //1 == memes models
};

#define HASH(v) ((123*((v) % (1<<16)) + 321)  % 1000)

uint hash_mdl(Mdl_t * mdl) {
    uint h = 0;
    for (uint i=0; i < sizeof(Mdl_t); i++) {
        h = HASH(h + ((char*)mdl)[i]);
    };
    return h;
};
