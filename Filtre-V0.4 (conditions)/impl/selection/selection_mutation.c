#include "selection.h"

void mixer(Mdl_t * G, Mdl_t * P, Env_t env, uint mode) {
    /*
        Mode : Quoi muter.
            0 : tout
            1 : que cst
            2 : que p
    */

	float PG = (1.00-env.GG);
	float PP = (1.00-env.GP);
	float MUTP_cst     = env.MUTP_cst;
	float MUTP_p       = env.MUTP_p;
	float MUTP_ema_int = env.MUTP_ema_int;
	float MUTP_depuis  = env.MUTP_depuis;

	assert(G->constes == P->constes);
	assert(G->poids == P->poids);

	float tmp;
	uint c;

	//	Mixage des Constantes
    if (mode == 0 || mode == 1) {
	    for (uint i=0; i < G->constes; i++) {
		    tmp = G->conste[i];
		    if (rnd() < PG) G->conste[i] = P->conste[i];
		    if (rnd() < GP) P->conste[i] = tmp;

		    if (rnd() < MUTP_cst) P->conste[i] = (3*G->conste[i] + rnd())/4;
	    };
    

	    float _maxG, _maxP, _minG, _minP;
	    float _P, _G;
	    uint p;
	    for (uint c=0; c < G->C; c++) {
		    if (G->type[c] != 2) {
			    for (uint j=0; j < G->y[c]; j++) {
				    _G = G->conste[G->conste_depart[c] + j*G->n[c] + 0];
				    _P = P->conste[G->conste_depart[c] + j*G->n[c] + 0];
				    _maxG = _G;
				    _maxP = _P;
				    _minG = _G;
				    _minP = _P;
				    for (uint k=1; k < G->n[c]; k++) {
					    _G = G->conste[G->conste_depart[c] + j*G->n[c] + k];
					    _P = P->conste[G->conste_depart[c] + j*G->n[c] + k];
					    if (_G >= _maxG) _maxG = _G;
					    if (_G <= _minG) _minG = _G;
					    if (_P >= _maxP) _maxP = _P;
					    if (_P <= _minP) _minP = _P;
				    }
				    //
				    if (_minG == _maxG) ERR("%f == %f\n", _minG, _maxG);
				    if (_maxP == _minP) ERR("%f == %f\n", _minP, _maxP);
				    //
				    for (uint k=0; k < G->n[c]; k++) {
					    p = G->conste_depart[c] + j*G->n[c] + k;
					    G->conste[p] = (G->conste[p]-_minG)/(_maxG-_minG);
					    P->conste[p] = (P->conste[p]-_minP)/(_maxP-_minP);
				    }
			    }
		    }
	    }
    }

	//	Mixage des poids
    if (mode == 0 || mode == 2) {
	    for (uint i=0; i < G->poids; i++) {
		    tmp = G->poid[i];
		    if (rnd() < PG) G->poid[i] = P->poid[i];
		    if (rnd() < GP) P->poid[i] = tmp;

		    if (rnd() < MUTP_p) P->poid[i] = poid_rnd();
	    };
    }
	
	//	Mixage des Ema & Intervalles
    if (mode == 0) {
	    for (uint i=0; i < G->y[0]; i++) {
		    //	Moyennes mouvantes
		    c = G->ema[i];
		    if (rnd() < PG) G->ema[i] = P->ema[i];
		    if (rnd() < GP) P->ema[i] = c;

		    if (rnd() < MUTP_ema_int) P->ema[i] = rand() % NB_DIFF_EMA;

		    //	Intervalles
		    c = G->intervalles[i];
		    if (rnd() < PG) G->intervalles[i] = P->intervalles[i];
		    if (rnd() < GP) P->intervalles[i] = c;

		    if (rnd() < MUTP_ema_int) P->intervalles[i] = rand() % INTERVALLES;
	    }

	    //	Mixage des `neu_depuis` & `flt_depuis`
	    for (uint i=0; i < G->C; i++) {
		    if (G->neu_depuis[i] != 0) {
			    for (uint j=0; j < G->y[i]; j++) {
				    for (uint k=0; k < G->n[i]; k++) {
					    c = G->neu_depuis[i][j][k];
					    if (rnd() < PG) G->neu_depuis[i][j][k] = P->neu_depuis[i][j][k];
					    if (rnd() < GP) P->neu_depuis[i][j][k] = c;

					    if (rnd() < MUTP_depuis) P->neu_depuis[i][j][k] = rand() % G->y[i-1];
				    };
			    };
		    } else if (G->flt_depuis[i] != 0) {
			    for (uint j=0; j < G->y[i]; j++) {
				    c = G->flt_depuis[i][j];
				    if (rnd() < PG) G->flt_depuis[i][j] = P->flt_depuis[i][j];
				    if (rnd() < GP) P->flt_depuis[i][j] = c;

				    if (rnd() < MUTP_depuis) P->flt_depuis[i][j] = rand() % (G->y[i-1] - G->n[i]);
			    }
		    };
	    };
    }
};
