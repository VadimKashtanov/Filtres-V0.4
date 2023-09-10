#include "selection.h"

uint C      = 5;
uint type[] = { 0, 3,2,3,3};
//
uint y[]    = {20,10,5,3,1};
uint n[]    = { 6, 2,3,2,2};

Env_t env_local = {
    //  % de ce que va du Gagant au Gagant/Perdant
    .GG = 1.00,
    .GP = 1.00,
    
    //  Le gagant ne muteras jamais
    .MUTP_cst     =.05,
    .MUTP_p       =.02,
    .MUTP_ema_int =.01,
    .MUTP_depuis  =.01,
    
    //  % Modes de selection
    .MODE0=0.6, .MODE1=0.3, .MODE2=0.1,

    //  Longeure
    .l=2
};

Env_t env_g = {
    //  % de ce que va du Gagant au Gagant/Perdant
    .GG = 1.00,
    .GP = 0.90,
    
    //  Le gagant ne muteras jamais
    .MUTP_cst     =.05,
    .MUTP_p       =.02,
    .MUTP_ema_int =.01,
    .MUTP_depuis  =.01,
    
    //  % Modes de selection
    .MODE0=0.6, .MODE1=0.3, .MODE2=0.1,

    //  Longeure
    .l=3
};

int main() {
    srand(0);
    charger_les_prixs();

    Mdl_t * mdl0 = cree_mdl(C, y, n, type);
    Mdl_t * mdl1 = cree_mdl(C, y, n, type);
    Mdl_t * mdl2 = cree_mdl(C, y, n, type);
    //
    UNE_COURBE(_pred_)
    UNE_COURBE(_gain_)

    uint n=3, K=7, T=10;
    float _gain[3], _pred[3];
    uint points[3];
    
    for (uint t=0; t < 5; t++) {
        printf("==========================\n");
        mdl0 = evolution(mdl0, n, K, T, env_local);
        mdl1 = evolution(mdl1, n, K, T, env_local);
        mdl2 = evolution(mdl2, n, K, T, env_local);
        
        //
        gain(mdl0, _gain+0, _pred+0, env_g.l);
        gain(mdl1, _gain+1, _pred+1, env_g.l);
        gain(mdl2, _gain+2, _pred+2, env_g.l);
        
        //
        if (MAX3(_pred[0], _pred[1], _pred[2]) == _pred[0]) {mixer(mdl0, mdl1, env_g, 0);mixer(mdl0, mdl2, env_g, 0);};
        if (MAX3(_pred[0], _pred[1], _pred[2]) == _pred[1]) {mixer(mdl1, mdl0, env_g, 0);mixer(mdl1, mdl2, env_g, 0);};
        if (MAX3(_pred[0], _pred[1], _pred[2]) == _pred[2]) {mixer(mdl2, mdl0, env_g, 0);mixer(mdl2, mdl1, env_g, 0);};

        printf(">>> %f %f %f\n", _pred[0], _pred[1], _pred[2]);
   
        //
        SUIVIE_COURBE(_gain_, MAX3(_gain[0], _gain[1], _gain[2]));
        SUIVIE_COURBE(_pred_, MAX3(_pred[0], _pred[1], _pred[2]));
    }
    //
    PLUMER_LA_COURBE(_gain_)
    PLUMER_LA_COURBE(_pred_)
    //LIBERER_LA_COURBE(score)
    
    //
    ecrire_mdl(mdl0, "mdl.bin");
};




