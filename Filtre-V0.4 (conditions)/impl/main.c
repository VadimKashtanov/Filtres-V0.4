#include "selection.h"

uint C      = 4; 
uint type[] = {0, 1,2,3};
//
uint y[]    = {10,4,3,1};
uint n[]    = { 6,4,3,2};

Env_t env = {
    //  % de ce que va du Gagant au Gagant/Perdant
    .GG = 1.00,
    .GP = 1.00,
    
    //  Le gagant ne muteras jamais
    .MUTP_cst     =.05,
    .MUTP_p       =.02,
    .MUTP_ema_int =.01,
    .MUTP_depuis  =.01,
    
    //  % Modes de selection
    .MODE0=0.6, .MODE1=0.3, .MODE2=0.1
};

int main() {
    srand(0);
    charger_les_prixs();

    Mdl_t * mdl = cree_mdl(C, y, n, type);
    //
    UNE_COURBE(score)

    uint n=3, K=7, T=100;
    float _gain, _pred;
    
    for (uint t=0; t < 100; t++) {
       mdl = evolution(mdl, n, K, T, env);
       gain(mdl, &_gain, &_pred);
       SUIVIE_COURBE(score, _pred);
    }
    //
    PLUMER_LA_COURBE(score)
    LIBERER_LA_COURBE(score)
    
    //
    ecrire_mdl(mdl, "mdl.bin");
};
