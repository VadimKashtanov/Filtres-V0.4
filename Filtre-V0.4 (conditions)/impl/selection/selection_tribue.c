#include "selection.h"

Mdl_t * tribues(
    Mdl_t * depart, Env_t env,
    uint tribues, uint T_tribues,
    uint n, uint K, uint T_evolution)
{
    Mdl_t * mdl[tribues];
    FOR(0, i, tribues) {
        mdl[i] = copier_mdl(depart);
        mixer(depart, mdl[i], env, 0);
    }
    //
    //UNE_COURBE(_pred_)

    float _gain[tribues], _pred[tribues];
    uint rang[tribues];

    float _max=0;
    uint max=0;

    for (uint t=0; t < T_tribues; t++) {
        //printf("==========================\n");
        _max=0;
        max=0;
        //printf("%i|", t);        
        FOR(0, i, tribues) {
            mdl[i] = evolution(mdl[i], n, K, T_evolution, env);
            gain(mdl[i], _gain+i, _pred+i, env.l);
            if (_max < _pred[i]) {_max=_pred[i]; max=i;};  
        }
        if (t != T_tribues)
            FOR(0, i, tribues)
                if (i != max)
                    mixer(mdl[max], mdl[i], env, 0);     
        //
        //SUIVIE_COURBE(_pred_, _max);
    }
    //
    //PLUMER_LA_COURBE(_pred_)
    //LIBERER_LA_COURBE(score)
    for (uint i=0; i < tribues; i++) if (i!=max) liberer_mdl(mdl[i]);
    
    //
    return mdl[max];
}
