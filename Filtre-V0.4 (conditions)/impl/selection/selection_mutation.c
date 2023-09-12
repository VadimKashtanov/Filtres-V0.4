#include "selection.h"

void mixer(Mdl_t * G, Mdl_t * P, Env_t env, uint mode) {
	/*
		Mode : Quoi muter.
			0 : tout
			1 : que cst
			2 : que p
	*/
	float GG = env.GG;
	float GP = env.GP;
	float PG = (1.00-GG);
	float PP = (1.00-GP);
	float MUTP_cst     = env.MUTP_cst;
	float MUTP_p       = env.MUTP_p;
	float MUTP_ema_int = env.MUTP_ema_int;
	float MUTP_depuis  = env.MUTP_depuis;

	assert(GG == 1.0);	//pour l'instant

	assert(G->constes == P->constes);
	assert(G->poids == P->poids);

	float tmp;
	uint c;

	//	Mixage des Constantes
	if (mode == 0 || mode == 1) {
		FOR(0, i, G->C) {
			if (G->type[i] < 2) {
				FOR(0, j, G->y[i]) {
					uint depart = G->conste_depart[i] + j*CONSTES_FLTR(G->n[i]);
					//if (rnd() < PG) memcpy(G->conste + depart, P->conste + depart, sizeof(float) * CONSTES_FLTR(G->n[i]));
					if (rnd() < GP) memcpy(P->conste + depart, G->conste + depart, sizeof(float) * CONSTES_FLTR(G->n[i]));
					//
					if (rnd() < MUTP_cst) {
#define COEF 3.0
						float s=2*(rnd()-.5);
						float _max=s+COEF*G->conste[depart + 0], _min=s+COEF*G->conste[depart + 0];
						FOR(0, k, CONSTES_FLTR(G->n[i])) {
							s = s + 0.5*(rnd()-0.5);
							if ((s+COEF*G->conste[depart + k]) > _max) _max = s+COEF*G->conste[depart + k];
							if ((s+COEF*G->conste[depart + k]) < _min) _min = s+COEF*G->conste[depart + k];
							P->conste[depart + k] = s+COEF*G->conste[depart + k];
						}
						FOR(0, k, CONSTES_FLTR(G->n[i])) {
							P->conste[depart + k] -= _min;
							P->conste[depart + k] /= (_max-_min);
						}
					};
				};
			};
		}
	}

	//	Mixage des poids
	if (mode == 0 || mode == 2) {
		for (uint i=0; i < G->poids; i++) {
			tmp = G->poid[i];
			if (rnd() < PG) G->poid[i] = P->poid[i];
			if (rnd() < GP) P->poid[i] = tmp;

			//if (rnd() < MUTP_p) P->poid[i] = poid_rnd();
		};
		FOR(0, c, G->C) {
			if (G->type[c] == 2) {
				FOR(0, k, POIDS_NEU(G->n[c])*G->y[c]) {
					if (rnd() < MUTP_p) P->poid[G->poid_depart[c]+k] = poid_neu_rnd();
				}
			} else if (G->type[c] == 3) {
				FOR(0, k, POIDS_COND(G->n[c])*G->y[c]) {
					if (rnd() < MUTP_p) P->poid[G->poid_depart[c]+k] = poid_cond_rnd();
				}
			}        
		}
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
		for (uint i=1; i < G->C; i++) {
			if (G->type[i] == 2 || G->type[i] == 3) {
				for (uint j=0; j < G->y[i]; j++) {
					for (uint k=0; k < G->n[i]; k++) {
						c = G->neu_depuis[i][j][k];
						if (rnd() < PG) G->neu_depuis[i][j][k] = P->neu_depuis[i][j][k];
						if (rnd() < GP) P->neu_depuis[i][j][k] = c;

						if (rnd() < MUTP_depuis) P->neu_depuis[i][j][k] = rand() % G->y[i-1];
					};
				};
			} else if (G->type[i] == 1) {
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


Mdl_t * etendre_a(Mdl_t * depuis, uint C, uint *y, uint *n, uint *type) {
	/*assert(depuis->C <= C);
	FOR(0, i, depuis->C) {
		assert();
	}
	//
	//  pour le rnd des poids, il faut moyenner le filtre normal
	//  a un filtre en s:=(s+random()-.5) afin que ca fasse une meilleur tendance
	//
	//
	Mdl_t * ret = cree_mdl(C, y, n, type);
	memcpy();*/
	return 0;
};





