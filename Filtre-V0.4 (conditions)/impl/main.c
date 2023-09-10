#include "selection.h"

uint C      = 2;
uint type[] = {0,3};
//
uint y[]    = {3,1};
uint n[]    = {5,3};

Env_t env_local = {
    //  % de ce que va du Gagant au Gagant/Perdant
    .GG = 1.00,
    .GP = 1.00,
    
    //  Le gagant ne muteras jamais
    .MUTP_cst     =.05,
    .MUTP_p       =.05,
    .MUTP_ema_int =.00,
    .MUTP_depuis  =.05,
    
    //  % Modes de selection
    .MODE0=0.0, .MODE1=1.0, .MODE2=0.0,

    //  Longeure
    .l=5
};

Env_t env_g = {
    //  % de ce que va du Gagant au Gagant/Perdant
    .GG = 1.00,
    .GP = 1.00,
    
    //  Le gagant ne muteras jamais
    .MUTP_cst     =.10,
    .MUTP_p       =.05,
    .MUTP_ema_int =.02,
    .MUTP_depuis  =.02,
    
    //  % Modes de selection
    .MODE0=0.6, .MODE1=0.3, .MODE2=0.1,

    //  Longeure
    .l=5
};

int main0() {
    srand(0);
    charger_les_prixs();
    
    //
    //ecrire_mdl(, "mdl.bin");
};



int main() {
    srand(0);
    charger_les_prixs();

    Mdl_t * mdl = cree_mdl(C, y, n, type);
    
    mdl = evolution(mdl, 1, 10, 100, env_local);

    comportement(mdl);
    plume_mdl(mdl);
};

























