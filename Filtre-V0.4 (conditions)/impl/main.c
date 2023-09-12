#include "selection.h"

uint C      = 2;
uint type[] = {0,3};
//
uint y[]    = {2,1};
uint n[]    = {8,2};

//
//selection mutation.c !!!!!!!!
//

Env_t env = {
	//  % de ce que va du Gagant au Gagant/Perdant
	.GG = 1.00,
	.GP = 1.00,
	
	//  Le gagant ne muteras jamais
	.MUTP_cst     =1.0,
	.MUTP_p       =.50,
	.MUTP_ema_int =.00,
	.MUTP_depuis  =1.00,
	
	//  % Modes de selection
	.MODE0=1.0, .MODE1=0.0, .MODE2=0.0,

	//  Longeure
	.l=1
};

void score(Mdl_t * mdl) {
	float _a, _b;
	gain(mdl, &_a, &_b, env.l);
	printf("%f\n", _b);
};

int main() {
	srand(0);
	charger_les_prixs();

	Mdl_t * mdl = cree_mdl(C, y, n, type);
	//plume_mdl(mdl);
	score(mdl);

	//
	mdl = evolution(mdl, 1, 3, 5, env);
	score(mdl);

	/*env.MUTP_cst = .20;
	env.MUTP_depuis = .05;
	env.MUTP_p = .05;
	mdl = evolution(mdl, 2, 10, 100, env);
	score(mdl);

	env.MUTP_cst = .10;
	env.MUTP_depuis = .00;
	env.MUTP_p = .01;
	mdl = evolution(mdl, 2, 10, 100, env);
	score(mdl);

	comportement(mdl);
	plume_mdl(mdl);*/
};

























