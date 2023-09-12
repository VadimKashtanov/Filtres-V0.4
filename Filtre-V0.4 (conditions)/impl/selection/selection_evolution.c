#include "selection.h"

Mdl_t * evolution(Mdl_t * depart, uint n, uint K, uint T, Env_t env) {
    assert(n > 0);
    assert(K > 0);
    assert(env.GG <= 1.00);
    assert(env.GP <= 1.00);
    assert(env.MODE0 + env.MODE1 + env.MODE2 == 1.00);
    //
    assert(env.MUTP_cst <= 1.00);
    assert(env.MUTP_p <= 1.00);
    assert(env.MUTP_ema_int <= 1.00);
    assert(env.MUTP_depuis <= 1.00);
    //
    assert(env.l >= 1);

    //
    uint N = (1+K)*n;
	Mdl_t * mdl[N];
	for (uint i=0; i < N; i++) {
        mdl[i] = copier_mdl(depart);
        mixer(depart, mdl[i], env, 0);    
    }			
	//	Les points de score
	float gains[N]; 		uint rang_gains[N];	//gain exacte depuis le depart avec un pret
	float prediction[N]; 	uint rang_prediction[N];//% reussite de prediction de tendance

	//	Points Finaux et 
	uint points[N];
	uint cintasat[N];	//#0, #1, #...  le 0-eme (1er) correspond a quel mdl, le 1-er ...

	/*printf("==============================================================\n");
	printf("==============================================================\n");
	printf("==============================================================\n");
	printf("==============================================================\n");
	printf("==============================================================\n");*/
	//
	INIT_CHRONO()
	//
	for (uint t=0; t < T; t++) {
		DEPART_CHRONO()
		//	Scores
		for (uint i=0; i < N; i++) {
			gain(mdl[i], gains+i, prediction+i, env.l);
			rang_gains[i] = i;
			rang_prediction[i] = i;
			points[i] = 0;
			cintasat[i] = i;
		//	printf("%f,", prediction[i]);
		}
		//printf("\n");

		//	Sommes des Points
		//printf("::::");
		for (uint i=0; i < N; i++) {
		//	printf("%i:%.3g,  ", rang_prediction[i], prediction[rang_prediction[i]]);
		}
		//printf(" <<< \n");

		//	Cintasar `gains` & `predictions`
		uint c; float tmp;
		for (uint i=0; i < N; i++) {
			for (uint j=i+1; j < N; j++) {
				if (gains[rang_gains[i]] < gains[rang_gains[j]]) {
					c = rang_gains[i];
					rang_gains[i] = rang_gains[j];
					rang_gains[j] = c;
				}

				if (prediction[rang_prediction[i]] < prediction[rang_prediction[j]]) {
					c = rang_prediction[i];
					rang_prediction[i] = rang_prediction[j];
					rang_prediction[j] = c;
				}
			}
		}

		//	Sommes des Points
		for (uint i=0; i < N; i++) {
			points[rang_gains[i]] += 0*(N-i);
			points[rang_prediction[i]] += (N-i);
			//printf("%i:%.3g,  ", rang_prediction[i], prediction[rang_prediction[i]]);
		}
		//printf(" <<< \n");

		//	Hierarchie des Points
		for (uint i=0; i < N; i++) {
			for (uint j=i; j < N; j++) {
				if (points[cintasat[i]] < points[cintasat[j]]) {
					c = cintasat[i];
					cintasat[i] = cintasat[j];
					cintasat[j] = c;
				}
			}
		}
		//for (uint i=0; i < N; i++) printf("%i:%i, ", i, cintasat[i]);
		//printf("\n");

		//	Plumage d'avancement
#define TOUT_LES 10
		/*if (t % TOUT_LES == 0 || t==(T-1)) {
			float tmis = VALEUR_CHRONO()/(1+t);	//temp par iteration
			uint reste_sec = (uint)roundf(tmis*(T-t-1));	//cmb il rest d'iterations
			uint secondes = reste_sec % 60;
			uint mins = (reste_sec-secondes)/60;
			printf("il rest %f secondes\n", tmis*(T-t-1));
			//
			printf("%i/%i %.3g%%   [gain=%f, prediction=%f]   ~temps=%i mins %i secs\n",
				t+1, T, (float)(t+1)/(T)*100,
				gains[cintasat[0]], prediction[cintasat[0]],
				mins, secondes
			);
		}*/

		//	Si pas derniere iteration
		if (t != T-1) {
            uint mode = 0;
            //float r = rnd();
            //if (r <= env.MODE0) mode = 0;
            //else if (r <= (env.MODE0+env.MODE1)) mode = 1;
            //else if (r <= (env.MODE0+env.MODE1+env.MODE2)) mode = 2;
            
			//	Mixage
			printf("====================================\n\033[93m");
			plume_mdl(mdl[cintasat[0]]);
			printf("\033[0m");
			for (uint i=0; i < n; i++) {	//	n = N / (K+1)
				for (uint k=0; k < K; k++) {
					mixer(mdl[cintasat[i]], mdl[cintasat[n + i*K + k]], env, mode);
					plume_mdl(mdl[cintasat[n + i*K + k]]);
				}
			}
		}
	}
    for (uint i=1; i < N; i++) liberer_mdl(mdl[cintasat[i]]);
    return mdl[cintasat[0]];
};
