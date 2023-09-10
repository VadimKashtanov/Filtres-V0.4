#include "mdl.h"

typedef struct  {
    //  % de ce que va du Gagant au Gagant/Perdant
    uint GG;// = 1.00;
    uint GP;// = 1.00;
    
    //  Le gagant ne muteras jamais
    float MUTP_cst;//     =.05;
    float MUTP_p;//       =.02;
    float MUTP_ema_int;// =.01;
    float MUTP_depuis;//  =.01;
    
    //  % Modes de selection
    float MODE0/*=0.6*/, MODE1/*=0.3*/, MODE2/*=0.1*/;

    //  Longeure / Distance de predition
    uint l; //=2;
} Env_t;

void gain(Mdl_t * mdl, float * _gain, float * _prediction, uint l);

void mixer(Mdl_t * G, Mdl_t * P, Env_t env, uint mode);

/*  Selection isolee.
Parametres:
    n : nb elites/gagants
    K : nb clones par elite
    T cmb d'iteations
*/
Mdl_t * evolution(Mdl_t * depart, uint n, uint K, uint T, Env_t env);
